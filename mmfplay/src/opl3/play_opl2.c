
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mmfplay.h"
#include "sound.h"
#include "device.h"
#include "sequencer.h"
#include "opl3.h"

#define OSD_CPU_H
#include "ymf262.h"

static int opl2_init(void);
static void opl2_deinit(void);
static void opl2_update(void);

struct device dev_opl2 = {
	"opl2",
	"YMF262 (OPL2) output device",
	opl2_init,
	opl2_deinit,
	opl2_update
};

#define NUM_CHIPS (((SEQUENCER_CHANNELS - 1)/OPL2_VOICES) + 1)
#define OPL2_CHIP(c) ((c)/OPL2_VOICES)
#define OPL2_CHAN(c) ((c)%OPL2_VOICES)


/* YMF262 registers */

static int opl2_op[2][OPL2_VOICES] = {
	 { 0, 1, 2, 6, 7, 8,12,13,14,18,19,20,24,25,26,30,31,32 },
	 { 3, 4, 5, 9,10,11,15,16,17,21,22,23,27,28,29,33,34,35 }
};

static int opl2_reg[] = {
	0x000, 0x001, 0x002, 0x003,
	0x004, 0x005, 0x008, 0x009,
	0x00a, 0x00b, 0x00c, 0x00d,
	0x010, 0x011, 0x012, 0x013,
	0x014, 0x015, 0x100, 0x101,
	0x102, 0x103, 0x104, 0x105,
	0x108, 0x109, 0x10A, 0x10B,
	0x10c, 0x10d, 0x110, 0x111,
	0x112, 0x113, 0x114, 0x115
};

static int opl2_ch[OPL2_VOICES] = {
	0x000, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007, 0x008,
	0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108
};

/* For regbase 0x20, 0x40, 0x60, 0x80 and 0xe0 */
#define OPL2_REG_OP(chn,o,base) ((opl2_reg[opl2_op[o][chn]])+(base))

/* For regbase 0xa0, 0xb0 and 0xc0 */
#define OPL2_REG_CHN(chn,base) (opl2_ch[chn]+(base))

#define opl2_write_op(c,o,b,d) \
	_opl3_write(OPL2_CHIP(c),OPL2_REG_OP(OPL2_CHAN(c),o,b),ins->op[o].d)

#define opl2_write_chan(c,b,d) \
	_opl3_write(OPL2_CHIP(c),OPL2_REG_CHN(OPL2_CHAN(c),b),d)


static void set_note(int c, int n)
{
	int fnum, freq, block;

	block = n / 12;
	n = n - 24;

	freq = 66.0 * pow(2.0, 1.0*n/12.0);

	fnum = (freq << (20 - block)) / 49716;
	
	opl2_write_chan(c, OPL3_REG_CH_FREQ, fnum & 0xff);
	opl2_write_chan(c, OPL3_REG_CH_KEY_BLOCK,
		(1 << 5) | ((block & 0x07) << 2) | ((fnum & 0x300) >> 8));
	
}

static void set_ins(int c, int n, int v)
{
	int i;
	int is_drum;
	struct opl2_instrument *ins;

	/*printf("channel %d set instrument %d\n", c, n);*/

	is_drum = ((n & 0x80) == 0x80);
	n &= 0x7f;

	if (is_drum)
		ins = &opl2_drum[n];
	else
		ins = &opl2_ins[n];

	for (i = 0; i < 2; i++) {
		opl2_write_op(c, i, OPL3_REG_OP_FLG_MUL, flg_mul);
		opl2_write_op(c, i, OPL3_REG_OP_KSL_TL, ksl_tl);
		opl2_write_op(c, i, OPL3_REG_OP_AR_DR, ar_dr);
		opl2_write_op(c, i, OPL3_REG_OP_SL_RR, sl_rr);
		opl2_write_op(c, i, OPL3_REG_OP_WS, ws);
	}

	opl2_write_chan(c, OPL3_REG_FB_ALG, 0x30 | ins->fb_alg);

	if (is_drum)
		set_note(c, ins->dpitch ? ins->dpitch : 60);
}

static void stop_note(int c)
{
	opl2_write_chan(c, OPL3_REG_CH_KEY_BLOCK, 0);
}

static int opl2_init()
{
	int i;

	printf(" dev: OPL2 driver by claudio@helllabs.org\n");
	printf(" dev: YMF262 FM sound emulator by Jarek Burczynski\n");
	printf(" dev: initializing %d YMF262 chips\n", NUM_CHIPS);
	YMF262Init(NUM_CHIPS, 8000000, 22050);

	printf(" dev: Reset YMF262 ");
	for (i = 0; i < NUM_CHIPS; i++) {
		printf("#%d ", i);
		YMF262ResetChip(i);
		_opl3_write(i, 0x01, 0x20);	/* Enable waveform selection */
		_opl3_write(i, 0x104, 0x00);	/* Disable 4-op mode */
		_opl3_write(i, 0x105, 0x01);	/* Disable OPL3 mode */
	}
	printf("\n");

	return 0;
}

static void opl2_deinit()
{
	YMF262Shutdown();
}

static void opl2_update()
{
	int c, i;

	for (c = 0; c < SEQUENCER_CHANNELS; c++) {
		if (channel[c].newkey) {
			if (channel[c].note > 0)
				set_note(c, channel[c].note);
			set_ins(c, channel[c].ins, channel[c].vol);
			channel[c].newkey = 0;
		}
		if (channel[c].timer == 0) {
			stop_note(c);
		}
	}

	YMF262UpdateOne(0, snd_buffer, BUFFER_SIZE, 1);
	for (i = 1; i < NUM_CHIPS; i++) {
		YMF262UpdateOne(i, snd_buffer, BUFFER_SIZE, 0);
	}
}


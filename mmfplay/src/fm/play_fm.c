
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mmfplay.h"
#include "sound.h"
#include "device.h"
#include "sequencer.h"
#include "fm.h"

#define OSD_CPU_H
#include "ymf262.h"

static int fm_init(void);
static void fm_deinit(void);
static void fm_update(void);

struct device dev_fm = {
	"fm",
	"YMF262 (OPL3) output device",
	fm_init,
	fm_deinit,
	fm_update
};

#define NUM_CHIPS (((SEQUENCER_CHANNELS - 1)/OPL3_VOICES) + 1)
#define OPL3_CHIP(c) ((c)/OPL3_VOICES)
#define OPL3_CHAN(c) ((c)%OPL3_VOICES)

static int opl3_op_mode[SEQUENCER_CHANNELS];

#ifdef UNROLL

#define opl3_write_op(c,o,b,d)	fm_set_parameter(OPL3_CHIP(c),OPL3_CHAN(c),o,b,ins->op[o].d)
#define opl3_write_chan(c,b,d)	fm_set_parameter(OPL3_CHIP(c),OPL3_CHAN(c),0,b,d)

#else

/* YMF262 registers */

static int opl3_op[4][OPL3_VOICES] = {
	 { 0,  1,  2,  18, 19, 20 },
	 { 3,  4,  5,  21, 22, 23 },
	 { 6,  7,  8,  24, 25, 26 },
	 { 9, 10, 11,  27, 28, 29 }
};

static int opl3_reg[] = {
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

/* For regbase 0x20, 0x40, 0x60, 0x80 and 0xe0 */
#define OPL3_REG_OP(chn,o,base) ((opl3_reg[opl3_op[o][chn]])+(base))

static int opl3_chn[OPL3_VOICES] = {
	0x000, 0x001, 0x002, /*0x003, 0x004, 0x005, 0x006, 0x007, 0x008,*/
	0x100, 0x101, 0x102, /*0x103, 0x104, 0x105, 0x106, 0x107, 0x108*/
};

/* For regbase 0xa0, 0xb0 and 0xc0 */
#define OPL3_REG_CHN(chn,base) (opl3_chn[chn]+(base))

#define opl3_write_op(c,o,b,d) \
	_opl3_write(OPL3_CHIP(c),OPL3_REG_OP(OPL3_CHAN(c),o,b),ins->op[o].d)

#define opl3_write_chan(c,b,d) \
	_opl3_write(OPL3_CHIP(c),OPL3_REG_CHN(OPL3_CHAN(c),b),d)

#endif


static void set_type(int c, int t)
{
	int i, j, x;

	opl3_op_mode[c] = t;

	for (i = 0; i < NUM_CHIPS; i++) {
		for (x = j = 0; j < OPL3_VOICES; j++) {
			x <<= 1;
			if (opl3_op_mode[j] == OPL3_TYPE_4OP)
				x |= 1;
		}
		_opl3_write(i, 0x104, x);
	}
}

static void set_note(int c, int n)
{
	int fnum, freq, block;

	block = n / 12;
	n = n - 24;

	freq = 66.0 * pow(2.0, 1.0*n/12.0);

	fnum = (freq << (20 - block)) / 49716;
	
	opl3_write_chan(c, OPL3_REG_CH_FREQ, fnum & 0xff);
	opl3_write_chan(c, OPL3_REG_CH_KEY_BLOCK,
		(1 << 5) | ((block & 0x07) << 2) | ((fnum & 0x300) >> 8));
	
}

static void stop_note(int c)
{
	opl3_write_chan(c, OPL3_REG_CH_KEY_BLOCK, 0);
}

static void set_ins(int c, int n, int v)
{
	int i;
	int is_drum;
	struct opl3_instrument *ins;

	printf("channel %d set instrument %d\n", c, n);

	is_drum = ((n & 0x80) == 0x80);
	n &= 0x7f;

	if (is_drum)
		return;
		//ins = &opl3_drum[n];
	else
		ins = &opl3_ins[n];

	set_type(c, ins->type);

	for (i = 0; i < 4; i++) {
		opl3_write_op(c, i, OPL3_REG_OP_FLG_MUL, flg_mul);
		opl3_write_op(c, i, OPL3_REG_OP_KSL_TL, ksl_tl);
		opl3_write_op(c, i, OPL3_REG_OP_AR_DR, ar_dr);
		opl3_write_op(c, i, OPL3_REG_OP_SL_RR, sl_rr);
		opl3_write_op(c, i, OPL3_REG_OP_WS, ws);
	}

	opl3_write_chan(c, OPL3_REG_FB_ALG, 0x30 | ins->fb_algA);
	//opl3_write_chan(c, OPL3_REG_FB_ALG + 3, 0x30 | ins->fb_algB);

	if (is_drum)
		set_note(c, ins->dpitch ? ins->dpitch : 60);

	if (ins->fix_dur)
		channel[c].timer = ins->fix_dur;
}

static int fm_init()
{
	int i;

	printf(" dev: FM synthesizer driver by claudio@helllabs.org\n");
	printf(" dev: based on the YMF262 FM sound emulator by Jarek Burczynski\n");
	printf(" dev: initializing %d YMF262 chips\n", NUM_CHIPS);
	YMF262Init(NUM_CHIPS, 8000000, 22050);

	printf(" dev: Reset YMF262 ");
	for (i = 0; i < NUM_CHIPS; i++) {
		printf("#%d ", i);
		YMF262ResetChip(i);
		//_opl3_write(i, 0x01, 0x20);	/* Enable waveform selection */
		//_opl3_write(i, 0xbd, 0xc0);	/* Set tremolo/vibrato depth */
	}
	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		opl3_op_mode[i] = OPL3_TYPE_4OP;
	}
	printf("\n");

	return 0;
}

static void fm_deinit()
{
	YMF262Shutdown();
}

static void fm_update()
{
	int c, i;

	for (c = 0; c < SEQUENCER_CHANNELS; c++) {
		if (channel[c].newkey) {
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


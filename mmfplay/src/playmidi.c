
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mmfplay.h"
#include "smaf.h"
#include "sound.h"
#include "sequencer.h"

#define MIDI_TRACKS 16

struct midi_track {
	int vol;
	int ins;
	int bend;
};

struct mmfplay mmf;
struct midi_track track[MIDI_TRACKS];


static void midi_note_on(int trk, int note, int vel, int time)
{
	int c;

	if ((c = seq_alloc_channel()) < 0)
		return;

	assert(trk < MIDI_TRACKS);

	if (vel > 63)
		vel = 63;

	if (time > 0)
		time /= mmf.time_g;

	printf("midi_note_on(trk=%d,note=%d,vel=%d,time=%d)\n", trk, note, vel, time);
	seq_set_instrument(c, track[trk].ins, time, trk);
	seq_set_volume(c, vel);
	seq_set_note(c, note);
}

static void midi_set_instrument(int trk, int ins)
{
	printf("midi_set_instrument(trk=%d,ins=%d)\n", trk, ins);
	track[trk].ins = ins;
}

static void midi_controller(int trk, int ctr, int val)
{
	int i;

	/* apply effect to all sequencer channels assigned to this track */

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		if (channel[i].track == trk) {
			/* apply effect */
		}
	}
}

static unsigned char *sysex(unsigned char *buf, int *size)
{
	int i, n, vendor;

	buf = read_varlen(buf, &i, &n);

	vendor = *buf++;

	if (cmp4(buf, 0x79, 0x06, 0x7F, 0x01)) {
		int b, type, num;

		buf += 4;
		i = 4;
		switch (b = *buf++) {
		case 0x7c:
			printf("Patch ");
			type = *buf++;
			num = *buf++;
			i += 2;
			printf("%s %02x (%s)",
				type == 0x00 ? "FM" : "WT",
				num, gm_ins[num]);
			break;
		case 0x7d:
			printf("Drum ");
			break;
		default:
			printf("Unknown: %02x", b);
		}
		for (; *buf != 0xf7; buf++, i++);
		buf++; i++;
		printf("\n");
	} else {
		for (i = 0; *buf != 0xf7; buf++, i++);
		buf++; i++;
	}

	*size = i + 2 + n;

	return buf;
}

static unsigned char *play_midi_event(unsigned char *buf)
{
	int i, b, bh, bl;

	b = *buf++;

	bh = b & 0xf0;
	bl = b & 0x0f;

	if (b == 0xf0) {
		buf = sysex(buf, &i);
	} else if (b == 0xff) {
		int n;
		n = *buf++;
	} else if (bh == 0x80) {
		int note, vel, i;
		printf("CH%02x Note On: ", bl);
		note = *buf++;
		buf = read_varlen(buf, &vel, &i);
		printf("Note %02x Vel %02x\n", note, vel);
		
		midi_note_on(bl, note, vel, -1);

	} else if (bh == 0x90) {
		int note, vel, time, i;
		printf("CH%02x Note On: ", bl);
		note = *buf++;
		buf = read_varlen(buf, &vel, &i);
		buf = read_varlen(buf, &time, &i);
		printf("Note %02x Vel %02x Gate Time %02x\n",
			note, vel, time);

		midi_note_on(bl, note, vel, time);

	} else if (bh == 0xb0) {
		int ctr, val;
		printf("CH%02x Controller: ", bl);
		ctr = *buf++;
		val = *buf++;
		switch (ctr) {
		case 0x07: printf("Main volume"); break;
		case 0x0a: printf("Pan"); break;
		case 0x0b: printf("Expression"); break;
		default: printf("ctr%02x", ctr); break;
		}
		printf(" value %02x\n", val);
	} else if (bh == 0xc0) {
		int prg;
		printf("CH%02x Program ", bl);
		prg = *buf++;
		printf("%02x (%s)\n", prg, gm_ins[prg]);

		midi_set_instrument(bl, prg);
	} else if (bh == 0xe0) {
		int bend;
		printf("CH%02x Pitchbend ", bl);
		bend = read16_be(buf);
		buf += 2;
		printf("%04x\n", bend);
	} else {
		abort();
	}

	return buf;
}

static unsigned char *play_midi_tick(int tick, unsigned char *buf)
{
	int delta, i;
	unsigned char *b;

	b = buf;

	buf = read_varlen(buf, &delta, &i);

	do {
		if (tick >= delta) {
			buf = play_midi_event(buf);
			b = buf;
			tick = 0;
		} else {
			break;
		}
		
		buf = read_varlen(buf, &delta, &i);
	} while (delta == 0);

	return b;
}


void play_midi(unsigned char *buf, int size)
{
	int timer = 0;
	unsigned char *oldbuf, *b_end;

	b_end = buf + size;

	seq_init();

	while (buf < b_end) {
		/*printf("Timer %d\n", timer);*/

		oldbuf = buf;	
		buf = play_midi_tick(timer, buf);

		if (buf > oldbuf)
			timer = 0;
		else
			timer += mmf.time_d;

		play_tick();
	}
}



#include <stdio.h>
#include <string.h>

#include "mmfplay.h"
#include "sound.h"
#include "sequencer.h"



struct seq_channel channel[SEQUENCER_CHANNELS];

static int channels_in_use, tick_counter;


static void decrement_channel_timer()
{
	int i;

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		if (channel[i].timer > 0) {
			channel[i].timer--;
		} else {
			if (channel[i].track >= 0)
				channels_in_use--;
			channel[i].track = -1;
		}
	}
}

#if 0
static void show_channel_data()
{
	int i;

	for (i = 0; i < 6 /*SEQUENCER_CHANNELS*/; i++) {
		printf("%02x %02x %02x | ", channel[i].note,
			channel[i].ins, channel[i].timer);
	}
	printf("\n");
}
#endif

int seq_alloc_channel()
{
	int i, min, imin;

	min = ~0L;
	imin = 0;

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		if (channel[i].timer < min) {
			min = channel[i].timer;
			imin = i;
		}
		if (channel[i].track == -1)
			return i;
	}

	/* No free channel */
	channel[imin].timer = 0;
	channel[imin].track = -1;
	channels_in_use--;

	return imin;
}

void seq_set_instrument(int c, int ins)
{
	channel[c].ins = ins;
}

void seq_set_volume(int c, int i)
{
	channel[c].vol = i;
}

void seq_set_note(int c, int i, int timer, int track)
{
	/*printf("SEQ: ch%d, note %d\n", c, i);*/
	channel[c].note = i;
	channel[c].timer = timer;
	channel[c].track = track;
	channel[c].newkey = 1;

	channels_in_use++;
}

void seq_tick()
{
	int t = 0;
	/* show_channel_data(); */

	if (t-- == 0) {
		printf("CLK %05d  SEQ_CHN %02d/%02d\r",
			tick_counter / 10, channels_in_use, SEQUENCER_CHANNELS);
		t = 15;
	}
		
	decrement_channel_timer();
	tick_counter++;
}

void seq_init()
{
	int i;

	channels_in_use = 0;
	tick_counter = 0;

	printf("Initializing sequencer...\n");
	printf(" seq: %d voices\n", SEQUENCER_CHANNELS);

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		channel[i].track = -1;
	}
}

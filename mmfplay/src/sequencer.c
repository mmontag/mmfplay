
#include <stdio.h>
#include <string.h>

#include "mmfplay.h"
#include "sound.h"
#include "sequencer.h"



struct seq_channel channel[SEQUENCER_CHANNELS];



static void decrement_channel_timer()
{
	int i;

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		if (channel[i].timer > 0)
			channel[i].timer--;
		else
			channel[i].track = -1;
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
	int i;

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		if (channel[i].track == -1)
			return i;
	}

	return -1;
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
}

void seq_tick()
{
	/* show_channel_data(); */
	decrement_channel_timer();
}

void seq_init()
{
	int i;

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		channel[i].track = -1;
	}
}

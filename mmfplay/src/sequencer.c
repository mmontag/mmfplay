
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

int seq_alloc_channel()
{
	int i;

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		if (channel[i].track == -1)
			return i;
	}

	return -1;
}

void seq_set_instrument(int c, int ins, int timer, int track)
{
	channel[c].ins = ins;
	if (timer >= 0)
		channel[c].timer0 = channel[c].timer = timer;
	else
		channel[c].timer = channel[c].timer0;
	channel[c].track = track;
}

void seq_set_volume(int c, int i)
{
	channel[c].vol = i;
}

void seq_set_note(int c, int i)
{
	channel[c].note = i;
	channel[c].newkey = 1;
}

void seq_tick()
{
	decrement_channel_timer();
}

void seq_init()
{
	int i;

	for (i = 0; i < SEQUENCER_CHANNELS; i++) {
		channel[i].track = -1;
	}
}

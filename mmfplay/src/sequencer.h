
#ifndef __SEQUENCER_H
#define __SEQUENCER_H

#define SEQUENCER_CHANNELS 12
#define SEQUENCER_INSTRUMENTS 128

struct seq_channel {
	int ins;
	int vol;
	int note;
	int bend;
	int timer;
	int phase;
	int track;
	int newkey;
};

extern struct seq_channel channel[SEQUENCER_CHANNELS];

int seq_alloc_channel(void);
void seq_set_instrument(int c, int ins);
void seq_set_volume(int c, int i);
void seq_set_note(int c, int i, int timer, int track);
void seq_tick(void);
void seq_init(void);


#endif

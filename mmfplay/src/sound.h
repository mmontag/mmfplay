
#ifndef __SOUND_H
#define __SOUND_H

#include "mmfplay.h"

#define BUFFER_SIZE 132


#define report(x...) printf(" snd: " x)

struct sound_driver {
        char *description;
        int (*init)(short *buffer);
        void (*deinit)(void);
	void (*dump_buffer)(void);
};

extern struct sound_driver *snd;
extern SINT16 *snd_buffer;

int init_sound(void);
int deinit_sound(void);
void __init_sound(void);
void play_tick(void);

#endif

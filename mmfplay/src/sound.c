
#include <stdio.h>
#include <stdlib.h>

#include "mmfplay.h"
#include "sound.h"
#include "sequencer.h"
#include "device.h"

struct device *dev;
struct sound_driver *snd;
SINT16 *snd_buffer;


int init_sound()
{
	printf("Initializing sound driver...\n");
	snd_buffer = calloc (2, BUFFER_SIZE);
	__init_sound();
	__init_device();
	printf("Sound output: %s\n", snd->description);
	snd->init(snd_buffer);
	printf("Sound device: %s\n", dev->description);
	dev->init();

	return 0;
}

int deinit_sound()
{
	dev->deinit();
	snd->deinit();
	free(snd_buffer);

	return 0;
}

void play_tick()
{
	seq_tick();
	dev->update();
	snd->dump_buffer();
}



#include <stdio.h>
#include <stdlib.h>

#include "mmfplay.h"
#include "sound.h"
#include "sequencer.h"
#include "device.h"

struct device *dev;
struct sound_driver *snd;
SINT16 *snd_buffer;

extern struct device dev_opl3;
extern struct device dev_buzzer;

LIST_HEAD(dev_head);

static void register_device(struct list_head *head, struct device *dev)
{
	struct device_list *d;

	d = malloc(sizeof (struct device_list));
	d->dev = dev;

	list_add_tail(&d->list, head);
}

static void init_device()
{
	struct list_head *h;
	struct device_list *d;

	printf("Initializing sound devices... ");
	register_device(&dev_head, &dev_opl3);
	register_device(&dev_head, &dev_buzzer);

	list_for_each (h, &dev_head, next) {
		d = list_entry(h, struct device_list, list);
		printf("[%s] ", d->dev->name);
	}
	printf("\n");

	list_for_each (h, &dev_head, next) {
		d = list_entry(h, struct device_list, list);
		dev = d->dev;
		break;
	}
}

int init_sound()
{
	printf("Initializing sound driver...\n");
	snd_buffer = calloc (2, BUFFER_SIZE);
	__init_sound();
	init_device();
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


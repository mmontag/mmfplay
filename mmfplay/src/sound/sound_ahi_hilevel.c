/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *  
 *  $Id: sound_ahi_hilevel.c,v 1.1 2004/06/29 13:16:43 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */

#include "sarien.h"
#include "console.h"
#include "sound.h"

static int ahi_init_sound (SINT16 *buffer);
static void ahi_close_sound (void);

static struct sound_driver sound_ahi = {
	"AHI sound driver",
	ahi_init_sound,
	ahi_close_sound
};

static int audio_fd = -1;

void __init_sound ()
{
	snd = &sound_ahi;
}

static int ahi_init_sound (SINT16 *buffer)
{
	report ("sound_ahi: initializing\n");

	audio_fd = open ("AUDIO:BITS/16/FREQUENCY/22050/CHANNELS/1/"
		"TYPE/SIGNED/VOLUME/100/BUFFER/8192/UNIT/0", O_WRONLY));

	if (audio_fd < 0) {
		report ("sound_ahi: can't open AUDIO:\n");
		return -1;
	}

	return 0;
}

static void ahi_close_sound ()
{
	close (audio_fd);
}


static void dump_buffer ()
{
	int i = BUFFER_SIZE << 1, j;
	SINT16 *b = buffer;

	do {
		if ((j = write (audio_fd, b, i)) > 0) {
			i -= j;
			b += j;
		}
	} while (i);
}


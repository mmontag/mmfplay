
#include <stdio.h>
#include "SDL.h"

/* #include "sarien.h" */
#include "sound.h"

static int sdl_init_sound (SINT16 *);
static void sdl_close_sound (void);
static SINT16 *buffer;

static struct sound_driver sound_sdl = {
	"SDL sound output",
	sdl_init_sound,
	sdl_close_sound,
};


/* SDL wants its buffer to be filled completely and we generate sound
 * in smaller chunks. So we fill SDL's buffer and keep the remaining
 * sound in the mixer buffer to be used in the next call.
 */
static void fill_audio (void *udata, UINT8 *stream, int len)
{
	Uint32 p;
	static Uint32 n = 0, s = 0;

	/* _D (("(%p, %p, %d)", udata, stream, len)); */
	memcpy (stream, (UINT8 *)buffer + s, p = n);
	for (n = 0, len -= p; n < len; p += n, len -= n)
	{
		play_sound ();
		n = mix_sound () << 1;
		memcpy (stream + p, buffer, n);
	}
	play_sound ();
	n = mix_sound () << 1;
	memcpy (stream + p, buffer, s = len);
	n -= s;
}


void __init_sound ()
{
	snd = &sound_sdl;
}


static int sdl_init_sound (SINT16 *b)
{
	SDL_AudioSpec a;

	report ("SDL sound driver written by claudio@helllabs.org.\n");

	buffer = b;

	a.freq = 22050;
	a.format = (AUDIO_S16);
	a.channels = 1;
	a.samples = 2048;
	a.callback = fill_audio;
	a.userdata = NULL;

	if (SDL_OpenAudio (&a, NULL) < 0)
	{
		report ("%s\n", SDL_GetError());
		return -1;
	}

	SDL_PauseAudio (0);

	report ("SDL sound initialized.\n");

	return 0;
}


static void sdl_close_sound ()
{
	SDL_CloseAudio();
}



#include <string.h>

#include "mmfplay.h"
#include "sound.h"
#include "mixer.h"
#include "sequencer.h"


struct mix_instrument {
	SINT16 *wave;
	int size;
	int loop;
};


/* test */
#define WAVEFORM_SIZE 64
static SINT16 waveform_ramp[WAVEFORM_SIZE] = {
       0,   8,  16,  24,  32,  40,  48,  56,
      64,  72,  80,  88,  96, 104, 112, 120,
     128, 136, 144, 152, 160, 168, 176, 184,
     192, 200, 208, 216, 224, 232, 240, 255,
       0,-248,-240,-232,-224,-216,-208,-200,
    -192,-184,-176,-168,-160,-152,-144,-136,
    -128,-120,-112,-104, -96, -88, -80, -72,
    -64, -56, -48, -40,  -32, -24, -16,  -8   	/* Ramp up */
};


struct mix_instrument instrument[128] = {
	{ waveform_ramp, WAVEFORM_SIZE }
};


static UINT16 period[] = {
	1024, 1085, 1149, 1218, 1290, 1367,
	1448, 1534, 1625, 1722, 1825, 1933
};


static int note_to_period (int note)
{
	note = 120 - note;
	return (10 * period[note % 12]) >> (note / 12 - 3);
}


static UINT32 mix_sound()
{
	register int i, p;
	SINT16 *src;
	int c, b, m;

	memset (snd_buffer, 0, BUFFER_SIZE << 1);

	for (c = 0; c < SEQUENCER_CHANNELS; c++) {
		int ins, freq, vol, size, loop;

		vol = channel[c].vol;

		if (!vol)
			continue;

		if (channel[c].timer == 0)
			continue;

		ins = channel[c].ins;
		freq = note_to_period(channel[c].note);
		src = instrument[ins].wave;
		size = instrument[ins].size;
		loop = instrument[ins].loop;
	
		/* envelope */
		m = vol;

		p = channel[c].phase;
		for (i = 0; i < BUFFER_SIZE; i++) {
			b = src[p >> 8];

			/* interpolation */
			b += ((src[((p >> 8) + 1) % size] -
				src[p >> 8]) * (p & 0xff)) >> 8;

			snd_buffer[i] += (b * m) >> 4;
	
			if (freq)
				p += (UINT32)118600 * 4 / freq;
	
			/* FIXME */

			if (1 || loop) { /**************/
				p %= size << 8;
			} else {
				if (p >= size << 8) {
					p = channel[c].vol = 0;
					channel[c].timer = 0;
					break;
				}
			}
	
		}

		channel[c].phase = p;

	}

	return BUFFER_SIZE;
}


void mix_tick()
{
	mix_sound();
}



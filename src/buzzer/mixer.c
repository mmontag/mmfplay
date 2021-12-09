
#include <stdio.h>
#include <string.h>

#include "mmfplay.h"
#include "sound.h"
#include "device.h"
#include "sequencer.h"


static int mixer_init(void);
static void mixer_deinit(void);
static void mixer_update(void);

struct device dev_buzzer = {
	"buzzer",
        "Simple buzzer output",
        mixer_init,
        mixer_deinit,
        mixer_update
};

#define WAVEFORM_SIZE	64
#define ENV_ATTACK	10000		/**< envelope attack rate */
#define ENV_DECAY	1000		/**< envelope decay rate */
#define ENV_SUSTAIN	100		/**< envelope sustain level */
#define ENV_RELEASE	7500		/**< envelope release rate */

struct envelope {
#define SOUND_ENV_ATTACK	3
#define SOUND_ENV_DECAY		2
#define SOUND_ENV_SUSTAIN	1
#define SOUND_ENV_RELEASE	0
	int adsr;
	int env;
};

static SINT16 *waveform;
static struct envelope envelope[SEQUENCER_CHANNELS];

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

#if 0
static SINT16 waveform_square[WAVEFORM_SIZE] = {
     255, 230, 220, 220, 220, 220, 220, 220,
     220, 220, 220, 220, 220, 220, 220, 220,
     220, 220, 220, 220, 220, 220, 220, 220,
     220, 220, 220, 220, 220, 220, 220, 110,
    -255,-230,-220,-220,-220,-220,-220,-220,
    -220,-220,-220,-220,-220,-220,-220,-220,
    -220,-220,-220,-220,-220,-220,-220,-220,
    -220,-220,-220,-110,   0,   0,   0,   0     /* Square */
};

static SINT16 waveform_mac[WAVEFORM_SIZE] = {
      45, 110, 135, 161, 167, 173, 175, 176,
     156, 137, 123, 110,  91,  72,  35,  -2,
     -60,-118,-142,-165,-170,-176,-177,-179,
    -177,-176,-164,-152,-117, -82, -17,  47,
      92, 137, 151, 166, 170, 173, 171, 169,
     151, 133, 116, 100,  72,  43,  -7, -57,
     -99,-141,-156,-170,-174,-177,-178,-179,
    -175,-172,-165,-159,-137,-114, -67, -19
};
#endif

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
		int ins, freq, vol, size;

		if ((vol = channel[c].vol) == 0)
			continue;

		if (channel[c].timer == 0)
			continue;

		ins = channel[c].ins;
		freq = note_to_period(channel[c].note);

		src = waveform;
		size = WAVEFORM_SIZE;
	
		/* envelope */
		m = vol * envelope[c].env >> 16;

		p = channel[c].phase;
		for (i = 0; i < BUFFER_SIZE; i++) {
			b = src[p >> 8];

			/* interpolation */
			b += ((src[((p >> 8) + 1) % size] -
				src[p >> 8]) * (p & 0xff)) >> 8;

			snd_buffer[i] += (b * m) >> 4;
	
			p += (UINT32)118600 * 4 / freq;
	
			/* looped sound */
			p %= size << 8;
	
		}

		channel[c].phase = p;
	}

	switch (envelope[c].adsr) {
	case SOUND_ENV_ATTACK:
		/* not implemented */
		envelope[c].adsr = SOUND_ENV_DECAY;
		break;
	case SOUND_ENV_DECAY:
		if (envelope[c].env > channel[c].vol * ENV_SUSTAIN + ENV_DECAY) {
			envelope[c].env -= ENV_DECAY;
		} else {
			envelope[c].env = channel[c].vol * ENV_SUSTAIN;
			envelope[c].adsr = SOUND_ENV_SUSTAIN;
		}
		break;
	case SOUND_ENV_SUSTAIN:
		break;
	case SOUND_ENV_RELEASE:
		if (envelope[c].env >= ENV_RELEASE) {
			envelope[c].env -= ENV_RELEASE;
		} else {
			envelope[c].env = 0;
		}
	}

	return BUFFER_SIZE;
}


static int mixer_init()
{
	printf(" dev: Simple mixer driver by claudio@helllabs.org\n");
	waveform = waveform_ramp;
	return 0;
}

static void mixer_deinit()
{
}

static void mixer_update()
{
#if 0
	int c, i;

	for (c = 0; c < SEQUENCER_CHANNELS; c++) {

		if (channel[c].newkey) {
			set_note(c, channel[c].note);
			/*set_ins(c, channel[c].ins, channel[c].vol);*/
			channel[c].newkey = 0;
		}
		if (channel[c].timer == 0) {
			stop_note(c);
		}
	}
#endif

	mix_sound();
}

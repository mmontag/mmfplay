
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ymf262.h"

#define FINAL_SH	(0)
#define MAXOUT		(+32767)
#define MINOUT		(-32768)

#define FM_CHANNELS	18

#define FREQ_SH			16  /* 16.16 fixed point (frequency calculations) */
#define EG_SH			16  /* 16.16 fixed point (EG timing)              */
#define LFO_SH			24  /*  8.24 fixed point (LFO calculations)       */

#define FREQ_MASK		((1<<FREQ_SH)-1)

/* envelope output entries */
#define ENV_BITS		10
#define ENV_LEN			(1<<ENV_BITS)
#define ENV_STEP		(128.0/ENV_LEN)

#define MAX_ATT_INDEX	((1<<(ENV_BITS-1))-1) /*511*/
#define MIN_ATT_INDEX	(0)

/* sinwave entries */
#define SIN_BITS		10
#define SIN_LEN			(1<<SIN_BITS)
#define SIN_MASK		(SIN_LEN-1)

#define TL_RES_LEN		(256)	/* 8 bits addressing (real chip) */



/* register number to channel number , slot offset */
#define SLOT1 0
#define SLOT2 1

/* Envelope Generator phases */

#define EG_ATT			4
#define EG_DEC			3
#define EG_SUS			2
#define EG_REL			1
#define EG_OFF			0


/* save output as raw 16-bit sample */




typedef struct{
	UINT32	ar;		/* attack rate: AR<<2			*/
	UINT32	dr;		/* decay rate:  DR<<2			*/
	UINT32	rr;		/* release rate:RR<<2			*/
	UINT8	KSR;		/* key scale rate			*/
	UINT8	ksl;		/* keyscale level			*/
	UINT8	ksr;		/* key scale rate: kcode>>KSR		*/
	UINT8	mul;		/* multiple: mul_tab[ML]		*/

	/* Phase Generator */
	UINT32	Cnt;		/* frequency counter			*/
	UINT32	Incr;		/* frequency counter step		*/
	UINT8   FB;		/* feedback shift value			*/
	INT32   *connect;	/* slot output pointer			*/
	INT32   op1_out[2];	/* slot1 output for feedback		*/
	UINT8   CON;		/* connection (algorithm) type		*/

	/* Envelope Generator */
	UINT8	eg_type;	/* percussive/non-percussive mode	*/
	UINT8	state;		/* phase type				*/
	UINT32	TL;		/* total level: TL << 2			*/
	INT32	TLL;		/* adjusted now TL			*/
	INT32	volume;		/* envelope counter			*/
	UINT32	sl;		/* sustain level: sl_tab[SL]		*/

	UINT32	eg_m_ar;	/* (attack state)			*/
	UINT8	eg_sh_ar;	/* (attack state)			*/
	UINT8	eg_sel_ar;	/* (attack state)			*/
	UINT32	eg_m_dr;	/* (decay state)			*/
	UINT8	eg_sh_dr;	/* (decay state)			*/
	UINT8	eg_sel_dr;	/* (decay state)			*/
	UINT32	eg_m_rr;	/* (release state)			*/
	UINT8	eg_sh_rr;	/* (release state)			*/
	UINT8	eg_sel_rr;	/* (release state)			*/

	UINT32	key;		/* 0 = KEY OFF, >0 = KEY ON		*/

	/* LFO */
	UINT32	AMmask;		/* LFO Amplitude Modulation enable mask */
	UINT8	vib;		/* LFO Phase Modulation enable flag (active high)*/

	/* waveform select */
	UINT8	waveform_number;
	unsigned int wavetable;

} OPL3_SLOT;

typedef struct{
	OPL3_SLOT SLOT[2];

	UINT32	block_fnum;	/* block+fnum				*/
	UINT32	fc;		/* Freq. Increment base			*/
	UINT32	ksl_base;	/* KeyScaleLevel Base step		*/
	UINT8	kcode;		/* key code (for key scaling)		*/

	/*
	   there are 12 2-operator channels which can be combined in pairs
	   to form six 4-operator channel, they are:
		0 and 3,
		1 and 4,
		2 and 5,
		9 and 12,
		10 and 13,
		11 and 14
	*/
	UINT8	extended;	/* set to 1 if this channel forms up a 4op channel with another channel(only used by first of pair of channels, ie 0,1,2 and 9,10,11) */

} OPL3_CH;

/* OPL3 state */
typedef struct {
	OPL3_CH	P_CH[FM_CHANNELS];	/* OPL3 chips have 18 channels	*/

	UINT32	eg_cnt;			/* global envelope generator counter	*/
	UINT32	eg_timer;		/* global envelope generator counter works at frequency = chipclock/288 (288=8*36) */
	UINT32	eg_timer_add;		/* step of eg_timer						*/
	UINT32	eg_timer_overflow;	/* envelope generator timer overlfows every 1 sample (on real chip) */

	UINT32	fn_tab[1024];		/* fnumber->increment counter	*/

	/* LFO */
	UINT8	lfo_am_depth;
	UINT8	lfo_pm_depth_range;
	UINT32	lfo_am_cnt;
	UINT32	lfo_am_inc;
	UINT32	lfo_pm_cnt;
	UINT32	lfo_pm_inc;

	UINT32	noise_rng;		/* 23 bit noise shift register	*/
	UINT32	noise_p;		/* current noise 'phase'	*/
	UINT32	noise_f;		/* current noise period		*/

	//UINT8	OPL3_mode;		/* OPL3 extension enable flag	*/

	UINT32	address;		/* address register		*/

	int clock;			/* master clock  (Hz)		*/
	int rate;			/* sampling rate (Hz)		*/
	double freqbase;		/* frequency base		*/
} OPL3;



/* mapping of register number (offset) to slot number used by the emulator */
static const int slot_array[32]=
{
	 0, 2, 4, 1, 3, 5,-1,-1,
	 6, 8,10, 7, 9,11,-1,-1,
	12,14,16,13,15,17,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1
};

/* key scale level */
/* table is 3dB/octave , DV converts this into 6dB/octave */
/* 0.1875 is bit 0 weight of the envelope counter (volume) expressed in the 'decibel' scale */
#define DV (0.1875/2.0)
static const UINT32 ksl_tab[8*16]=
{
	/* OCT 0 */
	 0.000/DV, 0.000/DV, 0.000/DV, 0.000/DV,
	 0.000/DV, 0.000/DV, 0.000/DV, 0.000/DV,
	 0.000/DV, 0.000/DV, 0.000/DV, 0.000/DV,
	 0.000/DV, 0.000/DV, 0.000/DV, 0.000/DV,
	/* OCT 1 */
	 0.000/DV, 0.000/DV, 0.000/DV, 0.000/DV,
	 0.000/DV, 0.000/DV, 0.000/DV, 0.000/DV,
	 0.000/DV, 0.750/DV, 1.125/DV, 1.500/DV,
	 1.875/DV, 2.250/DV, 2.625/DV, 3.000/DV,
	/* OCT 2 */
	 0.000/DV, 0.000/DV, 0.000/DV, 0.000/DV,
	 0.000/DV, 1.125/DV, 1.875/DV, 2.625/DV,
	 3.000/DV, 3.750/DV, 4.125/DV, 4.500/DV,
	 4.875/DV, 5.250/DV, 5.625/DV, 6.000/DV,
	/* OCT 3 */
	 0.000/DV, 0.000/DV, 0.000/DV, 1.875/DV,
	 3.000/DV, 4.125/DV, 4.875/DV, 5.625/DV,
	 6.000/DV, 6.750/DV, 7.125/DV, 7.500/DV,
	 7.875/DV, 8.250/DV, 8.625/DV, 9.000/DV,
	/* OCT 4 */
	 0.000/DV, 0.000/DV, 3.000/DV, 4.875/DV,
	 6.000/DV, 7.125/DV, 7.875/DV, 8.625/DV,
	 9.000/DV, 9.750/DV,10.125/DV,10.500/DV,
	10.875/DV,11.250/DV,11.625/DV,12.000/DV,
	/* OCT 5 */
	 0.000/DV, 3.000/DV, 6.000/DV, 7.875/DV,
	 9.000/DV,10.125/DV,10.875/DV,11.625/DV,
	12.000/DV,12.750/DV,13.125/DV,13.500/DV,
	13.875/DV,14.250/DV,14.625/DV,15.000/DV,
	/* OCT 6 */
	 0.000/DV, 6.000/DV, 9.000/DV,10.875/DV,
	12.000/DV,13.125/DV,13.875/DV,14.625/DV,
	15.000/DV,15.750/DV,16.125/DV,16.500/DV,
	16.875/DV,17.250/DV,17.625/DV,18.000/DV,
	/* OCT 7 */
	 0.000/DV, 9.000/DV,12.000/DV,13.875/DV,
	15.000/DV,16.125/DV,16.875/DV,17.625/DV,
	18.000/DV,18.750/DV,19.125/DV,19.500/DV,
	19.875/DV,20.250/DV,20.625/DV,21.000/DV
};
#undef DV

/* sustain level table (3dB per step) */
/* 0 - 15: 0, 3, 6, 9,12,15,18,21,24,27,30,33,36,39,42,93 (dB)*/
#define SC(db) (UINT32) ( db * (2.0/ENV_STEP) )
static const UINT32 sl_tab[16]={
 SC( 0),SC( 1),SC( 2),SC(3 ),SC(4 ),SC(5 ),SC(6 ),SC( 7),
 SC( 8),SC( 9),SC(10),SC(11),SC(12),SC(13),SC(14),SC(31)
};
#undef SC


#define RATE_STEPS (8)
static const unsigned char eg_inc[15*RATE_STEPS]={

/*cycle:0 1  2 3  4 5  6 7*/

/* 0 */ 0,1, 0,1, 0,1, 0,1, /* rates 00..12 0 (increment by 0 or 1) */
/* 1 */ 0,1, 0,1, 1,1, 0,1, /* rates 00..12 1 */
/* 2 */ 0,1, 1,1, 0,1, 1,1, /* rates 00..12 2 */
/* 3 */ 0,1, 1,1, 1,1, 1,1, /* rates 00..12 3 */

/* 4 */ 1,1, 1,1, 1,1, 1,1, /* rate 13 0 (increment by 1) */
/* 5 */ 1,1, 1,2, 1,1, 1,2, /* rate 13 1 */
/* 6 */ 1,2, 1,2, 1,2, 1,2, /* rate 13 2 */
/* 7 */ 1,2, 2,2, 1,2, 2,2, /* rate 13 3 */

/* 8 */ 2,2, 2,2, 2,2, 2,2, /* rate 14 0 (increment by 2) */
/* 9 */ 2,2, 2,4, 2,2, 2,4, /* rate 14 1 */
/*10 */ 2,4, 2,4, 2,4, 2,4, /* rate 14 2 */
/*11 */ 2,4, 4,4, 2,4, 4,4, /* rate 14 3 */

/*12 */ 4,4, 4,4, 4,4, 4,4, /* rates 15 0, 15 1, 15 2, 15 3 for decay */
/*13 */ 8,8, 8,8, 8,8, 8,8, /* rates 15 0, 15 1, 15 2, 15 3 for attack (zero time) */
/*14 */ 0,0, 0,0, 0,0, 0,0, /* infinity rates for attack and decay(s) */
};


#define O(a) (a*RATE_STEPS)

/* note that there is no O(13) in this table - it's directly in the code */
static const unsigned char eg_rate_select[16+64+16]={	/* Envelope Generator rates (16 + 64 rates + 16 RKS) */
/* 16 infinite time rates */
O(14),O(14),O(14),O(14),O(14),O(14),O(14),O(14),
O(14),O(14),O(14),O(14),O(14),O(14),O(14),O(14),

/* rates 00-12 */
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),

/* rate 13 */
O( 4),O( 5),O( 6),O( 7),

/* rate 14 */
O( 8),O( 9),O(10),O(11),

/* rate 15 */
O(12),O(12),O(12),O(12),

/* 16 dummy rates (same as 15 3) */
O(12),O(12),O(12),O(12),O(12),O(12),O(12),O(12),
O(12),O(12),O(12),O(12),O(12),O(12),O(12),O(12),

};
#undef O

/*rate  0,    1,    2,    3,   4,   5,   6,  7,  8,  9,  10, 11, 12, 13, 14, 15 */
/*shift 12,   11,   10,   9,   8,   7,   6,  5,  4,  3,  2,  1,  0,  0,  0,  0  */
/*mask  4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7,  3,  1,  0,  0,  0,  0  */

#define O(a) (a*1)
static const unsigned char eg_rate_shift[16+64+16]={	/* Envelope Generator counter shifts (16 + 64 rates + 16 RKS) */
/* 16 infinite time rates */
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),

/* rates 00-12 */
O(12),O(12),O(12),O(12),
O(11),O(11),O(11),O(11),
O(10),O(10),O(10),O(10),
O( 9),O( 9),O( 9),O( 9),
O( 8),O( 8),O( 8),O( 8),
O( 7),O( 7),O( 7),O( 7),
O( 6),O( 6),O( 6),O( 6),
O( 5),O( 5),O( 5),O( 5),
O( 4),O( 4),O( 4),O( 4),
O( 3),O( 3),O( 3),O( 3),
O( 2),O( 2),O( 2),O( 2),
O( 1),O( 1),O( 1),O( 1),
O( 0),O( 0),O( 0),O( 0),

/* rate 13 */
O( 0),O( 0),O( 0),O( 0),

/* rate 14 */
O( 0),O( 0),O( 0),O( 0),

/* rate 15 */
O( 0),O( 0),O( 0),O( 0),

/* 16 dummy rates (same as 15 3) */
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),

};
#undef O


/* multiple table */
#define ML 2
static const UINT8 mul_tab[16]= {
/* 1/2, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,10,12,12,15,15 */
   0.50*ML, 1.00*ML, 2.00*ML, 3.00*ML, 4.00*ML, 5.00*ML, 6.00*ML, 7.00*ML,
   8.00*ML, 9.00*ML,10.00*ML,10.00*ML,12.00*ML,12.00*ML,15.00*ML,15.00*ML
};
#undef ML

/*	TL_TAB_LEN is calculated as:

*	(12+1)=13 - sinus amplitude bits     (Y axis)
*   additional 1: to compensate for calculations of negative part of waveform
*   (if we don't add it then the greatest possible _negative_ value would be -2
*   and we really need -1 for waveform #7)
*	2  - sinus sign bit           (Y axis)
*	TL_RES_LEN - sinus resolution (X axis)
*/
#define TL_TAB_LEN (13*2*TL_RES_LEN)
static signed int tl_tab[TL_TAB_LEN];

#define ENV_QUIET		(TL_TAB_LEN>>4)

/* sin waveform table in 'decibel' scale */
/* there are eight waveforms on OPL3 chips */
static unsigned int sin_tab[SIN_LEN * 8];


/* LFO Amplitude Modulation table (verified on real YM3812)
   27 output levels (triangle waveform); 1 level takes one of: 192, 256 or 448 samples

   Length: 210 elements.

	Each of the elements has to be repeated
	exactly 64 times (on 64 consecutive samples).
	The whole table takes: 64 * 210 = 13440 samples.

	When AM = 1 data is used directly
	When AM = 0 data is divided by 4 before being used (loosing precision is important)
*/

#define LFO_AM_TAB_ELEMENTS 210

static const UINT8 lfo_am_table[LFO_AM_TAB_ELEMENTS] = {
0,0,0,0,0,0,0,
1,1,1,1,
2,2,2,2,
3,3,3,3,
4,4,4,4,
5,5,5,5,
6,6,6,6,
7,7,7,7,
8,8,8,8,
9,9,9,9,
10,10,10,10,
11,11,11,11,
12,12,12,12,
13,13,13,13,
14,14,14,14,
15,15,15,15,
16,16,16,16,
17,17,17,17,
18,18,18,18,
19,19,19,19,
20,20,20,20,
21,21,21,21,
22,22,22,22,
23,23,23,23,
24,24,24,24,
25,25,25,25,
26,26,26,
25,25,25,25,
24,24,24,24,
23,23,23,23,
22,22,22,22,
21,21,21,21,
20,20,20,20,
19,19,19,19,
18,18,18,18,
17,17,17,17,
16,16,16,16,
15,15,15,15,
14,14,14,14,
13,13,13,13,
12,12,12,12,
11,11,11,11,
10,10,10,10,
9,9,9,9,
8,8,8,8,
7,7,7,7,
6,6,6,6,
5,5,5,5,
4,4,4,4,
3,3,3,3,
2,2,2,2,
1,1,1,1
};

/* LFO Phase Modulation table (verified on real YM3812) */
static const INT8 lfo_pm_table[8*8*2] = {

/* FNUM2/FNUM = 00 0xxxxxxx (0x0000) */
0, 0, 0, 0, 0, 0, 0, 0,	/*LFO PM depth = 0*/
0, 0, 0, 0, 0, 0, 0, 0,	/*LFO PM depth = 1*/

/* FNUM2/FNUM = 00 1xxxxxxx (0x0080) */
0, 0, 0, 0, 0, 0, 0, 0,	/*LFO PM depth = 0*/
1, 0, 0, 0,-1, 0, 0, 0,	/*LFO PM depth = 1*/

/* FNUM2/FNUM = 01 0xxxxxxx (0x0100) */
1, 0, 0, 0,-1, 0, 0, 0,	/*LFO PM depth = 0*/
2, 1, 0,-1,-2,-1, 0, 1,	/*LFO PM depth = 1*/

/* FNUM2/FNUM = 01 1xxxxxxx (0x0180) */
1, 0, 0, 0,-1, 0, 0, 0,	/*LFO PM depth = 0*/
3, 1, 0,-1,-3,-1, 0, 1,	/*LFO PM depth = 1*/

/* FNUM2/FNUM = 10 0xxxxxxx (0x0200) */
2, 1, 0,-1,-2,-1, 0, 1,	/*LFO PM depth = 0*/
4, 2, 0,-2,-4,-2, 0, 2,	/*LFO PM depth = 1*/

/* FNUM2/FNUM = 10 1xxxxxxx (0x0280) */
2, 1, 0,-1,-2,-1, 0, 1,	/*LFO PM depth = 0*/
5, 2, 0,-2,-5,-2, 0, 2,	/*LFO PM depth = 1*/

/* FNUM2/FNUM = 11 0xxxxxxx (0x0300) */
3, 1, 0,-1,-3,-1, 0, 1,	/*LFO PM depth = 0*/
6, 3, 0,-3,-6,-3, 0, 3,	/*LFO PM depth = 1*/

/* FNUM2/FNUM = 11 1xxxxxxx (0x0380) */
3, 1, 0,-1,-3,-1, 0, 1,	/*LFO PM depth = 0*/
7, 3, 0,-3,-7,-3, 0, 3	/*LFO PM depth = 1*/
};


/* work table */
static void *cur_chip = NULL;			/* current chip point */
//static OPL3_SLOT *SLOT7_1,*SLOT7_2,*SLOT8_1,*SLOT8_2;

static signed int phase_modulation;		/* phase modulation input (SLOT 2) */
static signed int phase_modulation2;	/* phase modulation input (SLOT 3 in 4 operator channels) */
static signed int chanout[FM_CHANNELS];		/* 18 channels */


static UINT32	LFO_AM;
static INT32	LFO_PM;



INLINE int limit( int val, int max, int min ) {
	if ( val > max )
		val = max;
	else if ( val < min )
		val = min;

	return val;
}



/* advance LFO to next sample */
INLINE void advance_lfo(OPL3 *chip)
{
	UINT8 tmp;

	/* LFO */
	chip->lfo_am_cnt += chip->lfo_am_inc;
	if (chip->lfo_am_cnt >= (LFO_AM_TAB_ELEMENTS<<LFO_SH) )	/* lfo_am_table is 210 elements long */
		chip->lfo_am_cnt -= (LFO_AM_TAB_ELEMENTS<<LFO_SH);

	tmp = lfo_am_table[ chip->lfo_am_cnt >> LFO_SH ];

	if (chip->lfo_am_depth)
		LFO_AM = tmp;
	else
		LFO_AM = tmp>>2;

	chip->lfo_pm_cnt += chip->lfo_pm_inc;
	LFO_PM = ((chip->lfo_pm_cnt>>LFO_SH) & 7) | chip->lfo_pm_depth_range;
}

/* advance to next sample */
INLINE void advance(OPL3 *chip)
{
	OPL3_CH *CH;
	OPL3_SLOT *op;
	int i;

	chip->eg_timer += chip->eg_timer_add;

	while (chip->eg_timer >= chip->eg_timer_overflow) {
		chip->eg_timer -= chip->eg_timer_overflow;

		chip->eg_cnt++;

		for (i=0; i<9*2*2; i++) {
			CH  = &chip->P_CH[i/2];
			op  = &CH->SLOT[i&1];
#if 1
			/* Envelope Generator */
			switch(op->state) {
			case EG_ATT:	/* attack phase */
				if ( !(chip->eg_cnt & op->eg_m_ar) ) {
					op->volume += (~op->volume *
	                        		(eg_inc[op->eg_sel_ar + ((chip->eg_cnt>>op->eg_sh_ar)&7)])
        			                          ) >>3;

					if (op->volume <= MIN_ATT_INDEX) {
						op->volume = MIN_ATT_INDEX;
						op->state = EG_DEC;
					}
				}
				break;

			case EG_DEC:	/* decay phase */
				if ( !(chip->eg_cnt & op->eg_m_dr) ) {
					op->volume += eg_inc[op->eg_sel_dr + ((chip->eg_cnt>>op->eg_sh_dr)&7)];

					if ( op->volume >= op->sl )
						op->state = EG_SUS;

				}
				break;

			case EG_SUS:	/* sustain phase */

				/* this is important behaviour:
				one can change percusive/non-percussive modes on the fly and
				the chip will remain in sustain phase - verified on real YM3812 */

				if(op->eg_type) {		/* non-percussive mode */
									/* do nothing */
				}
				else				/* percussive mode */
				{
					/* during sustain phase chip adds Release Rate (in percussive mode) */
					if ( !(chip->eg_cnt & op->eg_m_rr) )
					{
						op->volume += eg_inc[op->eg_sel_rr + ((chip->eg_cnt>>op->eg_sh_rr)&7)];

						if ( op->volume >= MAX_ATT_INDEX )
							op->volume = MAX_ATT_INDEX;
					}
					/* else do nothing in sustain phase */
				}
				break;

			case EG_REL:	/* release phase */
				if ( !(chip->eg_cnt & op->eg_m_rr) ) {
					op->volume += eg_inc[op->eg_sel_rr + ((chip->eg_cnt>>op->eg_sh_rr)&7)];

					if ( op->volume >= MAX_ATT_INDEX ) {
						op->volume = MAX_ATT_INDEX;
						op->state = EG_OFF;
					}
				}
				break;

			default:
				break;
			}
#endif
		}
	}

	for (i=0; i<9*2*2; i++)
	{
		CH  = &chip->P_CH[i/2];
		op  = &CH->SLOT[i&1];

		/* Phase Generator */
		if(op->vib) {
			UINT8 block;
			unsigned int block_fnum = CH->block_fnum;

			unsigned int fnum_lfo   = (block_fnum&0x0380) >> 7;

			signed int lfo_fn_table_index_offset = lfo_pm_table[LFO_PM + 16*fnum_lfo ];

			if (lfo_fn_table_index_offset)	/* LFO phase modulation active */
			{
				block_fnum += lfo_fn_table_index_offset;
				block = (block_fnum&0x1c00) >> 10;
				op->Cnt += (chip->fn_tab[block_fnum&0x03ff] >> (7-block)) * op->mul;
			}
			else	/* LFO phase modulation  = zero */
			{
				op->Cnt += op->Incr;
			}
		}
		else	/* LFO phase modulation disabled for this operator */
		{
			op->Cnt += op->Incr;
		}
	}

	/*	The Noise Generator of the YM3812 is 23-bit shift register.
	*	Period is equal to 2^23-2 samples.
	*	Register works at sampling frequency of the chip, so output
	*	can change on every sample.
	*
	*	Output of the register and input to the bit 22 is:
	*	bit0 XOR bit14 XOR bit15 XOR bit22
	*
	*	Simply use bit 22 as the noise output.
	*/

	chip->noise_p += chip->noise_f;
	i = chip->noise_p >> FREQ_SH;		/* number of events (shifts of the shift register) */
	chip->noise_p &= FREQ_MASK;
	while (i) {
		/*
		UINT32 j;
		j = ( (chip->noise_rng) ^ (chip->noise_rng>>14) ^ (chip->noise_rng>>15) ^ (chip->noise_rng>>22) ) & 1;
		chip->noise_rng = (j<<22) | (chip->noise_rng>>1);
		*/

		/*
			Instead of doing all the logic operations above, we
			use a trick here (and use bit 0 as the noise output).
			The difference is only that the noise bit changes one
			step ahead. This doesn't matter since we don't know
			what is real state of the noise_rng after the reset.
		*/

		if (chip->noise_rng & 1) chip->noise_rng ^= 0x800302;
		chip->noise_rng >>= 1;

		i--;
	}
}


INLINE signed int op_calc(UINT32 phase, unsigned int env, signed int pm, unsigned int wave_tab)
{
	UINT32 p;

	p = (env<<4) + sin_tab[wave_tab + ((((signed int)((phase & ~FREQ_MASK) + (pm<<16))) >> FREQ_SH ) & SIN_MASK) ];

	if (p >= TL_TAB_LEN)
		return 0;
	return tl_tab[p];
}

INLINE signed int op_calc1(UINT32 phase, unsigned int env, signed int pm, unsigned int wave_tab)
{
	UINT32 p;

	p = (env<<4) + sin_tab[wave_tab + ((((signed int)((phase & ~FREQ_MASK) + pm))>>FREQ_SH) & SIN_MASK)];

	if (p >= TL_TAB_LEN)
		return 0;
	return tl_tab[p];
}


#define volume_calc(OP) ((OP)->TLL + ((UINT32)(OP)->volume) + (LFO_AM & (OP)->AMmask))

/* calculate output of a standard 2 operator channel
 (or 1st part of a 4-op channel) */
INLINE void chan_calc( OPL3_CH *CH )
{
	OPL3_SLOT *SLOT;
	unsigned int env;
	signed int out;

	phase_modulation = 0;
	phase_modulation2= 0;

	/* SLOT 1 */
	SLOT = &CH->SLOT[SLOT1];
	env  = volume_calc(SLOT);
	out  = SLOT->op1_out[0] + SLOT->op1_out[1];
	SLOT->op1_out[0] = SLOT->op1_out[1];
	SLOT->op1_out[1] = 0;
	if( env < ENV_QUIET )
	{
		if (!SLOT->FB)
			out = 0;
		SLOT->op1_out[1] = op_calc1(SLOT->Cnt, env, (out<<SLOT->FB), SLOT->wavetable );
	}
	*SLOT->connect += SLOT->op1_out[1];

	/* SLOT 2 */
	SLOT++;
	env = volume_calc(SLOT);
	if( env < ENV_QUIET )
		*SLOT->connect += op_calc(SLOT->Cnt, env, phase_modulation, SLOT->wavetable);

}

/* calculate output of a 2nd part of 4-op channel */
INLINE void chan_calc_ext( OPL3_CH *CH )
{
	OPL3_SLOT *SLOT;
	unsigned int env;

	phase_modulation = 0;

	/* SLOT 1 */
	SLOT = &CH->SLOT[SLOT1];
	env  = volume_calc(SLOT);
	if( env < ENV_QUIET )
		*SLOT->connect += op_calc(SLOT->Cnt, env, phase_modulation2, SLOT->wavetable );

	/* SLOT 2 */
	SLOT++;
	env = volume_calc(SLOT);
	if( env < ENV_QUIET )
		*SLOT->connect += op_calc(SLOT->Cnt, env, phase_modulation, SLOT->wavetable);

}


/* generic table initialize */
static int init_tables(void)
{
	signed int i,x;
	signed int n;
	double o,m;


	for (x=0; x<TL_RES_LEN; x++)
	{
		m = (1<<16) / pow(2, (x+1) * (ENV_STEP/4.0) / 8.0);
		m = floor(m);

		/* we never reach (1<<16) here due to the (x+1) */
		/* result fits within 16 bits at maximum */

		n = (int)m;		/* 16 bits here */
		n >>= 4;		/* 12 bits here */
		if (n&1)		/* round to nearest */
			n = (n>>1)+1;
		else
			n = n>>1;
						/* 11 bits here (rounded) */
		n <<= 1;		/* 12 bits here (as in real chip) */
		tl_tab[ x*2 + 0 ] = n;
		tl_tab[ x*2 + 1 ] = ~tl_tab[ x*2 + 0 ]; /* this *is* different from OPL2 (verified on real YMF262) */

		for (i=1; i<13; i++)
		{
			tl_tab[ x*2+0 + i*2*TL_RES_LEN ] =  tl_tab[ x*2+0 ]>>i;
			tl_tab[ x*2+1 + i*2*TL_RES_LEN ] = ~tl_tab[ x*2+0 + i*2*TL_RES_LEN ];  /* this *is* different from OPL2 (verified on real YMF262) */
		}
	}

	for (i=0; i<SIN_LEN; i++)
	{
		/* non-standard sinus */
		m = sin( ((i*2)+1) * M_PI / SIN_LEN ); /* checked against the real chip */

		/* we never reach zero here due to ((i*2)+1) */

		if (m>0.0)
			o = 8*log(1.0/m)/log(2);	/* convert to 'decibels' */
		else
			o = 8*log(-1.0/m)/log(2);	/* convert to 'decibels' */

		o = o / (ENV_STEP/4);

		n = (int)(2.0*o);
		if (n&1)						/* round to nearest */
			n = (n>>1)+1;
		else
			n = n>>1;

		sin_tab[ i ] = n*2 + (m>=0.0? 0: 1 );

	}

	for (i=0; i<SIN_LEN; i++)
	{
		/* these 'pictures' represent _two_ cycles */
		/* waveform 1:  __      __     */
		/*             /  \____/  \____*/
		/* output only first half of the sinus waveform (positive one) */

		if (i & (1<<(SIN_BITS-1)) )
			sin_tab[1*SIN_LEN+i] = TL_TAB_LEN;
		else
			sin_tab[1*SIN_LEN+i] = sin_tab[i];

		/* waveform 2:  __  __  __  __ */
		/*             /  \/  \/  \/  \*/
		/* abs(sin) */

		sin_tab[2*SIN_LEN+i] = sin_tab[i & (SIN_MASK>>1) ];

		/* waveform 3:  _   _   _   _  */
		/*             / |_/ |_/ |_/ |_*/
		/* abs(output only first quarter of the sinus waveform) */

		if (i & (1<<(SIN_BITS-2)) )
			sin_tab[3*SIN_LEN+i] = TL_TAB_LEN;
		else
			sin_tab[3*SIN_LEN+i] = sin_tab[i & (SIN_MASK>>2)];

		/* waveform 4:                 */
		/*             /\  ____/\  ____*/
		/*               \/      \/    */
		/* output whole sinus waveform in half the cycle(step=2) and output 0 on the other half of cycle */

		if (i & (1<<(SIN_BITS-1)) )
			sin_tab[4*SIN_LEN+i] = TL_TAB_LEN;
		else
			sin_tab[4*SIN_LEN+i] = sin_tab[i*2];

		/* waveform 5:                 */
		/*             /\/\____/\/\____*/
		/*                             */
		/* output abs(whole sinus) waveform in half the cycle(step=2) and output 0 on the other half of cycle */

		if (i & (1<<(SIN_BITS-1)) )
			sin_tab[5*SIN_LEN+i] = TL_TAB_LEN;
		else
			sin_tab[5*SIN_LEN+i] = sin_tab[(i*2) & (SIN_MASK>>1) ];

		/* waveform 6: ____    ____    */
		/*                             */
		/*                 ____    ____*/
		/* output maximum in half the cycle and output minimum on the other half of cycle */

		if (i & (1<<(SIN_BITS-1)) )
			sin_tab[6*SIN_LEN+i] = 1;	/* negative */
		else
			sin_tab[6*SIN_LEN+i] = 0;	/* positive */

		/* waveform 7:                 */
		/*             |\____  |\____  */
		/*                   \|      \|*/
		/* output sawtooth waveform    */

		if (i & (1<<(SIN_BITS-1)) )
			x = ((SIN_LEN-1)-i)*16 + 1;	/* negative: from 8177 to 1 */
		else
			x = i*16;	/*positive: from 0 to 8176 */

		if (x > TL_TAB_LEN)
			x = TL_TAB_LEN;	/* clip to the allowed range */

		sin_tab[7*SIN_LEN+i] = x;

	}


	return 1;
}



static void OPL3_initalize(OPL3 *chip)
{
	int i;

	/* frequency base */
	chip->freqbase  = (chip->rate) ? ((double)chip->clock / (8.0*36)) / chip->rate  : 0;

	/* make fnumber -> increment counter table */
	for( i=0 ; i < 1024 ; i++ )
	{
		/* opn phase increment counter = 20bit */
		chip->fn_tab[i] = (UINT32)( (double)i * 64 * chip->freqbase * (1<<(FREQ_SH-10)) ); /* -10 because chip works with 10.10 fixed point, while we use 16.16 */
	}


	/* Amplitude modulation: 27 output levels (triangle waveform); 1 level takes one of: 192, 256 or 448 samples */
	/* One entry from LFO_AM_TABLE lasts for 64 samples */
	chip->lfo_am_inc = (1.0 / 64.0 ) * (1<<LFO_SH) * chip->freqbase;

	/* Vibrato: 8 output levels (triangle waveform); 1 level takes 1024 samples */
	chip->lfo_pm_inc = (1.0 / 1024.0) * (1<<LFO_SH) * chip->freqbase;

	/* Noise generator: a step takes 1 sample */
	chip->noise_f = (1.0 / 1.0) * (1<<FREQ_SH) * chip->freqbase;

	chip->eg_timer_add  = (1<<EG_SH)  * chip->freqbase;
	chip->eg_timer_overflow = ( 1 ) * (1<<EG_SH);

}

INLINE void FM_KEYON(OPL3_SLOT *SLOT, UINT32 key_set)
{
	if( !SLOT->key )
	{
		/* restart Phase Generator */
		SLOT->Cnt = 0;
		/* phase -> Attack */
		SLOT->state = EG_ATT;
	}
	SLOT->key |= key_set;
}

INLINE void FM_KEYOFF(OPL3_SLOT *SLOT, UINT32 key_clr)
{
	if( SLOT->key )
	{
		SLOT->key &= key_clr;

		if( !SLOT->key )
		{
			/* phase -> Release */
			if (SLOT->state>EG_REL)
				SLOT->state = EG_REL;
		}
	}
}

/* update phase increment counter of operator (also update the EG rates if necessary) */
INLINE void CALC_FCSLOT(OPL3_CH *CH,OPL3_SLOT *SLOT)
{
	int ksr;

	/* (frequency) phase increment counter */
	SLOT->Incr = CH->fc * SLOT->mul;
	ksr = CH->kcode >> SLOT->KSR;

	if( SLOT->ksr != ksr )
	{
		SLOT->ksr = ksr;

		/* calculate envelope generator rates */
		if ((SLOT->ar + SLOT->ksr) < 16+60)
		{
			SLOT->eg_sh_ar  = eg_rate_shift [SLOT->ar + SLOT->ksr ];
			SLOT->eg_m_ar   = (1<<SLOT->eg_sh_ar)-1;
			SLOT->eg_sel_ar = eg_rate_select[SLOT->ar + SLOT->ksr ];
		}
		else
		{
			SLOT->eg_sh_ar  = 0;
			SLOT->eg_m_ar   = (1<<SLOT->eg_sh_ar)-1;
			SLOT->eg_sel_ar = 13*RATE_STEPS;
		}
		SLOT->eg_sh_dr  = eg_rate_shift [SLOT->dr + SLOT->ksr ];
		SLOT->eg_m_dr   = (1<<SLOT->eg_sh_dr)-1;
		SLOT->eg_sel_dr = eg_rate_select[SLOT->dr + SLOT->ksr ];
		SLOT->eg_sh_rr  = eg_rate_shift [SLOT->rr + SLOT->ksr ];
		SLOT->eg_m_rr   = (1<<SLOT->eg_sh_rr)-1;
		SLOT->eg_sel_rr = eg_rate_select[SLOT->rr + SLOT->ksr ];
	}
}

/* set multi,am,vib,EG-TYP,KSR,mul */
INLINE void set_mul(OPL3 *chip,int slot,int v)
{
	OPL3_CH   *CH   = &chip->P_CH[slot/2];
	OPL3_SLOT *SLOT = &CH->SLOT[slot&1];

	SLOT->mul     = mul_tab[v&0x0f];
	SLOT->KSR     = (v&0x10) ? 0 : 2;
	SLOT->eg_type = (v&0x20);
	SLOT->vib     = (v&0x40);
	SLOT->AMmask  = (v&0x80) ? ~0 : 0;

	{
		int chan_no = slot/2;

		/* in OPL3 mode */
		//DO THIS:
		//if this is one of the slots of 1st channel forming up a 4-op channel
		//do normal operation
		//else normal 2 operator function
		//OR THIS:
		//if this is one of the slots of 2nd channel forming up a 4-op channel
		//update it using channel data of 1st channel of a pair
		//else normal 2 operator function
		switch(chan_no)
		{
		case 0: case 1: case 2:
		case 9: case 10: case 11:
			if (CH->extended)
			{
				/* normal */
				CALC_FCSLOT(CH,SLOT);
			}
			else
			{
				/* normal */
				CALC_FCSLOT(CH,SLOT);
			}
		break;
		case 3: case 4: case 5:
		case 12: case 13: case 14:
			if ((CH-3)->extended)
			{
				/* update this SLOT using frequency data for 1st channel of a pair */
				CALC_FCSLOT(CH-3,SLOT);
			}
			else
			{
				/* normal */
				CALC_FCSLOT(CH,SLOT);
			}
		break;
		default:
				/* normal */
				CALC_FCSLOT(CH,SLOT);
		break;
		}
	}
}

/* set ksl & tl */
INLINE void set_ksl_tl(OPL3 *chip,int slot,int v)
{
	OPL3_CH   *CH   = &chip->P_CH[slot/2];
	OPL3_SLOT *SLOT = &CH->SLOT[slot&1];

	int ksl = v >> 6;		/* 0 / 1.5 / 3.0 / 6.0 dB/OCT */

	SLOT->ksl = ksl ? 3-ksl : 31;
	SLOT->TL  = (v&0x3f)<<(ENV_BITS-1-7); /* 7 bits TL (bit 6 = always 0) */

	{
		int chan_no = slot/2;

		/* in OPL3 mode */
		//DO THIS:
		//if this is one of the slots of 1st channel forming up a 4-op channel
		//do normal operation
		//else normal 2 operator function
		//OR THIS:
		//if this is one of the slots of 2nd channel forming up a 4-op channel
		//update it using channel data of 1st channel of a pair
		//else normal 2 operator function
		switch(chan_no)
		{
		case 0: case 1: case 2:
		case 9: case 10: case 11:
			if (CH->extended)
			{
				/* normal */
				SLOT->TLL = SLOT->TL + (CH->ksl_base>>SLOT->ksl);
			}
			else
			{
				/* normal */
				SLOT->TLL = SLOT->TL + (CH->ksl_base>>SLOT->ksl);
			}
		break;
		case 3: case 4: case 5:
		case 12: case 13: case 14:
			if ((CH-3)->extended)
			{
				/* update this SLOT using frequency data for 1st channel of a pair */
				SLOT->TLL = SLOT->TL + ((CH-3)->ksl_base>>SLOT->ksl);
			}
			else
			{
				/* normal */
				SLOT->TLL = SLOT->TL + (CH->ksl_base>>SLOT->ksl);
			}
		break;
		default:
				/* normal */
				SLOT->TLL = SLOT->TL + (CH->ksl_base>>SLOT->ksl);
		break;
		}
	}
}

/* set attack rate & decay rate  */
INLINE void set_ar_dr(OPL3 *chip,int slot,int v)
{
	OPL3_CH   *CH   = &chip->P_CH[slot/2];
	OPL3_SLOT *SLOT = &CH->SLOT[slot&1];

	SLOT->ar = (v>>4)  ? 16 + ((v>>4)  <<2) : 0;

	if ((SLOT->ar + SLOT->ksr) < 16+60) /* verified on real YMF262 - all 15 x rates take "zero" time */
	{
		SLOT->eg_sh_ar  = eg_rate_shift [SLOT->ar + SLOT->ksr ];
		SLOT->eg_m_ar   = (1<<SLOT->eg_sh_ar)-1;
		SLOT->eg_sel_ar = eg_rate_select[SLOT->ar + SLOT->ksr ];
	}
	else
	{
		SLOT->eg_sh_ar  = 0;
		SLOT->eg_m_ar   = (1<<SLOT->eg_sh_ar)-1;
		SLOT->eg_sel_ar = 13*RATE_STEPS;
	}

	SLOT->dr    = (v&0x0f)? 16 + ((v&0x0f)<<2) : 0;
	SLOT->eg_sh_dr  = eg_rate_shift [SLOT->dr + SLOT->ksr ];
	SLOT->eg_m_dr   = (1<<SLOT->eg_sh_dr)-1;
	SLOT->eg_sel_dr = eg_rate_select[SLOT->dr + SLOT->ksr ];
}

/* set sustain level & release rate */
INLINE void set_sl_rr(OPL3 *chip,int slot,int v)
{
	OPL3_CH   *CH   = &chip->P_CH[slot/2];
	OPL3_SLOT *SLOT = &CH->SLOT[slot&1];

	SLOT->sl  = sl_tab[ v>>4 ];

	SLOT->rr  = (v&0x0f)? 16 + ((v&0x0f)<<2) : 0;
	SLOT->eg_sh_rr  = eg_rate_shift [SLOT->rr + SLOT->ksr ];
	SLOT->eg_m_rr   = (1<<SLOT->eg_sh_rr)-1;
	SLOT->eg_sel_rr = eg_rate_select[SLOT->rr + SLOT->ksr ];
}


/* write a value v to register r on OPL chip */
static void OPL3WriteReg(OPL3 *chip, int r, int v)
{
	OPL3_CH *CH;
	unsigned int ch_offset = 0;
	int slot;
	int block_fnum;



	if(r&0x100)
	{
		switch(r)
		{
		case 0x101:	/* test register */
			return;
		break;
		case 0x104:	/* 6 channels enable */
			{
				UINT8 prev;

				CH = &chip->P_CH[0];		/* channel 0 */
				prev = CH->extended;
				CH->extended = (v>>0) & 1;

				CH++;				/* channel 1 */
				prev = CH->extended;
				CH->extended = (v>>1) & 1;

				CH++;				/* channel 2 */
				prev = CH->extended;
				CH->extended = (v>>2) & 1;

				CH = &chip->P_CH[9];		/* channel 9 */
				prev = CH->extended;
				CH->extended = (v>>3) & 1;

				CH++;				/* channel 10 */
				prev = CH->extended;
				CH->extended = (v>>4) & 1;

				CH++;				/* channel 11 */
				prev = CH->extended;
				CH->extended = (v>>5) & 1;

			}
			return;
		break;

		default:
		break;
		}

		ch_offset = 9;	/* register page #2 starts from channel 9 (counting from 0) */
	}

	/* adjust bus to 8 bits */
	r &= 0xff;
	v &= 0xff;


	switch(r&0xe0)
	{
	case 0x00:	/* 00-1f:control */
		break;
	case 0x20:	/* am ON, vib ON, ksr, eg_type, mul */
		slot = slot_array[r&0x1f];
		if(slot < 0) return;
		set_mul(chip, slot + ch_offset*2, v);
	break;
	case 0x40:
		slot = slot_array[r&0x1f];
		if(slot < 0) return;
		set_ksl_tl(chip, slot + ch_offset*2, v);
	break;
	case 0x60:
		slot = slot_array[r&0x1f];
		if(slot < 0) return;
		set_ar_dr(chip, slot + ch_offset*2, v);
	break;
	case 0x80:
		slot = slot_array[r&0x1f];
		if(slot < 0) return;
		set_sl_rr(chip, slot + ch_offset*2, v);
	break;
	case 0xa0:
		if (r == 0xbd)			/* am depth, vibrato depth, r,bd,sd,tom,tc,hh */
		{
			if (ch_offset != 0)	/* 0xbd register is present in set #1 only */
				return;

			chip->lfo_am_depth = v & 0x80;
			chip->lfo_pm_depth_range = (v&0x40) ? 8 : 0;

			return;
		}

		/* keyon,block,fnum */
		if( (r&0x0f) > 8) return;
		CH = &chip->P_CH[(r&0x0f) + ch_offset];

		if(!(r&0x10))
		{	/* a0-a8 */
			block_fnum  = (CH->block_fnum&0x1f00) | v;
		}
		else
		{	/* b0-b8 */
			block_fnum = ((v&0x1f)<<8) | (CH->block_fnum&0xff);

			{
				int chan_no = (r&0x0f) + ch_offset;

				/* in OPL3 mode */
				//DO THIS:
				//if this is 1st channel forming up a 4-op channel
				//ALSO keyon/off slots of 2nd channel forming up 4-op channel
				//else normal 2 operator function keyon/off
				//OR THIS:
				//if this is 2nd channel forming up 4-op channel just do nothing
				//else normal 2 operator function keyon/off
				switch(chan_no)
				{
				case 0: case 1: case 2:
				case 9: case 10: case 11:
					if (CH->extended)
					{
						//if this is 1st channel forming up a 4-op channel
						//ALSO keyon/off slots of 2nd channel forming up 4-op channel
						if(v&0x20)
						{
							FM_KEYON (&CH->SLOT[SLOT1], 1);
							FM_KEYON (&CH->SLOT[SLOT2], 1);
							FM_KEYON (&(CH+3)->SLOT[SLOT1], 1);
							FM_KEYON (&(CH+3)->SLOT[SLOT2], 1);
						}
						else
						{
							FM_KEYOFF(&CH->SLOT[SLOT1],~1);
							FM_KEYOFF(&CH->SLOT[SLOT2],~1);
							FM_KEYOFF(&(CH+3)->SLOT[SLOT1],~1);
							FM_KEYOFF(&(CH+3)->SLOT[SLOT2],~1);
						}
					}
					else
					{
						//else normal 2 operator function keyon/off
						if(v&0x20)
						{
							FM_KEYON (&CH->SLOT[SLOT1], 1);
							FM_KEYON (&CH->SLOT[SLOT2], 1);
						}
						else
						{
							FM_KEYOFF(&CH->SLOT[SLOT1],~1);
							FM_KEYOFF(&CH->SLOT[SLOT2],~1);
						}
					}
				break;

				case 3: case 4: case 5:
				case 12: case 13: case 14:
					if ((CH-3)->extended)
					{
						//if this is 2nd channel forming up 4-op channel just do nothing
					}
					else
					{
						//else normal 2 operator function keyon/off
						if(v&0x20)
						{
							FM_KEYON (&CH->SLOT[SLOT1], 1);
							FM_KEYON (&CH->SLOT[SLOT2], 1);
						}
						else
						{
							FM_KEYOFF(&CH->SLOT[SLOT1],~1);
							FM_KEYOFF(&CH->SLOT[SLOT2],~1);
						}
					}
				break;

				default:
					if(v&0x20)
					{
						FM_KEYON (&CH->SLOT[SLOT1], 1);
						FM_KEYON (&CH->SLOT[SLOT2], 1);
					}
					else
					{
						FM_KEYOFF(&CH->SLOT[SLOT1],~1);
						FM_KEYOFF(&CH->SLOT[SLOT2],~1);
					}
				break;
				}
			}
		}
		/* update */
		if(CH->block_fnum != block_fnum)
		{
			UINT8 block  = block_fnum >> 10;

			CH->block_fnum = block_fnum;

			CH->ksl_base = ksl_tab[block_fnum>>6];
			CH->fc       = chip->fn_tab[block_fnum&0x03ff] >> (7-block);

			/* BLK 2,1,0 bits -> bits 3,2,1 of kcode */
			CH->kcode    = (CH->block_fnum&0x1c00)>>9;

			/* the info below is actually opposite to what is stated in the Manuals (verifed on real YMF262) */
			/* if notesel == 0 -> lsb of kcode is bit 10 (MSB) of fnum  */
			/* if notesel == 1 -> lsb of kcode is bit 9 (MSB-1) of fnum */
			CH->kcode |= (CH->block_fnum&0x200)>>9;	/* notesel == 0 */

			{
				int chan_no = (r&0x0f) + ch_offset;
				/* in OPL3 mode */
				//DO THIS:
				//if this is 1st channel forming up a 4-op channel
				//ALSO update slots of 2nd channel forming up 4-op channel
				//else normal 2 operator function keyon/off
				//OR THIS:
				//if this is 2nd channel forming up 4-op channel just do nothing
				//else normal 2 operator function keyon/off
				switch(chan_no)
				{
				case 0: case 1: case 2:
				case 9: case 10: case 11:
					if (CH->extended)
					{
						//if this is 1st channel forming up a 4-op channel
						//ALSO update slots of 2nd channel forming up 4-op channel

						/* refresh Total Level in FOUR SLOTs of this channel and channel+3 using data from THIS channel */
						CH->SLOT[SLOT1].TLL = CH->SLOT[SLOT1].TL + (CH->ksl_base>>CH->SLOT[SLOT1].ksl);
						CH->SLOT[SLOT2].TLL = CH->SLOT[SLOT2].TL + (CH->ksl_base>>CH->SLOT[SLOT2].ksl);
						(CH+3)->SLOT[SLOT1].TLL = (CH+3)->SLOT[SLOT1].TL + (CH->ksl_base>>(CH+3)->SLOT[SLOT1].ksl);
						(CH+3)->SLOT[SLOT2].TLL = (CH+3)->SLOT[SLOT2].TL + (CH->ksl_base>>(CH+3)->SLOT[SLOT2].ksl);

						/* refresh frequency counter in FOUR SLOTs of this channel and channel+3 using data from THIS channel */
						CALC_FCSLOT(CH,&CH->SLOT[SLOT1]);
						CALC_FCSLOT(CH,&CH->SLOT[SLOT2]);
						CALC_FCSLOT(CH,&(CH+3)->SLOT[SLOT1]);
						CALC_FCSLOT(CH,&(CH+3)->SLOT[SLOT2]);
					}
					else
					{
						//else normal 2 operator function
						/* refresh Total Level in both SLOTs of this channel */
						CH->SLOT[SLOT1].TLL = CH->SLOT[SLOT1].TL + (CH->ksl_base>>CH->SLOT[SLOT1].ksl);
						CH->SLOT[SLOT2].TLL = CH->SLOT[SLOT2].TL + (CH->ksl_base>>CH->SLOT[SLOT2].ksl);

						/* refresh frequency counter in both SLOTs of this channel */
						CALC_FCSLOT(CH,&CH->SLOT[SLOT1]);
						CALC_FCSLOT(CH,&CH->SLOT[SLOT2]);
					}
				break;

				case 3: case 4: case 5:
				case 12: case 13: case 14:
					if ((CH-3)->extended)
					{
						//if this is 2nd channel forming up 4-op channel just do nothing
					}
					else
					{
						//else normal 2 operator function
						/* refresh Total Level in both SLOTs of this channel */
						CH->SLOT[SLOT1].TLL = CH->SLOT[SLOT1].TL + (CH->ksl_base>>CH->SLOT[SLOT1].ksl);
						CH->SLOT[SLOT2].TLL = CH->SLOT[SLOT2].TL + (CH->ksl_base>>CH->SLOT[SLOT2].ksl);

						/* refresh frequency counter in both SLOTs of this channel */
						CALC_FCSLOT(CH,&CH->SLOT[SLOT1]);
						CALC_FCSLOT(CH,&CH->SLOT[SLOT2]);
					}
				break;

				default:
					/* refresh Total Level in both SLOTs of this channel */
					CH->SLOT[SLOT1].TLL = CH->SLOT[SLOT1].TL + (CH->ksl_base>>CH->SLOT[SLOT1].ksl);
					CH->SLOT[SLOT2].TLL = CH->SLOT[SLOT2].TL + (CH->ksl_base>>CH->SLOT[SLOT2].ksl);

					/* refresh frequency counter in both SLOTs of this channel */
					CALC_FCSLOT(CH,&CH->SLOT[SLOT1]);
					CALC_FCSLOT(CH,&CH->SLOT[SLOT2]);
				break;
				}
			}
		}
	break;

	case 0xc0:
		/* CH.D, CH.C, CH.B, CH.A, FB(3bits), C */
		if( (r&0xf) > 8) return;

		CH = &chip->P_CH[(r&0xf) + ch_offset];


		CH->SLOT[SLOT1].FB  = (v>>1)&7 ? ((v>>1)&7) + 7 : 0;
		CH->SLOT[SLOT1].CON = v&1;

		{
			int chan_no = (r&0x0f) + ch_offset;

			switch(chan_no)
			{
			case 0: case 1: case 2:
			case 9: case 10: case 11:
				if (CH->extended)
				{
					UINT8 conn = (CH->SLOT[SLOT1].CON<<1) || ((CH+3)->SLOT[SLOT1].CON<<0);
					switch(conn)
					{
					case 0:
						/* 1 -> 2 -> 3 -> 4 - out */

						CH->SLOT[SLOT1].connect = &phase_modulation;
						CH->SLOT[SLOT2].connect = &phase_modulation2;
						(CH+3)->SLOT[SLOT1].connect = &phase_modulation;
						(CH+3)->SLOT[SLOT2].connect = &chanout[ chan_no + 3 ];
					break;
					case 1:
						/* 1 -> 2 -\
						   3 -> 4 -+- out */

						CH->SLOT[SLOT1].connect = &phase_modulation;
						CH->SLOT[SLOT2].connect = &chanout[ chan_no ];
						(CH+3)->SLOT[SLOT1].connect = &phase_modulation;
						(CH+3)->SLOT[SLOT2].connect = &chanout[ chan_no + 3 ];
					break;
					case 2:
						/* 1 -----------\
						   2 -> 3 -> 4 -+- out */

						CH->SLOT[SLOT1].connect = &chanout[ chan_no ];
						CH->SLOT[SLOT2].connect = &phase_modulation2;
						(CH+3)->SLOT[SLOT1].connect = &phase_modulation;
						(CH+3)->SLOT[SLOT2].connect = &chanout[ chan_no + 3 ];
					break;
					case 3:
						/* 1 ------\
						   2 -> 3 -+- out
						   4 ------/     */
						CH->SLOT[SLOT1].connect = &chanout[ chan_no ];
						CH->SLOT[SLOT2].connect = &phase_modulation2;
						(CH+3)->SLOT[SLOT1].connect = &chanout[ chan_no + 3 ];
						(CH+3)->SLOT[SLOT2].connect = &chanout[ chan_no + 3 ];
					break;
					}
				}
				else
				{
					/* 2 operators mode */
					CH->SLOT[SLOT1].connect = CH->SLOT[SLOT1].CON ? &chanout[(r&0xf)+ch_offset] : &phase_modulation;
					CH->SLOT[SLOT2].connect = &chanout[(r&0xf)+ch_offset];
				}
			break;

			case 3: case 4: case 5:
			case 12: case 13: case 14:
				if ((CH-3)->extended)
				{
					UINT8 conn = ((CH-3)->SLOT[SLOT1].CON<<1) || (CH->SLOT[SLOT1].CON<<0);
					switch(conn)
					{
					case 0:
						/* 1 -> 2 -> 3 -> 4 - out */

						(CH-3)->SLOT[SLOT1].connect = &phase_modulation;
						(CH-3)->SLOT[SLOT2].connect = &phase_modulation2;
						CH->SLOT[SLOT1].connect = &phase_modulation;
						CH->SLOT[SLOT2].connect = &chanout[ chan_no ];
					break;
					case 1:
						/* 1 -> 2 -\
						   3 -> 4 -+- out */

						(CH-3)->SLOT[SLOT1].connect = &phase_modulation;
						(CH-3)->SLOT[SLOT2].connect = &chanout[ chan_no - 3 ];
						CH->SLOT[SLOT1].connect = &phase_modulation;
						CH->SLOT[SLOT2].connect = &chanout[ chan_no ];
					break;
					case 2:
						/* 1 -----------\
						   2 -> 3 -> 4 -+- out */

						(CH-3)->SLOT[SLOT1].connect = &chanout[ chan_no - 3 ];
						(CH-3)->SLOT[SLOT2].connect = &phase_modulation2;
						CH->SLOT[SLOT1].connect = &phase_modulation;
						CH->SLOT[SLOT2].connect = &chanout[ chan_no ];
					break;
					case 3:
						/* 1 ------\
						   2 -> 3 -+- out
						   4 ------/     */
						(CH-3)->SLOT[SLOT1].connect = &chanout[ chan_no - 3 ];
						(CH-3)->SLOT[SLOT2].connect = &phase_modulation2;
						CH->SLOT[SLOT1].connect = &chanout[ chan_no ];
						CH->SLOT[SLOT2].connect = &chanout[ chan_no ];
					break;
					}
				}
				else
				{
					/* 2 operators mode */
					CH->SLOT[SLOT1].connect = CH->SLOT[SLOT1].CON ? &chanout[(r&0xf)+ch_offset] : &phase_modulation;
					CH->SLOT[SLOT2].connect = &chanout[(r&0xf)+ch_offset];
				}
			break;

			default:
					/* 2 operators mode */
					CH->SLOT[SLOT1].connect = CH->SLOT[SLOT1].CON ? &chanout[(r&0xf)+ch_offset] : &phase_modulation;
					CH->SLOT[SLOT2].connect = &chanout[(r&0xf)+ch_offset];
			break;
			}
		}
	break;

	case 0xe0: /* waveform select */
		slot = slot_array[r&0x1f];
		if(slot < 0) return;

		slot += ch_offset*2;

		CH = &chip->P_CH[slot/2];

		/* store 3-bit value written regardless of current OPL2 or OPL3 mode... (verified on real YMF262) */
		v &= 7;
		CH->SLOT[slot&1].waveform_number = v;

		CH->SLOT[slot&1].wavetable = v * SIN_LEN;
	break;
	}
}


static void OPL3ResetChip(OPL3 *chip)
{
	int c,s;

	chip->eg_timer = 0;
	chip->eg_cnt   = 0;

	chip->noise_rng = 1;	/* noise shift register */

	/* reset with register write */
	OPL3WriteReg(chip,0x01,0); /* test register */
	OPL3WriteReg(chip,0x02,0); /* Timer1 */
	OPL3WriteReg(chip,0x03,0); /* Timer2 */
	OPL3WriteReg(chip,0x04,0); /* IRQ mask clear */


//FIX IT  registers 101, 104 and 105


//FIX IT (dont change CH.D, CH.C, CH.B and CH.A in C0-C8 registers)
	for(c = 0xff ; c >= 0x20 ; c-- )
		OPL3WriteReg(chip,c,0);
//FIX IT (dont change CH.D, CH.C, CH.B and CH.A in C0-C8 registers)
	for(c = 0x1ff ; c >= 0x120 ; c-- )
		OPL3WriteReg(chip,c,0);



	/* reset operator parameters */
	for( c = 0 ; c < 9*2 ; c++ )
	{
		OPL3_CH *CH = &chip->P_CH[c];
		for(s = 0 ; s < 2 ; s++ )
		{
			CH->SLOT[s].state     = EG_OFF;
			CH->SLOT[s].volume    = MAX_ATT_INDEX;
		}
	}
}

/* Create one of virtual YMF262 */
/* 'clock' is chip clock in Hz  */
/* 'rate'  is sampling rate  */
static OPL3 *OPL3Create(int clock, int rate)
{
	OPL3 *chip;

	init_tables();

	/* allocate memory block */
	chip = (OPL3 *)malloc(sizeof(OPL3));

	if (chip==NULL)
		return NULL;

	/* clear */
	memset(chip, 0, sizeof(OPL3));

	//chip->type  = type;
	chip->clock = clock;
	chip->rate  = rate;

	/* init global tables */
	OPL3_initalize(chip);

	/* reset chip */
	OPL3ResetChip(chip);
	return chip;
}

/* Destroy one of virtual YMF262 */
static void OPL3Destroy(OPL3 *chip)
{
	free(chip);
}


/* YMF262 I/O interface */
static int OPL3Write(OPL3 *chip, int a, int v)
{
	/* data bus is 8 bits */
	v &= 0xff;

	switch(a&3)
	{
	case 0:	/* address port 0 (register set #1) */
		chip->address = v;
	break;

	case 1:	/* data port - ignore A1 */
	case 3:	/* data port - ignore A1 */
		//if(chip->UpdateHandler) chip->UpdateHandler(chip->UpdateParam,0);
		OPL3WriteReg(chip,chip->address,v);
	break;

	case 2:	/* address port 1 (register set #2) */

		/* verified on real YMF262:
		 in OPL3 mode:
		   address line A1 is stored during *address* write and ignored during *data* write.

		 in OPL2 mode:
		   register set#2 writes go to register set#1 (ignoring A1)
		   verified on registers from set#2: 0x01, 0x04, 0x20-0xef
		   The only exception is register 0x05.
		*/
		{
			/* OPL3 mode */
				chip->address = v | 0x100;
		}
	break;
	}

	return 0;
}




#define MAX_OPL3_CHIPS 4

static OPL3 *YMF262[MAX_OPL3_CHIPS];	/* array of pointers to the YMF262's */
static int YMF262NumChips = 0;				/* number of chips */

int YMF262Init(int num, int clock, int rate)
{
	int i;

	if (YMF262NumChips)
		return -1;	/* duplicate init. */

	YMF262NumChips = num;

	for (i = 0;i < YMF262NumChips; i++)
	{
		/* emulator create */
		YMF262[i] = OPL3Create(clock,rate);
		if(YMF262[i] == NULL)
		{
			/* it's really bad - we run out of memeory */
			YMF262NumChips = 0;
			return -1;
		}
	}

	return 0;
}

void YMF262Shutdown(void)
{
	int i;

	for (i = 0;i < YMF262NumChips; i++)
	{
		/* emulator shutdown */
		OPL3Destroy(YMF262[i]);
		YMF262[i] = NULL;
	}
	YMF262NumChips = 0;
}
void YMF262ResetChip(int which)
{
	OPL3ResetChip(YMF262[which]);
}

int YMF262Write(int which, int a, int v)
{
	return OPL3Write(YMF262[which], a, v);
}



/* Added for MMFplay */

void YMF262UpdateOne(int which, INT16 *ch_a, int length, int reset_buffer)
{
	OPL3		*chip  = YMF262[which];
	int i, c;

	if( (void *)chip != cur_chip ){
		cur_chip = (void *)chip;
	}

	for( i=0; i < length ; i++ )
	{
		int a;

		advance_lfo(chip);

		/* clear channel outputs */
		memset(chanout, 0, sizeof(signed int) * FM_CHANNELS);

#if 1
	/* register set #1 */
		chan_calc(&chip->P_CH[0]);			/* extended 4op ch#0 part 1 or 2op ch#0 */
		if (chip->P_CH[0].extended)
			chan_calc_ext(&chip->P_CH[3]);	/* extended 4op ch#0 part 2 */
		else
			chan_calc(&chip->P_CH[3]);		/* standard 2op ch#3 */


		chan_calc(&chip->P_CH[1]);			/* extended 4op ch#1 part 1 or 2op ch#1 */
		if (chip->P_CH[1].extended)
			chan_calc_ext(&chip->P_CH[4]);	/* extended 4op ch#1 part 2 */
		else
			chan_calc(&chip->P_CH[4]);		/* standard 2op ch#4 */


		chan_calc(&chip->P_CH[2]);			/* extended 4op ch#2 part 1 or 2op ch#2 */
		if (chip->P_CH[2].extended)
			chan_calc_ext(&chip->P_CH[5]);	/* extended 4op ch#2 part 2 */
		else
			chan_calc(&chip->P_CH[5]);		/* standard 2op ch#5 */


		chan_calc(&chip->P_CH[6]);
		chan_calc(&chip->P_CH[7]);
		chan_calc(&chip->P_CH[8]);

	/* register set #2 */
		chan_calc(&chip->P_CH[ 9]);
		if (chip->P_CH[9].extended)
			chan_calc_ext(&chip->P_CH[12]);
		else
			chan_calc(&chip->P_CH[12]);


		chan_calc(&chip->P_CH[10]);
		if (chip->P_CH[10].extended)
			chan_calc_ext(&chip->P_CH[13]);
		else
			chan_calc(&chip->P_CH[13]);


		chan_calc(&chip->P_CH[11]);
		if (chip->P_CH[11].extended)
			chan_calc_ext(&chip->P_CH[14]);
		else
			chan_calc(&chip->P_CH[14]);


        /* channels 15,16,17 are fixed 2-operator channels only */
		chan_calc(&chip->P_CH[15]);
		chan_calc(&chip->P_CH[16]);
		chan_calc(&chip->P_CH[17]);
#endif

		/* accumulator register set #1 */
		a =  chanout[0];
		for (c = 1; c < FM_CHANNELS; c++)
			a += chanout[c];

		/* limit check */
		a = limit( a , MAXOUT, MINOUT );

		/* store to sound buffer */
		if (reset_buffer)
			*ch_a = a;
		else
			*ch_a += a;

		ch_a++;


		advance(chip);
	}

}


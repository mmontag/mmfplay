
#ifndef __SBI_H
#define __SBI_H

#define SBI_INS_2OP	0
#define SBI_INS_4OP	1

struct instrument_2op {
	unsigned char m_flg_mul;
	unsigned char c_flg_mul;
	unsigned char m_ksl_tl;
	unsigned char c_ksl_tl;
	unsigned char m_ar_dr;
	unsigned char c_ar_dr;
	unsigned char m_sl_rr;
	unsigned char c_sl_rr;
	unsigned char m_ws;
	unsigned char c_ws;
	unsigned char fb_alg;
};

struct sbi {
	char magic[4];
	char name[29];
	char fix_dur;
	char modes;			
	char dpitch;
	struct instrument_2op A;
	struct instrument_2op B;
#if 0
	char res[2];
	char echo_delay;		/* Extended attributes from sbiload */
	char echo_atten;
	char chorus_spread;
	char trans;
	char fix_dur;
	char modes;
	char fix_key;
#endif
};

#define SBI_2OP 0
#define SBI_4OP 1
#define SBI_2OP_SIZE 42
#define SBI_4OP_SIZE 60


struct ibk {
	struct instrument_2op i;
	unsigned char percvoc;	/* Percussion voice number */
        char transpos;		/* Number of notes to transpose timbre */
        unsigned char dpitch;	/* percussion pitch: MIDI Note 0 - 127 */
        unsigned char rsv[2];	/* unsused - so far */
};

#endif



#ifndef __OPL3_H
#define __OPL3_H


#define OPL3_VOICES 6

struct opl3_operator {
	unsigned char flg_mul;
	unsigned char ksl_tl;
	unsigned char ar_dr;
	unsigned char sl_rr;
	unsigned char ws;
};

struct opl3_instrument {
	int type;
	struct opl3_operator op[4];
	unsigned char fb_algA;
	unsigned char fb_algB;
};

#define OPL3_TYPE_2OP		0
#define OPL3_TYPE_4OP		1

#define OPL3_REG_OP_FLG_MUL	0x20
#define OPL3_REG_OP_KSL_TL	0x40
#define OPL3_REG_OP_AR_DR	0x60
#define OPL3_REG_OP_SL_RR	0x80
#define OPL3_REG_OP_WS		0xe0

#define OPL3_REG_CH_FREQ	0xa0
#define OPL3_REG_CH_KEY_BLOCK	0xb0
#define OPL3_REG_FB_ALG		0xc0

#define OPL3_BASE		0x220

extern struct opl3_instrument opl3_ins[128];

#endif


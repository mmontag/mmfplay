
#ifndef __SBI_H
#define __SBI_H

#define SBI_INS_2OP	0
#define SBI_INS_4OP	1

struct sbi_ins {
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
	char name[32];
	struct sbi_ins A;
	struct sbi_ins B;
};

#define SBI_2OP 0
#define SBI_4OP 1
#define SBI_2OP_SIZE 42
#define SBI_4OP_SIZE 60


#endif


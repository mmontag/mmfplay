
#ifndef __SMAF_H
#define __SMAF_H

#include "smaf.h"

#define CRCPOLY  0x1021U 

unsigned char *read_varlen(unsigned char *buf, int *ret, int *size);
unsigned int get_crc(unsigned char *buf, int size);
int read32_be(unsigned char *buffer);
int read16_be(unsigned char *buffer);

struct ma3_operator {
	int ar;
	int dr;
	int sl;
	int rr;
	int tl;
	int ksl;
	int ksr;
	int am;
	int vi;
	int amc;
	int vic;
	int mul;
	int det;
	int ws;
	int fb;
};

struct ma3_instrument {
	int pan;
	int oct;
	int alg;
	struct ma3_operator op[4];
};


#define TRK_STATUS_PERC		(1 << 6)	/* Percussive channel */

#endif


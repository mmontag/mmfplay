
#ifndef __MMFPLAY_H
#define __MMFPLAY_H

struct options {
#define MMFPLAY_PLAY	0
#define MMFPLAY_SHOW	1
	int mode;
};

struct mmfplay {
	int time_d;
	int time_g;
};

extern char *optarg;
extern struct options opt;
extern struct mmfplay mmf;

typedef char SINT8;
typedef unsigned char UINT8;
typedef short SINT16;
typedef unsigned short UINT16;
typedef int SINT32;
typedef unsigned int UINT32;

typedef SINT8 INT8;
typedef SINT16 INT16;
typedef SINT32 INT32;

extern char *gm_ins[128];

unsigned char *read_chunk(unsigned char *, int);
unsigned char *read_chunk16(unsigned char *, int);

void play_midi(unsigned char *, int, unsigned char *);
int parse_cli(int, char **);

#define cmp4(x,a,b,c,d) (x[0]==(a) && x[1]==(b) && x[2]==(c) && x[3]==(d))

#endif



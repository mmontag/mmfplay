
#include "smaf.h"

unsigned char *read_varlen(unsigned char *buf, int *ret, int *size)
{
	int i, r, s;

	s = r = 0;

	while (1) {
		i = *buf++;
		r += i & 0x7f;
		s++;

		if (i & 0x80)
			r = (r /*+ 1*/) << 7;
		else
			break;
	}

	*ret = r;
	*size = s;

	return buf;
}


unsigned int get_crc(unsigned char *buf, int size)
{
	int i, j; 
	int crc = 0;

	for (i = 0; i < size; i++) {
		crc ^= (buf[i] << 8); 	
		for (j = 0; j < 8; j++)
 			if (crc & 0x8000U)
				crc = (crc << 1) ^ CRCPOLY;
 			else
				crc <<= 1;
	}

 	return ~crc;
}


int read16_be(unsigned char *buffer)
{
	int i;

	i = *(unsigned char *)buffer++;
	i = (i << 8) + *buffer++;

	return i;
}

int read32_be(unsigned char *buffer)
{
	int i;

	i = *(unsigned char *)buffer++;
	i = (i << 8) + *buffer++;
	i = (i << 8) + *buffer++;
	i = (i << 8) + *buffer++;

	return i;
}


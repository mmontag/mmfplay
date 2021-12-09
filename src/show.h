
#ifndef __SHOW_H
#define __SHOW_H

void show_mmmd(unsigned char *buf, int size);
void show_cnti(unsigned char *buf, int size);
void show_opda(unsigned char *buf, int size);
void show_dchx(unsigned char *buf, int size);
void show_mtrx(unsigned char *buf, int size);
void show_mtsu(unsigned char *buf, int size);
void show_mspi(unsigned char *buf, int size);
void show_mtsq(unsigned char *buf, int size);
void show_mstr(unsigned char *buf, int size);

void show_ca(unsigned char *buf, int size);
void show_cr(unsigned char *buf, int size);
void show_st(unsigned char *buf, int size);
void show_es(unsigned char *buf, int size);
void show_as(unsigned char *buf, int size);
void show_a0(unsigned char *buf, int size);
void show_a2(unsigned char *buf, int size);

void show_unhandled(unsigned char *buffer, int size);

#endif

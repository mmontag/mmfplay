
#ifndef __PLAY_H
#define __PLAY_H

void play_mmmd(unsigned char *buf, int size);
void play_opda(unsigned char *buf, int size);
void play_dchx(unsigned char *buf, int size);
void play_mtrx(unsigned char *buf, int size);
void play_mtsq(unsigned char *buf, int size);
#if 0
void play_cnti(unsigned char *buf, int size);
void play_mtsu(unsigned char *buf, int size);
void play_mspi(unsigned char *buf, int size);
void play_mstr(unsigned char *buf, int size);
#endif

void play_text(unsigned char *buf, int size);

void play_unhandled(unsigned char *buffer, int size);

#endif

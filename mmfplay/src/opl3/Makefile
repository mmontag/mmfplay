# $Id: Makefile,v 1.7 2004/07/02 23:59:12 cmatsuoka Exp $

TOPDIR	= ../..

XCFLAGS	= -DHAS_YMF262 -I..
XDEPS	= readsbi.c readibk.c
OBJS	= play_opl3.o play_opl2.o ymf262.o std_o3.o
XOBJS	= gm_ibk.o drum_ibk.o
DFILES	= Makefile $(OBJS:.o=.c) $(XDEPS) ymf262.h opl3.h sbi.h \
	  gm.ibk gmopl.ibk drum.ibk
CFILES	= $(LIB) readsbi readibk
DCFILES	=
DDIRS	=
LIBS	=
LIB	= libopl3.a
BIN	=


all: $(LIB)

readsbi: readsbi.o
	$(LD) $(LDFLAGS) -o $@ $+ -lm

readibk: readibk.o
	$(LD) $(LDFLAGS) -o $@ $+ -lm

opled: opled.c
	$(LD) $(LDFLAGS) -o $@ $+ `pkg-config --cflags --libs gtk+-2.0`

gm_ibk.c: readibk Makefile
	./readibk opl2_ins gm.ibk > $@

drum_ibk.c: readibk Makefile
	./readibk opl2_drum drum.ibk > $@

include $(TOPDIR)/Make.rules


# $Id: Makefile,v 1.5 2004/07/02 02:12:29 cmatsuoka Exp $

TOPDIR	= ../..

XCFLAGS	= -DHAS_YMF262 -I..
XDEPS	= readsbi.c
OBJS	= play_opl3.o play_opl2.o ymf262.o std_o3.o std_o2.o
DFILES	= Makefile $(OBJS:.o=.c) $(XDEPS) ymf262.h opl3.h sbi.h
CFILES	= $(LIB) readsbi
DCFILES	=
DDIRS	=
LIBS	=
LIB	= libopl3.a
BIN	=


all: $(LIB)

readsbi: readsbi.o
	$(LD) $(LDFLAGS) -o $@ $+ -lm

opled: opled.c
	$(LD) $(LDFLAGS) -o $@ $+ `pkg-config --cflags --libs gtk+-2.0`

include $(TOPDIR)/Make.rules


# $Id: Makefile,v 1.4 2004/07/01 23:50:05 cmatsuoka Exp $

TOPDIR	= ../..

XCFLAGS	= -DHAS_YMF262 -I..
XDEPS	= readsbi.c
OBJS	= play_opl3.o ymf262.o std_o3.o
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


# $Id: Makefile,v 1.2 2004/06/29 17:37:54 cmatsuoka Exp $

TOPDIR	= ../..

XCFLAGS	= -DHAS_YMF262 -I..
XDEPS	= readsbi.c
OBJS	= play_opl3.o ymf262.o std_o3.o
DFILES	= Makefile $(OBJS:.o=.c) $(XDEPS) ymf262.h opl3.h sbi.h
CFILES	= $(LIB) test
DCFILES	=
DDIRS	=
LIBS	=
LIB	= libopl3.a
BIN	= readsbi


all: $(LIB)

readsbi: readsbi.o
	$(LD) $(LDFLAGS) -o$@ $+ $(LIBS)

include $(TOPDIR)/Make.rules


# $Id: Makefile,v 1.1 2004/06/29 13:16:44 cmatsuoka Exp $

TOPDIR	= ../..

XCFLAGS	= -DHAS_YMF262 -I..
OBJS	= play_opl3.o ymf262.o std_o3.o
DFILES	= Makefile $(OBJS:.o=.c) ymf262.h opl3.h sbi.h readsbi.c
CFILES	= $(LIB) test
DCFILES	=
DDIRS	=
LIBS	=
LIB	= libopl3.a
BIN	= readsbi


all: $(LIB)

readsbi: readsbi.o
	$(LD) $(LDFLAGS) -o$@ $+ $(LIBS)

readsbi.o: readsbi.c sbi.h

include $(TOPDIR)/Make.rules


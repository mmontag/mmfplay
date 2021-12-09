# $Id: Makefile,v 1.2 2004/07/06 20:55:57 cmatsuoka Exp $

TOPDIR	= ../..

XCFLAGS	= -I.. -UUNROLL
XDEPS	=
OBJS	= fm.o play_fm.o std_o3.o
XOBJS	=
DFILES	= Makefile $(OBJS:.o=.c)
CFILES	= $(LIB)
DCFILES	=
DDIRS	=
LIBS	=
LIB	= libfm.a
BIN	=


all: $(LIB)

include $(TOPDIR)/Make.rules


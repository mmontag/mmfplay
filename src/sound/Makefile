# $Id: Makefile,v 1.3 2004/07/01 02:26:13 cmatsuoka Exp $

TOPDIR	= ../..

XCFLAGS	= -I..
OBJS	= sound_$(SOUND_DRIVER).o
DFILES	= Makefile README sound_ahi.c sound_ahi_hilevel.c \
	  sound_alsa.c sound_macos.c sound_bsd.c sound_dart.c \
	  sound_freebsd.c sound_hpux.c sound_oss.c sound_qnx.c \
	  sound_sdl.c sound_sgi.c sound_solaris.c sound_win32.c
CFILES	= $(LIB)
DCFILES	=
DDIRS	=
LIBS	=
LIB	= libsound.a
BIN	=


all: lib

lib: $(LIB)

include $(TOPDIR)/Make.rules


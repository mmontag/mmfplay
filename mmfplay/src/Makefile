# $Id: Makefile,v 1.16 2004/07/06 09:01:29 cmatsuoka Exp $

TOPDIR	= ..

XCFLAGS	= -DVERSION=\"$(VERSION)\" -DUSERNAME=\"$(USERNAME)\" \
	  -DSOUND_DRIVER=\"$(SOUND_DRIVER)\"
OBJS	= show.o play.o smaf.o tree.o chunk.o main.o gmmap.o sound.o \
	  playmidi.o sequencer.o options.o getopt_long.o
DFILES	= Makefile $(OBJS:.o=.c) chunk.h list.h mmfplay.h play.h \
	  show.h smaf.h sound.h tree.h sequencer.h device.h getopt.h
CFILES	= $(BIN)
DCFILES	=
DDIRS	= sound fm opl3 buzzer
LIBS	= -Lfm -lfm -Lsound -lsound -lm
BIN	= mmfplay


all: subdirs $(BIN)

$(BIN): fm/libfm.a

include $(TOPDIR)/Make.rules


# $Id: Makefile,v 1.14 2004/07/06 02:16:34 cmatsuoka Exp $

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
LIBS	= -Lopl3 -lopl3 -Lfm -lfm -Lbuzzer -lbuzzer -Lsound -lsound -lm
BIN	= mmfplay


all: subdirs $(BIN)

$(BIN): sound/libsound.a opl3/libopl3.a

include $(TOPDIR)/Make.rules


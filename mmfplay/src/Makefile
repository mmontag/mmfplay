# $Id: Makefile,v 1.7 2004/06/30 17:56:22 cmatsuoka Exp $

TOPDIR	= ..

XCFLAGS	= -DVERSION=\"$(VERSION)\" -DUSERNAME=\"$(USERNAME)\"
OBJS	= show.o play.o smaf.o tree.o chunk.o main.o gmmap.o sound.o \
	  playmidi.o sequencer.o options.o getopt_long.o
DFILES	= Makefile $(OBJS:.o=.c) chunk.h list.h mmfplay.h play.h \
	  show.h smaf.h sound.h tree.h sequencer.h device.h getopt.h
CFILES	=
DCFILES	=
DDIRS	= sound opl3 buzzer
LIBS	= -Lopl3 -lopl3 -Lbuzzer -lbuzzer -Lsound -lsound -lm
BIN	= mmfplay


all: subdirs $(BIN)

$(BIN): $(OBJS)
	$(LD) -o$@ $+ $(LIBS)

$(BIN): sound/libsound.a opl3/libopl3.a

include $(TOPDIR)/Make.rules


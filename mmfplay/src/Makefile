# $Id: Makefile,v 1.4 2004/06/30 13:28:39 cmatsuoka Exp $

TOPDIR	= ..

XCFLAGS	= -I. -DVERSION=\"$(VERSION)\" -DUSERNAME=\"$(USERNAME)\"
OBJS	= show.o play.o smaf.o tree.o chunk.o main.o gmmap.o sound.o \
	  playmidi.o sequencer.o mixer.o options.o getopt_long.o
DFILES	= Makefile $(OBJS:.o=.c) chunk.h list.h mixer.h mmfplay.h play.h \
	  show.h smaf.h sound.h tree.h sequencer.h device.h getopt.h
CFILES	=
DCFILES	=
DDIRS	= sound opl3
LIBS	= -Lopl3 -lopl3 -Lsound -lsound -lm
BIN	= mmfplay


all: subdirs $(BIN)

$(BIN): $(OBJS)
	$(LD) -o$@ $+ $(LIBS)

$(BIN): sound/libsound.a opl3/libopl3.a

include $(TOPDIR)/Make.rules


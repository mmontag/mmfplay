#!/bin/sh
#
# Poor man's autoconf
# $Id: genconf.sh,v 1.3 2004/07/01 10:58:32 cmatsuoka Exp $

OS=`uname -s`

case $OS in
*Linux)
	SOUND=oss
	CFLAGS="-DHAS_GETOPT_LONG"
	;;
SunOS)
	SOUND=solaris
	;;
FreeBSD)
	SOUND=freebsd
	CFLAGS="-DHAS_GETOPT_LONG"
	;;
NetBSD|OpenBSD)
	SOUND=bsd
	CFLAGS="-DHAS_GETOPT_LONG"
	;;
Darwin)
	SOUND=macos
	CC=cc
	LD=cc
	CFLAGS="-no-cpp-precomp -DMACOSX"
	LDFLAGS="-flat_namespace -framework Carbon"
	LIBS="-lobjc"
	;;
*)
	SOUND=`uname -s|tr A-Z a-z`;;
esac

test -n "$CC" && echo "CC = $CC"
test -n "$LD" && echo "LD = $LD"
echo "SOUND_DRIVER = $SOUND"
echo "AUTO_CFLAGS = $CFLAGS"
echo "AUTO_LDFLAGS = $LDFLAGS"
echo "AUTO_LIBS = $LIBS"
echo "USERNAME = `whoami`@`hostname`"

#!/bin/sh
#
# $Id: genconf.sh,v 1.2 2004/07/01 02:19:12 cmatsuoka Exp $

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

if [ -n "$CC" ]; then
	echo "CC = $CC"
fi
if [ -n "$LD" ]; then
	echo "LD = $LD"
fi
echo "SOUND_DRIVER = $SOUND"
echo "XXCFLAGS = $CFLAGS"
echo "XXLDFLAGS = $LDFLAGS"
echo "XXLIBS = $LIBS"
echo "USERNAME = `whoami`@`hostname`"

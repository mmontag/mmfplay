#!/bin/sh
#
# $Id: genconf.sh,v 1.1 2004/06/30 15:18:03 cmatsuoka Exp $

OS=`uname -s`

case $OS in
*Linux)
	SOUND=oss
	FLAGS="-DHAS_GETOPT_LONG"
	;;
SunOS)
	SOUND=solaris
	;;
FreeBSD)
	SOUND=freebsd
	FLAGS="-DHAS_GETOPT_LONG"
	;;
NetBSD|OpenBSD)
	SOUND=bsd
	FLAGS="-DHAS_GETOPT_LONG"
	;;
*)
	SOUND=`uname -s|tr A-Z a-z`;;
esac

echo "SOUND_DRIVER = $SOUND"
echo "XXCFLAGS = $FLAGS"
echo "USERNAME = `whoami`@`hostname`"

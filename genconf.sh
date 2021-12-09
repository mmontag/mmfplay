#!/bin/sh
#
# Poor man's autoconf
# $Id: genconf.sh,v 1.6 2004/07/01 11:35:47 cmatsuoka Exp $

OS=`uname -s`
TMPOUT=tmpout.$$

has_header() {
	echo "#include <$1>"|cpp > $TMPOUT
	return $?
}

case $OS in
*Linux)
	SOUND=oss
	;;
SunOS)
	SOUND=solaris
	;;
FreeBSD)
	SOUND=freebsd
	;;
NetBSD|OpenBSD)
	SOUND=bsd
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

has_header "getopt.h" && CFLAGS="$CFLAGS -DHAS_GETOPT_LONG"

rm -f $TMPOUT

test -n "$CC" && echo "CC = $CC"
test -n "$LD" && echo "LD = $LD"

cat <<EOF
# generated with `basename $0` \$Revision: 1.6 $
# on `uname -srm` (`uname -n`)
# `date`
SOUND_DRIVER = $SOUND
AUTO_CFLAGS = $CFLAGS
AUTO_LDFLAGS = $LDFLAGS
AUTO_LIBS = $LIBS
USERNAME = `whoami`@`hostname`
EOF


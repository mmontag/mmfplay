# $Id: Makefile,v 1.4 2004/06/30 15:18:03 cmatsuoka Exp $

TOPDIR	= .

PKGNAME	= mmfplay
DIST	= $(PKGNAME)-$(VERSION)
DFILES	= Makefile Make.rules Version README genconf.sh
CFILES	=
DDIRS	= doc src


all:
	for i in $(DDIRS); do \
		(cd $$i; [ -f Makefile ] && $(MAKE) $@) \
	done; true

dist:
	rm -Rf $(DIST) $(DIST).tar.gz
	mkdir $(DIST)
	$(MAKE) DISTDIR=$(DIST) subdist
	chmod -R u+w $(DIST)/*
	tar cvf - $(DIST) | gzip -9c > $(DIST).tar.gz
	(cd $(DIST); make; test -x src/mmfplay)
	rm -Rf $(DIST)
	sync
	ls -l $(DIST).tar.gz

include Make.rules

clean::
	rm Make.conf

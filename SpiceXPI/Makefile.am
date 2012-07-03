SUBDIRS=src

XPI=SpiceXPI.xpi

EXTRA_DIST = logger.ini
spicedatadir = $(datadir)/spice
spicedata_DATA = logger.ini

DISTDIR=dist
ZIP=zip

all-local: SpiceXPI.xpi

CLEANFILES = SpiceXPI.xpi

SpiceXPI.xpi: $(top_builddir)/SpiceXPI/src/install.rdf $(top_builddir)/SpiceXPI/src/plugin/nsISpicec.xpt $(top_builddir)/SpiceXPI/src/plugin/.libs/libnsISpicec.so
	rm -rf $(DISTDIR)
	@[ -d $(DISTDIR)/plugins ] || mkdir -p $(DISTDIR)/plugins
	cp $(top_srcdir)/SpiceXPI/src/install.rdf $(DISTDIR)
	cp $(top_builddir)/SpiceXPI/src/plugin/*.xpt $(DISTDIR)/plugins
	cp $(top_builddir)/SpiceXPI/src/plugin/.libs/libnsISpicec.so* $(DISTDIR)/plugins/nsISpicec.so
	cp $(top_srcdir)/SpiceXPI/logger.ini $(DISTDIR)/plugins/logger.ini
	(cd $(DISTDIR); $(ZIP) -r ../$(XPI) .)

distclean-local:
	rm -rf $(DISTDIR)

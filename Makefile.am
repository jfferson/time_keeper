## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

dist_doc_DATA = \
	README.md \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

pkgdata_DATA = time_keeper.dat

INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = \
	$(INTLTOOL_FILES) $(pkgdata_DATA)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)

depend:
	makedepend $(AM_CPPFLAGS) $(AM_CFLAGS) $(time_keeper_SOURCES) $(TIME_KEEPER_LIBS)

time_keeper.dat:
	touch time_keeper.dat

install-data-hook:
	dd if=/dev/null of=$(pkgdatadir)/time_keeper.dat
	chmod o+w $(pkgdatadir)/time_keeper.dat

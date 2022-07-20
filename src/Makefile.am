## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = time_keeper.ui \
	n_time.ui \
	n_counter.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(gtkmm-3.0_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(TIME_KEEPER_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = time_keeper

time_keeper_SOURCES = \
	main.cc \
	activity-display.cc \
	activity-display.h \
	time-keeper.cc \
	time-keeper.h \
	u-i-controller.cc \
	u-i-controller.h

time_keeper_LDFLAGS = -lskarnet 

time_keeper_LDADD = $(TIME_KEEPER_LIBS) \
	$(gtkmm-3.0_LIBS)




EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)


## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = time_keeper.ui \
	n_time.ui \
	n_counter.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(TIME_KEEPER_CFLAGS) -std=c++20

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = time_keeper

time_keeper_SOURCES = \
	main.cc \
 	time-keeper-application.cc \
 	time-keeper-application.h \
 	time-keeper-window.cc \
 	time-keeper-window.h \
 	u-i-controller.cc \
 	u-i-controller.h \
 	time-keeper.cc \
 	time-keeper.h \
 	time-serializer.cc \
 	zpp/zpp_bits.h
 	

# time_keeper_LDFLAGS = -lskarnet 

time_keeper_LDADD = $(TIME_KEEPER_LIBS)




EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

depend:
	makedepend $(AM_CPPFLAGS) $(AM_CFLAGS) $(time_keeper_SOURCES) $(TIME_KEEPER_LIBS)

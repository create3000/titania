## Process this file with automake to produce Makefile.in

current_dir = $(shell pwd)

bin_PROGRAMS =  \
	experimental

AM_CPPFLAGS = $(TITANIA_EXPERIMENTAL_CFLAGS) \
	$(TITANIA_STREAM_CFLAGS) \
	$(TITANIA_X3D_CFLAGS)  \
	-I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream \
	-I$(top_builddir)/libtitania-x3d

experimental_LDADD = $(TITANIA_EXPERIMENTAL_LIBS) \
	$(TITANIA_STREAM_LIBS) \
	$(TITANIA_X3D_LIBS)  \
	$(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la \
	$(top_builddir)/libtitania-x3d/libtitania-x3d.la

experimental_SOURCES = $(current_dir)/main.cpp

touch:
	touch main.cpp

run:
	$(builddir)/run.pl 2>&1

x3d:
	cd ../libtitania-x3d; \
		make -j4; \
		cd ../experimental; \
		make; \
		make run;

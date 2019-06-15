## Process this file with automake to produce Makefile.in

current_dir = $(shell pwd)

bin_PROGRAMS =  \
	titania-experimental

AM_CPPFLAGS = $(TITANIA_EXPERIMENTAL_CFLAGS) \
	$(TITANIA_STREAM_CFLAGS) \
	$(TITANIA_X3D_CFLAGS)  \
	-I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream \
	-I$(top_builddir)/libtitania-x3d

titania_experimental_LDADD = $(TITANIA_EXPERIMENTAL_LIBS) \
	$(TITANIA_STREAM_LIBS) \
	$(TITANIA_X3D_LIBS)  \
	$(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la \
	$(top_builddir)/libtitania-x3d/libtitania-x3d.la

titania_experimental_SOURCES = $(current_dir)/main.cpp

run:
	$(builddir)/$(bin_PROGRAMS)

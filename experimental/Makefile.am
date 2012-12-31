## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	experimental
	
AM_CPPFLAGS = -I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream \
	-I$(top_builddir)/libtitania-gzstream

AM_LDFLAGS = -lv8

experimental_LDADD = $(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la \
	$(top_builddir)/libtitania-gzstream/libtitania-gzstream.la

experimental_SOURCES = main.cpp

all:
	mkdir -p $(builddir)/bin/$(CONFIGURATION)
	cp $(builddir)/*.o $(builddir)/bin/$(CONFIGURATION)
	cp $(builddir)/.libs/* $(builddir)/bin/$(CONFIGURATION)

run:
	$(builddir)/$(bin_PROGRAMS)


## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	titania-experimental
	
AM_CPPFLAGS = -I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream \
	-I$(top_builddir)/libtitania-gzstream

AM_LDFLAGS = -lv8 -lcurl

titania_experimental_LDADD = $(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la \
	$(top_builddir)/libtitania-gzstream/libtitania-gzstream.la

titania_experimental_SOURCES = main.cpp

run:
	$(builddir)/$(bin_PROGRAMS)


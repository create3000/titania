## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	experimental
	
AM_CPPFLAGS = -I$(top_builddir)/libtitania-standard

experimental_LDADD = $(top_builddir)/libtitania-standard/libtitania-standard.la

experimental_SOURCES = main.cpp

run:
	$(builddir)/$(bin_PROGRAMS)


## Process this file with automake to produce Makefile.in

current_dir = $(shell pwd)

lib_LTLIBRARIES =  \
	libtitania-stream.la

libtitania_stream_la_SOURCES =  \
	$(current_dir)/Titania/Stream/GZFilterBuf.cpp \
	$(current_dir)/Titania/Stream/GZStream.cpp \
	$(current_dir)/Titania/Stream/InputFileStream.cpp \
	$(current_dir)/Titania/Stream/InputUrlStream.cpp \
	$(current_dir)/Titania/Stream/UrlStreamBuf.cpp

libtitania_stream_la_LIBADD = $(TITANIA_STREAM_LIBS) \
	$(top_builddir)/libtitania-standard/libtitania-standard.la

AM_CPPFLAGS = $(TITANIA_STREAM_CFLAGS) \
	-I$(top_builddir)/libtitania-standard

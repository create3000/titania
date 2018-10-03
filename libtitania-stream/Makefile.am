## Process this file with automake to produce Makefile.in


lib_LTLIBRARIES =  \
	libtitania-stream.la

libtitania_stream_la_SOURCES =  \
	Titania/Stream/GZFilterBuf.cpp \
	Titania/Stream/GZStream.cpp \
	Titania/Stream/InputFileStream.cpp \
	Titania/Stream/InputUrlStream.cpp \
	Titania/Stream/UrlStreamBuf.cpp

libtitania_stream_la_LIBADD = $(TITANIA_STREAM_LIBS) \
	$(top_builddir)/libtitania-standard/libtitania-standard.la

AM_CPPFLAGS = $(TITANIA_STREAM_CFLAGS) \
	-I$(top_builddir)/libtitania-standard


## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	x3dtidy

x3dtidy_SOURCES = main.cpp
	
x3dtidy_LDADD = $(TITANIA_ANYOPTIONS_LIBS)  \
	$(TITANIA_GZSTREAM_LIBS) \
	$(TITANIA_OPENGL_SURFACE_LIBS) \
	$(TITANIA_STANDARD_LIBS) \
	$(TITANIA_STREAM_LIBS) \
	$(TITANIA_X3D_LIBS)  \
	$(top_builddir)/libtitania-anyoptions/libtitania-anyoptions.la \
	$(top_builddir)/libtitania-x3d/libtitania-x3d.la \
	$(top_builddir)/libtitania-gzstream/libtitania-gzstream.la \
	$(top_builddir)/libtitania-opengl-surface/libtitania-opengl-surface.la \
	$(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la

AM_CPPFLAGS = $(TITANIA_ANYOPTIONS_CFLAGS)  \
	$(TITANIA_GZSTREAM_CFLAGS) \
	$(TITANIA_OPENGL_SURFACE_CFLAGS) \
	$(TITANIA_STANDARD_CFLAGS) \
	$(TITANIA_STREAM_CFLAGS) \
	$(TITANIA_X3D_CFLAGS)  \
	-I$(top_builddir)/libtitania-anyoptions \
	-I$(top_builddir)/libtitania-x3d \
	-I$(top_builddir)/libtitania-gzstream \
	-I$(top_builddir)/libtitania-opengl-surface \
	-I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream	

run:
	env XDG_DATA_DIRS="/home/holger/Projekte/Titania/Titania/share:/home/holger/Projekte/Titania/libtitania-x3d/share:"${XDG_DATA_DIRS} \
	$(builddir)/$(bin_PROGRAMS)

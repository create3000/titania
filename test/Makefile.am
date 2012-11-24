## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	test

test_SOURCES =  \
	main.cpp \
	misc.cpp

test_LDADD = $(TITANIA_OPENGL_SURFACE_LIBS) \
	$(TITANIA_STANDARD_LIBS) \
	$(TITANIA_STREAM_LIBS) \
	$(TITANIA_X3D_LIBS) \
	$(top_builddir)/libtitania-x3d/libtitania-x3d.la \
	$(top_builddir)/libtitania-opengl-surface/libtitania-opengl-surface.la \
	$(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la

AM_CPPFLAGS = $(TITANIA_OPENGL_SURFACE_CFLAGS) \
	$(TITANIA_STANDARD_CFLAGS) \
	$(TITANIA_STREAM_CFLAGS) \
	$(TITANIA_X3D_CFLAGS) \
	-I$(top_builddir)/libtitania-x3d \
	-I$(top_builddir)/libtitania-opengl-surface \
	-I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream	

run:
	$(builddir)/$(bin_PROGRAMS)


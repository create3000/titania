## Process this file with automake to produce Makefile.in


lib_LTLIBRARIES =  \
	Titania.la

Titania_la_SOURCES =  \
	Titania/Python/PyX3D.cpp \
	Titania/Python/X3D/Fields/PySFVec3f.cpp \
	Titania/Python/X3D/PyBrowser.cpp

Titania_la_LIBADD = $(TITANIA_PYTHON_LIBS) \
	$(TITANIA_GZSTREAM_LIBS) \
	$(TITANIA_OPENGL_SURFACE_LIBS) \
	$(TITANIA_SQLITE_LIBS) \
	$(TITANIA_STANDARD_LIBS) \
	$(TITANIA_STREAM_LIBS) \
	$(TITANIA_X3D_LIBS)  \
	$(top_builddir)/libtitania-x3d/libtitania-x3d.la \
	$(top_builddir)/libtitania-gzstream/libtitania-gzstream.la \
	$(top_builddir)/libtitania-opengl-surface/libtitania-opengl-surface.la \
	$(top_builddir)/libtitania-sqlite/libtitania-sqlite.la \
	$(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la


AM_CPPFLAGS = $(TITANIA_PYTHON_CFLAGS) \
	$(TITANIA_GZSTREAM_CFLAGS) \
	$(TITANIA_OPENGL_SURFACE_CFLAGS) \
	$(TITANIA_SQLITE_CFLAGS) \
	$(TITANIA_STANDARD_CFLAGS) \
	$(TITANIA_STREAM_CFLAGS) \
	$(TITANIA_X3D_CFLAGS)  \
	-I$(top_builddir)/libtitania-x3d \
	-I$(top_builddir)/libtitania-gzstream \
	-I$(top_builddir)/libtitania-opengl-surface \
	-I$(top_builddir)/libtitania-sqlite \
	-I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream

AM_LDFLAGS=-module

run:
	@PYTHONPATH=`dirname \`find . -name "Titania.so"\`` \
		python test.py

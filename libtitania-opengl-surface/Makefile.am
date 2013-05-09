## Process this file with automake to produce Makefile.in


lib_LTLIBRARIES =  \
	libtitania-opengl-surface.la

libtitania_opengl_surface_la_SOURCES =  \
	Titania/OpenGL/Context/Context.cpp \
	Titania/OpenGL/Context/WindowContext.cpp \
	Titania/OpenGL/DrawingArea.cpp \
	Titania/OpenGL/Surface.cpp

libtitania_opengl_surface_la_LIBADD =  \
	$(TITANIA_OPENGL_SURFACE_LIBS) \
	$(top_srcdir)/libtitania-standard/libtitania-standard.la

AM_CPPFLAGS = $(TITANIA_OPENGL_SURFACE_CFLAGS) \
	-I$(top_builddir)/libtitania-standard


## -*- Makefile -*-
## Makefile.am for gtkglext/gdk/x11

AM_CPPFLAGS = \
	-DG_LOG_DOMAIN=\"GdkGLExt\"	\
	-DGDK_GL_COMPILATION		\
	-DINSIDE_GDK_GL_X11		\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/gdk		\
	-I$(top_builddir)/gdk		\
	$(GTKGLEXT_DEBUG_FLAGS)		\
	$(GDK_CFLAGS)			\
	$(GL_CFLAGS)			\
	$(GDKGLEXT_WIN_CFLAGS)

gdkglext_public_h_sources = \
	gdkglx.h

gdkglext_x11_public_h_sources = \
	gdkx11glconfig.h \
	gdkx11glcontext.h \
	gdkx11glquery.h \
	gdkx11glwindow.h

gdkglext_x11_private_h_sources = \
	gdkglquery-x11.h	\
	gdkglconfig-x11.h	\
	gdkglcontext-x11.h	\
	gdkglwindow-x11.h	\
	gdkglprivate-x11.h

gdkglext_x11_c_sources = \
	gdkglquery-x11.c	\
	gdkglconfig-x11.c	\
	gdkglcontext-x11.c	\
	gdkglwindow-x11.c \
	gdkx11glconfig.c \
	gdkx11glcontext.c \
	gdkx11glwindow.c

gdkglext_headers = \
	$(gdkglext_public_h_sources)

gdkglext_x11_headers = \
	$(gdkglext_x11_public_h_sources)

gdkglext_x11_sources = \
	$(gdkglext_x11_private_h_sources)	\
	$(gdkglext_x11_c_sources)

gdkglextincludedir = $(includedir)/gtkglext-@GTKGLEXT_API_VERSION@/gdk
gdkglextinclude_HEADERS = $(gdkglext_headers)

gdkglextx11includedir = $(includedir)/gtkglext-@GTKGLEXT_API_VERSION@/gdk/x11
gdkglextx11include_HEADERS = $(gdkglext_x11_headers)

noinst_LTLIBRARIES = libgdkglext-x11.la

libgdkglext_x11_la_SOURCES = $(gdkglext_x11_sources)
libgdkglext_x11_la_LDFLAGS = $(GDK_LIBS) $(GL_LIBS) $(GDKGLEXT_WIN_LIBS)

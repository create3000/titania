## -*- Makefile -*-
## Makefile.am for gtkglext/gtk

CLEANFILES =

EXTRA_DIST = \
	gtkglversion.h.in	\
	gtkglext.def

if OS_WIN32
gtkglext_def = $(srcdir)/gtkglext.def
gtkglext_win32_symbols = -export-symbols $(gtkglext_def)
endif

if MS_LIB_AVAILABLE
mslibdir = $(libdir)
noinst_DATA = gtkglext-@GTKGLEXT_API_VERSION@.lib

gtkglext-@GTKGLEXT_API_VERSION@.lib: libgtkglext-@GTKGLEXT_API_VERSION@.la $(gtkglext_def)
	lib -name:libgtkglext-@GTKGLEXT_API_VERSION@-@GTKGLEXT_CURRENT_MINUS_AGE@.dll -def:$(gtkglext_def) -out:$@
endif

common_includes = \
	-DG_LOG_DOMAIN=\"GtkGLExt\"	\
	-DGTK_GL_COMPILATION		\
	-I$(top_srcdir)			\
	-I$(top_builddir)/gdk		\
	$(GTKGLEXT_DEBUG_FLAGS)		\
	$(GTK_CFLAGS)			\
	$(GL_CFLAGS)			\
	$(GDKGLEXT_WIN_CFLAGS)

common_ldflags = \
	-version-info $(GTKGLEXT_CURRENT):$(GTKGLEXT_REVISION):$(GTKGLEXT_AGE) 	\
	-export-dynamic 					\
	-rpath $(libdir) 					\
	-no-undefined						\
	@LIBTOOL_EXPORT_OPTIONS@				\
	$(gtkglext_win32_symbols)

common_libadd = \
	$(top_builddir)/gdk/$(gdkglext_targetlib)		\
	$(GTK_LIBS)						\
	$(GL_LIBS)						\
	$(GDKGLEXT_WIN_LIBS)					\
	$(MATH_LIB)

#
# setup source file variables
#

gtkglext_public_h_sources = \
	gtkgl.h		\
	gtkgldefs.h	\
	gtkglversion.h	\
	gtkglinit.h	\
	gtkglwidget.h

gtkglext_private_h_sources = \
	gtkglprivate.h

gtkglext_c_sources = \
	gtkglversion.c	\
	gtkglinit.c	\
	gtkglwidget.c

gtkglext_headers = \
	$(gtkglext_public_h_sources)	\
	gtkgldebug.h

gtkglext_sources = \
	$(gtkglext_private_h_sources)	\
	$(gtkglext_c_sources)

#
# setup GtkGLExt sources and their dependancies
#

gtkglextincludedir = $(includedir)/gtkglext-@GTKGLEXT_API_VERSION@/gtk
gtkglextinclude_HEADERS = $(gtkglext_headers)

AM_CPPFLAGS = $(common_includes)

lib_LTLIBRARIES = libgtkglext-@API_MJ@.@API_MI@.la

libgtkglext_@API_MJ@_@API_MI@_la_SOURCES = $(gtkglext_sources)
libgtkglext_@API_MJ@_@API_MI@_la_LDFLAGS = $(common_ldflags)
libgtkglext_@API_MJ@_@API_MI@_la_LIBADD = $(common_libadd)

#
# Introspection
#

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=../gdk --warn-all
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir) --includedir=../gdk

if HAVE_INTROSPECTION
introspection_sources = \
	$(gtkglext_public_h_sources) \
	$(gtkglext_c_sources)

GtkGLExt-3.0.gir: $(gtkglext_targetlib) Makefile
GtkGLExt_3_0_gir_SCANNERFLAGS = \
	--identifier-prefix=GtkGL \
	--symbol-prefix=gtk_gl \
	--symbol-prefix=gtk
GtkGLExt_3_0_gir_INCLUDES = Gtk-3.0 GdkGLExt-3.0
GtkGLExt_3_0_gir_LIBS = $(gtkglext_targetlib)
GtkGLExt_3_0_gir_FILES = $(introspection_sources)
GtkGLExt_3_0_gir_CFLAGS = $(AM_CPPFLAGS)
INTROSPECTION_GIRS += GtkGLExt-3.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif # HAVE_INTROSPECTION

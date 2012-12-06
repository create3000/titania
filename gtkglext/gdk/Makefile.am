## -*- Makefile -*-
## Makefile.am for gtkglext/gdk

SUBDIRS = $(GDKGLEXT_BACKENDS)
DIST_SUBDIRS = x11 win32

CLEANFILES =

EXTRA_DIST = \
	gdkglversion.h.in	\
	gdkglext.def

if OS_WIN32
gdkglext_def = $(srcdir)/gdkglext.def
gdkglext_win32_symbols = -export-symbols $(gdkglext_def)
endif

if MS_LIB_AVAILABLE
mslibdir = $(libdir)
mslib_DATA = gdkglext-win32-@GTKGLEXT_API_VERSION@.lib

gdkglext-win32-@GTKGLEXT_API_VERSION@.lib: libgdkglext-win32-@GTKGLEXT_API_VERSION@.la $(gdkglext_def)
	lib -name:libgdkglext-win32-@GTKGLEXT_API_VERSION@-@GTKGLEXT_CURRENT_MINUS_AGE@.dll -def:$(gdkglext_def) -out:$@
endif

common_includes = \
	-DG_LOG_DOMAIN=\"GdkGLExt\"	\
	-DGDK_GL_COMPILATION		\
	-I$(top_srcdir)			\
	$(GTKGLEXT_DEBUG_FLAGS)		\
	$(GDK_CFLAGS)			\
	$(GTK_CFLAGS)			\
	$(GL_CFLAGS)			\
	$(GDKGLEXT_WIN_CFLAGS)

common_ldflags = \
	-version-info $(GTKGLEXT_CURRENT):$(GTKGLEXT_REVISION):$(GTKGLEXT_AGE) 	\
	-export-dynamic 					\
	-rpath $(libdir) 					\
	-no-undefined						\
	@LIBTOOL_EXPORT_OPTIONS@				\
	$(gdkglext_win32_symbols)

common_libadd = \
	$(GDK_LIBS) $(GLU_LIBS) $(GDKGLEXT_WIN_LIBS)

#
# setup source file variables
#
gdkglext_configure_generated_public_h_source = gdkglversion.h

gdkglext_public_h_sources = \
	gdkgl.h			\
	gdkgldefs.h		\
	gdkgltokens.h		\
	gdkgltypes.h		\
	gdkglinit.h		\
	gdkglquery.h		\
	gdkglconfig.h		\
	gdkglcontext.h		\
	gdkgldrawable.h		\
	gdkglwindow.h

gdkglext_private_h_sources = \
	gdkglconfigimpl.h \
	gdkglcontextimpl.h \
	gdkglprivate.h \
	gdkglwindowimpl.h

gdkglext_built_public_h_sources = \
	gdkglenumtypes.h

gdkglext_c_sources = \
	gdkglversion.c		\
	gdkglinit.c		\
	gdkglquery.c		\
	gdkglconfig.c		\
	gdkglconfigimpl.c \
	gdkglcontext.c		\
	gdkglcontextimpl.c \
	gdkgldrawable.c		\
	gdkglwindow.c \
	gdkglwindowimpl.c

gdkglext_built_c_sources = \
	gdkglenumtypes.c

gdkglext_headers = \
	$(gdkglext_public_h_sources)			\
	$(gdkglext_built_public_h_sources)		\
	$(gdkglext_configure_generated_public_h_source) \
	gdkgldebug.h

gdkglext_sources = \
	$(gdkglext_private_h_sources)		\
	$(gdkglext_c_sources)			\
	$(gdkglext_built_c_sources)

gdkglext_built_sources = \
	$(gdkglext_built_public_h_sources)	\
	$(gdkglext_built_c_sources)

stamp_files = \
	stamp-gdkglenumtypes-h

#
# setup GdkGLExt sources and their dependancies
#

gdkglextincludedir = $(includedir)/gtkglext-@GTKGLEXT_API_VERSION@/gdk
gdkglextinclude_HEADERS = $(gdkglext_headers)

AM_CPPFLAGS = $(common_includes)

lib_LTLIBRARIES = libgdkglext-@API_MJ@.@API_MI@.la

libgdkglext_@API_MJ@_@API_MI@_la_SOURCES = $(gdkglext_sources)
libgdkglext_@API_MJ@_@API_MI@_la_LDFLAGS = $(common_ldflags)
libgdkglext_@API_MJ@_@API_MI@_la_LIBADD  =

if USE_X11
libgdkglext_@API_MJ@_@API_MI@_la_LIBADD += x11/libgdkglext-x11.la
endif

if USE_WIN32
libgdkglext_@API_MJ@_@API_MI@_la_LIBADD += win32/libgdkglext-win32.la
libgdkglext_@API_MJ@_@API_MI@_la_DEPENDENCIES = $(gdkglext_def)
endif

libgdkglext_@API_MJ@_@API_MI@_la_LIBADD += $(common_libadd)

BUILT_SOURCES = \
	$(gdkglext_built_sources)

DISTCLEANFILES = $(stamp_files) $(gdkglext_configure_generated_public_h_source)

MAINTAINERCLEANFILES = \
	$(gdkglext_built_sources)

# Generate built header without using automake BUILT_SOURCES
$(libgdkglext_@API_MJ@_@API_MI@_la_OBJECTS): $(gdkglext_built_public_h_sources)

$(srcdir)/gdkglenumtypes.h: $(srcdir)/gtkglenumtypes.h.template $(gdkglext_public_h_sources) $(abs_builddir)/$(gdkglext_configure_generated_public_h_source)
	( cd $(srcdir) && $(GLIB_MKENUMS) --template gtkglenumtypes.h.template \
$(gdkglext_public_h_sources) $(abs_builddir)/$(gdkglext_configure_generated_public_h_source) ) >> xgen-geth \
&& (cmp -s xgen-geth $(srcdir)/gdkglenumtypes.h || cp xgen-geth $(srcdir)/gdkglenumtypes.h ) \
&& rm -f xgen-geth

$(srcdir)/gdkglenumtypes.c: $(srcdir)/gtkglenumtypes.c.template $(gdkglext_public_h_sources) $(abs_builddir)/$(gdkglext_configure_generated_public_h_source)
	( cd $(srcdir) && $(GLIB_MKENUMS) --template gtkglenumtypes.c.template \
$(gdkglext_public_h_sources) $(abs_builddir)/$(gdkglext_configure_generated_public_h_source) ) > xgen-getc \
&& (cmp -s xgen-getc $(srcdir)/gdkglenumtypes.c || cp xgen-getc $(srcdir)/gdkglenumtypes.c ) \
&& rm -f xgen-getc

#
# Rule to install gdkglext-config.h header file
#
configexecincludedir = $(libdir)/gtkglext-@GTKGLEXT_API_VERSION@/include
configexecinclude_DATA = gdkglext-config.h

install-exec-local: gdkglext-config.h
	$(mkinstalldirs) $(DESTDIR)$(configexecincludedir)
	file=$(DESTDIR)$(configexecincludedir)/gdkglext-config.h; \
	if test -r $$file && cmp -s gdkglext-config.h $$file; then :; \
	else $(INSTALL_DATA) gdkglext-config.h $$file; fi

gdkglext-config.h: stamp-gdkglext-config-h
	@if test -f gdkglext-config.h; then :; \
	else rm -f stamp-gdkglext-config-h; $(MAKE) stamp-gdkglext-config-h; fi

stamp-gdkglext-config-h: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status gdk/gdkglext-config.h
	echo timestamp > stamp-gdkglext-config-h

DISTCLEANFILES += gdkglext-config.h stamp-gdkglext-config-h

EXTRA_DIST += \
  gtkglenumtypes.h.template \
  gtkglenumtypes.c.template

#
# Introspection
#

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --warn-all
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = \
	$(filter-out gdkgldebug.h gdkglglext.h, $(gdkglext_headers)) \
	$(gdkglext_c_sources) \
	$(gdkglext_built_c_sources)

if USE_X11
introspection_sources += \
	x11/gdkglconfig-x11.c	\
	x11/gdkglcontext-x11.c	\
	x11/gdkglquery-x11.c	\
	x11/gdkglwindow-x11.c
endif # USE_X11

GdkGLExt-3.0.gir: $(gdkglext_targetlib) Makefile
GdkGLExt_3_0_gir_SCANNERFLAGS = \
	--identifier-prefix=GdkGL \
	--symbol-prefix=gdk_gl \
	--symbol-prefix=gdk
GdkGLExt_3_0_gir_INCLUDES = Gdk-3.0
GdkGLExt_3_0_gir_LIBS = $(gdkglext_targetlib)
GdkGLExt_3_0_gir_FILES = $(introspection_sources)
GdkGLExt_3_0_gir_CFLAGS = $(AM_CPPFLAGS)
INTROSPECTION_GIRS += GdkGLExt-3.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif # HAVE_INTROSPECTION

#
# Extra rules
#

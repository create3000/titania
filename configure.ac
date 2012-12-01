dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(titania, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CXX

CXXFLAGS="-Wall -std=c++11 -Wno-pmf-conversions"


dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=titania
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT



LT_INIT

PKG_CHECK_MODULES(TITANIA,gconfmm-2.6)

PKG_CHECK_MODULES(TITANIA_X3D,fontconfig
ftgl
glu
mozjs185
libpcrecpp
ImageMagick++)

PKG_CHECK_MODULES(TITANIA_STREAM,libcurl)

PKG_CHECK_MODULES(TITANIA_SQLITE,sqlite3)

PKG_CHECK_MODULES(TITANIA_OPENGL_SURFACE,gtkmm-3.0
glew
gl)

PKG_CHECK_MODULES(TITANIA_GZSTREAM,zlib)

PKG_CHECK_MODULES(TITANIA_STANDARD, giomm-2.4
glibmm-2.4)


AC_OUTPUT([
Makefile
po/Makefile.in
libtitania-standard/Makefile
experimental/Makefile
libtitania-anyoptions/Makefile
libtitania-gzstream/Makefile
libtitania-opengl-surface/Makefile
libtitania-sqlite/Makefile
libtitania-stream/Makefile
libtitania-x3d/Makefile
Titania/Makefile
titania-info/Makefile
x3dtidy/Makefile
test/Makefile
])

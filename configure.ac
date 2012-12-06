dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(titania, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AM_MAINTAINER_MODE

AC_PROG_INSTALL

AC_PROG_CXX



dnl ***************************************************************************
dnl Configurations
dnl ***************************************************************************

AC_ARG_ENABLE(debug,
	AC_HELP_STRING([--enable-debug],
		[Use 'DEBUG' Configuration [default=YES]]),
		enable_debug=yes, enable_debug=no)

AM_CONDITIONAL(ENABLE_DEBUG, test x$enable_debug = xyes)

if test "x$enable_debug" = "xyes" ; then
	CONFIG_REQUESTED="yes"
fi


AC_ARG_ENABLE(release,
	AC_HELP_STRING([--enable-release],
		[Use 'RELEASE' Configuration [default=NO]]),
		enable_release=yes, enable_release=no)

AM_CONDITIONAL(ENABLE_RELEASE, test x$enable_release = xyes)

if test "x$enable_release" = "xyes" ; then
	CONFIG_REQUESTED="yes"
fi


AC_ARG_ENABLE(parallel,
	AC_HELP_STRING([--enable-parallel],
		[Use 'PARALLE' Configuration [default=NO]]),
		enable_parallel=yes, enable_parallel=no)

AM_CONDITIONAL(ENABLE_PARALLEL, test x$enable_parallel = xyes)

if test "x$enable_parallel" = "xyes" ; then
	CONFIG_REQUESTED="yes"
fi



dnl ***************************************************************************
dnl Compile options
dnl ***************************************************************************

CXXFLAGS="-Wall -std=c++11 -Wno-pmf-conversions"

if test "x$ENABLE_RELEASE" ; then
	CXXFLAGS=$CXXFLAGS" -O3"
fi

if test "x$ENABLE_PARALLEL" ; then
	CXXFLAGS=$CXXFLAGS" -O3 -ftree-parallelize-loops=4 -fopenmp"
fi


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

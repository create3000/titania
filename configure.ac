dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(titania, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([subdir-objects])

AM_SILENT_RULES([yes])

AM_MAINTAINER_MODE

AC_PROG_INSTALL

AC_PROG_CXX



dnl ***************************************************************************
dnl Configurations
dnl ***************************************************************************

AC_ARG_ENABLE(debug,
	AC_HELP_STRING([--enable-debug],
		[Use 'DEBUG' Configuration [default=NO]]),
		enable_debug=yes, enable_debug=no)

AC_ARG_ENABLE(parallel,
	AC_HELP_STRING([--enable-parallel],
		[Use 'PARALLEL' Configuration [default=NO]]),
		enable_parallel=yes, enable_parallel=no)

dnl AC_ARG_ENABLE(fixed-pipeline,
dnl 	AC_HELP_STRING([--enable-fixed-pipeline],
dnl 		[Use 'FIXED PIPELINE' Configuration [default=YES]]),
dnl 		enable_fixed_pipeline=yes, enable_fixed_pipeline=no)



dnl ***************************************************************************
dnl Compile options
dnl ***************************************************************************

dnl ******* add "-flto" flag (link time optimization) to CXXFLAGS or LDFLAGS if it is working

CXXFLAGS="-Wall -std=c++14 -O3 -fopenmp -Wno-narrowing"

if test "x$enable_release" = "xyes" ; then
	CXXFLAGS=$CXXFLAGS" "
fi

if test "x$enable_parallel" = "xyes" ; then
	CXXFLAGS=$CXXFLAGS" -floop-parallelize-all -ftree-parallelize-loops=4"
fi

CXXFLAGS=$CXXFLAGS" -DFIXED_PIPELINE"

if test "x$enable_debug" = "xyes" ; then
	CXXFLAGS=$CXXFLAGS" -DTITANIA_DEBUG -Wno-deprecated-declarations"
fi

LDFLAGS="-rdynamic"



dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************

IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=titania
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT

LT_INIT



dnl ***************************************************************************
dnl Check for C++11 compiler support
dnl ***************************************************************************

if test x$ax_cxx_compile_cxx11_required = xtrue; then
	if test x$ac_success = xno; then
		AC_MSG_ERROR([*** A compiler with support for C++11 language features is required.])
	fi
fi



dnl ***************************************************************************
dnl Check for GLX
dnl ***************************************************************************

USE_GLX="no"
PKG_CHECK_MODULES([GLX], [gl x11], [USE_GLX="yes"], [:])
AC_CHECK_HEADERS([GL/gl.h GL/glx.h], [:], [USE_GLX="no"])
AC_CHECK_LIB([GL], [glXCreateContext], [:] [USE_GLX="no"])
AM_CONDITIONAL(USE_GLX, test "$USE_GLX" = "yes")

if test "$USE_GLX" = "no"; then
	AC_MSG_ERROR([*** Please install X11 backend.])
fi



dnl ***************************************************************************
dnl Check required libraries
dnl ***************************************************************************

PKG_CHECK_MODULES(TITANIA,
gtksourceviewmm-3.0)

PKG_CHECK_MODULES(TITANIA_X3D,
glibmm-2.4
gstreamermm-1.0
fontconfig
glu
mozjs185
libxml++-2.6
json-c
ImageMagick++
libgtop-2.0)

PKG_CHECK_MODULES(TITANIA_FTGL,
freetype2
fontconfig
gl
glu)

PKG_CHECK_MODULES(TITANIA_STREAM,
libcurl
giomm-2.4)

PKG_CHECK_MODULES(TITANIA_SQLITE,
sqlite3)

PKG_CHECK_MODULES(TITANIA_OPENGL_SURFACE,
gtkmm-3.0
glew
gl)

PKG_CHECK_MODULES(TITANIA_GZSTREAM,
zlib)

PKG_CHECK_MODULES(TITANIA_STANDARD,
giomm-2.4
glibmm-2.4
gl
glu)


AC_OUTPUT([
libtitania-standard/Makefile
libtitania-sqlite/Makefile
libtitania-stream/Makefile
libtitania-gzstream/Makefile
libtitania-opengl-surface/Makefile
libtitania-peaseblossom/Makefile
libtitania-ftgl/Makefile
libtitania-x3d/Makefile
Titania/Makefile
experimental/Makefile
po/Makefile.in
Makefile
])

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([natsort], [0.1.0], [i.am.jack.mail@gmail.com])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([-Wall -Werror foreign silent-rules])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC

# Option to use git version
AC_ARG_ENABLE([git-version],
              AS_HELP_STRING([--enable-git-version],
                             [enable the use of git version]),
              [wantgitver=$enableval],
              [wantgitver=no])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

# git version
AC_MSG_CHECKING([if git version must be used])
if test "x$wantgitver" = "xyes"; then
    AC_MSG_RESULT([yes])
    AC_CHECK_PROGS([GIT], [git])
    if test "x$GIT" = "x"; then
        AC_MSG_ERROR([Cannot use git version: git not found])
    fi
    AC_CHECK_FILE([.git/], hasgitdir=yes)
    if test "x$hasgitdir" = "xyes"; then
        usegitver=yes
        gitver=-git
        AC_DEFINE([USE_GIT_VERSION], , [Use GIT version])
    else
        AC_MSG_ERROR([Cannot use git version: .git not found])
    fi
else
    AC_MSG_RESULT([no])
    usegitver=no
    gitver=
fi
AM_CONDITIONAL(USE_GIT_VERSION, test "x$usegitver" = "xyes")

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
        ${PACKAGE} version ${PACKAGE_VERSION}${gitver}

 Build information:
    source code location            : ${srcdir}
    prefix                          : ${prefix}

 Install paths:
    binaries                        : $(eval echo $(eval echo ${bindir}))
    documentation                   : $(eval echo $(eval echo ${docdir}))
    man pages                       : $(eval echo $(eval echo ${mandir}))
"

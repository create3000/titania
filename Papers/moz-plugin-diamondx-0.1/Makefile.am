SUBDIRS = doc test

lib_LTLIBRARIES = libdiamondx.la

# Note that the include files from moz-sdk were taken from the 
# mozilla/dist/sdk/include directory of the Firefox 2.0.0.2 release.

# need better solution than hardcoded X11 include path, but
# 'pkg-config --cflags' does not work correctly for x11 or xproto packages
INCLUDES = \
	-I$(top_srcdir)/moz-sdk \
	`pkg-config --cflags gtk+-2.0` \
	`pkg-config --cflags nspr` \
	-I/usr/include/X11

libdiamondx_la_LDFLAGS = \
	`pkg-config --libs gtk+-2.0` \
	`pkg-config --libs nspr`

libdiamondx_la_SOURCES = \
	diamondx.c \
	npshell.c \
	npunix.c \
	diamondx.h \
	moz-sdk/jni.h \
	moz-sdk/jni_md.h \
	moz-sdk/jri.h \
	moz-sdk/jri_md.h \
	moz-sdk/jritypes.h \
	moz-sdk/npapi.h \
	moz-sdk/npruntime.h \
	moz-sdk/nptypes.h \
	moz-sdk/npupp.h



# custom install rule
install-plugin:
	mkdir -p ~/.mozilla/plugins
	cp .libs/libdiamondx.so ~/.mozilla/plugins

## -*- Makefile -*-
## Makefile.am for gtkglext

SUBDIRS = gdk gtk docs examples

ACLOCAL_AMFLAGS = -I m4macros
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc --enable-introspection

EXTRA_DIST = \
	README.win32			\
	ChangeLog.pre-1-0		\
	ChangeLog.pre-1-2		\
	gdkglext.pc.in			\
	gtkglext.pc.in			\
	gdkglext-uninstalled.pc.in	\
	gtkglext-uninstalled.pc.in	\
	gtkglext.nsi.in			\
	gtkglext.nsi			\
	setup-gtkglext.sh

## Copy .pc files to target-specific names
GDK_TARGET = @GDKGLEXT_BACKENDS@
API_VER = @GTKGLEXT_API_VERSION@

gdkglext-$(GDK_TARGET)-$(API_VER).pc: gdkglext.pc gdkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc
	rm -f $@ && cp gdkglext.pc $@

gdkglext-$(API_VER).pc: gdkglext.pc gdkglext-$(API_VER)-uninstalled.pc
	rm -f $@ && cp gdkglext.pc $@

gtkglext-$(GDK_TARGET)-$(API_VER).pc: gtkglext.pc gtkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc
	rm -f $@ && cp gtkglext.pc $@

gtkglext-$(API_VER).pc: gtkglext.pc gtkglext-$(API_VER)-uninstalled.pc
	rm -f $@ && cp gtkglext.pc $@

gdkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc: gdkglext-uninstalled.pc
	rm -f $@ && cp gdkglext-uninstalled.pc $@

gdkglext-$(API_VER)-uninstalled.pc: gdkglext-uninstalled.pc gdkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc
	rm -f $@ && cp gdkglext-uninstalled.pc $@

gtkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc: gtkglext-uninstalled.pc
	rm -f $@ && cp gtkglext-uninstalled.pc $@

gtkglext-$(API_VER)-uninstalled.pc: gtkglext-uninstalled.pc gtkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc
	rm -f $@ && cp gtkglext-uninstalled.pc $@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = \
	gdkglext-$(GDK_TARGET)-$(API_VER).pc	\
	gdkglext-$(API_VER).pc			\
	gtkglext-$(GDK_TARGET)-$(API_VER).pc	\
	gtkglext-$(API_VER).pc
$(pkgconfig_DATA): $(top_builddir)/config.status

DISTCLEANFILES = \
	gdkglext-$(GDK_TARGET)-$(API_VER).pc			\
	gdkglext-$(API_VER).pc					\
	gtkglext-$(GDK_TARGET)-$(API_VER).pc			\
	gtkglext-$(API_VER).pc					\
	gdkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc	\
	gdkglext-$(API_VER)-uninstalled.pc			\
	gtkglext-$(GDK_TARGET)-$(API_VER)-uninstalled.pc	\
	gtkglext-$(API_VER)-uninstalled.pc

ChangeLog:
	@echo Creating $@
	@if test -d "$(srcdir)/.git"; then \
	  (GIT_DIR=$(top_srcdir)/.git $(top_srcdir)/missing --run git log --stat) | fmt --split-only > $@.tmp \
	  && mv -f $@.tmp $@ \
	  || ($(RM) $@.tmp; \
	      echo Failed to generate ChangeLog, your ChangeLog may be outdated >&2; \
	      (test -f $@ || echo git-log is required to generate this file >> $@)); \
	else \
	  test -f $@ || \
	  (echo A git checkout and git-log is required to generate ChangeLog >&2 && \
	  echo A git checkout and git-log is required to generate this file >> $@); \
	fi

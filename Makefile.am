## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS =  \
	libtitania-standard \
	libtitania-sqlite \
	libtitania-stream \
	libtitania-ftgl \
	libtitania-x3d \
	Titania


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(titaniadoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update

run:
	$(builddir)/Titania/run.pl

version:
	perl build/version.pl

flathub:
	perl build/flathub.pl

# Remove doc directory on uninstall
#uninstall-local:
#	-rm -r $(titaniadocdir)

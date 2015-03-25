## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS =  \
	libtitania-standard \
	libtitania-sqlite \
	libtitania-stream \
	libtitania-anyoptions \
	libtitania-gzstream \
	libtitania-opengl-surface \
	libtitania-peaseblossom \
	libtitania-x3d \
	Titania \
	titania-info \
	x3dtidy \
	x3d2vrml


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(titaniadoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update


# Remove doc directory on uninstall
#uninstall-local:
#	-rm -r $(titaniadocdir)

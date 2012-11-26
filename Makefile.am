## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = libtitania-standard experimental libtitania-anyoptions libtitania-gzstream libtitania-opengl-surface libtitania-sqlite libtitania-stream libtitania-x3d Titania titania-info x3dtidy test


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

## Process this file with automake to produce Makefile.in

plugindir = $(libdir)/mozilla/plugins

plugin_LTLIBRARIES =  \
	libtitania-mozilla.la

libtitania_mozilla_la_SOURCES = plugin.cpp

libtitania_mozilla_la_LIBADD = $(TITANIA_MOZILLA_LIBS) \
	$(TITANIA_GZSTREAM_LIBS) \
	$(TITANIA_OPENGL_SURFACE_LIBS) \
	$(TITANIA_STANDARD_LIBS) \
	$(TITANIA_STREAM_LIBS) \
	$(TITANIA_X3D_LIBS)  \
	$(top_builddir)/libtitania-x3d/libtitania-x3d.la \
	$(top_builddir)/libtitania-gzstream/libtitania-gzstream.la \
	$(top_builddir)/libtitania-opengl-surface/libtitania-opengl-surface.la \
	$(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la

AM_CPPFLAGS = $(TITANIA_MOZILLA_CFLAGS) \
	$(TITANIA_GZSTREAM_CFLAGS) \
	$(TITANIA_OPENGL_SURFACE_CFLAGS) \
	$(TITANIA_STANDARD_CFLAGS) \
	$(TITANIA_STREAM_CFLAGS) \
	$(TITANIA_X3D_CFLAGS)  \
	-I$(top_builddir)/libtitania-x3d \
	-I$(top_builddir)/libtitania-gzstream \
	-I$(top_builddir)/libtitania-opengl-surface \
	-I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream

# Only leave the .so file in the plugins directory.
install-data-hook:
	cp -a -L $(DESTDIR)$(plugindir)/libtitania-mozilla.so $(DESTDIR)$(plugindir)/so.libtitania-mozilla.so 
	rm -f $(DESTDIR)$(plugindir)/libtitania-mozilla.*
	mv $(DESTDIR)$(plugindir)/so.libtitania-mozilla.so $(DESTDIR)$(plugindir)/libtitania-mozilla.so
	rm -r $(DESTDIR)$(plugindir)

run:
	mkdir -p ~/.mozilla/plugins
	cp -a -L .libs/libtitania-mozilla.so ~/.mozilla/plugins
	LD_LIBRARIES_PATH=`find \`realpath ..\` -name .libs | tr '\n' ':'` \
		firefox
	rm ~/.mozilla/plugins/libtitania-mozilla.so

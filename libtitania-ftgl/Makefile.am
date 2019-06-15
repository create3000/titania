## Process this file with automake to produce Makefile.in

current_dir = $(shell pwd)

lib_LTLIBRARIES =  \
	libtitania-ftgl.la

libtitania_ftgl_la_LIBADD = $(TITANIA_FTGL_LIBS) \
	$(TITANIA_STANDARD_LIBS) \
	$(top_builddir)/libtitania-standard/libtitania-standard.la

libtitania_ftgl_la_SOURCES =  \
	$(current_dir)/Titania/FTGL/Charmap.cpp \
	$(current_dir)/Titania/FTGL/Contour.cpp \
	$(current_dir)/Titania/FTGL/Face.cpp \
	$(current_dir)/Titania/FTGL/Font/Font.cpp \
	$(current_dir)/Titania/FTGL/Font/PolygonFont.cpp \
	$(current_dir)/Titania/FTGL/Glyph/Glyph.cpp \
	$(current_dir)/Titania/FTGL/Glyph/PolygonGlyph.cpp \
	$(current_dir)/Titania/FTGL/GlyphContainer.cpp \
	$(current_dir)/Titania/FTGL/Library.cpp \
	$(current_dir)/Titania/FTGL/Size.cpp \
	$(current_dir)/Titania/FTGL/Vectorizer.cpp

AM_CPPFLAGS = $(TITANIA_FTGL_CFLAGS) \
	$(TITANIA_STANDARD_CFLAGS) \
	-I$(top_builddir)/libtitania-standard

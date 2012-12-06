## -*- Makefile -*-
## Makefile.am for gtkglext/examples

EXTRA_DIST = \
	Makefile.msc	\
	alien.lwo	\
	penguin.lwo	\
	girl.rgb	\
	reflect.rgb

AM_CPPFLAGS = \
	-I$(top_srcdir) 		\
	-I$(top_builddir)/gdk		\
	-I$(top_builddir)/gtk		\
	$(GTKGLEXT_DEBUG_FLAGS)		\
	$(GTK_CFLAGS)			\
	$(BASE_DEPENDENCIES_CFLAGS)	\
	$(GL_CFLAGS)			\
	$(GDKGLEXT_WIN_CFLAGS)		\
	$(EXAMPLES_DEP_CFLAGS)

LDADD = \
	../gdk/$(gdkglext_targetlib)	\
	../gtk/$(gtkglext_targetlib)

AM_LDFLAGS = \
	$(GTK_LIBS)		\
	$(GDKGLEXT_WIN_LIBS)

if GLU
noinst_LTLIBRARIES = libdrawshapes.la
nodist_EXTRA_libdrawshapes_la_SOURCES = ltdummy.cpp
libdrawshapes_la_SOURCES = drawshapes.c drawshapes.h
libdrawshapes_la_LIBADD = $(MATH_LIB) $(GLU_LIBS) $(GL_LIBS)
endif

noinst_PROGRAMS =

if GLU
noinst_PROGRAMS += low-level
nodist_EXTRA_low_level_SOURCES = dummy.cpp
low_level_SOURCES = low-level.c
low_level_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
low_level_LDADD = ../gdk/$(gdkglext_targetlib)
endif

if GLU
noinst_PROGRAMS += simple
nodist_EXTRA_simple_SOURCES = dummy.cpp
simple_SOURCES = simple.c
simple_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif

if GLU
noinst_PROGRAMS += simple-mixed
nodist_EXTRA_simple_mixed_SOURCES = dummy.cpp
simple_mixed_SOURCES = simple-mixed.c
simple_mixed_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif

if GLU
noinst_PROGRAMS += share-lists
nodist_EXTRA_share_lists_SOURCES = dummy.cpp
share_lists_SOURCES = share-lists.c
share_lists_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif

if GLU
noinst_PROGRAMS += button
nodist_EXTRA_button_SOURCES = dummy.cpp
button_SOURCES = button.c
button_LDADD = $(LDADD) libdrawshapes.la
endif

if GLU
noinst_PROGRAMS += shapes
nodist_EXTRA_shapes_SOURCES = dummy.cpp
shapes_SOURCES = trackball.h trackball.c shapes.c
shapes_LDADD = $(LDADD) libdrawshapes.la
endif

noinst_PROGRAMS += logo
logo_SOURCES = trackball.h trackball.c logo-model.h logo-model.c logo.c
logo_SOURCES += logo-g.h logo-t.h logo-k.h

noinst_PROGRAMS += gears
gears_SOURCES = gears.c

if GLU
noinst_PROGRAMS += multiarb
nodist_EXTRA_multiarb_SOURCES = dummy.cpp
multiarb_SOURCES = readtex.h readtex.c multiarb.c
multiarb_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif

if GLU
noinst_PROGRAMS += viewlw
nodist_EXTRA_viewlw_SOURCES = dummy.cpp
viewlw_SOURCES = viewlw.c lw.h lw.c trackball.h trackball.c
viewlw_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif

noinst_PROGRAMS += rotating-square
rotating_square_SOURCES = rotating-square.c

if GLU
noinst_PROGRAMS += coolwave
nodist_EXTRA_coolwave_SOURCES = dummy.cpp
coolwave_SOURCES = coolwave.c
coolwave_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif

if GLU
noinst_PROGRAMS += coolwave2
nodist_EXTRA_coolwave2_SOURCES = dummy.cpp
coolwave2_SOURCES = coolwave2.c
coolwave2_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif

noinst_PROGRAMS += template
template_SOURCES = template.c

noinst_PROGRAMS += scribble-gl
scribble_gl_SOURCES = scribble-gl.c

if GLU
noinst_PROGRAMS += font-pangoft2
nodist_EXTRA_font_pangoft2_SOURCES = dummy.cpp
font_pangoft2_SOURCES = font-pangoft2.c
font_pangoft2_CPPFLAGS = $(AM_CPPFLAGS) $(PANGOFT2_CFLAGS)
font_pangoft2_LDFLAGS = $(AM_LDFLAGS) $(PANGOFT2_LIBS) $(GTKGLEXT_DEP_LIBS_WITH_PANGOFT2) $(GLU_LIBS) $(GL_LIBS)
endif

if GLU
noinst_PROGRAMS += font-pangoft2-tex
nodist_EXTRA_font_pangoft2_tex_SOURCES = dummy.cpp
font_pangoft2_tex_SOURCES = font-pangoft2-tex.c
font_pangoft2_tex_CPPFLAGS = $(AM_CPPFLAGS) $(PANGOFT2_CFLAGS)
font_pangoft2_tex_LDFLAGS = $(AM_LDFLAGS) $(PANGOFT2_LIBS) $(GTKGLEXT_DEP_LIBS_WITH_PANGOFT2) $(GLU_LIBS) $(GL_LIBS)
endif

if !USE_WIN32
if GLU
noinst_PROGRAMS += simple-pbuffer-sgix
nodist_EXTRA_simple_pbuffer_sgix_SOURCES = dummy.cpp
simple_pbuffer_sgix_SOURCES = simple-pbuffer-sgix.c
simple_pbuffer_sgix_LDFLAGS = $(AM_LDFLAGS) $(GLU_LIBS) $(GL_LIBS)
endif
endif

if USE_WIN32
noinst_PROGRAMS += wglinfo
wglinfo_SOURCES = wglinfo.c
wglinfo_LDADD = $(GL_LIBS) $(WINDOWING_LIBS)
endif

if !USE_WIN32
if GLU
noinst_PROGRAMS += glxinfo
nodist_EXTRA_glxinfo_SOURCES = dummy.cpp
glxinfo_SOURCES = glxinfo.c
glxinfo_LDADD = $(WINDOWING_LIBS) $(GLU_LIBS) $(GL_LIBS) $(MATH_LIB)
endif
endif


EXTRA_DIST += glade/README \
glade/simple.glade \
glade/simple.c \
glade/share-lists.glade	\
glade/share-lists.c

EXTRA_DIST += \
glsl/gears.c \
glsl/shiny.fsh \
glsl/shiny.vsh \
glsl/velvet.fsh \
glsl/velvet.vsh

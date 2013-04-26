## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	titania

titania_SOURCES =  \
	Titania/Base/X3DBaseInterface.cpp \
	Titania/Base/X3DBrowserInterface.cpp \
	Titania/Base/X3DUserInterface.cpp \
	Titania/Browser/BrowserWidget.cpp \
	Titania/Browser/BrowserWindow.cpp \
	Titania/Browser/ViewpointMenuItem.cpp \
	Titania/Browser/X3DBrowserWidget.cpp \
	Titania/Browser/X3DBrowserWindow.cpp \
	Titania/Configuration/config.cpp \
	Titania/Configuration/Configuration.cpp \
	Titania/HistoryEditor/History.cpp \
	Titania/HistoryEditor/HistoryEditor.cpp \
	Titania/main.cpp \
	Titania/MotionBlurEditor/MotionBlurEditor.cpp \
	Titania/OutlineEditor/OutlineEditor.cpp \
	Titania/OutlineEditor/OutlineSelection.cpp \
	Titania/OutlineEditor/OutlineTreeModel.cpp \
	Titania/OutlineEditor/OutlineTreeView.cpp \
	Titania/UserInterfaces/X3DBrowserUserInterface.cpp \
	Titania/UserInterfaces/X3DBrowserWidgetUI.cpp \
	Titania/UserInterfaces/X3DBrowserWindowUI.cpp \
	Titania/UserInterfaces/X3DConsoleUI.cpp \
	Titania/UserInterfaces/X3DHistoryEditorUI.cpp \
	Titania/UserInterfaces/X3DMotionBlurEditorUI.cpp \
	Titania/UserInterfaces/X3DOutlineEditorUI.cpp \
	Titania/UserInterfaces/X3DViewpointEditorUI.cpp \
	Titania/ViewpointEditor/ViewpointEditor.cpp

titania_LDADD = $(TITANIA_LIBS) \
	$(TITANIA_GZSTREAM_LIBS) \
	$(TITANIA_OPENGL_SURFACE_LIBS) \
	$(TITANIA_SQLITE_LIBS) \
	$(TITANIA_STANDARD_LIBS) \
	$(TITANIA_STREAM_LIBS) \
	$(TITANIA_X3D_LIBS)  \
	$(top_builddir)/libtitania-x3d/libtitania-x3d.la \
	$(top_builddir)/libtitania-gzstream/libtitania-gzstream.la \
	$(top_builddir)/libtitania-opengl-surface/libtitania-opengl-surface.la \
	$(top_builddir)/libtitania-sqlite/libtitania-sqlite.la \
	$(top_builddir)/libtitania-standard/libtitania-standard.la \
	$(top_builddir)/libtitania-stream/libtitania-stream.la

AM_CPPFLAGS = $(TITANIA_CFLAGS) \
	$(TITANIA_GZSTREAM_CFLAGS) \
	$(TITANIA_OPENGL_SURFACE_CFLAGS) \
	$(TITANIA_SQLITE_CFLAGS) \
	$(TITANIA_STANDARD_CFLAGS) \
	$(TITANIA_STREAM_CFLAGS) \
	$(TITANIA_X3D_CFLAGS)  \
	-I$(top_builddir)/libtitania-x3d \
	-I$(top_builddir)/libtitania-gzstream \
	-I$(top_builddir)/libtitania-opengl-surface \
	-I$(top_builddir)/libtitania-sqlite \
	-I$(top_builddir)/libtitania-standard \
	-I$(top_builddir)/libtitania-stream
	
run:
	env XDG_DATA_DIRS="/home/holger/Projekte/Titania/Titania/share:/home/holger/Projekte/Titania/libtitania-x3d/share:"${XDG_DATA_DIRS} \
	$(builddir)/$(bin_PROGRAMS)


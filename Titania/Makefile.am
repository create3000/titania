## Process this file with automake to produce Makefile.in


bin_PROGRAMS =  \
	titania

titania_SOURCES =  \
	Titania/Base/X3DBaseInterface.cpp \
	Titania/Base/X3DDialogInterface.cpp \
	Titania/Base/X3DUserInterface.cpp \
	Titania/Browser/BrowserSelection.cpp \
	Titania/Browser/BrowserWindow.cpp \
	Titania/Browser/MagicImport.cpp \
	Titania/Browser/X3DBrowserEditor.cpp \
	Titania/Browser/X3DBrowserWidget.cpp \
	Titania/Configuration/config.cpp \
	Titania/Configuration/Configuration.cpp \
	Titania/Console/Console.cpp \
	Titania/GeometryPropertiesEditor/GeometryPropertiesEditor.cpp \
	Titania/HistoryEditor/History.cpp \
	Titania/HistoryEditor/HistoryEditor.cpp \
	Titania/LibraryView/LibraryView.cpp \
	Titania/main.cpp \
	Titania/MaterialEditor/MaterialEditor.cpp \
	Titania/MotionBlurEditor/MotionBlurEditor.cpp \
	Titania/NodePropertiesEditor/NodePropertiesEditor.cpp \
	Titania/OutlineEditor/CellRenderer/OutlineCellRenderer.cpp \
	Titania/OutlineEditor/CellRenderer/OutlineSeparator.cpp \
	Titania/OutlineEditor/CellRenderer/TextViewEditable.cpp \
	Titania/OutlineEditor/CellRenderer/X3DTextViewEditable.cpp \
	Titania/OutlineEditor/OutlineDragDrop.cpp \
	Titania/OutlineEditor/OutlineEditor.cpp \
	Titania/OutlineEditor/OutlineRouteGraph.cpp \
	Titania/OutlineEditor/OutlineSelection.cpp \
	Titania/OutlineEditor/OutlineTreeData.cpp \
	Titania/OutlineEditor/OutlineTreeModel.cpp \
	Titania/OutlineEditor/OutlineTreeObserver.cpp \
	Titania/OutlineEditor/OutlineTreeViewEditor.cpp \
	Titania/OutlineEditor/X3DOutlineTreeView.cpp \
	Titania/TextEditor/TextEditor.cpp \
	Titania/TextEditor/X3DFontStyleNodeEditor.cpp \
	Titania/TextureEditor/TextureEditor.cpp \
	Titania/TextureEditor/X3DImageTextureEditor.cpp \
	Titania/TextureEditor/X3DPixelTextureEditor.cpp \
	Titania/TextureEditor/X3DTexture2DNodeEditor.cpp \
	Titania/TextureEditor/X3DTextureCoordinateGeneratorEditor.cpp \
	Titania/TextureEditor/X3DTextureNodeEditor.cpp \
	Titania/TextureEditor/X3DTexturePropertiesEditor.cpp \
	Titania/TextureEditor/X3DTextureTransformEditor.cpp \
	Titania/TextureEditor/X3DTextureTransformNodeEditor.cpp \
	Titania/Undo/UndoGroup.cpp \
	Titania/Undo/UndoHistory.cpp \
	Titania/Undo/UndoStep.cpp \
	Titania/UserInterfaces/X3DBrowserWindowInterface.cpp \
	Titania/UserInterfaces/X3DConsoleInterface.cpp \
	Titania/UserInterfaces/X3DGeometryPropertiesEditorInterface.cpp \
	Titania/UserInterfaces/X3DHistoryEditorInterface.cpp \
	Titania/UserInterfaces/X3DLibraryViewInterface.cpp \
	Titania/UserInterfaces/X3DMaterialEditorInterface.cpp \
	Titania/UserInterfaces/X3DMotionBlurEditorInterface.cpp \
	Titania/UserInterfaces/X3DNodePropertiesEditorInterface.cpp \
	Titania/UserInterfaces/X3DOutlineEditorInterface.cpp \
	Titania/UserInterfaces/X3DTextEditorInterface.cpp \
	Titania/UserInterfaces/X3DTextureEditorInterface.cpp \
	Titania/UserInterfaces/X3DViewpointListInterface.cpp \
	Titania/ViewpointList/ViewpointList.cpp

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
	$(builddir)/run.pl 2>&1 | $(builddir)/../miscellaneous/benchmarks/save.pl

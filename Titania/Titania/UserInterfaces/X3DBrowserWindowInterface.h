/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/
#ifndef __TMP_GLAD2CPP_BROWSER_WINDOW_H__
#define __TMP_GLAD2CPP_BROWSER_WINDOW_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DBrowserWindowInterface :
	public X3DUserInterface
{
public:

	template <class ... Arguments>
	X3DBrowserWindowInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		        filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const std::string & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		widget -> set_name (name);
		return widget;
	}

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAllFiles () const
	{ return m_fileFilterAllFiles; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAudio () const
	{ return m_fileFilterAudio; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterImage () const
	{ return m_fileFilterImage; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterVideo () const
	{ return m_fileFilterVideo; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterX3D () const
	{ return m_fileFilterX3D; }

	const Glib::RefPtr <Gtk::IconFactory> &
	getIconFactory () const
	{ return m_iconFactory; }

	const Glib::RefPtr <Gtk::AccelGroup> &
	getMenuAccelGroup () const
	{ return m_menuAccelGroup; }

	Gtk::Image &
	getExamineViewerImage () const
	{ return *m_examineViewerImage; }

	Gtk::FileChooserDialog &
	getFileImportDialog () const
	{ return *m_fileImportDialog; }

	Gtk::CheckButton &
	getImportAsInlineButton () const
	{ return *m_importAsInlineButton; }

	Gtk::Image &
	getFileImportImage () const
	{ return *m_fileImportImage; }

	Gtk::FileChooserDialog &
	getFileOpenDialog () const
	{ return *m_fileOpenDialog; }

	Gtk::FileChooserDialog &
	getFileSaveDialog () const
	{ return *m_fileSaveDialog; }

	Gtk::CheckButton &
	getSaveCompressedButton () const
	{ return *m_saveCompressedButton; }

	Gtk::Dialog &
	getFileSaveWarningDialog () const
	{ return *m_fileSaveWarningDialog; }

	Gtk::Image &
	getFlyViewerImage () const
	{ return *m_flyViewerImage; }

	Gtk::Image &
	getImportImage () const
	{ return *m_importImage; }

	Gtk::MessageDialog &
	getMessageDialog () const
	{ return *m_messageDialog; }

	Gtk::Image &
	getNoneViewerImage () const
	{ return *m_noneViewerImage; }

	Gtk::Dialog &
	getOpenLocationDialog () const
	{ return *m_openLocationDialog; }

	Gtk::Entry &
	getOpenLocationEntry () const
	{ return *m_openLocationEntry; }

	Gtk::Image &
	getOpenLocationImage () const
	{ return *m_openLocationImage; }

	Gtk::Image &
	getWalkViewerImage () const
	{ return *m_walkViewerImage; }

	Gtk::Menu &
	getViewerTypeMenu () const
	{ return *m_viewerTypeMenu; }

	Gtk::ImageMenuItem &
	getExamineViewerMenuItem () const
	{ return *m_examineViewerMenuItem; }

	Gtk::ImageMenuItem &
	getWalkViewerMenuItem () const
	{ return *m_walkViewerMenuItem; }

	Gtk::ImageMenuItem &
	getFlyViewerMenuItem () const
	{ return *m_flyViewerMenuItem; }

	Gtk::ImageMenuItem &
	getNoneViewerMenuItem () const
	{ return *m_noneViewerMenuItem; }

	Gtk::Window &
	getWindow () const
	{ return *m_window; }

	Gtk::VBox &
	getWidget () const
	{ return *m_widget; }

	Gtk::MenuBar &
	getMenuBar () const
	{ return *m_menuBar; }

	Gtk::MenuItem &
	getFileMenuItem () const
	{ return *m_fileMenuItem; }

	Gtk::ImageMenuItem &
	getNewMenuItem () const
	{ return *m_newMenuItem; }

	Gtk::ImageMenuItem &
	getOpenMenuItem () const
	{ return *m_openMenuItem; }

	Gtk::ImageMenuItem &
	getOpenLocationMenuItem () const
	{ return *m_openLocationMenuItem; }

	Gtk::ImageMenuItem &
	getImportMenuItem () const
	{ return *m_importMenuItem; }

	Gtk::ImageMenuItem &
	getSaveMenuItem () const
	{ return *m_saveMenuItem; }

	Gtk::ImageMenuItem &
	getSaveAsMenuItem () const
	{ return *m_saveAsMenuItem; }

	Gtk::ImageMenuItem &
	getRevertMenuItem () const
	{ return *m_revertMenuItem; }

	Gtk::ImageMenuItem &
	getQuitMenuItem () const
	{ return *m_quitMenuItem; }

	Gtk::MenuItem &
	getEditMenuItem () const
	{ return *m_editMenuItem; }

	Gtk::ImageMenuItem &
	getUndoMenuItem () const
	{ return *m_undoMenuItem; }

	Gtk::ImageMenuItem &
	getRedoMenuItem () const
	{ return *m_redoMenuItem; }

	Gtk::ImageMenuItem &
	getCutMenuItem () const
	{ return *m_cutMenuItem; }

	Gtk::ImageMenuItem &
	getCopyMenuItem () const
	{ return *m_copyMenuItem; }

	Gtk::ImageMenuItem &
	getPasteMenuItem () const
	{ return *m_pasteMenuItem; }

	Gtk::ImageMenuItem &
	getDeleteMenuItem () const
	{ return *m_deleteMenuItem; }

	Gtk::ImageMenuItem &
	getGroupSelectedNodesMenuItem () const
	{ return *m_groupSelectedNodesMenuItem; }

	Gtk::ImageMenuItem &
	getUngroupMenuItem () const
	{ return *m_ungroupMenuItem; }

	Gtk::MenuItem &
	getAddToGroupMenuItem () const
	{ return *m_addToGroupMenuItem; }

	Gtk::MenuItem &
	getDetachFromGroupMenuItem () const
	{ return *m_detachFromGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentGroupMenuItem () const
	{ return *m_createParentGroupMenuItem; }

	Gtk::MenuItem &
	getViewMenuItem () const
	{ return *m_viewMenuItem; }

	Gtk::CheckMenuItem &
	getToolBarMenuItem () const
	{ return *m_toolBarMenuItem; }

	Gtk::CheckMenuItem &
	getSideBarMenuItem () const
	{ return *m_sideBarMenuItem; }

	Gtk::CheckMenuItem &
	getFooterMenuItem () const
	{ return *m_footerMenuItem; }

	Gtk::ImageMenuItem &
	getWorkspacesMenuItem () const
	{ return *m_workspacesMenuItem; }

	Gtk::RadioMenuItem &
	getBrowserMenuItem () const
	{ return *m_browserMenuItem; }

	Gtk::RadioMenuItem &
	getEditorMenuItem () const
	{ return *m_editorMenuItem; }

	Gtk::SeparatorMenuItem &
	getBrowserOptionsSeparator () const
	{ return *m_browserOptionsSeparator; }

	Gtk::MenuItem &
	getShadingMenuItem () const
	{ return *m_shadingMenuItem; }

	Gtk::RadioMenuItem &
	getPhongMenuItem () const
	{ return *m_phongMenuItem; }

	Gtk::RadioMenuItem &
	getGouraudMenuItem () const
	{ return *m_gouraudMenuItem; }

	Gtk::RadioMenuItem &
	getFlatMenuItem () const
	{ return *m_flatMenuItem; }

	Gtk::RadioMenuItem &
	getWireFrameMenuItem () const
	{ return *m_wireFrameMenuItem; }

	Gtk::RadioMenuItem &
	getPointSetMenuItem () const
	{ return *m_pointSetMenuItem; }

	Gtk::MenuItem &
	getPrimitiveQualityMenuItem () const
	{ return *m_primitiveQualityMenuItem; }

	Gtk::RadioMenuItem &
	getHighQualityMenuItem () const
	{ return *m_highQualityMenuItem; }

	Gtk::RadioMenuItem &
	getMediumQualityMenuItem () const
	{ return *m_mediumQualityMenuItem; }

	Gtk::RadioMenuItem &
	getLowQualityMenuItem () const
	{ return *m_lowQualityMenuItem; }

	Gtk::CheckMenuItem &
	getRenderingPropertiesMenuItem () const
	{ return *m_renderingPropertiesMenuItem; }

	Gtk::ImageMenuItem &
	getFullScreenMenuItem () const
	{ return *m_fullScreenMenuItem; }

	Gtk::ImageMenuItem &
	getUnFullScreenMenuItem () const
	{ return *m_unFullScreenMenuItem; }

	Gtk::MenuItem &
	getSelectionMenuItem () const
	{ return *m_selectionMenuItem; }

	Gtk::ImageMenuItem &
	getSelectAllMenuItem () const
	{ return *m_selectAllMenuItem; }

	Gtk::MenuItem &
	getDeselectAllMenuItem () const
	{ return *m_deselectAllMenuItem; }

	Gtk::MenuItem &
	getNavigationMenuItem () const
	{ return *m_navigationMenuItem; }

	Gtk::CheckMenuItem &
	getRubberbandMenuItem () const
	{ return *m_rubberbandMenuItem; }

	Gtk::MenuItem &
	getToolsMenuItem () const
	{ return *m_toolsMenuItem; }

	Gtk::MenuItem &
	getMotionBlurMenuItem () const
	{ return *m_motionBlurMenuItem; }

	Gtk::MenuItem &
	getHelpMenuItem () const
	{ return *m_helpMenuItem; }

	Gtk::ImageMenuItem &
	getStandardSizeMenuItem () const
	{ return *m_standardSizeMenuItem; }

	Gtk::ImageMenuItem &
	getInfoMenuItem () const
	{ return *m_infoMenuItem; }

	Gtk::Toolbar &
	getToolBar () const
	{ return *m_toolBar; }

	Gtk::ToolButton &
	getNewButton () const
	{ return *m_newButton; }

	Gtk::MenuToolButton &
	getOpenButton () const
	{ return *m_openButton; }

	Gtk::ToolButton &
	getImportButton () const
	{ return *m_importButton; }

	Gtk::ToolButton &
	getSaveButton () const
	{ return *m_saveButton; }

	Gtk::ToolButton &
	getRefreshButton () const
	{ return *m_refreshButton; }

	Gtk::SeparatorToolItem &
	getSeparatorToolItem1 () const
	{ return *m_separatorToolItem1; }

	Gtk::ToolButton &
	getUndoButton () const
	{ return *m_undoButton; }

	Gtk::ToolButton &
	getRedoButton () const
	{ return *m_redoButton; }

	Gtk::SeparatorToolItem &
	getSeparatorToolItem2 () const
	{ return *m_separatorToolItem2; }

	Gtk::ToolButton &
	getNodePropertiesEditorButton () const
	{ return *m_nodePropertiesEditorButton; }

	Gtk::ToolButton &
	getMaterialEditorButton () const
	{ return *m_materialEditorButton; }

	Gtk::Paned &
	getVPaned () const
	{ return *m_vPaned; }

	Gtk::Paned &
	getHPaned () const
	{ return *m_hPaned; }

	Gtk::HBox &
	getSurfaceBox () const
	{ return *m_surfaceBox; }

	Gtk::Box &
	getDashboard () const
	{ return *m_dashboard; }

	Gtk::Toolbar &
	getDashboardToolBar () const
	{ return *m_dashboardToolBar; }

	Gtk::RadioToolButton &
	getHandButton () const
	{ return *m_handButton; }

	Gtk::RadioToolButton &
	getArrowButton () const
	{ return *m_arrowButton; }

	Gtk::MenuToolButton &
	getViewerButton () const
	{ return *m_viewerButton; }

	Gtk::ToolButton &
	getLookAtAllButton () const
	{ return *m_lookAtAllButton; }

	Gtk::ToggleToolButton &
	getLookAtButton () const
	{ return *m_lookAtButton; }

	Gtk::Box &
	getFooter () const
	{ return *m_footer; }

	Gtk::Notebook &
	getFooterNotebook () const
	{ return *m_footerNotebook; }

	Gtk::Box &
	getConsoleBox () const
	{ return *m_consoleBox; }

	Gtk::TextView &
	getConsole () const
	{ return *m_console; }

	Gtk::Box &
	getSideBar () const
	{ return *m_sideBar; }

	Gtk::Label &
	getSideBarLabel () const
	{ return *m_sideBarLabel; }

	Gtk::Notebook &
	getSideBarNotebook () const
	{ return *m_sideBarNotebook; }

	Gtk::Box &
	getViewpointListBox () const
	{ return *m_viewpointListBox; }

	Gtk::Box &
	getHistoryEditorBox () const
	{ return *m_historyEditorBox; }

	Gtk::Box &
	getLibraryViewBox () const
	{ return *m_libraryViewBox; }

	Gtk::Box &
	getOutlineEditorBox () const
	{ return *m_outlineEditorBox; }

	Gtk::Image &
	getWorkspacesImage () const
	{ return *m_workspacesImage; }

	virtual
	void
	on_messageDialog_response (int response_id) = 0;

	virtual
	void
	on_open_location_entry_changed () = 0;

	virtual
	bool
	on_open_location_entry_key_release_event (GdkEventKey* event) = 0;

	virtual
	void
	on_examine_viewer_activate () = 0;

	virtual
	void
	on_walk_viewer_activate () = 0;

	virtual
	void
	on_fly_viewer_activate () = 0;

	virtual
	void
	on_none_viewer_activate () = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* event) = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* event) = 0;

	virtual
	void
	on_new () = 0;

	virtual
	void
	on_open () = 0;

	virtual
	void
	on_open_location () = 0;

	virtual
	void
	on_import () = 0;

	virtual
	void
	on_save () = 0;

	virtual
	void
	on_save_as () = 0;

	virtual
	void
	on_revert_to_saved () = 0;

	virtual
	void
	on_close () = 0;

	virtual
	void
	on_undo () = 0;

	virtual
	void
	on_redo () = 0;

	virtual
	void
	on_cut_nodes_activate () = 0;

	virtual
	void
	on_copy_nodes_activate () = 0;

	virtual
	void
	on_paste_nodes_activate () = 0;

	virtual
	void
	on_delete_nodes_activate () = 0;

	virtual
	void
	on_group_selected_nodes_activate () = 0;

	virtual
	void
	on_ungroup_node_activate () = 0;

	virtual
	void
	on_add_to_group_activate () = 0;

	virtual
	void
	on_detach_from_group_activate () = 0;

	virtual
	void
	on_create_parent_group_activate () = 0;

	virtual
	void
	on_toolBar_toggled () = 0;

	virtual
	void
	on_sideBar_toggled () = 0;

	virtual
	void
	on_footer_toggled () = 0;

	virtual
	void
	on_browser_toggled () = 0;

	virtual
	void
	on_editor_toggled () = 0;

	virtual
	void
	phong_activate () = 0;

	virtual
	void
	gouraud_activate () = 0;

	virtual
	void
	flat_activate () = 0;

	virtual
	void
	wireframe_activate () = 0;

	virtual
	void
	pointset_activate () = 0;

	virtual
	void
	on_high_quality_activate () = 0;

	virtual
	void
	on_medium_quality_activate () = 0;

	virtual
	void
	on_low_quality_activate () = 0;

	virtual
	void
	on_rendering_properties_toggled () = 0;

	virtual
	void
	on_fullscreen () = 0;

	virtual
	void
	on_unfullscreen () = 0;

	virtual
	void
	on_select_all_activate () = 0;

	virtual
	void
	on_deselect_all_activate () = 0;

	virtual
	void
	on_rubberband_toggled () = 0;

	virtual
	void
	on_motion_blur_editor_activate () = 0;

	virtual
	void
	on_standard_size () = 0;

	virtual
	void
	on_info () = 0;

	virtual
	void
	on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_node_properties_editor () = 0;

	virtual
	void
	on_material_editor () = 0;

	virtual
	void
	on_surface_box_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_hand_button_toggled () = 0;

	virtual
	void
	on_arrow_button_toggled () = 0;

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	void
	on_look_at_toggled () = 0;

	~X3DBrowserWindowInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                     filename;
	Glib::RefPtr <Gtk::Builder>     m_builder;
	Glib::RefPtr <Gtk::FileFilter>  m_fileFilterAllFiles;
	Glib::RefPtr <Gtk::FileFilter>  m_fileFilterAudio;
	Glib::RefPtr <Gtk::FileFilter>  m_fileFilterImage;
	Glib::RefPtr <Gtk::FileFilter>  m_fileFilterVideo;
	Glib::RefPtr <Gtk::FileFilter>  m_fileFilterX3D;
	Glib::RefPtr <Gtk::IconFactory> m_iconFactory;
	Glib::RefPtr <Gtk::AccelGroup>  m_menuAccelGroup;
	Gtk::Image*                     m_examineViewerImage;
	Gtk::FileChooserDialog*         m_fileImportDialog;
	Gtk::CheckButton*               m_importAsInlineButton;
	Gtk::Image*                     m_fileImportImage;
	Gtk::FileChooserDialog*         m_fileOpenDialog;
	Gtk::FileChooserDialog*         m_fileSaveDialog;
	Gtk::CheckButton*               m_saveCompressedButton;
	Gtk::Dialog*                    m_fileSaveWarningDialog;
	Gtk::Image*                     m_flyViewerImage;
	Gtk::Image*                     m_importImage;
	Gtk::MessageDialog*             m_messageDialog;
	Gtk::Image*                     m_noneViewerImage;
	Gtk::Dialog*                    m_openLocationDialog;
	Gtk::Entry*                     m_openLocationEntry;
	Gtk::Image*                     m_openLocationImage;
	Gtk::Image*                     m_walkViewerImage;
	Gtk::Menu*                      m_viewerTypeMenu;
	Gtk::ImageMenuItem*             m_examineViewerMenuItem;
	Gtk::ImageMenuItem*             m_walkViewerMenuItem;
	Gtk::ImageMenuItem*             m_flyViewerMenuItem;
	Gtk::ImageMenuItem*             m_noneViewerMenuItem;
	Gtk::Window*                    m_window;
	Gtk::VBox*                      m_widget;
	Gtk::MenuBar*                   m_menuBar;
	Gtk::MenuItem*                  m_fileMenuItem;
	Gtk::ImageMenuItem*             m_newMenuItem;
	Gtk::ImageMenuItem*             m_openMenuItem;
	Gtk::ImageMenuItem*             m_openLocationMenuItem;
	Gtk::ImageMenuItem*             m_importMenuItem;
	Gtk::ImageMenuItem*             m_saveMenuItem;
	Gtk::ImageMenuItem*             m_saveAsMenuItem;
	Gtk::ImageMenuItem*             m_revertMenuItem;
	Gtk::ImageMenuItem*             m_quitMenuItem;
	Gtk::MenuItem*                  m_editMenuItem;
	Gtk::ImageMenuItem*             m_undoMenuItem;
	Gtk::ImageMenuItem*             m_redoMenuItem;
	Gtk::ImageMenuItem*             m_cutMenuItem;
	Gtk::ImageMenuItem*             m_copyMenuItem;
	Gtk::ImageMenuItem*             m_pasteMenuItem;
	Gtk::ImageMenuItem*             m_deleteMenuItem;
	Gtk::ImageMenuItem*             m_groupSelectedNodesMenuItem;
	Gtk::ImageMenuItem*             m_ungroupMenuItem;
	Gtk::MenuItem*                  m_addToGroupMenuItem;
	Gtk::MenuItem*                  m_detachFromGroupMenuItem;
	Gtk::MenuItem*                  m_createParentGroupMenuItem;
	Gtk::MenuItem*                  m_viewMenuItem;
	Gtk::CheckMenuItem*             m_toolBarMenuItem;
	Gtk::CheckMenuItem*             m_sideBarMenuItem;
	Gtk::CheckMenuItem*             m_footerMenuItem;
	Gtk::ImageMenuItem*             m_workspacesMenuItem;
	Gtk::RadioMenuItem*             m_browserMenuItem;
	Gtk::RadioMenuItem*             m_editorMenuItem;
	Gtk::SeparatorMenuItem*         m_browserOptionsSeparator;
	Gtk::MenuItem*                  m_shadingMenuItem;
	Gtk::RadioMenuItem*             m_phongMenuItem;
	Gtk::RadioMenuItem*             m_gouraudMenuItem;
	Gtk::RadioMenuItem*             m_flatMenuItem;
	Gtk::RadioMenuItem*             m_wireFrameMenuItem;
	Gtk::RadioMenuItem*             m_pointSetMenuItem;
	Gtk::MenuItem*                  m_primitiveQualityMenuItem;
	Gtk::RadioMenuItem*             m_highQualityMenuItem;
	Gtk::RadioMenuItem*             m_mediumQualityMenuItem;
	Gtk::RadioMenuItem*             m_lowQualityMenuItem;
	Gtk::CheckMenuItem*             m_renderingPropertiesMenuItem;
	Gtk::ImageMenuItem*             m_fullScreenMenuItem;
	Gtk::ImageMenuItem*             m_unFullScreenMenuItem;
	Gtk::MenuItem*                  m_selectionMenuItem;
	Gtk::ImageMenuItem*             m_selectAllMenuItem;
	Gtk::MenuItem*                  m_deselectAllMenuItem;
	Gtk::MenuItem*                  m_navigationMenuItem;
	Gtk::CheckMenuItem*             m_rubberbandMenuItem;
	Gtk::MenuItem*                  m_toolsMenuItem;
	Gtk::MenuItem*                  m_motionBlurMenuItem;
	Gtk::MenuItem*                  m_helpMenuItem;
	Gtk::ImageMenuItem*             m_standardSizeMenuItem;
	Gtk::ImageMenuItem*             m_infoMenuItem;
	Gtk::Toolbar*                   m_toolBar;
	Gtk::ToolButton*                m_newButton;
	Gtk::MenuToolButton*            m_openButton;
	Gtk::ToolButton*                m_importButton;
	Gtk::ToolButton*                m_saveButton;
	Gtk::ToolButton*                m_refreshButton;
	Gtk::SeparatorToolItem*         m_separatorToolItem1;
	Gtk::ToolButton*                m_undoButton;
	Gtk::ToolButton*                m_redoButton;
	Gtk::SeparatorToolItem*         m_separatorToolItem2;
	Gtk::ToolButton*                m_nodePropertiesEditorButton;
	Gtk::ToolButton*                m_materialEditorButton;
	Gtk::Paned*                     m_vPaned;
	Gtk::Paned*                     m_hPaned;
	Gtk::HBox*                      m_surfaceBox;
	Gtk::Box*                       m_dashboard;
	Gtk::Toolbar*                   m_dashboardToolBar;
	Gtk::RadioToolButton*           m_handButton;
	Gtk::RadioToolButton*           m_arrowButton;
	Gtk::MenuToolButton*            m_viewerButton;
	Gtk::ToolButton*                m_lookAtAllButton;
	Gtk::ToggleToolButton*          m_lookAtButton;
	Gtk::Box*                       m_footer;
	Gtk::Notebook*                  m_footerNotebook;
	Gtk::Box*                       m_consoleBox;
	Gtk::TextView*                  m_console;
	Gtk::Box*                       m_sideBar;
	Gtk::Label*                     m_sideBarLabel;
	Gtk::Notebook*                  m_sideBarNotebook;
	Gtk::Box*                       m_viewpointListBox;
	Gtk::Box*                       m_historyEditorBox;
	Gtk::Box*                       m_libraryViewBox;
	Gtk::Box*                       m_outlineEditorBox;
	Gtk::Image*                     m_workspacesImage;

};

} // puck
} // titania

#endif

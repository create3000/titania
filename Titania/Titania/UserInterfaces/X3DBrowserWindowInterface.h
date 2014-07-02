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

	const Glib::RefPtr <Gtk::IconFactory> &
	getIconFactory () const
	{ return m_IconFactory; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAllFiles () const
	{ return m_FileFilterAllFiles; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterAudio () const
	{ return m_FileFilterAudio; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterImage () const
	{ return m_FileFilterImage; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterVideo () const
	{ return m_FileFilterVideo; }

	const Glib::RefPtr <Gtk::FileFilter> &
	getFileFilterX3D () const
	{ return m_FileFilterX3D; }

	const Glib::RefPtr <Gtk::AccelGroup> &
	getMenuAccelGroup () const
	{ return m_MenuAccelGroup; }

	Gtk::Image &
	getExamineViewerImage () const
	{ return *m_ExamineViewerImage; }

	Gtk::Image &
	getFileImportImage () const
	{ return *m_FileImportImage; }

	Gtk::Image &
	getFlyViewerImage () const
	{ return *m_FlyViewerImage; }

	Gtk::Image &
	getImportImage () const
	{ return *m_ImportImage; }

	Gtk::Image &
	getNoneViewerImage () const
	{ return *m_NoneViewerImage; }

	Gtk::Image &
	getOpenLocationImage () const
	{ return *m_OpenLocationImage; }

	Gtk::Image &
	getPlaneViewerImage () const
	{ return *m_PlaneViewerImage; }

	Gtk::Menu &
	getPrototypeMenu () const
	{ return *m_PrototypeMenu; }

	Gtk::Image &
	getWalkViewerImage () const
	{ return *m_WalkViewerImage; }

	Gtk::Menu &
	getViewerTypeMenu () const
	{ return *m_ViewerTypeMenu; }

	Gtk::ImageMenuItem &
	getExamineViewerMenuItem () const
	{ return *m_ExamineViewerMenuItem; }

	Gtk::ImageMenuItem &
	getWalkViewerMenuItem () const
	{ return *m_WalkViewerMenuItem; }

	Gtk::ImageMenuItem &
	getFlyViewerMenuItem () const
	{ return *m_FlyViewerMenuItem; }

	Gtk::ImageMenuItem &
	getPlaneViewerMenuItem () const
	{ return *m_PlaneViewerMenuItem; }

	Gtk::ImageMenuItem &
	getNoneViewerMenuItem () const
	{ return *m_NoneViewerMenuItem; }

	Gtk::Image &
	getWorkspacesImage () const
	{ return *m_WorkspacesImage; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::MenuBar &
	getMenuBar () const
	{ return *m_MenuBar; }

	Gtk::MenuItem &
	getFileMenuItem () const
	{ return *m_FileMenuItem; }

	Gtk::ImageMenuItem &
	getNewMenuItem () const
	{ return *m_NewMenuItem; }

	Gtk::ImageMenuItem &
	getOpenMenuItem () const
	{ return *m_OpenMenuItem; }

	Gtk::ImageMenuItem &
	getOpenLocationMenuItem () const
	{ return *m_OpenLocationMenuItem; }

	Gtk::ImageMenuItem &
	getImportMenuItem () const
	{ return *m_ImportMenuItem; }

	Gtk::CheckMenuItem &
	getImportAsInlineMenuItem () const
	{ return *m_ImportAsInlineMenuItem; }

	Gtk::ImageMenuItem &
	getSaveMenuItem () const
	{ return *m_SaveMenuItem; }

	Gtk::ImageMenuItem &
	getSaveAsMenuItem () const
	{ return *m_SaveAsMenuItem; }

	Gtk::ImageMenuItem &
	getRevertMenuItem () const
	{ return *m_RevertMenuItem; }

	Gtk::ImageMenuItem &
	getRemoveUnusedPrototypesMenuItem () const
	{ return *m_RemoveUnusedPrototypesMenuItem; }

	Gtk::ImageMenuItem &
	getQuitMenuItem () const
	{ return *m_QuitMenuItem; }

	Gtk::MenuItem &
	getEditMenuItem () const
	{ return *m_EditMenuItem; }

	Gtk::ImageMenuItem &
	getUndoMenuItem () const
	{ return *m_UndoMenuItem; }

	Gtk::ImageMenuItem &
	getRedoMenuItem () const
	{ return *m_RedoMenuItem; }

	Gtk::ImageMenuItem &
	getCutMenuItem () const
	{ return *m_CutMenuItem; }

	Gtk::ImageMenuItem &
	getCopyMenuItem () const
	{ return *m_CopyMenuItem; }

	Gtk::ImageMenuItem &
	getPasteMenuItem () const
	{ return *m_PasteMenuItem; }

	Gtk::ImageMenuItem &
	getDeleteMenuItem () const
	{ return *m_DeleteMenuItem; }

	Gtk::MenuItem &
	getCloneMenuItem () const
	{ return *m_CloneMenuItem; }

	Gtk::MenuItem &
	getCreateCloneMenuItem () const
	{ return *m_CreateCloneMenuItem; }

	Gtk::MenuItem &
	getUnlinkCloneMenuItem () const
	{ return *m_UnlinkCloneMenuItem; }

	Gtk::ImageMenuItem &
	getGroupSelectedNodesMenuItem () const
	{ return *m_GroupSelectedNodesMenuItem; }

	Gtk::ImageMenuItem &
	getUngroupMenuItem () const
	{ return *m_UngroupMenuItem; }

	Gtk::MenuItem &
	getAddToGroupMenuItem () const
	{ return *m_AddToGroupMenuItem; }

	Gtk::MenuItem &
	getDetachFromGroupMenuItem () const
	{ return *m_DetachFromGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentGroupMenuItem () const
	{ return *m_CreateParentGroupMenuItem; }

	Gtk::MenuItem &
	getViewMenuItem () const
	{ return *m_ViewMenuItem; }

	Gtk::CheckMenuItem &
	getToolBarMenuItem () const
	{ return *m_ToolBarMenuItem; }

	Gtk::CheckMenuItem &
	getSideBarMenuItem () const
	{ return *m_SideBarMenuItem; }

	Gtk::CheckMenuItem &
	getFooterMenuItem () const
	{ return *m_FooterMenuItem; }

	Gtk::ImageMenuItem &
	getEnvironmentMenuItem () const
	{ return *m_EnvironmentMenuItem; }

	Gtk::RadioMenuItem &
	getBrowserMenuItem () const
	{ return *m_BrowserMenuItem; }

	Gtk::RadioMenuItem &
	getEditorMenuItem () const
	{ return *m_EditorMenuItem; }

	Gtk::SeparatorMenuItem &
	getBrowserOptionsSeparator () const
	{ return *m_BrowserOptionsSeparator; }

	Gtk::MenuItem &
	getShadingMenuItem () const
	{ return *m_ShadingMenuItem; }

	Gtk::RadioMenuItem &
	getPhongMenuItem () const
	{ return *m_PhongMenuItem; }

	Gtk::RadioMenuItem &
	getGouraudMenuItem () const
	{ return *m_GouraudMenuItem; }

	Gtk::RadioMenuItem &
	getFlatMenuItem () const
	{ return *m_FlatMenuItem; }

	Gtk::RadioMenuItem &
	getWireFrameMenuItem () const
	{ return *m_WireFrameMenuItem; }

	Gtk::RadioMenuItem &
	getPointSetMenuItem () const
	{ return *m_PointSetMenuItem; }

	Gtk::MenuItem &
	getPrimitiveQualityMenuItem () const
	{ return *m_PrimitiveQualityMenuItem; }

	Gtk::RadioMenuItem &
	getHighQualityMenuItem () const
	{ return *m_HighQualityMenuItem; }

	Gtk::RadioMenuItem &
	getMediumQualityMenuItem () const
	{ return *m_MediumQualityMenuItem; }

	Gtk::RadioMenuItem &
	getLowQualityMenuItem () const
	{ return *m_LowQualityMenuItem; }

	Gtk::MenuItem &
	getShowHideEnvironmentalEffectsMenuItem () const
	{ return *m_ShowHideEnvironmentalEffectsMenuItem; }

	Gtk::CheckMenuItem &
	getBackgroundsMenuItem () const
	{ return *m_BackgroundsMenuItem; }

	Gtk::CheckMenuItem &
	getFogsMenuItem () const
	{ return *m_FogsMenuItem; }

	Gtk::MenuItem &
	getObjectIconsMenuItem () const
	{ return *m_ObjectIconsMenuItem; }

	Gtk::CheckMenuItem &
	getProximitySensorsMenuItem () const
	{ return *m_ProximitySensorsMenuItem; }

	Gtk::CheckMenuItem &
	getVisibilitySensorsMenuItem () const
	{ return *m_VisibilitySensorsMenuItem; }

	Gtk::MenuItem &
	getHideAllObjectIconsMenuItem () const
	{ return *m_HideAllObjectIconsMenuItem; }

	Gtk::CheckMenuItem &
	getRenderingPropertiesMenuItem () const
	{ return *m_RenderingPropertiesMenuItem; }

	Gtk::ImageMenuItem &
	getFullScreenMenuItem () const
	{ return *m_FullScreenMenuItem; }

	Gtk::ImageMenuItem &
	getUnFullScreenMenuItem () const
	{ return *m_UnFullScreenMenuItem; }

	Gtk::MenuItem &
	getSelectionMenuItem () const
	{ return *m_SelectionMenuItem; }

	Gtk::ImageMenuItem &
	getSelectAllMenuItem () const
	{ return *m_SelectAllMenuItem; }

	Gtk::MenuItem &
	getDeselectAllMenuItem () const
	{ return *m_DeselectAllMenuItem; }

	Gtk::MenuItem &
	getHideSelectedObjectsMenuItem () const
	{ return *m_HideSelectedObjectsMenuItem; }

	Gtk::MenuItem &
	getHideUnselectedObjectsMenuItem () const
	{ return *m_HideUnselectedObjectsMenuItem; }

	Gtk::MenuItem &
	getShowSelectedObjectsMenuItem () const
	{ return *m_ShowSelectedObjectsMenuItem; }

	Gtk::MenuItem &
	getShowAllObjectsMenuItem () const
	{ return *m_ShowAllObjectsMenuItem; }

	Gtk::CheckMenuItem &
	getSelectLowestMenuItem () const
	{ return *m_SelectLowestMenuItem; }

	Gtk::CheckMenuItem &
	getFollowPrimarySelectionMenuItem () const
	{ return *m_FollowPrimarySelectionMenuItem; }

	Gtk::MenuItem &
	getNavigationMenuItem () const
	{ return *m_NavigationMenuItem; }

	Gtk::CheckMenuItem &
	getRubberbandMenuItem () const
	{ return *m_RubberbandMenuItem; }

	Gtk::MenuItem &
	getToolsMenuItem () const
	{ return *m_ToolsMenuItem; }

	Gtk::MenuItem &
	getMotionBlurMenuItem () const
	{ return *m_MotionBlurMenuItem; }

	Gtk::MenuItem &
	getHelpMenuItem () const
	{ return *m_HelpMenuItem; }

	Gtk::ImageMenuItem &
	getStandardSizeMenuItem () const
	{ return *m_StandardSizeMenuItem; }

	Gtk::ImageMenuItem &
	getInfoMenuItem () const
	{ return *m_InfoMenuItem; }

	Gtk::Toolbar &
	getToolBar () const
	{ return *m_ToolBar; }

	Gtk::ToolButton &
	getNewButton () const
	{ return *m_NewButton; }

	Gtk::MenuToolButton &
	getOpenButton () const
	{ return *m_OpenButton; }

	Gtk::ToolButton &
	getImportButton () const
	{ return *m_ImportButton; }

	Gtk::ToolButton &
	getSaveButton () const
	{ return *m_SaveButton; }

	Gtk::ToolButton &
	getRefreshButton () const
	{ return *m_RefreshButton; }

	Gtk::SeparatorToolItem &
	getSeparatorToolItem1 () const
	{ return *m_SeparatorToolItem1; }

	Gtk::ToolButton &
	getUndoButton () const
	{ return *m_UndoButton; }

	Gtk::ToolButton &
	getRedoButton () const
	{ return *m_RedoButton; }

	Gtk::SeparatorToolItem &
	getSeparatorToolItem2 () const
	{ return *m_SeparatorToolItem2; }

	Gtk::ToolButton &
	getNodePropertiesEditorButton () const
	{ return *m_NodePropertiesEditorButton; }

	Gtk::ToolButton &
	getMaterialEditorButton () const
	{ return *m_MaterialEditorButton; }

	Gtk::ToolButton &
	getUpdateViewpointButton () const
	{ return *m_UpdateViewpointButton; }

	Gtk::ToolButton &
	getCreatePrototypeInstanceButton () const
	{ return *m_CreatePrototypeInstanceButton; }

	Gtk::ToolButton &
	getGeometryPropertiesEditorButton () const
	{ return *m_GeometryPropertiesEditorButton; }

	Gtk::Paned &
	getVPaned () const
	{ return *m_VPaned; }

	Gtk::Paned &
	getHPaned () const
	{ return *m_HPaned; }

	Gtk::Box &
	getSurfaceBox () const
	{ return *m_SurfaceBox; }

	Gtk::Box &
	getDashboard () const
	{ return *m_Dashboard; }

	Gtk::Toolbar &
	getDashboardToolBar () const
	{ return *m_DashboardToolBar; }

	Gtk::RadioToolButton &
	getHandButton () const
	{ return *m_HandButton; }

	Gtk::RadioToolButton &
	getArrowButton () const
	{ return *m_ArrowButton; }

	Gtk::ToolButton &
	getPlayPauseButton () const
	{ return *m_PlayPauseButton; }

	Gtk::SeparatorToolItem &
	getSelectSeparator () const
	{ return *m_SelectSeparator; }

	Gtk::ToolButton &
	getSelectParentButton () const
	{ return *m_SelectParentButton; }

	Gtk::ToolButton &
	getSelectChildrenButton () const
	{ return *m_SelectChildrenButton; }

	Gtk::SeparatorToolItem &
	getViewerSeparator () const
	{ return *m_ViewerSeparator; }

	Gtk::MenuToolButton &
	getViewerButton () const
	{ return *m_ViewerButton; }

	Gtk::ToolButton &
	getStraightenButton () const
	{ return *m_StraightenButton; }

	Gtk::SeparatorToolItem &
	getLookAtSeparator () const
	{ return *m_LookAtSeparator; }

	Gtk::ToolButton &
	getLookAtAllButton () const
	{ return *m_LookAtAllButton; }

	Gtk::ToggleToolButton &
	getLookAtButton () const
	{ return *m_LookAtButton; }

	Gtk::Box &
	getFooter () const
	{ return *m_Footer; }

	Gtk::Label &
	getFooterLabel () const
	{ return *m_FooterLabel; }

	Gtk::Notebook &
	getFooterNotebook () const
	{ return *m_FooterNotebook; }

	Gtk::Box &
	getConsoleBox () const
	{ return *m_ConsoleBox; }

	Gtk::Box &
	getSideBar () const
	{ return *m_SideBar; }

	Gtk::Label &
	getSideBarLabel () const
	{ return *m_SideBarLabel; }

	Gtk::Notebook &
	getSideBarNotebook () const
	{ return *m_SideBarNotebook; }

	Gtk::Box &
	getViewpointListBox () const
	{ return *m_ViewpointListBox; }

	Gtk::Box &
	getHistoryEditorBox () const
	{ return *m_HistoryEditorBox; }

	Gtk::Box &
	getLibraryViewBox () const
	{ return *m_LibraryViewBox; }

	Gtk::Box &
	getOutlineEditorBox () const
	{ return *m_OutlineEditorBox; }

	Gtk::FileChooserDialog &
	getFileImportDialog () const
	{ return *m_FileImportDialog; }

	Gtk::FileChooserDialog &
	getFileOpenDialog () const
	{ return *m_FileOpenDialog; }

	Gtk::FileChooserDialog &
	getFileSaveDialog () const
	{ return *m_FileSaveDialog; }

	Gtk::CheckButton &
	getSaveCompressedButton () const
	{ return *m_SaveCompressedButton; }

	Gtk::Dialog &
	getFileSaveWarningDialog () const
	{ return *m_FileSaveWarningDialog; }

	Gtk::MessageDialog &
	getMessageDialog () const
	{ return *m_MessageDialog; }

	Gtk::Dialog &
	getOpenLocationDialog () const
	{ return *m_OpenLocationDialog; }

	Gtk::Entry &
	getOpenLocationEntry () const
	{ return *m_OpenLocationEntry; }

	Gtk::Dialog &
	getPrototypeInstanceDialog () const
	{ return *m_PrototypeInstanceDialog; }

	Gtk::Button &
	getPrototypeOkButton () const
	{ return *m_PrototypeOkButton; }

	Gtk::Box &
	getWidget1 () const
	{ return *m_Widget1; }

	Gtk::Label &
	getHeaderLabel () const
	{ return *m_HeaderLabel; }

	Gtk::MenuButton &
	getMenuButton () const
	{ return *m_MenuButton; }

	Gtk::Label &
	getPrototypeLabel () const
	{ return *m_PrototypeLabel; }

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
	on_plane_viewer_activate () = 0;

	virtual
	void
	on_none_viewer_activate () = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* event) = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* event) = 0;

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
	on_import_as_inline_toggled () = 0;

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
	on_remove_unused_prototypes () = 0;

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
	on_create_clone_activate () = 0;

	virtual
	void
	on_unlink_clone_activate () = 0;

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
	on_backgrounds_toggled () = 0;

	virtual
	void
	on_fogs_toggled () = 0;

	virtual
	void
	on_proximity_sensors_toggled () = 0;

	virtual
	void
	on_visibility_sensors_toggled () = 0;

	virtual
	void
	on_hide_all_object_icons_activate () = 0;

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
	on_hide_selected_objects_activate () = 0;

	virtual
	void
	on_hide_unselected_objects_activate () = 0;

	virtual
	void
	on_show_selected_objects_activate () = 0;

	virtual
	void
	on_show_all_objects_activate () = 0;

	virtual
	void
	on_select_lowest_toggled () = 0;

	virtual
	void
	on_follow_primary_selection_toggled () = 0;

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
	on_node_properties_editor_clicked () = 0;

	virtual
	void
	on_material_editor_clicked () = 0;

	virtual
	void
	on_update_viewpoint_clicked () = 0;

	virtual
	void
	on_prototype_instance_dialog_clicked () = 0;

	virtual
	void
	on_geometry_properties_editor_clicked () = 0;

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
	on_play_pause_button_clicked () = 0;

	virtual
	void
	on_select_parent_button_clicked () = 0;

	virtual
	void
	on_select_children_button_clicked () = 0;

	virtual
	void
	on_viewer_clicked () = 0;

	virtual
	void
	on_straighten_clicked () = 0;

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	void
	on_look_at_toggled () = 0;

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
	~X3DBrowserWindowInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                     filename;
	Glib::RefPtr <Gtk::Builder>     m_builder;
	Glib::RefPtr <Gtk::IconFactory> m_IconFactory;
	Glib::RefPtr <Gtk::FileFilter>  m_FileFilterAllFiles;
	Glib::RefPtr <Gtk::FileFilter>  m_FileFilterAudio;
	Glib::RefPtr <Gtk::FileFilter>  m_FileFilterImage;
	Glib::RefPtr <Gtk::FileFilter>  m_FileFilterVideo;
	Glib::RefPtr <Gtk::FileFilter>  m_FileFilterX3D;
	Glib::RefPtr <Gtk::AccelGroup>  m_MenuAccelGroup;
	Gtk::Image*                     m_ExamineViewerImage;
	Gtk::Image*                     m_FileImportImage;
	Gtk::Image*                     m_FlyViewerImage;
	Gtk::Image*                     m_ImportImage;
	Gtk::Image*                     m_NoneViewerImage;
	Gtk::Image*                     m_OpenLocationImage;
	Gtk::Image*                     m_PlaneViewerImage;
	Gtk::Menu*                      m_PrototypeMenu;
	Gtk::Image*                     m_WalkViewerImage;
	Gtk::Menu*                      m_ViewerTypeMenu;
	Gtk::ImageMenuItem*             m_ExamineViewerMenuItem;
	Gtk::ImageMenuItem*             m_WalkViewerMenuItem;
	Gtk::ImageMenuItem*             m_FlyViewerMenuItem;
	Gtk::ImageMenuItem*             m_PlaneViewerMenuItem;
	Gtk::ImageMenuItem*             m_NoneViewerMenuItem;
	Gtk::Image*                     m_WorkspacesImage;
	Gtk::Window*                    m_Window;
	Gtk::Box*                       m_Widget;
	Gtk::MenuBar*                   m_MenuBar;
	Gtk::MenuItem*                  m_FileMenuItem;
	Gtk::ImageMenuItem*             m_NewMenuItem;
	Gtk::ImageMenuItem*             m_OpenMenuItem;
	Gtk::ImageMenuItem*             m_OpenLocationMenuItem;
	Gtk::ImageMenuItem*             m_ImportMenuItem;
	Gtk::CheckMenuItem*             m_ImportAsInlineMenuItem;
	Gtk::ImageMenuItem*             m_SaveMenuItem;
	Gtk::ImageMenuItem*             m_SaveAsMenuItem;
	Gtk::ImageMenuItem*             m_RevertMenuItem;
	Gtk::ImageMenuItem*             m_RemoveUnusedPrototypesMenuItem;
	Gtk::ImageMenuItem*             m_QuitMenuItem;
	Gtk::MenuItem*                  m_EditMenuItem;
	Gtk::ImageMenuItem*             m_UndoMenuItem;
	Gtk::ImageMenuItem*             m_RedoMenuItem;
	Gtk::ImageMenuItem*             m_CutMenuItem;
	Gtk::ImageMenuItem*             m_CopyMenuItem;
	Gtk::ImageMenuItem*             m_PasteMenuItem;
	Gtk::ImageMenuItem*             m_DeleteMenuItem;
	Gtk::MenuItem*                  m_CloneMenuItem;
	Gtk::MenuItem*                  m_CreateCloneMenuItem;
	Gtk::MenuItem*                  m_UnlinkCloneMenuItem;
	Gtk::ImageMenuItem*             m_GroupSelectedNodesMenuItem;
	Gtk::ImageMenuItem*             m_UngroupMenuItem;
	Gtk::MenuItem*                  m_AddToGroupMenuItem;
	Gtk::MenuItem*                  m_DetachFromGroupMenuItem;
	Gtk::MenuItem*                  m_CreateParentGroupMenuItem;
	Gtk::MenuItem*                  m_ViewMenuItem;
	Gtk::CheckMenuItem*             m_ToolBarMenuItem;
	Gtk::CheckMenuItem*             m_SideBarMenuItem;
	Gtk::CheckMenuItem*             m_FooterMenuItem;
	Gtk::ImageMenuItem*             m_EnvironmentMenuItem;
	Gtk::RadioMenuItem*             m_BrowserMenuItem;
	Gtk::RadioMenuItem*             m_EditorMenuItem;
	Gtk::SeparatorMenuItem*         m_BrowserOptionsSeparator;
	Gtk::MenuItem*                  m_ShadingMenuItem;
	Gtk::RadioMenuItem*             m_PhongMenuItem;
	Gtk::RadioMenuItem*             m_GouraudMenuItem;
	Gtk::RadioMenuItem*             m_FlatMenuItem;
	Gtk::RadioMenuItem*             m_WireFrameMenuItem;
	Gtk::RadioMenuItem*             m_PointSetMenuItem;
	Gtk::MenuItem*                  m_PrimitiveQualityMenuItem;
	Gtk::RadioMenuItem*             m_HighQualityMenuItem;
	Gtk::RadioMenuItem*             m_MediumQualityMenuItem;
	Gtk::RadioMenuItem*             m_LowQualityMenuItem;
	Gtk::MenuItem*                  m_ShowHideEnvironmentalEffectsMenuItem;
	Gtk::CheckMenuItem*             m_BackgroundsMenuItem;
	Gtk::CheckMenuItem*             m_FogsMenuItem;
	Gtk::MenuItem*                  m_ObjectIconsMenuItem;
	Gtk::CheckMenuItem*             m_ProximitySensorsMenuItem;
	Gtk::CheckMenuItem*             m_VisibilitySensorsMenuItem;
	Gtk::MenuItem*                  m_HideAllObjectIconsMenuItem;
	Gtk::CheckMenuItem*             m_RenderingPropertiesMenuItem;
	Gtk::ImageMenuItem*             m_FullScreenMenuItem;
	Gtk::ImageMenuItem*             m_UnFullScreenMenuItem;
	Gtk::MenuItem*                  m_SelectionMenuItem;
	Gtk::ImageMenuItem*             m_SelectAllMenuItem;
	Gtk::MenuItem*                  m_DeselectAllMenuItem;
	Gtk::MenuItem*                  m_HideSelectedObjectsMenuItem;
	Gtk::MenuItem*                  m_HideUnselectedObjectsMenuItem;
	Gtk::MenuItem*                  m_ShowSelectedObjectsMenuItem;
	Gtk::MenuItem*                  m_ShowAllObjectsMenuItem;
	Gtk::CheckMenuItem*             m_SelectLowestMenuItem;
	Gtk::CheckMenuItem*             m_FollowPrimarySelectionMenuItem;
	Gtk::MenuItem*                  m_NavigationMenuItem;
	Gtk::CheckMenuItem*             m_RubberbandMenuItem;
	Gtk::MenuItem*                  m_ToolsMenuItem;
	Gtk::MenuItem*                  m_MotionBlurMenuItem;
	Gtk::MenuItem*                  m_HelpMenuItem;
	Gtk::ImageMenuItem*             m_StandardSizeMenuItem;
	Gtk::ImageMenuItem*             m_InfoMenuItem;
	Gtk::Toolbar*                   m_ToolBar;
	Gtk::ToolButton*                m_NewButton;
	Gtk::MenuToolButton*            m_OpenButton;
	Gtk::ToolButton*                m_ImportButton;
	Gtk::ToolButton*                m_SaveButton;
	Gtk::ToolButton*                m_RefreshButton;
	Gtk::SeparatorToolItem*         m_SeparatorToolItem1;
	Gtk::ToolButton*                m_UndoButton;
	Gtk::ToolButton*                m_RedoButton;
	Gtk::SeparatorToolItem*         m_SeparatorToolItem2;
	Gtk::ToolButton*                m_NodePropertiesEditorButton;
	Gtk::ToolButton*                m_MaterialEditorButton;
	Gtk::ToolButton*                m_UpdateViewpointButton;
	Gtk::ToolButton*                m_CreatePrototypeInstanceButton;
	Gtk::ToolButton*                m_GeometryPropertiesEditorButton;
	Gtk::Paned*                     m_VPaned;
	Gtk::Paned*                     m_HPaned;
	Gtk::Box*                       m_SurfaceBox;
	Gtk::Box*                       m_Dashboard;
	Gtk::Toolbar*                   m_DashboardToolBar;
	Gtk::RadioToolButton*           m_HandButton;
	Gtk::RadioToolButton*           m_ArrowButton;
	Gtk::ToolButton*                m_PlayPauseButton;
	Gtk::SeparatorToolItem*         m_SelectSeparator;
	Gtk::ToolButton*                m_SelectParentButton;
	Gtk::ToolButton*                m_SelectChildrenButton;
	Gtk::SeparatorToolItem*         m_ViewerSeparator;
	Gtk::MenuToolButton*            m_ViewerButton;
	Gtk::ToolButton*                m_StraightenButton;
	Gtk::SeparatorToolItem*         m_LookAtSeparator;
	Gtk::ToolButton*                m_LookAtAllButton;
	Gtk::ToggleToolButton*          m_LookAtButton;
	Gtk::Box*                       m_Footer;
	Gtk::Label*                     m_FooterLabel;
	Gtk::Notebook*                  m_FooterNotebook;
	Gtk::Box*                       m_ConsoleBox;
	Gtk::Box*                       m_SideBar;
	Gtk::Label*                     m_SideBarLabel;
	Gtk::Notebook*                  m_SideBarNotebook;
	Gtk::Box*                       m_ViewpointListBox;
	Gtk::Box*                       m_HistoryEditorBox;
	Gtk::Box*                       m_LibraryViewBox;
	Gtk::Box*                       m_OutlineEditorBox;
	Gtk::FileChooserDialog*         m_FileImportDialog;
	Gtk::FileChooserDialog*         m_FileOpenDialog;
	Gtk::FileChooserDialog*         m_FileSaveDialog;
	Gtk::CheckButton*               m_SaveCompressedButton;
	Gtk::Dialog*                    m_FileSaveWarningDialog;
	Gtk::MessageDialog*             m_MessageDialog;
	Gtk::Dialog*                    m_OpenLocationDialog;
	Gtk::Entry*                     m_OpenLocationEntry;
	Gtk::Dialog*                    m_PrototypeInstanceDialog;
	Gtk::Button*                    m_PrototypeOkButton;
	Gtk::Box*                       m_Widget1;
	Gtk::Label*                     m_HeaderLabel;
	Gtk::MenuButton*                m_MenuButton;
	Gtk::Label*                     m_PrototypeLabel;

};

} // puck
} // titania

#endif

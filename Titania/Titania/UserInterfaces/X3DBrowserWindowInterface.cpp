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
#include "X3DBrowserWindowInterface.h"

namespace titania {
namespace puck {

const std::string X3DBrowserWindowInterface::m_widgetName = "BrowserWindow";

void
X3DBrowserWindowInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_IconFactory = Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (m_builder -> get_object ("IconFactory"));

	// Get widgets.
	m_builder -> get_widget ("ExamineViewerImage", m_ExamineViewerImage);
	m_builder -> get_widget ("FileImportImage", m_FileImportImage);
	m_builder -> get_widget ("FlyViewerImage", m_FlyViewerImage);
	m_builder -> get_widget ("HistoryMenu", m_HistoryMenu);
	m_builder -> get_widget ("NoneViewerImage", m_NoneViewerImage);
	m_builder -> get_widget ("OpenLocationImage", m_OpenLocationImage);
	m_builder -> get_widget ("PlaneViewerImage", m_PlaneViewerImage);
	m_builder -> get_widget ("WalkViewerImage", m_WalkViewerImage);
	m_builder -> get_widget ("ViewerTypeMenu", m_ViewerTypeMenu);
	m_builder -> get_widget ("ExamineViewerMenuItem", m_ExamineViewerMenuItem);
	m_builder -> get_widget ("WalkViewerMenuItem", m_WalkViewerMenuItem);
	m_builder -> get_widget ("FlyViewerMenuItem", m_FlyViewerMenuItem);
	m_builder -> get_widget ("PlaneViewerMenuItem", m_PlaneViewerMenuItem);
	m_builder -> get_widget ("NoneViewerMenuItem", m_NoneViewerMenuItem);
	m_builder -> get_widget ("WorkspacesImage", m_WorkspacesImage);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("MenuBar", m_MenuBar);
	m_builder -> get_widget ("FileMenuItem", m_FileMenuItem);
	m_builder -> get_widget ("NewMenuItem", m_NewMenuItem);
	m_builder -> get_widget ("OpenMenuItem", m_OpenMenuItem);
	m_builder -> get_widget ("OpenLocationMenuItem", m_OpenLocationMenuItem);
	m_builder -> get_widget ("ImportMenuItem", m_ImportMenuItem);
	m_builder -> get_widget ("ImportAsInlineMenuItem", m_ImportAsInlineMenuItem);
	m_builder -> get_widget ("SaveMenuItem", m_SaveMenuItem);
	m_builder -> get_widget ("SaveAsMenuItem", m_SaveAsMenuItem);
	m_builder -> get_widget ("RevertMenuItem", m_RevertMenuItem);
	m_builder -> get_widget ("RemoveUnusedPrototypesMenuItem", m_RemoveUnusedPrototypesMenuItem);
	m_builder -> get_widget ("CloseMenuItem", m_CloseMenuItem);
	m_builder -> get_widget ("QuitMenuItem", m_QuitMenuItem);
	m_builder -> get_widget ("EditMenuItem", m_EditMenuItem);
	m_builder -> get_widget ("UndoMenuItem", m_UndoMenuItem);
	m_builder -> get_widget ("RedoMenuItem", m_RedoMenuItem);
	m_builder -> get_widget ("CutMenuItem", m_CutMenuItem);
	m_builder -> get_widget ("CopyMenuItem", m_CopyMenuItem);
	m_builder -> get_widget ("PasteMenuItem", m_PasteMenuItem);
	m_builder -> get_widget ("DeleteMenuItem", m_DeleteMenuItem);
	m_builder -> get_widget ("CloneMenuItem", m_CloneMenuItem);
	m_builder -> get_widget ("CreateCloneMenuItem", m_CreateCloneMenuItem);
	m_builder -> get_widget ("UnlinkCloneMenuItem", m_UnlinkCloneMenuItem);
	m_builder -> get_widget ("GroupSelectedNodesMenuItem", m_GroupSelectedNodesMenuItem);
	m_builder -> get_widget ("UngroupMenuItem", m_UngroupMenuItem);
	m_builder -> get_widget ("AddToGroupMenuItem", m_AddToGroupMenuItem);
	m_builder -> get_widget ("DetachFromGroupMenuItem", m_DetachFromGroupMenuItem);
	m_builder -> get_widget ("CreateParentMenuItem", m_CreateParentMenuItem);
	m_builder -> get_widget ("CreateParentTransformMenuItem", m_CreateParentTransformMenuItem);
	m_builder -> get_widget ("CreateParentGroupMenuItem", m_CreateParentGroupMenuItem);
	m_builder -> get_widget ("CreateParentSwitchMenuItem", m_CreateParentSwitchMenuItem);
	m_builder -> get_widget ("CreateParentBillboardMenuItem", m_CreateParentBillboardMenuItem);
	m_builder -> get_widget ("CreateParentCollisionMenuItem", m_CreateParentCollisionMenuItem);
	m_builder -> get_widget ("CreateParentLODMenuItem", m_CreateParentLODMenuItem);
	m_builder -> get_widget ("CreateParentAnchorMenuItem", m_CreateParentAnchorMenuItem);
	m_builder -> get_widget ("CreateParentScreenGroupMenuItem", m_CreateParentScreenGroupMenuItem);
	m_builder -> get_widget ("CreateParentLayoutGroupMenuItem", m_CreateParentLayoutGroupMenuItem);
	m_builder -> get_widget ("CreateParentGeoTransformMenuItem", m_CreateParentGeoTransformMenuItem);
	m_builder -> get_widget ("CreateParentGeoLocationMenuItem", m_CreateParentGeoLocationMenuItem);
	m_builder -> get_widget ("CreateParentCADPartMenuItem", m_CreateParentCADPartMenuItem);
	m_builder -> get_widget ("CreateParentCADAssemblyMenuItem", m_CreateParentCADAssemblyMenuItem);
	m_builder -> get_widget ("CreateParentCADLayerMenuItem", m_CreateParentCADLayerMenuItem);
	m_builder -> get_widget ("ViewMenuItem", m_ViewMenuItem);
	m_builder -> get_widget ("ToolBarMenuItem", m_ToolBarMenuItem);
	m_builder -> get_widget ("SideBarMenuItem", m_SideBarMenuItem);
	m_builder -> get_widget ("FooterMenuItem", m_FooterMenuItem);
	m_builder -> get_widget ("EnvironmentMenuItem", m_EnvironmentMenuItem);
	m_builder -> get_widget ("BrowserMenuItem", m_BrowserMenuItem);
	m_builder -> get_widget ("EditorMenuItem", m_EditorMenuItem);
	m_builder -> get_widget ("BrowserOptionsSeparator", m_BrowserOptionsSeparator);
	m_builder -> get_widget ("ShadingMenuItem", m_ShadingMenuItem);
	m_builder -> get_widget ("PhongMenuItem", m_PhongMenuItem);
	m_builder -> get_widget ("GouraudMenuItem", m_GouraudMenuItem);
	m_builder -> get_widget ("FlatMenuItem", m_FlatMenuItem);
	m_builder -> get_widget ("WireFrameMenuItem", m_WireFrameMenuItem);
	m_builder -> get_widget ("PointSetMenuItem", m_PointSetMenuItem);
	m_builder -> get_widget ("PrimitiveQualityMenuItem", m_PrimitiveQualityMenuItem);
	m_builder -> get_widget ("HighQualityMenuItem", m_HighQualityMenuItem);
	m_builder -> get_widget ("MediumQualityMenuItem", m_MediumQualityMenuItem);
	m_builder -> get_widget ("LowQualityMenuItem", m_LowQualityMenuItem);
	m_builder -> get_widget ("ShowHideEnvironmentalEffectsMenuItem", m_ShowHideEnvironmentalEffectsMenuItem);
	m_builder -> get_widget ("BackgroundsMenuItem", m_BackgroundsMenuItem);
	m_builder -> get_widget ("FogsMenuItem", m_FogsMenuItem);
	m_builder -> get_widget ("ObjectIconsMenuItem", m_ObjectIconsMenuItem);
	m_builder -> get_widget ("LightsMenuItem", m_LightsMenuItem);
	m_builder -> get_widget ("ProximitySensorsMenuItem", m_ProximitySensorsMenuItem);
	m_builder -> get_widget ("VisibilitySensorsMenuItem", m_VisibilitySensorsMenuItem);
	m_builder -> get_widget ("ViewpointsMenuItem", m_ViewpointsMenuItem);
	m_builder -> get_widget ("HideAllObjectIconsMenuItem", m_HideAllObjectIconsMenuItem);
	m_builder -> get_widget ("RubberbandMenuItem", m_RubberbandMenuItem);
	m_builder -> get_widget ("RenderingPropertiesMenuItem", m_RenderingPropertiesMenuItem);
	m_builder -> get_widget ("FullScreenMenuItem", m_FullScreenMenuItem);
	m_builder -> get_widget ("UnFullScreenMenuItem", m_UnFullScreenMenuItem);
	m_builder -> get_widget ("SelectionMenuItem", m_SelectionMenuItem);
	m_builder -> get_widget ("SelectAllMenuItem", m_SelectAllMenuItem);
	m_builder -> get_widget ("DeselectAllMenuItem", m_DeselectAllMenuItem);
	m_builder -> get_widget ("HideSelectedObjectsMenuItem", m_HideSelectedObjectsMenuItem);
	m_builder -> get_widget ("HideUnselectedObjectsMenuItem", m_HideUnselectedObjectsMenuItem);
	m_builder -> get_widget ("ShowSelectedObjectsMenuItem", m_ShowSelectedObjectsMenuItem);
	m_builder -> get_widget ("ShowAllObjectsMenuItem", m_ShowAllObjectsMenuItem);
	m_builder -> get_widget ("SelectLowestMenuItem", m_SelectLowestMenuItem);
	m_builder -> get_widget ("FollowPrimarySelectionMenuItem", m_FollowPrimarySelectionMenuItem);
	m_builder -> get_widget ("LayoutMenuItem", m_LayoutMenuItem);
	m_builder -> get_widget ("GridToolMenuItem", m_GridToolMenuItem);
	m_builder -> get_widget ("HelpMenuItem", m_HelpMenuItem);
	m_builder -> get_widget ("StandardSizeMenuItem", m_StandardSizeMenuItem);
	m_builder -> get_widget ("InfoMenuItem", m_InfoMenuItem);
	m_builder -> get_widget ("ToolBar", m_ToolBar);
	m_builder -> get_widget ("LocationBar", m_LocationBar);
	m_builder -> get_widget ("LocationBar1", m_LocationBar1);
	m_builder -> get_widget ("HomeButton", m_HomeButton);
	m_builder -> get_widget ("PreviousButton", m_PreviousButton);
	m_builder -> get_widget ("NextButton", m_NextButton);
	m_builder -> get_widget ("LocationEntry", m_LocationEntry);
	m_builder -> get_widget ("LocationBar2", m_LocationBar2);
	m_builder -> get_widget ("ReloadButton", m_ReloadButton);
	m_builder -> get_widget ("EditToolBar", m_EditToolBar);
	m_builder -> get_widget ("NewButton", m_NewButton);
	m_builder -> get_widget ("OpenButton", m_OpenButton);
	m_builder -> get_widget ("ImportButton", m_ImportButton);
	m_builder -> get_widget ("SaveButton", m_SaveButton);
	m_builder -> get_widget ("RefreshButton", m_RefreshButton);
	m_builder -> get_widget ("SeparatorToolItem1", m_SeparatorToolItem1);
	m_builder -> get_widget ("UndoButton", m_UndoButton);
	m_builder -> get_widget ("RedoButton", m_RedoButton);
	m_builder -> get_widget ("SeparatorToolItem2", m_SeparatorToolItem2);
	m_builder -> get_widget ("NodePropertiesEditorButton", m_NodePropertiesEditorButton);
	m_builder -> get_widget ("AppearanceEditorButton", m_AppearanceEditorButton);
	m_builder -> get_widget ("TextureEditorButton", m_TextureEditorButton);
	m_builder -> get_widget ("TextEditorButton", m_TextEditorButton);
	m_builder -> get_widget ("GeometryPropertiesEditorButton", m_GeometryPropertiesEditorButton);
	m_builder -> get_widget ("ViewpointEditorButton", m_ViewpointEditorButton);
	m_builder -> get_widget ("LightEditorButton", m_LightEditorButton);
	m_builder -> get_widget ("LODEditorButton", m_LODEditorButton);
	m_builder -> get_widget ("InlineEditorButton", m_InlineEditorButton);
	m_builder -> get_widget ("PrecisionPlacementPanelButton", m_PrecisionPlacementPanelButton);
	m_builder -> get_widget ("CreatePrototypeInstanceButton", m_CreatePrototypeInstanceButton);
	m_builder -> get_widget ("NodeIndexButton", m_NodeIndexButton);
	m_builder -> get_widget ("VPaned", m_VPaned);
	m_builder -> get_widget ("HPaned", m_HPaned);
	m_builder -> get_widget ("SplashBox", m_SplashBox);
	m_builder -> get_widget ("BrowserNotebook", m_BrowserNotebook);
	m_builder -> get_widget ("Dashboard", m_Dashboard);
	m_builder -> get_widget ("DashboardToolBar", m_DashboardToolBar);
	m_builder -> get_widget ("TabToolButton", m_TabToolButton);
	m_builder -> get_widget ("HandButton", m_HandButton);
	m_builder -> get_widget ("ArrowButton", m_ArrowButton);
	m_builder -> get_widget ("PlayPauseButton", m_PlayPauseButton);
	m_builder -> get_widget ("SelectSeparator", m_SelectSeparator);
	m_builder -> get_widget ("SelectParentButton", m_SelectParentButton);
	m_builder -> get_widget ("SelectChildrenButton", m_SelectChildrenButton);
	m_builder -> get_widget ("ViewerSeparator", m_ViewerSeparator);
	m_builder -> get_widget ("ViewerButton", m_ViewerButton);
	m_builder -> get_widget ("StraightenButton", m_StraightenButton);
	m_builder -> get_widget ("LookAtSeparator", m_LookAtSeparator);
	m_builder -> get_widget ("LookAtSelectionButton", m_LookAtSelectionButton);
	m_builder -> get_widget ("LookAtAllButton", m_LookAtAllButton);
	m_builder -> get_widget ("LookAtButton", m_LookAtButton);
	m_builder -> get_widget ("Footer", m_Footer);
	m_builder -> get_widget ("FooterLabel", m_FooterLabel);
	m_builder -> get_widget ("FooterNotebook", m_FooterNotebook);
	m_builder -> get_widget ("ConsoleBox", m_ConsoleBox);
	m_builder -> get_widget ("ScriptEditorBox", m_ScriptEditorBox);
	m_builder -> get_widget ("SideBar", m_SideBar);
	m_builder -> get_widget ("SideBarLabel", m_SideBarLabel);
	m_builder -> get_widget ("SideBarNotebook", m_SideBarNotebook);
	m_builder -> get_widget ("ViewpointListBox", m_ViewpointListBox);
	m_builder -> get_widget ("HistoryViewBox", m_HistoryViewBox);
	m_builder -> get_widget ("LibraryViewBox", m_LibraryViewBox);
	m_builder -> get_widget ("OutlineEditorBox", m_OutlineEditorBox);

	// Connect object Gtk::ImageMenuItem with id 'ExamineViewerMenuItem'.
	m_ExamineViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_examine_viewer_activate));
	m_WalkViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_walk_viewer_activate));
	m_FlyViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fly_viewer_activate));
	m_PlaneViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_plane_viewer_activate));
	m_NoneViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_none_viewer_activate));

	// Connect object Gtk::ApplicationWindow with id 'Window'.
	m_Window -> signal_focus_out_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_focus_out_event));
	m_Window -> signal_key_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_key_press_event), false);
	m_Window -> signal_key_release_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_key_release_event), false);

	// Connect object Gtk::MenuBar with id 'MenuBar'.
	m_MenuBar -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_menubar_button_press_event), false);

	// Connect object Gtk::ImageMenuItem with id 'NewMenuItem'.
	m_NewMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new));
	m_OpenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open));
	m_OpenLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_location));
	m_ImportMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import));

	// Connect object Gtk::CheckMenuItem with id 'ImportAsInlineMenuItem'.
	m_ImportAsInlineMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import_as_inline_toggled));

	// Connect object Gtk::ImageMenuItem with id 'SaveMenuItem'.
	m_SaveMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save));
	m_SaveAsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_as));
	m_RevertMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved));
	m_RemoveUnusedPrototypesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_remove_unused_prototypes));
	m_CloseMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_close));
	m_QuitMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_quit));
	m_UndoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_undo));
	m_RedoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_redo));
	m_CutMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_cut_nodes_activate));
	m_CopyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_copy_nodes_activate));
	m_PasteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_paste_nodes_activate));
	m_DeleteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_delete_nodes_activate));

	// Connect object Gtk::MenuItem with id 'CreateCloneMenuItem'.
	m_CreateCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_clone_activate));
	m_UnlinkCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_unlink_clone_activate));

	// Connect object Gtk::ImageMenuItem with id 'GroupSelectedNodesMenuItem'.
	m_GroupSelectedNodesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_group_selected_nodes_activate));
	m_UngroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_ungroup_node_activate));

	// Connect object Gtk::MenuItem with id 'AddToGroupMenuItem'.
	m_AddToGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_add_to_group_activate));
	m_DetachFromGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_detach_from_group_activate));
	m_CreateParentTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_transform_activate));
	m_CreateParentGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_group_activate));
	m_CreateParentSwitchMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_switch_activate));
	m_CreateParentBillboardMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_billboard_activate));
	m_CreateParentCollisionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_collision_activate));
	m_CreateParentLODMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_lod_activate));
	m_CreateParentAnchorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_anchor_activate));
	m_CreateParentScreenGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_screen_group_activate));
	m_CreateParentLayoutGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layout_group_activate));
	m_CreateParentGeoTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_geo_transform_activate));
	m_CreateParentGeoLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_geo_location_activate));
	m_CreateParentCADPartMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_part_activate));
	m_CreateParentCADAssemblyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_assembly_activate));
	m_CreateParentCADLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_layer_activate));

	// Connect object Gtk::CheckMenuItem with id 'ToolBarMenuItem'.
	m_ToolBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolBar_toggled));
	m_SideBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_sideBar_toggled));
	m_FooterMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_footer_toggled));

	// Connect object Gtk::RadioMenuItem with id 'BrowserMenuItem'.
	m_BrowserMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_browser_toggled));
	m_EditorMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_editor_toggled));
	m_PhongMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_phong_activate));
	m_GouraudMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_gouraud_activate));
	m_FlatMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_flat_activate));
	m_WireFrameMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_wireframe_activate));
	m_PointSetMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_pointset_activate));
	m_HighQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_high_quality_activate));
	m_MediumQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_medium_quality_activate));
	m_LowQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_low_quality_activate));

	// Connect object Gtk::CheckMenuItem with id 'BackgroundsMenuItem'.
	m_BackgroundsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_backgrounds_toggled));
	m_FogsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fogs_toggled));
	m_LightsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_lights_toggled));
	m_ProximitySensorsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_proximity_sensors_toggled));
	m_VisibilitySensorsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_visibility_sensors_toggled));
	m_ViewpointsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_viewpoints_toggled));

	// Connect object Gtk::MenuItem with id 'HideAllObjectIconsMenuItem'.
	m_HideAllObjectIconsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_all_object_icons_activate));

	// Connect object Gtk::CheckMenuItem with id 'RubberbandMenuItem'.
	m_RubberbandMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_rubberband_toggled));
	m_RenderingPropertiesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_rendering_properties_toggled));

	// Connect object Gtk::ImageMenuItem with id 'FullScreenMenuItem'.
	m_FullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fullscreen));
	m_UnFullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_unfullscreen));
	m_SelectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_select_all_activate));

	// Connect object Gtk::MenuItem with id 'DeselectAllMenuItem'.
	m_DeselectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_deselect_all_activate));
	m_HideSelectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_selected_objects_activate));
	m_HideUnselectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_unselected_objects_activate));
	m_ShowSelectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_show_selected_objects_activate));
	m_ShowAllObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_show_all_objects_activate));

	// Connect object Gtk::CheckMenuItem with id 'SelectLowestMenuItem'.
	m_SelectLowestMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_select_lowest_toggled));
	m_FollowPrimarySelectionMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_follow_primary_selection_toggled));

	// Connect object Gtk::MenuItem with id 'GridToolMenuItem'.
	m_GridToolMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_grid_tool_activate));

	// Connect object Gtk::ImageMenuItem with id 'StandardSizeMenuItem'.
	m_StandardSizeMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_standard_size));
	m_InfoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_info));

	// Connect object Gtk::Box with id 'ToolBar'.
	m_ToolBar -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolbar_drag_data_received));

	// Connect object Gtk::Toolbar with id 'LocationBar1'.
	m_LocationBar1 -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolbar_drag_data_received));

	// Connect object Gtk::ToolButton with id 'HomeButton'.
	m_HomeButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_home));
	m_PreviousButton -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_previous_button_press_event));
	m_PreviousButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_previous_page));
	m_NextButton -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_next_button_press_event));
	m_NextButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_next_page));

	// Connect object Gtk::Entry with id 'LocationEntry'.
	m_LocationEntry -> signal_icon_release () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_location_icon_release));
	m_LocationEntry -> signal_key_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_location_key_press_event), false);

	// Connect object Gtk::ToolButton with id 'ReloadButton'.
	m_ReloadButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved));
	m_NewButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new));

	// Connect object Gtk::MenuToolButton with id 'OpenButton'.
	m_OpenButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open));

	// Connect object Gtk::ToolButton with id 'ImportButton'.
	m_ImportButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import));
	m_SaveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save));
	m_RefreshButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved));
	m_UndoButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_undo));
	m_RedoButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_redo));
	m_NodePropertiesEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_node_properties_editor_clicked));
	m_AppearanceEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_appearance_editor_clicked));
	m_TextureEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_editor_clicked));
	m_TextEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_text_editor_clicked));
	m_GeometryPropertiesEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_geometry_properties_editor_clicked));
	m_ViewpointEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_viewpoint_editor_clicked));
	m_LightEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_light_editor_clicked));
	m_LODEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_lod_editor_clicked));
	m_InlineEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_inline_editor_clicked));
	m_PrecisionPlacementPanelButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_precision_placement_panel_clicked));
	m_CreatePrototypeInstanceButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_prototype_instance_dialog_clicked));
	m_NodeIndexButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_node_index_clicked));

	// Connect object Gtk::Notebook with id 'BrowserNotebook'.
	m_BrowserNotebook -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_browser_drag_data_received));
	m_BrowserNotebook -> signal_page_reordered () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_browser_reordered));
	m_BrowserNotebook -> signal_switch_page () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_switch_browser));

	// Connect object Gtk::RadioToolButton with id 'HandButton'.
	m_HandButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hand_button_toggled));
	m_ArrowButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_arrow_button_toggled));

	// Connect object Gtk::ToolButton with id 'PlayPauseButton'.
	m_PlayPauseButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_play_pause_button_clicked));
	m_SelectParentButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_select_parent_button_clicked));
	m_SelectChildrenButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_select_children_button_clicked));

	// Connect object Gtk::MenuToolButton with id 'ViewerButton'.
	m_ViewerButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_viewer_clicked));

	// Connect object Gtk::ToolButton with id 'StraightenButton'.
	m_StraightenButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_straighten_clicked));
	m_LookAtSelectionButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_selection_clicked));
	m_LookAtAllButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_all_clicked));

	// Connect object Gtk::ToggleToolButton with id 'LookAtButton'.
	m_LookAtButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_toggled));

	// Call construct handler of base class.
	construct ();
}

X3DBrowserWindowInterface::~X3DBrowserWindowInterface ()
{ }

} // puck
} // titania

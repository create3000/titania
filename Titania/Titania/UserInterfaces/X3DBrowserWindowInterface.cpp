/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
	m_IconFactory                  = Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (m_builder -> get_object ("IconFactory"));
	m_AngleLayoutToolAction        = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("AngleLayoutToolAction"));
	m_BackgroundsAction            = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("BackgroundsAction"));
	m_BrowserAction                = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("BrowserAction"));
	m_EditorAction                 = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("EditorAction"));
	m_FlatAction                   = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("FlatAction"));
	m_FogsAction                   = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("FogsAction"));
	m_FollowPrimarySelectionAction = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("FollowPrimarySelectionAction"));
	m_FooterAction                 = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("FooterAction"));
	m_GouraudAction                = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("GouraudAction"));
	m_GridLayoutToolAction         = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("GridLayoutToolAction"));
	m_LightsAction                 = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("LightsAction"));
	m_MenubarAction                = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("MenubarAction"));
	m_PhongAction                  = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("PhongAction"));
	m_PointsetAction               = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("PointsetAction"));
	m_PrimitiveQualityHighAction   = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("PrimitiveQualityHighAction"));
	m_PrimitiveQualityLowAction    = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("PrimitiveQualityLowAction"));
	m_PrimitiveQualityMediumAction = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("PrimitiveQualityMediumAction"));
	m_ProximitySensorsAction       = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("ProximitySensorsAction"));
	m_RenderingPropertiesAction    = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("RenderingPropertiesAction"));
	m_RubberbandAction             = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("RubberbandAction"));
	m_SelectLowestAction           = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("SelectLowestAction"));
	m_SidebarAction                = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("SidebarAction"));
	m_SoundsAction                 = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("SoundsAction"));
	m_TabsAction                   = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("TabsAction"));
	m_TextureQualityHighAction     = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("TextureQualityHighAction"));
	m_TextureQualityLowAction      = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("TextureQualityLowAction"));
	m_TextureQualityMediumAction   = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("TextureQualityMediumAction"));
	m_ToolbarAction                = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("ToolbarAction"));
	m_TransformSensorsAction       = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("TransformSensorsAction"));
	m_TransformToolModeAction      = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("TransformToolModeAction"));
	m_ViewpointsAction             = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("ViewpointsAction"));
	m_VisibilitySensorsAction      = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("VisibilitySensorsAction"));
	m_WireframeAction              = Glib::RefPtr <Gtk::ToggleAction>::cast_dynamic (m_builder -> get_object ("WireframeAction"));

	// Get widgets.
	m_builder -> get_widget ("ExamineViewerImage", m_ExamineViewerImage);
	m_builder -> get_widget ("FileImportImage", m_FileImportImage);
	m_builder -> get_widget ("FileImportImage1", m_FileImportImage1);
	m_builder -> get_widget ("FlyViewerImage", m_FlyViewerImage);
	m_builder -> get_widget ("HistoryMenu", m_HistoryMenu);
	m_builder -> get_widget ("NoneViewerImage", m_NoneViewerImage);
	m_builder -> get_widget ("OpenLocationImage", m_OpenLocationImage);
	m_builder -> get_widget ("OpenLocationImage1", m_OpenLocationImage1);
	m_builder -> get_widget ("PlaneViewerImage", m_PlaneViewerImage);
	m_builder -> get_widget ("WalkViewerImage", m_WalkViewerImage);
	m_builder -> get_widget ("ViewerTypeMenu", m_ViewerTypeMenu);
	m_builder -> get_widget ("ExamineViewerMenuItem", m_ExamineViewerMenuItem);
	m_builder -> get_widget ("WalkViewerMenuItem", m_WalkViewerMenuItem);
	m_builder -> get_widget ("FlyViewerMenuItem", m_FlyViewerMenuItem);
	m_builder -> get_widget ("PlaneViewerMenuItem", m_PlaneViewerMenuItem);
	m_builder -> get_widget ("NoneViewerMenuItem", m_NoneViewerMenuItem);
	m_builder -> get_widget ("WorkspacesImage", m_WorkspacesImage);
	m_builder -> get_widget ("WorkspacesImage1", m_WorkspacesImage1);
	m_builder -> get_widget ("BrowserMenu", m_BrowserMenu);
	m_builder -> get_widget ("BrowserFileMenuItem", m_BrowserFileMenuItem);
	m_builder -> get_widget ("BrowserNewMenuItem", m_BrowserNewMenuItem);
	m_builder -> get_widget ("BrowserOpenMenuItem", m_BrowserOpenMenuItem);
	m_builder -> get_widget ("BrowserOpenRecentMenuItem", m_BrowserOpenRecentMenuItem);
	m_builder -> get_widget ("BrowserOpenLocationMenuItem", m_BrowserOpenLocationMenuItem);
	m_builder -> get_widget ("BrowserImportMenuItem", m_BrowserImportMenuItem);
	m_builder -> get_widget ("BrowserSaveMenuItem", m_BrowserSaveMenuItem);
	m_builder -> get_widget ("BrowserSaveAsMenuItem", m_BrowserSaveAsMenuItem);
	m_builder -> get_widget ("BrowserSaveACopyMenuItem", m_BrowserSaveACopyMenuItem);
	m_builder -> get_widget ("BrowserExportMenuItem", m_BrowserExportMenuItem);
	m_builder -> get_widget ("BrowserRevertMenuItem", m_BrowserRevertMenuItem);
	m_builder -> get_widget ("BrowserRemoveUnusedPrototypesMenuItem", m_BrowserRemoveUnusedPrototypesMenuItem);
	m_builder -> get_widget ("BrowserScenePropertiesMenuItem", m_BrowserScenePropertiesMenuItem);
	m_builder -> get_widget ("BrowserCloseMenuItem", m_BrowserCloseMenuItem);
	m_builder -> get_widget ("BrowserQuitMenuItem", m_BrowserQuitMenuItem);
	m_builder -> get_widget ("BrowserEditMenuItem", m_BrowserEditMenuItem);
	m_builder -> get_widget ("BrowserUndoMenuItem", m_BrowserUndoMenuItem);
	m_builder -> get_widget ("BrowserRedoMenuItem", m_BrowserRedoMenuItem);
	m_builder -> get_widget ("BrowserCutMenuItem", m_BrowserCutMenuItem);
	m_builder -> get_widget ("BrowserCopyMenuItem", m_BrowserCopyMenuItem);
	m_builder -> get_widget ("BrowserPasteMenuItem", m_BrowserPasteMenuItem);
	m_builder -> get_widget ("BrowserDeleteMenuItem", m_BrowserDeleteMenuItem);
	m_builder -> get_widget ("BrowserCloneMenuItem", m_BrowserCloneMenuItem);
	m_builder -> get_widget ("BrowserCreateCloneMenuItem", m_BrowserCreateCloneMenuItem);
	m_builder -> get_widget ("BrowserUnlinkCloneMenuItem", m_BrowserUnlinkCloneMenuItem);
	m_builder -> get_widget ("BrowserGroupSelectedNodesMenuItem", m_BrowserGroupSelectedNodesMenuItem);
	m_builder -> get_widget ("BrowserUngroupMenuItem", m_BrowserUngroupMenuItem);
	m_builder -> get_widget ("BrowserAddToGroupMenuItem", m_BrowserAddToGroupMenuItem);
	m_builder -> get_widget ("BrowserDetachFromGroupMenuItem", m_BrowserDetachFromGroupMenuItem);
	m_builder -> get_widget ("BrowserCreateParentMenuItem", m_BrowserCreateParentMenuItem);
	m_builder -> get_widget ("BrowserCreateParentTransformMenuItem", m_BrowserCreateParentTransformMenuItem);
	m_builder -> get_widget ("BrowserCreateParentGroupMenuItem", m_BrowserCreateParentGroupMenuItem);
	m_builder -> get_widget ("BrowserCreateParentStaticGroupMenuItem", m_BrowserCreateParentStaticGroupMenuItem);
	m_builder -> get_widget ("BrowserCreateParentSwitchMenuItem", m_BrowserCreateParentSwitchMenuItem);
	m_builder -> get_widget ("BrowserCreateParentBillboardMenuItem", m_BrowserCreateParentBillboardMenuItem);
	m_builder -> get_widget ("BrowserCreateParentCollisionMenuItem", m_BrowserCreateParentCollisionMenuItem);
	m_builder -> get_widget ("BrowserCreateParentLODMenuItem", m_BrowserCreateParentLODMenuItem);
	m_builder -> get_widget ("BrowserCreateParentViewpointGroupMenuItem", m_BrowserCreateParentViewpointGroupMenuItem);
	m_builder -> get_widget ("BrowserCreateParentAnchorMenuItem", m_BrowserCreateParentAnchorMenuItem);
	m_builder -> get_widget ("BrowserCreateParentLayoutLayerMenuItem", m_BrowserCreateParentLayoutLayerMenuItem);
	m_builder -> get_widget ("BrowserCreateParentScreenGroupMenuItem", m_BrowserCreateParentScreenGroupMenuItem);
	m_builder -> get_widget ("BrowserCreateParentLayoutGroupMenuItem", m_BrowserCreateParentLayoutGroupMenuItem);
	m_builder -> get_widget ("BrowserCreateParentGeoTransformMenuItem", m_BrowserCreateParentGeoTransformMenuItem);
	m_builder -> get_widget ("BrowserCreateParentGeoLocationMenuItem", m_BrowserCreateParentGeoLocationMenuItem);
	m_builder -> get_widget ("BrowserCreateParentCADPartMenuItem", m_BrowserCreateParentCADPartMenuItem);
	m_builder -> get_widget ("BrowserCreateParentCADAssemblyMenuItem", m_BrowserCreateParentCADAssemblyMenuItem);
	m_builder -> get_widget ("BrowserCreateParentCADLayerMenuItem", m_BrowserCreateParentCADLayerMenuItem);
	m_builder -> get_widget ("BrowserCreateParentLayerSetMenuItem", m_BrowserCreateParentLayerSetMenuItem);
	m_builder -> get_widget ("BrowserCreateParentLayerMenuItem", m_BrowserCreateParentLayerMenuItem);
	m_builder -> get_widget ("BrowserCreateParentViewportMenuItem", m_BrowserCreateParentViewportMenuItem);
	m_builder -> get_widget ("BrowserViewMenuItem", m_BrowserViewMenuItem);
	m_builder -> get_widget ("BrowserMenubarMenuItem", m_BrowserMenubarMenuItem);
	m_builder -> get_widget ("BrowserToolbarMenuItem", m_BrowserToolbarMenuItem);
	m_builder -> get_widget ("BrowserSidebarMenuItem", m_BrowserSidebarMenuItem);
	m_builder -> get_widget ("BrowserFooterMenuItem", m_BrowserFooterMenuItem);
	m_builder -> get_widget ("BrowserTabsMenuItem", m_BrowserTabsMenuItem);
	m_builder -> get_widget ("BrowserEnvironmentMenuItem", m_BrowserEnvironmentMenuItem);
	m_builder -> get_widget ("BrowserEditorMenuItem", m_BrowserEditorMenuItem);
	m_builder -> get_widget ("BrowserBrowserMenuItem", m_BrowserBrowserMenuItem);
	m_builder -> get_widget ("BrowserBrowserOptionsSeparator", m_BrowserBrowserOptionsSeparator);
	m_builder -> get_widget ("BrowserMotionBlurMenuItem", m_BrowserMotionBlurMenuItem);
	m_builder -> get_widget ("BrowserShadingMenuItem", m_BrowserShadingMenuItem);
	m_builder -> get_widget ("BrowserPhongMenuItem", m_BrowserPhongMenuItem);
	m_builder -> get_widget ("BrowserGouraudMenuItem", m_BrowserGouraudMenuItem);
	m_builder -> get_widget ("BrowserFlatMenuItem", m_BrowserFlatMenuItem);
	m_builder -> get_widget ("BrowserWireframeMenuItem", m_BrowserWireframeMenuItem);
	m_builder -> get_widget ("BrowserPointsetMenuItem", m_BrowserPointsetMenuItem);
	m_builder -> get_widget ("BrowserPrimitiveQualityMenuItem", m_BrowserPrimitiveQualityMenuItem);
	m_builder -> get_widget ("BrowserPrimitiveQualityHighMenuItem", m_BrowserPrimitiveQualityHighMenuItem);
	m_builder -> get_widget ("BrowserPrimitiveQualityMediumMenuItem", m_BrowserPrimitiveQualityMediumMenuItem);
	m_builder -> get_widget ("BrowserPrimitiveQualityLowMenuItem", m_BrowserPrimitiveQualityLowMenuItem);
	m_builder -> get_widget ("BrowserTextureQualityMenuItem", m_BrowserTextureQualityMenuItem);
	m_builder -> get_widget ("BrowserTextureQualityHighMenuItem", m_BrowserTextureQualityHighMenuItem);
	m_builder -> get_widget ("BrowserTextureQualityMediumMenuItem", m_BrowserTextureQualityMediumMenuItem);
	m_builder -> get_widget ("BrowserTextureQualityLowMenuItem", m_BrowserTextureQualityLowMenuItem);
	m_builder -> get_widget ("BrowserShowHideEnvironmentalEffectsMenuItem", m_BrowserShowHideEnvironmentalEffectsMenuItem);
	m_builder -> get_widget ("BrowserBackgroundsMenuItem", m_BrowserBackgroundsMenuItem);
	m_builder -> get_widget ("BrowserFogsMenuItem", m_BrowserFogsMenuItem);
	m_builder -> get_widget ("BrowserObjectIconsMenuItem", m_BrowserObjectIconsMenuItem);
	m_builder -> get_widget ("BrowserLightsMenuItem", m_BrowserLightsMenuItem);
	m_builder -> get_widget ("BrowserProximitySensorsMenuItem", m_BrowserProximitySensorsMenuItem);
	m_builder -> get_widget ("BrowserTransformSensorsMenuItem", m_BrowserTransformSensorsMenuItem);
	m_builder -> get_widget ("BrowserSoundsMenuItem", m_BrowserSoundsMenuItem);
	m_builder -> get_widget ("BrowserVisibilitySensorsMenuItem", m_BrowserVisibilitySensorsMenuItem);
	m_builder -> get_widget ("BrowserViewpointsMenuItem", m_BrowserViewpointsMenuItem);
	m_builder -> get_widget ("BrowserHideAllObjectIconsMenuItem", m_BrowserHideAllObjectIconsMenuItem);
	m_builder -> get_widget ("BrowserRubberbandMenuItem", m_BrowserRubberbandMenuItem);
	m_builder -> get_widget ("BrowserRenderingPropertiesMenuItem", m_BrowserRenderingPropertiesMenuItem);
	m_builder -> get_widget ("BrowserFullScreenMenuItem", m_BrowserFullScreenMenuItem);
	m_builder -> get_widget ("BrowserUnFullScreenMenuItem", m_BrowserUnFullScreenMenuItem);
	m_builder -> get_widget ("BrowserSelectionMenuItem", m_BrowserSelectionMenuItem);
	m_builder -> get_widget ("BrowserSelectAllMenuItem", m_BrowserSelectAllMenuItem);
	m_builder -> get_widget ("BrowserDeselectAllMenuItem", m_BrowserDeselectAllMenuItem);
	m_builder -> get_widget ("BrowserHideSelectedObjectsMenuItem", m_BrowserHideSelectedObjectsMenuItem);
	m_builder -> get_widget ("BrowserHideUnselectedObjectsMenuItem", m_BrowserHideUnselectedObjectsMenuItem);
	m_builder -> get_widget ("BrowserShowSelectedObjectsMenuItem", m_BrowserShowSelectedObjectsMenuItem);
	m_builder -> get_widget ("BrowserShowAllObjectsMenuItem", m_BrowserShowAllObjectsMenuItem);
	m_builder -> get_widget ("BrowserSelectLowestMenuItem", m_BrowserSelectLowestMenuItem);
	m_builder -> get_widget ("BrowserFollowPrimarySelectionMenuItem", m_BrowserFollowPrimarySelectionMenuItem);
	m_builder -> get_widget ("BrowserTransformToolModeMenuItem", m_BrowserTransformToolModeMenuItem);
	m_builder -> get_widget ("BrowserGeometryMenuItem", m_BrowserGeometryMenuItem);
	m_builder -> get_widget ("BrowserUnionMenuItem", m_BrowserUnionMenuItem);
	m_builder -> get_widget ("BrowserDifferenceMenuItem", m_BrowserDifferenceMenuItem);
	m_builder -> get_widget ("BrowserIntersectionMenuItem", m_BrowserIntersectionMenuItem);
	m_builder -> get_widget ("BrowserExclusionMenuItem", m_BrowserExclusionMenuItem);
	m_builder -> get_widget ("BrowserFusionMenuItem", m_BrowserFusionMenuItem);
	m_builder -> get_widget ("BrowserCombineMenuItem", m_BrowserCombineMenuItem);
	m_builder -> get_widget ("BrowserTransformToZeroMenuItem", m_BrowserTransformToZeroMenuItem);
	m_builder -> get_widget ("BrowserColorPerVertexMenuItem", m_BrowserColorPerVertexMenuItem);
	m_builder -> get_widget ("BrowserTextureCoordinateEditorMenuItem", m_BrowserTextureCoordinateEditorMenuItem);
	m_builder -> get_widget ("BrowserLayoutMenuItem", m_BrowserLayoutMenuItem);
	m_builder -> get_widget ("BrowserGridLayoutToolMenuItem", m_BrowserGridLayoutToolMenuItem);
	m_builder -> get_widget ("BrowserAngleLayoutToolMenuItem", m_BrowserAngleLayoutToolMenuItem);
	m_builder -> get_widget ("BrowserGridPropertiesMenuItem", m_BrowserGridPropertiesMenuItem);
	m_builder -> get_widget ("BrowserScenesMenuItem", m_BrowserScenesMenuItem);
	m_builder -> get_widget ("BrowserScenesMenu", m_BrowserScenesMenu);
	m_builder -> get_widget ("BrowserHelpMenuItem", m_BrowserHelpMenuItem);
	m_builder -> get_widget ("BrowserInfoMenuItem", m_BrowserInfoMenuItem);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBox", m_HeaderBox);
	m_builder -> get_widget ("HeaderSubBox", m_HeaderSubBox);
	m_builder -> get_widget ("Menubar", m_Menubar);
	m_builder -> get_widget ("FileMenuItem", m_FileMenuItem);
	m_builder -> get_widget ("NewMenuItem", m_NewMenuItem);
	m_builder -> get_widget ("OpenMenuItem", m_OpenMenuItem);
	m_builder -> get_widget ("OpenRecentMenuItem", m_OpenRecentMenuItem);
	m_builder -> get_widget ("OpenLocationMenuItem", m_OpenLocationMenuItem);
	m_builder -> get_widget ("ImportMenuItem", m_ImportMenuItem);
	m_builder -> get_widget ("SaveMenuItem", m_SaveMenuItem);
	m_builder -> get_widget ("SaveAsMenuItem", m_SaveAsMenuItem);
	m_builder -> get_widget ("SaveACopyMenuItem", m_SaveACopyMenuItem);
	m_builder -> get_widget ("ExportMenuItem", m_ExportMenuItem);
	m_builder -> get_widget ("RevertMenuItem", m_RevertMenuItem);
	m_builder -> get_widget ("RemoveUnusedPrototypesMenuItem", m_RemoveUnusedPrototypesMenuItem);
	m_builder -> get_widget ("ScenePropertiesMenuItem", m_ScenePropertiesMenuItem);
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
	m_builder -> get_widget ("CreateParentStaticGroupMenuItem", m_CreateParentStaticGroupMenuItem);
	m_builder -> get_widget ("CreateParentSwitchMenuItem", m_CreateParentSwitchMenuItem);
	m_builder -> get_widget ("CreateParentBillboardMenuItem", m_CreateParentBillboardMenuItem);
	m_builder -> get_widget ("CreateParentCollisionMenuItem", m_CreateParentCollisionMenuItem);
	m_builder -> get_widget ("CreateParentLODMenuItem", m_CreateParentLODMenuItem);
	m_builder -> get_widget ("CreateParentViewpointGroupMenuItem", m_CreateParentViewpointGroupMenuItem);
	m_builder -> get_widget ("CreateParentAnchorMenuItem", m_CreateParentAnchorMenuItem);
	m_builder -> get_widget ("CreateParentLayoutLayerMenuItem", m_CreateParentLayoutLayerMenuItem);
	m_builder -> get_widget ("CreateParentScreenGroupMenuItem", m_CreateParentScreenGroupMenuItem);
	m_builder -> get_widget ("CreateParentLayoutGroupMenuItem", m_CreateParentLayoutGroupMenuItem);
	m_builder -> get_widget ("CreateParentGeoTransformMenuItem", m_CreateParentGeoTransformMenuItem);
	m_builder -> get_widget ("CreateParentGeoLocationMenuItem", m_CreateParentGeoLocationMenuItem);
	m_builder -> get_widget ("CreateParentCADPartMenuItem", m_CreateParentCADPartMenuItem);
	m_builder -> get_widget ("CreateParentCADAssemblyMenuItem", m_CreateParentCADAssemblyMenuItem);
	m_builder -> get_widget ("CreateParentCADLayerMenuItem", m_CreateParentCADLayerMenuItem);
	m_builder -> get_widget ("CreateParentLayerSetMenuItem", m_CreateParentLayerSetMenuItem);
	m_builder -> get_widget ("CreateParentLayerMenuItem", m_CreateParentLayerMenuItem);
	m_builder -> get_widget ("CreateParentViewportMenuItem", m_CreateParentViewportMenuItem);
	m_builder -> get_widget ("ViewMenuItem", m_ViewMenuItem);
	m_builder -> get_widget ("MenubarMenuItem", m_MenubarMenuItem);
	m_builder -> get_widget ("ToolbarMenuItem", m_ToolbarMenuItem);
	m_builder -> get_widget ("SidebarMenuItem", m_SidebarMenuItem);
	m_builder -> get_widget ("FooterMenuItem", m_FooterMenuItem);
	m_builder -> get_widget ("TabsMenuItem", m_TabsMenuItem);
	m_builder -> get_widget ("EnvironmentMenuItem", m_EnvironmentMenuItem);
	m_builder -> get_widget ("EditorMenuItem", m_EditorMenuItem);
	m_builder -> get_widget ("BrowserMenuItem", m_BrowserMenuItem);
	m_builder -> get_widget ("BrowserOptionsSeparator", m_BrowserOptionsSeparator);
	m_builder -> get_widget ("MotionBlurMenuItem", m_MotionBlurMenuItem);
	m_builder -> get_widget ("ShadingMenuItem", m_ShadingMenuItem);
	m_builder -> get_widget ("PhongMenuItem", m_PhongMenuItem);
	m_builder -> get_widget ("GouraudMenuItem", m_GouraudMenuItem);
	m_builder -> get_widget ("FlatMenuItem", m_FlatMenuItem);
	m_builder -> get_widget ("WireframeMenuItem", m_WireframeMenuItem);
	m_builder -> get_widget ("PointsetMenuItem", m_PointsetMenuItem);
	m_builder -> get_widget ("PrimitiveQualityMenuItem", m_PrimitiveQualityMenuItem);
	m_builder -> get_widget ("PrimitiveQualityHighMenuItem", m_PrimitiveQualityHighMenuItem);
	m_builder -> get_widget ("PrimitiveQualityMediumMenuItem", m_PrimitiveQualityMediumMenuItem);
	m_builder -> get_widget ("PrimitiveQualityLowMenuItem", m_PrimitiveQualityLowMenuItem);
	m_builder -> get_widget ("TextureQualityMenuItem", m_TextureQualityMenuItem);
	m_builder -> get_widget ("TextureQualityHighMenuItem", m_TextureQualityHighMenuItem);
	m_builder -> get_widget ("TextureQualityMediumMenuItem", m_TextureQualityMediumMenuItem);
	m_builder -> get_widget ("TextureQualityLowMenuItem", m_TextureQualityLowMenuItem);
	m_builder -> get_widget ("ShowHideEnvironmentalEffectsMenuItem", m_ShowHideEnvironmentalEffectsMenuItem);
	m_builder -> get_widget ("BackgroundsMenuItem", m_BackgroundsMenuItem);
	m_builder -> get_widget ("FogsMenuItem", m_FogsMenuItem);
	m_builder -> get_widget ("ObjectIconsMenuItem", m_ObjectIconsMenuItem);
	m_builder -> get_widget ("LightsMenuItem", m_LightsMenuItem);
	m_builder -> get_widget ("ProximitySensorsMenuItem", m_ProximitySensorsMenuItem);
	m_builder -> get_widget ("SoundsMenuItem", m_SoundsMenuItem);
	m_builder -> get_widget ("TransformSensorsMenuItem", m_TransformSensorsMenuItem);
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
	m_builder -> get_widget ("TransformToolModeMenuItem", m_TransformToolModeMenuItem);
	m_builder -> get_widget ("GeometryMenuItem", m_GeometryMenuItem);
	m_builder -> get_widget ("UnionMenuItem", m_UnionMenuItem);
	m_builder -> get_widget ("DifferenceMenuItem", m_DifferenceMenuItem);
	m_builder -> get_widget ("IntersectionMenuItem", m_IntersectionMenuItem);
	m_builder -> get_widget ("ExclusionMenuItem", m_ExclusionMenuItem);
	m_builder -> get_widget ("FusionMenuItem", m_FusionMenuItem);
	m_builder -> get_widget ("CombineMenuItem", m_CombineMenuItem);
	m_builder -> get_widget ("TransformToZeroMenuItem", m_TransformToZeroMenuItem);
	m_builder -> get_widget ("ColorPerVertexEditorMenuItem", m_ColorPerVertexEditorMenuItem);
	m_builder -> get_widget ("TextureCoordinateEditorMenuItem", m_TextureCoordinateEditorMenuItem);
	m_builder -> get_widget ("LayoutMenuItem", m_LayoutMenuItem);
	m_builder -> get_widget ("GridLayoutToolMenuItem", m_GridLayoutToolMenuItem);
	m_builder -> get_widget ("AngleLayoutToolMenuItem", m_AngleLayoutToolMenuItem);
	m_builder -> get_widget ("GridPropertiesMenuItem", m_GridPropertiesMenuItem);
	m_builder -> get_widget ("ScenesMenuItem", m_ScenesMenuItem);
	m_builder -> get_widget ("ScenesMenu", m_ScenesMenu);
	m_builder -> get_widget ("HelpMenuItem", m_HelpMenuItem);
	m_builder -> get_widget ("InfoMenuItem", m_InfoMenuItem);
	m_builder -> get_widget ("Toolbar", m_Toolbar);
	m_builder -> get_widget ("LocationBar", m_LocationBar);
	m_builder -> get_widget ("LocationBar1", m_LocationBar1);
	m_builder -> get_widget ("AddTabButton", m_AddTabButton);
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
	m_builder -> get_widget ("NodeIndexButton", m_NodeIndexButton);
	m_builder -> get_widget ("NodePropertiesEditorButton", m_NodePropertiesEditorButton);
	m_builder -> get_widget ("AppearanceEditorButton", m_AppearanceEditorButton);
	m_builder -> get_widget ("TextureEditorButton", m_TextureEditorButton);
	m_builder -> get_widget ("TextEditorButton", m_TextEditorButton);
	m_builder -> get_widget ("GeometryPropertiesEditorButton", m_GeometryPropertiesEditorButton);
	m_builder -> get_widget ("GeometryEditorButton", m_GeometryEditorButton);
	m_builder -> get_widget ("ColorPerVertexEditorButton", m_ColorPerVertexEditorButton);
	m_builder -> get_widget ("TextureCoordinateEditorButton", m_TextureCoordinateEditorButton);
	m_builder -> get_widget ("LayerEditorButton", m_LayerEditorButton);
	m_builder -> get_widget ("BackgroundEditorButton", m_BackgroundEditorButton);
	m_builder -> get_widget ("NavigationInfoEditorButton", m_NavigationInfoEditorButton);
	m_builder -> get_widget ("ViewpointEditorButton", m_ViewpointEditorButton);
	m_builder -> get_widget ("LightEditorButton", m_LightEditorButton);
	m_builder -> get_widget ("LODEditorButton", m_LODEditorButton);
	m_builder -> get_widget ("InlineEditorButton", m_InlineEditorButton);
	m_builder -> get_widget ("PrecisionPlacementPanelButton", m_PrecisionPlacementPanelButton);
	m_builder -> get_widget ("CreatePrototypeInstanceButton", m_CreatePrototypeInstanceButton);
	m_builder -> get_widget ("NodeEditorButton", m_NodeEditorButton);
	m_builder -> get_widget ("VPaned", m_VPaned);
	m_builder -> get_widget ("HPaned", m_HPaned);
	m_builder -> get_widget ("BrowserOverlay", m_BrowserOverlay);
	m_builder -> get_widget ("BrowserNotebook", m_BrowserNotebook);
	m_builder -> get_widget ("Dashboard", m_Dashboard);
	m_builder -> get_widget ("DashboardToolBar", m_DashboardToolBar);
	m_builder -> get_widget ("LogoBox", m_LogoBox);
	m_builder -> get_widget ("HandButton", m_HandButton);
	m_builder -> get_widget ("ArrowButton", m_ArrowButton);
	m_builder -> get_widget ("PlayPauseButton", m_PlayPauseButton);
	m_builder -> get_widget ("SelectSeparator", m_SelectSeparator);
	m_builder -> get_widget ("SelectParentButton", m_SelectParentButton);
	m_builder -> get_widget ("SelectChildrenButton", m_SelectChildrenButton);
	m_builder -> get_widget ("ViewerSeparator", m_ViewerSeparator);
	m_builder -> get_widget ("ViewerButton", m_ViewerButton);
	m_builder -> get_widget ("StraightenButton", m_StraightenButton);
	m_builder -> get_widget ("StraightenHorizonButton", m_StraightenHorizonButton);
	m_builder -> get_widget ("LookAtSeparator", m_LookAtSeparator);
	m_builder -> get_widget ("LookAtSelectionButton", m_LookAtSelectionButton);
	m_builder -> get_widget ("LookAtAllButton", m_LookAtAllButton);
	m_builder -> get_widget ("LookAtButton", m_LookAtButton);
	m_builder -> get_widget ("FooterBox", m_FooterBox);
	m_builder -> get_widget ("SidebarBox", m_SidebarBox);
	m_builder -> get_widget ("MessageDialog", m_MessageDialog);

	// Connect object Gtk::ToggleAction with id 'AngleLayoutToolAction'.
	m_AngleLayoutToolAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_angle_layout_tool_toggled));
	m_BackgroundsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_backgrounds_toggled));
	m_BrowserAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_browser_toggled));
	m_EditorAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_editor_toggled));
	m_FlatAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_flat_toggled));
	m_FogsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fogs_toggled));
	m_FollowPrimarySelectionAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_follow_primary_selection_toggled));
	m_FooterAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_footer_toggled));
	m_GouraudAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_gouraud_toggled));
	m_GridLayoutToolAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_grid_layout_tool_toggled));
	m_LightsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_lights_toggled));
	m_MenubarAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_menubar_toggled));
	m_PhongAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_phong_toggled));
	m_PointsetAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_pointset_toggled));
	m_PrimitiveQualityHighAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_primitive_quality_high_toggled));
	m_PrimitiveQualityLowAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_primitive_quality_low_toggled));
	m_PrimitiveQualityMediumAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_primitive_quality_medium_toggled));
	m_ProximitySensorsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_proximity_sensors_toggled));
	m_RenderingPropertiesAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_rendering_properties_toggled));
	m_RubberbandAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_rubberband_toggled));
	m_SelectLowestAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_select_lowest_toggled));
	m_SidebarAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_sidebar_toggled));
	m_SoundsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_sounds_toggled));
	m_TabsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_tabs_toggled));
	m_TextureQualityHighAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_quality_high_toggled));
	m_TextureQualityLowAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_quality_low_toggled));
	m_TextureQualityMediumAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_quality_medium_toggled));
	m_ToolbarAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolbar_toggled));
	m_TransformSensorsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_transform_sensors_toggled));
	m_TransformToolModeAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_transform_tool_mode_toggled));
	m_ViewpointsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_viewpoints_toggled));
	m_VisibilitySensorsAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_visibility_sensors_toggled));

	// Connect object Gtk::ImageMenuItem with id 'ExamineViewerMenuItem'.
	m_ExamineViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_examine_viewer_activated));
	m_WalkViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_walk_viewer_activated));
	m_FlyViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fly_viewer_activated));
	m_PlaneViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_plane_viewer_activated));
	m_NoneViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_none_viewer_activated));

	// Connect object Gtk::ToggleAction with id 'WireframeAction'.
	m_WireframeAction -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_wireframe_toggled));

	// Connect object Gtk::ImageMenuItem with id 'BrowserNewMenuItem'.
	m_BrowserNewMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new_activated));
	m_BrowserOpenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_activated));
	m_BrowserOpenRecentMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_recent_activated));
	m_BrowserOpenLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_location_activated));
	m_BrowserImportMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import_activated));
	m_BrowserSaveMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_activated));
	m_BrowserSaveAsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_as_activated));

	// Connect object Gtk::MenuItem with id 'BrowserSaveACopyMenuItem'.
	m_BrowserSaveACopyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_a_copy_activated));

	// Connect object Gtk::ImageMenuItem with id 'BrowserExportMenuItem'.
	m_BrowserExportMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_export_activated));
	m_BrowserRevertMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved_activated));
	m_BrowserRemoveUnusedPrototypesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_remove_unused_prototypes_activated));
	m_BrowserScenePropertiesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_scene_properties_activated));
	m_BrowserCloseMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_close_activated));
	m_BrowserQuitMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_quit_activated));
	m_BrowserUndoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_undo_activated));
	m_BrowserRedoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_redo_activated));
	m_BrowserCutMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_cut_activated));
	m_BrowserCopyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_copy_activated));
	m_BrowserPasteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_paste_activated));
	m_BrowserDeleteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_delete_activated));

	// Connect object Gtk::MenuItem with id 'BrowserCreateCloneMenuItem'.
	m_BrowserCreateCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_clone_activated));
	m_BrowserUnlinkCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_unlink_clone_activated));

	// Connect object Gtk::ImageMenuItem with id 'BrowserGroupSelectedNodesMenuItem'.
	m_BrowserGroupSelectedNodesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_group_selected_nodes_activated));
	m_BrowserUngroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_ungroup_activated));

	// Connect object Gtk::MenuItem with id 'BrowserAddToGroupMenuItem'.
	m_BrowserAddToGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_add_to_group_activated));
	m_BrowserDetachFromGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_detach_from_group_activated));
	m_BrowserCreateParentTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_transform_activated));
	m_BrowserCreateParentGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_group_activated));
	m_BrowserCreateParentStaticGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_static_group_activated));
	m_BrowserCreateParentSwitchMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_switch_activated));
	m_BrowserCreateParentBillboardMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_billboard_activated));
	m_BrowserCreateParentCollisionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_collision_activated));
	m_BrowserCreateParentLODMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_lod_activated));
	m_BrowserCreateParentViewpointGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_viewpoint_group_activated));
	m_BrowserCreateParentAnchorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_anchor_activated));
	m_BrowserCreateParentLayoutLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layout_layer_activated));
	m_BrowserCreateParentScreenGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_screen_group_activated));
	m_BrowserCreateParentLayoutGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layout_group_activated));
	m_BrowserCreateParentGeoTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_geo_transform_activated));
	m_BrowserCreateParentGeoLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_geo_location_activated));
	m_BrowserCreateParentCADPartMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_part_activated));
	m_BrowserCreateParentCADAssemblyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_assembly_activated));
	m_BrowserCreateParentCADLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_layer_activated));
	m_BrowserCreateParentLayerSetMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layer_set_activated));
	m_BrowserCreateParentLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layer_activated));
	m_BrowserCreateParentViewportMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_viewport_activated));

	// Connect object Gtk::ImageMenuItem with id 'BrowserMotionBlurMenuItem'.
	m_BrowserMotionBlurMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_motion_blur_activated));

	// Connect object Gtk::MenuItem with id 'BrowserHideAllObjectIconsMenuItem'.
	m_BrowserHideAllObjectIconsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_all_object_icons_activated));

	// Connect object Gtk::ImageMenuItem with id 'BrowserFullScreenMenuItem'.
	m_BrowserFullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fullscreen_activated));
	m_BrowserUnFullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_unfullscreen_activated));
	m_BrowserSelectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_select_all_activated));

	// Connect object Gtk::MenuItem with id 'BrowserDeselectAllMenuItem'.
	m_BrowserDeselectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_deselect_all_activated));
	m_BrowserHideSelectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_selected_objects_activated));
	m_BrowserHideUnselectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_unselected_objects_activated));
	m_BrowserShowSelectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_show_selected_objects_activated));
	m_BrowserShowAllObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_show_all_objects_activated));

	// Connect object Gtk::ImageMenuItem with id 'BrowserUnionMenuItem'.
	m_BrowserUnionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_union_activated));
	m_BrowserDifferenceMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_difference_activated));
	m_BrowserIntersectionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_intersection_activated));
	m_BrowserExclusionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_exclusion_activated));
	m_BrowserFusionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fusion_activated));
	m_BrowserCombineMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_combine_activated));
	m_BrowserTransformToZeroMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_transform_to_zero_activated));
	m_BrowserColorPerVertexMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_color_editor_clicked));
	m_BrowserTextureCoordinateEditorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_mapping_editor_clicked));
	m_BrowserGridPropertiesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_grid_properties_activated));

	// Connect object Gtk::MenuItem with id 'BrowserScenesMenuItem'.
	m_BrowserScenesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_browser_scenes_activated));

	// Connect object Gtk::ImageMenuItem with id 'BrowserInfoMenuItem'.
	m_BrowserInfoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_info_activated));

	// Connect object Gtk::ApplicationWindow with id 'Window'.
	m_Window -> signal_focus_out_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_focus_out_event));
	m_Window -> signal_key_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_key_press_event), false);
	m_Window -> signal_key_release_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_key_release_event), false);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_style_updated () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_style_updated));

	// Connect object Gtk::MenuBar with id 'Menubar'.
	m_Menubar -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_menubar_button_press_event), false);

	// Connect object Gtk::ImageMenuItem with id 'NewMenuItem'.
	m_NewMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new_activated));
	m_OpenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_activated));
	m_OpenRecentMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_recent_activated));
	m_OpenLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_location_activated));
	m_ImportMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import_activated));
	m_SaveMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_activated));
	m_SaveAsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_as_activated));

	// Connect object Gtk::MenuItem with id 'SaveACopyMenuItem'.
	m_SaveACopyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_a_copy_activated));

	// Connect object Gtk::ImageMenuItem with id 'ExportMenuItem'.
	m_ExportMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_export_activated));
	m_RevertMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved_activated));
	m_RemoveUnusedPrototypesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_remove_unused_prototypes_activated));
	m_ScenePropertiesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_scene_properties_activated));
	m_CloseMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_close_activated));
	m_QuitMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_quit_activated));
	m_UndoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_undo_activated));
	m_RedoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_redo_activated));
	m_CutMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_cut_activated));
	m_CopyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_copy_activated));
	m_PasteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_paste_activated));
	m_DeleteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_delete_activated));

	// Connect object Gtk::MenuItem with id 'CreateCloneMenuItem'.
	m_CreateCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_clone_activated));
	m_UnlinkCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_unlink_clone_activated));

	// Connect object Gtk::ImageMenuItem with id 'GroupSelectedNodesMenuItem'.
	m_GroupSelectedNodesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_group_selected_nodes_activated));
	m_UngroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_ungroup_activated));

	// Connect object Gtk::MenuItem with id 'AddToGroupMenuItem'.
	m_AddToGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_add_to_group_activated));
	m_DetachFromGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_detach_from_group_activated));
	m_CreateParentTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_transform_activated));
	m_CreateParentGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_group_activated));
	m_CreateParentStaticGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_static_group_activated));
	m_CreateParentSwitchMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_switch_activated));
	m_CreateParentBillboardMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_billboard_activated));
	m_CreateParentCollisionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_collision_activated));
	m_CreateParentLODMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_lod_activated));
	m_CreateParentViewpointGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_viewpoint_group_activated));
	m_CreateParentAnchorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_anchor_activated));
	m_CreateParentLayoutLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layout_layer_activated));
	m_CreateParentScreenGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_screen_group_activated));
	m_CreateParentLayoutGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layout_group_activated));
	m_CreateParentGeoTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_geo_transform_activated));
	m_CreateParentGeoLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_geo_location_activated));
	m_CreateParentCADPartMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_part_activated));
	m_CreateParentCADAssemblyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_assembly_activated));
	m_CreateParentCADLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_cad_layer_activated));
	m_CreateParentLayerSetMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layer_set_activated));
	m_CreateParentLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_layer_activated));
	m_CreateParentViewportMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_viewport_activated));

	// Connect object Gtk::ImageMenuItem with id 'MotionBlurMenuItem'.
	m_MotionBlurMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_motion_blur_activated));

	// Connect object Gtk::MenuItem with id 'HideAllObjectIconsMenuItem'.
	m_HideAllObjectIconsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_all_object_icons_activated));

	// Connect object Gtk::ImageMenuItem with id 'FullScreenMenuItem'.
	m_FullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fullscreen_activated));
	m_UnFullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_unfullscreen_activated));
	m_SelectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_select_all_activated));

	// Connect object Gtk::MenuItem with id 'DeselectAllMenuItem'.
	m_DeselectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_deselect_all_activated));
	m_HideSelectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_selected_objects_activated));
	m_HideUnselectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hide_unselected_objects_activated));
	m_ShowSelectedObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_show_selected_objects_activated));
	m_ShowAllObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_show_all_objects_activated));

	// Connect object Gtk::ImageMenuItem with id 'UnionMenuItem'.
	m_UnionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_union_activated));
	m_DifferenceMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_difference_activated));
	m_IntersectionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_intersection_activated));
	m_ExclusionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_exclusion_activated));
	m_FusionMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fusion_activated));
	m_CombineMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_combine_activated));
	m_TransformToZeroMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_transform_to_zero_activated));
	m_ColorPerVertexEditorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_color_editor_clicked));
	m_TextureCoordinateEditorMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_mapping_editor_clicked));
	m_GridPropertiesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_grid_properties_activated));

	// Connect object Gtk::MenuItem with id 'ScenesMenuItem'.
	m_ScenesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_scenes_activated));

	// Connect object Gtk::ImageMenuItem with id 'InfoMenuItem'.
	m_InfoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_info_activated));

	// Connect object Gtk::Box with id 'Toolbar'.
	m_Toolbar -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolbar_drag_data_received));

	// Connect object Gtk::Toolbar with id 'LocationBar1'.
	m_LocationBar1 -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolbar_drag_data_received));

	// Connect object Gtk::ToolButton with id 'AddTabButton'.
	m_AddTabButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new_activated));
	m_HomeButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_home));
	m_PreviousButton -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_previous_button_press_event), false);
	m_PreviousButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_previous_page));
	m_NextButton -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_next_button_press_event), false);
	m_NextButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_next_page));

	// Connect object Gtk::Entry with id 'LocationEntry'.
	m_LocationEntry -> signal_icon_release () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_location_icon_released));
	m_LocationEntry -> signal_key_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_location_key_press_event), false);

	// Connect object Gtk::ToolButton with id 'ReloadButton'.
	m_ReloadButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved_activated));
	m_NewButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new_activated));

	// Connect object Gtk::MenuToolButton with id 'OpenButton'.
	m_OpenButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_activated));

	// Connect object Gtk::ToolButton with id 'ImportButton'.
	m_ImportButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import_activated));
	m_SaveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_activated));
	m_RefreshButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved_activated));
	m_UndoButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_undo_activated));
	m_RedoButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_redo_activated));
	m_NodeIndexButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_node_index_clicked));
	m_NodePropertiesEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_node_properties_editor_clicked));
	m_AppearanceEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_appearance_editor_clicked));
	m_TextureEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_editor_clicked));
	m_TextEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_text_editor_clicked));
	m_GeometryPropertiesEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_geometry_properties_editor_clicked));
	m_GeometryEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_geometry_editor_clicked));
	m_ColorPerVertexEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_color_editor_clicked));
	m_TextureCoordinateEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_texture_mapping_editor_clicked));
	m_LayerEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_layer_editor_clicked));
	m_BackgroundEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_background_editor_clicked));
	m_NavigationInfoEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_navigation_info_editor_clicked));
	m_ViewpointEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_viewpoint_editor_clicked));
	m_LightEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_light_editor_clicked));
	m_LODEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_lod_editor_clicked));
	m_InlineEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_inline_editor_clicked));
	m_PrecisionPlacementPanelButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_precision_placement_panel_clicked));
	m_CreatePrototypeInstanceButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_prototype_instance_dialog_clicked));
	m_NodeEditorButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_node_editor_clicked));

	// Connect object Gtk::Notebook with id 'BrowserNotebook'.
	m_BrowserNotebook -> signal_button_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_notebook_button_press_event), false);
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

	// Connect object Gtk::ToggleToolButton with id 'StraightenHorizonButton'.
	m_StraightenHorizonButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_straighten_horizon_toggled));

	// Connect object Gtk::ToolButton with id 'LookAtSelectionButton'.
	m_LookAtSelectionButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_selection_clicked));
	m_LookAtAllButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_all_clicked));

	// Connect object Gtk::RadioToolButton with id 'LookAtButton'.
	m_LookAtButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_toggled));

	// Call construct handler of base class.
	construct ();
}

X3DBrowserWindowInterface::~X3DBrowserWindowInterface ()
{
	delete m_Window;
	delete m_MessageDialog;
}

} // puck
} // titania

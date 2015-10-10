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
#ifndef __TMP_GLAD2CPP_BROWSER_WINDOW_H__
#define __TMP_GLAD2CPP_BROWSER_WINDOW_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DBrowserWindowInterface :
	public X3DUserInterface
{
public:

	X3DBrowserWindowInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DBrowserWindowInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		        filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::IconFactory> &
	getIconFactory () const
	{ return m_IconFactory; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getAngleLayoutToolAction () const
	{ return m_AngleLayoutToolAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getBackgroundsAction () const
	{ return m_BackgroundsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getBrowserAction () const
	{ return m_BrowserAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getEditorAction () const
	{ return m_EditorAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getFlatAction () const
	{ return m_FlatAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getFogsAction () const
	{ return m_FogsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getFollowPrimarySelectionAction () const
	{ return m_FollowPrimarySelectionAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getFooterAction () const
	{ return m_FooterAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getGouraudAction () const
	{ return m_GouraudAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getGridLayoutToolAction () const
	{ return m_GridLayoutToolAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getLightsAction () const
	{ return m_LightsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getMenubarAction () const
	{ return m_MenubarAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getPhongAction () const
	{ return m_PhongAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getPointsetAction () const
	{ return m_PointsetAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getPrimitiveQualityHighAction () const
	{ return m_PrimitiveQualityHighAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getPrimitiveQualityLowAction () const
	{ return m_PrimitiveQualityLowAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getPrimitiveQualityMediumAction () const
	{ return m_PrimitiveQualityMediumAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getProximitySensorsAction () const
	{ return m_ProximitySensorsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getRenderingPropertiesAction () const
	{ return m_RenderingPropertiesAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getRubberbandAction () const
	{ return m_RubberbandAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getSelectLowestAction () const
	{ return m_SelectLowestAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getSidebarAction () const
	{ return m_SidebarAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getTabsAction () const
	{ return m_TabsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getTextureQualityHighAction () const
	{ return m_TextureQualityHighAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getTextureQualityLowAction () const
	{ return m_TextureQualityLowAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getTextureQualityMediumAction () const
	{ return m_TextureQualityMediumAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getToolbarAction () const
	{ return m_ToolbarAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getTransformSensorsAction () const
	{ return m_TransformSensorsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getViewpointsAction () const
	{ return m_ViewpointsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getVisibilitySensorsAction () const
	{ return m_VisibilitySensorsAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getWireframeAction () const
	{ return m_WireframeAction; }

	Gtk::Image &
	getExamineViewerImage () const
	{ return *m_ExamineViewerImage; }

	Gtk::Image &
	getFileImportImage () const
	{ return *m_FileImportImage; }

	Gtk::Image &
	getFileImportImage1 () const
	{ return *m_FileImportImage1; }

	Gtk::Image &
	getFlyViewerImage () const
	{ return *m_FlyViewerImage; }

	Gtk::Menu &
	getHistoryMenu () const
	{ return *m_HistoryMenu; }

	Gtk::Image &
	getNoneViewerImage () const
	{ return *m_NoneViewerImage; }

	Gtk::Image &
	getOpenLocationImage () const
	{ return *m_OpenLocationImage; }

	Gtk::Image &
	getOpenLocationImage1 () const
	{ return *m_OpenLocationImage1; }

	Gtk::Image &
	getPlaneViewerImage () const
	{ return *m_PlaneViewerImage; }

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

	Gtk::Image &
	getWorkspacesImage1 () const
	{ return *m_WorkspacesImage1; }

	Gtk::Menu &
	getBrowserMenu () const
	{ return *m_BrowserMenu; }

	Gtk::MenuItem &
	getBrowserFileMenuItem () const
	{ return *m_BrowserFileMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserNewMenuItem () const
	{ return *m_BrowserNewMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserOpenMenuItem () const
	{ return *m_BrowserOpenMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserOpenRecentMenuItem () const
	{ return *m_BrowserOpenRecentMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserOpenLocationMenuItem () const
	{ return *m_BrowserOpenLocationMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserImportMenuItem () const
	{ return *m_BrowserImportMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserSaveMenuItem () const
	{ return *m_BrowserSaveMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserSaveAsMenuItem () const
	{ return *m_BrowserSaveAsMenuItem; }

	Gtk::MenuItem &
	getBrowserSaveACopyMenuItem () const
	{ return *m_BrowserSaveACopyMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserExportMenuItem () const
	{ return *m_BrowserExportMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserRevertMenuItem () const
	{ return *m_BrowserRevertMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserRemoveUnusedPrototypesMenuItem () const
	{ return *m_BrowserRemoveUnusedPrototypesMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserCloseMenuItem () const
	{ return *m_BrowserCloseMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserQuitMenuItem () const
	{ return *m_BrowserQuitMenuItem; }

	Gtk::MenuItem &
	getBrowserEditMenuItem () const
	{ return *m_BrowserEditMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserUndoMenuItem () const
	{ return *m_BrowserUndoMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserRedoMenuItem () const
	{ return *m_BrowserRedoMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserCutMenuItem () const
	{ return *m_BrowserCutMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserCopyMenuItem () const
	{ return *m_BrowserCopyMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserPasteMenuItem () const
	{ return *m_BrowserPasteMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserDeleteMenuItem () const
	{ return *m_BrowserDeleteMenuItem; }

	Gtk::MenuItem &
	getBrowserCloneMenuItem () const
	{ return *m_BrowserCloneMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateCloneMenuItem () const
	{ return *m_BrowserCreateCloneMenuItem; }

	Gtk::MenuItem &
	getBrowserUnlinkCloneMenuItem () const
	{ return *m_BrowserUnlinkCloneMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserGroupSelectedNodesMenuItem () const
	{ return *m_BrowserGroupSelectedNodesMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserUngroupMenuItem () const
	{ return *m_BrowserUngroupMenuItem; }

	Gtk::MenuItem &
	getBrowserAddToGroupMenuItem () const
	{ return *m_BrowserAddToGroupMenuItem; }

	Gtk::MenuItem &
	getBrowserDetachFromGroupMenuItem () const
	{ return *m_BrowserDetachFromGroupMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentMenuItem () const
	{ return *m_BrowserCreateParentMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentTransformMenuItem () const
	{ return *m_BrowserCreateParentTransformMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentGroupMenuItem () const
	{ return *m_BrowserCreateParentGroupMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentStaticGroupMenuItem () const
	{ return *m_BrowserCreateParentStaticGroupMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentSwitchMenuItem () const
	{ return *m_BrowserCreateParentSwitchMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentBillboardMenuItem () const
	{ return *m_BrowserCreateParentBillboardMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentCollisionMenuItem () const
	{ return *m_BrowserCreateParentCollisionMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentLODMenuItem () const
	{ return *m_BrowserCreateParentLODMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentAnchorMenuItem () const
	{ return *m_BrowserCreateParentAnchorMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentLayoutLayerMenuItem () const
	{ return *m_BrowserCreateParentLayoutLayerMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentScreenGroupMenuItem () const
	{ return *m_BrowserCreateParentScreenGroupMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentLayoutGroupMenuItem () const
	{ return *m_BrowserCreateParentLayoutGroupMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentGeoTransformMenuItem () const
	{ return *m_BrowserCreateParentGeoTransformMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentGeoLocationMenuItem () const
	{ return *m_BrowserCreateParentGeoLocationMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentCADPartMenuItem () const
	{ return *m_BrowserCreateParentCADPartMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentCADAssemblyMenuItem () const
	{ return *m_BrowserCreateParentCADAssemblyMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentCADLayerMenuItem () const
	{ return *m_BrowserCreateParentCADLayerMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentLayerSetMenuItem () const
	{ return *m_BrowserCreateParentLayerSetMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentLayerMenuItem () const
	{ return *m_BrowserCreateParentLayerMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentViewportMenuItem () const
	{ return *m_BrowserCreateParentViewportMenuItem; }

	Gtk::MenuItem &
	getBrowserViewMenuItem () const
	{ return *m_BrowserViewMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserMenubarMenuItem () const
	{ return *m_BrowserMenubarMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserToolbarMenuItem () const
	{ return *m_BrowserToolbarMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserSidebarMenuItem () const
	{ return *m_BrowserSidebarMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserFooterMenuItem () const
	{ return *m_BrowserFooterMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserTabsMenuItem () const
	{ return *m_BrowserTabsMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserEnvironmentMenuItem () const
	{ return *m_BrowserEnvironmentMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserEditorMenuItem () const
	{ return *m_BrowserEditorMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserBrowserMenuItem () const
	{ return *m_BrowserBrowserMenuItem; }

	Gtk::SeparatorMenuItem &
	getBrowserBrowserOptionsSeparator () const
	{ return *m_BrowserBrowserOptionsSeparator; }

	Gtk::ImageMenuItem &
	getBrowserMotionBlurMenuItem () const
	{ return *m_BrowserMotionBlurMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserShadingMenuItem () const
	{ return *m_BrowserShadingMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserPhongMenuItem () const
	{ return *m_BrowserPhongMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserGouraudMenuItem () const
	{ return *m_BrowserGouraudMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserFlatMenuItem () const
	{ return *m_BrowserFlatMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserWireframeMenuItem () const
	{ return *m_BrowserWireframeMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserPointsetMenuItem () const
	{ return *m_BrowserPointsetMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserPrimitiveQualityMenuItem () const
	{ return *m_BrowserPrimitiveQualityMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserPrimitiveQualityHighMenuItem () const
	{ return *m_BrowserPrimitiveQualityHighMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserPrimitiveQualityMediumMenuItem () const
	{ return *m_BrowserPrimitiveQualityMediumMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserPrimitiveQualityLowMenuItem () const
	{ return *m_BrowserPrimitiveQualityLowMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserTextureQualityMenuItem () const
	{ return *m_BrowserTextureQualityMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserTextureQualityHighMenuItem () const
	{ return *m_BrowserTextureQualityHighMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserTextureQualityMediumMenuItem () const
	{ return *m_BrowserTextureQualityMediumMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserTextureQualityLowMenuItem () const
	{ return *m_BrowserTextureQualityLowMenuItem; }

	Gtk::MenuItem &
	getBrowserShowHideEnvironmentalEffectsMenuItem () const
	{ return *m_BrowserShowHideEnvironmentalEffectsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserBackgroundsMenuItem () const
	{ return *m_BrowserBackgroundsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserFogsMenuItem () const
	{ return *m_BrowserFogsMenuItem; }

	Gtk::MenuItem &
	getBrowserObjectIconsMenuItem () const
	{ return *m_BrowserObjectIconsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserLightsMenuItem () const
	{ return *m_BrowserLightsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserProximitySensorsMenuItem () const
	{ return *m_BrowserProximitySensorsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserTransformSensorsMenuItem () const
	{ return *m_BrowserTransformSensorsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserVisibilitySensorsMenuItem () const
	{ return *m_BrowserVisibilitySensorsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserViewpointsMenuItem () const
	{ return *m_BrowserViewpointsMenuItem; }

	Gtk::MenuItem &
	getBrowserHideAllObjectIconsMenuItem () const
	{ return *m_BrowserHideAllObjectIconsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserRubberbandMenuItem () const
	{ return *m_BrowserRubberbandMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserRenderingPropertiesMenuItem () const
	{ return *m_BrowserRenderingPropertiesMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserFullScreenMenuItem () const
	{ return *m_BrowserFullScreenMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserUnFullScreenMenuItem () const
	{ return *m_BrowserUnFullScreenMenuItem; }

	Gtk::MenuItem &
	getBrowserSelectionMenuItem () const
	{ return *m_BrowserSelectionMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserSelectAllMenuItem () const
	{ return *m_BrowserSelectAllMenuItem; }

	Gtk::MenuItem &
	getBrowserDeselectAllMenuItem () const
	{ return *m_BrowserDeselectAllMenuItem; }

	Gtk::MenuItem &
	getBrowserHideSelectedObjectsMenuItem () const
	{ return *m_BrowserHideSelectedObjectsMenuItem; }

	Gtk::MenuItem &
	getBrowserHideUnselectedObjectsMenuItem () const
	{ return *m_BrowserHideUnselectedObjectsMenuItem; }

	Gtk::MenuItem &
	getBrowserShowSelectedObjectsMenuItem () const
	{ return *m_BrowserShowSelectedObjectsMenuItem; }

	Gtk::MenuItem &
	getBrowserShowAllObjectsMenuItem () const
	{ return *m_BrowserShowAllObjectsMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserSelectLowestMenuItem () const
	{ return *m_BrowserSelectLowestMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserFollowPrimarySelectionMenuItem () const
	{ return *m_BrowserFollowPrimarySelectionMenuItem; }

	Gtk::MenuItem &
	getBrowserObjectMenuItem () const
	{ return *m_BrowserObjectMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserCombineMenuItem () const
	{ return *m_BrowserCombineMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserTransformToZeroMenuItem () const
	{ return *m_BrowserTransformToZeroMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserTextureCoordinateEditorMenuItem () const
	{ return *m_BrowserTextureCoordinateEditorMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserPaintPolygonsMenuItem () const
	{ return *m_BrowserPaintPolygonsMenuItem; }

	Gtk::MenuItem &
	getBrowserLayoutMenuItem () const
	{ return *m_BrowserLayoutMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserGridLayoutToolMenuItem () const
	{ return *m_BrowserGridLayoutToolMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserAngleLayoutToolMenuItem () const
	{ return *m_BrowserAngleLayoutToolMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserGridPropertiesMenuItem () const
	{ return *m_BrowserGridPropertiesMenuItem; }

	Gtk::MenuItem &
	getBrowserScenesMenuItem () const
	{ return *m_BrowserScenesMenuItem; }

	Gtk::Menu &
	getBrowserScenesMenu () const
	{ return *m_BrowserScenesMenu; }

	Gtk::MenuItem &
	getBrowserHelpMenuItem () const
	{ return *m_BrowserHelpMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserInfoMenuItem () const
	{ return *m_BrowserInfoMenuItem; }

	Gtk::ApplicationWindow &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getHeaderBox () const
	{ return *m_HeaderBox; }

	Gtk::Box &
	getHeaderSubBox () const
	{ return *m_HeaderSubBox; }

	Gtk::MenuBar &
	getMenubar () const
	{ return *m_Menubar; }

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
	getOpenRecentMenuItem () const
	{ return *m_OpenRecentMenuItem; }

	Gtk::ImageMenuItem &
	getOpenLocationMenuItem () const
	{ return *m_OpenLocationMenuItem; }

	Gtk::ImageMenuItem &
	getImportMenuItem () const
	{ return *m_ImportMenuItem; }

	Gtk::ImageMenuItem &
	getSaveMenuItem () const
	{ return *m_SaveMenuItem; }

	Gtk::ImageMenuItem &
	getSaveAsMenuItem () const
	{ return *m_SaveAsMenuItem; }

	Gtk::MenuItem &
	getSaveACopyMenuItem () const
	{ return *m_SaveACopyMenuItem; }

	Gtk::ImageMenuItem &
	getExportMenuItem () const
	{ return *m_ExportMenuItem; }

	Gtk::ImageMenuItem &
	getRevertMenuItem () const
	{ return *m_RevertMenuItem; }

	Gtk::ImageMenuItem &
	getRemoveUnusedPrototypesMenuItem () const
	{ return *m_RemoveUnusedPrototypesMenuItem; }

	Gtk::ImageMenuItem &
	getCloseMenuItem () const
	{ return *m_CloseMenuItem; }

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
	getCreateParentMenuItem () const
	{ return *m_CreateParentMenuItem; }

	Gtk::MenuItem &
	getCreateParentTransformMenuItem () const
	{ return *m_CreateParentTransformMenuItem; }

	Gtk::MenuItem &
	getCreateParentGroupMenuItem () const
	{ return *m_CreateParentGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentStaticGroupMenuItem () const
	{ return *m_CreateParentStaticGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentSwitchMenuItem () const
	{ return *m_CreateParentSwitchMenuItem; }

	Gtk::MenuItem &
	getCreateParentBillboardMenuItem () const
	{ return *m_CreateParentBillboardMenuItem; }

	Gtk::MenuItem &
	getCreateParentCollisionMenuItem () const
	{ return *m_CreateParentCollisionMenuItem; }

	Gtk::MenuItem &
	getCreateParentLODMenuItem () const
	{ return *m_CreateParentLODMenuItem; }

	Gtk::MenuItem &
	getCreateParentAnchorMenuItem () const
	{ return *m_CreateParentAnchorMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayoutLayerMenuItem () const
	{ return *m_CreateParentLayoutLayerMenuItem; }

	Gtk::MenuItem &
	getCreateParentScreenGroupMenuItem () const
	{ return *m_CreateParentScreenGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayoutGroupMenuItem () const
	{ return *m_CreateParentLayoutGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentGeoTransformMenuItem () const
	{ return *m_CreateParentGeoTransformMenuItem; }

	Gtk::MenuItem &
	getCreateParentGeoLocationMenuItem () const
	{ return *m_CreateParentGeoLocationMenuItem; }

	Gtk::MenuItem &
	getCreateParentCADPartMenuItem () const
	{ return *m_CreateParentCADPartMenuItem; }

	Gtk::MenuItem &
	getCreateParentCADAssemblyMenuItem () const
	{ return *m_CreateParentCADAssemblyMenuItem; }

	Gtk::MenuItem &
	getCreateParentCADLayerMenuItem () const
	{ return *m_CreateParentCADLayerMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayerSetMenuItem () const
	{ return *m_CreateParentLayerSetMenuItem; }

	Gtk::MenuItem &
	getCreateParentLayerMenuItem () const
	{ return *m_CreateParentLayerMenuItem; }

	Gtk::MenuItem &
	getCreateParentViewportMenuItem () const
	{ return *m_CreateParentViewportMenuItem; }

	Gtk::MenuItem &
	getViewMenuItem () const
	{ return *m_ViewMenuItem; }

	Gtk::CheckMenuItem &
	getMenubarMenuItem () const
	{ return *m_MenubarMenuItem; }

	Gtk::CheckMenuItem &
	getToolbarMenuItem () const
	{ return *m_ToolbarMenuItem; }

	Gtk::CheckMenuItem &
	getSidebarMenuItem () const
	{ return *m_SidebarMenuItem; }

	Gtk::CheckMenuItem &
	getFooterMenuItem () const
	{ return *m_FooterMenuItem; }

	Gtk::CheckMenuItem &
	getTabsMenuItem () const
	{ return *m_TabsMenuItem; }

	Gtk::ImageMenuItem &
	getEnvironmentMenuItem () const
	{ return *m_EnvironmentMenuItem; }

	Gtk::CheckMenuItem &
	getEditorMenuItem () const
	{ return *m_EditorMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserMenuItem () const
	{ return *m_BrowserMenuItem; }

	Gtk::SeparatorMenuItem &
	getBrowserOptionsSeparator () const
	{ return *m_BrowserOptionsSeparator; }

	Gtk::ImageMenuItem &
	getMotionBlurMenuItem () const
	{ return *m_MotionBlurMenuItem; }

	Gtk::ImageMenuItem &
	getShadingMenuItem () const
	{ return *m_ShadingMenuItem; }

	Gtk::CheckMenuItem &
	getPhongMenuItem () const
	{ return *m_PhongMenuItem; }

	Gtk::CheckMenuItem &
	getGouraudMenuItem () const
	{ return *m_GouraudMenuItem; }

	Gtk::CheckMenuItem &
	getFlatMenuItem () const
	{ return *m_FlatMenuItem; }

	Gtk::CheckMenuItem &
	getWireframeMenuItem () const
	{ return *m_WireframeMenuItem; }

	Gtk::CheckMenuItem &
	getPointsetMenuItem () const
	{ return *m_PointsetMenuItem; }

	Gtk::ImageMenuItem &
	getPrimitiveQualityMenuItem () const
	{ return *m_PrimitiveQualityMenuItem; }

	Gtk::CheckMenuItem &
	getPrimitiveQualityHighMenuItem () const
	{ return *m_PrimitiveQualityHighMenuItem; }

	Gtk::CheckMenuItem &
	getPrimitiveQualityMediumMenuItem () const
	{ return *m_PrimitiveQualityMediumMenuItem; }

	Gtk::CheckMenuItem &
	getPrimitiveQualityLowMenuItem () const
	{ return *m_PrimitiveQualityLowMenuItem; }

	Gtk::ImageMenuItem &
	getTextureQualityMenuItem () const
	{ return *m_TextureQualityMenuItem; }

	Gtk::CheckMenuItem &
	getTextureQualityHighMenuItem () const
	{ return *m_TextureQualityHighMenuItem; }

	Gtk::CheckMenuItem &
	getTextureQualityMediumMenuItem () const
	{ return *m_TextureQualityMediumMenuItem; }

	Gtk::CheckMenuItem &
	getTextureQualityLowMenuItem () const
	{ return *m_TextureQualityLowMenuItem; }

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
	getLightsMenuItem () const
	{ return *m_LightsMenuItem; }

	Gtk::CheckMenuItem &
	getProximitySensorsMenuItem () const
	{ return *m_ProximitySensorsMenuItem; }

	Gtk::CheckMenuItem &
	getTransformSensorsMenuItem () const
	{ return *m_TransformSensorsMenuItem; }

	Gtk::CheckMenuItem &
	getVisibilitySensorsMenuItem () const
	{ return *m_VisibilitySensorsMenuItem; }

	Gtk::CheckMenuItem &
	getViewpointsMenuItem () const
	{ return *m_ViewpointsMenuItem; }

	Gtk::MenuItem &
	getHideAllObjectIconsMenuItem () const
	{ return *m_HideAllObjectIconsMenuItem; }

	Gtk::CheckMenuItem &
	getRubberbandMenuItem () const
	{ return *m_RubberbandMenuItem; }

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
	getObjectMenuItem () const
	{ return *m_ObjectMenuItem; }

	Gtk::ImageMenuItem &
	getCombineMenuItem () const
	{ return *m_CombineMenuItem; }

	Gtk::ImageMenuItem &
	getTransformToZeroMenuItem () const
	{ return *m_TransformToZeroMenuItem; }

	Gtk::ImageMenuItem &
	getTextureCoordinateEditorMenuItem () const
	{ return *m_TextureCoordinateEditorMenuItem; }

	Gtk::ImageMenuItem &
	getPaintPolygonsMenuItem () const
	{ return *m_PaintPolygonsMenuItem; }

	Gtk::MenuItem &
	getLayoutMenuItem () const
	{ return *m_LayoutMenuItem; }

	Gtk::CheckMenuItem &
	getGridLayoutToolMenuItem () const
	{ return *m_GridLayoutToolMenuItem; }

	Gtk::CheckMenuItem &
	getAngleLayoutToolMenuItem () const
	{ return *m_AngleLayoutToolMenuItem; }

	Gtk::ImageMenuItem &
	getGridPropertiesMenuItem () const
	{ return *m_GridPropertiesMenuItem; }

	Gtk::MenuItem &
	getScenesMenuItem () const
	{ return *m_ScenesMenuItem; }

	Gtk::Menu &
	getScenesMenu () const
	{ return *m_ScenesMenu; }

	Gtk::MenuItem &
	getHelpMenuItem () const
	{ return *m_HelpMenuItem; }

	Gtk::ImageMenuItem &
	getInfoMenuItem () const
	{ return *m_InfoMenuItem; }

	Gtk::Box &
	getToolbar () const
	{ return *m_Toolbar; }

	Gtk::Box &
	getLocationBar () const
	{ return *m_LocationBar; }

	Gtk::Toolbar &
	getLocationBar1 () const
	{ return *m_LocationBar1; }

	Gtk::ToolButton &
	getAddTabButton () const
	{ return *m_AddTabButton; }

	Gtk::ToolButton &
	getHomeButton () const
	{ return *m_HomeButton; }

	Gtk::ToolButton &
	getPreviousButton () const
	{ return *m_PreviousButton; }

	Gtk::ToolButton &
	getNextButton () const
	{ return *m_NextButton; }

	Gtk::Entry &
	getLocationEntry () const
	{ return *m_LocationEntry; }

	Gtk::Toolbar &
	getLocationBar2 () const
	{ return *m_LocationBar2; }

	Gtk::ToolButton &
	getReloadButton () const
	{ return *m_ReloadButton; }

	Gtk::Toolbar &
	getEditToolBar () const
	{ return *m_EditToolBar; }

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
	getAppearanceEditorButton () const
	{ return *m_AppearanceEditorButton; }

	Gtk::ToolButton &
	getTextureEditorButton () const
	{ return *m_TextureEditorButton; }

	Gtk::ToolButton &
	getTextEditorButton () const
	{ return *m_TextEditorButton; }

	Gtk::ToolButton &
	getGeometryPropertiesEditorButton () const
	{ return *m_GeometryPropertiesEditorButton; }

	Gtk::ToolButton &
	getNavigationInfoEditorButton () const
	{ return *m_NavigationInfoEditorButton; }

	Gtk::ToolButton &
	getViewpointEditorButton () const
	{ return *m_ViewpointEditorButton; }

	Gtk::ToolButton &
	getLightEditorButton () const
	{ return *m_LightEditorButton; }

	Gtk::ToolButton &
	getLODEditorButton () const
	{ return *m_LODEditorButton; }

	Gtk::ToolButton &
	getInlineEditorButton () const
	{ return *m_InlineEditorButton; }

	Gtk::ToolButton &
	getLayerEditorButton () const
	{ return *m_LayerEditorButton; }

	Gtk::ToolButton &
	getPrecisionPlacementPanelButton () const
	{ return *m_PrecisionPlacementPanelButton; }

	Gtk::ToolButton &
	getCreatePrototypeInstanceButton () const
	{ return *m_CreatePrototypeInstanceButton; }

	Gtk::ToolButton &
	getNodeIndexButton () const
	{ return *m_NodeIndexButton; }

	Gtk::ToolButton &
	getHammerButton () const
	{ return *m_HammerButton; }

	Gtk::Paned &
	getVPaned () const
	{ return *m_VPaned; }

	Gtk::Paned &
	getHPaned () const
	{ return *m_HPaned; }

	Gtk::Box &
	getMasterBox () const
	{ return *m_MasterBox; }

	Gtk::Notebook &
	getBrowserNotebook () const
	{ return *m_BrowserNotebook; }

	Gtk::Box &
	getDashboard () const
	{ return *m_Dashboard; }

	Gtk::Toolbar &
	getDashboardToolBar () const
	{ return *m_DashboardToolBar; }

	Gtk::ToolButton &
	getTabButton () const
	{ return *m_TabButton; }

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
	getLookAtSelectionButton () const
	{ return *m_LookAtSelectionButton; }

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
	getScriptEditorBox () const
	{ return *m_ScriptEditorBox; }

	Gtk::Box &
	getAnimationEditorBox () const
	{ return *m_AnimationEditorBox; }

	Gtk::Box &
	getSidebar () const
	{ return *m_Sidebar; }

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
	getHistoryViewBox () const
	{ return *m_HistoryViewBox; }

	Gtk::Box &
	getLibraryViewBox () const
	{ return *m_LibraryViewBox; }

	Gtk::Box &
	getOutlineEditorBox () const
	{ return *m_OutlineEditorBox; }

	Gtk::MessageDialog &
	getMessageDialog () const
	{ return *m_MessageDialog; }

	virtual
	void
	on_angle_layout_tool_toggled () = 0;

	virtual
	void
	on_backgrounds_toggled () = 0;

	virtual
	void
	on_browser_toggled () = 0;

	virtual
	void
	on_editor_toggled () = 0;

	virtual
	void
	on_flat_toggled () = 0;

	virtual
	void
	on_fogs_toggled () = 0;

	virtual
	void
	on_follow_primary_selection_toggled () = 0;

	virtual
	void
	on_footer_toggled () = 0;

	virtual
	void
	on_gouraud_toggled () = 0;

	virtual
	void
	on_grid_layout_tool_toggled () = 0;

	virtual
	void
	on_lights_toggled () = 0;

	virtual
	void
	on_menubar_toggled () = 0;

	virtual
	void
	on_phong_toggled () = 0;

	virtual
	void
	on_pointset_toggled () = 0;

	virtual
	void
	on_primitive_quality_high_toggled () = 0;

	virtual
	void
	on_primitive_quality_low_toggled () = 0;

	virtual
	void
	on_primitive_quality_medium_toggled () = 0;

	virtual
	void
	on_proximity_sensors_toggled () = 0;

	virtual
	void
	on_rendering_properties_toggled () = 0;

	virtual
	void
	on_rubberband_toggled () = 0;

	virtual
	void
	on_select_lowest_toggled () = 0;

	virtual
	void
	on_sidebar_toggled () = 0;

	virtual
	void
	on_tabs_toggled () = 0;

	virtual
	void
	on_texture_quality_high_toggled () = 0;

	virtual
	void
	on_texture_quality_low_toggled () = 0;

	virtual
	void
	on_texture_quality_medium_toggled () = 0;

	virtual
	void
	on_toolbar_toggled () = 0;

	virtual
	void
	on_transform_sensors_toggled () = 0;

	virtual
	void
	on_viewpoints_toggled () = 0;

	virtual
	void
	on_visibility_sensors_toggled () = 0;

	virtual
	void
	on_examine_viewer_activated () = 0;

	virtual
	void
	on_walk_viewer_activated () = 0;

	virtual
	void
	on_fly_viewer_activated () = 0;

	virtual
	void
	on_plane_viewer_activated () = 0;

	virtual
	void
	on_none_viewer_activated () = 0;

	virtual
	void
	on_wireframe_toggled () = 0;

	virtual
	void
	on_new_activated () = 0;

	virtual
	void
	on_open_activated () = 0;

	virtual
	void
	on_open_recent_activated () = 0;

	virtual
	void
	on_open_location_activated () = 0;

	virtual
	void
	on_import_activated () = 0;

	virtual
	void
	on_save_activated () = 0;

	virtual
	void
	on_save_as_activated () = 0;

	virtual
	void
	on_save_a_copy_activated () = 0;

	virtual
	void
	on_export_activated () = 0;

	virtual
	void
	on_revert_to_saved_activated () = 0;

	virtual
	void
	on_remove_unused_prototypes_activated () = 0;

	virtual
	void
	on_close_activated () = 0;

	virtual
	void
	on_quit_activated () = 0;

	virtual
	void
	on_undo_activated () = 0;

	virtual
	void
	on_redo_activated () = 0;

	virtual
	void
	on_cut_activated () = 0;

	virtual
	void
	on_copy_activated () = 0;

	virtual
	void
	on_paste_activated () = 0;

	virtual
	void
	on_delete_activated () = 0;

	virtual
	void
	on_create_clone_activated () = 0;

	virtual
	void
	on_unlink_clone_activated () = 0;

	virtual
	void
	on_group_selected_nodes_activated () = 0;

	virtual
	void
	on_ungroup_activated () = 0;

	virtual
	void
	on_add_to_group_activated () = 0;

	virtual
	void
	on_detach_from_group_activated () = 0;

	virtual
	void
	on_create_parent_transform_activated () = 0;

	virtual
	void
	on_create_parent_group_activated () = 0;

	virtual
	void
	on_create_parent_static_group_activated () = 0;

	virtual
	void
	on_create_parent_switch_activated () = 0;

	virtual
	void
	on_create_parent_billboard_activated () = 0;

	virtual
	void
	on_create_parent_collision_activated () = 0;

	virtual
	void
	on_create_parent_lod_activated () = 0;

	virtual
	void
	on_create_parent_anchor_activated () = 0;

	virtual
	void
	on_create_parent_layout_layer_activated () = 0;

	virtual
	void
	on_create_parent_screen_group_activated () = 0;

	virtual
	void
	on_create_parent_layout_group_activated () = 0;

	virtual
	void
	on_create_parent_geo_transform_activated () = 0;

	virtual
	void
	on_create_parent_geo_location_activated () = 0;

	virtual
	void
	on_create_parent_cad_part_activated () = 0;

	virtual
	void
	on_create_parent_cad_assembly_activated () = 0;

	virtual
	void
	on_create_parent_cad_layer_activated () = 0;

	virtual
	void
	on_create_parent_layer_set_activated () = 0;

	virtual
	void
	on_create_parent_layer_activated () = 0;

	virtual
	void
	on_create_parent_viewport_activated () = 0;

	virtual
	void
	on_motion_blur_activated () = 0;

	virtual
	void
	on_hide_all_object_icons_activated () = 0;

	virtual
	void
	on_fullscreen_activated () = 0;

	virtual
	void
	on_unfullscreen_activated () = 0;

	virtual
	void
	on_select_all_activated () = 0;

	virtual
	void
	on_deselect_all_activated () = 0;

	virtual
	void
	on_hide_selected_objects_activated () = 0;

	virtual
	void
	on_hide_unselected_objects_activated () = 0;

	virtual
	void
	on_show_selected_objects_activated () = 0;

	virtual
	void
	on_show_all_objects_activated () = 0;

	virtual
	void
	on_combine_activated () = 0;

	virtual
	void
	on_transform_to_zero_activated () = 0;

	virtual
	void
	on_texture_coordinate_editor_activated () = 0;

	virtual
	void
	on_paint_polygons_activated () = 0;

	virtual
	void
	on_grid_properties_activated () = 0;

	virtual
	void
	on_browser_scenes_activated () = 0;

	virtual
	void
	on_info_activated () = 0;

	virtual
	bool
	on_focus_out_event (GdkEventFocus* event) = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* event) = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* event) = 0;

	virtual
	bool
	on_menubar_button_press_event (GdkEventButton* event) = 0;

	virtual
	void
	on_scenes_activated () = 0;

	virtual
	void
	on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_home () = 0;

	virtual
	bool
	on_previous_button_press_event (GdkEventButton* event) = 0;

	virtual
	void
	on_previous_page () = 0;

	virtual
	bool
	on_next_button_press_event (GdkEventButton* event) = 0;

	virtual
	void
	on_next_page () = 0;

	virtual
	void
	on_location_icon_released (Gtk::EntryIconPosition icon_position, const GdkEventButton* event) = 0;

	virtual
	bool
	on_location_key_press_event (GdkEventKey* event) = 0;

	virtual
	void
	on_node_properties_editor_clicked () = 0;

	virtual
	void
	on_appearance_editor_clicked () = 0;

	virtual
	void
	on_texture_editor_clicked () = 0;

	virtual
	void
	on_text_editor_clicked () = 0;

	virtual
	void
	on_geometry_properties_editor_clicked () = 0;

	virtual
	void
	on_navigation_info_editor_clicked () = 0;

	virtual
	void
	on_viewpoint_editor_clicked () = 0;

	virtual
	void
	on_light_editor_clicked () = 0;

	virtual
	void
	on_lod_editor_clicked () = 0;

	virtual
	void
	on_inline_editor_clicked () = 0;

	virtual
	void
	on_layer_editor_clicked () = 0;

	virtual
	void
	on_precision_placement_panel_clicked () = 0;

	virtual
	void
	on_prototype_instance_dialog_clicked () = 0;

	virtual
	void
	on_node_index_clicked () = 0;

	virtual
	void
	on_hammer_clicked () = 0;

	virtual
	bool
	on_notebook_button_press_event (GdkEventButton* event) = 0;

	virtual
	void
	on_browser_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_browser_reordered (Gtk::Widget* page, guint page_num) = 0;

	virtual
	void
	on_switch_browser (Gtk::Widget* page, guint page_num) = 0;

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
	on_look_at_selection_clicked () = 0;

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	void
	on_look_at_toggled () = 0;

	virtual
	~X3DBrowserWindowInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                      filename;
	Glib::RefPtr <Gtk::Builder>      m_builder;
	Glib::RefPtr <Gtk::IconFactory>  m_IconFactory;
	Glib::RefPtr <Gtk::ToggleAction> m_AngleLayoutToolAction;
	Glib::RefPtr <Gtk::ToggleAction> m_BackgroundsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_BrowserAction;
	Glib::RefPtr <Gtk::ToggleAction> m_EditorAction;
	Glib::RefPtr <Gtk::ToggleAction> m_FlatAction;
	Glib::RefPtr <Gtk::ToggleAction> m_FogsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_FollowPrimarySelectionAction;
	Glib::RefPtr <Gtk::ToggleAction> m_FooterAction;
	Glib::RefPtr <Gtk::ToggleAction> m_GouraudAction;
	Glib::RefPtr <Gtk::ToggleAction> m_GridLayoutToolAction;
	Glib::RefPtr <Gtk::ToggleAction> m_LightsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_MenubarAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PhongAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PointsetAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PrimitiveQualityHighAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PrimitiveQualityLowAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PrimitiveQualityMediumAction;
	Glib::RefPtr <Gtk::ToggleAction> m_ProximitySensorsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_RenderingPropertiesAction;
	Glib::RefPtr <Gtk::ToggleAction> m_RubberbandAction;
	Glib::RefPtr <Gtk::ToggleAction> m_SelectLowestAction;
	Glib::RefPtr <Gtk::ToggleAction> m_SidebarAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TabsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TextureQualityHighAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TextureQualityLowAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TextureQualityMediumAction;
	Glib::RefPtr <Gtk::ToggleAction> m_ToolbarAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TransformSensorsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_ViewpointsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_VisibilitySensorsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_WireframeAction;
	Gtk::Image*                      m_ExamineViewerImage;
	Gtk::Image*                      m_FileImportImage;
	Gtk::Image*                      m_FileImportImage1;
	Gtk::Image*                      m_FlyViewerImage;
	Gtk::Menu*                       m_HistoryMenu;
	Gtk::Image*                      m_NoneViewerImage;
	Gtk::Image*                      m_OpenLocationImage;
	Gtk::Image*                      m_OpenLocationImage1;
	Gtk::Image*                      m_PlaneViewerImage;
	Gtk::Image*                      m_WalkViewerImage;
	Gtk::Menu*                       m_ViewerTypeMenu;
	Gtk::ImageMenuItem*              m_ExamineViewerMenuItem;
	Gtk::ImageMenuItem*              m_WalkViewerMenuItem;
	Gtk::ImageMenuItem*              m_FlyViewerMenuItem;
	Gtk::ImageMenuItem*              m_PlaneViewerMenuItem;
	Gtk::ImageMenuItem*              m_NoneViewerMenuItem;
	Gtk::Image*                      m_WorkspacesImage;
	Gtk::Image*                      m_WorkspacesImage1;
	Gtk::Menu*                       m_BrowserMenu;
	Gtk::MenuItem*                   m_BrowserFileMenuItem;
	Gtk::ImageMenuItem*              m_BrowserNewMenuItem;
	Gtk::ImageMenuItem*              m_BrowserOpenMenuItem;
	Gtk::ImageMenuItem*              m_BrowserOpenRecentMenuItem;
	Gtk::ImageMenuItem*              m_BrowserOpenLocationMenuItem;
	Gtk::ImageMenuItem*              m_BrowserImportMenuItem;
	Gtk::ImageMenuItem*              m_BrowserSaveMenuItem;
	Gtk::ImageMenuItem*              m_BrowserSaveAsMenuItem;
	Gtk::MenuItem*                   m_BrowserSaveACopyMenuItem;
	Gtk::ImageMenuItem*              m_BrowserExportMenuItem;
	Gtk::ImageMenuItem*              m_BrowserRevertMenuItem;
	Gtk::ImageMenuItem*              m_BrowserRemoveUnusedPrototypesMenuItem;
	Gtk::ImageMenuItem*              m_BrowserCloseMenuItem;
	Gtk::ImageMenuItem*              m_BrowserQuitMenuItem;
	Gtk::MenuItem*                   m_BrowserEditMenuItem;
	Gtk::ImageMenuItem*              m_BrowserUndoMenuItem;
	Gtk::ImageMenuItem*              m_BrowserRedoMenuItem;
	Gtk::ImageMenuItem*              m_BrowserCutMenuItem;
	Gtk::ImageMenuItem*              m_BrowserCopyMenuItem;
	Gtk::ImageMenuItem*              m_BrowserPasteMenuItem;
	Gtk::ImageMenuItem*              m_BrowserDeleteMenuItem;
	Gtk::MenuItem*                   m_BrowserCloneMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateCloneMenuItem;
	Gtk::MenuItem*                   m_BrowserUnlinkCloneMenuItem;
	Gtk::ImageMenuItem*              m_BrowserGroupSelectedNodesMenuItem;
	Gtk::ImageMenuItem*              m_BrowserUngroupMenuItem;
	Gtk::MenuItem*                   m_BrowserAddToGroupMenuItem;
	Gtk::MenuItem*                   m_BrowserDetachFromGroupMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentTransformMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentGroupMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentStaticGroupMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentSwitchMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentBillboardMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentCollisionMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentLODMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentAnchorMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentLayoutLayerMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentScreenGroupMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentLayoutGroupMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentGeoTransformMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentGeoLocationMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentCADPartMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentCADAssemblyMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentCADLayerMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentLayerSetMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentLayerMenuItem;
	Gtk::MenuItem*                   m_BrowserCreateParentViewportMenuItem;
	Gtk::MenuItem*                   m_BrowserViewMenuItem;
	Gtk::CheckMenuItem*              m_BrowserMenubarMenuItem;
	Gtk::CheckMenuItem*              m_BrowserToolbarMenuItem;
	Gtk::CheckMenuItem*              m_BrowserSidebarMenuItem;
	Gtk::CheckMenuItem*              m_BrowserFooterMenuItem;
	Gtk::CheckMenuItem*              m_BrowserTabsMenuItem;
	Gtk::ImageMenuItem*              m_BrowserEnvironmentMenuItem;
	Gtk::CheckMenuItem*              m_BrowserEditorMenuItem;
	Gtk::CheckMenuItem*              m_BrowserBrowserMenuItem;
	Gtk::SeparatorMenuItem*          m_BrowserBrowserOptionsSeparator;
	Gtk::ImageMenuItem*              m_BrowserMotionBlurMenuItem;
	Gtk::ImageMenuItem*              m_BrowserShadingMenuItem;
	Gtk::CheckMenuItem*              m_BrowserPhongMenuItem;
	Gtk::CheckMenuItem*              m_BrowserGouraudMenuItem;
	Gtk::CheckMenuItem*              m_BrowserFlatMenuItem;
	Gtk::CheckMenuItem*              m_BrowserWireframeMenuItem;
	Gtk::CheckMenuItem*              m_BrowserPointsetMenuItem;
	Gtk::ImageMenuItem*              m_BrowserPrimitiveQualityMenuItem;
	Gtk::CheckMenuItem*              m_BrowserPrimitiveQualityHighMenuItem;
	Gtk::CheckMenuItem*              m_BrowserPrimitiveQualityMediumMenuItem;
	Gtk::CheckMenuItem*              m_BrowserPrimitiveQualityLowMenuItem;
	Gtk::ImageMenuItem*              m_BrowserTextureQualityMenuItem;
	Gtk::CheckMenuItem*              m_BrowserTextureQualityHighMenuItem;
	Gtk::CheckMenuItem*              m_BrowserTextureQualityMediumMenuItem;
	Gtk::CheckMenuItem*              m_BrowserTextureQualityLowMenuItem;
	Gtk::MenuItem*                   m_BrowserShowHideEnvironmentalEffectsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserBackgroundsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserFogsMenuItem;
	Gtk::MenuItem*                   m_BrowserObjectIconsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserLightsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserProximitySensorsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserTransformSensorsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserVisibilitySensorsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserViewpointsMenuItem;
	Gtk::MenuItem*                   m_BrowserHideAllObjectIconsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserRubberbandMenuItem;
	Gtk::CheckMenuItem*              m_BrowserRenderingPropertiesMenuItem;
	Gtk::ImageMenuItem*              m_BrowserFullScreenMenuItem;
	Gtk::ImageMenuItem*              m_BrowserUnFullScreenMenuItem;
	Gtk::MenuItem*                   m_BrowserSelectionMenuItem;
	Gtk::ImageMenuItem*              m_BrowserSelectAllMenuItem;
	Gtk::MenuItem*                   m_BrowserDeselectAllMenuItem;
	Gtk::MenuItem*                   m_BrowserHideSelectedObjectsMenuItem;
	Gtk::MenuItem*                   m_BrowserHideUnselectedObjectsMenuItem;
	Gtk::MenuItem*                   m_BrowserShowSelectedObjectsMenuItem;
	Gtk::MenuItem*                   m_BrowserShowAllObjectsMenuItem;
	Gtk::CheckMenuItem*              m_BrowserSelectLowestMenuItem;
	Gtk::CheckMenuItem*              m_BrowserFollowPrimarySelectionMenuItem;
	Gtk::MenuItem*                   m_BrowserObjectMenuItem;
	Gtk::ImageMenuItem*              m_BrowserCombineMenuItem;
	Gtk::ImageMenuItem*              m_BrowserTransformToZeroMenuItem;
	Gtk::ImageMenuItem*              m_BrowserTextureCoordinateEditorMenuItem;
	Gtk::ImageMenuItem*              m_BrowserPaintPolygonsMenuItem;
	Gtk::MenuItem*                   m_BrowserLayoutMenuItem;
	Gtk::CheckMenuItem*              m_BrowserGridLayoutToolMenuItem;
	Gtk::CheckMenuItem*              m_BrowserAngleLayoutToolMenuItem;
	Gtk::ImageMenuItem*              m_BrowserGridPropertiesMenuItem;
	Gtk::MenuItem*                   m_BrowserScenesMenuItem;
	Gtk::Menu*                       m_BrowserScenesMenu;
	Gtk::MenuItem*                   m_BrowserHelpMenuItem;
	Gtk::ImageMenuItem*              m_BrowserInfoMenuItem;
	Gtk::ApplicationWindow*          m_Window;
	Gtk::Box*                        m_Widget;
	Gtk::Box*                        m_HeaderBox;
	Gtk::Box*                        m_HeaderSubBox;
	Gtk::MenuBar*                    m_Menubar;
	Gtk::MenuItem*                   m_FileMenuItem;
	Gtk::ImageMenuItem*              m_NewMenuItem;
	Gtk::ImageMenuItem*              m_OpenMenuItem;
	Gtk::ImageMenuItem*              m_OpenRecentMenuItem;
	Gtk::ImageMenuItem*              m_OpenLocationMenuItem;
	Gtk::ImageMenuItem*              m_ImportMenuItem;
	Gtk::ImageMenuItem*              m_SaveMenuItem;
	Gtk::ImageMenuItem*              m_SaveAsMenuItem;
	Gtk::MenuItem*                   m_SaveACopyMenuItem;
	Gtk::ImageMenuItem*              m_ExportMenuItem;
	Gtk::ImageMenuItem*              m_RevertMenuItem;
	Gtk::ImageMenuItem*              m_RemoveUnusedPrototypesMenuItem;
	Gtk::ImageMenuItem*              m_CloseMenuItem;
	Gtk::ImageMenuItem*              m_QuitMenuItem;
	Gtk::MenuItem*                   m_EditMenuItem;
	Gtk::ImageMenuItem*              m_UndoMenuItem;
	Gtk::ImageMenuItem*              m_RedoMenuItem;
	Gtk::ImageMenuItem*              m_CutMenuItem;
	Gtk::ImageMenuItem*              m_CopyMenuItem;
	Gtk::ImageMenuItem*              m_PasteMenuItem;
	Gtk::ImageMenuItem*              m_DeleteMenuItem;
	Gtk::MenuItem*                   m_CloneMenuItem;
	Gtk::MenuItem*                   m_CreateCloneMenuItem;
	Gtk::MenuItem*                   m_UnlinkCloneMenuItem;
	Gtk::ImageMenuItem*              m_GroupSelectedNodesMenuItem;
	Gtk::ImageMenuItem*              m_UngroupMenuItem;
	Gtk::MenuItem*                   m_AddToGroupMenuItem;
	Gtk::MenuItem*                   m_DetachFromGroupMenuItem;
	Gtk::MenuItem*                   m_CreateParentMenuItem;
	Gtk::MenuItem*                   m_CreateParentTransformMenuItem;
	Gtk::MenuItem*                   m_CreateParentGroupMenuItem;
	Gtk::MenuItem*                   m_CreateParentStaticGroupMenuItem;
	Gtk::MenuItem*                   m_CreateParentSwitchMenuItem;
	Gtk::MenuItem*                   m_CreateParentBillboardMenuItem;
	Gtk::MenuItem*                   m_CreateParentCollisionMenuItem;
	Gtk::MenuItem*                   m_CreateParentLODMenuItem;
	Gtk::MenuItem*                   m_CreateParentAnchorMenuItem;
	Gtk::MenuItem*                   m_CreateParentLayoutLayerMenuItem;
	Gtk::MenuItem*                   m_CreateParentScreenGroupMenuItem;
	Gtk::MenuItem*                   m_CreateParentLayoutGroupMenuItem;
	Gtk::MenuItem*                   m_CreateParentGeoTransformMenuItem;
	Gtk::MenuItem*                   m_CreateParentGeoLocationMenuItem;
	Gtk::MenuItem*                   m_CreateParentCADPartMenuItem;
	Gtk::MenuItem*                   m_CreateParentCADAssemblyMenuItem;
	Gtk::MenuItem*                   m_CreateParentCADLayerMenuItem;
	Gtk::MenuItem*                   m_CreateParentLayerSetMenuItem;
	Gtk::MenuItem*                   m_CreateParentLayerMenuItem;
	Gtk::MenuItem*                   m_CreateParentViewportMenuItem;
	Gtk::MenuItem*                   m_ViewMenuItem;
	Gtk::CheckMenuItem*              m_MenubarMenuItem;
	Gtk::CheckMenuItem*              m_ToolbarMenuItem;
	Gtk::CheckMenuItem*              m_SidebarMenuItem;
	Gtk::CheckMenuItem*              m_FooterMenuItem;
	Gtk::CheckMenuItem*              m_TabsMenuItem;
	Gtk::ImageMenuItem*              m_EnvironmentMenuItem;
	Gtk::CheckMenuItem*              m_EditorMenuItem;
	Gtk::CheckMenuItem*              m_BrowserMenuItem;
	Gtk::SeparatorMenuItem*          m_BrowserOptionsSeparator;
	Gtk::ImageMenuItem*              m_MotionBlurMenuItem;
	Gtk::ImageMenuItem*              m_ShadingMenuItem;
	Gtk::CheckMenuItem*              m_PhongMenuItem;
	Gtk::CheckMenuItem*              m_GouraudMenuItem;
	Gtk::CheckMenuItem*              m_FlatMenuItem;
	Gtk::CheckMenuItem*              m_WireframeMenuItem;
	Gtk::CheckMenuItem*              m_PointsetMenuItem;
	Gtk::ImageMenuItem*              m_PrimitiveQualityMenuItem;
	Gtk::CheckMenuItem*              m_PrimitiveQualityHighMenuItem;
	Gtk::CheckMenuItem*              m_PrimitiveQualityMediumMenuItem;
	Gtk::CheckMenuItem*              m_PrimitiveQualityLowMenuItem;
	Gtk::ImageMenuItem*              m_TextureQualityMenuItem;
	Gtk::CheckMenuItem*              m_TextureQualityHighMenuItem;
	Gtk::CheckMenuItem*              m_TextureQualityMediumMenuItem;
	Gtk::CheckMenuItem*              m_TextureQualityLowMenuItem;
	Gtk::MenuItem*                   m_ShowHideEnvironmentalEffectsMenuItem;
	Gtk::CheckMenuItem*              m_BackgroundsMenuItem;
	Gtk::CheckMenuItem*              m_FogsMenuItem;
	Gtk::MenuItem*                   m_ObjectIconsMenuItem;
	Gtk::CheckMenuItem*              m_LightsMenuItem;
	Gtk::CheckMenuItem*              m_ProximitySensorsMenuItem;
	Gtk::CheckMenuItem*              m_TransformSensorsMenuItem;
	Gtk::CheckMenuItem*              m_VisibilitySensorsMenuItem;
	Gtk::CheckMenuItem*              m_ViewpointsMenuItem;
	Gtk::MenuItem*                   m_HideAllObjectIconsMenuItem;
	Gtk::CheckMenuItem*              m_RubberbandMenuItem;
	Gtk::CheckMenuItem*              m_RenderingPropertiesMenuItem;
	Gtk::ImageMenuItem*              m_FullScreenMenuItem;
	Gtk::ImageMenuItem*              m_UnFullScreenMenuItem;
	Gtk::MenuItem*                   m_SelectionMenuItem;
	Gtk::ImageMenuItem*              m_SelectAllMenuItem;
	Gtk::MenuItem*                   m_DeselectAllMenuItem;
	Gtk::MenuItem*                   m_HideSelectedObjectsMenuItem;
	Gtk::MenuItem*                   m_HideUnselectedObjectsMenuItem;
	Gtk::MenuItem*                   m_ShowSelectedObjectsMenuItem;
	Gtk::MenuItem*                   m_ShowAllObjectsMenuItem;
	Gtk::CheckMenuItem*              m_SelectLowestMenuItem;
	Gtk::CheckMenuItem*              m_FollowPrimarySelectionMenuItem;
	Gtk::MenuItem*                   m_ObjectMenuItem;
	Gtk::ImageMenuItem*              m_CombineMenuItem;
	Gtk::ImageMenuItem*              m_TransformToZeroMenuItem;
	Gtk::ImageMenuItem*              m_TextureCoordinateEditorMenuItem;
	Gtk::ImageMenuItem*              m_PaintPolygonsMenuItem;
	Gtk::MenuItem*                   m_LayoutMenuItem;
	Gtk::CheckMenuItem*              m_GridLayoutToolMenuItem;
	Gtk::CheckMenuItem*              m_AngleLayoutToolMenuItem;
	Gtk::ImageMenuItem*              m_GridPropertiesMenuItem;
	Gtk::MenuItem*                   m_ScenesMenuItem;
	Gtk::Menu*                       m_ScenesMenu;
	Gtk::MenuItem*                   m_HelpMenuItem;
	Gtk::ImageMenuItem*              m_InfoMenuItem;
	Gtk::Box*                        m_Toolbar;
	Gtk::Box*                        m_LocationBar;
	Gtk::Toolbar*                    m_LocationBar1;
	Gtk::ToolButton*                 m_AddTabButton;
	Gtk::ToolButton*                 m_HomeButton;
	Gtk::ToolButton*                 m_PreviousButton;
	Gtk::ToolButton*                 m_NextButton;
	Gtk::Entry*                      m_LocationEntry;
	Gtk::Toolbar*                    m_LocationBar2;
	Gtk::ToolButton*                 m_ReloadButton;
	Gtk::Toolbar*                    m_EditToolBar;
	Gtk::ToolButton*                 m_NewButton;
	Gtk::MenuToolButton*             m_OpenButton;
	Gtk::ToolButton*                 m_ImportButton;
	Gtk::ToolButton*                 m_SaveButton;
	Gtk::ToolButton*                 m_RefreshButton;
	Gtk::SeparatorToolItem*          m_SeparatorToolItem1;
	Gtk::ToolButton*                 m_UndoButton;
	Gtk::ToolButton*                 m_RedoButton;
	Gtk::SeparatorToolItem*          m_SeparatorToolItem2;
	Gtk::ToolButton*                 m_NodePropertiesEditorButton;
	Gtk::ToolButton*                 m_AppearanceEditorButton;
	Gtk::ToolButton*                 m_TextureEditorButton;
	Gtk::ToolButton*                 m_TextEditorButton;
	Gtk::ToolButton*                 m_GeometryPropertiesEditorButton;
	Gtk::ToolButton*                 m_NavigationInfoEditorButton;
	Gtk::ToolButton*                 m_ViewpointEditorButton;
	Gtk::ToolButton*                 m_LightEditorButton;
	Gtk::ToolButton*                 m_LODEditorButton;
	Gtk::ToolButton*                 m_InlineEditorButton;
	Gtk::ToolButton*                 m_LayerEditorButton;
	Gtk::ToolButton*                 m_PrecisionPlacementPanelButton;
	Gtk::ToolButton*                 m_CreatePrototypeInstanceButton;
	Gtk::ToolButton*                 m_NodeIndexButton;
	Gtk::ToolButton*                 m_HammerButton;
	Gtk::Paned*                      m_VPaned;
	Gtk::Paned*                      m_HPaned;
	Gtk::Box*                        m_MasterBox;
	Gtk::Notebook*                   m_BrowserNotebook;
	Gtk::Box*                        m_Dashboard;
	Gtk::Toolbar*                    m_DashboardToolBar;
	Gtk::ToolButton*                 m_TabButton;
	Gtk::RadioToolButton*            m_HandButton;
	Gtk::RadioToolButton*            m_ArrowButton;
	Gtk::ToolButton*                 m_PlayPauseButton;
	Gtk::SeparatorToolItem*          m_SelectSeparator;
	Gtk::ToolButton*                 m_SelectParentButton;
	Gtk::ToolButton*                 m_SelectChildrenButton;
	Gtk::SeparatorToolItem*          m_ViewerSeparator;
	Gtk::MenuToolButton*             m_ViewerButton;
	Gtk::ToolButton*                 m_StraightenButton;
	Gtk::SeparatorToolItem*          m_LookAtSeparator;
	Gtk::ToolButton*                 m_LookAtSelectionButton;
	Gtk::ToolButton*                 m_LookAtAllButton;
	Gtk::ToggleToolButton*           m_LookAtButton;
	Gtk::Box*                        m_Footer;
	Gtk::Label*                      m_FooterLabel;
	Gtk::Notebook*                   m_FooterNotebook;
	Gtk::Box*                        m_ConsoleBox;
	Gtk::Box*                        m_ScriptEditorBox;
	Gtk::Box*                        m_AnimationEditorBox;
	Gtk::Box*                        m_Sidebar;
	Gtk::Label*                      m_SideBarLabel;
	Gtk::Notebook*                   m_SideBarNotebook;
	Gtk::Box*                        m_ViewpointListBox;
	Gtk::Box*                        m_HistoryViewBox;
	Gtk::Box*                        m_LibraryViewBox;
	Gtk::Box*                        m_OutlineEditorBox;
	Gtk::MessageDialog*              m_MessageDialog;

};

} // puck
} // titania

#endif

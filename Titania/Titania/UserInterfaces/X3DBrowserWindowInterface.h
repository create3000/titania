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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

/**
 *  Gtk Interface for BrowserWindow.
 */
class X3DBrowserWindowInterface :
	public X3DUserInterface
{
public:

	///  @name Construction

	X3DBrowserWindowInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DBrowserWindowInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DBrowserWindowInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::AccelGroup> &
	getAccelGroup () const
	{ return m_AccelGroup; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getActivateSnapSourceAction () const
	{ return m_ActivateSnapSourceAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getActivateSnapTargetAction () const
	{ return m_ActivateSnapTargetAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getEditFogCoordinatesAction () const
	{ return m_EditFogCoordinatesAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getFollowPrimarySelectionAction () const
	{ return m_FollowPrimarySelectionAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getFooterAction () const
	{ return m_FooterAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getLogarithmicDepthBufferAction () const
	{ return m_LogarithmicDepthBufferAction; }

	const Glib::RefPtr <Gtk::ToggleAction> &
	getMenubarAction () const
	{ return m_MenubarAction; }

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
	getTransformToolModeAction () const
	{ return m_TransformToolModeAction; }

	Gtk::Image &
	getFileImportImage () const
	{ return *m_FileImportImage; }

	Gtk::Image &
	getFileImportImage1 () const
	{ return *m_FileImportImage1; }

	Gtk::Menu &
	getHistoryMenu () const
	{ return *m_HistoryMenu; }

	Gtk::Image &
	getOpenLocationImage () const
	{ return *m_OpenLocationImage; }

	Gtk::Image &
	getOpenLocationImage1 () const
	{ return *m_OpenLocationImage1; }

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
	getBrowserScenePropertiesMenuItem () const
	{ return *m_BrowserScenePropertiesMenuItem; }

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
	getBrowserUndoHistoryMenuItem () const
	{ return *m_BrowserUndoHistoryMenuItem; }

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
	getBrowserCreateParentViewpointGroupMenuItem () const
	{ return *m_BrowserCreateParentViewpointGroupMenuItem; }

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
	getBrowserCreateParentPickableGroupMenuItem () const
	{ return *m_BrowserCreateParentPickableGroupMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentCollidableShapeMenuItem () const
	{ return *m_BrowserCreateParentCollidableShapeMenuItem; }

	Gtk::MenuItem &
	getBrowserCreateParentCollidableOffsetMenuItem () const
	{ return *m_BrowserCreateParentCollidableOffsetMenuItem; }

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
	getBrowserMotionBlurMenuItem () const
	{ return *m_BrowserMotionBlurMenuItem; }

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

	Gtk::CheckMenuItem &
	getBrowserTransformToolModeMenuItem () const
	{ return *m_BrowserTransformToolModeMenuItem; }

	Gtk::MenuItem &
	getBrowserGeometryMenuItem () const
	{ return *m_BrowserGeometryMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserUnionMenuItem () const
	{ return *m_BrowserUnionMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserDifferenceMenuItem () const
	{ return *m_BrowserDifferenceMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserIntersectionMenuItem () const
	{ return *m_BrowserIntersectionMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserExclusionMenuItem () const
	{ return *m_BrowserExclusionMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserCombineMenuItem () const
	{ return *m_BrowserCombineMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserTransformToZeroMenuItem () const
	{ return *m_BrowserTransformToZeroMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserFogCoordinateEditorMenuItem () const
	{ return *m_BrowserFogCoordinateEditorMenuItem; }

	Gtk::MenuItem &
	getBrowserLayoutMenuItem () const
	{ return *m_BrowserLayoutMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserBrowserSizeMenuItem () const
	{ return *m_BrowserBrowserSizeMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserBackgroundImageMenuItem () const
	{ return *m_BrowserBackgroundImageMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserGridLayoutToolMenuItem () const
	{ return *m_BrowserGridLayoutToolMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserAngleLayoutToolMenuItem () const
	{ return *m_BrowserAngleLayoutToolMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserAxonometricGridLayoutToolMenuItem () const
	{ return *m_BrowserAxonometricGridLayoutToolMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserGridPropertiesMenuItem () const
	{ return *m_BrowserGridPropertiesMenuItem; }

	Gtk::SeparatorMenuItem &
	getBrowserSeparatorMenuItem18 () const
	{ return *m_BrowserSeparatorMenuItem18; }

	Gtk::CheckMenuItem &
	getBrowserActivateSnapTargetMenuItem () const
	{ return *m_BrowserActivateSnapTargetMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserActivateSnapSourceMenuItem () const
	{ return *m_BrowserActivateSnapSourceMenuItem; }

	Gtk::MenuItem &
	getBrowserCenterSnapTargetInSelectionMenuItem () const
	{ return *m_BrowserCenterSnapTargetInSelectionMenuItem; }

	Gtk::MenuItem &
	getBrowserMoveSelectionToSnapTargetMenuItem () const
	{ return *m_BrowserMoveSelectionToSnapTargetMenuItem; }

	Gtk::MenuItem &
	getBrowserMoveSelectionCenterToSnapTargetMenuItem () const
	{ return *m_BrowserMoveSelectionCenterToSnapTargetMenuItem; }

	Gtk::MenuItem &
	getBrowserExternalToolsMenuItem () const
	{ return *m_BrowserExternalToolsMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserManageExternalToolsMenuItem () const
	{ return *m_BrowserManageExternalToolsMenuItem; }

	Gtk::MenuItem &
	getBrowserScenesMenuItem () const
	{ return *m_BrowserScenesMenuItem; }

	Gtk::Menu &
	getBrowserScenesMenu () const
	{ return *m_BrowserScenesMenu; }

	Gtk::MenuItem &
	getBrowserHelpMenuItem () const
	{ return *m_BrowserHelpMenuItem; }

	Gtk::CheckMenuItem &
	getBrowserLogarithmicDepthBufferMenuItem () const
	{ return *m_BrowserLogarithmicDepthBufferMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserInfoMenuItem () const
	{ return *m_BrowserInfoMenuItem; }

	Gtk::ApplicationWindow &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

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
	getScenePropertiesMenuItem () const
	{ return *m_ScenePropertiesMenuItem; }

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
	getUndoHistoryMenuItem () const
	{ return *m_UndoHistoryMenuItem; }

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
	getCreateParentViewpointGroupMenuItem () const
	{ return *m_CreateParentViewpointGroupMenuItem; }

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
	getCreateParentPickableGroupMenuItem () const
	{ return *m_CreateParentPickableGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentCollidableShapeMenuItem () const
	{ return *m_CreateParentCollidableShapeMenuItem; }

	Gtk::MenuItem &
	getCreateParentCollidableOffsetMenuItem () const
	{ return *m_CreateParentCollidableOffsetMenuItem; }

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
	getMotionBlurMenuItem () const
	{ return *m_MotionBlurMenuItem; }

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

	Gtk::CheckMenuItem &
	getTransformToolModeMenuItem () const
	{ return *m_TransformToolModeMenuItem; }

	Gtk::MenuItem &
	getGeometryMenuItem () const
	{ return *m_GeometryMenuItem; }

	Gtk::ImageMenuItem &
	getUnionMenuItem () const
	{ return *m_UnionMenuItem; }

	Gtk::ImageMenuItem &
	getDifferenceMenuItem () const
	{ return *m_DifferenceMenuItem; }

	Gtk::ImageMenuItem &
	getIntersectionMenuItem () const
	{ return *m_IntersectionMenuItem; }

	Gtk::ImageMenuItem &
	getExclusionMenuItem () const
	{ return *m_ExclusionMenuItem; }

	Gtk::ImageMenuItem &
	getCombineMenuItem () const
	{ return *m_CombineMenuItem; }

	Gtk::ImageMenuItem &
	getTransformToZeroMenuItem () const
	{ return *m_TransformToZeroMenuItem; }

	Gtk::CheckMenuItem &
	getFogCoordinateEditorMenuItem () const
	{ return *m_FogCoordinateEditorMenuItem; }

	Gtk::MenuItem &
	getLayoutMenuItem () const
	{ return *m_LayoutMenuItem; }

	Gtk::ImageMenuItem &
	getBrowserSizeMenuItem () const
	{ return *m_BrowserSizeMenuItem; }

	Gtk::ImageMenuItem &
	getBackgroundImageMenuItem () const
	{ return *m_BackgroundImageMenuItem; }

	Gtk::CheckMenuItem &
	getGridLayoutToolMenuItem () const
	{ return *m_GridLayoutToolMenuItem; }

	Gtk::CheckMenuItem &
	getAngleLayoutToolMenuItem () const
	{ return *m_AngleLayoutToolMenuItem; }

	Gtk::CheckMenuItem &
	getAxonometricGridLayoutToolMenuItem () const
	{ return *m_AxonometricGridLayoutToolMenuItem; }

	Gtk::ImageMenuItem &
	getGridPropertiesMenuItem () const
	{ return *m_GridPropertiesMenuItem; }

	Gtk::SeparatorMenuItem &
	getSeparatorMenuItem38 () const
	{ return *m_SeparatorMenuItem38; }

	Gtk::CheckMenuItem &
	getActivateSnapTargetMenuItem () const
	{ return *m_ActivateSnapTargetMenuItem; }

	Gtk::CheckMenuItem &
	getActivateSnapSourceMenuItem () const
	{ return *m_ActivateSnapSourceMenuItem; }

	Gtk::MenuItem &
	getCenterSnapTargetInSelectionMenuItem () const
	{ return *m_CenterSnapTargetInSelectionMenuItem; }

	Gtk::MenuItem &
	getMoveSelectionToSnapTargetMenuItem () const
	{ return *m_MoveSelectionToSnapTargetMenuItem; }

	Gtk::MenuItem &
	getMoveSelectionCenterToSnapTargetMenuItem () const
	{ return *m_MoveSelectionCenterToSnapTargetMenuItem; }

	Gtk::MenuItem &
	getExternalToolsMenuItem () const
	{ return *m_ExternalToolsMenuItem; }

	Gtk::ImageMenuItem &
	getManageExternalToolsMenuItem () const
	{ return *m_ManageExternalToolsMenuItem; }

	Gtk::MenuItem &
	getScenesMenuItem () const
	{ return *m_ScenesMenuItem; }

	Gtk::Menu &
	getScenesMenu () const
	{ return *m_ScenesMenu; }

	Gtk::MenuItem &
	getHelpMenuItem () const
	{ return *m_HelpMenuItem; }

	Gtk::CheckMenuItem &
	getLogarithmicDepthBufferMenuItem () const
	{ return *m_LogarithmicDepthBufferMenuItem; }

	Gtk::ImageMenuItem &
	getInfoMenuItem () const
	{ return *m_InfoMenuItem; }

	Gtk::Box &
	getToolbar () const
	{ return *m_Toolbar; }

	Gtk::Toolbar &
	getEditToolBar () const
	{ return *m_EditToolBar; }

	Gtk::ToolButton &
	getNewButton () const
	{ return *m_NewButton; }

	Gtk::ToolButton &
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
	getNodeIndexButton () const
	{ return *m_NodeIndexButton; }

	Gtk::ToolButton &
	getGeometryEditorButton () const
	{ return *m_GeometryEditorButton; }

	Gtk::ToolButton &
	getPolygonColoringEditorButton () const
	{ return *m_PolygonColoringEditorButton; }

	Gtk::ToolButton &
	getTextureMappingEditorButton () const
	{ return *m_TextureMappingEditorButton; }

	Gtk::ToolButton &
	getPrototypeEditorButton () const
	{ return *m_PrototypeEditorButton; }

	Gtk::ToolButton &
	getNodeEditorButton () const
	{ return *m_NodeEditorButton; }

	Gtk::Toolbar &
	getPrimitivesToolBar () const
	{ return *m_PrimitivesToolBar; }

	Gtk::ToolButton &
	getArcCloseButton () const
	{ return *m_ArcCloseButton; }

	Gtk::ToolButton &
	getDiskButton () const
	{ return *m_DiskButton; }

	Gtk::ToolButton &
	getRectangleButton () const
	{ return *m_RectangleButton; }

	Gtk::ToolButton &
	getStarButton () const
	{ return *m_StarButton; }

	Gtk::SeparatorToolItem &
	getSeparatorToolItem3 () const
	{ return *m_SeparatorToolItem3; }

	Gtk::ToolButton &
	getBoxButton () const
	{ return *m_BoxButton; }

	Gtk::ToolButton &
	getConeButton () const
	{ return *m_ConeButton; }

	Gtk::ToolButton &
	getCylinderButton () const
	{ return *m_CylinderButton; }

	Gtk::ToolButton &
	getElevationGridButton () const
	{ return *m_ElevationGridButton; }

	Gtk::ToolButton &
	getPyramidButton () const
	{ return *m_PyramidButton; }

	Gtk::ToolButton &
	getSphereButton () const
	{ return *m_SphereButton; }

	Gtk::ToolButton &
	getTextButton () const
	{ return *m_TextButton; }

	Gtk::Paned &
	getVPaned () const
	{ return *m_VPaned; }

	Gtk::Paned &
	getHPaned () const
	{ return *m_HPaned; }

	Gtk::Overlay &
	getBrowserOverlay () const
	{ return *m_BrowserOverlay; }

	Gtk::Notebook &
	getBrowserNotebook () const
	{ return *m_BrowserNotebook; }

	Gtk::Box &
	getDashboard () const
	{ return *m_Dashboard; }

	Gtk::Toolbar &
	getDashboardToolBar () const
	{ return *m_DashboardToolBar; }

	Gtk::Box &
	getLogoBox () const
	{ return *m_LogoBox; }

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
	getSelectChildButton () const
	{ return *m_SelectChildButton; }

	Gtk::SeparatorToolItem &
	getViewerSeparator () const
	{ return *m_ViewerSeparator; }

	Gtk::ToolButton &
	getSelectViewerButton () const
	{ return *m_SelectViewerButton; }

	Gtk::ToolButton &
	getStraightenButton () const
	{ return *m_StraightenButton; }

	Gtk::ToggleToolButton &
	getStraightenHorizonButton () const
	{ return *m_StraightenHorizonButton; }

	Gtk::SeparatorToolItem &
	getLookAtSeparator () const
	{ return *m_LookAtSeparator; }

	Gtk::ToolButton &
	getLookAtSelectionButton () const
	{ return *m_LookAtSelectionButton; }

	Gtk::ToolButton &
	getLookAtAllButton () const
	{ return *m_LookAtAllButton; }

	Gtk::RadioToolButton &
	getLookAtButton () const
	{ return *m_LookAtButton; }

	Gtk::Box &
	getFooterBox () const
	{ return *m_FooterBox; }

	Gtk::Box &
	getSidebarBox () const
	{ return *m_SidebarBox; }

	Gtk::Popover &
	getSelectViewerPopover () const
	{ return *m_SelectViewerPopover; }

	Gtk::RadioButton &
	getExamineViewerButton () const
	{ return *m_ExamineViewerButton; }

	Gtk::RadioButton &
	getWalkViewerButton () const
	{ return *m_WalkViewerButton; }

	Gtk::RadioButton &
	getFlyViewerButton () const
	{ return *m_FlyViewerButton; }

	Gtk::RadioButton &
	getPlaneViewerButton () const
	{ return *m_PlaneViewerButton; }

	Gtk::RadioButton &
	getNoneViewerButton () const
	{ return *m_NoneViewerButton; }

	Gtk::RadioButton &
	getOtherViewerButton () const
	{ return *m_OtherViewerButton; }

	///  @name Signal handlers

	virtual
	void
	on_activate_snap_source_toggled () = 0;

	virtual
	void
	on_activate_snap_target_toggled () = 0;

	virtual
	void
	on_fog_coordinate_editor_toggled () = 0;

	virtual
	void
	on_follow_primary_selection_toggled () = 0;

	virtual
	void
	on_footer_toggled () = 0;

	virtual
	void
	on_logarithmic_depth_buffer_toggled () = 0;

	virtual
	void
	on_menubar_toggled () = 0;

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
	on_transform_tool_mode_toggled () = 0;

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
	on_scene_properties_activated () = 0;

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
	on_undo_history_activated () = 0;

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
	on_create_parent_viewpoint_group_activated () = 0;

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
	on_create_parent_pickable_group_activated () = 0;

	virtual
	void
	on_create_parent_collidable_shape_activated () = 0;

	virtual
	void
	on_create_parent_collidable_offset_activated () = 0;

	virtual
	void
	on_motion_blur_activated () = 0;

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
	on_union_activated () = 0;

	virtual
	void
	on_difference_activated () = 0;

	virtual
	void
	on_intersection_activated () = 0;

	virtual
	void
	on_exclusion_activated () = 0;

	virtual
	void
	on_combine_activated () = 0;

	virtual
	void
	on_transform_to_zero_activated () = 0;

	virtual
	void
	on_browser_size_activate () = 0;

	virtual
	void
	on_background_image_activate () = 0;

	virtual
	void
	on_grid_layout_tool_toggled () = 0;

	virtual
	void
	on_angle_layout_tool_toggled () = 0;

	virtual
	void
	on_axonometric_layout_tool_toggled () = 0;

	virtual
	void
	on_grid_properties_activated () = 0;

	virtual
	void
	on_center_snap_target_in_selection () = 0;

	virtual
	void
	on_move_selection_to_snap_target_activate () = 0;

	virtual
	void
	on_move_selection_center_to_snap_target_activate () = 0;

	virtual
	void
	on_manage_external_tools_activate () = 0;

	virtual
	void
	on_browser_scenes_activated () = 0;

	virtual
	void
	on_info_activated () = 0;

	virtual
	bool
	on_focus_out_event (GdkEventFocus* gdk_event) = 0;

	virtual
	bool
	on_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	bool
	on_key_release_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_style_updated () = 0;

	virtual
	bool
	on_menubar_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	void
	on_scenes_activated () = 0;

	virtual
	void
	on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_node_index_clicked () = 0;

	virtual
	void
	on_geometry_editor_clicked () = 0;

	virtual
	void
	on_color_editor_clicked () = 0;

	virtual
	void
	on_texture_mapping_editor_clicked () = 0;

	virtual
	void
	on_prototype_editor_clicked () = 0;

	virtual
	void
	on_node_editor_clicked () = 0;

	virtual
	void
	on_arc_close_clicked () = 0;

	virtual
	void
	on_disk_clicked () = 0;

	virtual
	void
	on_rectangle_clicked () = 0;

	virtual
	void
	on_star_clicked () = 0;

	virtual
	void
	on_box_clicked () = 0;

	virtual
	void
	on_cone_clicked () = 0;

	virtual
	void
	on_cylinder_clicked () = 0;

	virtual
	void
	on_elevation_grid_clicked () = 0;

	virtual
	void
	on_pyramid_clicked () = 0;

	virtual
	void
	on_sphere_clicked () = 0;

	virtual
	void
	on_text_clicked () = 0;

	virtual
	bool
	on_notebook_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	void
	on_browser_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_page_reordered (Gtk::Widget* page, guint page_num) = 0;

	virtual
	void
	on_switch_page (Gtk::Widget* page, guint page_number) = 0;

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
	on_select_child_button_clicked () = 0;

	virtual
	void
	on_select_viewer_clicked () = 0;

	virtual
	void
	on_straighten_clicked () = 0;

	virtual
	void
	on_straighten_horizon_toggled () = 0;

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
	void
	on_examine_viewer_toggled () = 0;

	virtual
	void
	on_walk_viewer_toggled () = 0;

	virtual
	void
	on_fly_viewer_toggled () = 0;

	virtual
	void
	on_plane_viewer_toggled () = 0;

	virtual
	void
	on_none_viewer_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DBrowserWindowInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::AccelGroup> m_AccelGroup;
	Glib::RefPtr <Gtk::ToggleAction> m_ActivateSnapSourceAction;
	Glib::RefPtr <Gtk::ToggleAction> m_ActivateSnapTargetAction;
	Glib::RefPtr <Gtk::ToggleAction> m_EditFogCoordinatesAction;
	Glib::RefPtr <Gtk::ToggleAction> m_FollowPrimarySelectionAction;
	Glib::RefPtr <Gtk::ToggleAction> m_FooterAction;
	Glib::RefPtr <Gtk::ToggleAction> m_LogarithmicDepthBufferAction;
	Glib::RefPtr <Gtk::ToggleAction> m_MenubarAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PrimitiveQualityHighAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PrimitiveQualityLowAction;
	Glib::RefPtr <Gtk::ToggleAction> m_PrimitiveQualityMediumAction;
	Glib::RefPtr <Gtk::ToggleAction> m_RenderingPropertiesAction;
	Glib::RefPtr <Gtk::ToggleAction> m_RubberbandAction;
	Glib::RefPtr <Gtk::ToggleAction> m_SelectLowestAction;
	Glib::RefPtr <Gtk::ToggleAction> m_SidebarAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TabsAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TextureQualityHighAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TextureQualityLowAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TextureQualityMediumAction;
	Glib::RefPtr <Gtk::ToggleAction> m_ToolbarAction;
	Glib::RefPtr <Gtk::ToggleAction> m_TransformToolModeAction;
	Gtk::Image* m_FileImportImage;
	Gtk::Image* m_FileImportImage1;
	Gtk::Menu* m_HistoryMenu;
	Gtk::Image* m_OpenLocationImage;
	Gtk::Image* m_OpenLocationImage1;
	Gtk::Menu* m_BrowserMenu;
	Gtk::MenuItem* m_BrowserFileMenuItem;
	Gtk::ImageMenuItem* m_BrowserNewMenuItem;
	Gtk::ImageMenuItem* m_BrowserOpenMenuItem;
	Gtk::ImageMenuItem* m_BrowserOpenRecentMenuItem;
	Gtk::ImageMenuItem* m_BrowserOpenLocationMenuItem;
	Gtk::ImageMenuItem* m_BrowserImportMenuItem;
	Gtk::ImageMenuItem* m_BrowserSaveMenuItem;
	Gtk::ImageMenuItem* m_BrowserSaveAsMenuItem;
	Gtk::MenuItem* m_BrowserSaveACopyMenuItem;
	Gtk::ImageMenuItem* m_BrowserExportMenuItem;
	Gtk::ImageMenuItem* m_BrowserRevertMenuItem;
	Gtk::ImageMenuItem* m_BrowserRemoveUnusedPrototypesMenuItem;
	Gtk::ImageMenuItem* m_BrowserScenePropertiesMenuItem;
	Gtk::ImageMenuItem* m_BrowserCloseMenuItem;
	Gtk::ImageMenuItem* m_BrowserQuitMenuItem;
	Gtk::MenuItem* m_BrowserEditMenuItem;
	Gtk::ImageMenuItem* m_BrowserUndoMenuItem;
	Gtk::ImageMenuItem* m_BrowserRedoMenuItem;
	Gtk::ImageMenuItem* m_BrowserUndoHistoryMenuItem;
	Gtk::ImageMenuItem* m_BrowserCutMenuItem;
	Gtk::ImageMenuItem* m_BrowserCopyMenuItem;
	Gtk::ImageMenuItem* m_BrowserPasteMenuItem;
	Gtk::ImageMenuItem* m_BrowserDeleteMenuItem;
	Gtk::MenuItem* m_BrowserCloneMenuItem;
	Gtk::MenuItem* m_BrowserCreateCloneMenuItem;
	Gtk::MenuItem* m_BrowserUnlinkCloneMenuItem;
	Gtk::ImageMenuItem* m_BrowserGroupSelectedNodesMenuItem;
	Gtk::ImageMenuItem* m_BrowserUngroupMenuItem;
	Gtk::MenuItem* m_BrowserAddToGroupMenuItem;
	Gtk::MenuItem* m_BrowserDetachFromGroupMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentTransformMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentGroupMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentStaticGroupMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentSwitchMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentBillboardMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentCollisionMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentLODMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentViewpointGroupMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentAnchorMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentLayoutLayerMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentScreenGroupMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentLayoutGroupMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentGeoTransformMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentGeoLocationMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentCADPartMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentCADAssemblyMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentCADLayerMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentLayerSetMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentLayerMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentViewportMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentPickableGroupMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentCollidableShapeMenuItem;
	Gtk::MenuItem* m_BrowserCreateParentCollidableOffsetMenuItem;
	Gtk::MenuItem* m_BrowserViewMenuItem;
	Gtk::CheckMenuItem* m_BrowserMenubarMenuItem;
	Gtk::CheckMenuItem* m_BrowserToolbarMenuItem;
	Gtk::CheckMenuItem* m_BrowserSidebarMenuItem;
	Gtk::CheckMenuItem* m_BrowserFooterMenuItem;
	Gtk::CheckMenuItem* m_BrowserTabsMenuItem;
	Gtk::ImageMenuItem* m_BrowserMotionBlurMenuItem;
	Gtk::ImageMenuItem* m_BrowserPrimitiveQualityMenuItem;
	Gtk::CheckMenuItem* m_BrowserPrimitiveQualityHighMenuItem;
	Gtk::CheckMenuItem* m_BrowserPrimitiveQualityMediumMenuItem;
	Gtk::CheckMenuItem* m_BrowserPrimitiveQualityLowMenuItem;
	Gtk::ImageMenuItem* m_BrowserTextureQualityMenuItem;
	Gtk::CheckMenuItem* m_BrowserTextureQualityHighMenuItem;
	Gtk::CheckMenuItem* m_BrowserTextureQualityMediumMenuItem;
	Gtk::CheckMenuItem* m_BrowserTextureQualityLowMenuItem;
	Gtk::CheckMenuItem* m_BrowserRubberbandMenuItem;
	Gtk::CheckMenuItem* m_BrowserRenderingPropertiesMenuItem;
	Gtk::ImageMenuItem* m_BrowserFullScreenMenuItem;
	Gtk::ImageMenuItem* m_BrowserUnFullScreenMenuItem;
	Gtk::MenuItem* m_BrowserSelectionMenuItem;
	Gtk::ImageMenuItem* m_BrowserSelectAllMenuItem;
	Gtk::MenuItem* m_BrowserDeselectAllMenuItem;
	Gtk::MenuItem* m_BrowserHideSelectedObjectsMenuItem;
	Gtk::MenuItem* m_BrowserHideUnselectedObjectsMenuItem;
	Gtk::MenuItem* m_BrowserShowSelectedObjectsMenuItem;
	Gtk::MenuItem* m_BrowserShowAllObjectsMenuItem;
	Gtk::CheckMenuItem* m_BrowserSelectLowestMenuItem;
	Gtk::CheckMenuItem* m_BrowserFollowPrimarySelectionMenuItem;
	Gtk::CheckMenuItem* m_BrowserTransformToolModeMenuItem;
	Gtk::MenuItem* m_BrowserGeometryMenuItem;
	Gtk::ImageMenuItem* m_BrowserUnionMenuItem;
	Gtk::ImageMenuItem* m_BrowserDifferenceMenuItem;
	Gtk::ImageMenuItem* m_BrowserIntersectionMenuItem;
	Gtk::ImageMenuItem* m_BrowserExclusionMenuItem;
	Gtk::ImageMenuItem* m_BrowserCombineMenuItem;
	Gtk::ImageMenuItem* m_BrowserTransformToZeroMenuItem;
	Gtk::CheckMenuItem* m_BrowserFogCoordinateEditorMenuItem;
	Gtk::MenuItem* m_BrowserLayoutMenuItem;
	Gtk::ImageMenuItem* m_BrowserBrowserSizeMenuItem;
	Gtk::ImageMenuItem* m_BrowserBackgroundImageMenuItem;
	Gtk::CheckMenuItem* m_BrowserGridLayoutToolMenuItem;
	Gtk::CheckMenuItem* m_BrowserAngleLayoutToolMenuItem;
	Gtk::CheckMenuItem* m_BrowserAxonometricGridLayoutToolMenuItem;
	Gtk::ImageMenuItem* m_BrowserGridPropertiesMenuItem;
	Gtk::SeparatorMenuItem* m_BrowserSeparatorMenuItem18;
	Gtk::CheckMenuItem* m_BrowserActivateSnapTargetMenuItem;
	Gtk::CheckMenuItem* m_BrowserActivateSnapSourceMenuItem;
	Gtk::MenuItem* m_BrowserCenterSnapTargetInSelectionMenuItem;
	Gtk::MenuItem* m_BrowserMoveSelectionToSnapTargetMenuItem;
	Gtk::MenuItem* m_BrowserMoveSelectionCenterToSnapTargetMenuItem;
	Gtk::MenuItem* m_BrowserExternalToolsMenuItem;
	Gtk::ImageMenuItem* m_BrowserManageExternalToolsMenuItem;
	Gtk::MenuItem* m_BrowserScenesMenuItem;
	Gtk::Menu* m_BrowserScenesMenu;
	Gtk::MenuItem* m_BrowserHelpMenuItem;
	Gtk::CheckMenuItem* m_BrowserLogarithmicDepthBufferMenuItem;
	Gtk::ImageMenuItem* m_BrowserInfoMenuItem;
	Gtk::ApplicationWindow* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Box* m_HeaderBox;
	Gtk::Box* m_HeaderSubBox;
	Gtk::MenuBar* m_Menubar;
	Gtk::MenuItem* m_FileMenuItem;
	Gtk::ImageMenuItem* m_NewMenuItem;
	Gtk::ImageMenuItem* m_OpenMenuItem;
	Gtk::ImageMenuItem* m_OpenRecentMenuItem;
	Gtk::ImageMenuItem* m_OpenLocationMenuItem;
	Gtk::ImageMenuItem* m_ImportMenuItem;
	Gtk::ImageMenuItem* m_SaveMenuItem;
	Gtk::ImageMenuItem* m_SaveAsMenuItem;
	Gtk::MenuItem* m_SaveACopyMenuItem;
	Gtk::ImageMenuItem* m_ExportMenuItem;
	Gtk::ImageMenuItem* m_RevertMenuItem;
	Gtk::ImageMenuItem* m_RemoveUnusedPrototypesMenuItem;
	Gtk::ImageMenuItem* m_ScenePropertiesMenuItem;
	Gtk::ImageMenuItem* m_CloseMenuItem;
	Gtk::ImageMenuItem* m_QuitMenuItem;
	Gtk::MenuItem* m_EditMenuItem;
	Gtk::ImageMenuItem* m_UndoMenuItem;
	Gtk::ImageMenuItem* m_RedoMenuItem;
	Gtk::ImageMenuItem* m_UndoHistoryMenuItem;
	Gtk::ImageMenuItem* m_CutMenuItem;
	Gtk::ImageMenuItem* m_CopyMenuItem;
	Gtk::ImageMenuItem* m_PasteMenuItem;
	Gtk::ImageMenuItem* m_DeleteMenuItem;
	Gtk::MenuItem* m_CloneMenuItem;
	Gtk::MenuItem* m_CreateCloneMenuItem;
	Gtk::MenuItem* m_UnlinkCloneMenuItem;
	Gtk::ImageMenuItem* m_GroupSelectedNodesMenuItem;
	Gtk::ImageMenuItem* m_UngroupMenuItem;
	Gtk::MenuItem* m_AddToGroupMenuItem;
	Gtk::MenuItem* m_DetachFromGroupMenuItem;
	Gtk::MenuItem* m_CreateParentMenuItem;
	Gtk::MenuItem* m_CreateParentTransformMenuItem;
	Gtk::MenuItem* m_CreateParentGroupMenuItem;
	Gtk::MenuItem* m_CreateParentStaticGroupMenuItem;
	Gtk::MenuItem* m_CreateParentSwitchMenuItem;
	Gtk::MenuItem* m_CreateParentBillboardMenuItem;
	Gtk::MenuItem* m_CreateParentCollisionMenuItem;
	Gtk::MenuItem* m_CreateParentLODMenuItem;
	Gtk::MenuItem* m_CreateParentViewpointGroupMenuItem;
	Gtk::MenuItem* m_CreateParentAnchorMenuItem;
	Gtk::MenuItem* m_CreateParentLayoutLayerMenuItem;
	Gtk::MenuItem* m_CreateParentScreenGroupMenuItem;
	Gtk::MenuItem* m_CreateParentLayoutGroupMenuItem;
	Gtk::MenuItem* m_CreateParentGeoTransformMenuItem;
	Gtk::MenuItem* m_CreateParentGeoLocationMenuItem;
	Gtk::MenuItem* m_CreateParentCADPartMenuItem;
	Gtk::MenuItem* m_CreateParentCADAssemblyMenuItem;
	Gtk::MenuItem* m_CreateParentCADLayerMenuItem;
	Gtk::MenuItem* m_CreateParentLayerSetMenuItem;
	Gtk::MenuItem* m_CreateParentLayerMenuItem;
	Gtk::MenuItem* m_CreateParentViewportMenuItem;
	Gtk::MenuItem* m_CreateParentPickableGroupMenuItem;
	Gtk::MenuItem* m_CreateParentCollidableShapeMenuItem;
	Gtk::MenuItem* m_CreateParentCollidableOffsetMenuItem;
	Gtk::MenuItem* m_ViewMenuItem;
	Gtk::CheckMenuItem* m_MenubarMenuItem;
	Gtk::CheckMenuItem* m_ToolbarMenuItem;
	Gtk::CheckMenuItem* m_SidebarMenuItem;
	Gtk::CheckMenuItem* m_FooterMenuItem;
	Gtk::CheckMenuItem* m_TabsMenuItem;
	Gtk::ImageMenuItem* m_MotionBlurMenuItem;
	Gtk::ImageMenuItem* m_PrimitiveQualityMenuItem;
	Gtk::CheckMenuItem* m_PrimitiveQualityHighMenuItem;
	Gtk::CheckMenuItem* m_PrimitiveQualityMediumMenuItem;
	Gtk::CheckMenuItem* m_PrimitiveQualityLowMenuItem;
	Gtk::ImageMenuItem* m_TextureQualityMenuItem;
	Gtk::CheckMenuItem* m_TextureQualityHighMenuItem;
	Gtk::CheckMenuItem* m_TextureQualityMediumMenuItem;
	Gtk::CheckMenuItem* m_TextureQualityLowMenuItem;
	Gtk::CheckMenuItem* m_RubberbandMenuItem;
	Gtk::CheckMenuItem* m_RenderingPropertiesMenuItem;
	Gtk::ImageMenuItem* m_FullScreenMenuItem;
	Gtk::ImageMenuItem* m_UnFullScreenMenuItem;
	Gtk::MenuItem* m_SelectionMenuItem;
	Gtk::ImageMenuItem* m_SelectAllMenuItem;
	Gtk::MenuItem* m_DeselectAllMenuItem;
	Gtk::MenuItem* m_HideSelectedObjectsMenuItem;
	Gtk::MenuItem* m_HideUnselectedObjectsMenuItem;
	Gtk::MenuItem* m_ShowSelectedObjectsMenuItem;
	Gtk::MenuItem* m_ShowAllObjectsMenuItem;
	Gtk::CheckMenuItem* m_SelectLowestMenuItem;
	Gtk::CheckMenuItem* m_FollowPrimarySelectionMenuItem;
	Gtk::CheckMenuItem* m_TransformToolModeMenuItem;
	Gtk::MenuItem* m_GeometryMenuItem;
	Gtk::ImageMenuItem* m_UnionMenuItem;
	Gtk::ImageMenuItem* m_DifferenceMenuItem;
	Gtk::ImageMenuItem* m_IntersectionMenuItem;
	Gtk::ImageMenuItem* m_ExclusionMenuItem;
	Gtk::ImageMenuItem* m_CombineMenuItem;
	Gtk::ImageMenuItem* m_TransformToZeroMenuItem;
	Gtk::CheckMenuItem* m_FogCoordinateEditorMenuItem;
	Gtk::MenuItem* m_LayoutMenuItem;
	Gtk::ImageMenuItem* m_BrowserSizeMenuItem;
	Gtk::ImageMenuItem* m_BackgroundImageMenuItem;
	Gtk::CheckMenuItem* m_GridLayoutToolMenuItem;
	Gtk::CheckMenuItem* m_AngleLayoutToolMenuItem;
	Gtk::CheckMenuItem* m_AxonometricGridLayoutToolMenuItem;
	Gtk::ImageMenuItem* m_GridPropertiesMenuItem;
	Gtk::SeparatorMenuItem* m_SeparatorMenuItem38;
	Gtk::CheckMenuItem* m_ActivateSnapTargetMenuItem;
	Gtk::CheckMenuItem* m_ActivateSnapSourceMenuItem;
	Gtk::MenuItem* m_CenterSnapTargetInSelectionMenuItem;
	Gtk::MenuItem* m_MoveSelectionToSnapTargetMenuItem;
	Gtk::MenuItem* m_MoveSelectionCenterToSnapTargetMenuItem;
	Gtk::MenuItem* m_ExternalToolsMenuItem;
	Gtk::ImageMenuItem* m_ManageExternalToolsMenuItem;
	Gtk::MenuItem* m_ScenesMenuItem;
	Gtk::Menu* m_ScenesMenu;
	Gtk::MenuItem* m_HelpMenuItem;
	Gtk::CheckMenuItem* m_LogarithmicDepthBufferMenuItem;
	Gtk::ImageMenuItem* m_InfoMenuItem;
	Gtk::Box* m_Toolbar;
	Gtk::Toolbar* m_EditToolBar;
	Gtk::ToolButton* m_NewButton;
	Gtk::ToolButton* m_OpenButton;
	Gtk::ToolButton* m_ImportButton;
	Gtk::ToolButton* m_SaveButton;
	Gtk::ToolButton* m_RefreshButton;
	Gtk::SeparatorToolItem* m_SeparatorToolItem1;
	Gtk::ToolButton* m_UndoButton;
	Gtk::ToolButton* m_RedoButton;
	Gtk::SeparatorToolItem* m_SeparatorToolItem2;
	Gtk::ToolButton* m_NodeIndexButton;
	Gtk::ToolButton* m_GeometryEditorButton;
	Gtk::ToolButton* m_PolygonColoringEditorButton;
	Gtk::ToolButton* m_TextureMappingEditorButton;
	Gtk::ToolButton* m_PrototypeEditorButton;
	Gtk::ToolButton* m_NodeEditorButton;
	Gtk::Toolbar* m_PrimitivesToolBar;
	Gtk::ToolButton* m_ArcCloseButton;
	Gtk::ToolButton* m_DiskButton;
	Gtk::ToolButton* m_RectangleButton;
	Gtk::ToolButton* m_StarButton;
	Gtk::SeparatorToolItem* m_SeparatorToolItem3;
	Gtk::ToolButton* m_BoxButton;
	Gtk::ToolButton* m_ConeButton;
	Gtk::ToolButton* m_CylinderButton;
	Gtk::ToolButton* m_ElevationGridButton;
	Gtk::ToolButton* m_PyramidButton;
	Gtk::ToolButton* m_SphereButton;
	Gtk::ToolButton* m_TextButton;
	Gtk::Paned* m_VPaned;
	Gtk::Paned* m_HPaned;
	Gtk::Overlay* m_BrowserOverlay;
	Gtk::Notebook* m_BrowserNotebook;
	Gtk::Box* m_Dashboard;
	Gtk::Toolbar* m_DashboardToolBar;
	Gtk::Box* m_LogoBox;
	Gtk::RadioToolButton* m_HandButton;
	Gtk::RadioToolButton* m_ArrowButton;
	Gtk::ToolButton* m_PlayPauseButton;
	Gtk::SeparatorToolItem* m_SelectSeparator;
	Gtk::ToolButton* m_SelectParentButton;
	Gtk::ToolButton* m_SelectChildButton;
	Gtk::SeparatorToolItem* m_ViewerSeparator;
	Gtk::ToolButton* m_SelectViewerButton;
	Gtk::ToolButton* m_StraightenButton;
	Gtk::ToggleToolButton* m_StraightenHorizonButton;
	Gtk::SeparatorToolItem* m_LookAtSeparator;
	Gtk::ToolButton* m_LookAtSelectionButton;
	Gtk::ToolButton* m_LookAtAllButton;
	Gtk::RadioToolButton* m_LookAtButton;
	Gtk::Box* m_FooterBox;
	Gtk::Box* m_SidebarBox;
	Gtk::Popover* m_SelectViewerPopover;
	Gtk::RadioButton* m_ExamineViewerButton;
	Gtk::RadioButton* m_WalkViewerButton;
	Gtk::RadioButton* m_FlyViewerButton;
	Gtk::RadioButton* m_PlaneViewerButton;
	Gtk::RadioButton* m_NoneViewerButton;
	Gtk::RadioButton* m_OtherViewerButton;

};

} // puck
} // titania

#endif

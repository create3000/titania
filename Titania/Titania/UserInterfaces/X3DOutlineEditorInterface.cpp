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

#include "X3DOutlineEditorInterface.h"

namespace titania {
namespace puck {

void
X3DOutlineEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DOutlineEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DOutlineEditorInterface::create ()
{
	// Get objects.

	// Get widgets.
	m_builder -> get_widget ("DragActionMenu", m_DragActionMenu);
	m_builder -> get_widget ("DragMoveMenuItem", m_DragMoveMenuItem);
	m_builder -> get_widget ("DragCopyMenuItem", m_DragCopyMenuItem);
	m_builder -> get_widget ("DragLinkMenuItem", m_DragLinkMenuItem);
	m_builder -> get_widget ("DragCancelMenuItem", m_DragCancelMenuItem);
	m_builder -> get_widget ("SceneMenu", m_SceneMenu);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("Headerbar", m_Headerbar);
	m_builder -> get_widget ("SceneMenuBox", m_SceneMenuBox);
	m_builder -> get_widget ("SceneMenuButton", m_SceneMenuButton);
	m_builder -> get_widget ("SceneImage", m_SceneImage);
	m_builder -> get_widget ("SceneLabel", m_SceneLabel);
	m_builder -> get_widget ("PreviousSceneButton", m_PreviousSceneButton);
	m_builder -> get_widget ("NextSceneButton", m_NextSceneButton);
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_builder -> get_widget ("RenamePopover", m_RenamePopover);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("ContextMenu", m_ContextMenu);
	m_builder -> get_widget ("RenameMenuItem", m_RenameMenuItem);
	m_builder -> get_widget ("CommonSeparator", m_CommonSeparator);
	m_builder -> get_widget ("CutMenuItem", m_CutMenuItem);
	m_builder -> get_widget ("CopyMenuItem", m_CopyMenuItem);
	m_builder -> get_widget ("PasteMenuItem", m_PasteMenuItem);
	m_builder -> get_widget ("ClipboardSeparator", m_ClipboardSeparator);
	m_builder -> get_widget ("SetAsCurrentSceneMenuItem", m_SetAsCurrentSceneMenuItem);
	m_builder -> get_widget ("CreateInstanceMenuItem", m_CreateInstanceMenuItem);
	m_builder -> get_widget ("ReloadMenuItem", m_ReloadMenuItem);
	m_builder -> get_widget ("UpdateInstancesMenuItem", m_UpdateInstancesMenuItem);
	m_builder -> get_widget ("ProtoSeparator", m_ProtoSeparator);
	m_builder -> get_widget ("AddReferenceMenuItem", m_AddReferenceMenuItem);
	m_builder -> get_widget ("AddReferenceMenu", m_AddReferenceMenu);
	m_builder -> get_widget ("RemoveReferenceMenuItem", m_RemoveReferenceMenuItem);
	m_builder -> get_widget ("RemoveReferenceMenu", m_RemoveReferenceMenu);
	m_builder -> get_widget ("ReferencesSeparator", m_ReferencesSeparator);
	m_builder -> get_widget ("RemoveMenuItem", m_RemoveMenuItem);
	m_builder -> get_widget ("CreateCloneMenuItem", m_CreateCloneMenuItem);
	m_builder -> get_widget ("UnlinkCloneMenuItem", m_UnlinkCloneMenuItem);
	m_builder -> get_widget ("DetachFromGroupMenuItem", m_DetachFromGroupMenuItem);
	m_builder -> get_widget ("CreateParentGroupMenuItem", m_CreateParentGroupMenuItem);
	m_builder -> get_widget ("CreateParentTransformMenuItem", m_CreateParentTransformMenuItem);
	m_builder -> get_widget ("CreateParentGroupMenuItem1", m_CreateParentGroupMenuItem1);
	m_builder -> get_widget ("CreateParentStaticGroupMenuItem", m_CreateParentStaticGroupMenuItem);
	m_builder -> get_widget ("CreateParentSwitchMenuItem", m_CreateParentSwitchMenuItem);
	m_builder -> get_widget ("CreateParentBillboardMenuItem", m_CreateParentBillboardMenuItem);
	m_builder -> get_widget ("CreateParentCollisionMenuItem", m_CreateParentCollisionMenuItem);
	m_builder -> get_widget ("CreateParentLODMenuItem", m_CreateParentLODMenuItem);
	m_builder -> get_widget ("CreateParentViewpontGroupMenuItem", m_CreateParentViewpontGroupMenuItem);
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
	m_builder -> get_widget ("CreateParentPickableGroupMenuItem", m_CreateParentPickableGroupMenuItem);
	m_builder -> get_widget ("CreateParentCollidableShapeMenuItem", m_CreateParentCollidableShapeMenuItem);
	m_builder -> get_widget ("CreateParentCollidableOffsetMenuItem", m_CreateParentCollidableOffsetMenuItem);
	m_builder -> get_widget ("RemoveParentMenuItem", m_RemoveParentMenuItem);
	m_builder -> get_widget ("EditSeparator", m_EditSeparator);
	m_builder -> get_widget ("ViewMenuItem", m_ViewMenuItem);
	m_builder -> get_widget ("ShowExternProtosMenuItem", m_ShowExternProtosMenuItem);
	m_builder -> get_widget ("ShowPrototypesMenuItem", m_ShowPrototypesMenuItem);
	m_builder -> get_widget ("ShowImportedNodesMenuItem", m_ShowImportedNodesMenuItem);
	m_builder -> get_widget ("ShowExportedNodesMenuItem", m_ShowExportedNodesMenuItem);
	m_builder -> get_widget ("ExpandExternProtosMenuItem", m_ExpandExternProtosMenuItem);
	m_builder -> get_widget ("ExpandPrototypeInstancesMenuItem", m_ExpandPrototypeInstancesMenuItem);
	m_builder -> get_widget ("ExpandInlineNodesMenuItem", m_ExpandInlineNodesMenuItem);
	m_builder -> get_widget ("ColorizeTreeViewMenuItem", m_ColorizeTreeViewMenuItem);
	m_builder -> get_widget ("UseLocaleMenuItem", m_UseLocaleMenuItem);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_unmap));

	// Connect object Gtk::Button with id 'PreviousSceneButton'.
	m_PreviousSceneButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_previous_scene_clicked));
	m_NextSceneButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_next_scene_clicked));

	// Connect object Gtk::ScrolledWindow with id 'ScrolledWindow'.
	m_ScrolledWindow -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_button_press_event));

	// Connect object Gtk::ImageMenuItem with id 'RenameMenuItem'.
	m_RenameMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_rename_activate));
	m_CutMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_cut_activate));
	m_CopyMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_copy_activate));
	m_PasteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_paste_activate));
	m_SetAsCurrentSceneMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_set_as_current_scene_activate));
	m_CreateInstanceMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_instance_activate));
	m_ReloadMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_reload_activated));

	// Connect object Gtk::MenuItem with id 'UpdateInstancesMenuItem'.
	m_UpdateInstancesMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_update_instances_activated));

	// Connect object Gtk::ImageMenuItem with id 'RemoveMenuItem'.
	m_RemoveMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_remove_activate));

	// Connect object Gtk::MenuItem with id 'CreateCloneMenuItem'.
	m_CreateCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_clone_activate));
	m_UnlinkCloneMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_unlink_clone_activate));
	m_DetachFromGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_detach_from_group_activated));
	m_CreateParentTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_transform_activate));
	m_CreateParentGroupMenuItem1 -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_group_activate));
	m_CreateParentStaticGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_static_group_activate));
	m_CreateParentSwitchMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_switch_activate));
	m_CreateParentBillboardMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_billboard_activate));
	m_CreateParentCollisionMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_collision_activate));
	m_CreateParentLODMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_lod_activate));
	m_CreateParentViewpontGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_viewpoint_group_activated));
	m_CreateParentAnchorMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_anchor_activate));
	m_CreateParentLayoutLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_layout_layer_activate));
	m_CreateParentScreenGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_screen_group_activate));
	m_CreateParentLayoutGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_layout_group_activate));
	m_CreateParentGeoTransformMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_geo_transform_activate));
	m_CreateParentGeoLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_geo_location_activate));
	m_CreateParentCADPartMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_cad_part_activate));
	m_CreateParentCADAssemblyMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_cad_assembly_activate));
	m_CreateParentCADLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_cad_layer_activate));
	m_CreateParentLayerSetMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_layer_set_activate));
	m_CreateParentLayerMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_layer_activate));
	m_CreateParentViewportMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_viewport_activate));
	m_CreateParentPickableGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_pickable_group_activate));
	m_CreateParentCollidableShapeMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_collidable_shape_activate));
	m_CreateParentCollidableOffsetMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_create_parent_collidable_offset_activate));
	m_RemoveParentMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_remove_parent_activate));

	// Connect object Gtk::CheckMenuItem with id 'ShowExternProtosMenuItem'.
	m_ShowExternProtosMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_show_extern_protos_toggled));
	m_ShowPrototypesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_show_prototypes_toggled));
	m_ShowImportedNodesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_show_imported_nodes_toggled));
	m_ShowExportedNodesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_show_exported_nodes_toggled));
	m_ExpandExternProtosMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_expand_extern_protos_toggled));
	m_ExpandPrototypeInstancesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_expand_prototype_instances_toggled));
	m_ExpandInlineNodesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_expand_inline_nodes_toggled));
	m_ColorizeTreeViewMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_colorize_tree_view_menu_item_toggled));
	m_UseLocaleMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DOutlineEditorInterface::on_use_locale_menu_item_toggled));
}

X3DOutlineEditorInterface::~X3DOutlineEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania

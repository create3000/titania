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

#ifndef __TMP_GLAD2CPP_OUTLINE_EDITOR_H__
#define __TMP_GLAD2CPP_OUTLINE_EDITOR_H__

#include "../Base/X3DUserInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for OutlineEditor.
 */
class X3DOutlineEditorInterface :
	public X3DUserInterface
{
public:

	///  @name Construction

	X3DOutlineEditorInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DOutlineEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DOutlineEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::ListStore> &
	getAddNodeListStore () const
	{ return m_AddNodeListStore; }

	const Glib::RefPtr <Gtk::EntryCompletion> &
	getAddNodeCompletion () const
	{ return m_AddNodeCompletion; }

	Gtk::Menu &
	getDragActionMenu () const
	{ return *m_DragActionMenu; }

	Gtk::MenuItem &
	getDragMoveMenuItem () const
	{ return *m_DragMoveMenuItem; }

	Gtk::MenuItem &
	getDragCopyMenuItem () const
	{ return *m_DragCopyMenuItem; }

	Gtk::MenuItem &
	getDragLinkMenuItem () const
	{ return *m_DragLinkMenuItem; }

	Gtk::MenuItem &
	getDragCancelMenuItem () const
	{ return *m_DragCancelMenuItem; }

	Gtk::Menu &
	getSceneMenu () const
	{ return *m_SceneMenu; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderbar () const
	{ return *m_Headerbar; }

	Gtk::Box &
	getSceneMenuBox () const
	{ return *m_SceneMenuBox; }

	Gtk::MenuButton &
	getSceneMenuButton () const
	{ return *m_SceneMenuButton; }

	Gtk::Image &
	getSceneImage () const
	{ return *m_SceneImage; }

	Gtk::Label &
	getSceneLabel () const
	{ return *m_SceneLabel; }

	Gtk::Button &
	getPreviousSceneButton () const
	{ return *m_PreviousSceneButton; }

	Gtk::Button &
	getNextSceneButton () const
	{ return *m_NextSceneButton; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::Popover &
	getAddNodePopover () const
	{ return *m_AddNodePopover; }

	Gtk::Entry &
	getAddNodeEntry () const
	{ return *m_AddNodeEntry; }

	Gtk::Button &
	getAddNodeButton () const
	{ return *m_AddNodeButton; }

	Gtk::Popover &
	getRenamePopover () const
	{ return *m_RenamePopover; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Menu &
	getContextMenu () const
	{ return *m_ContextMenu; }

	Gtk::ImageMenuItem &
	getAddNodeMenuItem () const
	{ return *m_AddNodeMenuItem; }

	Gtk::ImageMenuItem &
	getRenameMenuItem () const
	{ return *m_RenameMenuItem; }

	Gtk::SeparatorMenuItem &
	getCommonSeparator () const
	{ return *m_CommonSeparator; }

	Gtk::ImageMenuItem &
	getCutMenuItem () const
	{ return *m_CutMenuItem; }

	Gtk::ImageMenuItem &
	getCopyMenuItem () const
	{ return *m_CopyMenuItem; }

	Gtk::ImageMenuItem &
	getPasteMenuItem () const
	{ return *m_PasteMenuItem; }

	Gtk::SeparatorMenuItem &
	getClipboardSeparator () const
	{ return *m_ClipboardSeparator; }

	Gtk::ImageMenuItem &
	getSetAsCurrentSceneMenuItem () const
	{ return *m_SetAsCurrentSceneMenuItem; }

	Gtk::ImageMenuItem &
	getCreateInstanceMenuItem () const
	{ return *m_CreateInstanceMenuItem; }

	Gtk::ImageMenuItem &
	getReloadMenuItem () const
	{ return *m_ReloadMenuItem; }

	Gtk::MenuItem &
	getUpdateInstancesMenuItem () const
	{ return *m_UpdateInstancesMenuItem; }

	Gtk::SeparatorMenuItem &
	getProtoSeparator () const
	{ return *m_ProtoSeparator; }

	Gtk::MenuItem &
	getAddReferenceMenuItem () const
	{ return *m_AddReferenceMenuItem; }

	Gtk::Menu &
	getAddReferenceMenu () const
	{ return *m_AddReferenceMenu; }

	Gtk::MenuItem &
	getRemoveReferenceMenuItem () const
	{ return *m_RemoveReferenceMenuItem; }

	Gtk::Menu &
	getRemoveReferenceMenu () const
	{ return *m_RemoveReferenceMenu; }

	Gtk::SeparatorMenuItem &
	getReferencesSeparator () const
	{ return *m_ReferencesSeparator; }

	Gtk::ImageMenuItem &
	getRemoveMenuItem () const
	{ return *m_RemoveMenuItem; }

	Gtk::MenuItem &
	getCreateCloneMenuItem () const
	{ return *m_CreateCloneMenuItem; }

	Gtk::MenuItem &
	getUnlinkCloneMenuItem () const
	{ return *m_UnlinkCloneMenuItem; }

	Gtk::MenuItem &
	getDetachFromGroupMenuItem () const
	{ return *m_DetachFromGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentGroupMenuItem () const
	{ return *m_CreateParentGroupMenuItem; }

	Gtk::MenuItem &
	getCreateParentTransformMenuItem () const
	{ return *m_CreateParentTransformMenuItem; }

	Gtk::MenuItem &
	getCreateParentGroupMenuItem1 () const
	{ return *m_CreateParentGroupMenuItem1; }

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
	getCreateParentViewpontGroupMenuItem () const
	{ return *m_CreateParentViewpontGroupMenuItem; }

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
	getRemoveParentMenuItem () const
	{ return *m_RemoveParentMenuItem; }

	Gtk::SeparatorMenuItem &
	getEditSeparator () const
	{ return *m_EditSeparator; }

	Gtk::MenuItem &
	getViewMenuItem () const
	{ return *m_ViewMenuItem; }

	Gtk::CheckMenuItem &
	getShowExternProtosMenuItem () const
	{ return *m_ShowExternProtosMenuItem; }

	Gtk::CheckMenuItem &
	getShowPrototypesMenuItem () const
	{ return *m_ShowPrototypesMenuItem; }

	Gtk::CheckMenuItem &
	getShowImportedNodesMenuItem () const
	{ return *m_ShowImportedNodesMenuItem; }

	Gtk::CheckMenuItem &
	getShowExportedNodesMenuItem () const
	{ return *m_ShowExportedNodesMenuItem; }

	Gtk::CheckMenuItem &
	getExpandExternProtosMenuItem () const
	{ return *m_ExpandExternProtosMenuItem; }

	Gtk::CheckMenuItem &
	getExpandPrototypeInstancesMenuItem () const
	{ return *m_ExpandPrototypeInstancesMenuItem; }

	Gtk::CheckMenuItem &
	getExpandInlineNodesMenuItem () const
	{ return *m_ExpandInlineNodesMenuItem; }

	Gtk::CheckMenuItem &
	getColorizeTreeViewMenuItem () const
	{ return *m_ColorizeTreeViewMenuItem; }

	Gtk::CheckMenuItem &
	getUseLocaleMenuItem () const
	{ return *m_UseLocaleMenuItem; }

	///  @name Signal handlers

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_previous_scene_clicked () = 0;

	virtual
	void
	on_next_scene_clicked () = 0;

	virtual
	bool
	on_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	bool
	on_add_node_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_add_node_clicked () = 0;

	virtual
	void
	on_add_node_activate () = 0;

	virtual
	void
	on_rename_activate () = 0;

	virtual
	void
	on_cut_activate () = 0;

	virtual
	void
	on_copy_activate () = 0;

	virtual
	void
	on_paste_activate () = 0;

	virtual
	void
	on_set_as_current_scene_activate () = 0;

	virtual
	void
	on_create_instance_activate () = 0;

	virtual
	void
	on_reload_activated () = 0;

	virtual
	void
	on_update_instances_activated () = 0;

	virtual
	void
	on_remove_activate () = 0;

	virtual
	void
	on_create_clone_activate () = 0;

	virtual
	void
	on_unlink_clone_activate () = 0;

	virtual
	void
	on_detach_from_group_activated () = 0;

	virtual
	void
	on_create_parent_transform_activate () = 0;

	virtual
	void
	on_create_parent_group_activate () = 0;

	virtual
	void
	on_create_parent_static_group_activate () = 0;

	virtual
	void
	on_create_parent_switch_activate () = 0;

	virtual
	void
	on_create_parent_billboard_activate () = 0;

	virtual
	void
	on_create_parent_collision_activate () = 0;

	virtual
	void
	on_create_parent_lod_activate () = 0;

	virtual
	void
	on_create_parent_viewpoint_group_activated () = 0;

	virtual
	void
	on_create_parent_anchor_activate () = 0;

	virtual
	void
	on_create_parent_layout_layer_activate () = 0;

	virtual
	void
	on_create_parent_screen_group_activate () = 0;

	virtual
	void
	on_create_parent_layout_group_activate () = 0;

	virtual
	void
	on_create_parent_geo_transform_activate () = 0;

	virtual
	void
	on_create_parent_geo_location_activate () = 0;

	virtual
	void
	on_create_parent_cad_part_activate () = 0;

	virtual
	void
	on_create_parent_cad_assembly_activate () = 0;

	virtual
	void
	on_create_parent_cad_layer_activate () = 0;

	virtual
	void
	on_create_parent_layer_set_activate () = 0;

	virtual
	void
	on_create_parent_layer_activate () = 0;

	virtual
	void
	on_create_parent_viewport_activate () = 0;

	virtual
	void
	on_create_parent_pickable_group_activate () = 0;

	virtual
	void
	on_create_parent_collidable_shape_activate () = 0;

	virtual
	void
	on_create_parent_collidable_offset_activate () = 0;

	virtual
	void
	on_remove_parent_activate () = 0;

	virtual
	void
	on_show_extern_protos_toggled () = 0;

	virtual
	void
	on_show_prototypes_toggled () = 0;

	virtual
	void
	on_show_imported_nodes_toggled () = 0;

	virtual
	void
	on_show_exported_nodes_toggled () = 0;

	virtual
	void
	on_expand_extern_protos_toggled () = 0;

	virtual
	void
	on_expand_prototype_instances_toggled () = 0;

	virtual
	void
	on_expand_inline_nodes_toggled () = 0;

	virtual
	void
	on_colorize_tree_view_menu_item_toggled () = 0;

	virtual
	void
	on_use_locale_menu_item_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DOutlineEditorInterface () override;


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
	Glib::RefPtr <Gtk::ListStore> m_AddNodeListStore;
	Glib::RefPtr <Gtk::EntryCompletion> m_AddNodeCompletion;
	Gtk::Menu* m_DragActionMenu;
	Gtk::MenuItem* m_DragMoveMenuItem;
	Gtk::MenuItem* m_DragCopyMenuItem;
	Gtk::MenuItem* m_DragLinkMenuItem;
	Gtk::MenuItem* m_DragCancelMenuItem;
	Gtk::Menu* m_SceneMenu;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_Headerbar;
	Gtk::Box* m_SceneMenuBox;
	Gtk::MenuButton* m_SceneMenuButton;
	Gtk::Image* m_SceneImage;
	Gtk::Label* m_SceneLabel;
	Gtk::Button* m_PreviousSceneButton;
	Gtk::Button* m_NextSceneButton;
	Gtk::ScrolledWindow* m_ScrolledWindow;
	Gtk::Popover* m_AddNodePopover;
	Gtk::Entry* m_AddNodeEntry;
	Gtk::Button* m_AddNodeButton;
	Gtk::Popover* m_RenamePopover;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;
	Gtk::Menu* m_ContextMenu;
	Gtk::ImageMenuItem* m_AddNodeMenuItem;
	Gtk::ImageMenuItem* m_RenameMenuItem;
	Gtk::SeparatorMenuItem* m_CommonSeparator;
	Gtk::ImageMenuItem* m_CutMenuItem;
	Gtk::ImageMenuItem* m_CopyMenuItem;
	Gtk::ImageMenuItem* m_PasteMenuItem;
	Gtk::SeparatorMenuItem* m_ClipboardSeparator;
	Gtk::ImageMenuItem* m_SetAsCurrentSceneMenuItem;
	Gtk::ImageMenuItem* m_CreateInstanceMenuItem;
	Gtk::ImageMenuItem* m_ReloadMenuItem;
	Gtk::MenuItem* m_UpdateInstancesMenuItem;
	Gtk::SeparatorMenuItem* m_ProtoSeparator;
	Gtk::MenuItem* m_AddReferenceMenuItem;
	Gtk::Menu* m_AddReferenceMenu;
	Gtk::MenuItem* m_RemoveReferenceMenuItem;
	Gtk::Menu* m_RemoveReferenceMenu;
	Gtk::SeparatorMenuItem* m_ReferencesSeparator;
	Gtk::ImageMenuItem* m_RemoveMenuItem;
	Gtk::MenuItem* m_CreateCloneMenuItem;
	Gtk::MenuItem* m_UnlinkCloneMenuItem;
	Gtk::MenuItem* m_DetachFromGroupMenuItem;
	Gtk::MenuItem* m_CreateParentGroupMenuItem;
	Gtk::MenuItem* m_CreateParentTransformMenuItem;
	Gtk::MenuItem* m_CreateParentGroupMenuItem1;
	Gtk::MenuItem* m_CreateParentStaticGroupMenuItem;
	Gtk::MenuItem* m_CreateParentSwitchMenuItem;
	Gtk::MenuItem* m_CreateParentBillboardMenuItem;
	Gtk::MenuItem* m_CreateParentCollisionMenuItem;
	Gtk::MenuItem* m_CreateParentLODMenuItem;
	Gtk::MenuItem* m_CreateParentViewpontGroupMenuItem;
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
	Gtk::MenuItem* m_RemoveParentMenuItem;
	Gtk::SeparatorMenuItem* m_EditSeparator;
	Gtk::MenuItem* m_ViewMenuItem;
	Gtk::CheckMenuItem* m_ShowExternProtosMenuItem;
	Gtk::CheckMenuItem* m_ShowPrototypesMenuItem;
	Gtk::CheckMenuItem* m_ShowImportedNodesMenuItem;
	Gtk::CheckMenuItem* m_ShowExportedNodesMenuItem;
	Gtk::CheckMenuItem* m_ExpandExternProtosMenuItem;
	Gtk::CheckMenuItem* m_ExpandPrototypeInstancesMenuItem;
	Gtk::CheckMenuItem* m_ExpandInlineNodesMenuItem;
	Gtk::CheckMenuItem* m_ColorizeTreeViewMenuItem;
	Gtk::CheckMenuItem* m_UseLocaleMenuItem;

};

} // puck
} // titania

#endif

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

#ifndef __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_EDITOR_H__
#define __TITANIA_WIDGETS_OUTLINE_EDITOR_OUTLINE_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DOutlineEditorInterface.h"

namespace titania {
namespace puck {

class OutlineTreeViewEditor;

class OutlineEditor :
	public X3DOutlineEditorInterface
{
public:

	///  @name Construction

	OutlineEditor (X3DBrowserWindow* const browserWindow);

	///  @name Member access

	const std::shared_ptr <OutlineTreeViewEditor> &
	getTreeView () const
	{ return treeView; }

	///  @name Destruction

	virtual
	~OutlineEditor () final override;


private:

	///  @name Construction

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_scene ();

	void
	set_executionContext ();

	// Pointing Device

	virtual
	bool
	on_button_press_event (GdkEventButton*) final override;

	// Edit Menu Item

	// Set current scene

	virtual
	void
	on_set_as_current_scene_activate () final override;

	void
	on_scene_activate (Gtk::RadioMenuItem* const, const size_t);

	virtual
	void
	on_previous_scene_clicked () final override;

	virtual
	void
	on_next_scene_clicked () final override;

	void
	set_scenes_menu ();

	std::pair <Gtk::RadioMenuItem*, size_t>
	addSceneMenuItem (const X3D::X3DExecutionContextPtr &);

	std::string
	getSceneLabelText (const X3D::X3DExecutionContextPtr & scene) const;

	std::string
	getSceneMenuLabelText (const X3D::X3DExecutionContextPtr & scene, const bool) const;

	// Common

	virtual
	void
	on_rename_activate () final override;

	void
	set_name ();

	// Clipboard

	virtual
	void
	on_cut_activate () final override;

	virtual
	void
	on_copy_activate () final override;

	virtual
	void
	on_paste_activate () final override;

	// Proto and externproto

	virtual
	void
	on_update_instances_activated () final override;

	virtual
	void
	on_reload_activated () final override;

	virtual
	void
	on_create_instance_activate () final override;

	// Field references

	void
	on_add_reference_activate (const X3D::SFNode & node, const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr);

	void
	on_remove_reference_activate (const X3D::SFNode & node, const X3D::FieldPtr & fieldPtr, const X3D::FieldPtr & referencePtr);

	// Edit

	virtual
	void
	on_remove_activate () final override;

	void
	remove (const X3D::UndoStepPtr & undoStep);

	virtual
	void
	on_create_clone_activate () final override;

	virtual
	void
	on_unlink_clone_activate () final override;

	virtual
	void
	on_detach_from_group_activated () final override;

	virtual
	void
	on_create_parent_transform_activate () final override;

	virtual
	void
	on_create_parent_group_activate () final override;

	virtual
	void
	on_create_parent_static_group_activate () final override;

	virtual
	void
	on_create_parent_switch_activate () final override;

	virtual
	void
	on_create_parent_billboard_activate () final override;

	virtual
	void
	on_create_parent_collision_activate () final override;

	virtual
	void
	on_create_parent_lod_activate () final override;

	virtual
	void
	on_create_parent_viewpoint_group_activated () final override;

	virtual
	void
	on_create_parent_anchor_activate () final override;

	virtual
	void
	on_create_parent_layout_layer_activate () final override;

	virtual
	void
	on_create_parent_screen_group_activate () final override;

	virtual
	void
	on_create_parent_layout_group_activate () final override;

	virtual
	void
	on_create_parent_geo_transform_activate () final override;

	virtual
	void
	on_create_parent_geo_location_activate () final override;

	virtual
	void
	on_create_parent_cad_part_activate () final override;

	virtual
	void
	on_create_parent_cad_assembly_activate () final override;

	virtual
	void
	on_create_parent_cad_layer_activate () final override;

	virtual
	void
	on_create_parent_layer_set_activate () final override;

	virtual
	void
	on_create_parent_layer_activate () final override;

	virtual
	void
	on_create_parent_viewport_activate () final override;

	virtual
	void
	on_create_parent_pickable_group_activate () final override;

	virtual
	void
	on_create_parent_collidable_shape_activate () final override;

	virtual
	void
	on_create_parent_collidable_offset_activate () final override;

	X3D::SFNode
	on_create_parent (const std::string &, const std::string & = "children", const X3D::FieldType fieldType = X3D::X3DConstants::MFNode);

	virtual
	void
	on_remove_parent_activate () final override;

	void
	applyTransformation (const X3D::SFNode & node, const X3D::SFNode & parent, const X3D::UndoStepPtr & undoStep);

	// View Menu Item

	virtual
	void
	on_show_extern_protos_toggled () final override;

	virtual
	void
	on_show_prototypes_toggled () final override;

	virtual
	void
	on_show_imported_nodes_toggled () final override;

	virtual
	void
	on_show_exported_nodes_toggled () final override;

	virtual
	void
	on_expand_extern_protos_toggled () final override;

	virtual
	void
	on_expand_prototype_instances_toggled () final override;

	virtual
	void
	on_expand_inline_nodes_toggled () final override;

	virtual
	void
	on_colorize_tree_view_menu_item_toggled () final override;

	virtual
	void
	on_use_locale_menu_item_toggled () final override;

	///  @name Operations

	X3D::X3DPtr <X3D::X3DLayerNode>
	getLayer (Gtk::TreeIter iter) const;

	void
	selectNode (const double, const double);

	void
	selectField (const double, const double);

	Gtk::TreePath
	getNodeAtPosition (const double, const double);

	Gtk::TreePath
	getFieldAtPosition (const double, const double);

	Gtk::TreePath
	getPathAtPosition (const double, const double);

	X3D::X3DExecutionContextPtr
	getParentContext (Gtk::TreePath parentPath) const;

	// Expanded

	void
	restoreExpanded ();

	void
	saveExpanded ();

	void
	saveExpandedImpl ();

	void
	getExpanded (const Gtk::TreeModel::Children &, std::deque <std::string> &) const;

	///  @name Members

	using MenuItemPair = std::pair <X3D::X3DExecutionContextPtr, Gtk::RadioMenuItem*>;

	std::shared_ptr <OutlineTreeViewEditor>        treeView;
	Gtk::RadioButtonGroup                          sceneGroup;
	std::map <X3D::X3DExecutionContextPtr, size_t> sceneIndex;
	std::deque <MenuItemPair>                      scenes;

	NameEntry     nodeName;
	Gtk::TreePath nodePath;
	Gtk::TreePath fieldPath;
	bool          realized;

};

} // puck
} // titania

#endif

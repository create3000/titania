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

#ifndef __TITANIA_BROWSER_BROWSER_WINDOW_H__
#define __TITANIA_BROWSER_BROWSER_WINDOW_H__

#include "../Browser/X3DBrowserEditor.h"

namespace titania {
namespace puck {

class HistoryView;
class LibraryView;
class MotionBlurEditor;
class OutlineEditor;
class OutlineTreeViewEditor;
class ViewpointList;
class Console;
class ScriptEditor;

class BrowserWindow :
	public X3DBrowserEditor
{
public:

	/// @name Construction

	BrowserWindow (const X3D::BrowserPtr &, int, char**);

	/// @name Widgets

	const std::shared_ptr <OutlineTreeViewEditor> &
	getOutlineTreeView () const;

	/// @name Member access

	X3D::Keys &
	getKeys ()
	{ return keys; }

	const X3D::Keys &
	getKeys () const
	{ return keys; }

	const X3D::Output &
	saveAs () const
	{ return saveAsOutput; }

	/// @name Menu

	void
	setAccelerators (const bool);

	void
	expandNodes (const X3D::MFNode &);

	/// @name Destruction

	virtual
	~BrowserWindow ();


private:

	/// @name Construction

	virtual
	void
	initialize ();

	void
	loadStyles () const;

	void
	expandNodesImpl (const X3D::MFNode &);

	///  @name Scene handling

	void
	set_scene ();

	void
	set_executionContext ();

	///  @name Selection handling

	void
	set_touchTime ();

	void
	set_selection (const X3D::MFNode & children);

	///  @name Key events

	virtual
	bool
	on_focus_out_event (GdkEventFocus* event) final override;

	virtual
	bool
	on_key_press_event (GdkEventKey*) final override;

	virtual
	bool
	on_key_release_event (GdkEventKey*);

	virtual
	bool
	on_menubar_button_press_event (GdkEventButton*) final override;

	/// @name File menu

	virtual
	void
	on_new () final override;

	virtual
	void
	on_open () final override;

	virtual
	void
	on_open_location () final override;

	virtual
	void
	on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint info, guint) final override;

	virtual
	void
	on_import () final override;

	virtual
	void
	on_import_as_inline_toggled () final override;

	virtual
	void
	on_surface_box_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint info, guint) final override;

	void
	dragDataHandling (const Glib::RefPtr <Gdk::DragContext> &, const Gtk::SelectionData &, const guint, const bool);

	virtual
	void
	on_save () final override;

	virtual
	void
	on_save_as () final override;

	virtual
	void
	on_revert_to_saved () final override;

	virtual
	void
	on_remove_unused_prototypes () final override;

	virtual
	void
	on_close () final override;

	/// @name Undo/Redo

	virtual
	void
	on_undo () final override;

	virtual
	void
	on_redo () final override;

	///  @name Clipboard handling

	void
	on_clipboard_owner_change (GdkEventOwnerChange*);

	virtual
	void
	on_cut_nodes_activate () final override;

	virtual
	void
	on_copy_nodes_activate () final override;

	virtual
	void
	on_paste_nodes_activate () final override;

	///  @name Edit menu

	virtual
	void
	on_delete_nodes_activate () final override;

	virtual
	void
	on_create_clone_activate () final override;

	virtual
	void
	on_unlink_clone_activate () final override;

	virtual
	void
	on_group_selected_nodes_activate () final override;

	virtual
	void
	on_ungroup_node_activate () final override;

	virtual
	void
	on_add_to_group_activate () final override;

	virtual
	void
	on_detach_from_group_activate () final override;

	virtual
	void
	on_create_parent_transform_activate () final override;

	virtual
	void
	on_create_parent_group_activate () final override;

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
	on_create_parent_anchor_activate () final override;

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

	void
	on_create_parent (const std::string &);

	/// @name Bar view handling

	virtual
	void
	on_toolBar_toggled () final override;

	virtual
	void
	on_sideBar_toggled () final override;

	virtual
	void
	on_footer_toggled () final override;

	virtual
	void
	on_browser_toggled () final override;

	virtual
	void
	on_editor_toggled () final override;

	void
	enableEditor (const bool);

	/// @name Shading

	virtual
	void
	on_phong_activate () final override;

	virtual
	void
	on_gouraud_activate () final override;

	virtual
	void
	on_flat_activate () final override;

	virtual
	void
	on_wireframe_activate () final override;

	virtual
	void
	on_pointset_activate () final override;

	void
	on_shading_activate (const std::string &);

	void
	set_shading (const X3D::SFString &);

	void
	connectShading (const X3D::SFString &);

	///  @name Rendering quality

	virtual
	void
	on_high_quality_activate () final override;

	virtual
	void
	on_medium_quality_activate () final override;

	virtual
	void
	on_low_quality_activate () final override;

	void
	on_primitiveQuality_activate (const std::string &);

	void
	set_primitiveQuality (const X3D::SFString &);

	void
	connectPrimitiveQuality (const X3D::SFString &);

	///  @name Object Icons

	virtual
	void
	on_fogs_toggled () final override;

	virtual
	void
	on_backgrounds_toggled () final override;

	virtual
	void
	on_lights_toggled () final override;

	virtual
	void
	on_proximity_sensors_toggled () final override;

	virtual
	void
	on_visibility_sensors_toggled () final override;

	virtual
	void
	on_viewpoints_toggled () final override;

	virtual
	void
	on_hide_all_object_icons_activate () final override;

	virtual
	void
	on_rubberband_toggled () final override;

	///  @name Rendering properties

	virtual
	void
	on_rendering_properties_toggled () final override;

	///  @name Fullscreen modus

	virtual
	void
	on_fullscreen () final override;

	virtual
	void
	on_unfullscreen () final override;

	///  @name Selection

	virtual
	void
	on_select_all_activate () final override;

	virtual
	void
	on_deselect_all_activate () final override;

	virtual
	void
	on_hide_selected_objects_activate () final override;

	virtual
	void
	on_hide_unselected_objects_activate () final override;

	virtual
	void
	on_show_selected_objects_activate () final override;

	virtual
	void
	on_show_all_objects_activate () final override;

	virtual
	void
	on_select_lowest_toggled () final override;

	virtual
	void
	on_follow_primary_selection_toggled () final override;

	///  @name Layout

	virtual
	void
	on_grid_tool_activate () final override;

	///  @name Help menu

	virtual
	void
	on_info () final override;

	virtual
	void
	on_standard_size () final override;

	/// Toolbar

	virtual
	void
	on_node_properties_editor_clicked () final override;

	virtual
	void
	on_appearance_editor_clicked () final override;

	virtual
	void
	on_texture_editor_clicked () final override;

	virtual
	void
	on_text_editor_clicked () final override;

	virtual
	void
	on_geometry_properties_editor_clicked () final override;

	virtual
	void
	on_viewpoint_editor_clicked () final override;

	virtual
	void
	on_light_editor_clicked () final override;

	virtual
	void
	on_lod_editor_clicked () final override;

	virtual
	void
	on_inline_editor_clicked () final override;

	virtual
	void
	on_prototype_instance_dialog_clicked () final override;

	virtual
	void
	on_node_index_clicked () final override;

	//virtual
	//void
	//on_motion_blur_editor_clicked () final override;

	///  @name Browser dashboard handling

	void
	set_dashboard (const bool);

	virtual
	void
	on_hand_button_toggled () final override;

	virtual
	void
	on_arrow_button_toggled () final override;

	virtual
	void
	on_play_pause_button_clicked () final override;

	virtual
	void
	on_select_parent_button_clicked () final override;

	virtual
	void
	on_select_children_button_clicked () final override;

	X3D::MFNode
	getChildren (const X3D::SFNode &, const bool = false) const;

	X3D::MFNode
	getChildrenInProtoinstance (const X3D::SFNode &, const std::set <X3D::SFNode> &, std::set <X3D::SFNode> &) const;

	void
	set_viewer (X3D::ViewerType);

	void
	set_available_viewers (const X3D::MFEnum <X3D::ViewerType> &);

	virtual
	void
	on_viewer_clicked () final override;

	virtual
	void
	on_examine_viewer_activate () final override;

	virtual
	void
	on_walk_viewer_activate () final override;

	virtual
	void
	on_fly_viewer_activate () final override;

	virtual
	void
	on_plane_viewer_activate () final override;

	virtual
	void
	on_none_viewer_activate () final override;

	virtual
	void
	on_straighten_clicked () final override;

	virtual
	void
	on_look_at_selection_clicked () final override;

	virtual
	void
	on_look_at_all_clicked () final override;

	virtual
	void
	on_look_at_toggled () final override;

	///  @name Members

	std::unique_ptr <ViewpointList> viewpointList;
	std::unique_ptr <HistoryView>   historyEditor;
	std::unique_ptr <LibraryView>   libraryView;
	std::unique_ptr <OutlineEditor> outlineEditor;
	std::unique_ptr <Console>       console;
	std::unique_ptr <ScriptEditor>  scriptEditor;

	X3D::Keys keys;
	bool      shortcuts;
	bool      toggle;
	bool      changing;

	X3D::ViewerType viewer;

	X3D::Output saveAsOutput;

};

} // puck
} // titania

#endif

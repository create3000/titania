/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BROWSER_BROWSER_WINDOW_H__
#define __TITANIA_BROWSER_BROWSER_WINDOW_H__

#include "../Browser/X3DBrowserWindow.h"

namespace titania {
namespace puck {

class BrowserWindow :
	public X3DBrowserWindow
{
public:

	/// @name Member types

	using BooleanOperation = std::function <bool (const X3D::X3DExecutionContextPtr &, const X3D::X3DPtrArray <X3D::X3DShapeNode> &, const X3D::UndoStepPtr &)>;

	/// @name Construction

	BrowserWindow (const X3D::BrowserPtr & masterBrowser);

	/// @name Destruction

	virtual
	~BrowserWindow () final override;


private:

	/// @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	///  @name Scene handling

	virtual
	void
	setPage (const NotebookPagePtr & value) final override;

	void
	set_masterBrowser ();

	void
	set_activeLayer ();

	void
	set_executionContext ();

	///  @name Selection handling

	void
	set_touchTime ();

	virtual
	void
	set_selection (const X3D::MFNode & children) final override;

	///  @name Key events

	virtual
	void
	on_style_updated () final override;

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

	virtual
	bool
	on_notebook_button_press_event (GdkEventButton*) final override;

	/// @name File menu

	virtual
	void
	on_new_activated () final override;

	virtual
	void
	on_open_activated () final override;

	virtual
	void
	on_open_recent_activated () final override;

	virtual
	void
	on_open_location_activated () final override;

	virtual
	void
	on_toolbar_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint info, guint) final override;

	virtual
	void
	on_import_activated () final override;

	virtual
	void
	on_browser_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &, int, int, const Gtk::SelectionData &, guint info, guint) final override;

	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> &, const Gtk::SelectionData &, const guint, const bool);

	virtual
	void
	on_save_activated () final override;

	virtual
	void
	on_save_as_activated () final override;

	virtual
	void
	on_save_a_copy_activated () final override;

	virtual
	void
	on_export_activated () final override;

	virtual
	void
	on_revert_to_saved_activated () final override;

	virtual
	void
	on_remove_unused_prototypes_activated () final override;

	virtual
	void
	on_scene_properties_activated () final override;

	virtual
	void
	on_close_activated () final override;

	virtual
	void
	on_quit_activated () final override;

	/// @name Undo/Redo

	virtual
	void
	on_undo_activated () final override;

	virtual
	void
	on_redo_activated () final override;

	virtual
	void
	on_undo_history_activated () final override;

	///  @name Clipboard handling

	virtual
	void
	on_cut_activated () final override;

	virtual
	void
	on_copy_activated () final override;

	virtual
	void
	on_paste_activated () final override;

	///  @name Edit menu

	virtual
	void
	on_delete_activated () final override;

	virtual
	void
	on_create_clone_activated () final override;

	virtual
	void
	on_unlink_clone_activated () final override;

	virtual
	void
	on_group_selected_nodes_activated () final override;

	virtual
	void
	on_ungroup_activated () final override;

	virtual
	void
	on_add_to_group_activated () final override;

	virtual
	void
	on_detach_from_group_activated () final override;

	virtual
	void
	on_create_parent_transform_activated () final override;

	virtual
	void
	on_create_parent_group_activated () final override;

	virtual
	void
	on_create_parent_static_group_activated () final override;

	virtual
	void
	on_create_parent_switch_activated () final override;

	virtual
	void
	on_create_parent_billboard_activated () final override;

	virtual
	void
	on_create_parent_collision_activated () final override;

	virtual
	void
	on_create_parent_lod_activated () final override;

	virtual
	void
	on_create_parent_viewpoint_group_activated () final override;

	virtual
	void
	on_create_parent_anchor_activated () final override;

	virtual
	void
	on_create_parent_layout_layer_activated () final override;

	virtual
	void
	on_create_parent_screen_group_activated () final override;

	virtual
	void
	on_create_parent_layout_group_activated () final override;

	virtual
	void
	on_create_parent_geo_transform_activated () final override;

	virtual
	void
	on_create_parent_geo_location_activated () final override;

	virtual
	void
	on_create_parent_cad_part_activated () final override;

	virtual
	void
	on_create_parent_cad_assembly_activated () final override;

	virtual
	void
	on_create_parent_cad_layer_activated () final override;

	virtual
	void
	on_create_parent_layer_set_activated () final override;

	virtual
	void
	on_create_parent_layer_activated () final override;

	virtual
	void
	on_create_parent_viewport_activated () final override;

	X3D::SFNode
	on_create_parent (const std::string &, const std::string & = "children");

	/// @name Bar view handling

	virtual
	void
	on_menubar_toggled () final override;

	virtual
	void
	on_toolbar_toggled () final override;

	virtual
	void
	on_sidebar_toggled () final override;

	virtual
	void
	on_footer_toggled () final override;

	virtual
	void
	on_tabs_toggled () final override;

	virtual
	void
	on_motion_blur_activated () final override;

	///  @name Primitive Quality

	virtual
	void
	on_primitive_quality_high_toggled () final override;
	
	virtual
	void
	on_primitive_quality_medium_toggled () final override;

	virtual
	void
	on_primitive_quality_low_toggled () final override;

	void
	on_primitive_quality_changed (const std::string &);

	void
	set_primitiveQuality (const X3D::SFString &);

	///  @name Primitive Quality

	virtual
	void
	on_texture_quality_high_toggled () final override;

	virtual
	void
	on_texture_quality_medium_toggled () final override;

	virtual
	void
	on_texture_quality_low_toggled () final override;

	void
	on_texture_quality_changed (const std::string &);

	void
	set_textureQuality (const X3D::SFString &);

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
	on_fullscreen_activated () final override;

	virtual
	void
	on_unfullscreen_activated () final override;

	virtual
	void
	on_fullscreen (const bool) final override;

	///  @name Selection

	virtual
	void
	on_select_all_activated () final override;

	virtual
	void
	on_deselect_all_activated () final override;

	virtual
	void
	on_hide_selected_objects_activated () final override;

	virtual
	void
	on_hide_unselected_objects_activated () final override;

	virtual
	void
	on_show_selected_objects_activated () final override;

	virtual
	void
	on_show_all_objects_activated () final override;

	virtual
	void
	on_select_lowest_toggled () final override;

	virtual
	void
	on_follow_primary_selection_toggled () final override;

	virtual
	void
	on_transform_tool_mode_toggled () final override;

	///  @name Geometry

	virtual
	void
	on_union_activated () final override;

	virtual
	void
	on_difference_activated () final override;

	virtual
	void
	on_intersection_activated () final override;

	virtual
	void
	on_exclusion_activated () final override;

	virtual
	void
	on_combine_activated () final override;

	void
	on_boolean_activated (const std::string &, const BooleanOperation &, const bool);

	virtual
	void
	on_transform_to_zero_activated () final override;

	///  @name Layout

	virtual
	void
	on_browser_size_activate () final override;

	virtual
	void
	on_background_image_activate () final override;

	///  @name Grid

	void
	set_grid_visible ();

	void
	set_angle_grid_visible ();

	void
	set_axonometric_grid_visible ();

	virtual
	void
	on_grid_layout_tool_toggled () final override;

	virtual
	void
	on_angle_layout_tool_toggled () final override;

	virtual
	void
	on_axonometric_layout_tool_toggled () final override;

	virtual
	void
	on_grid_properties_activated () final override;

	///  @name Extenal Tools menu

	virtual
	void
	on_manage_external_tools_activate () final override;

	///  @name Scenes menu

	virtual
	void
	on_scenes_activated () final override;

	virtual
	void
	on_browser_scenes_activated () final override;

	void
	on_scenes_activated (Gtk::Menu &);

	///  @name Help menu

	virtual
	void
	on_cobweb_compatibility_toggled () final override;

	virtual
	void
	on_logarithmic_depth_buffer_toggled () final override;

	virtual
	void
	on_info_activated () final override;

	///  @name Editors

	virtual
	void
	on_node_index_clicked () final override;

	virtual
	void
	on_color_editor_clicked () final override;

	virtual
	void
	on_texture_mapping_editor_clicked () final override;

	virtual
	void
	on_prototype_editor_clicked () final override;

	virtual
	void
	on_node_editor_clicked () final override;

	///  @name Primitives toolbar

	virtual
	void
	on_arc_close_clicked () final override;

	virtual
	void
	on_disk_clicked () final override;

	virtual
	void
	on_rectangle_clicked () final override;

	virtual
	void
	on_star_clicked () final override;

	virtual
	void
	on_box_clicked () final override;

	virtual
	void
	on_cone_clicked () final override;

	virtual
	void
	on_cylinder_clicked () final override;

	virtual
	void
	on_elevation_grid_clicked () final override;

	virtual
	void
	on_pyramid_clicked () final override;

	virtual
	void
	on_sphere_clicked () final override;

	virtual
	void
	on_text_clicked () final override;

	void
	on_primitive_clicked (const std::string & description, const std::string & path);

	///  @name Browser dashboard handling

	void
	set_dashboard (const bool);

	virtual
	void
	on_hand_button_toggled () final override;

	virtual
	void
	on_arrow_button_toggled () final override;

	void
	set_arrow_button (const bool);

	virtual
	void
	on_play_pause_button_clicked () final override;

	void
	set_hierarchy ();

	virtual
	void
	on_select_parent_button_clicked () final override;

	virtual
	void
	on_select_child_button_clicked () final override;

	X3D::MFNode
	getChildren (const X3D::SFNode &, const bool = false) const;

	X3D::MFNode
	getChildrenInProtoinstance (const X3D::SFNode &, const std::set <X3D::SFNode> &, std::set <X3D::SFNode> &) const;

	void
	set_viewer ();

	void
	set_available_viewers (const X3D::MFEnum <X3D::X3DConstants::NodeType> &);

	virtual
	void
	on_select_viewer_clicked () final override;

	virtual
	void
	on_examine_viewer_toggled () final override;

	virtual
	void
	on_walk_viewer_toggled () final override;

	virtual
	void
	on_fly_viewer_toggled () final override;

	virtual
	void
	on_plane_viewer_toggled () final override;

	virtual
	void
	on_none_viewer_toggled () final override;

	void
	on_viewer_toggled (const X3D::X3DConstants::NodeType viewerType);

	void
	set_straighten_horizon ();

	virtual
	void
	on_straighten_clicked () final override;

	virtual
	void
	on_straighten_horizon_toggled () final override;

	virtual
	void
	on_look_at_selection_clicked () final override;

	virtual
	void
	on_look_at_all_clicked () final override;

	virtual
	void
	on_look_at_toggled () final override;

	bool
	checkForClones (const X3D::MFNode::const_iterator & first, const X3D::MFNode::const_iterator & last);

	void
	toggleActions (const Glib::RefPtr <Gtk::ToggleAction> &, const std::vector <Glib::RefPtr <Gtk::ToggleAction>> &);

	///  @name Members

	Glib::RefPtr <Gtk::CssProvider>                cssProvider;
	std::vector <Glib::RefPtr <Gtk::ToggleAction>> primitiveQualityActions;
	std::vector <Glib::RefPtr <Gtk::ToggleAction>> textureQualityActions;

	bool                        hand;
	X3D::X3DConstants::NodeType viewer;
	bool                        changing;
};

} // puck
} // titania

#endif

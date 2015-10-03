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

#include "X3DBrowserMenu.h"

namespace titania {
namespace puck {

X3DBrowserMenu::X3DBrowserMenu () :
	X3DBrowserWindowInterface ()
{
}

void
X3DBrowserMenu::initialize ()
{
}

bool
X3DBrowserMenu::on_notebook_button_press_event (GdkEventButton* event)
{
	if (event -> button == 3)
	{
	   getBrowserMenu () .popup (event -> button, event -> time);
		return true;
	}

	return false;
}

/*

void
X3DBrowserMenu::on_browser_new ()
{
	on_new ();
}

void
X3DBrowserMenu::on_browser_open ()
{
	on_open ();
}

void
X3DBrowserMenu::on_browser_open_recent ()
{
	on_open_recent ();
}

void
X3DBrowserMenu::on_browser_open_location ()
{
	on_open_location ();
}

void
X3DBrowserMenu::on_browser_import ()
{
	on_import ();
}

void
X3DBrowserMenu::on_browser_import_as_inline_toggled ()
{
	on_browser_import_as_inline_toggled ();
}

void
X3DBrowserMenu::on_browser_save ()
{
	on_save ();
}

void
X3DBrowserMenu::on_browser_save_as ()
{
	on_save_as ();
}

void
X3DBrowserMenu::on_browser_save_a_copy ()
{
	on_save_a_copy ();
}

void
X3DBrowserMenu::on_browser_export ()
{
	on_export ();
}

void
X3DBrowserMenu::on_browser_revert_to_saved ()
{
	on_revert_to_saved ();
}

void
X3DBrowserMenu::on_browser_remove_unused_prototypes ()
{
	on_remove_unused_prototypes ();
}

void
X3DBrowserMenu::on_browser_close ()
{
	on_close ();
}

void
X3DBrowserMenu::on_browser_quit ()
{
	on_quit ();
}

void
X3DBrowserMenu::on_browser_undo ()
{
	on_undo ();
}

void
X3DBrowserMenu::on_browser_redo ()
{
	on_redo ();
}

void
X3DBrowserMenu::on_browser_cut_nodes_activate ()
{
	on_cut_nodes_activate ();
}

void
X3DBrowserMenu::on_browser_copy_nodes_activate ()
{
	on_copy_nodes_activate ();
}

void
X3DBrowserMenu::on_browser_paste_nodes_activate ()
{
	on_paste_nodes_activate ();
}

void
X3DBrowserMenu::on_browser_delete_nodes_activate ()
{
	on_delete_nodes_activate ();
}

void
X3DBrowserMenu::on_browser_create_clone_activate ()
{
	on_create_clone_activate ();
}

void
X3DBrowserMenu::on_browser_unlink_clone_activate ()
{
	on_unlink_clone_activate ();
}

void
X3DBrowserMenu::on_browser_group_selected_nodes_activate ()
{
	on_group_selected_nodes_activate ();
}

void
X3DBrowserMenu::on_browser_ungroup_node_activate ()
{
	on_ungroup_node_activate ();
}

void
X3DBrowserMenu::on_browser_add_to_group_activate ()
{
	on_add_to_group_activate ();
}

void
X3DBrowserMenu::on_browser_detach_from_group_activate ()
{
	on_detach_from_group_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_transform_activate ()
{
	on_create_parent_transform_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_group_activate ()
{
	on_create_parent_group_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_static_group_activate ()
{
	on_create_parent_static_group_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_switch_activate ()
{
	on_create_parent_switch_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_billboard_activate ()
{
	on_create_parent_billboard_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_collision_activate ()
{
	on_create_parent_collision_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_lod_activate ()
{
	on_create_parent_lod_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_anchor_activate ()
{
	on_create_parent_anchor_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_layout_layer_activate ()
{
	on_create_parent_layout_layer_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_screen_group_activate ()
{
	on_create_parent_screen_group_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_layout_group_activate ()
{
	on_create_parent_layout_group_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_geo_transform_activate ()
{
	on_create_parent_geo_transform_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_geo_location_activate ()
{
	on_create_parent_geo_location_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_cad_part_activate ()
{
	on_create_parent_cad_part_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_cad_assembly_activate ()
{
	on_create_parent_cad_assembly_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_cad_layer_activate ()
{
	on_create_parent_cad_layer_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_layer_set_activate ()
{
	on_create_parent_layer_set_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_layer_activate ()
{
	on_create_parent_layer_activate ();
}

void
X3DBrowserMenu::on_browser_create_parent_viewport_activate ()
{
	on_create_parent_viewport_activate ();
}

void
X3DBrowserMenu::on_browser_toolBar_toggled ()
{
	on_toolBar_toggled ();
}

void
X3DBrowserMenu::on_browser_sideBar_toggled ()
{
	on_sideBar_toggled ();
}

void
X3DBrowserMenu::on_browser_footer_toggled ()
{
	on_footer_toggled ();
}

void
X3DBrowserMenu::on_browser_browser_toggled ()
{
	on_browser_toggled ();
}

void
X3DBrowserMenu::on_browser_editor_toggled ()
{
	on_editor_toggled ();
}

void
X3DBrowserMenu::on_browser_backgrounds_toggled ()
{
	on_backgrounds_toggled ();
}

void
X3DBrowserMenu::on_browser_fogs_toggled ()
{
	on_fogs_toggled ();
}

void
X3DBrowserMenu::on_browser_lights_toggled ()
{
	on_lights_toggled ();
}

void
X3DBrowserMenu::on_browser_proximity_sensors_toggled ()
{
	on_proximity_sensors_toggled ();
}

void
X3DBrowserMenu::on_browser_transform_sensors_toggled ()
{
	on_transform_sensors_toggled ();
}

void
X3DBrowserMenu::on_browser_visibility_sensors_toggled ()
{
	on_visibility_sensors_toggled ();
}

void
X3DBrowserMenu::on_browser_viewpoints_toggled ()
{
	on_viewpoints_toggled ();
}

void
X3DBrowserMenu::on_browser_rubberband_toggled ()
{
	on_rubberband_toggled ();
}

void
X3DBrowserMenu::on_browser_rendering_properties_toggled ()
{
	on_rendering_properties_toggled ();
}

void
X3DBrowserMenu::on_browser_select_lowest_toggled ()
{
	on_select_lowest_toggled ();
}

void
X3DBrowserMenu::on_browser_follow_primary_selection_toggled ()
{
	on_follow_primary_selection_toggled ();
}

void
X3DBrowserMenu::on_browser_grid_layout_tool_toggled ()
{
	on_grid_layout_tool_toggled ();
}

void
X3DBrowserMenu::on_browser_angle_layout_tool_toggled ()
{
	on_angle_layout_tool_toggled ();
}

*/

} // puck
} // titania

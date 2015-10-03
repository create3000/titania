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

#ifndef __TITANIA_BROWSER_X3DBROWSER_MENU_H__
#define __TITANIA_BROWSER_X3DBROWSER_MENU_H__

#include "../UserInterfaces/X3DBrowserWindowInterface.h"

namespace titania {
namespace puck {

class X3DBrowserMenu :
	virtual public X3DBrowserWindowInterface
{
public:

	~X3DBrowserMenu () = default;


protected:

	///  @name Construction

	X3DBrowserMenu ();

	virtual
	void
	initialize () override;


private:

	virtual
	bool
	on_notebook_button_press_event (GdkEventButton*) final override;

	/*

	virtual
	void
	on_browser_new () final override;

	virtual
	void
	on_browser_open () final override;

	virtual
	void
	on_browser_open_recent () final override;

	virtual
	void
	on_browser_open_location () final override;

	virtual
	void
	on_browser_import () final override;

	virtual
	void
	on_browser_import_as_inline_toggled () final override;

	virtual
	void
	on_browser_save () final override;

	virtual
	void
	on_browser_save_as () final override;

	virtual
	void
	on_browser_save_a_copy () final override;

	virtual
	void
	on_browser_export () final override;

	virtual
	void
	on_browser_revert_to_saved () final override;

	virtual
	void
	on_browser_remove_unused_prototypes () final override;

	virtual
	void
	on_browser_close () final override;

	virtual
	void
	on_browser_quit () final override;

	virtual
	void
	on_browser_undo () final override;

	virtual
	void
	on_browser_redo () final override;

	virtual
	void
	on_browser_cut_nodes_activate () final override;

	virtual
	void
	on_browser_copy_nodes_activate () final override;

	virtual
	void
	on_browser_paste_nodes_activate () final override;

	virtual
	void
	on_browser_delete_nodes_activate () final override;

	virtual
	void
	on_browser_create_clone_activate () final override;

	virtual
	void
	on_browser_unlink_clone_activate () final override;

	virtual
	void
	on_browser_group_selected_nodes_activate () final override;

	virtual
	void
	on_browser_ungroup_node_activate () final override;

	virtual
	void
	on_browser_add_to_group_activate () final override;

	virtual
	void
	on_browser_detach_from_group_activate () final override;

	virtual
	void
	on_browser_create_parent_transform_activate () final override;

	virtual
	void
	on_browser_create_parent_group_activate () final override;

	virtual
	void
	on_browser_create_parent_static_group_activate () final override;

	virtual
	void
	on_browser_create_parent_switch_activate () final override;

	virtual
	void
	on_browser_create_parent_billboard_activate () final override;

	virtual
	void
	on_browser_create_parent_collision_activate () final override;

	virtual
	void
	on_browser_create_parent_lod_activate () final override;

	virtual
	void
	on_browser_create_parent_anchor_activate () final override;

	virtual
	void
	on_browser_create_parent_layout_layer_activate () final override;

	virtual
	void
	on_browser_create_parent_screen_group_activate () final override;

	virtual
	void
	on_browser_create_parent_layout_group_activate () final override;

	virtual
	void
	on_browser_create_parent_geo_transform_activate () final override;

	virtual
	void
	on_browser_create_parent_geo_location_activate () final override;

	virtual
	void
	on_browser_create_parent_cad_part_activate () final override;

	virtual
	void
	on_browser_create_parent_cad_assembly_activate () final override;

	virtual
	void
	on_browser_create_parent_cad_layer_activate () final override;

	virtual
	void
	on_browser_create_parent_layer_set_activate () final override;

	virtual
	void
	on_browser_create_parent_layer_activate () final override;

	virtual
	void
	on_browser_create_parent_viewport_activate () final override;

	virtual
	void
	on_browser_toolBar_toggled () final override;

	virtual
	void
	on_browser_sideBar_toggled () final override;

	virtual
	void
	on_browser_footer_toggled () final override;

	virtual
	void
	on_browser_browser_toggled () final override;

	virtual
	void
	on_browser_editor_toggled () final override;

	virtual
	void
	on_browser_backgrounds_toggled () final override;

	virtual
	void
	on_browser_fogs_toggled () final override;

	virtual
	void
	on_browser_lights_toggled () final override;

	virtual
	void
	on_browser_proximity_sensors_toggled () final override;

	virtual
	void
	on_browser_transform_sensors_toggled () final override;

	virtual
	void
	on_browser_visibility_sensors_toggled () final override;

	virtual
	void
	on_browser_viewpoints_toggled () final override;

	virtual
	void
	on_browser_rubberband_toggled () final override;

	virtual
	void
	on_browser_rendering_properties_toggled () final override;

	virtual
	void
	on_browser_select_lowest_toggled () final override;

	virtual
	void
	on_browser_follow_primary_selection_toggled () final override;

	virtual
	void
	on_browser_grid_layout_tool_toggled () final override;

	virtual
	void
	on_browser_angle_layout_tool_toggled () final override;

	*/

};

} // puck
} // titania

#endif

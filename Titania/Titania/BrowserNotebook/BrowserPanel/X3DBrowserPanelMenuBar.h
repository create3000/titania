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

#ifndef __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_X3DBROWSER_VIEW_MENU_BAR_H__
#define __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_X3DBROWSER_VIEW_MENU_BAR_H__

#include "X3DBrowserPanel.h"

namespace titania {
namespace puck {

class ViewpointObserver;

class X3DBrowserPanelMenuBar :
	virtual public X3DBrowserPanel
{
public:

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowserPanelMenuBar () override;


protected:

	///  @name Construction

	X3DBrowserPanelMenuBar ();

	virtual
	void
	initialize () override;

	virtual
	void
	setLocalBrowser (const X3D::BrowserPtr & value);


private:

	///  @name Event handlers

	void
	on_menubar_toggled ();

	void
	set_initialized ();

	void
	set_focus (const bool focus);

	void
	set_scene ();

	void
	on_main_browser_hierarchy_changed (Gtk::Widget* previous_toplevel);

	void
	set_undoHistory ();

	///  @name View menu

	virtual
	void
	undo_view_activate () final override;

	virtual
	void
	redo_view_activate () final override;

	void
	set_viewer ();

	void
	set_straighten_horizon ();

	virtual
	void
	on_straighten_horizon_toggled () final override;

	void
	on_straighten_clicked ();

	virtual
	void
	on_look_at_selection_activate () final override;

	virtual
	void
	on_look_at_all_activate () final override;

	virtual
	void
	on_reset_user_offsets_activate () final override;

	///  @name Shading menu

	virtual
	void
	on_phong_toggled () final override;
	
	virtual
	void
	on_gouraud_toggled () final override;
	
	virtual
	void
	on_flat_toggled () final override;
	
	virtual
	void
	on_wireframe_toggled () final override;
	
	virtual
	void
	on_pointset_toggled () final override;

	void
	on_shading_changed (const std::string & value);
	
	void
	set_shading (const X3D::SFString & value);

	void
	set_texturing ();

	virtual
	void
	on_textures_toggled () final override;

	void
	set_shaders ();

	virtual
	void
	on_shaders_toggled () final override;

	///  @name Show menu

	void
	set_lightTools (const X3D::X3DWeakPtrArray <X3D::X3DLightNodeTool> & tools);
	
	void
	set_proximitySensorTools (const X3D::X3DWeakPtrArray <X3D::ProximitySensorTool> & tools);
	
	void
	set_soundTools (const X3D::X3DWeakPtrArray <X3D::SoundTool> & tools);
	
	void
	set_transformSensorTools (const X3D::X3DWeakPtrArray <X3D::TransformSensorTool> & tools);
	
	void
	set_visibilitySensorTools (const X3D::X3DWeakPtrArray <X3D::VisibilitySensorTool> & tools);
	
	void
	set_viewpointTools (const X3D::X3DWeakPtrArray <X3D::X3DViewpointNodeTool> & tools);

	///  @name Object Icons

	virtual
	void
	on_backgrounds_toggled () final override;

	virtual
	void
	on_fogs_toggled () final override;

	virtual
	void
	on_lights_toggled () final override;

	virtual
	void
	on_proximity_sensors_toggled () final override;

	virtual
	void
	on_sounds_toggled () final override;

	virtual
	void
	on_transform_sensors_toggled () final override;

	virtual
	void
	on_visibility_sensors_toggled () final override;

	virtual
	void
	on_viewpoints_toggled () final override;

	virtual
	void
	on_hide_all_object_icons_activated () final override;

	///  @name Layers menu

	virtual
	void
	on_layers_activate () final override;

	void
	on_layers_activate (const X3D::X3DPtr <X3D::X3DLayerNode> & layer, const size_t layerNumber);

	void
	on_layer_activate (const size_t layerNumber);

	///  @name Camera menu

	virtual
	void
	on_main_view_activate () final override;

	virtual
	void
	on_perspective_view_activate () final override;

	virtual
	void
	on_top_view_activate () final override;

	virtual
	void
	on_right_view_activate () final override;

	virtual
	void
	on_front_view_activate () final override;

	virtual
	void
	on_bottom_view_activate () final override;

	virtual
	void
	on_left_view_activate () final override;

	virtual
	void
	on_back_view_activate () final override;

	///  @name Members

	X3D::BrowserPtr                     browser;
	std::unique_ptr <ViewpointObserver> viewpointObserver;
	bool                                changing;

};

} // puck
} // titania

#endif

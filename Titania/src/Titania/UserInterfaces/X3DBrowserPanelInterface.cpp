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

#include "X3DBrowserPanelInterface.h"

namespace titania {
namespace puck {

void
X3DBrowserPanelInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DBrowserPanelInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DBrowserPanelInterface::create ()
{
	// Get objects.
	m_AccelGroup = Glib::RefPtr <Gtk::AccelGroup>::cast_dynamic (m_builder -> get_object ("AccelGroup"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("MenuBar", m_MenuBar);
	m_builder -> get_widget ("ViewMenuItem", m_ViewMenuItem);
	m_builder -> get_widget ("UndoViewMenuItem", m_UndoViewMenuItem);
	m_builder -> get_widget ("RedoViewMenuItem", m_RedoViewMenuItem);
	m_builder -> get_widget ("StraightenHorizonMenuItem", m_StraightenHorizonMenuItem);
	m_builder -> get_widget ("StraightenHorizonSeparatorMenuItem", m_StraightenHorizonSeparatorMenuItem);
	m_builder -> get_widget ("LookAtSelectionMenuItem", m_LookAtSelectionMenuItem);
	m_builder -> get_widget ("LookAtAllMenuItem", m_LookAtAllMenuItem);
	m_builder -> get_widget ("ResetUserOffsetsMenuItem", m_ResetUserOffsetsMenuItem);
	m_builder -> get_widget ("ShadingMenuItem", m_ShadingMenuItem);
	m_builder -> get_widget ("PhongMenuItem", m_PhongMenuItem);
	m_builder -> get_widget ("GouraudMenuItem", m_GouraudMenuItem);
	m_builder -> get_widget ("FlatMenuItem", m_FlatMenuItem);
	m_builder -> get_widget ("WireframeMenuItem", m_WireframeMenuItem);
	m_builder -> get_widget ("PointsetMenuItem", m_PointsetMenuItem);
	m_builder -> get_widget ("TexturesMenuItem", m_TexturesMenuItem);
	m_builder -> get_widget ("ShadersMenuItem", m_ShadersMenuItem);
	m_builder -> get_widget ("ShowMenuItem", m_ShowMenuItem);
	m_builder -> get_widget ("ShowHideEnvironmentalEffectsMenuItem", m_ShowHideEnvironmentalEffectsMenuItem);
	m_builder -> get_widget ("BackgroundsMenuItem", m_BackgroundsMenuItem);
	m_builder -> get_widget ("FogsMenuItem", m_FogsMenuItem);
	m_builder -> get_widget ("IconicObjectsMenuItem", m_IconicObjectsMenuItem);
	m_builder -> get_widget ("LightsMenuItem", m_LightsMenuItem);
	m_builder -> get_widget ("ProximitySensorsMenuItem", m_ProximitySensorsMenuItem);
	m_builder -> get_widget ("SoundsMenuItem", m_SoundsMenuItem);
	m_builder -> get_widget ("TextureProjectorsMenuItem", m_TextureProjectorsMenuItem);
	m_builder -> get_widget ("TransformSensorsMenuItem", m_TransformSensorsMenuItem);
	m_builder -> get_widget ("VisibilitySensorsMenuItem", m_VisibilitySensorsMenuItem);
	m_builder -> get_widget ("ViewpointsMenuItem", m_ViewpointsMenuItem);
	m_builder -> get_widget ("HideAllIconicObjectsMenuItem", m_HideAllIconicObjectsMenuItem);
	m_builder -> get_widget ("LayersMenuItem", m_LayersMenuItem);
	m_builder -> get_widget ("LayersMenu", m_LayersMenu);
	m_builder -> get_widget ("PanelsMenuItem", m_PanelsMenuItem);
	m_builder -> get_widget ("CameraMenuItem", m_CameraMenuItem);
	m_builder -> get_widget ("MainViewMenuItem", m_MainViewMenuItem);
	m_builder -> get_widget ("MainViewSeparatorMenuItem", m_MainViewSeparatorMenuItem);
	m_builder -> get_widget ("PerspectiveViewMenuItem", m_PerspectiveViewMenuItem);
	m_builder -> get_widget ("TopViewMenuItem", m_TopViewMenuItem);
	m_builder -> get_widget ("RightViewMenuItem", m_RightViewMenuItem);
	m_builder -> get_widget ("FrontViewMenuItem", m_FrontViewMenuItem);
	m_builder -> get_widget ("BottomViewMenuItem", m_BottomViewMenuItem);
	m_builder -> get_widget ("LeftViewMenuItem", m_LeftViewMenuItem);
	m_builder -> get_widget ("BackViewMenuItem", m_BackViewMenuItem);
	m_builder -> get_widget ("BrowserBox", m_BrowserBox);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_unmap));

	// Connect object Gtk::ImageMenuItem with id 'UndoViewMenuItem'.
	m_UndoViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::undo_view_activate));
	m_RedoViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::redo_view_activate));

	// Connect object Gtk::CheckMenuItem with id 'StraightenHorizonMenuItem'.
	m_StraightenHorizonMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_straighten_horizon_toggled));

	// Connect object Gtk::ImageMenuItem with id 'LookAtSelectionMenuItem'.
	m_LookAtSelectionMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_look_at_selection_activate));
	m_LookAtAllMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_look_at_all_activate));
	m_ResetUserOffsetsMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_reset_user_offsets_activate));

	// Connect object Gtk::RadioMenuItem with id 'PhongMenuItem'.
	m_PhongMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_phong_toggled));
	m_GouraudMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_gouraud_toggled));
	m_FlatMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_flat_toggled));
	m_WireframeMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_wireframe_toggled));
	m_PointsetMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_pointset_toggled));

	// Connect object Gtk::CheckMenuItem with id 'TexturesMenuItem'.
	m_TexturesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_textures_toggled));
	m_ShadersMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_shaders_toggled));
	m_BackgroundsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_backgrounds_toggled));
	m_FogsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_fogs_toggled));
	m_LightsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_lights_toggled));
	m_ProximitySensorsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_proximity_sensors_toggled));
	m_SoundsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_sounds_toggled));
	m_TextureProjectorsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_texture_projectors_toggled));
	m_TransformSensorsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_transform_sensors_toggled));
	m_VisibilitySensorsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_visibility_sensors_toggled));
	m_ViewpointsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_viewpoints_toggled));

	// Connect object Gtk::MenuItem with id 'HideAllIconicObjectsMenuItem'.
	m_HideAllIconicObjectsMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_hide_all_iconic_objects_activated));
	m_LayersMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_layers_activate));
	m_MainViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_main_view_activate));
	m_PerspectiveViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_perspective_view_activate));
	m_TopViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_top_view_activate));
	m_RightViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_right_view_activate));
	m_FrontViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_front_view_activate));
	m_BottomViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_bottom_view_activate));
	m_LeftViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_left_view_activate));
	m_BackViewMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DBrowserPanelInterface::on_back_view_activate));
}

X3DBrowserPanelInterface::~X3DBrowserPanelInterface ()
{
	delete m_Window;
}

} // puck
} // titania

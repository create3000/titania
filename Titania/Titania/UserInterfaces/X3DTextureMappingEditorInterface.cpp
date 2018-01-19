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

#include "X3DTextureMappingEditorInterface.h"

namespace titania {
namespace puck {

void
X3DTextureMappingEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DTextureMappingEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DTextureMappingEditorInterface::create ()
{
	// Get objects.
	m_TextureStageAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TextureStageAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("UndoMenuItem", m_UndoMenuItem);
	m_builder -> get_widget ("RedoMenuItem", m_RedoMenuItem);
	m_builder -> get_widget ("SelectionMenuItem", m_SelectionMenuItem);
	m_builder -> get_widget ("SelectAllMenuItem", m_SelectAllMenuItem);
	m_builder -> get_widget ("DeselectAllMenuItem", m_DeselectAllMenuItem);
	m_builder -> get_widget ("MappingsMenuItem", m_MappingsMenuItem);
	m_builder -> get_widget ("XPlaneMenuItem", m_XPlaneMenuItem);
	m_builder -> get_widget ("YPlaneMenuItem", m_YPlaneMenuItem);
	m_builder -> get_widget ("ZPlaneMenuItem", m_ZPlaneMenuItem);
	m_builder -> get_widget ("CameraMenuItem", m_CameraMenuItem);
	m_builder -> get_widget ("BoxMenuItem", m_BoxMenuItem);
	m_builder -> get_widget ("CylinderXMenuItem", m_CylinderXMenuItem);
	m_builder -> get_widget ("CylinderYMenuItem", m_CylinderYMenuItem);
	m_builder -> get_widget ("CylinderZMenuItem", m_CylinderZMenuItem);
	m_builder -> get_widget ("SphereXMenuItem", m_SphereXMenuItem);
	m_builder -> get_widget ("SphereYMenuItem", m_SphereYMenuItem);
	m_builder -> get_widget ("SphereZMenuItem", m_SphereZMenuItem);
	m_builder -> get_widget ("UndoButton", m_UndoButton);
	m_builder -> get_widget ("RedoButton", m_RedoButton);
	m_builder -> get_widget ("RotateCounterlockwiseButton", m_RotateCounterlockwiseButton);
	m_builder -> get_widget ("RotateClockwiseButton", m_RotateClockwiseButton);
	m_builder -> get_widget ("FlipButton", m_FlipButton);
	m_builder -> get_widget ("FlopButton", m_FlopButton);
	m_builder -> get_widget ("MergePointsButton", m_MergePointsButton);
	m_builder -> get_widget ("SplitPointButton", m_SplitPointButton);
	m_builder -> get_widget ("LeftBox", m_LeftBox);
	m_builder -> get_widget ("LeftHandButton", m_LeftHandButton);
	m_builder -> get_widget ("LeftArrowButton", m_LeftArrowButton);
	m_builder -> get_widget ("RotateButton", m_RotateButton);
	m_builder -> get_widget ("ScaleButton", m_ScaleButton);
	m_builder -> get_widget ("SnapCenterButton", m_SnapCenterButton);
	m_builder -> get_widget ("LeftLookAtButton", m_LeftLookAtButton);
	m_builder -> get_widget ("RightBox", m_RightBox);
	m_builder -> get_widget ("RightHandButton", m_RightHandButton);
	m_builder -> get_widget ("RightArrowButton", m_RightArrowButton);
	m_builder -> get_widget ("RightStraightenHorizonButton", m_RightStraightenHorizonButton);
	m_builder -> get_widget ("RightLookAtAllButton", m_RightLookAtAllButton);
	m_builder -> get_widget ("RightLookAtButton", m_RightLookAtButton);
	m_builder -> get_widget ("RemoveButton", m_RemoveButton);
	m_builder -> get_widget ("ApplyButton", m_ApplyButton);

	// Connect object Gtk::Adjustment with id 'TextureStageAdjustment'.
	m_TextureStageAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_texture_stage_changed));

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_key_press_event));
	m_Widget -> signal_key_release_event () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_key_release_event));

	// Connect object Gtk::ImageMenuItem with id 'UndoMenuItem'.
	m_UndoMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_undo));
	m_RedoMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_redo));
	m_SelectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_select_all_activate));

	// Connect object Gtk::MenuItem with id 'DeselectAllMenuItem'.
	m_DeselectAllMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_deselect_all_activate));
	m_XPlaneMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_x_plane_activate));
	m_YPlaneMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_y_plane_activate));
	m_ZPlaneMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_z_plane_activate));
	m_CameraMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_camera_activate));
	m_BoxMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_box_activate));
	m_CylinderXMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_cylinder_x_activate));
	m_CylinderYMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_cylinder_y_activate));
	m_CylinderZMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_cylinder_z_activate));
	m_SphereXMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_sphere_x_activate));
	m_SphereYMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_sphere_y_activate));
	m_SphereZMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_sphere_z_activate));

	// Connect object Gtk::ToolButton with id 'UndoButton'.
	m_UndoButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_undo));
	m_RedoButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_redo));
	m_RotateCounterlockwiseButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_rotate_counterclockwise));
	m_RotateClockwiseButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_rotate_clockwise));
	m_FlipButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_flip));
	m_FlopButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_flop));
	m_MergePointsButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_merge_points));
	m_SplitPointButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_split_point));

	// Connect object Gtk::RadioToolButton with id 'LeftHandButton'.
	m_LeftHandButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_left_hand_toggled));
	m_LeftArrowButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_left_arrow_toggled));
	m_RotateButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_left_rotate_toggled));
	m_ScaleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_left_scale_toggled));

	// Connect object Gtk::ToggleToolButton with id 'SnapCenterButton'.
	m_SnapCenterButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_left_snap_center_toggled));

	// Connect object Gtk::ToolButton with id 'LeftLookAtButton'.
	m_LeftLookAtButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_left_look_at_all_clicked));

	// Connect object Gtk::RadioToolButton with id 'RightHandButton'.
	m_RightHandButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_right_hand_toggled));
	m_RightArrowButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_right_arrow_toggled));

	// Connect object Gtk::ToggleToolButton with id 'RightStraightenHorizonButton'.
	m_RightStraightenHorizonButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_right_straighten_horizon_toggled));

	// Connect object Gtk::ToolButton with id 'RightLookAtAllButton'.
	m_RightLookAtAllButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_right_look_at_all_clicked));

	// Connect object Gtk::ToggleToolButton with id 'RightLookAtButton'.
	m_RightLookAtButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_right_look_at_toggled));

	// Connect object Gtk::Button with id 'RemoveButton'.
	m_RemoveButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_remove_clicked));
	m_ApplyButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DTextureMappingEditorInterface::on_apply_clicked));
}

X3DTextureMappingEditorInterface::~X3DTextureMappingEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania

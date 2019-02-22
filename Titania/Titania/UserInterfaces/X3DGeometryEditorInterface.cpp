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

#include "X3DGeometryEditorInterface.h"

namespace titania {
namespace puck {

void
X3DGeometryEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DGeometryEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DGeometryEditorInterface::create ()
{
	// Get objects.

	// Get widgets.
	m_builder -> get_widget ("CutPolygonsMenu", m_CutPolygonsMenu);
	m_builder -> get_widget ("CutPolygonsEnableSnappingMenuItem", m_CutPolygonsEnableSnappingMenuItem);
	m_builder -> get_widget ("TransformToolMenu", m_TransformToolMenu);
	m_builder -> get_widget ("AxisAlignedBoundingBoxMenuItem", m_AxisAlignedBoundingBoxMenuItem);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("GeometryEditorBox", m_GeometryEditorBox);
	m_builder -> get_widget ("EditToggleButton", m_EditToggleButton);
	m_builder -> get_widget ("HammerButton", m_HammerButton);
	m_builder -> get_widget ("TitleButton", m_TitleButton);
	m_builder -> get_widget ("GeometryToolsBox", m_GeometryToolsBox);
	m_builder -> get_widget ("NormalEnabledButton", m_NormalEnabledButton);
	m_builder -> get_widget ("FormNewFaceButton", m_FormNewFaceButton);
	m_builder -> get_widget ("ExtrudeSelectedEdgesButton", m_ExtrudeSelectedEdgesButton);
	m_builder -> get_widget ("ExtrudeSelectedFacesButton", m_ExtrudeSelectedFacesButton);
	m_builder -> get_widget ("ChipOfFacesButton", m_ChipOfFacesButton);
	m_builder -> get_widget ("DeleteFacesButton", m_DeleteFacesButton);
	m_builder -> get_widget ("FlipVertexOrderingButton", m_FlipVertexOrderingButton);
	m_builder -> get_widget ("PaintSelectionButton", m_PaintSelectionButton);
	m_builder -> get_widget ("PaintSelectionImage", m_PaintSelectionImage);
	m_builder -> get_widget ("SplitPointsButton", m_SplitPointsButton);
	m_builder -> get_widget ("MergePointsButton", m_MergePointsButton);
	m_builder -> get_widget ("TransformToolButton", m_TransformToolButton);
	m_builder -> get_widget ("CutPolygonsButton", m_CutPolygonsButton);
	m_builder -> get_widget ("SelectionTypeMenu", m_SelectionTypeMenu);
	m_builder -> get_widget ("BrushMenuItem", m_BrushMenuItem);
	m_builder -> get_widget ("RectangleMenuItem", m_RectangleMenuItem);
	m_builder -> get_widget ("LassoMenuItem", m_LassoMenuItem);
	m_builder -> get_widget ("PointsMenuItem", m_PointsMenuItem);
	m_builder -> get_widget ("EdgesMenuItem", m_EdgesMenuItem);
	m_builder -> get_widget ("FacesMenuItem", m_FacesMenuItem);
	m_builder -> get_widget ("SelectLineLoopMenuItem", m_SelectLineLoopMenuItem);

	// Connect object Gtk::CheckMenuItem with id 'CutPolygonsEnableSnappingMenuItem'.
	m_CutPolygonsEnableSnappingMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_cut_polygons_enable_snapping_toggled));
	m_AxisAlignedBoundingBoxMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_axis_aligned_bounding_box_toggled));

	// Connect object Gtk::Revealer with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_unmap));

	// Connect object Gtk::ToggleButton with id 'EditToggleButton'.
	m_EditToggleButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_edit_toggled));

	// Connect object Gtk::Button with id 'HammerButton'.
	m_HammerButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_hammer_clicked));
	m_TitleButton -> signal_button_press_event () .connect (sigc::bind (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_title_button_press_event), sigc::ref (*m_TitleButton)), false);
	m_TitleButton -> signal_button_release_event () .connect (sigc::bind (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_title_button_release_event), sigc::ref (*m_TitleButton)), false);
	m_TitleButton -> signal_motion_notify_event () .connect (sigc::bind (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_title_button_motion_notify_event), sigc::ref (*m_TitleButton)), false);
	m_FormNewFaceButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_form_new_face_clicked));
	m_ExtrudeSelectedEdgesButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_extrude_selected_edges_clicked));
	m_ExtrudeSelectedFacesButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_extrude_selected_faces_clicked));
	m_ChipOfFacesButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_chip_of_face_clicked));
	m_DeleteFacesButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_delete_selected_faces_clicked));
	m_FlipVertexOrderingButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_flip_vertex_ordering_clicked));

	// Connect object Gtk::RadioButton with id 'PaintSelectionButton'.
	m_PaintSelectionButton -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_selection_type_button_press_event));
	m_PaintSelectionButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_paint_selection_toggled));

	// Connect object Gtk::Button with id 'SplitPointsButton'.
	m_SplitPointsButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_split_points_clicked));
	m_MergePointsButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_merge_points_clicked));

	// Connect object Gtk::ToggleButton with id 'TransformToolButton'.
	m_TransformToolButton -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_transform_tool_button_press_event));
	m_TransformToolButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_transform_tool_toggled));

	// Connect object Gtk::RadioButton with id 'CutPolygonsButton'.
	m_CutPolygonsButton -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_cut_polygons_button_press_event));
	m_CutPolygonsButton -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_cut_polygons_toggled));

	// Connect object Gtk::ImageMenuItem with id 'BrushMenuItem'.
	m_BrushMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_brush_activated));
	m_RectangleMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_rectangle_activated));
	m_LassoMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_lasso_activated));

	// Connect object Gtk::RadioMenuItem with id 'PointsMenuItem'.
	m_PointsMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_points_toggled));
	m_EdgesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_edges_toggled));
	m_FacesMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_faces_toggled));

	// Connect object Gtk::CheckMenuItem with id 'SelectLineLoopMenuItem'.
	m_SelectLineLoopMenuItem -> signal_toggled () .connect (sigc::mem_fun (this, &X3DGeometryEditorInterface::on_select_line_loop_toggled));
}

X3DGeometryEditorInterface::~X3DGeometryEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania

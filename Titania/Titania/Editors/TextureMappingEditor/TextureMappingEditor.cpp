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

#include "TextureMappingEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Layering/X3DViewportNode.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Navigation/X3DViewpointNode.h>
#include <Titania/X3D/Components/PointingDeviceSensor/TouchSensor.h>
#include <Titania/X3D/Components/PointingDeviceSensor/PlaneSensor.h>
#include <Titania/X3D/Components/Rendering/Coordinate.h>
#include <Titania/X3D/Components/Rendering/IndexedLineSet.h>
#include <Titania/X3D/Components/Shape/Shape.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Texturing/MultiTexture.h>
#include <Titania/X3D/Components/Texturing/MultiTextureTransform.h>
#include <Titania/X3D/Editing/Selection/FaceSelection.h>

#include <complex>

namespace titania {
namespace puck {

using math::pi;
using math::pi2;

// Cylinder and Sphere Mapping Constants
constexpr double POLE_TRHESHOLD       = 0.001;
constexpr double NORTH_POLE_THRESHOLD = 1 - POLE_TRHESHOLD;
constexpr double SOUTH_POLE_THRESHOLD = POLE_TRHESHOLD;
constexpr double OVERLAP_THRESHOLD    = 0.6;

// Constants for the left Editor
constexpr double POINT_SIZE    = 6; // Use linewidthScaleFactor / 2 + 2.
constexpr double SNAP_DISTANCE = POINT_SIZE;

constexpr X3D::Vector2d
infinity2f (std::numeric_limits <float>::infinity (), std::numeric_limits <float>::infinity ());

TextureMappingEditor::TextureMappingEditor (X3DBrowserWindow* const browserWindow) :
	                X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DTextureMappingEditorInterface (get_ui ("Editors/TextureMappingEditor.glade")),
	                            left (X3D::createBrowser (getMasterBrowser (), { get_ui ("Editors/TextureMappingEditorLeftPreview.x3dv") })),
	                           right (X3D::createBrowser (getMasterBrowser (), { get_ui ("Editors/TextureMappingEditorRightPreview.x3dv") })),
	                           shape (),
	                      appearance (),
	                        material (),
	                         texture (),
	                textureTransform (),
	                        geometry (),
	                   multiTexCoord (),
	                           coord (),
	                 previewGeometry (),
	                        texCoord (),
	                     initialized (0),
	                           focus (BrowserType::LEFT),
	                           stage (0),
	                            tool (ToolType::MOVE),
	                  rightSelection (new X3D::FaceSelection (getBrowserWindow () -> getMasterBrowser ())),
	              rightPaintSelecion (false),
	                   selectedFaces (),
	                     activePoint (-1),
	                     masterPoint (-1),
	                  selectedPoints (),
	                   startHitPoint (),
	                     pointOffset (),
	                   startPosition (),
	                  startPositions (),
	                   startDistance (),
	                            keys (),
	                     undoHistory (),
	                        undoStep ()
{
	addChildObjects (left,
	                 right,
	                 shape,
	                 appearance,
	                 material,
	                 texture,
	                 textureTransform,
	                 geometry,
	                 multiTexCoord,
	                 coord,
	                 previewGeometry,
	                 texCoord,
                    rightSelection);

	setup ();
}

void
TextureMappingEditor::initialize ()
{
	X3DTextureMappingEditorInterface::initialize ();

	left  -> signal_focus_in_event ()  .connect (sigc::mem_fun (this, &TextureMappingEditor::on_focus_in_event));
	right -> signal_focus_in_event ()  .connect (sigc::mem_fun (this, &TextureMappingEditor::on_focus_in_event));

	left  -> initialized () .addInterest (&TextureMappingEditor::set_left_initialized, this);
	right -> initialized () .addInterest (&TextureMappingEditor::set_right_initialized, this);

	left  -> setAntialiasing (4);
	right -> setAntialiasing (4);
	left  -> setPrivateCursor ("ARROW");
	right -> setPrivateCursor ("ARROW");
	left  -> show ();
	right -> show ();

	getLeftBox ()  .pack_start (*left,  true, true, 0);
	getRightBox () .pack_start (*right, true, true, 0);

	rightSelection -> setup ();

	undoHistory .addInterest (&TextureMappingEditor::set_undoHistory, this);
}

void
TextureMappingEditor::configure ()
{
	X3DTextureMappingEditorInterface::configure ();

	getSnapCenterButton ()             .set_active (getConfig () -> getItem <bool> ("snapCenter"));
	getRightStraightenHorizonButton () .set_active (getConfig () -> getItem <bool> ("rightStraightenHorizon"));
}

void
TextureMappingEditor::set_left_initialized ()
{
	left -> initialized () .removeInterest (&TextureMappingEditor::set_left_initialized, this);
	++ initialized;
	set_initialized ();
}

void
TextureMappingEditor::set_right_initialized ()
{
	right -> initialized () .removeInterest (&TextureMappingEditor::set_left_initialized, this);
	++ initialized;
	set_initialized ();
}

void
TextureMappingEditor::set_initialized ()
{
	if (initialized not_eq 2)
		return;

	try
	{
		const auto appearance       = left -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto touchSensor      = left -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto centerSensor     = left -> getExecutionContext () -> getNamedNode <X3D::PlaneSensor> ("CenterSensor");

		touchSensor -> isActive ()             .addInterest (&TextureMappingEditor::set_left_active, this);
		touchSensor -> touchTime ()            .addInterest (&TextureMappingEditor::set_left_touchTime, this);
		touchSensor -> hitPoint_changed ()     .addInterest (&TextureMappingEditor::set_left_hitPoint, this);
		selectedGeometry ->                     addInterest (&TextureMappingEditor::set_left_image, this);
		centerSensor -> translation_changed () .addInterest (&TextureMappingEditor::set_left_center, this);

		appearance -> setPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	try
	{
		const auto transform         = right -> getExecutionContext () -> getNamedNode <X3D::Transform> ("Transform");
		const auto shape             = right -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto appearance        = right -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto touchSensor       = right -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const auto selectedGeometry  = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto selectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");

		transform -> addInterest (&TextureMappingEditor::set_right_viewer, this);
		shape -> geometry ()               .addInterest (&TextureMappingEditor::set_right_viewer, this);
		touchSensor -> isActive ()         .addInterest (&TextureMappingEditor::set_right_active, this);
		touchSensor -> touchTime ()        .addInterest (&TextureMappingEditor::set_right_touchTime, this);
		touchSensor -> hitPoint_changed () .addInterest (&TextureMappingEditor::set_right_hitPoint, this);

		appearance        -> setPrivate (true);
		selectedGeometry  -> setPrivate (true);
		selectionGeometry -> setPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	configure ();

	set_selection (getBrowserWindow () -> getSelection () -> getNodes ());
}

void
TextureMappingEditor::set_selection (const X3D::MFNode & selection)
{
	undoHistory .clear ();

	try
	{
		const auto shapes = getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

		if (shapes .empty ())
			set_shape (nullptr);

		else
			set_shape (shapes .back ());
	}
	catch (const X3D::X3DError &)
	{ }
}

// Focus handling

bool
TextureMappingEditor::on_focus_in_event (GdkEventFocus* event)
{
	if (left -> has_focus ())
	{
		focus = BrowserType::LEFT;

		getLeftBox ()  .get_style_context () -> add_class ("titania-widget-box-selected");
		getRightBox () .get_style_context () -> remove_class ("titania-widget-box-selected");
	}

	if (right -> has_focus ())
	{
		focus = BrowserType::RIGHT;

		getRightBox () .get_style_context () -> add_class ("titania-widget-box-selected");
		getLeftBox ()  .get_style_context () -> remove_class ("titania-widget-box-selected");
	}

	return false;
}

// Keyboard

bool
TextureMappingEditor::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);
	return false;
}

bool
TextureMappingEditor::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);
	return false;
}

// Edit

void
TextureMappingEditor::on_undo ()
{
	undoHistory .undo ();
}

void
TextureMappingEditor::on_redo ()
{
	undoHistory .redo ();
}

void
TextureMappingEditor::set_undoHistory ()
{
	if (undoHistory .hasUndo ())
	{
		getUndoMenuItem () .set_label (undoHistory .getUndoDescription ());
		getUndoButton ()   .set_tooltip_text (undoHistory .getUndoDescription ());
		getUndoMenuItem () .set_sensitive (true);
		getUndoButton ()   .set_sensitive (true);
	}
	else
	{
		getUndoMenuItem () .set_label (_ ("Undo"));
		getUndoButton ()   .set_tooltip_text (_ ("Undo last action (Ctrl-Z)."));
		getUndoMenuItem () .set_sensitive (false);
		getUndoButton ()   .set_sensitive (false);
	}

	if (undoHistory .hasRedo ())
	{
		getRedoMenuItem () .set_label (undoHistory .getRedoDescription ());
		getRedoButton ()   .set_tooltip_text (undoHistory .getRedoDescription ());
		getRedoMenuItem () .set_sensitive (true);
		getRedoButton ()   .set_sensitive (true);
	}
	else
	{
		getRedoMenuItem () .set_label (_ ("Redo"));
		getRedoButton ()   .set_tooltip_text (_ ("Redo last action (Ctrl-Shift-Z)."));
		getRedoMenuItem () .set_sensitive (false);
		getRedoButton ()   .set_sensitive (false);
	}
}

std::vector <int32_t>
TextureMappingEditor::on_remove_unused_texCoord_activate ()
{
	std::set <int32_t> indexIndex;

	for (const auto & index : previewGeometry -> texCoordIndex ())
		indexIndex .emplace (index);

	std::vector <int32_t> remap (texCoord -> point () .size ());
	X3D::MFVec2f          texCoords;

	texCoords .reserve (texCoord -> point () .size ());

	for (int32_t index = 0, size = texCoord -> point () .size (); index < size; ++ index)
	{
		if (indexIndex .count (index))
		{
			remap [index] = texCoords .size ();
			texCoords .emplace_back (texCoord -> point () .get1Value (index));
		}
	}

	X3D::MFInt32 texCoordIndex;

	texCoordIndex .reserve (previewGeometry -> texCoordIndex () .size ());

	for (const auto & index : previewGeometry -> texCoordIndex ())
	{
		try
		{
			texCoordIndex .emplace_back (index < 0 ? -1 : remap .at (index));
		}
		catch (const std::out_of_range &)
		{
			texCoordIndex .emplace_back (0);
		}
	}

	// Assign texCoordIndex and color.

	if (texCoords .size () == texCoord -> point () .size ())
		return remap;

	previewGeometry -> texCoordIndex () = std::move (texCoordIndex);
	texCoord -> point () = std::move (texCoords);

	return remap;
}

// Mappings

void
TextureMappingEditor::on_x_plane_activate ()
{
	on_plane_activate (2, 1, false);
}

void
TextureMappingEditor::on_y_plane_activate ()
{
	on_plane_activate (0, 2, true);
}

void
TextureMappingEditor::on_z_plane_activate ()
{
	on_plane_activate (0, 1, false);
}

void
TextureMappingEditor::on_plane_activate (const size_t x, const size_t y, const bool flop)
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Plane Mapping"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox    = getBBox (x, y);
	const auto extents = bbox .extents ();
	const auto size    = bbox .size ();
	const auto m       = size .x () >= size .y () ? 0 : 1;

	// Apply mapping.
	
	std::map <int32_t, std::vector <size_t>> indices;
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
			indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
	}

	for (const auto & pair : indices)
	{
		const auto point    = coord -> get1Point (pair .first);
		auto       texPoint = (X3D::Vector2f (point [x], point [y]) - extents .first) / size [m];

		if (flop)
			texPoint [1] = 1 - texPoint [1];

		for (const auto & vertex : pair .second)
			previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

		texCoord -> point () .emplace_back (texPoint);
	}

	on_remove_unused_texCoord_activate ();

	selectedPoints .clear ();
	set_selectedPoints ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureMappingEditor::on_camera_activate ()
{
	try
	{
		const auto & activeLayer = right -> getActiveLayer ();

		if (not activeLayer)
			return;

		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Camera Mapping"));

		undoStep -> addObjects (previewGeometry, texCoord);
		undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
		undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

		// Apply camera projection.

		const auto          viewport   = X3D::Vector4i (0, 0, 1, 1);
		const auto          projection = activeLayer -> getViewpoint () -> getProjectionMatrix (0.125, -10000000.0, viewport);
		const X3D::Matrix4d modelview  = activeLayer -> getViewpoint () -> getInverseCameraSpaceMatrix ();
	
		std::map <int32_t, std::vector <size_t>> indices;
		
		for (const auto & face : selectedFaces)
		{
			for (const auto & vertex : rightSelection -> getFaceVertices (face))
				indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
		}

		for (const auto & pair : indices)
		{
			const auto point    = coord -> get1Point (pair .first);
			const auto texPoint = X3D::ViewVolume::projectPoint (point, modelview, projection, viewport);

			for (const auto & vertex : pair .second)
				previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

			texCoord -> point () .emplace_back (texPoint .x (), texPoint .y ());
		}

		on_remove_unused_texCoord_activate ();

		selectedPoints .clear ();
		set_selectedPoints ();

		undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
		undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

		addUndoStep (undoStep);
	}
	catch (const std::domain_error &)
	{ }
}

void
TextureMappingEditor::on_box_activate ()
{
	__LOG__ << std::endl;
}

void
TextureMappingEditor::on_cylinder_x_activate ()
{
	on_cylinder_activate (1, 0, 2, true, 0);
}

void
TextureMappingEditor::on_cylinder_y_activate ()
{
	on_cylinder_activate (0, 1, 2, false, 0);
}

void
TextureMappingEditor::on_cylinder_z_activate ()
{
	on_cylinder_activate (0, 2, 1, true, 1);
}

void
TextureMappingEditor::on_cylinder_activate (const size_t x, const size_t y, const size_t z, const bool flip, const size_t f)
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Cylinder Mapping"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox    = previewGeometry -> getBBox ();
	const auto extents = bbox .extents ();
	const auto center  = bbox .center ();
	const auto size    = bbox .size ();

	// Apply mapping.
	
	std::map <int32_t, std::vector <size_t>> indices;
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
			indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
	}

	for (const auto & pair : indices)
	{
		const auto point    = coord -> get1Point (pair .first);
		const auto complex  = std::complex <float> (point [z] - center [z], point [x] - center [x]);
		auto       texPoint = X3D::Vector2f (std::arg (complex) / pi2 <double> + 0.5, (point [y] - extents .first [y]) / size [y]);

		if (flip)
			texPoint [f] = 1 - texPoint [f];

		for (const auto & vertex : pair .second)
			previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

		texCoord -> point () .emplace_back (texPoint);
	}
	
	resolveOverlaps ();

	on_remove_unused_texCoord_activate ();

	selectedPoints .clear ();
	set_selectedPoints ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureMappingEditor::on_sphere_x_activate ()
{
	on_sphere_activate (1, 0, 2, true, 0);
}

void
TextureMappingEditor::on_sphere_y_activate ()
{
	on_sphere_activate (0, 1, 2, false, 0);
}

void
TextureMappingEditor::on_sphere_z_activate ()
{
	on_sphere_activate (0, 2, 1, true, 1);
}

void
TextureMappingEditor::on_sphere_activate (const size_t x, const size_t y, const size_t z, const bool flip, const size_t f)
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Sphere Mapping"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto          bbox   = previewGeometry -> getBBox ();
	const X3D::Vector3d center = bbox .center ();

	// Apply mapping.

	std::map <int32_t, std::vector <size_t>> indices;
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
			indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
	}

	for (const auto & pair : indices)
	{
		const auto point    = math::normalize (coord -> get1Point (pair .first) - center);
		auto       texPoint = X3D::Vector2f (std::atan2 (point [x], point [z]) / pi2 <double> + 0.5, std::asin (point [y]) / pi <double> + 0.5);

		if (flip)
			texPoint [f] = 1 - texPoint [f];

		for (const auto & vertex : pair .second)
			previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

		texCoord -> point () .emplace_back (texPoint);
	}

	resolveOverlaps ();

	for (const auto & face : selectedFaces)
	{
		const auto vertices = rightSelection -> getFaceVertices (face);

		float min = 1;
		float max = 0;

		for (const auto & vertex : vertices)
		{
			const auto          index    = previewGeometry -> texCoordIndex () [vertex];
			const X3D::Vector2f texPoint = texCoord -> point () .get1Value (index);
			
			if (texPoint .y () < NORTH_POLE_THRESHOLD and texPoint .y () > SOUTH_POLE_THRESHOLD)
			{
				min = std::min (min, texPoint .x ());
				max = std::max (max, texPoint .x ());
			}
		}
		
		const auto center = (min + max) / 2;

		for (const auto & vertex : vertices)
		{
			const auto          index    = previewGeometry -> texCoordIndex () [vertex];
			const X3D::Vector2f texPoint = texCoord -> point () .get1Value (index);

			if (texPoint .y () >= NORTH_POLE_THRESHOLD)
			{
				previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();
				texCoord -> point () .emplace_back (center, texPoint .y ());
				continue;
			}

			if (texPoint .y () <= SOUTH_POLE_THRESHOLD)
			{
				previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();
				texCoord -> point () .emplace_back (center, texPoint .y ());
			}
		}
	}

	on_remove_unused_texCoord_activate ();

	selectedPoints .clear ();
	set_selectedPoints ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureMappingEditor::resolveOverlaps ()
{
	std::map <int32_t, size_t> remap;

	for (const auto & face : selectedFaces)
	{
		const auto vertices = rightSelection -> getFaceVertices (face);
		
		if (vertices .empty ())
			continue;

		// Find first point not north pole or south pole.  This is a good start point to calculate
		// the distance to the other points.

		size_t first = 0;
		
		for (size_t size = vertices .size (); first < size; ++ first)
		{
			const auto          index    = previewGeometry -> texCoordIndex () [vertices [first]];
			const X3D::Vector2f texPoint = texCoord -> point () .get1Value (index);
			
			if (texPoint .y () < NORTH_POLE_THRESHOLD and texPoint .y () > SOUTH_POLE_THRESHOLD)
				break;
		}

		if (first == vertices .size ())
			first = 0;

		// Remap

		const auto          index1    = previewGeometry -> texCoordIndex () [vertices [first]];
		const X3D::Vector2f texPoint1 = texCoord -> point () .get1Value (index1);

		for (size_t j = first + 1, size = first + vertices .size (); j < size; ++ j)
		{
			const auto          i         = j % vertices .size ();
			const auto          index2    = previewGeometry -> texCoordIndex () [vertices [i]];
			const X3D::Vector2f texPoint2 = texCoord -> point () .get1Value (index2);
			const auto          distance  = texPoint1 .x () - texPoint2 .x ();
	
			if (distance > OVERLAP_THRESHOLD)
			{
				const auto pair = remap .emplace (previewGeometry -> texCoordIndex () [vertices [i]], texCoord -> point () .size ());
			
				if (pair .second)
				{
					previewGeometry -> texCoordIndex () [vertices [i]] = texCoord -> point () .size ();
					texCoord -> point () .emplace_back (texPoint2 .x () + 1, texPoint2 .y ());
				}
				else
					previewGeometry -> texCoordIndex () [vertices [i]] = pair .first -> second;
			}
			else if (distance < -OVERLAP_THRESHOLD)
			{
				const auto pair = remap .emplace (previewGeometry -> texCoordIndex () [vertices [i]], texCoord -> point () .size ());
			
				if (pair .second)
				{
					previewGeometry -> texCoordIndex () [vertices [i]] = texCoord -> point () .size ();
					texCoord -> point () .emplace_back (texPoint2 .x () - 1, texPoint2 .y ());
				}
				else
					previewGeometry -> texCoordIndex () [vertices [i]] = pair .first -> second;
			}
		}
	}
}

X3D::Box2f
TextureMappingEditor::getTexBBox () const
{
	// Determine bbox extents.

	std::vector <X3D::Vector2f> points;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
			points .emplace_back (texCoord -> point () .get1Value (previewGeometry -> texCoordIndex () [vertex]));
	}

	return X3D::Box2f (points .begin (), points .end (), math::iterator_type ());
}

X3D::Box2f
TextureMappingEditor::getBBox (const size_t i1, const size_t i2) const
{
	// Determine bbox of selected faces.

	std::vector <X3D::Vector2f> points;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
		{
			const auto point = coord -> get1Point (previewGeometry -> coordIndex () [vertex]);
			points .emplace_back (point [i1], point [i2]);
		}
	}

	return X3D::Box2f (points .begin (), points .end (), math::iterator_type ());
}

// Selection

void
TextureMappingEditor::on_select_all_activate ()
{
	switch (focus)
	{
		case BrowserType::LEFT:
		{
			try
			{
				const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
	
				for (const auto & index : selectedGeometry -> coordIndex ())
				{
					if (index >= 0)
						selectedPoints .emplace (index);
				}
	
				set_selectedPoints ();
			}
			catch (const X3D::X3DError & error)
			{ }

			break;
		}
		case BrowserType::RIGHT:
		{
			const auto faces = rightSelection -> getFaces ();
	
			selectedFaces .insert (faces .begin (), faces .end ());
	
			set_selected_faces ();
			break;
		}
	}
}

void
TextureMappingEditor::on_deselect_all_activate ()
{
	switch (focus)
	{
		case BrowserType::LEFT:
		{
			selectedPoints .clear ();
	
			set_selectedPoints ();
			break;
		}
		case BrowserType::RIGHT:
		{
			selectedFaces .clear ();
	
			set_selected_faces ();
			break;
		}
	}
}

// Toolbar

void
TextureMappingEditor::on_rotate_counterclockwise ()
{
	on_rotate (_ ("Rotate 90° Counterclockwise"), pi <float> / 2);
}

void
TextureMappingEditor::on_rotate_clockwise ()
{
	on_rotate (_ ("Rotate 90° Clockwise"), -pi <float> / 2);
}

void
TextureMappingEditor::on_rotate (const std::string & description, const float angle)
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (description);

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox   = getTexBBox ();
	const auto center = bbox .center ();

	// Apply mapping.

	std::set <int32_t> vertices;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
			vertices .emplace (previewGeometry -> texCoordIndex () .get1Value (vertex));
	}

	for (const auto & vertex : vertices)
	{
		auto &     point   = texCoord -> point () .get1Value (vertex);
		const auto rotated = math::rotate (point - center, angle) + center;

		texCoord -> point () .set1Value (vertex, rotated);
	}

	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}


void
TextureMappingEditor::on_flip ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Flip Horizontally"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox    = getTexBBox ();
	const auto center2 = 2 * bbox .center () .x ();

	// Apply mapping.

	std::set <int32_t> vertices;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
			vertices .emplace (previewGeometry -> texCoordIndex () [vertex]);
	}

	for (const auto & vertex : vertices)
	{
		auto point = texCoord -> point () .get1Value (vertex);

		point .x (center2 - point .x ());

		texCoord -> point () .set1Value (vertex, point);
	}

	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureMappingEditor::on_flop ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Flip Vertically"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox    = getTexBBox ();
	const auto center2 = 2 * bbox .center () .y ();

	// Apply mapping.

	std::set <int32_t> vertices;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
			vertices .emplace (previewGeometry -> texCoordIndex () [vertex]);
	}

	for (const auto & vertex : vertices)
	{
		auto point = texCoord -> point () .get1Value (vertex);

		point .y (center2 - point .y ());

		texCoord -> point () .set1Value (vertex, point);
	}

	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureMappingEditor::on_merge_points ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Split Selected Points"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Merge.
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
		{
			const auto index = previewGeometry -> texCoordIndex () [vertex];

			if (selectedPoints .count (index))
				 previewGeometry -> texCoordIndex () [vertex] = masterPoint;
		}
	}

	// Set new selected point.
	
	const auto remap = on_remove_unused_texCoord_activate ();
	
	if ((size_t) masterPoint < remap .size ())
		selectedPoints = { masterPoint = remap [masterPoint] };
	else
		selectedPoints .clear ();

	set_selectedPoints ();

	// Redo.
	
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureMappingEditor::on_split_point ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Split Selected Points"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Split.
	
	std::map <int32_t, std::vector <size_t>> points;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getFaceVertices (face))
		{
			const auto index = previewGeometry -> texCoordIndex () [vertex];
		
			if (selectedPoints .count (index))
				points [index] .emplace_back (vertex);
		}
	}
	
	std::vector <size_t> selection;
	
	for (const auto & point : points)
	{
		const auto & indices = point .second;

		if (indices .empty ())
			continue;

		const auto vertex   = previewGeometry -> texCoordIndex () [indices .front ()];
		const auto texPoint = texCoord -> point () .get1Value (vertex);

		selection .emplace_back (texCoord -> point () .size ());

		for (const auto & index : indices)
		{
			previewGeometry -> texCoordIndex () [index] = texCoord -> point () .size ();
			texCoord -> point () .emplace_back (texPoint);
		}
	}
	
	const auto remap = on_remove_unused_texCoord_activate ();
	
	// Set new selected points.
	
	selectedPoints .clear ();

	for (const auto & point : selection)
	{
		if (point < remap .size ())
			selectedPoints .emplace (remap [point]);
	}

	if (not selectedPoints .empty ())
		masterPoint = *selectedPoints .begin ();

	set_selectedPoints ();

	// Redo.
	
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

// Dashboard

void
TextureMappingEditor::on_left_hand_toggled ()
{
	left -> setPickable (false);
	left -> grab_focus ();
}

void
TextureMappingEditor::on_left_arrow_toggled ()
{
	tool = ToolType::MOVE;
	left -> setPickable (true);
	left -> grab_focus ();
}

void
TextureMappingEditor::on_left_rotate_toggled ()
{
	tool = ToolType::ROTATE;
	left -> setPickable (true);
	left -> grab_focus ();
}

void
TextureMappingEditor::on_left_scale_toggled ()
{
	tool = ToolType::SCALE;
	left -> setPickable (true);
	left -> grab_focus ();
}

void
TextureMappingEditor::on_left_snap_center_toggled ()
{
	getConfig () -> setItem ("snapCenter", getSnapCenterButton () .get_active ());
}

void
TextureMappingEditor::on_left_look_at_all_clicked ()
{
	if (left -> getActiveLayer ())
		left -> getActiveLayer () -> lookAt ();

	left -> grab_focus ();
}

void
TextureMappingEditor::on_right_hand_toggled ()
{
	right -> setPickable (false);
	right -> grab_focus ();
}

void
TextureMappingEditor::on_right_arrow_toggled ()
{
	right -> setPickable (true);
	right -> grab_focus ();
}

void
TextureMappingEditor::on_right_straighten_horizon_toggled ()
{
	right -> setStraightenHorizon (getRightStraightenHorizonButton () .get_active ());

	if (getRightStraightenHorizonButton () .get_active ())
	{
		const auto & activeLayer = right -> getActiveLayer ();
	
		if (activeLayer)
			activeLayer -> getViewpoint () -> straighten (right -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
	}

	getConfig () -> setItem <bool> ("rightStraightenHorizon", getRightStraightenHorizonButton () .get_active ());
}

void
TextureMappingEditor::on_right_look_at_all_clicked ()
{
	if (right -> getActiveLayer ())
		right -> getActiveLayer () -> lookAt ();

	right -> grab_focus ();
}

void
TextureMappingEditor::on_right_look_at_toggled ()
{
	if (getRightLookAtButton () .get_active ())
	{
		if (right -> getViewerType () not_eq X3D::X3DConstants::LookAtViewer)
			right -> setViewerType (X3D::X3DConstants::LookAtViewer);
	}
	else
	{
		if (right -> getViewerType () not_eq X3D::X3DConstants::ExamineViewer)
			right -> setViewerType (X3D::X3DConstants::ExamineViewer);
	}

	right -> grab_focus ();
}

void
TextureMappingEditor::on_remove_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Texture Coordinates"));

	undoStep -> addObjects (geometry);

	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), geometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFInt32::clear, std::ref (geometry -> texCoordIndex ()));
	geometry -> texCoordIndex () .clear ();

	X3D::X3DEditor::replaceNode (getCurrentContext (), geometry, geometry -> texCoord (), nullptr, undoStep);

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
TextureMappingEditor::on_apply_clicked ()
{
	geometry -> texCoordIndex () .removeInterest (&TextureMappingEditor::set_texCoordIndex, this);
	geometry -> texCoordIndex () .addInterest (&TextureMappingEditor::connectTexCoordIndex, this);

	if (multiTexCoord)
	{
		multiTexCoord -> removeInterest (&TextureMappingEditor::set_texCoord, this);
		multiTexCoord -> addInterest (&TextureMappingEditor::connectMultiTexCoord, this);
	}
	else
	{
		geometry -> texCoord () .removeInterest (&TextureMappingEditor::set_texCoord, this);
		geometry -> texCoord () .addInterest (&TextureMappingEditor::connectTexCoord, this);
	}

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Apply Texture Coordinates"));

	undoStep -> addObjects (geometry);

	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), geometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	geometry -> texCoordIndex () = previewGeometry -> texCoordIndex ();

	// Replace texCoord or multiTexCoord.

	const X3D::SFNode texCoordNode (texCoord -> copy (geometry -> getExecutionContext (), X3D::FLAT_COPY));
	const X3D::X3DPtr <X3D::MultiTextureCoordinate> multiTextureCoordinate (geometry -> texCoord ());

	if (multiTextureCoordinate)
	{
		//		X3D::MFNode texCoords (multiTextureCoordinate -> getTexCoord () .begin (), multiTextureCoordinate -> getTexCoord () .end ());
		//
		//		if (texCoords .empty ())
		//		{
		//			// XXX: Fill with default texture coordinates.
		//			for (size_t i = 0, size = stage; i < size; ++ i)
		//			 	texCoords .emplace_back (texCoordNode);
		//		}
		//		else
		//		{
		//			for (size_t i = texCoords .size () - 1, size = stage; i < size; ++ i)
		//				texCoords .emplace_back (texCoords .back ());
		//		}
		//
		//		texCoords .set1Value (stage, texCoordNode);
		//
		//		getBrowserWindow () -> replaceNodes (getCurrentContext (), X3D::SFNode (multiTextureCoordinate), multiTextureCoordinate -> texCoord (), texCoords, undoStep);

		X3D::X3DEditor::replaceNodes (getCurrentContext (), multiTextureCoordinate, multiTextureCoordinate -> texCoord (), { texCoordNode }, undoStep);
	}
	else
		X3D::X3DEditor::replaceNode (getCurrentContext (), geometry, geometry -> texCoord (), texCoordNode, undoStep);

	geometry -> getExecutionContext () -> realize ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
TextureMappingEditor::connectTexCoordIndex ()
{
	geometry -> texCoordIndex () .removeInterest (&TextureMappingEditor::connectTexCoordIndex, this);
	geometry -> texCoordIndex () .addInterest (&TextureMappingEditor::set_texCoordIndex, this);
}

void
TextureMappingEditor::connectTexCoord ()
{
	geometry -> texCoord () .removeInterest (&TextureMappingEditor::connectTexCoord, this);
	geometry -> texCoord () .addInterest (&TextureMappingEditor::set_texCoord, this);
}

void
TextureMappingEditor::connectMultiTexCoord ()
{
	multiTexCoord -> removeInterest (&TextureMappingEditor::connectMultiTexCoord, this);
	multiTexCoord -> addInterest (&TextureMappingEditor::set_texCoord, this, multiTexCoord);
}

void
TextureMappingEditor::set_left_viewer ()
{
	if (left -> getActiveLayer ())
	{
		left -> getActiveLayer () -> getViewpoint () -> resetUserOffsets ();
		left -> getActiveLayer () -> lookAt ();
	}
}

void
TextureMappingEditor::set_right_viewer ()
{
	if (right -> getActiveLayer ())
	{
		right -> getActiveLayer () -> getViewpoint () -> resetUserOffsets ();
		right -> getActiveLayer () -> lookAt ();
	}
}

void
TextureMappingEditor::set_shape (const X3D::X3DPtr <X3D::X3DShapeNode> & value)
{
	try
	{
		if (shape)
		{
			shape -> appearance () .removeInterest (&TextureMappingEditor::set_appearance, this);
			shape -> geometry ()   .removeInterest (&TextureMappingEditor::set_geometry, this);
		}

		shape = value;

		if (shape)
		{
			const auto transform   = right -> getExecutionContext () -> getNamedNode <X3D::Transform> ("Transform");
			const auto modelMatrix = X3D::X3DEditor::getModelMatrix (getCurrentContext (), shape);

			transform -> setMatrix (modelMatrix);

			shape -> appearance () .addInterest (&TextureMappingEditor::set_appearance, this);
			shape -> geometry ()   .addInterest (&TextureMappingEditor::set_geometry, this);

			set_appearance (shape -> appearance ());
			set_geometry (shape -> geometry ());
		}
		else
		{
			set_appearance (nullptr);
			set_geometry (nullptr);
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_appearance (const X3D::SFNode & value)
{
	if (appearance)
	{
		appearance -> material ()         .removeInterest (&TextureMappingEditor::set_material, this);
		appearance -> texture ()          .removeInterest (&TextureMappingEditor::set_texture_stage, this);
		appearance -> textureTransform () .removeInterest (&TextureMappingEditor::set_textureTransform, this);
	}

	appearance = value;

	if (appearance)
	{
		appearance -> material ()         .addInterest (&TextureMappingEditor::set_material, this);
		appearance -> texture ()          .addInterest (&TextureMappingEditor::set_texture_stage, this);
		appearance -> textureTransform () .addInterest (&TextureMappingEditor::set_textureTransform, this);

		set_material (appearance -> material ());
		set_texture_stage (appearance -> texture ());
		set_textureTransform (appearance -> textureTransform ());
	}
	else
	{
		set_material (nullptr);
		set_texture_stage (nullptr);
		set_textureTransform (nullptr);
	}
}

void
TextureMappingEditor::set_material (const X3D::SFNode & value)
{
	if (material)
		material -> removeInterest (&X3D::Browser::addEvent, *right);

	material = value;

	if (material)
		material -> addInterest (&X3D::Browser::addEvent, *right);

	right -> addEvent ();
}

void
TextureMappingEditor::set_texture_stage (const X3D::SFNode & value)
{
	const X3D::X3DPtr <X3D::MultiTexture> multiTexture (value);

	const size_t textureStages = multiTexture ? multiTexture -> getTexture () .size () : 1;

	getTextureStageAdjustment () -> set_upper (textureStages);
	getTextureStageAdjustment () -> set_value (std::min <size_t> (getTextureStageAdjustment () -> get_value (), textureStages));

	set_texture (value);
}

void
TextureMappingEditor::set_texture (const X3D::SFNode & value)
{
	if (texture)
	{
		texture -> removeInterest (&X3D::Browser::addEvent, *left);
		texture -> removeInterest (&X3D::Browser::addEvent, *right);
	}

	const X3D::X3DPtr <X3D::MultiTexture> multiTexture (value);

	if (multiTexture)
	{
		if (stage < multiTexture -> getTexture () .size ())
			texture = multiTexture -> getTexture () [stage];
		else
			texture = nullptr;
	}
	else
		texture = value;

	try
	{
		const auto imageSwitch     = left  -> getExecutionContext () -> getNamedNode <X3D::Switch> ("ImageSwitch");
		const auto leftAppearance  = left  -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto rightAppearance = right -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		imageSwitch -> whichChoice () = texture;
		leftAppearance  -> texture () = texture;
		rightAppearance -> texture () = texture;
	}
	catch (const X3D::X3DError &)
	{ }

	if (texture)
	{
		texture -> addInterest (&X3D::Browser::addEvent, *left);
		texture -> addInterest (&X3D::Browser::addEvent, *right);
	}
}

void
TextureMappingEditor::set_textureTransform (const X3D::SFNode & value)
{
	if (textureTransform)
	{
		textureTransform -> removeInterest (&X3D::Browser::addEvent, *left);
		textureTransform -> removeInterest (&X3D::Browser::addEvent, *right);
	}

	const X3D::X3DPtr <X3D::MultiTextureTransform> multiTextureTransform (value);

	if (multiTextureTransform)
	{
		if (multiTextureTransform -> getTextureTransform () .empty ())
			textureTransform = nullptr;
		else
			textureTransform = multiTextureTransform -> getTextureTransform () [std::min (stage, multiTextureTransform -> getTextureTransform () .size () - 1)];
	}
	else
		textureTransform = value;

	try
	{
		const auto leftAppearance  = left  -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto rightAppearance = right -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		leftAppearance  -> textureTransform () = textureTransform;
		rightAppearance -> textureTransform () = textureTransform;
	}
	catch (const X3D::X3DError &)
	{ }

	if (textureTransform)
	{
		textureTransform -> addInterest (&X3D::Browser::addEvent, *left);
		textureTransform -> addInterest (&X3D::Browser::addEvent, *right);
	}

	left  -> addEvent ();
	right -> addEvent ();
}

void
TextureMappingEditor::set_geometry (const X3D::SFNode & value)
{
	try
	{
		const auto rightShape             = right -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto rightSelectedGeometry  = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto rightSelectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");

		if (geometry)
		{
			geometry -> texCoordIndex () .removeInterest (&TextureMappingEditor::set_texCoordIndex, this);
			geometry -> coordIndex ()    .removeInterest (&TextureMappingEditor::set_coordIndex, this);
			geometry -> texCoord ()      .removeInterest (&TextureMappingEditor::set_texCoord, this);
			geometry -> coord ()         .removeInterest (&TextureMappingEditor::set_coord, this);
		}

		geometry = value;

		if (geometry)
		{
			coord           = geometry -> coord ();
			previewGeometry = X3D::X3DPtr <X3D::IndexedFaceSet> (geometry -> copy (rightShape -> getExecutionContext (), X3D::FLAT_COPY));

			previewGeometry -> setPrivate (true);
			previewGeometry -> texCoordIndex () .addInterest (&TextureMappingEditor::set_left_selected_faces, this);

			geometry -> solid ()           .addInterest (previewGeometry -> solid ());
			geometry -> convex ()          .addInterest (previewGeometry -> convex ());
			geometry -> ccw ()             .addInterest (previewGeometry -> ccw ());
			geometry -> creaseAngle ()     .addInterest (previewGeometry -> creaseAngle ());
			geometry -> normalPerVertex () .addInterest (previewGeometry -> normalPerVertex ());
			geometry -> colorIndex ()      .addInterest (previewGeometry -> colorIndex ());
			geometry -> normalIndex ()     .addInterest (previewGeometry -> normalIndex ());
			geometry -> coordIndex ()      .addInterest (previewGeometry -> coordIndex ());
			geometry -> color ()           .addInterest (previewGeometry -> color ());
			geometry -> normal ()          .addInterest (previewGeometry -> normal ());
			geometry -> coord ()           .addInterest (previewGeometry -> coord ());

			geometry -> texCoordIndex () .addInterest (&TextureMappingEditor::set_texCoordIndex, this);
			geometry -> coordIndex ()    .addInterest (&TextureMappingEditor::set_coordIndex, this);
			geometry -> texCoord ()      .addInterest (&TextureMappingEditor::set_texCoord, this);
			geometry -> coord ()         .addInterest (&TextureMappingEditor::set_coord, this);

			rightShape -> geometry () = previewGeometry;
			rightShape -> getExecutionContext () -> realize ();

			set_texCoordIndex ();
			set_texCoord (geometry -> texCoord ());

			rightSelectedGeometry  -> coordIndex () .clear ();
			rightSelectionGeometry -> coordIndex () .clear ();

			rightSelectedGeometry  -> coord () = coord;
			rightSelectionGeometry -> coord () = coord;

			rightSelection -> geometry () = geometry;

			getWidget () .set_sensitive (true);
		}
		else
		{
			rightSelection -> geometry () = nullptr;

			set_texCoord (nullptr);
			set_coord (nullptr);

			previewGeometry                    = nullptr;
			rightShape -> geometry ()          = nullptr;
			rightSelectedGeometry  -> coord () = nullptr;
			rightSelectionGeometry -> coord () = nullptr;

			getWidget () .set_sensitive (false);
		}

		right -> addEvent ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_texCoordIndex ()
{
	undoHistory .clear ();

	// Generate texCoordIndex.

	if (geometry -> texCoordIndex () .empty ())
		previewGeometry -> texCoordIndex () = geometry -> coordIndex ();
	else
	{
		previewGeometry -> texCoordIndex () = geometry -> texCoordIndex ();

		// Verify texCoordIndex.

		for (size_t i = 0, size = geometry -> coordIndex () .size (); i < size; ++ i)
		{
			if (geometry -> coordIndex () [i] < 0)
				previewGeometry -> texCoordIndex () .set1Value (i, -1);
		}
	}

	clear ();
}

void
TextureMappingEditor::set_coordIndex ()
{
	set_texCoordIndex ();
}

void
TextureMappingEditor::set_texCoord (const X3D::SFNode & value)
{
	undoHistory .clear ();

	if (multiTexCoord)
		multiTexCoord -> removeInterest (&TextureMappingEditor::set_texCoord, this);

	// Determine texCoord.

	X3D::X3DPtr <X3D::TextureCoordinate> texCoordNode;

	multiTexCoord = value;

	if (multiTexCoord)
	{
		multiTexCoord -> addInterest (&TextureMappingEditor::set_texCoord, this, value);

		if (multiTexCoord -> getTexCoord () .empty ())
			texCoordNode = nullptr;
		else
			texCoordNode = multiTexCoord -> getTexCoord () [std::min (stage, multiTexCoord -> getTexCoord () .size () - 1)];
	}
	else
		texCoordNode = value;

	if (texCoordNode or not previewGeometry)
	{
		// Create preview texCoord node.

		texCoord = right -> getExecutionContext () -> createNode <X3D::TextureCoordinate> ();

		if (texCoordNode)
			texCoord -> point () = texCoordNode -> point ();

		if (previewGeometry)
			previewGeometry -> texCoord () = texCoord;

		right -> getExecutionContext () -> realize ();
	}
	else
	{
		// Create default texCoord.

		previewGeometry -> addTexCoords ();

		texCoord = previewGeometry -> texCoord ();
	}

	texCoord -> point () .addInterest (&TextureMappingEditor::set_left_coord, this);

	clear ();
	set_left_coord ();

	left  -> addEvent ();
	right -> addEvent ();
}

void
TextureMappingEditor::on_texture_stage_changed ()
{
	stage = getTextureStageAdjustment () -> get_value () - 1;

	if (appearance)
	{
		set_texture (appearance -> texture ());
		set_textureTransform (appearance -> textureTransform ());
	}
	else
	{
		set_texture (nullptr);
		set_textureTransform (nullptr);
	}

	if (geometry)
		set_texCoord (geometry -> texCoord ());
	else
		set_texCoord (nullptr);
}

void
TextureMappingEditor::set_coord (const X3D::SFNode & value)
{
	coord = value;
}

void
TextureMappingEditor::clear ()
{
	selectedFaces .clear ();

	set_selected_faces ();
}

void
TextureMappingEditor::set_selected_faces ()
{
	set_left_selected_faces ();
	set_right_selected_faces ();

	const bool haveSelection = not selectedFaces .empty ();

	getMappingsMenuItem ()            .set_sensitive (haveSelection);
	getRotateCounterlockwiseButton () .set_sensitive (haveSelection);
	getRotateClockwiseButton ()       .set_sensitive (haveSelection);
	getFlipButton ()                  .set_sensitive (haveSelection);
	getFlopButton ()                  .set_sensitive (haveSelection);
}

void
TextureMappingEditor::set_left_coord ()
{
	try
	{
		const auto selectedCoord = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");

		size_t i = 0;

		selectedCoord -> point () .resize (texCoord -> point () .size ());

		for (const auto & point : basic::make_const_range (texCoord -> point ()))
			selectedCoord -> point () [i ++] = X3D::Vector3f (point .x (), point .y (), 0);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_left_image ()
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto imageTexCoord    = left -> getExecutionContext () -> getNamedNode <X3D::TextureCoordinate> ("ImageTextureCoordinate");
		const auto imageCoord       = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("ImageCoordinate");

		auto bbox = selectedGeometry -> getBBox ();

		if (bbox .empty ())
			bbox = X3D::Box3d (X3D::Vector3d (1, 1, 0), X3D::Vector3d (0.5, 0.5, 0));

		auto   extents = bbox .extents ();
		auto & min     = extents .first;
		auto & max     = extents .second;

		min .x (std::floor (min .x () - 0.5));
		min .y (std::floor (min .y () - 0.5));
		max .x (std::ceil  (max .x () + 0.5));
		max .y (std::ceil  (max .y () + 0.5));

		imageTexCoord -> point () = {
			X3D::Vector2f (min .x (), min .y ()),
			X3D::Vector2f (max .x (), min .y ()),
			X3D::Vector2f (max .x (), max .y ()),
			X3D::Vector2f (min .x (), max .y ())
		};

		imageCoord -> point () = {
			X3D::Vector3f (min .x (), min .y (), 0),
			X3D::Vector3f (max .x (), min .y (), 0),
			X3D::Vector3f (max .x (), max .y (), 0),
			X3D::Vector3f (min .x (), max .y (), 0)
		};
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_left_selected_faces ()
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");

		selectedGeometry -> coordIndex () .clear ();

		for (const auto & face : selectedFaces)
		{
			const auto vertices = rightSelection -> getFaceVertices (face);

			if (vertices .size () < 3)
				continue;

			const auto first = selectedGeometry -> coordIndex () .size ();

			for (const auto & i : vertices)
				selectedGeometry -> coordIndex () .emplace_back (previewGeometry -> texCoordIndex () .get1Value (i));

			selectedGeometry -> coordIndex () .emplace_back (selectedGeometry -> coordIndex () [first]);
			selectedGeometry -> coordIndex () .emplace_back (-1);
		}

		// Remove point from selection not in new indices anymore.

		std::set <int32_t> indices (selectedGeometry -> coordIndex () .begin (), selectedGeometry -> coordIndex () .end ());

		std::vector <int32_t> difference;

		std::set_difference (selectedPoints .begin (), selectedPoints .end (),
		                     indices .begin (), indices .end (),
		                     std::back_inserter (difference));

		for (const auto & index : difference)
			selectedPoints .erase (index);

		set_selectedPoints ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_left_active (const bool value)
{
	using set1Value = void (X3D::MFVec2f::*) (const X3D::MFVec2f::size_type, const X3D::MFVec2f::value_type &);

	if (value)
	{
		set_startDrag ();

		// Init undo step.

		switch (tool)
		{
			case ToolType::MOVE:
			{
				undoStep = std::make_shared <X3D::UndoStep> (_ ("Move Points"));
				break;
			}
			case ToolType::ROTATE:
			{
				undoStep = std::make_shared <X3D::UndoStep> (_ ("Rotate Points"));
				break;
			}
			case ToolType::SCALE:
			{
				undoStep = std::make_shared <X3D::UndoStep> (_ ("Scale Points"));
				break;
			}
		}

		undoStep -> addObjects (texCoord);

		for (const auto & index : selectedPoints)
			undoStep -> addUndoFunction ((set1Value) &X3D::MFVec2f::set1Value, std::ref (texCoord -> point ()), index, texCoord -> point () .get1Value (index));
	}
	else
	{
		// Drag has started.
		if (startHitPoint == infinity2f)
		{
			for (const auto & index : selectedPoints)
				undoStep -> addRedoFunction ((set1Value) &X3D::MFVec2f::set1Value, std::ref (texCoord -> point ()), index, texCoord -> point () .get1Value (index));

			addUndoStep (undoStep);
		}
	}
}

void
TextureMappingEditor::set_startDrag ()
{
	try
	{
		if ((size_t) activePoint >= texCoord -> point () .size ())
			return;

		// Move

		const auto          touchSensor = left -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const X3D::Vector3f hitPoint    = touchSensor -> hitPoint_changed ();

		startHitPoint = projectPoint (hitPoint, left);
		startPosition = texCoord -> point () .get1Value (activePoint);
		pointOffset   = startPosition - X3D::Vector2f (hitPoint .x (), hitPoint .y ());

		startPositions .clear ();

		for (const auto & index : selectedPoints)
			startPositions .emplace_back (index, texCoord -> point () .get1Value (index));

		// Rotate and Scale

		const auto centerPoint = left -> getExecutionContext () -> getNamedNode <X3D::Transform> ("CenterPoint");
		const auto center      = X3D::Vector2f (centerPoint -> translation () .getX (), centerPoint -> translation () .getY ());
		const auto point       = X3D::Vector2f (hitPoint .x (), hitPoint .y ());

		startDistance = point - center;

		// Clear selection.

		if (keys .shift ())
			return;

		if (selectedPoints .count (activePoint))
			return;

		selectedPoints = { activePoint };
		startPositions = { std::pair (activePoint, startPosition) };

		set_selectedPoints ();
	}
	catch (const std::exception &)
	{ }
}

void
TextureMappingEditor::set_left_touchTime ()
{
	try
	{
		if (startHitPoint == infinity2f)
		{
			startHitPoint = X3D::Vector2d ();
			return;
		}

		if (not keys .shift ())
			return;

		if ((size_t) activePoint >= texCoord -> point () .size ())
			return;

		const auto selectedPoint = selectedPoints .find (activePoint);

		if (selectedPoint == selectedPoints .end ())
			selectedPoints .emplace (activePoint);
		else
			selectedPoints .erase (selectedPoint);

		masterPoint = activePoint;

		set_selectedPoints ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_left_hitPoint (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto touchSensor = left -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

		if ((not keys .shift () or startHitPoint == infinity2f) and touchSensor -> isActive ())
		{
			if ((size_t) activePoint < texCoord -> point () .size ())
			{
				// Prevent accidentialy moving points.
				if (math::abs (startHitPoint - projectPoint (hitPoint, left)) > POINT_SIZE)
				{
					startHitPoint = infinity2f; // Drag has started.

					switch (tool)
					{
						case ToolType::MOVE:
						{
							move (hitPoint);
							break;
						}
						case ToolType::ROTATE:
						{
							rotate (hitPoint);
							break;
						}
						case ToolType::SCALE:
						{
							scale (hitPoint);
							break;
						}
					}
				}
			}
		}
		else
			set_left_point (hitPoint);
	}
	catch (const std::exception &)
	{ }
}

void
TextureMappingEditor::move (const X3D::Vector3f & hitPoint)
{
	const auto point       = X3D::Vector2f (hitPoint .x (), hitPoint .y ()) + pointOffset;
	auto       translation = point - startPosition;

	if (keys .control ())
	{
		if (std::abs (translation .x ()) > std::abs (translation .y ()))
			translation .y (0);
		else
			translation .x (0);
	}

	for (const auto & pair : startPositions)
		texCoord -> point () .set1Value (pair .first, pair .second + translation);
}

void
TextureMappingEditor::rotate (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto centerPoint = left -> getExecutionContext () -> getNamedNode <X3D::Transform> ("CenterPoint");
		const auto center      = X3D::Vector2f (centerPoint -> translation () .getX (), centerPoint -> translation () .getY ());
		const auto point       = X3D::Vector2f (hitPoint .x (), hitPoint .y ());
		const auto distance    = point - center;
		auto       rotation    = std::atan2 (startDistance .x (), startDistance .y ()) - std::atan2 (distance .x (), distance .y ());

		if (keys .control ())
		{
			constexpr double snapAngle = 11.25 / 180 * pi <double>;

			rotation = std::round (rotation / snapAngle) * snapAngle;
		}

		X3D::Matrix3f matrix;
		matrix .rotate (rotation);

		for (const auto & pair : startPositions)
			texCoord -> point () .set1Value (pair .first, (pair .second - center) * matrix + center);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::scale (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto centerPoint = left -> getExecutionContext () -> getNamedNode <X3D::Transform> ("CenterPoint");
		const auto center      = X3D::Vector2f (centerPoint -> translation () .getX (), centerPoint -> translation () .getY ());
		const auto point       = X3D::Vector2f (hitPoint .x (), hitPoint .y ());
		const auto distance    = point - center;

		if (startDistance .x () == 0 or startDistance .y () == 0)
			return;

		if (distance .x () == 0 or distance .y () == 0)
			return;

		X3D::Matrix3f matrix;

		if (keys .control ())
		{
			const float sgn    = math::dot (startDistance, distance) > 0 ? 1 : -1;
			const auto  length = sgn * math::abs (distance) / math::abs (startDistance);

			if (length == 0)
				return;

			matrix .scale (X3D::Vector2f (length, length));
		}
		else
			matrix .scale (distance / startDistance);

		for (const auto & pair : startPositions)
			texCoord -> point () .set1Value (pair .first, (pair .second - center) * matrix + center);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_left_point (const X3D::Vector3d & hitPoint)
{
	try
	{
		const auto selectedCoord  = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");
		const auto activePointSet = left -> getExecutionContext () -> getNamedNode ("ActivePointSet");

		activePoint = getNearestPoint (hitPoint);

		if (activePoint < 0)
			activePointSet -> getField <X3D::MFInt32> ("set_coordIndex") .clear ();

		else
		{
			const auto point    = selectedCoord -> get1Point (activePoint);
			const auto p        = projectPoint (point, left);    // point in pixel coordinates.
			const auto h        = projectPoint (hitPoint, left); // hitPoint in pixel coordinates.
			const auto distance = math::abs (h - p);

			if (distance < POINT_SIZE)
				activePointSet -> getField <X3D::MFInt32> ("set_coordIndex") = { activePoint };
			else
			{
				activePoint = -1;
				activePointSet -> getField <X3D::MFInt32> ("set_coordIndex") .clear ();
			}
		}
	}
	catch (const std::exception &)
	{ }
}

int32_t
TextureMappingEditor::getNearestPoint (const X3D::Vector3d & hitPoint) const
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto selectedCoord    = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");

		// Determine activePoint of nearest point to hitPoint.

		if (selectedGeometry -> coordIndex () .empty ())
			return -1;

		if (selectedCoord -> point () .empty ())
			return -1;

		double  minDistance = std::numeric_limits <double>::infinity ();
		int32_t minIndex    = -1;

		for (const auto & index : selectedGeometry -> coordIndex ())
		{
			if (index >= 0)
			{
				const auto distance = math::abs (hitPoint - selectedCoord -> get1Point (index));
			
				if (distance < minDistance)
				{
					minDistance = distance;
					minIndex    = index;
				}
			}
		}

		return minIndex;
	}
	catch (const X3D::X3DError &)
	{
		return -1;
	}
}

void
TextureMappingEditor::set_selectedPoints ()
{
	try
	{
		const auto selectedPointSet = left -> getExecutionContext () -> getNamedNode ("SelectedPointSet");

		selectedPointSet -> getField <X3D::MFInt32> ("set_coordIndex") .assign (selectedPoints .begin (), selectedPoints .end ());

		getMergePointsButton () .set_sensitive (selectedPoints .size () > 1);
		getSplitPointButton ()  .set_sensitive (not selectedPoints .empty ());
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_left_center (const X3D::Vector3f & value)
{
	try
	{
		auto       translation = value;
		const auto centerPoint = left -> getExecutionContext () -> getNamedNode <X3D::Transform> ("CenterPoint");

		if (getSnapCenterButton () .get_active ())
		{
			const auto selectedCoord = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");
			const auto index         = getNearestPoint (translation);
			
			if (index >= 0)
			{
				const auto nearestPoint = selectedCoord -> get1Point (index);
				const auto n            = projectPoint (nearestPoint, left);
				const auto t            = projectPoint (translation, left);
				const auto distance     = math::abs (t - n);

				if (distance < SNAP_DISTANCE)
					translation = nearestPoint;
			}
		}

		centerPoint -> translation () = translation;
	}
	catch (const std::exception &)
	{ }
}

void
TextureMappingEditor::set_right_selection (const X3D::Vector3d & hitPoint, const std::vector <int32_t> & coincidentPoints)
{
	try
	{
		const auto selectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");
		const auto adjacentFaces     = rightSelection -> getAdjacentFaces (coincidentPoints);
		const auto nearestFace       = rightSelection -> getClosestFace (hitPoint, adjacentFaces);
		const auto vertices          = rightSelection -> getFaceVertices (nearestFace .index);

		if (vertices .size () < 3)
			return;

		selectionGeometry -> coordIndex () .clear ();

		for (const auto & i : vertices)
			selectionGeometry -> coordIndex () .emplace_back (previewGeometry -> coordIndex () [i]);

		selectionGeometry -> coordIndex () .emplace_back (selectionGeometry -> coordIndex () .front ());
		selectionGeometry -> coordIndex () .emplace_back (-1);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_right_active (const bool value)
{
	if (value)
		rightPaintSelecion = false;
}

void
TextureMappingEditor::set_right_touchTime ()
{
	const auto   touchSensor      = right -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
	const auto   coincidentPoints = rightSelection -> getCoincidentPoints (touchSensor -> getClosestPoint ());
	const auto   adjacentFaces    = rightSelection -> getAdjacentFaces (coincidentPoints);
	const auto   nearestFace      = rightSelection -> getClosestFace (touchSensor -> hitPoint_changed () .getValue (), adjacentFaces);

	if (coincidentPoints .empty ())
		return;

	if (not keys .shift () and not keys .control () and not rightPaintSelecion)
		selectedFaces .clear ();

	if (keys .control ())
		selectedFaces .erase (nearestFace .index);

	else if (not selectedFaces .emplace (nearestFace .index) .second)
		return;

	set_selected_faces ();
}

void
TextureMappingEditor::set_right_selected_faces ()
{
	try
	{
		const auto selectedGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");

		selectedGeometry -> coordIndex () .clear ();

		for (const auto & face : selectedFaces)
		{
			const auto vertices = rightSelection -> getFaceVertices (face);

			if (vertices .size () < 3)
				continue;

			const auto first = selectedGeometry -> coordIndex () .size ();

			for (const auto & i : vertices)
				selectedGeometry -> coordIndex () .emplace_back (previewGeometry -> coordIndex () [i]);

			selectedGeometry -> coordIndex () .emplace_back (selectedGeometry -> coordIndex () [first]);
			selectedGeometry -> coordIndex () .emplace_back (-1);
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureMappingEditor::set_right_hitPoint ()
{
	try
	{
		// Determine face and faces

		const auto touchSensor      = right -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const auto coincidentPoints = rightSelection -> getCoincidentPoints (touchSensor -> getClosestPoint ());

		if (coincidentPoints .empty ())
			return;

		// Setup cross hair

		set_right_selection (touchSensor -> getHitPoint (), coincidentPoints);

		if (touchSensor -> isActive () and (keys .shift () or keys .control ()))
		{
			rightPaintSelecion = true;
			set_right_touchTime ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

///  throws std::runtime_error
X3D::Vector2d
TextureMappingEditor::projectPoint (const X3D::Vector3d & point, const X3D::BrowserPtr & browser) const
{
	const auto & activeLayer = browser -> getActiveLayer ();

	if (not activeLayer)
		throw std::runtime_error ("No active layer.");

	const auto viewport   = activeLayer -> getViewport () -> getRectangle (browser);
	const auto projection = activeLayer -> getViewpoint () -> getProjectionMatrix (0, -10, viewport);
	const auto modelview  = X3D::Matrix4d ();
	const auto p          = X3D::ViewVolume::projectPoint (point, modelview, projection, viewport);

	return X3D::Vector2d (p. x (), p .y ());
}

TextureMappingEditor::~TextureMappingEditor ()
{
	undoHistory .clear ();
	dispose ();
}

} // puck
} // titania

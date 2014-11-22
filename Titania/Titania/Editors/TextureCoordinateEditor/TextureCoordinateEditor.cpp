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

#include "TextureCoordinateEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../FaceSelection.h"

#include <complex>

namespace titania {
namespace puck {

constexpr double POINT_SIZE    = 6; // Use linewidthScaleFactor / 2 + 2.
constexpr double SNAP_DISTANCE = POINT_SIZE;

constexpr X3D::Vector2d
infinity2f (std::numeric_limits <float>::infinity (), std::numeric_limits <float>::infinity ());

TextureCoordinateEditor::TextureCoordinateEditor (X3DBrowserWindow* const browserWindow) :
	                   X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DTextureCoordinateEditorInterface (get_ui ("Editors/TextureCoordinateEditor.xml"), gconf_dir ()),
	                               left (X3D::createBrowser (getBrowserWindow () -> getBrowser ())),
	                              right (X3D::createBrowser (getBrowserWindow () -> getBrowser ())),
	                        initialized (0),
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
	                              stage (0),
	                               tool (ToolType::MOVE),
	                     rightSelection (new FaceSelection ()),
	                 rightPaintSelecion (false),
	                      selectedFaces (),
	                        activePoint (-1),
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
	left  -> set_antialiasing (4);
	right -> set_antialiasing (4);

	setup ();
}

void
TextureCoordinateEditor::initialize ()
{
	X3DTextureCoordinateEditorInterface::initialize ();

	getLeftBox ()  .pack_start (*left, true, true, 0);
	getRightBox () .pack_start (*right, true, true, 0);

	left  -> show ();
	right -> show ();
	left  -> initialized () .addInterest (this, &TextureCoordinateEditor::set_left_initialized);
	right -> initialized () .addInterest (this, &TextureCoordinateEditor::set_right_initialized);

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &TextureCoordinateEditor::set_selection);

	undoHistory .addInterest (this, &TextureCoordinateEditor::set_undoHistory);
}

void
TextureCoordinateEditor::set_left_initialized ()
{
	++ initialized;

	left -> initialized () .removeInterest (this, &TextureCoordinateEditor::set_left_initialized);

	try
	{
		left -> loadURL ({ get_ui ("Editors/TextureCoordinateEditorLeftPreview.x3dv") });

		set_initialized ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_right_initialized ()
{
	++ initialized;

	right -> initialized () .removeInterest (this, &TextureCoordinateEditor::set_right_initialized);

	try
	{
		right -> loadURL ({ get_ui ("Editors/TextureCoordinateEditorRightPreview.x3dv") });

		set_initialized ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_initialized ()
{
	if (initialized not_eq 2)
		return;

	try
	{
		const auto appearance       = left -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto touchSensor      = left -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto centerSensor     = left -> getExecutionContext () -> getNamedNode <X3D::PlaneSensor> ("CenterSensor");

		touchSensor -> isActive ()             .addInterest (this, &TextureCoordinateEditor::set_left_active);
		touchSensor -> touchTime ()            .addInterest (this, &TextureCoordinateEditor::set_left_touchTime);
		touchSensor -> hitPoint_changed ()     .addInterest (this, &TextureCoordinateEditor::set_left_hitPoint);
		selectedGeometry ->                     addInterest (this, &TextureCoordinateEditor::set_left_image);
		centerSensor -> translation_changed () .addInterest (this, &TextureCoordinateEditor::set_left_center);

		appearance -> isPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	try
	{
		const auto shape             = right -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto appearance        = right -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");
		const auto touchSensor       = right -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");
		const auto selectedGeometry  = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto selectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");

		shape -> geometry ()               .addInterest (this, &TextureCoordinateEditor::set_right_viewer);
		touchSensor -> isActive ()         .addInterest (this, &TextureCoordinateEditor::set_right_active);
		touchSensor -> touchTime ()        .addInterest (this, &TextureCoordinateEditor::set_right_touchTime);
		touchSensor -> hitPoint_changed () .addInterest (this, &TextureCoordinateEditor::set_right_hitPoint);

		appearance        -> isPrivate (true);
		selectedGeometry  -> isPrivate (true);
		selectionGeometry -> isPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	configure ();
	set_selection ();
}

void
TextureCoordinateEditor::configure ()
{
	getSnapCenterButton () .set_active (getConfig () .getBoolean ("snapCenter"));
}

void
TextureCoordinateEditor::set_selection ()
{
	undoHistory .clear ();

	try
	{
		const auto shapes = getSelection <X3D::X3DShapeNode> ({ X3D::X3DConstants::X3DShapeNode });

		if (shapes .empty ())
			set_shape (nullptr);

		else
			set_shape (shapes .back ());
	}
	catch (const X3D::X3DError &)
	{ }
}

// Keyboard

bool
TextureCoordinateEditor::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);
	return false;
}

bool
TextureCoordinateEditor::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);
	return false;
}

// Edit

void
TextureCoordinateEditor::on_undo ()
{
	undoHistory .undoChanges ();
}

void
TextureCoordinateEditor::on_redo ()
{
	undoHistory .redoChanges ();
}

void
TextureCoordinateEditor::set_undoHistory ()
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

void
TextureCoordinateEditor::on_remove_unused_texCoord_activate ()
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
			texCoords .emplace_back (texCoord -> point () [index]);
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
		return;

	previewGeometry -> texCoordIndex () = std::move (texCoordIndex);
	texCoord -> point () = std::move (texCoords);
}

// Mappings

void
TextureCoordinateEditor::on_x_plane_activate ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("X-Plane Mapping"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox = getBBox (2, 1);

	X3D::Vector2f min, max;
	bbox .extents (min, max);

	const auto size = bbox .size ();
	const auto m    = size .x () >= size .y () ? 0 : 1;

	// Apply mapping.
	
	std::map <int32_t, std::vector <size_t>> indices;
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
	}

	for (const auto pair : indices)
	{
		const auto point    = coord -> get1Point (pair .first);
		const auto texPoint = (X3D::Vector2f (point .z (), point .y ()) - min) / size [m];

		for (const auto & vertex : pair .second)
			previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

		texCoord -> point () .emplace_back (texPoint);
	}

	on_remove_unused_texCoord_activate ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureCoordinateEditor::on_y_plane_activate ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("X-Plane Mapping"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox = getBBox (0, 2);

	X3D::Vector2f min, max;
	bbox .extents (min, max);

	const auto size = bbox .size ();
	const auto m    = size .x () >= size .y () ? 0 : 1;

	// Apply mapping.
	
	std::map <int32_t, std::vector <size_t>> indices;
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
	}

	for (const auto pair : indices)
	{
		const auto point    = coord -> get1Point (pair .first);
		const auto texPoint = X3D::Vector2f (point .x () - min .x (), -point .z () + max .y ()) / size [m];

		for (const auto & vertex : pair .second)
			previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

		texCoord -> point () .emplace_back (texPoint);
	}

	on_remove_unused_texCoord_activate ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureCoordinateEditor::on_z_plane_activate ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("X-Plane Mapping"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox = getBBox (0, 1);

	X3D::Vector2f min, max;
	bbox .extents (min, max);

	const auto size = bbox .size ();
	const auto m    = size .x () >= size .y () ? 0 : 1;

	// Apply mapping.
	
	std::map <int32_t, std::vector <size_t>> indices;
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
	}

	for (const auto pair : indices)
	{
		const auto point    = coord -> get1Point (pair .first);
		const auto texPoint = (X3D::Vector2f (point .x (), point .y ()) - min) / size [m];

		for (const auto & vertex : pair .second)
			previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

		texCoord -> point () .emplace_back (texPoint);
	}

	on_remove_unused_texCoord_activate ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureCoordinateEditor::on_camera_activate ()
{
	try
	{
		const auto & activeLayer = right -> getActiveLayer ();

		if (not activeLayer)
			return;

		const auto undoStep = std::make_shared <UndoStep> (_ ("Camera Mapping"));

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
			for (const auto & vertex : rightSelection -> getVertices (face))
				indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
		}

		for (const auto pair : indices)
		{
			const auto point    = coord -> get1Point (pair .first);
			const auto texPoint = X3D::ViewVolume::projectPoint (point, modelview, projection, viewport);

			for (const auto & vertex : pair .second)
				previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

			texCoord -> point () .emplace_back (texPoint .x (), texPoint .y ());
		}

		on_remove_unused_texCoord_activate ();

		undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
		undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

		addUndoStep (undoStep);
	}
	catch (const std::domain_error &)
	{ }
}

void
TextureCoordinateEditor::on_box_activate ()
{
	__LOG__ << std::endl;
}

void
TextureCoordinateEditor::on_cylinder_activate ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("Cylinder Mapping"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox = getBBox (0, 1);

	X3D::Vector2f min, max;
	bbox .extents (min, max);

	const auto size = bbox .size ();

	// Apply mapping.
	
	std::map <int32_t, std::vector <size_t>> indices;
	
	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			indices [previewGeometry -> coordIndex () [vertex]] .emplace_back (vertex);
	}

	for (const auto pair : indices)
	{
		const auto point    = coord -> get1Point (pair .first);
		const auto complex  = std::complex <double> (point .z (), point .x ());
		const auto texPoint = X3D::Vector2f (std::arg (complex) / (M_PI * 2) + 0.5, (point .y () - min .y ()) / size .y ());

		for (const auto & vertex : pair .second)
			previewGeometry -> texCoordIndex () [vertex] = texCoord -> point () .size ();

		texCoord -> point () .emplace_back (texPoint);
	}
	
	resolveOverlaps ();

	on_remove_unused_texCoord_activate ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (previewGeometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureCoordinateEditor::on_sphere_activate ()
{
	__LOG__ << std::endl;
}

void
TextureCoordinateEditor::resolveOverlaps ()
{
	// Resolve overlaps.

	constexpr double OVERLAP_THRESHOLD = 0.5;

	for (const auto & face : selectedFaces)
	{
		const auto vertices = rightSelection -> getVertices (face);
		
		if (vertices .empty ())
			continue;

		const auto          index1 = previewGeometry -> texCoordIndex () [vertices [0]];
		const X3D::Vector2f point1 = texCoord -> point () [index1];

		for (size_t i = 1, size = vertices .size (); i < size; ++ i)
		{
			const auto          index2   = previewGeometry -> texCoordIndex () [vertices [i]];
			const X3D::Vector2f point2   = texCoord -> point () [index2];
			const auto          distance = point1 .x () - point2 .x ();
	
			if (distance > OVERLAP_THRESHOLD)
			{
				previewGeometry -> texCoordIndex () [vertices [i]] = texCoord -> point () .size ();
				texCoord -> point () .emplace_back (point2 .x () + 1, point2 .y ());
			}
			else if (distance < -OVERLAP_THRESHOLD)
			{
				previewGeometry -> texCoordIndex () [vertices [i]] = texCoord -> point () .size ();
				texCoord -> point () .emplace_back (point2 .x () - 1, point2 .y ());
			}
		}
	}
}

X3D::Box2f
TextureCoordinateEditor::getTexBBox () const
{
	// Determine bbox extents.

	std::vector <X3D::Vector2f> points;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			points .emplace_back (texCoord -> point () .get1Value (previewGeometry -> texCoordIndex () [vertex]));
	}

	return X3D::Box2f (points .begin (), points .end (), math::iterator_type ());
}

X3D::Box2f
TextureCoordinateEditor::getBBox (const size_t i1, const size_t i2) const
{
	// Determine bbox extents.

	std::vector <X3D::Vector2f> points;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
		{
			const auto point = coord -> get1Point (previewGeometry -> coordIndex () [vertex]);
			points .emplace_back (point [i1], point [i2]);
		}
	}

	return X3D::Box2f (points .begin (), points .end (), math::iterator_type ());
}

// Selection

void
TextureCoordinateEditor::on_select_all_activate ()
{
	if (left -> has_focus ())
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
		catch (const X3D::X3DError &)
		{ }
	}

	else if (right -> has_focus ())
	{
		selectedFaces = rightSelection -> getFaces ();

		set_selected_faces ();
	}
}

void
TextureCoordinateEditor::on_deselect_all_activate ()
{
	if (left -> has_focus ())
	{
		selectedPoints .clear ();

		set_selectedPoints ();
	}

	else if (right -> has_focus ())
	{
		selectedFaces .clear ();

		set_selected_faces ();
	}
}

// Toolbar

void
TextureCoordinateEditor::on_rotate_counterclockwise ()
{
	on_rotate (_ ("Rotate 90° Counterclockwise"), M_PI / 2);
}

void
TextureCoordinateEditor::on_rotate_clockwise ()
{
	on_rotate (_ ("Rotate 90° Clockwise"), -M_PI / 2);
}

void
TextureCoordinateEditor::on_rotate (const std::string & description, const double angle)
{
	const auto undoStep = std::make_shared <UndoStep> (description);

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox     = getTexBBox ();
	const auto center   = bbox .center ();
	const auto rotation = X3D::Matrix3f (angle);

	// Apply mapping.

	std::set <int32_t> vertices;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			vertices .emplace (previewGeometry -> texCoordIndex () .get1Value (vertex));
	}

	for (const auto & vertex : vertices)
	{
		auto &     point   = texCoord -> point () .get1Value (vertex);
		const auto rotated = (point .getValue () - center) * rotation + center;
		point .setValue (rotated);
	}

	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}


void
TextureCoordinateEditor::on_flip ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("Flip Horizontally"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox    = getTexBBox ();
	const auto center2 = 2 * bbox .center () .x ();

	// Apply mapping.

	std::set <int32_t> vertices;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			vertices .emplace (previewGeometry -> texCoordIndex () .get1Value (vertex));
	}

	for (const auto & vertex : vertices)
	{
		auto &     point = texCoord -> point () .get1Value (vertex);
		const auto x     = point .getX ();
		point .setX (center2 - x);
	}

	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

void
TextureCoordinateEditor::on_flop ()
{
	const auto undoStep = std::make_shared <UndoStep> (_ ("Flip Vertically"));

	undoStep -> addObjects (previewGeometry, texCoord);
	undoStep -> addUndoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	// Determine bbox extents.

	const auto bbox    = getTexBBox ();
	const auto center2 = 2 * bbox .center () .y ();

	// Apply mapping.

	std::set <int32_t> vertices;

	for (const auto & face : selectedFaces)
	{
		for (const auto & vertex : rightSelection -> getVertices (face))
			vertices .emplace (previewGeometry -> texCoordIndex () .get1Value (vertex));
	}

	for (const auto & vertex : vertices)
	{
		auto &     point = texCoord -> point () .get1Value (vertex);
		const auto y     = point .getY ();
		point .setY (center2 - y);
	}

	undoStep -> addRedoFunction (&X3D::MFVec2f::setValue, std::ref (texCoord -> point ()), texCoord -> point ());

	addUndoStep (undoStep);
}

// Dashboard

void
TextureCoordinateEditor::on_left_hand_toggled ()
{
	left -> isPickable (false);
	left -> grab_focus ();
}

void
TextureCoordinateEditor::on_left_arrow_toggled ()
{
	tool = ToolType::MOVE;
	left -> isPickable (true);
	left -> grab_focus ();
}

void
TextureCoordinateEditor::on_left_rotate_toggled ()
{
	tool = ToolType::ROTATE;
	left -> isPickable (true);
	left -> grab_focus ();
}

void
TextureCoordinateEditor::on_left_scale_toggled ()
{
	tool = ToolType::SCALE;
	left -> isPickable (true);
	left -> grab_focus ();
}

void
TextureCoordinateEditor::on_left_snap_center_toggled ()
{
	getConfig () .setItem ("snapCenter", getSnapCenterButton () .get_active ());
}

void
TextureCoordinateEditor::on_left_look_at_all_clicked ()
{
	if (left -> getActiveLayer ())
		left -> getActiveLayer () -> lookAt ();

	left -> grab_focus ();
}

void
TextureCoordinateEditor::on_right_hand_toggled ()
{
	right -> isPickable (false);
	right -> grab_focus ();
}

void
TextureCoordinateEditor::on_right_arrow_toggled ()
{
	right -> isPickable (true);
	right -> grab_focus ();
}

void
TextureCoordinateEditor::on_right_look_at_all_clicked ()
{
	if (right -> getActiveLayer ())
		right -> getActiveLayer () -> lookAt ();

	right -> grab_focus ();
}

void
TextureCoordinateEditor::on_right_look_at_toggled ()
{
	if (getRightLookAtButton () .get_active ())
	{
		if (right -> getViewer () not_eq X3D::ViewerType::LOOKAT)
			right -> setViewer (X3D::ViewerType::LOOKAT);
	}
	else
	{
		if (right -> getViewer () not_eq X3D::ViewerType::EXAMINE)
			right -> setViewer (X3D::ViewerType::EXAMINE);
	}

	right -> grab_focus ();
}

void
TextureCoordinateEditor::on_remove_clicked ()
{ }

void
TextureCoordinateEditor::on_apply_clicked ()
{
	geometry -> texCoordIndex () .removeInterest (this, &TextureCoordinateEditor::set_texCoordIndex);
	geometry -> texCoordIndex () .addInterest (this, &TextureCoordinateEditor::connectTexCoordIndex);
	geometry -> texCoord ()      .removeInterest (this, &TextureCoordinateEditor::set_texCoord);
	geometry -> texCoord ()      .addInterest (this, &TextureCoordinateEditor::connectTexCoord);

	if (multiTexCoord)
	{
		multiTexCoord -> removeInterest (this, &TextureCoordinateEditor::set_texCoord);
		multiTexCoord -> addInterest (this, &TextureCoordinateEditor::connectMultiTexCoord);
	}

	const auto undoStep = std::make_shared <UndoStep> (_ ("Apply Texture Coordinates"));

	undoStep -> addObjects (geometry);

	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), geometry -> texCoordIndex ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), previewGeometry -> texCoordIndex ());
	geometry -> texCoordIndex () = previewGeometry -> texCoordIndex ();

	// Replace texCoord or multiTexCoord.

	const auto texCoordNode = texCoord -> copy (geometry -> getExecutionContext (), X3D::FLAT_COPY);
	const X3D::X3DPtr <X3D::MultiTextureCoordinate> multiTextureCoordinate (geometry -> texCoord ());

	if (multiTextureCoordinate)
	{
		X3D::MFNode texCoords (multiTextureCoordinate -> getTexCoord () .begin (), multiTextureCoordinate -> getTexCoord () .end ());

		if (texCoords .empty ())
		{
			// XXX: Fill with default texture coordinates.
			for (size_t i = 0, size = stage; i < size; ++ i)
			 	texCoords .emplace_back (texCoordNode);
		}
		else
		{
			for (size_t i = texCoords .size () - 1, size = stage; i < size; ++ i)
				texCoords .emplace_back (texCoords .back ());
		}

		texCoords .set1Value (stage, texCoordNode);

		getBrowserWindow () -> replaceNodes (X3D::SFNode (multiTextureCoordinate), multiTextureCoordinate -> texCoord (), texCoords, undoStep);
	}
	else
		getBrowserWindow () -> replaceNode (X3D::SFNode (geometry), geometry -> texCoord (), X3D::SFNode (texCoordNode), undoStep);

	geometry -> getExecutionContext () -> realize ();

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
TextureCoordinateEditor::connectTexCoordIndex ()
{
	geometry -> texCoordIndex () .removeInterest (this, &TextureCoordinateEditor::connectTexCoordIndex);
	geometry -> texCoordIndex () .addInterest (this, &TextureCoordinateEditor::set_texCoordIndex);
}

void
TextureCoordinateEditor::connectTexCoord ()
{
	geometry -> texCoord () .removeInterest (this, &TextureCoordinateEditor::connectTexCoord);
	geometry -> texCoord () .addInterest (this, &TextureCoordinateEditor::set_texCoord);
}

void
TextureCoordinateEditor::connectMultiTexCoord ()
{
	multiTexCoord -> removeInterest (this, &TextureCoordinateEditor::connectMultiTexCoord);
	multiTexCoord -> addInterest (this, &TextureCoordinateEditor::set_texCoord, X3D::SFNode (multiTexCoord));
}

void
TextureCoordinateEditor::set_left_viewer ()
{
	if (left -> getActiveLayer ())
	{
		left -> getActiveLayer () -> getViewpoint () -> resetUserOffsets ();
		left -> getActiveLayer () -> lookAt ();
	}
}

void
TextureCoordinateEditor::set_right_viewer ()
{
	if (right -> getActiveLayer ())
	{
		right -> getActiveLayer () -> getViewpoint () -> resetUserOffsets ();
		right -> getActiveLayer () -> lookAt ();
	}
}

void
TextureCoordinateEditor::set_shape (const X3D::X3DPtr <X3D::X3DShapeNode> & value)
{
	if (shape)
	{
		shape -> appearance () .removeInterest (this, &TextureCoordinateEditor::set_appearance);
		shape -> geometry ()   .removeInterest (this, &TextureCoordinateEditor::set_geometry);
	}

	shape = value;

	if (shape)
	{
		shape -> appearance () .addInterest (this, &TextureCoordinateEditor::set_appearance);
		shape -> geometry ()   .addInterest (this, &TextureCoordinateEditor::set_geometry);

		set_appearance (shape -> appearance ());
		set_geometry (shape -> geometry ());
	}
	else
	{
		set_appearance (nullptr);
		set_geometry (nullptr);
	}
}

void
TextureCoordinateEditor::set_appearance (const X3D::SFNode & value)
{
	if (appearance)
	{
		appearance -> material ()         .removeInterest (this, &TextureCoordinateEditor::set_material);
		appearance -> texture ()          .removeInterest (this, &TextureCoordinateEditor::set_texture_stage);
		appearance -> textureTransform () .removeInterest (this, &TextureCoordinateEditor::set_textureTransform);
	}

	appearance = value;

	if (appearance)
	{
		appearance -> material ()         .addInterest (this, &TextureCoordinateEditor::set_material);
		appearance -> texture ()          .addInterest (this, &TextureCoordinateEditor::set_texture_stage);
		appearance -> textureTransform () .addInterest (this, &TextureCoordinateEditor::set_textureTransform);

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
TextureCoordinateEditor::set_material (const X3D::SFNode & value)
{
	if (material)
		material -> removeInterest (*right, &X3D::Browser::addEvent);

	material = value;

	if (material)
		material -> addInterest (*right,  &X3D::Browser::addEvent);

	right -> addEvent ();
}

void
TextureCoordinateEditor::set_texture_stage (const X3D::SFNode & value)
{
	const X3D::X3DPtr <X3D::MultiTexture> multiTexture (value);

	const size_t textureStages = multiTexture ? multiTexture -> getTexture () .size () : 1;

	getTextureStageAdjustment () -> set_upper (textureStages);
	getTextureStageAdjustment () -> set_value (std::min <size_t> (getTextureStageAdjustment () -> get_value (), textureStages));

	set_texture (value);
}

void
TextureCoordinateEditor::set_texture (const X3D::SFNode & value)
{
	if (texture)
	{
		texture -> removeInterest (*left,  &X3D::Browser::addEvent);
		texture -> removeInterest (*right, &X3D::Browser::addEvent);
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
		texture -> addInterest (*left,  &X3D::Browser::addEvent);
		texture -> addInterest (*right, &X3D::Browser::addEvent);
	}
}

void
TextureCoordinateEditor::set_textureTransform (const X3D::SFNode & value)
{
	if (textureTransform)
	{
		textureTransform -> removeInterest (*left,  &X3D::Browser::addEvent);
		textureTransform -> removeInterest (*right, &X3D::Browser::addEvent);
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
		textureTransform -> addInterest (*left,  &X3D::Browser::addEvent);
		textureTransform -> addInterest (*right, &X3D::Browser::addEvent);
	}

	left  -> addEvent ();
	right -> addEvent ();
}

void
TextureCoordinateEditor::set_geometry (const X3D::SFNode & value)
{
	try
	{
		const auto rightShape             = right -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto rightSelectedGeometry  = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto rightSelectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");

		if (geometry)
		{
			geometry -> texCoordIndex () .removeInterest (this, &TextureCoordinateEditor::set_texCoordIndex);
			geometry -> coordIndex ()    .removeInterest (this, &TextureCoordinateEditor::set_coordIndex);
			geometry -> texCoord ()      .removeInterest (this, &TextureCoordinateEditor::set_texCoord);
			geometry -> coord ()         .removeInterest (this, &TextureCoordinateEditor::set_coord);
		}

		geometry = value;

		if (geometry)
		{
			coord           = geometry -> coord ();
			previewGeometry = geometry -> copy (rightShape -> getExecutionContext (), X3D::FLAT_COPY);

			previewGeometry -> isPrivate (true);
			previewGeometry -> texCoordIndex () .addInterest (this, &TextureCoordinateEditor::set_left_selected_faces);

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

			geometry -> texCoordIndex () .addInterest (this, &TextureCoordinateEditor::set_texCoordIndex);
			geometry -> coordIndex ()    .addInterest (this, &TextureCoordinateEditor::set_coordIndex);
			geometry -> texCoord ()      .addInterest (this, &TextureCoordinateEditor::set_texCoord);
			geometry -> coord ()         .addInterest (this, &TextureCoordinateEditor::set_coord);

			rightShape -> geometry () = previewGeometry;
			rightShape -> getExecutionContext () -> realize ();

			set_texCoordIndex ();
			set_texCoord (geometry -> texCoord ());

			rightSelectedGeometry  -> coordIndex () .clear ();
			rightSelectionGeometry -> coordIndex () .clear ();

			rightSelectedGeometry  -> coord () = coord;
			rightSelectionGeometry -> coord () = coord;

			rightSelection -> setGeometry (geometry);
			rightSelection -> setCoord (coord);

			getWidget () .set_sensitive (true);
		}
		else
		{
			rightSelection -> setGeometry (nullptr);
			rightSelection -> setCoord (nullptr);

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
TextureCoordinateEditor::set_texCoordIndex ()
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
TextureCoordinateEditor::set_coordIndex ()
{
	rightSelection -> setGeometry (previewGeometry);

	set_texCoordIndex ();
}

void
TextureCoordinateEditor::set_texCoord (const X3D::SFNode & value)
{
	undoHistory .clear ();

	if (multiTexCoord)
		multiTexCoord -> removeInterest (this, &TextureCoordinateEditor::set_texCoord);

	// Determine texCoord.

	X3D::X3DPtr <X3D::TextureCoordinate> texCoordNode;

	multiTexCoord = value;

	if (multiTexCoord)
	{
		multiTexCoord -> addInterest (this, &TextureCoordinateEditor::set_texCoord, value);

		if (multiTexCoord -> getTexCoord () .empty ())
			texCoordNode = nullptr;
		else
			texCoordNode = multiTexCoord -> getTexCoord () [std::min (stage, multiTexCoord -> getTexCoord () .size () - 1)];
	}
	else
		texCoordNode = value;

	// Create preview texCoord node.

	texCoord = right -> getExecutionContext () -> createNode <X3D::TextureCoordinate> ();

	texCoord -> point () .addInterest (this, &TextureCoordinateEditor::set_left_coord);

	if (texCoordNode)
		texCoord -> point () = texCoordNode -> point ();

	right -> getExecutionContext () -> realize ();

	clear ();
	set_left_coord ();

	// Add texCoord.

	if (previewGeometry)
	{
		try
		{
			previewGeometry -> getField <X3D::SFNode> ("texCoord") = texCoord;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	left  -> addEvent ();
	right -> addEvent ();
}

void
TextureCoordinateEditor::on_texture_stage_changed ()
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
	{
		try
		{
			set_texCoord (geometry -> getField <X3D::SFNode> ("texCoord"));
		}
		catch (const X3D::X3DError &)
		{
			set_texCoord (nullptr);
		}
	}
	else
		set_texCoord (nullptr);
}

void
TextureCoordinateEditor::set_coord (const X3D::SFNode & value)
{
	coord = value;

	rightSelection -> setCoord (coord);
}

void
TextureCoordinateEditor::clear ()
{
	selectedFaces .clear ();

	set_selected_faces ();
}

void
TextureCoordinateEditor::set_selected_faces ()
{
	set_left_selected_faces ();
	set_right_selected_faces ();
}

void
TextureCoordinateEditor::set_left_coord ()
{
	try
	{
		const auto selectedCoord = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("SelectedCoord");

		size_t i = 0;

		selectedCoord -> point () .resize (texCoord -> point () .size ());

		for (const auto & point : texCoord -> point ())
			selectedCoord -> point () [i ++] = X3D::Vector3f (point .getX (), point .getY (), 0);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_left_image ()
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");
		const auto imageTexCoord    = left -> getExecutionContext () -> getNamedNode <X3D::TextureCoordinate> ("ImageTextureCoordinate");
		const auto imageCoord       = left -> getExecutionContext () -> getNamedNode <X3D::Coordinate> ("ImageCoordinate");

		auto bbox = selectedGeometry -> getBBox ();

		if (bbox .empty ())
			bbox = X3D::Box3f (X3D::Vector3f (1, 1, 0), X3D::Vector3f (0.5, 0.5, 0));

		X3D::Vector3f min, max;
		bbox .extents (min, max);

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
TextureCoordinateEditor::set_left_selected_faces ()
{
	try
	{
		const auto selectedGeometry = left -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");

		selectedGeometry -> coordIndex () .clear ();

		for (const auto & face : selectedFaces)
		{
			const auto vertices = rightSelection -> getVertices (face);

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
TextureCoordinateEditor::set_left_active (const bool value)
{
	using set1Value = void (X3D::MFVec2f::*) (const X3D::MFVec2f::size_type, const X3D::SFVec2f &);

	if (value)
	{
		set_startDrag ();

		// Init undo step.

		switch (tool)
		{
			case ToolType::MOVE:
			{
				undoStep = std::make_shared <UndoStep> (_ ("Move Points"));
				break;
			}
			case ToolType::ROTATE:
			{
				undoStep = std::make_shared <UndoStep> (_ ("Rotate Points"));
				break;
			}
			case ToolType::SCALE:
			{
				undoStep = std::make_shared <UndoStep> (_ ("Scale Points"));
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
TextureCoordinateEditor::set_startDrag ()
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
		startPositions = { std::make_pair (activePoint, startPosition) };

		set_selectedPoints ();
	}
	catch (const X3D::X3DError &)
	{ }
	catch (const std::domain_error &)
	{ }
}

void
TextureCoordinateEditor::set_left_touchTime ()
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

		set_selectedPoints ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_left_hitPoint (const X3D::Vector3f & hitPoint)
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
	catch (const X3D::X3DError &)
	{ }
	catch (const std::domain_error &)
	{ }
}

void
TextureCoordinateEditor::move (const X3D::Vector3f & hitPoint)
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
TextureCoordinateEditor::rotate (const X3D::Vector3f & hitPoint)
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
			constexpr double snapAngle = 11.25 / 180 * M_PI;

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
TextureCoordinateEditor::scale (const X3D::Vector3f & hitPoint)
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
TextureCoordinateEditor::set_left_point (const X3D::Vector3d & hitPoint)
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
	catch (const X3D::X3DError &)
	{ }
	catch (const std::domain_error &)
	{ }
}

int32_t
TextureCoordinateEditor::getNearestPoint (const X3D::Vector3d & hitPoint) const
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

		std::map <double, int32_t> distances;

		for (const auto & index : selectedGeometry -> coordIndex ())
		{
			if (index >= 0)
				distances .emplace (math::abs (hitPoint - selectedCoord -> get1Point (index)), index);
		}

		return distances .begin () -> second;
	}
	catch (const X3D::X3DError &)
	{
		return -1;
	}
}

void
TextureCoordinateEditor::set_selectedPoints ()
{
	try
	{
		const auto selectedPointSet = left -> getExecutionContext () -> getNamedNode ("SelectedPointSet");

		selectedPointSet -> getField <X3D::MFInt32> ("set_coordIndex") .assign (selectedPoints .begin (), selectedPoints .end ());
	}
	catch (const X3D::X3DError &)
	{ }
}

void
TextureCoordinateEditor::set_left_center (const X3D::Vector3f & value)
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
	catch (const X3D::X3DError &)
	{ }
	catch (const std::domain_error &)
	{ }
}

void
TextureCoordinateEditor::set_right_selection (const X3D::Vector3f & point)
{
	try
	{
		const auto selectionGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectionGeometry");
		const auto vertices          = rightSelection -> getVertices (rightSelection -> getFace () .first);

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
TextureCoordinateEditor::set_right_active (const bool value)
{
	if (value)
		rightPaintSelecion = false;
}

void
TextureCoordinateEditor::set_right_touchTime ()
{
	if (rightSelection -> isEmpty ())
		return;

	if (not keys .shift () and not keys .control () and not rightPaintSelecion)
		selectedFaces .clear ();

	if (keys .control ())
		selectedFaces .erase (rightSelection -> getFace () .first);

	else if (not selectedFaces .emplace (rightSelection -> getFace () .first) .second)
		return;

	set_selected_faces ();
}

void
TextureCoordinateEditor::set_right_selected_faces ()
{
	try
	{
		const auto selectedGeometry = right -> getExecutionContext () -> getNamedNode <X3D::IndexedLineSet> ("SelectedGeometry");

		selectedGeometry -> coordIndex () .clear ();

		for (const auto & face : selectedFaces)
		{
			const auto vertices = rightSelection -> getVertices (face);

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
TextureCoordinateEditor::set_right_hitPoint (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto touchSensor = right -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

		// Determine face and faces

		rightSelection -> setHitPoint (hitPoint, touchSensor -> hitTriangle_changed ());

		if (rightSelection -> isEmpty ())
			return;

		// Setup cross hair

		set_right_selection (coord -> get1Point (rightSelection -> getIndices () [0]));

		if (touchSensor -> isActive () and (keys .shift () or keys .control ()))
		{
			rightPaintSelecion = true;
			set_right_touchTime ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

X3D::Vector2d
TextureCoordinateEditor::projectPoint (const X3D::Vector3d & point, const X3D::BrowserPtr & browser) const
throw (std::domain_error)
{
	const auto & activeLayer = browser -> getActiveLayer ();

	if (not activeLayer)
		throw std::domain_error ("No active layer.");

	const auto viewport   = activeLayer -> getViewport () -> getRectangle ();
	const auto projection = activeLayer -> getViewpoint () -> getProjectionMatrix (0, -10, viewport);
	const auto modelview  = X3D::Matrix4d ();
	const auto p          = X3D::ViewVolume::projectPoint (point, modelview, projection, viewport);

	return X3D::Vector2d (p. x (), p .y ());
}

TextureCoordinateEditor::~TextureCoordinateEditor ()
{
	undoHistory .clear ();
	dispose ();
}

} // puck
} // titania

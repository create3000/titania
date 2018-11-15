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

#include <Titania/Math/Geometry/MinimumBoundingBox.h>

#include "X3DIndexedFaceSetTransformObject.h"

#include "../../Rendering/CoordinateTool.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Layout/ScreenGroup.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Editing/Selection/FaceSelection.h"
#include "../../../Editing/Undo/UndoStepContainer.h"
#include "../../../Editing/X3DEditor.h"
#include "../../../Prototype/X3DProtoDeclarationNode.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

static constexpr double SIZE_MIN = 0;
static constexpr size_t EVENTS   = 4;

X3DIndexedFaceSetTransformObject::Fields::Fields () :
	             transform (new SFBool ()),
	axisAlignedBoundingBox (new SFBool (true))
{ }

X3DIndexedFaceSetTransformObject::X3DIndexedFaceSetTransformObject () :
	                  IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetSelectionObject (),
	                          fields (),
	                     touchSensor (),
	                     planeSensor (),
	               planeSensorNormal (),
	             transformToolSwitch (),
	                   transformNode (),
	                   transformTool (),
	              selectionTransform (new Transform (getExecutionContext ())),
	                  selectionCoord (),
	                    axisRotation (),
	                          active (false),
	                          events (0),
	                        undoStep (std::make_shared <X3D::UndoStep> (_ ("Empty UndoStep")))
{
	addType (X3DConstants::X3DIndexedFaceSetTransformObject);

	addChildObjects (touchSensor,
	                 planeSensor,
	                 planeSensorNormal,
	                 transformToolSwitch,
	                 transformNode,
	                 transformTool,
	                 selectionTransform,
	                 selectionCoord);
}

void
X3DIndexedFaceSetTransformObject::initialize ()
{
	selectionTransform -> setup ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (&X3DIndexedFaceSetTransformObject::set_loadState, this);

	set_loadState ();
}

void
X3DIndexedFaceSetTransformObject::setExecutionContext (X3DExecutionContext* const executionContext)
{
	selectionTransform -> setExecutionContext (executionContext);
}

void
X3DIndexedFaceSetTransformObject::set_loadState ()
{
	try
	{
		const auto & inlineNode = getCoordinateTool () -> getInlineNode ();

		touchSensor         = inlineNode -> getExportedNode <TouchSensor>      ("TouchSensor");
		planeSensor         = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensor");
		planeSensorNormal   = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensorNormal");
		transformToolSwitch = inlineNode -> getExportedNode <Switch>           ("SelectionTransformToolSwitch");
		transformNode       = inlineNode -> getExportedNode <Transform>        ("SelectionTransform");
		transformTool       = inlineNode -> getExportedNode <Transform>        ("SelectionTransformTool");
		selectionCoord      = inlineNode -> getExportedNode <CoordinateDouble> ("SelectionCoord");

		transform ()              .addInterest (&X3DIndexedFaceSetTransformObject::set_transform, this);
		axisAlignedBoundingBox () .addInterest (&X3DIndexedFaceSetTransformObject::set_transform, this);

		getBrowser () -> getControlKey ()  .addInterest (&X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint, this);
		touchSensor -> hitPoint_changed () .addInterest (&X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint, this);

		planeSensor -> isActive ()            .addInterest (&X3DIndexedFaceSetTransformObject::set_plane_sensor_active, this);
		planeSensor -> translation_changed () .addInterest (&X3DIndexedFaceSetTransformObject::set_plane_sensor_translation, this);

		// Transform Tool

		selectionTransform -> translation ()      .addInterest (transformTool -> translation ());
		selectionTransform -> rotation ()         .addInterest (transformTool -> rotation ());
		selectionTransform -> scale ()            .addInterest (transformTool -> scale ());
		selectionTransform -> scaleOrientation () .addInterest (transformTool -> scaleOrientation ());
		selectionTransform -> center ()           .addInterest (transformTool -> center ());

		transformTool -> translation ()      .addInterest (selectionTransform -> translation ());
		transformTool -> rotation ()         .addInterest (selectionTransform -> rotation ());
		transformTool -> scale ()            .addInterest (selectionTransform -> scale ());
		transformTool -> scaleOrientation () .addInterest (selectionTransform -> scaleOrientation ());
		transformTool -> center ()           .addInterest (selectionTransform -> center ());

		transformTool -> addInterest (&X3DIndexedFaceSetTransformObject::set_transform_modelViewMatrix, this);
		transformTool -> addTool ();
		transformTool -> getField <SFBool> ("isActive")  .addInterest (&X3DIndexedFaceSetTransformObject::set_transform_active, this);
		transformTool -> getField <SFBool> ("isActive")  .addInterest (isActive ());
		transformTool -> getField <SFTime> ("touchTime") .addInterest (touchTime ());

		selectionCoord -> getField <MFVec3d> ("point") .addInterest (&X3DIndexedFaceSetTransformObject::set_selection, this);

		set_transform ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetTransformObject::set_transform ()
{
	// Update tranform tool if selection changes.

	if (active)
		return;

	if (transformTool -> getField <SFBool> ("grouped"))
		return;

	Vector3d   center, size;
	Rotation4d orientation;

	if (axisAlignedBoundingBox ())
	{
		// AABB

		const auto bbox = selectionCoord -> getBBox ();

		center = bbox .center ();
		size   = bbox .size ();
	}
	else
	{
		// OBB

		const auto bbox = getMinimumBBox ();

		if (abs (bbox .size ()))
		{
			Vector3d t;

			normalize (bbox .matrix ()) .get (t, orientation);

			center = bbox .center () * ~orientation;
			size   = Vector3d (abs (bbox .matrix () .x_axis ()), abs (bbox .matrix () .y_axis ()), abs (bbox .matrix () .z_axis ())) * 2.0;
			size   = max (size, -size); // max (v, -v): Componentwise abs.
		}
		else
		{
			// Point
			center = bbox .center ();
			size   = bbox .size ();
		}
	}

	transformToolSwitch -> whichChoice () = transform () and not getSelectedPoints () .empty ();
	transformNode       -> rotation ()    = orientation;

	// Reset transformTool matrix.

	transformTool -> translation ()      = Vector3f ();
	transformTool -> rotation ()         = Rotation4f ();
	transformTool -> scale ()            = Vector3f (1, 1, 1);
	transformTool -> scaleOrientation () = Rotation4f ();
	transformTool -> center ()           = center;

	// Update selectionTransform bbox.

	selectionTransform -> bboxCenter () = transformTool -> bboxCenter () = center;
	selectionTransform -> bboxSize ()   = transformTool -> bboxSize ()   = size;

	// Remember axisRotation.

	axisRotation = Matrix4d (orientation);
}

void
X3DIndexedFaceSetTransformObject::set_selection ()
{
	if (transform ())
		set_transform ();
}

void
X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint ()
{
	try
	{
		// Setup PlaneSensor

		if (planeSensor -> isActive ())
			return;

		switch (getHotPoints () .size ())
		{
			case 0:
			{
				planeSensor       -> enabled () = false;
				planeSensorNormal -> enabled () = false;
				break;
			}
			case 1:
			{
				try
				{
					// Translate over screen plane
	
					const auto vector       = inverse (touchSensor -> getModelViewMatrix ()) .mult_dir_matrix (Vector3d (0, 0, 1));
					const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), vector);
	
					planeSensorNormal -> enabled () = false;
	
					planeSensor -> enabled ()      = toolType () == "SELECT" and not paintSelection ();
					planeSensor -> axisRotation () = axisRotation;
					planeSensor -> offset ()       = Vector3d ();
					planeSensor -> maxPosition ()  = Vector2d (-1, -1);
				}
				catch (const std::domain_error &)
				{
					planeSensorNormal -> enabled () = false;
				}

				break;
			}
			case 2:
			{
				// Translate along edge

				const auto vector       = getCoord () -> get1Point (getHotPoints () [0]) - getCoord () -> get1Point (getHotPoints () [1]);
				const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), vector);

				planeSensorNormal -> enabled () = false;

				planeSensor -> enabled ()      = toolType () == "SELECT" and not paintSelection ();
				planeSensor -> axisRotation () = axisRotation;
				planeSensor -> offset ()       = Vector3d ();
				planeSensor -> maxPosition ()  = Vector2d (-1, 0);
				break;
			}
			default:
			{
				const auto normal = getPolygonNormal (getFaceSelection () -> getFaceVertices (getHotFace ()));

				// Translate along face normal

				planeSensorNormal -> enabled ()      = toolType () == "SELECT" and not paintSelection ();
				planeSensorNormal -> axisRotation () = Rotation4d (Vector3d (1, 0, 0), Vector3d (normal));
				planeSensorNormal -> offset ()       = Vector3d ();
				planeSensorNormal -> maxPosition ()  = Vector2d (-1, 0);

				// Translate along plane

				planeSensor -> enabled ()      = toolType () == "SELECT" and not paintSelection ();
				planeSensor -> axisRotation () = Rotation4d (Vector3d (0, 0, 1), Vector3d (normal));
				planeSensor -> offset ()       = Vector3d ();
				planeSensor -> maxPosition ()  = Vector2d (-1, -1);

				break;
			}
		}
	}
	catch (const std::exception & error)
	{ }
}

void
X3DIndexedFaceSetTransformObject::set_plane_sensor_active (const bool active)
{
	if (active)
	{
		// Create undo step for translation.

		undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Transform %s »point«"), getCoord () -> getTypeName () .c_str ()));

		undoSetCoord (undoStep);

		events = 0;

		setTranslate (false);
		setTranslation (Vector3d ());
	}
	else
	{
		redoSetCoord (undoStep);

		// Reset fields and send undo step.

		if (abs (getTranslation ()))
		{
			// Prototype support
		
			X3DEditor::requestUpdateInstances (SFNode (getNode <IndexedFaceSet> ()), undoStep);
		
			// Send undo step

			undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
		}

		setTranslate (false);
	}
}

void
X3DIndexedFaceSetTransformObject::set_plane_sensor_translation ()
{
	// Prevent accidentially move.
	if (events ++ < EVENTS)
		return;

	setTranslate (true);
	setTranslation (planeSensor -> translation_changed () .getValue ());

	if (getBrowser () -> getControlKey () and getHotPoints () .size () >= 3)
	{
		// Translate along plane normal
		for (const auto & pair : getSelectedPoints ())
			getCoord () -> set1Point (pair .first, pair .second + Vector3d (planeSensorNormal -> translation_changed () .getValue ()));
	}
	else
	{
		for (const auto & pair : getSelectedPoints ())
			getCoord () -> set1Point (pair .first, pair .second + getTranslation ());
	}
}

void
X3DIndexedFaceSetTransformObject::set_transform_active (const bool value)
{
	active = value;

	if (active)
	{
		events = 0;

		setTranslate (false);
	}
	else
	{
		if (getTranslate ())
			set_transform ();

		setTranslate (false);
	}

	set_plane_sensor_active (active);
}

void
X3DIndexedFaceSetTransformObject::set_transform_modelViewMatrix ()
{
	try
	{
		// Transform tool was moved:

		if (not (active or transformTool -> getField <SFBool> ("grouped")))
			return;
	
		setTranslate (true);
		setTranslation (Vector3f (1, 1, 1));
	
		const auto M = ~axisRotation * transformTool -> getMatrix () * axisRotation;
	
		for (const auto & pair : getSelectedPoints ())
			getCoord () -> set1Point (pair .first, pair .second * M);
	}
	catch (const std::domain_error &)
	{ }
}

Box3d
X3DIndexedFaceSetTransformObject::getMinimumBBox () const
{
	auto points = std::vector <Vector3d> (selectionCoord -> point () .cbegin (), selectionCoord -> point () .cend ());
	auto bbox   = Box3d ();

	std::sort (points .begin (), points .end ());

	points .erase (std::unique (points .begin (), points .end ()), points .end ());

	switch (points .size ())
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			bbox = Box3d (Vector3d (), points .front ());
			break;
		}
		case 2:
		{
			const auto & point1 = points .front ();
			const auto & point2 = points .back ();
			const auto   yAxis  = (point2 - point1) / 2.0;
			const auto   zAxis  = Vector3d ();
			const auto   xAxis  = Vector3d ();
			const auto   center = (point2 + point1) / 2.0;

			bbox = Box3d (Matrix4d (xAxis .x (), xAxis .y (), xAxis .z (), 0,
			                        yAxis .x (), yAxis .y (), yAxis .z (), 0,
			                        zAxis .x (), zAxis .y (), zAxis .z (), 0,
			                        center .x (), center .y (), center .z (), 1));

			break;
		}
		default:
		{
			bbox = getMinimumBBox (points);
			break;
		}
	}

	return bbox;
}

Box3d
X3DIndexedFaceSetTransformObject::getMinimumBBox (const std::vector <Vector3d> & points) const
{
	auto bbox = minimum_bounding_box (points);

	auto       axes   = std::vector <X3D::Vector3d> ({ bbox .matrix () .x_axis (), bbox .matrix () .y_axis (), bbox .matrix () .z_axis () });
	const auto center = bbox .center ();

	std::sort (axes .begin (), axes .end (), [ ] (const X3D::Vector3d & a, const X3D::Vector3d & b) { return abs (a) < abs (b); });

	if (dot (axes [0], Vector3d (0, 0, 1)) < 0)
		axes [0] .negate ();

	if (dot (axes [1], Vector3d (1, 0, 0)) < 0)
		axes [1] .negate ();

	if (dot (axes [2], Vector3d (0, 1, 0)) < 0)
		axes [2] .negate ();

	bbox .matrix (Matrix4d (axes [1] .x (), axes [1] .y (), axes [1] .z (), 0,
	                        axes [2] .x (), axes [2] .y (), axes [2] .z (), 0,
	                        axes [0] .x (), axes [0] .y (), axes [0] .z (), 0,
	                        center .x (), center .y (), center .z (), 1));

	return bbox;
}

X3DIndexedFaceSetTransformObject::~X3DIndexedFaceSetTransformObject ()
{ }

} // X3D
} // titania

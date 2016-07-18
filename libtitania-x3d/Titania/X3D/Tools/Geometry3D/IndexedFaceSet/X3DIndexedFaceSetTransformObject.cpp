/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include <Titania/Math/Geometry/MinimumBoundingBox.h>

#include "X3DIndexedFaceSetTransformObject.h"

#include "../../Rendering/CoordinateTool.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/Layout/ScreenGroup.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../../Editing/Selection/FaceSelection.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

static constexpr size_t TRANSLATIONS_EVENTS = 4;

X3DIndexedFaceSetTransformObject::Fields::Fields () :
	             transform (new SFBool ()),
	axisAlignedBoundingBox (new SFBool ())
{ }

X3DIndexedFaceSetTransformObject::X3DIndexedFaceSetTransformObject () :
	       IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetTool (),
	               fields (),
	          touchSensor (),
	          planeSensor (),
	    planeSensorNormal (),
	  transformToolSwitch (),
	        transformNode (),
	        transformTool (),
	       selectionCoord (),
	         translations (0),
	         axisRotation (),
	               active (false),
	             undoStep (std::make_shared <X3D::UndoStep> (_ ("Empty UndoStep")))
{
	//addType (X3DConstants::X3DIndexedFaceSetTransformObject);

	addChildren (touchSensor,
	             planeSensor,
	             planeSensorNormal,
	             transformToolSwitch,
	             transformNode,
	             transformTool,
	             selectionCoord);
}

void
X3DIndexedFaceSetTransformObject::initialize ()
{
	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DIndexedFaceSetTransformObject::set_loadState);

	set_loadState ();
}

void
X3DIndexedFaceSetTransformObject::set_loadState ()
{
	try
	{
		const auto & inlineNode = getCoordinateTool () -> getInlineNode ();

		touchSensor          = inlineNode -> getExportedNode <TouchSensor>      ("TouchSensor");
		planeSensor          = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensor");
		planeSensorNormal    = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensorNormal");
		transformToolSwitch  = inlineNode -> getExportedNode <Switch>           ("TransformToolSwitch");
		transformNode        = inlineNode -> getExportedNode <Transform>        ("Transform");
		transformTool        = inlineNode -> getExportedNode <Transform>        ("TransformTool");
		selectionCoord       = inlineNode -> getExportedNode <CoordinateDouble> ("SelectionCoord");

		transform ()              .addInterest (this, &X3DIndexedFaceSetTransformObject::set_transform);
		axisAlignedBoundingBox () .addInterest (this, &X3DIndexedFaceSetTransformObject::set_transform);

		getBrowser () -> getControlKey ()  .addInterest (this, &X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint);
		touchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint);

		planeSensor -> isActive ()            .addInterest (this, &X3DIndexedFaceSetTransformObject::set_plane_sensor_active);
		planeSensor -> translation_changed () .addInterest (this, &X3DIndexedFaceSetTransformObject::set_plane_sensor_translation);

		// Transform Tool

		transformTool -> addInterest (this, &X3DIndexedFaceSetTransformObject::set_transform_modelViewMatrix);
		transformTool -> addTool ();
		transformTool -> getField <SFBool> ("isActive") .addInterest (this, &X3DIndexedFaceSetTransformObject::set_transform_active);
		transformTool -> setField <SFBool> ("bbox", false);

		selectionCoord -> getField <MFVec3d> ("point") .addInterest (this, &X3DIndexedFaceSetTransformObject::set_selection);

		set_transform ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

//void
//X3DIndexedFaceSetTransformObject::set_transform ()
//{
//	if (active)
//		return;
//
//	const auto rotation = getAxisRotation ();
//
//	axisRotation = Matrix4d (rotation);
//
//	const auto bbox = selectionCoord -> getBBox () * ~axisRotation;
//
//	transformToolSwitch -> whichChoice () = transform () and not getSelectedPoints () .empty ();
//	transformNode       -> rotation ()    = rotation;
//
//	transformTool -> translation ()      = Vector3f ();
//	transformTool -> rotation ()         = Rotation4f ();
//	transformTool -> scale ()            = Vector3f (1, 1, 1);
//	transformTool -> scaleOrientation () = Rotation4f ();
//	transformTool -> center ()           = bbox .center ();
//
//	transformTool -> bboxCenter () = bbox .center ();
//	transformTool -> bboxSize ()   = max (bbox .size (), Vector3d (1e-5, 1e-5, 1e-5));
//}

void
X3DIndexedFaceSetTransformObject::set_transform ()
{
	if (active)
		return;

	Vector3d translation, scale;
	Rotation4d rotation;

	if (axisAlignedBoundingBox ())
	{
		const auto bbox = selectionCoord -> getBBox ();

		translation = bbox .center ();
		scale       = bbox .size ();
	}
	else
	{
		const auto bbox = getSelectionBBox ();
	
		bbox .matrix () .get (translation, rotation, scale);
	
		translation = translation * ~rotation;
		scale      *= 2.0;
		scale       = Vector3d (std::abs (scale .x ()), std::abs (scale .y ()), std::abs (scale .z ()));
	}

	transformToolSwitch -> whichChoice () = transform () and not getSelectedPoints () .empty ();
	transformNode       -> rotation ()    = rotation;

	transformTool -> translation ()      = Vector3f ();
	transformTool -> rotation ()         = Rotation4f ();
	transformTool -> scale ()            = Vector3f (1, 1, 1);
	transformTool -> scaleOrientation () = Rotation4f ();
	transformTool -> center ()           = translation;

	transformTool -> bboxCenter () = translation;
	transformTool -> bboxSize ()   = max (scale, Vector3d (1e-5, 1e-5, 1e-5));

	axisRotation = Matrix4d (rotation);
}

void
X3DIndexedFaceSetTransformObject::set_selection ()
{
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
				// Translate over screen plane

				const auto vector       = inverse (getModelViewMatrix ()) .mult_dir_matrix (Vector3d (0, 0, 1));
				const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), vector);

				planeSensorNormal -> enabled () = false;

				planeSensor -> enabled ()      = select () and not paintSelection ();
				planeSensor -> axisRotation () = axisRotation;
				planeSensor -> offset ()       = Vector3d ();
				planeSensor -> maxPosition ()  = Vector2d (-1, -1);
				break;
			}
			case 2:
			{
				// Translate along edge

				const auto vector       = getCoord () -> get1Point (getHotPoints () [0]) - getCoord () -> get1Point (getHotPoints () [1]);
				const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), vector);

				planeSensorNormal -> enabled () = false;

				planeSensor -> enabled ()      = select () and not paintSelection ();
				planeSensor -> axisRotation () = axisRotation;
				planeSensor -> offset ()       = Vector3d ();
				planeSensor -> maxPosition ()  = Vector2d (-1, 0);
				break;
			}
			default:
			{
				const auto normal = getPolygonNormal (getFaceSelection () -> getFaceVertices (getHotFace ()));
					
				// Translate along face normal

				planeSensorNormal -> enabled ()      = select () and not paintSelection ();
				planeSensorNormal -> axisRotation () = Rotation4d (Vector3d (1, 0, 0), Vector3d (normal));
				planeSensorNormal -> offset ()       = Vector3d ();
				planeSensorNormal -> maxPosition ()  = Vector2d (-1, 0);

				// Translate along plane

				planeSensor -> enabled ()      = select () and not paintSelection ();
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

		undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Translate %s »point«"), getCoord () -> getTypeName () .c_str ()));

	   undoSetCoordPoint (undoStep);
	
		translations = 0;

		setTranslation (Vector3d ());
	}
	else
	{
		redoSetCoordPoint (undoStep);

		// Reset fields and send undo step.

		if (abs (getTranslation ()))
			undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);

		setTranslate (false);
	}
}

void
X3DIndexedFaceSetTransformObject::set_plane_sensor_translation ()
{
	// Prevent accidentially move.
	if (translations ++ < TRANSLATIONS_EVENTS)
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

	if (not active and getTranslate ())
		set_transform ();

	set_plane_sensor_active (active);
}

void
X3DIndexedFaceSetTransformObject::set_transform_modelViewMatrix ()
{
	if (not active)
		return;

	// The Transform Tool has own exidential event handling.

	setTranslate (true);
	setTranslation (Vector3f (1, 1, 1));

	const auto & M = ~axisRotation * transformTool -> getMatrix () * axisRotation;

	for (const auto & pair : getSelectedPoints ())
		getCoord () -> set1Point (pair .first, pair .second * M);
}

Box3d
X3DIndexedFaceSetTransformObject::getSelectionBBox () const
{
	const auto points = std::vector <Vector3d> (selectionCoord -> point () .begin (), selectionCoord -> point () .end ());
	auto       bbox   = Box3d ();

	switch (points .size ())
	{
		case 0:
			break;
		case 1:
			bbox = Box3d (Vector3d (1e-5, 1e-5, 1e-5), points .front ());
			break;
		case 2:
		{
			switch (getSelectionType ())
			{
				case SelectionType::EDGES:
				{
					if (getSelectedEdges () .size () == 1)
					{
						const auto & edge = *getSelectedEdges () .begin ();

						std::set <size_t> faces;
						
						for (const auto & e : edge .second)
						{
							for (const auto & face : getFaceSelection () -> getAdjacentFaces (X3DFaceSelection::Edge { e .first, e .second }))
								faces. emplace (face);
							
							break;
						}
		
						Vector3d normal;
		
						for (const auto & face : faces)
							normal += getPolygonNormal (getFaceSelection () -> getFaceVertices (face));

						const auto point1 = getCoord () -> get1Point (edge .first .first);
						const auto point2 = getCoord () -> get1Point (edge .first .second);
						const auto yAxis  = (point2 - point1) / 2.0;
						const auto zAxis  = normalize (cross <double> (yAxis, normal)) * 1e-5;
						const auto xAxis  = normalize (cross <double> (zAxis, yAxis))  * 1e-5;
						const auto center = (point2 + point1) / 2.0;

						if (abs (xAxis))
							return Box3d (Matrix4d (xAxis .x (), xAxis .y (), xAxis .z (), 0,
							                        yAxis .x (), yAxis .y (), yAxis .z (), 0,
							                        zAxis .x (), zAxis .y (), zAxis .z (), 0,
							                        center .x (), center .y (), center .z (), 1));
					}

					break;
				}
				default:
					break;
			}

			bbox = minimum_bounding_box (points);
			break;
		}
		default:
			bbox = minimum_bounding_box (points);
			break;
	}

	return bbox;
}

X3DIndexedFaceSetTransformObject::~X3DIndexedFaceSetTransformObject ()
{ }

} // X3D
} // titania

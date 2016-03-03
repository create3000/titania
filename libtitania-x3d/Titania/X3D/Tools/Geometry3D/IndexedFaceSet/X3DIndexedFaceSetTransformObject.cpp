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

#include "X3DIndexedFaceSetTransformObject.h"

#include "../../Rendering/CoordinateTool.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../../Editing/Selection/FaceSelection.h"

#include <Titania/String/sprintf.h>

namespace titania {
namespace X3D {

static constexpr size_t TRANSLATIONS_EVENTS = 4;

X3DIndexedFaceSetTransformObject::X3DIndexedFaceSetTransformObject () :
	                  IndexedFaceSet (getExecutionContext ()),
	           X3DIndexedFaceSetTool (),
	                     touchSensor (),
	                     planeSensor (),
	                     translation (),
	                    translations (0),
	                        undoStep (std::make_shared <X3D::UndoStep> (_ ("Empty UndoStep")))
{
	//addType (X3DConstants::X3DIndexedFaceSetTransformObject);

	addChildren (touchSensor,
	             planeSensor);

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DIndexedFaceSetTransformObject::set_loadState);
}

void
X3DIndexedFaceSetTransformObject::initialize ()
{
}

void
X3DIndexedFaceSetTransformObject::set_loadState ()
{
	try
	{
		const auto & inlineNode         = getCoordinateTool () -> getInlineNode ();
		const auto   activeFaceGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("ActiveFaceGeometry");

		planeSensor = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensor");
		touchSensor = inlineNode -> getExportedNode <TouchSensor>      ("TouchSensor");

		getBrowser () -> hasControlKey ()  .addInterest (this, &X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint);
		touchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint);

		planeSensor -> isActive ()            .addInterest (this, &X3DIndexedFaceSetTransformObject::set_plane_sensor_active);
		planeSensor -> translation_changed () .addInterest (this, &X3DIndexedFaceSetTransformObject::set_plane_sensor_translation);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetTransformObject::set_touch_sensor_hitPoint ()
{
	try
	{
		// Setup PlaneSensor

		if (not select ())
		   return;

		if (planeSensor -> isActive ())
			return;

		switch (getHotPoints () .size ())
		{
			case 0:
			{
				planeSensor -> enabled () = false;
				break;
			}
			case 1:
			{
				// Translate over screen plane

				const auto vector       = inverse (getModelViewMatrix ()) .mult_dir_matrix (Vector3d (0, 0, 1));
				const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), vector);

				planeSensor -> enabled ()      = not paintSelection ();
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

				planeSensor -> enabled ()      = not paintSelection ();
				planeSensor -> axisRotation () = axisRotation;
				planeSensor -> offset ()       = Vector3d ();
				planeSensor -> maxPosition ()  = Vector2d (-1, 0);
				break;
			}
			default:
			{
				const auto normal = getPolygonNormal (getFaceSelection () -> getFaceVertices (getHotFace ()));
					
				if (getBrowser () -> hasControlKey ())
				{
					// Translate along face normal

					const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), Vector3d (normal));

					planeSensor -> enabled ()      = not paintSelection ();
					planeSensor -> axisRotation () = axisRotation;
					planeSensor -> offset ()       = Vector3d ();
					planeSensor -> maxPosition ()  = Vector2d (-1, 0);
				}
				else
				{
					// Translate along plane

					const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (normal));

					planeSensor -> enabled ()      = not paintSelection ();
					planeSensor -> axisRotation () = axisRotation;
					planeSensor -> offset ()       = Vector3d ();
					planeSensor -> maxPosition ()  = Vector2d (-1, -1);
				}
		
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
	if (not select ())
	   return;

	if (active)
	{
		// Create undo step for translation.

		undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Translate %s »point«"), getCoord () -> getTypeName () .c_str ()));

	   undoSetCoordPoint (undoStep);
	}
	else
	{
		setTranslate (false);

		redoSetCoordPoint (undoStep);

		// Reset fields and send undo step.

		if (abs (translation))
			undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);

		translation   = Vector3d ();
		translations  = 0;
	}
}

void
X3DIndexedFaceSetTransformObject::set_plane_sensor_translation ()
{
	if (not select ())
	   return;

	// Prevent accidentially move.
	if (translations ++ < TRANSLATIONS_EVENTS)
	   return;

	translation = planeSensor -> translation_changed () .getValue ();

	if (abs (translation))
	{
		setTranslate (true);

		for (const auto & pair : getSelectedPoints ())
			getCoord () -> set1Point (pair .first, pair .second + translation);
	}
}

X3DIndexedFaceSetTransformObject::~X3DIndexedFaceSetTransformObject ()
{ }

} // X3D
} // titania

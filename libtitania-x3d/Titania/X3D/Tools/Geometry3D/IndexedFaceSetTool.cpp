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

#include "IndexedFaceSetTool.h"

#include "../Rendering/CoordinateTool.h"

#include "../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../Editing/Selection/FaceSelection.h"

namespace titania {
namespace X3D {

IndexedFaceSetTool::Fields::Fields () :
	 mergePoints (new SFTime ()),
	 splitPoints (new SFTime ())
{ }

IndexedFaceSetTool::IndexedFaceSetTool (IndexedFaceSet* const node) :
	                     X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	                  IndexedFaceSet (node -> getExecutionContext ()),
	                     X3DBaseTool (node),
	     X3DComposedGeometryNodeTool (),
	X3DIndexedFaceSetSelectionObject (),
	                          fields (),
	                     touchSensor (),
	                     planeSensor (),
	                     translation ()
{
	addType (X3DConstants::IndexedFaceSetTool);

	mergePoints () .isHidden (true);
	splitPoints () .isHidden (true);

	addField (inputOutput, "pickable",         pickable ());
	addField (inputOutput, "selectable",       selectable ());
	addField (inputOnly,   "set_selection",    set_selection ());
	addField (inputOutput, "replaceSelection", replaceSelection ());
	addField (inputOutput, "mergePoints",      mergePoints ());
	addField (inputOutput, "splitPoints",      splitPoints ());
	addField (inputOutput, "normalTool",       normalTool ());
	addField (inputOutput, "coordTool",        coordTool ());

	addChildren (touchSensor,
	             planeSensor);
}

void
IndexedFaceSetTool::initialize ()
{
	X3DComposedGeometryNodeTool::initialize ();
	X3DIndexedFaceSetSelectionObject::initialize ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &IndexedFaceSetTool::set_loadState);

	mergePoints () .addInterest (this, &IndexedFaceSetTool::set_mergePoints);
	splitPoints () .addInterest (this, &IndexedFaceSetTool::set_splitPoints);
}

void
IndexedFaceSetTool::set_loadState ()
{
	try
	{
		const auto & inlineNode         = getCoordinateTool () -> getInlineNode ();
		const auto   activeFaceGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("ActiveFaceGeometry");

		planeSensor = inlineNode -> getExportedNode <PlaneSensor> ("PlaneSensor");
		touchSensor = inlineNode -> getExportedNode <TouchSensor> ("TouchSensor");

		touchSensor -> hitPoint_changed () .addInterest (this, &IndexedFaceSetTool::set_touch_sensor_hitPoint);

		planeSensor -> isActive ()            .addInterest (this, &IndexedFaceSetTool::set_plane_sensor_active);
		planeSensor -> translation_changed () .addInterest (this, &IndexedFaceSetTool::set_plane_sensor_translation);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
IndexedFaceSetTool::set_touch_sensor_hitPoint ()
{
	if (planeSensor -> isActive ())
		return;

	// Setup PlaneSensor

	switch (getActivePoints () .size ())
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

			planeSensor -> enabled ()      = replaceSelection ();
			planeSensor -> axisRotation () = axisRotation;
			planeSensor -> maxPosition ()  = Vector2f (-1, -1);
			break;
		}
		case 2:
		{
			// Translate along edge

			const auto vector       = getCoord () -> get1Point (getActivePoints () [0]) - getCoord () -> get1Point (getActivePoints () [1]);
			const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), vector);

			planeSensor -> enabled ()      = replaceSelection ();
			planeSensor -> axisRotation () = axisRotation;
			planeSensor -> maxPosition ()  = Vector2f (-1, 0);
			break;
		}
		default:
		{
			// Translate along face normal
			const auto normal       = getPolygonNormal (getFaceSelection () -> getFaceVertices (getActiveFace ()));
			const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (normal));

			planeSensor -> enabled ()      = replaceSelection ();
			planeSensor -> axisRotation () = axisRotation;
			planeSensor -> maxPosition ()  = Vector2f (-1, -1);
			break;
		}
	}
}

void
IndexedFaceSetTool::set_plane_sensor_active (const bool active)
{
	if (not active)
		selectable () = true;
}

void
IndexedFaceSetTool::set_plane_sensor_translation (const Vector3f & translation)
{
	if (abs (translation) and selectable ())
		selectable () = false;

	for (const auto & pair : getSelectedPoints ())
		getCoord () -> set1Point (pair .first, pair .second + Vector3d (translation));
}

void
IndexedFaceSetTool::set_mergePoints ()
{
	__LOG__ << std::endl;
}

void
IndexedFaceSetTool::set_splitPoints ()
{
	__LOG__ << std::endl;

	for (const auto selectedPoint : getSelectedPoints ())
	{
	   const auto & index   = selectedPoint .first;
	   const auto & point   = selectedPoint .second;
	   const auto   indices = getFaceSelection () -> getSharedVertices (index);

	   if (indices .empty ())
	      continue;

	   for (const auto & index : std::make_pair (indices .begin () + 1, indices .end ()))
	   {
	      __LOG__ << index << std::endl;

	      coordIndex () [index] = getCoord () -> getSize ();

	      getCoord () -> set1Point (getCoord () -> getSize (), point);
	   }
	}
}

void
IndexedFaceSetTool::dispose ()
{
	X3DIndexedFaceSetSelectionObject::dispose ();
	X3DComposedGeometryNodeTool::dispose ();
}

IndexedFaceSetTool::~IndexedFaceSetTool ()
{ }

} // X3D
} // titania

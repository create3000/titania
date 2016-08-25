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

#include "X3DIndexedFaceSetLightSaberObject.h"

#include "../../../Editing/Selection/FaceSelection.h"
#include "../../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

X3DIndexedFaceSetLightSaberObject::X3DIndexedFaceSetLightSaberObject () :
	            IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetCutObject ()
{
	//addType (X3DConstants::X3DIndexedFaceSetLightSaberObject);
}

/**
 *  Cuts all faces intersecting with @a cutLine into two connected faces.
 *
 *  @param  cutLine  Line on viewport
 */

bool
X3DIndexedFaceSetLightSaberObject::cut (const Line2d & cutLine)
{
	try
	{
		const auto modelViewProjection  = getModelViewMatrix () * getProjectionMatrix ();
		const auto viewport             = getViewport ();
		auto       intersectingFaces    = std::vector <size_t> ();
		auto       intersectingVertices = std::vector <std::vector <size_t>> ();
		auto       intersectingPoints   = std::vector <std::vector <Vector2d>> ();
	
		for (const auto & face : getFaceSelection () -> getFaces ())
		{
			auto vertices     = getFaceSelection () -> getFaceVertices (face);
			auto screenPoints = std::vector <Vector2d> ();
	
			for (const auto & vertex : vertices)
			{
				const auto point       = getCoord () -> get1Point (coordIndex () [vertex]);
				const auto screenPoint = ViewVolume::projectPoint (point, modelViewProjection, viewport);

				screenPoints .emplace_back (screenPoint .x (), screenPoint .y ());
			}
	
			if (cutLine .intersects (screenPoints .begin (), screenPoints .end ()))
			{
				intersectingFaces    .emplace_back (face);
				intersectingVertices .emplace_back (std::move (vertices));
				intersectingPoints   .emplace_back (std::move (screenPoints));
			}
		}
	
		if (intersectingFaces .empty ())
			return false;

		const auto invModelViewProjection = inverse (modelViewProjection);
		auto       intersectingEdges      = std::vector <std::vector <std::vector <int32_t>>> ();
		auto       cutPoints              = std::vector <std::vector <Vector3d>> ();

		for (size_t i = 0, sizeI = intersectingPoints .size (); i < sizeI; ++ i)
		{
			const auto & vertices     = intersectingVertices [i];
			const auto & screenPoints = intersectingPoints [i];
			auto         edges        = std::vector <std::vector <int32_t>> ();
			auto         points       = std::vector <Vector3d> (); // cut points for edge

			for (size_t k = 0, sizeK = screenPoints .size (); k < sizeK; ++ k)
			{
				const auto index1       = k;
				const auto index2       = (k + 1) % sizeK;
				const auto lineSegment  = LineSegment2d (screenPoints [index1], screenPoints [index2]);
				auto       intersection = Vector2d ();

				if (lineSegment .intersects (cutLine, intersection))
				{
					const auto point1   = coordIndex () [vertices [index1]];
					const auto point2   = coordIndex () [vertices [index2]];
					const auto line     = Line3d (getCoord () -> get1Point (point1), getCoord () -> get1Point (point2), points_type ());
					const auto ray      = ViewVolume::unProjectRay (intersection .x (), intersection .y (), invModelViewProjection, viewport);
					auto       cutPoint = Vector3d ();

					line .closest_point (ray, cutPoint);

					points .emplace_back (cutPoint);
					edges  .emplace_back (std::vector <int32_t> { point1, point2 });
				}
			}

			intersectingEdges .emplace_back (std::move (edges));
			cutPoints         .emplace_back (std::move (points));
		}

		// Now cut faces.

		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Cut Polygons"));
	
		undoRestoreSelection (undoStep);
		undoSetColorIndex    (undoStep);
		undoSetTexCoordIndex (undoStep);
		undoSetNormalIndex   (undoStep);
		undoSetCoordIndex    (undoStep);
		undoSetColorColor    (undoStep);
		undoSetTexCoordPoint (undoStep);
		undoSetNormalVector  (undoStep);
		undoSetCoordPoint    (undoStep);

		auto selection = X3DIndexedFaceSetCutObject::cut (intersectingFaces, cutPoints, intersectingEdges);
	
		if (selection .empty ())
			return false;
	
		// Remove degenerated edges and faces.
		rebuildIndices  ();
		rebuildColor    ();
		rebuildTexCoord ();
		rebuildNormal   ();
		rewriteArray (rebuildCoord (), selection);
	
		redoSetCoordPoint    (undoStep);
		redoSetNormalVector  (undoStep);
		redoSetTexCoordPoint (undoStep);
		redoSetColorColor    (undoStep);
		redoSetCoordIndex    (undoStep);
		redoSetNormalIndex   (undoStep);
		redoSetTexCoordIndex (undoStep);
		redoSetColorIndex    (undoStep);
		redoRestoreSelection ({ }, undoStep);

		replaceSelectedEdges () .assign (selection .begin (), selection .end ());

		undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);

		return true;
	}
	catch (const std::domain_error &)
	{
		// Catch exception from ViewVolume::projectPoint and matrix inverse.
		return false;
	}
}

//	std::vector <int32_t>
//	cut (const size_t, const std::pair <Vector3d, Vector3d> &, const std::vector <int32_t> &, const std::vector <int32_t> &);

X3DIndexedFaceSetLightSaberObject::~X3DIndexedFaceSetLightSaberObject ()
{ }

} // X3D
} // titania

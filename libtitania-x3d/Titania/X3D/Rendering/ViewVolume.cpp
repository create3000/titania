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

#include "ViewVolume.h"

#include "../Rendering/OpenGL.h"

#include <Titania/LOG.h>

namespace titania {
namespace X3D {

/*
 * p7 -------- p6
 * | \         | \
 * | p3 --------- p2
 * |  |        |  |
 * |  |        |  |
 * p4 |______ p5  |
 *  \ |         \ |
 *   \|          \|
 *    p0 -------- p1
 */

ViewVolume::ViewVolume () :
   scissor (),
	 planes (6),
	  valid (false)
{ }

ViewVolume::ViewVolume (const Matrix4d & projectionMatrix, const Vector4i & viewport, const Vector4i & scissor) :
	viewport (viewport),
	 scissor (scissor),
	  points (),
	 normals (),
	   edges (),
	  planes (),
	   valid (true)
{
	try
	{
		const double x1     = scissor [0];
		const double x2     = scissor [0] + scissor [2];
		const double y1     = scissor [1];
		const double y2     = scissor [1] + scissor [3];
		const auto   matrix = inverse (projectionMatrix);

		points .reserve (8);
		points .emplace_back (unProjectPoint (x1, y1, 0, matrix, viewport));
		points .emplace_back (unProjectPoint (x2, y1, 0, matrix, viewport));
		points .emplace_back (unProjectPoint (x2, y2, 0, matrix, viewport));
		points .emplace_back (unProjectPoint (x1, y2, 0, matrix, viewport));
		points .emplace_back (unProjectPoint (x1, y1, 1, matrix, viewport));
		points .emplace_back (unProjectPoint (x2, y1, 1, matrix, viewport));
		points .emplace_back (unProjectPoint (x2, y2, 1, matrix, viewport));
		points .emplace_back (unProjectPoint (x1, y2, 1, matrix, viewport));

		normals .reserve (6);
		normals .emplace_back (Triangle3d (points [0], points [1], points [2]) .normal ());  // front
		normals .emplace_back (Triangle3d (points [7], points [4], points [0]) .normal ());  // left
		normals .emplace_back (Triangle3d (points [6], points [2], points [1]) .normal ());  // right
		normals .emplace_back (Triangle3d (points [2], points [6], points [7]) .normal ());  // top
		normals .emplace_back (Triangle3d (points [1], points [0], points [4]) .normal ());  // bottom
		normals .emplace_back (Triangle3d (points [4], points [7], points [6]) .normal ());  // back

		planes .reserve (6);
		planes .emplace_back (points [1], normals [0]);  // front
		planes .emplace_back (points [4], normals [1]);  // left
		planes .emplace_back (points [2], normals [2]);  // right
		planes .emplace_back (points [6], normals [3]);  // top
		planes .emplace_back (points [0], normals [4]);  // bottom
		planes .emplace_back (points [7], normals [5]);  // back
	}
	catch (const std::domain_error & error)
	{
		valid = false;
	}
}

const std::vector <Vector3d> &
ViewVolume::getEdges () const
{
	if (edges .empty ())
	{
		edges .reserve (12);
		edges .emplace_back (points [0] - points [1]);
		edges .emplace_back (points [1] - points [2]);
		edges .emplace_back (points [2] - points [3]);
		edges .emplace_back (points [3] - points [0]);

		edges .emplace_back (points [0] - points [4]);
		edges .emplace_back (points [1] - points [5]);
		edges .emplace_back (points [2] - points [6]);
		edges .emplace_back (points [3] - points [7]);

		edges .emplace_back (points [4] - points [5]);
		edges .emplace_back (points [5] - points [6]);
		edges .emplace_back (points [6] - points [7]);
		edges .emplace_back (points [7] - points [4]);
	}

	return edges;
}

bool
ViewVolume::intersects (const double radius, const Vector3d & center) const
{
	if (valid)
	{
		for (const auto & plane : planes)
		{
			if (plane .distance (center) > radius)
				return false;
		}
	}

	return true;
}

bool
ViewVolume::intersects (const Box3d & box) const
{
	// Get points.

	const auto   points1 = box .points ();
	const auto & points2 = points;

	// Test the three planes spanned by the normal vectors of the faces of the box.

	const auto normals1 = box .normals ();

	if (sat::separated (std::vector <Vector3d> (normals1 .begin (), normals1 .end ()), points1, points2))
		return false;

	const auto & normals2 = normals;

	// Test the six planes spanned by the normal vectors of the faces of the view volume.

	if (sat::separated (normals2, points1, points2))
		return false;

	// Test the planes spanned by the edges of each object.

	std::vector <Vector3d> axes;

	for (const auto & axis1 : box .axes ())
	{
		for (const auto & axis2 : getEdges ())
			axes .emplace_back (cross (axis1, axis2));
	}

	if (sat::separated (axes, points1, points2))
		return false;

	// Both boxes intersect.

	return true;
}

// http://www.opengl.org/wiki/GluProject_and_gluUnProject_code

///  throws std::domain_error
Vector3d
ViewVolume::unProjectPoint (const double winx, const double winy, const double winz,
                            const Matrix4d & modelViewMatrix,
                            const Matrix4d & projectionMatrix,
                            const Vector4i & viewport)
{
	const Matrix4d invModelViewProjection = inverse (modelViewMatrix * projectionMatrix);

	return unProjectPoint (winx, winy, winz, invModelViewProjection, viewport);
}

///  throws std::domain_error
Vector3d
ViewVolume::unProjectPoint (const double winx, const double winy, const double winz, const Matrix4d & invModelViewProjection, const Vector4i & viewport)
{
	// Transformation of normalized coordinates between -1 and 1
	Vector4d in ((winx - viewport [0]) / viewport [2] * 2 - 1,
	             (winy - viewport [1]) / viewport [3] * 2 - 1,
	             2 * winz - 1,
	             1);

	//Objects coordinates
	in = in * invModelViewProjection;

	if (in .w () == 0)
		throw std::domain_error ("Couldn't unproject point: divisor is 0.");

	const double d = 1 / in .w ();

	return Vector3d (in .x () * d, in .y () * d, in . z () * d);
}

///  throws std::domain_error
Line3d
ViewVolume::unProjectRay (const Vector2d & point,
                          const Matrix4d & modelViewMatrix,
                          const Matrix4d & projectionMatrix,
                          const Vector4i & viewport)
{
	return unProjectRay (point, inverse (modelViewMatrix * projectionMatrix), viewport);
}

///  throws std::domain_error
Line3d
ViewVolume::unProjectRay (const Vector2d & point, const Matrix4d & invModelViewProjection, const Vector4i & viewport)
{
	const Vector3f near = ViewVolume::unProjectPoint (point .x (), point .y (), 0.0, invModelViewProjection, viewport);
	const Vector3f far  = ViewVolume::unProjectPoint (point .x (), point .y (), 0.9, invModelViewProjection, viewport);

	return Line3d (near, far, points_type ());
}

///  throws std::domain_error
Vector3d
ViewVolume::projectPoint (const Vector3d & point, const Matrix4d & modelViewMatrix, const Matrix4d & projectionMatrix, const Vector4i & viewport)
{
	return projectPoint (point, modelViewMatrix * projectionMatrix, viewport);
}

///  throws std::domain_error
Vector3d
ViewVolume::projectPoint (const Vector3d & point, const Matrix4d & modelViewProjection, const Vector4i & viewport)
{
	Vector4d in (point .x (), point .y (), point .z (), 1);

	in = in * modelViewProjection;

	if (in .w () == 0)
		throw std::domain_error ("Couldn't project point: divisor is 0.");

	const double d = 1 / in .w ();

	return Vector3d ((in .x () * d / 2 + 0.5) * viewport [2] + viewport [0],
	                 (in .y () * d / 2 + 0.5) * viewport [3] + viewport [1],
	                 (1 + in .z () * d) / 2);
}

///  throws std::domain_error
Line2d
ViewVolume::projectLine (const Line3d & line, const Matrix4d & modelViewMatrix, const Matrix4d & projectionMatrix, const Vector4i & viewport)
{
	const auto modelViewProjection = modelViewMatrix * projectionMatrix;

	Vector3d point1 = ViewVolume::projectPoint (line .point (),                            modelViewProjection, viewport);
	Vector3d point2 = ViewVolume::projectPoint (line .point () + line .direction () * 1e9, modelViewProjection, viewport);

	return Line2d (Vector2d (point1 .x (), point1 .y ()),
	               Vector2d (point2 .x (), point2 .y ()),
	               points_type ());
}

} // X3D
} // titania

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
 * p1 -------- p6
 * | \         | \
 * |   ---------- p5
 * |  |        |  |
 * |  |        |  |
 * p2 |________|  |
 *  \ |         \ |
 *   \|          \|
 *    p3 -------- p4
 */

ViewVolume::ViewVolume () :
   scissor (),
	 planes (6),
	  valid (false)
{ }

ViewVolume::ViewVolume (const Matrix4d & projectionMatrix, const Vector4i & viewport, const Vector4i & scissor) :
	viewport (viewport),
	 scissor (scissor),
	  planes (),
	   valid (true)
{
	try
	{
		const int32_t x1 = scissor [0];
		const int32_t x2 = scissor [0] + scissor [2];
		const int32_t y1 = scissor [1];
		const int32_t y2 = scissor [1] + scissor [3];

		const Matrix4d matrix = inverse (projectionMatrix);

		Vector3f p1 = unProjectPoint (x1, y2, 1, matrix, viewport);
		Vector3f p2 = unProjectPoint (x1, y1, 1, matrix, viewport);
		Vector3f p3 = unProjectPoint (x1, y1, 0, matrix, viewport);
		Vector3f p4 = unProjectPoint (x2, y1, 0, matrix, viewport);
		Vector3f p5 = unProjectPoint (x2, y2, 0, matrix, viewport);
		Vector3f p6 = unProjectPoint (x2, y2, 1, matrix, viewport);

		planes .reserve (6);
		planes .emplace_back (p4, normalize (cross (p3 - p4, p5 - p4)));  // front
		planes .emplace_back (p1, normalize (cross (p2 - p1, p6 - p1)));  // back
		planes .emplace_back (p2, normalize (cross (p1 - p2, p3 - p2)));  // left
		planes .emplace_back (p5, normalize (cross (p6 - p5, p4 - p5)));  // right
		planes .emplace_back (p6, normalize (cross (p5 - p6, p1 - p6)));  // top
		planes .emplace_back (p3, normalize (cross (p4 - p3, p2 - p3)));  // bottom
	}
	catch (const std::domain_error & error)
	{
		valid = false;
	}
}

bool
ViewVolume::intersects (const Box3d & bbox) const
{
	if (valid)
	{
		const double nradius = math::abs (bbox .size ()) / 2;

		for (const auto & plane : planes)
		{
			if (plane .distance (bbox .center ()) + nradius < 0)
				return false;
		}
	}

	return true;
}

// http://www.opengl.org/wiki/GluProject_and_gluUnProject_code

Vector3d
ViewVolume::unProjectPoint (double winx, double winy, double winz,
                            const Matrix4d & modelViewMatrix,
                            const Matrix4d & projectionMatrix,
                            const Vector4i & viewport)
throw (std::domain_error)
{
	const Matrix4d invModelViewProjection = inverse (modelViewMatrix * projectionMatrix);

	return unProjectPoint (winx, winy, winz, invModelViewProjection, viewport);
}

Vector3d
ViewVolume::unProjectPoint (double winx, double winy, double winz, const Matrix4d & invModelViewProjection, const Vector4i & viewport)
throw (std::domain_error)
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

Line3d
ViewVolume::unProjectRay (double winx, double winy,
                          const Matrix4d & modelViewMatrix,
                          const Matrix4d & projectionMatrix,
                          const Vector4i & viewport)
throw (std::domain_error)
{
	return unProjectRay (winx, winy, inverse (modelViewMatrix * projectionMatrix), viewport);
}

Line3d
ViewVolume::unProjectRay (double winx, double winy, const Matrix4d & invModelViewProjection, const Vector4i & viewport)
throw (std::domain_error)
{
	const Vector3f near = ViewVolume::unProjectPoint (winx, winy, 0.0, invModelViewProjection, viewport);
	const Vector3f far  = ViewVolume::unProjectPoint (winx, winy, 0.9, invModelViewProjection, viewport);

	return Line3d (near, far, points_type ());
}

Vector3d
ViewVolume::projectPoint (const Vector3d & point, const Matrix4d & modelViewMatrix, const Matrix4d & projectionMatrix, const Vector4i & viewport)
throw (std::domain_error)
{
	return projectPoint (point, modelViewMatrix * projectionMatrix, viewport);
}

Vector3d
ViewVolume::projectPoint (const Vector3d & point, const Matrix4d & modelViewProjection, const Vector4i & viewport)
throw (std::domain_error)
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

Line2d
ViewVolume::projectLine (const Line3d & line, const Matrix4d & modelViewMatrix, const Matrix4d & projectionMatrix, const Vector4i & viewport)
throw (std::domain_error)
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

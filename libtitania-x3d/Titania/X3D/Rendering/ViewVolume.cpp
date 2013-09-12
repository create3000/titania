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
	ViewVolume (ModelViewMatrix4d (), ProjectionMatrix4d ())
{ }

ViewVolume::ViewVolume (const Matrix4d & modelview) :
	ViewVolume (modelview, ProjectionMatrix4d ())
{ }

ViewVolume::ViewVolume (const Matrix4d & modelview, const Matrix4d & projection) :
	    planes (),
	     valid (true)
{
	Vector4i viewport = Viewport4i ();

	try
	{
		Matrix4d matrix = ~(modelview * projection);

		Vector3f p1 = unProjectPoint (0, viewport [3], 1, matrix, viewport);
		Vector3f p2 = unProjectPoint (0, 0, 1, matrix, viewport);
		Vector3f p3 = unProjectPoint (0, 0, 0, matrix, viewport);
		Vector3f p4 = unProjectPoint (viewport [2], 0, 0, matrix, viewport);
		Vector3f p5 = unProjectPoint (viewport [2], viewport [3], 0, matrix, viewport);
		Vector3f p6 = unProjectPoint (viewport [2], viewport [3], 1, matrix, viewport);

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
ViewVolume::intersect (const Box3f & bbox) const
{
	if (valid)
	{
		float nradius = math::abs (bbox .size ()) * 0.5f;

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
ViewVolume::unProjectPoint (double winx, double winy, double winz, const Matrix4d & modelview, const Matrix4d & projection, const Vector4i & viewport)
{
	// Calculation for inverting a matrix, compute projection x modelview
	// and store in A
	Matrix4d matrix = ~(modelview * projection);

	return unProjectPoint (winx, winy, winz, matrix, viewport);
}

Vector3d
ViewVolume::unProjectPoint (double winx, double winy, double winz, const Matrix4d & matrix, const Vector4i & viewport)
{
	// Transformation of normalized coordinates between -1 and 1
	Vector4d in ((winx - viewport [0]) / viewport [2] * 2 - 1,
	             (winy - viewport [1]) / viewport [3] * 2 - 1,
	             2 * winz - 1,
	             1);

	//Objects coordinates
	Vector4d out = in * matrix;

	if (out [3] == 0)
		throw std::domain_error ("Couldn't project point: divisor is 0.");

	double d = 1 / out [3];

	return Vector3d (out [0] * d, out [1] * d, out [2] * d);
}

Line3d
ViewVolume::unProjectLine (double winx, double winy, const Matrix4d & modelview, const Matrix4d & projection, const Vector4i & viewport)
{
	Matrix4d matrix = ~(modelview * projection);

	Vector3f near = ViewVolume::unProjectPoint (winx, winy, 0.0, matrix, viewport);
	Vector3f far  = ViewVolume::unProjectPoint (winx, winy, 0.9, matrix, viewport);

	return Line3d (near, far, true);
}

} // X3D
} // titania

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

#include "../Rendering/Matrix.h"

#include <GL/glu.h>

#include <GL/gl.h>

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

ViewVolume::ViewVolume (const Matrix4d & modelview, const Matrix4d & projection)
{
	GLint viewport [4];

	glGetIntegerv (GL_VIEWPORT, viewport);

	GLdouble x, y, z;

	gluUnProject (0, viewport [3], 1, modelview .data (), projection .data (), viewport, &x, &y, &z);
	Vector3f p1 = Vector3f (x, y, z);

	gluUnProject (0, 0, 1, modelview .data (), projection .data (), viewport, &x, &y, &z);
	Vector3f p2 = Vector3f (x, y, z);

	gluUnProject (0, 0, 0, modelview .data (), projection .data (), viewport, &x, &y, &z);
	Vector3f p3 = Vector3f (x, y, z);

	gluUnProject (viewport [2], 0, 0, modelview .data (), projection .data (), viewport, &x, &y, &z);
	Vector3f p4 = Vector3f (x, y, z);

	gluUnProject (viewport [2], viewport [3], 0, modelview .data (), projection .data (), viewport, &x, &y, &z);
	Vector3f p5 = Vector3f (x, y, z);

	gluUnProject (viewport [2], viewport [3], 1, modelview .data (), projection .data (), viewport, &x, &y, &z);
	Vector3f p6 = Vector3f (x, y, z);

	planes .reserve (6);
	planes .push_back (Plane3f (p4, normalize (cross (p3 - p4, p5 - p4))));  // front
	planes .push_back (Plane3f (p1, normalize (cross (p2 - p1, p6 - p1))));  // back
	planes .push_back (Plane3f (p2, normalize (cross (p1 - p2, p3 - p2))));  // left
	planes .push_back (Plane3f (p5, normalize (cross (p6 - p5, p4 - p5))));  // right
	planes .push_back (Plane3f (p6, normalize (cross (p5 - p6, p1 - p6))));  // top
	planes .push_back (Plane3f (p3, normalize (cross (p4 - p3, p2 - p3))));  // bottom
}

bool
ViewVolume::intersect (const Box3f & bbox) const
{
	float nradius = math::abs (bbox .size ()) * 0.5f;

	for (const auto & plane : planes)
	{
		if (plane .distance (bbox .center ()) + nradius < 0)
			return false;
	}

	return true;
}

} // X3D
} // titania

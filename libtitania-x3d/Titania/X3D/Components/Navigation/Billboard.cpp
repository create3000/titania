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

#include "Billboard.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/Matrix.h"

namespace titania {
namespace X3D {

// https://bitbucket.org/Coin3D/coin/src/abc9f50968c9/src/vrml97/Billboard.cpp

static constexpr Vector3f xAxis (1, 0, 0);
static constexpr Vector3f yAxis (0, 1, 0);
static constexpr Vector3f zAxis (0, 0, 1);

Billboard::Fields::Fields () :
	axisOfRotation (new SFVec3f (yAxis))
{ }

Billboard::Billboard (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	         fields ()
{
	setComponent ("Navigation");
	setTypeName ("Billboard");

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "axisOfRotation", axisOfRotation ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
}

X3DBaseNode*
Billboard::create (X3DExecutionContext* const executionContext) const
{
	return new Billboard (executionContext);
}

Rotation4f
rotation (const Vector3f & fromVector, const Vector3f & toVector)
{
	Vector3f from (normalize (fromVector));
	Vector3f to (normalize (toVector));

	const float cos_angle = dot (from, to);
	Vector3f    crossvec  = normalize (cross (from, to));
	const float crosslen  = abs (crossvec);

	if (crosslen == 0)
	{
		// Parallel vectors
		// Check if they are pointing in the same direction.
		if (cos_angle > 0)
			return Rotation4f ();

		// Ok, so they are parallel and pointing in the opposite direction
		// of each other.
		else
		{
			// Try crossing with x axis.
			Vector3f t = cross (from, Vector3f (1, 0, 0));

			// If not ok, cross with y axis.
			if (norm (t) == 0)
				t = cross (from, Vector3f (0, 1, 0));

			t .normalize ();

			return Rotation4f (math::quaternion <float> (t [0], t [1], t [2], 0));
		}
	}
	else
	{
		// Vectors are not parallel
		crossvec *= std::sqrt (0.5 * std::abs (1 - cos_angle));
		return Rotation4f (math::quaternion <float> (crossvec [0], crossvec [1], crossvec [2], std::sqrt (0.5 * std::abs (1 + cos_angle))));
	}
}

void
Billboard::rotate (TraverseType type)
{
	try
	{
		Matrix4f modelViewMatrix = ModelViewMatrix4f ();

		if (type == TraverseType::CAMERA)
			modelViewMatrix *= getInverseCameraSpaceMatrix ();

		Matrix4f inverseModelViewMatrix = ~modelViewMatrix;

		Vector3f billboardToViewer = inverseModelViewMatrix .translation ();

		if (axisOfRotation () == Vector3f ())
		{
			Vector3f viewerYAxis = yAxis * inverseModelViewMatrix;

			Vector3f x = cross (viewerYAxis, billboardToViewer);
			Vector3f y = cross (billboardToViewer, x);
			Vector3f z = billboardToViewer;

			//

			x .normalize ();
			y .normalize ();
			z .normalize ();

			Matrix4f rotation (x [0], x [1], x [2], 0,
			                   y [0], y [1], y [2], 0,
			                   z [0], z [1], z [2], 0,
			                   0,     0,     0,     1);

			glMultMatrixf (rotation .data ());
		}
		else
		{
			Vector3f vector = cross (axisOfRotation () .getValue (), billboardToViewer);           // direction vector of plane
			float    angle  = std::acos (dot (zAxis, normalize (axisOfRotation () .getValue ()))); // angle between zAxis and axisOfRotation

			Vector3f z = Rotation4f (vector, angle) * axisOfRotation () .getValue ();              // intersection of zAxis and plane

			glMultMatrixf (Matrix4f (Rotation4f (zAxis, z)) .data ());                             // rotate zAxis in plane
		}
	}
	catch (const std::domain_error &)
	{ }
}

void
Billboard::traverse (TraverseType type)
{
	glPushMatrix ();

	rotate (type);

	X3DGroupingNode::traverse (type);

	glPopMatrix ();
}

} // X3D
} // titania

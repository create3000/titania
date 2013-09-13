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

#include "OrthoViewpoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

OrthoViewpoint::Fields::Fields () :
	   position (new SFVec3f (0, 0, 10)),
	fieldOfView (new MFFloat ({ -1, -1, 1, 1 }))
{ }

OrthoViewpoint::OrthoViewpoint (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DViewpointNode (),
	          fields ()
{
	setComponent ("Navigation");
	setTypeName ("OrthoViewpoint");

	addField (inputOutput, "metadata",          metadata ());
	addField (inputOutput, "description",       description ());
	addField (inputOnly,   "set_bind",          set_bind ());
	addField (inputOutput, "position",          position ());
	addField (inputOutput, "orientation",       orientation ());
	addField (inputOutput, "centerOfRotation",  centerOfRotation ());
	addField (inputOutput, "fieldOfView",       fieldOfView ());
	addField (inputOutput, "jump",              jump ());
	addField (inputOutput, "retainUserOffsets", retainUserOffsets ());
	addField (outputOnly,  "isBound",           isBound ());
	addField (outputOnly,  "bindTime",          bindTime ());
}

X3DBaseNode*
OrthoViewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new OrthoViewpoint (executionContext);
}

Vector3f
OrthoViewpoint::getPosition () const
{
	return position ();
}

float
OrthoViewpoint::getMinimumX () const
{
	return fieldOfView () .size () > 0 ? fieldOfView () [0] : -1;
}

float
OrthoViewpoint::getMinimumY () const
{
	return fieldOfView () .size () > 1 ? fieldOfView () [1] : -1;
}

float
OrthoViewpoint::getMaximumX () const
{
	return fieldOfView () .size () > 2 ? fieldOfView () [2] : 1;
}

float
OrthoViewpoint::getMaximumY () const
{
	return fieldOfView () .size () > 3 ? fieldOfView () [3] : 1;
}

Vector3f
OrthoViewpoint::getScreenScale (float, const Vector4i & viewport) const
{
	int width  = viewport [2];
	int height = viewport [3];

	if (width > height)
		return Vector3f (getSizeX () / height, getSizeY () / height, 1);

	return Vector3f (getSizeX () / width, getSizeY () / width, 1);
}

Vector2f
OrthoViewpoint::getViewportSize (const Vector4i & viewport) const
{
	int width  = viewport [2];
	int height = viewport [3];

	if (width > height)
	{
		return Vector2f (width * getSizeX () / height,
		                 getSizeY ());
	}
	else
	{
		return Vector2f (getSizeX (),
		                 height * getSizeY () / width);
	}
}

Vector3f
OrthoViewpoint::getLookAtPositionOffset (Box3f bbox) const
{
	return bbox .center ()
	       + getUserOrientation () * (Vector3f (0, 0, abs (bbox .size ()) * 0.5 + 10))
	       - position ();
}

void
OrthoViewpoint::reshape (const float zNear, const float zFar)
{
	//	glMatrixMode (GL_PROJECTION);
	//	glLoadIdentity ();
	//
	//	Vector2f size_2 = getViewportSize (Viewport4i ()) * 0.5f;
	//
	//	glOrtho (-size_2 .x (), size_2 .x (), -size_2 .y (), size_2 .y (), zNear, zFar);
	//
	//	glMatrixMode (GL_MODELVIEW);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	
	Vector4i viewport = Viewport4i ();

	int width  = viewport [2];
	int height = viewport [3];

	Vector2f size = getViewportSize (viewport);

	if (width > height)
	{
		float left = getMinimumX () + (getMaximumX () - getMinimumX ()) / 2 - size .x () / 2;

		glOrtho (left, left + size .x (), getMinimumY (), getMaximumY (), zNear, zFar);
	}
	else
	{
		float bottom = getMinimumY () + (getMaximumY () - getMinimumY ()) / 2 - size .y () / 2;

		glOrtho (getMinimumX (), getMaximumX (), bottom, bottom + size .y (), zNear, zFar);
	}

	glMatrixMode (GL_MODELVIEW);
}

} // X3D
} // titania

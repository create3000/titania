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
 ******************************************************************************/

#include "Viewpoint.h"

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include <iostream>

namespace titania {
namespace X3D {

Viewpoint::Viewpoint (X3DExecutionContext* const executionContext) :
	    X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	X3DViewpointNode (),                                                    
	     fieldOfView (0.785398),                                            // SFFloat [in,out] fieldOfView        π/4           (0,π)
	        position (0, 0, 10)                                             // SFVec3f [in,out] position           0 0 10        (-∞,∞)
{
	setComponent ("Navigation");
	setTypeName ("Viewpoint");

	appendField (inputOutput, "metadata",          metadata);
	appendField (inputOnly,   "set_bind",          set_bind);
	appendField (inputOutput, "fieldOfView",       fieldOfView);
	appendField (inputOutput, "jump",              jump);
	appendField (inputOutput, "retainUserOffsets", retainUserOffsets);
	appendField (inputOutput, "position",          position);
	appendField (inputOutput, "orientation",       orientation);
	appendField (inputOutput, "centerOfRotation",  centerOfRotation);
	appendField (inputOutput, "description",       description);
	appendField (outputOnly,  "bindTime",          bindTime);
	appendField (outputOnly,  "isBound",           isBound);
}

X3DBasicNode*
Viewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new Viewpoint (executionContext);
}

void
Viewpoint::initialize ()
{
	X3DViewpointNode::initialize ();

	jump .addInterest (this, &Viewpoint::set_jump);

	set_jump ();
}

void
Viewpoint::set_jump ()
{
	if (not jump)
	{
		Matrix4f differenceMatrix;
		differenceMatrix .translate (position);
		differenceMatrix .rotate (orientation);
		setDifferenceMatrix (differenceMatrix);
	}
}

void
Viewpoint::display ()
{
	//	std::clog << "### 1: " << (void*) getBrowser () -> getLayer () << std::endl;
	//	std::clog << "### 2: " << getBrowser () -> getLayer () -> currentViewpoint << std::endl;

	const Matrix4f & currentMatrix = getBrowser () -> getLayer () -> getActiveViewpoint () -> getCurrentMatrix ();
	Matrix4f         matrix        = currentMatrix .multLeft (ModelViewMatrix4f ());

	setTransformationMatrix (matrix);

	if (jump)
	{
		matrix .translate (position);
		matrix .rotate (orientation);
		setMatrix (matrix);
	}
	else
	{
		setMatrix (matrix .multLeft (getDifferenceMatrix ()));
	}
}

void
Viewpoint::reshape (const float zNear, const float zFar)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	GLfloat viewport [4];
	glGetFloatv (GL_VIEWPORT, viewport);

	GLfloat width  = viewport [2];
	GLfloat height = viewport [3];

	float ratio = std::tan (fieldOfView / 2) * zNear;

	if (width > height)
	{
		float aspect = width * ratio / height;
		glFrustum (-aspect, aspect, -ratio, ratio, zNear, zFar);
	}
	else
	{
		float aspect = height * ratio / width;
		glFrustum (-ratio, ratio, -aspect, aspect, zNear, zFar);
	}

	glMatrixMode (GL_MODELVIEW);
}

} // X3D
} // titania

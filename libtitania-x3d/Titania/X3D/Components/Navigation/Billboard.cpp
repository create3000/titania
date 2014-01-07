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

#include "Billboard.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string Billboard::componentName  = "Navigation";
const std::string Billboard::typeName       = "Billboard";
const std::string Billboard::containerField = "children";

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

void
Billboard::rotate (const TraverseType type) const
{
	try
	{
		Matrix4f modelViewMatrix        = getModelViewMatrix (type);
		Matrix4f inverseModelViewMatrix = ~modelViewMatrix;

		Vector3f billboardToViewer = inverseModelViewMatrix .translation ();

		if (axisOfRotation () == Vector3f ())
		{
			Vector3f viewerYAxis = inverseModelViewMatrix .multDirMatrix (yAxis);

			Vector3f x = cross (viewerYAxis, billboardToViewer);
			Vector3f y = cross (billboardToViewer, x);
			Vector3f z = billboardToViewer;

			// Compose rotation

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
			Vector3f normal = cross (axisOfRotation () .getValue (), billboardToViewer); // normal vector of plane
			Vector3f vector = cross (axisOfRotation () .getValue (), zAxis);             // normal vector of plane between axisOfRotation and zAxis

			Rotation4f rotation (vector, normal);

			glMultMatrixf (Matrix4f (rotation) .data ());                                // rotate zAxis in plane
		}
	}
	catch (const std::domain_error &)
	{ }
}

void
Billboard::traverse (const TraverseType type)
{
	glPushMatrix ();

	rotate (type);

	X3DGroupingNode::traverse (type);

	glPopMatrix ();
}

} // X3D
} // titania

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

bool
intersect (const Plane3f & p1, const Plane3f & p2, Line3f & line)
{
	// http://stackoverflow.com/questions/6408670/intersection-between-two-planes
	
	line = Line3f (Vector3f (), cross (p1 .normal (), p2 .normal ()));

	return true;
}

void
Billboard::transform (TraverseType type)
{
	Matrix4f modelViewMatrix = ModelViewMatrix4f ();

	if (type == TraverseType::CAMERA)
		modelViewMatrix *= getCurrentViewpoint () -> getInverseTransformationMatrix ();

	Matrix4f inverseModelViewMatrix = ~modelViewMatrix;

	Vector3f billboardToViewer = -modelViewMatrix .translation () * inverseModelViewMatrix;

	if (axisOfRotation () == Vector3f ())
	{
		Vector3f localYAxis = yAxis * inverseModelViewMatrix;

		Vector3f v1 = cross (localYAxis, billboardToViewer);
		Vector3f v2 = cross (billboardToViewer, v1);
		Vector3f v3 = billboardToViewer;
		
		//

		v1 .normalize ();
		v2 .normalize ();
		v3 .normalize ();

		Matrix4f rotation (v1 [0], v1 [1], v1 [2], 0,
		                   v2 [0], v2 [1], v2 [2], 0,
		                   v3 [0], v3 [1], v3 [2], 0,
		                   0,      0,      0,      1);

		glMultMatrixf (rotation .data ());
	}
	else
	{
//		Vector3f v1 = axisOfRotation () .getValue ();
//		Vector3f v2 = cross (billboardToViewer, v1);
//		Vector3f v3 = cross (v1, v2);
//	
//		v1 .normalize ();
//		v2 .normalize ();
//		v3 .normalize ();
//
//		__LOG__ << v1 << " : " << v2 << " : " << v3 << std::endl;

		//

		Vector3f v2 = cross (billboardToViewer, axisOfRotation () .getValue ());
		Vector3f v3 = dot (normalize (billboardToViewer), zAxis) < 0 ? cross (v2, cross (v2, zAxis)) : cross (cross (v2, zAxis), v2);
		Vector3f v1 = cross (v2, v3);

		//
	
		v1 .normalize ();
		v2 .normalize ();
		v3 .normalize ();

//		__LOG__ << v1 << " : " << v2 << " : " << v3 << std::endl;
//		
//		__LOG__
//			<< dot (normalize (billboardToViewer), normalize (zAxis)) << " : "
//			<< dot (normalize (billboardToViewer), normalize (cross (v2, zAxis))) << " : "
//			<< dot (normalize (billboardToViewer), normalize (axisOfRotation () .getValue ())) << " : "
//			<< dot (normalize (billboardToViewer), normalize (v1)) << " : "
//			<< dot (normalize (billboardToViewer), normalize (v2)) << " : "
//			<< dot (normalize (billboardToViewer), normalize (v3)) << " : "
//			<< std::endl;

		Matrix4f rotation (v1 [0], v1 [1], v1 [2], 0,
		                   v2 [0], v2 [1], v2 [2], 0,
		                   v3 [0], v3 [1], v3 [2], 0,
		                   0,      0,      0,      1);

		glMultMatrixf (rotation .data ());

		glMultMatrixf (Matrix4f (Rotation4f (axisOfRotation () .getValue (), xAxis)) .data ());
	}
}

void
Billboard::traverse (TraverseType type)
{
	glPushMatrix ();

	transform (type);

	X3DGroupingNode::traverse (type);

	glPopMatrix ();
}

} // X3D
} // titania

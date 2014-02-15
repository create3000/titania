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

#include "TransformHandle.h"

#include "../Bits/config.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Grouping/Transform.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Rendering/PolygonModeContainer.h"

namespace titania {
namespace X3D {

TransformHandle::TransformHandle (Transform* const transform, SFBool* const isActive, X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	      Transform (executionContext),
	X3DHandleObject (),
	      transform (transform),
	       isActive (isActive),
	          scene (),
	   parentMatrix (),
	         matrix (),
	 interestEvents (transform -> isTainted ())
{
	for (auto & field : transform -> getFieldDefinitions ())
		addField (field -> getAccessType (), field -> getName (), *field);

	X3DChildObject::addChildren (scene);
	transform -> addParent (this);
}

void
TransformHandle::initialize ()
{
	Transform::initialize ();
	X3DHandleObject::initialize ();
	
	transform -> addInterest (this, &TransformHandle::interestsProcessed);

	try
	{
		scene = getBrowser () -> createX3DFromURL ({ get_tool ("TransformTool.wrl") .str () });

		const SFNode & handle = scene -> getNamedNode ("Handle");

		handle -> getField ("isActive") -> addInterest (*isActive);

		handle -> setField <SFNode> ("transform", transform);
	}
	catch (const X3DError & error)
	{
		std::clog << error .what () << std::endl;

		scene = getBrowser () -> createScene ();
		scene -> setup ();
	}
}

void
TransformHandle::setName (const std::string & value)
{
	transform -> setName (value);

	X3DHandleObject::setName (value);
}

void
TransformHandle::setUserData (const UserDataPtr & value)
{
	transform -> setUserData (value);

	X3DHandleObject::setUserData (value);
}

Box3f
TransformHandle::getBBox ()
{
	return transform -> getBBox ();
}

MFNode &
TransformHandle::getRootNodes ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return scene -> getRootNodes ();
}

const MFNode &
TransformHandle::getRootNodes () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return scene -> getRootNodes ();
}

void
TransformHandle::removeHandle ()
{
	transform -> removeHandle ();
}

// T  P
// P  T'
// I
//
// I * P * T * ~P = T'
//
// T = ~P * ~I * T' * P
//
// Add to group:
//
// childModelViewMatrix = childModelViewMatrix * ~groupModelViewMatrix
//
// transform -> setMatrix (childModelViewMatrix);

void
TransformHandle::addAbsoluteMatrix (const Matrix4f & absoluteMatrix)
{
	++ interestEvents;

	transform -> setMatrix (getMatrix () * parentMatrix * absoluteMatrix * ~parentMatrix);
}

void
TransformHandle::setMatrix (const Matrix4f & matrix)
{
	++ interestEvents;

	transform -> setMatrix (matrix);
}

void
TransformHandle::setMatrixWithCenter (const Matrix4f & matrix, const Vector3f & center)
{
	++ interestEvents;

	transform -> setMatrixWithCenter (matrix, center);
}

void
TransformHandle::interestsProcessed ()
{
	if (interestEvents)
		-- interestEvents;
		
	else
	{
		const auto differenceMatrix = ~(matrix * parentMatrix) * getMatrix () * parentMatrix;

		for (const auto & node : getBrowser () -> getSelection () -> getChildren ())
		{
			if (node == this)
				continue;
			
			const auto handle = dynamic_cast <TransformHandle*> (node .getValue ());
			
			if (handle)
				handle -> addAbsoluteMatrix (differenceMatrix);
		}
	}
}

void
TransformHandle::reshape ()
{
	try
	{
		const auto & handle = scene -> getNamedNode ("Handle");
		const auto   bbox   = transform -> X3DGroupingNode::getBBox ();

		handle -> setField <SFMatrix4f> ("cameraSpaceMatrix", getCameraSpaceMatrix (), true);
		handle -> setField <SFMatrix4f> ("modelViewMatrix",   ModelViewMatrix4f (),    true);
		handle -> setField <SFVec3f>    ("bboxSize",          bbox .size (),           true);
		handle -> setField <SFVec3f>    ("bboxCenter",        bbox .center (),         true);
	}
	catch (const X3DError & error)
	{ }
}

void
TransformHandle::traverse (const TraverseType type)
{
	transform -> traverse (type);

	getCurrentLayer () -> getLocalObjects () .emplace_back (new PolygonModeContainer (GL_FILL));
	
	//
	
	if (type == TraverseType::CAMERA)
	{
		parentMatrix = ModelViewMatrix4f ();
		matrix       = getMatrix ();
	}

	// Handle

	glPushMatrix ();

	glMultMatrixf (getMatrix () .data ());

	if (type == TraverseType::CAMERA) // Last chance to process events
		reshape ();

	for (const auto & rootNode : scene -> getRootNodes ())
	{
		if (rootNode)
			rootNode -> traverse (type);
	}

	glPopMatrix ();

	getCurrentLayer () -> getLocalObjects () .pop_back ();
}

void
TransformHandle::dispose ()
{
	transform -> removeParent (this);
	scene .dispose ();

	X3DHandleObject::dispose ();
	Transform::dispose ();
}

} // X3D
} // titania

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

#include "X3DNBodyCollidableNode.h"

#include "../RigidBodyPhysics/RigidBody.h"

namespace titania {
namespace X3D {

X3DNBodyCollidableNode::Fields::Fields () :
	    enabled (new SFBool (true)),
	translation (new SFVec3f ()),
	   rotation (new SFRotation ())
{ }

X3DNBodyCollidableNode::X3DNBodyCollidableNode () :
	              X3DChildNode (),
	X3DTransformMatrix3DObject (),
	          X3DBoundedObject (),
	                    fields (),
	             compoundShape (new btCompoundShape ()),
	                    offset ()
{
	addType (X3DConstants::X3DNBodyCollidableNode);

	translation () .setUnit (UnitCategory::LENGTH);
}

void
X3DNBodyCollidableNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DTransformMatrix3DObject::initialize ();
	X3DBoundedObject::initialize ();

	addInterest (&X3DNBodyCollidableNode::eventsProcessed, this);

	eventsProcessed ();
}

void
X3DNBodyCollidableNode::setLocalTransform (const Vector3f & t, const Rotation4d & r)
{
	translation () = t;
	rotation ()    = r;

	eventsProcessed ();
}

btTransform
X3DNBodyCollidableNode::getLocalTransform () const
{
	auto l = btTransform ();
	auto m = Matrix4f ();

	m .set (translation () .getValue (), rotation () .getValue ());
	m .translate (offset);

	l .setFromOpenGLMatrix (m [0] .data ());

	return l;
}

void
X3DNBodyCollidableNode::eventsProcessed ()
{
	setMatrix (translation () .getValue (), rotation () .getValue ());

	if (compoundShape -> getNumChildShapes ())
		compoundShape -> updateChildTransform (0, getLocalTransform (), true);
}

void
X3DNBodyCollidableNode::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DTransformMatrix3DObject::dispose ();
	X3DChildNode::dispose ();
}

X3DNBodyCollidableNode::~X3DNBodyCollidableNode ()
{ }

} // X3D
} // titania

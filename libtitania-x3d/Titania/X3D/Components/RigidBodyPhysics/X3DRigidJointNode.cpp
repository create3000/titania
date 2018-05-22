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

#include "X3DRigidJointNode.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/RigidBody.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"

namespace titania {
namespace X3D {

X3DRigidJointNode::Fields::Fields () :
	forceOutput (new MFString ({ "NONE" })),
	      body1 (new SFNode ()),
	      body2 (new SFNode ())
{ }

X3DRigidJointNode::X3DRigidJointNode () :
	             X3DNode (),
	              fields (),
	      collectionNode (),
	           bodyNode1 (),
	           bodyNode2 (),
	initalInverseMatrix1 (),
	initalInverseMatrix2 ()
{
	addType (X3DConstants::X3DRigidJointNode);

	addChildObjects (collectionNode, bodyNode1, bodyNode2);
}

void
X3DRigidJointNode::initialize ()
{
	X3DNode::initialize ();

	forceOutput () .addInterest (&X3DRigidJointNode::set_forceOutput, this);
	body1 ()       .addInterest (&X3DRigidJointNode::set_body1,       this);
	body2 ()       .addInterest (&X3DRigidJointNode::set_body2,       this);

	set_forceOutput ();
	set_body1 ();
	set_body2 ();
}

void
X3DRigidJointNode::setCollection (RigidBodyCollection* const value)
{
	removeJoint ();

	collectionNode = value;

	addJoint ();
}

void
X3DRigidJointNode::set_body1 ()
{
	removeJoint ();

	if (bodyNode1)
	{
		bodyNode1 -> removeInterest (&X3DRigidJointNode::update1, this);
		bodyNode1 -> getCollection () .removeInterest (&X3DRigidJointNode::set_joint, this);
	}

	bodyNode1 .set (x3d_cast <RigidBody*> (body1 ()));

	if (bodyNode1)
	{
		bodyNode1 -> addInterest (&X3DRigidJointNode::update1, this);
		bodyNode1 -> getCollection () .addInterest (&X3DRigidJointNode::set_joint, this);

		initialize1 ();
		addJoint ();
	}
}

void
X3DRigidJointNode::set_body2 ()
{
	removeJoint ();

	if (bodyNode2)
	{
		bodyNode2 -> removeInterest (&X3DRigidJointNode::update2, this);
		bodyNode2 -> getCollection () .removeInterest (&X3DRigidJointNode::set_joint, this);
	}

	bodyNode2 .set (x3d_cast <RigidBody*> (body2 ()));

	if (bodyNode2)
	{
		bodyNode2 -> addInterest (&X3DRigidJointNode::update2, this);
		bodyNode2 -> getCollection () .addInterest (&X3DRigidJointNode::set_joint, this);

		initialize2 ();
		addJoint ();
	}
}

void
X3DRigidJointNode::set_joint ()
{
	removeJoint ();
	addJoint ();
}

void
X3DRigidJointNode::initialize1 ()
{
	initalInverseMatrix1 = inverse (bodyNode1 -> getMatrix ());
}

void
X3DRigidJointNode::initialize2 ()
{
	initalInverseMatrix2 = inverse (bodyNode2 -> getMatrix ());
}

void
X3DRigidJointNode::dispose ()
{
	removeJoint ();

	X3DNode::dispose ();
}

X3DRigidJointNode::~X3DRigidJointNode ()
{ }

} // X3D
} // titania

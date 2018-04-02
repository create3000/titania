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

#include "BallJoint.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/RigidBody.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"

namespace titania {
namespace X3D {

const ComponentType BallJoint::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   BallJoint::typeName       = "BallJoint";
const std::string   BallJoint::containerField = "joints";

BallJoint::Fields::Fields () :
	     anchorPoint (new SFVec3f ()),
	body1AnchorPoint (new SFVec3f ()),
	body2AnchorPoint (new SFVec3f ())
{ }

BallJoint::BallJoint (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DRigidJointNode (),
	           fields (),
	          outputs (),
	            joint ()
{
	addType (X3DConstants::BallJoint);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "forceOutput",      forceOutput ());
	addField (inputOutput, "anchorPoint",      anchorPoint ());
	addField (outputOnly,  "body1AnchorPoint", body1AnchorPoint ());
	addField (outputOnly,  "body2AnchorPoint", body2AnchorPoint ());
	addField (inputOutput, "body1",            body1 ());
	addField (inputOutput, "body2",            body2 ());

	// Units

	anchorPoint () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
BallJoint::create (X3DExecutionContext* const executionContext) const
{
	return new BallJoint (executionContext);
}

void
BallJoint::initialize ()
{
	X3DRigidJointNode::initialize ();

	forceOutput () .addInterest (&BallJoint::set_forceOutput, this);
	anchorPoint () .addInterest (&BallJoint::set_anchorPoint, this);

	set_forceOutput ();
}

void
BallJoint::addJoint ()
{
	if (getBody1 () and getBody1 () -> getCollection () == getCollection () and getBody2 () and getBody2 () -> getCollection () == getCollection ())
	{
		joint .reset (new btPoint2PointConstraint (*getBody1 () -> getRigidBody (), *getBody2 () -> getRigidBody (), btVector3 (), btVector3 ()));

		set_anchorPoint ();
	}
	else
	{
		joint .reset ();
	}

	if (getCollection ())
	{
		if (joint)
			getCollection () -> getDynamicsWorld () -> addConstraint (joint .get (), true);
	}
}

void
BallJoint::removeJoint ()
{
	if (getCollection ())
	{
		if (joint)
			getCollection () -> getDynamicsWorld () -> removeConstraint (joint .get ());
	}
}

void
BallJoint::set_forceOutput ()
{
	const std::map <std::string, OutputType> outputTypes = {
		std::make_pair ("body1AnchorPoint", OutputType::body1AnchorPoint),
		std::make_pair ("body2AnchorPoint", OutputType::body2AnchorPoint),
	};

	std::fill (outputs .begin (), outputs .end (), false);

	for (const auto & value : basic::make_const_range (forceOutput ()))
	{
		try
		{
			if (value == "ALL")
			{
				std::fill (outputs .begin (), outputs .end (), true);
			}
			else
			{
				outputs [size_t (outputTypes .at (value))] = true;
			}
		}
		catch (const std::out_of_range & error)
		{ }
	}
}

void
BallJoint::set_anchorPoint ()
{
	if (joint)
	{
		auto anchorPoint1 = anchorPoint () .getValue ();
		auto anchorPoint2 = anchorPoint () .getValue ();

		anchorPoint1 = anchorPoint1 * getInverseMatrix1 ();
		anchorPoint2 = anchorPoint2 * getInverseMatrix2 ();

		joint -> setPivotA (btVector3 (anchorPoint1 .x (), anchorPoint1 .y (), anchorPoint1 .z ()));
		joint -> setPivotB (btVector3 (anchorPoint2 .x (), anchorPoint2 .y (), anchorPoint2 .z ()));

		if (outputs [size_t (OutputType::body1AnchorPoint)])
			body1AnchorPoint () = Vector3f (anchorPoint1 .x (), anchorPoint1 .y (), anchorPoint1 .z ());

		if (outputs [size_t (OutputType::body2AnchorPoint)])
			body2AnchorPoint () = Vector3f (anchorPoint2 .x (), anchorPoint2 .y (), anchorPoint2 .z ());
	}
}

void
BallJoint::update1 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize1 ();

	set_anchorPoint ();
}

void
BallJoint::update2 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize2 ();

	set_anchorPoint ();
}

BallJoint::~BallJoint ()
{ }

} // X3D
} // titania

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

const Component   BallJoint::component      = Component ("RigidBodyPhysics", 2);
const std::string BallJoint::typeName       = "BallJoint";
const std::string BallJoint::containerField = "joints";

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
	            joint (),
	localAnchorPoint1 (),
	localAnchorPoint2 ()
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

	anchorPoint () .addInterest (&BallJoint::set_anchorPoint, this);
}

void
BallJoint::addJoint ()
{
	if (not getCollection ())
		return;

	if (not getBody1 ())
		return;

	if (not getBody2 ())
		return;

   if (getBody1 () -> getCollection () not_eq getCollection ())
		return;

   if (getBody2 () -> getCollection () not_eq getCollection ())
		return;

	joint .reset (new btPoint2PointConstraint (*getBody1 () -> getRigidBody (), *getBody2 () -> getRigidBody (), btVector3 (), btVector3 ()));

	set_anchorPoint ();

	getCollection () -> getDynamicsWorld () -> addConstraint (joint .get (), true);
}

void
BallJoint::removeJoint ()
{
	if (not joint)
		return;

	if (getCollection ())
		getCollection () -> getDynamicsWorld () -> removeConstraint (joint .get ());

	joint .reset ();
}

void
BallJoint::set_forceOutput ()
{
	const std::map <std::string, OutputType> outputTypes = {
		std::pair ("body1AnchorPoint", OutputType::body1AnchorPoint),
		std::pair ("body2AnchorPoint", OutputType::body2AnchorPoint),
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
	if (not joint)
		return;

	// The anchor position and axis values of joints are always specified in world coordinate positions, regardless of
	// whether the two joining bodies have been offset or not.

	localAnchorPoint1 = anchorPoint () .getValue () * getInitialInverseMatrix1 ();
	localAnchorPoint2 = anchorPoint () .getValue () * getInitialInverseMatrix2 ();

	joint -> setPivotA (btVector3 (localAnchorPoint1 .x (), localAnchorPoint1 .y (), localAnchorPoint1 .z ()));
	joint -> setPivotB (btVector3 (localAnchorPoint2 .x (), localAnchorPoint2 .y (), localAnchorPoint2 .z ()));
}

void
BallJoint::update1 ()
{
	// Editing support.

	if (not getExecutionContext () -> isLive ())
	{
		initialize1 ();
		set_anchorPoint ();
	}

	// When the two bodies are initially placed in the scene, their initial positions define the resting coordinate
	// frames for the two bodies on that joint. Output values from those joints are then relative to this initial position.

	if (outputs [size_t (OutputType::body1AnchorPoint)])
		body1AnchorPoint () = localAnchorPoint1 * getBody1 () -> getMatrix () * getInitialInverseMatrix1 ();
}

void
BallJoint::update2 ()
{
	// Editing support.

	if (not getExecutionContext () -> isLive ())
	{
		initialize2 ();
		set_anchorPoint ();
	}

	// When the two bodies are initially placed in the scene, their initial positions define the resting coordinate
	// frames for the two bodies on that joint. Output values from those joints are then relative to this initial position.

	if (outputs [size_t (OutputType::body2AnchorPoint)])
		body2AnchorPoint () = localAnchorPoint2 * getBody2 () -> getMatrix () * getInitialInverseMatrix2 ();
}

BallJoint::~BallJoint ()
{ }

} // X3D
} // titania

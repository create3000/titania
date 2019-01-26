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

#include "CollisionSensor.h"

#include "../../Bits/Cast.h"
#include "../../Browser/RigidBodyPhysics/ContactContext.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/CollisionCollection.h"
#include "../RigidBodyPhysics/Contact.h"
#include "../RigidBodyPhysics/RigidBody.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"
#include "../RigidBodyPhysics/X3DNBodyCollidableNode.h"

namespace titania {
namespace X3D {

const Component   CollisionSensor::component      = Component ("RigidBodyPhysics", 2);
const std::string CollisionSensor::typeName       = "CollisionSensor";
const std::string CollisionSensor::containerField = "children";

CollisionSensor::Fields::Fields () :
	intersections (new MFNode ()),
	     contacts (new MFNode ()),
	     collider (new SFNode ())
{ }

CollisionSensor::CollisionSensor (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DSensorNode (),
	       fields (),
	 colliderNode (),
	 contactCache ()
{
	addType (X3DConstants::CollisionSensor);

	addField (inputOutput, "metadata",      metadata ());
	addField (inputOutput, "enabled",       enabled ());
	addField (outputOnly,  "isActive",      isActive ());
	addField (outputOnly,  "intersections", intersections ());
	addField (outputOnly,  "contacts",      contacts ());
	addField (inputOutput, "collider",      collider ());

	addField (VRML_V2_0, "collidables", "collider");

	addChildObjects (colliderNode, contactCache);
}

X3DBaseNode*
CollisionSensor::create (X3DExecutionContext* const executionContext) const
{
	return new CollisionSensor (executionContext);
}

void
CollisionSensor::initialize ()
{
	X3DSensorNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (&CollisionSensor::set_enabled, this);
	isLive () .addInterest (&CollisionSensor::set_enabled, this);

	enabled ()  .addInterest (&CollisionSensor::set_enabled,  this);
	collider () .addInterest (&CollisionSensor::set_collider, this);

	set_collider ();
}

const X3DPtr <Contact> &
CollisionSensor::getContact () const
{
	if (contactCache .size ())
	{
		std::sort (contactCache .begin (), contactCache .end (),
		[ ] (const X3DPtr <Contact> & lhs, const X3DPtr <Contact> & rhs)
		{
			return lhs -> getParents () .size () < rhs -> getParents () .size ();
		});

		const auto & front = contactCache .front ();

		// If a Contact is only referenced in the cache, parents size is 1.
		if (front -> getParents () .size () == 1)
			return front;
	}

	auto contactNode = MakePtr <Contact> (getExecutionContext ());

	contactNode -> setup ();
	contactCache .emplace_back (std::move (contactNode));

	return contactCache .back ();
}

void
CollisionSensor::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
	{
		getBrowser () -> sensorEvents ()    .removeInterest (&CollisionSensor::update,      this);
		getExecutionContext () -> isLive () .removeInterest (&CollisionSensor::set_enabled, this);
	}

	X3DSensorNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (&CollisionSensor::set_enabled, this);

		set_enabled ();
	}
}

void
CollisionSensor::set_enabled ()
{
	if (colliderNode and enabled () and isLive () and getExecutionContext () -> isLive ())
		getBrowser () -> sensorEvents () .addInterest (&CollisionSensor::update, this);
	else
		getBrowser () -> sensorEvents () .removeInterest (&CollisionSensor::update, this);
}

void
CollisionSensor::set_collider ()
{
	colliderNode = x3d_cast <CollisionCollection*> (collider ());

	set_enabled ();
}

void
CollisionSensor::update ()
{
	const auto & collidableNodes = colliderNode -> getCollidables ();
	auto         bodyIndex       = std::map <const btRigidBody*, std::pair <RigidBody*, X3DNBodyCollidableNode*>> ();

	for (const auto & collidableNode : collidableNodes)
	{
		const auto & bodyNode = collidableNode -> getBody ();

		if (bodyNode)
			bodyIndex .emplace (bodyNode -> getRigidBody () .get (), std::pair (bodyNode, collidableNode));
	}

	// Check collisions and create Contact nodes.

	X3DPtrArray <X3DNBodyCollidableNode> intersectionNodes;
	X3DPtrArray <Contact>                contactNodes;

	for (const auto & pair1 : bodyIndex)
	{
		const auto   bodyNode       = pair1 .second .first;
		const auto   collidableNode = pair1 .second .second;
		const auto & rigidBody      = bodyNode -> getRigidBody ();
		const auto & collectionNode = bodyNode -> getCollection ();

		if (not collectionNode)
			continue;

		const auto & dynamicsWorld = collectionNode -> getDynamicsWorld ();

		ContactContext context;
		ContactSensorCallback callback (rigidBody .get (), context);

		dynamicsWorld -> contactTest (rigidBody .get (), callback);

		if (not context .active)
			continue;

		const auto & contactNode = getContact ();

		contactNode -> position ()                 = context .position;
		contactNode -> contactNormal ()            = context .contactNormal;
		contactNode -> depth ()                    = context .depth;
		contactNode -> frictionDirection ()        = context .frictionDirection;
		contactNode -> appliedParameters ()        = colliderNode -> appliedParameters ();
		contactNode -> bounce ()                   = colliderNode -> bounce ();
		contactNode -> minBounceSpeed ()           = colliderNode -> minBounceSpeed ();
		contactNode -> frictionCoefficients ()     = colliderNode -> frictionCoefficients ();
		contactNode -> surfaceSpeed ()             = colliderNode -> surfaceSpeed ();
		contactNode -> slipCoefficients ()         = colliderNode -> slipFactors ();
		contactNode -> softnessConstantForceMix () = colliderNode -> softnessConstantForceMix ();
		contactNode -> softnessErrorCorrection ()  = colliderNode -> softnessErrorCorrection ();
		contactNode -> geometry1 ()                = collidableNode;
		contactNode -> body1 ()                    = bodyNode;

		try
		{
			const auto & pair2 = bodyIndex .at (context .rigidBody2);
	
			contactNode -> geometry2 () = pair2 .second;
			contactNode -> body2 ()     = pair2 .first;
		}
		catch (const std::out_of_range & error)
		{
			contactNode -> geometry2 () = nullptr;
			contactNode -> body2 ()     = nullptr;
		}

		intersectionNodes .emplace_back (collidableNode);
		contactNodes      .emplace_back (contactNode);
	}

	// Send events

	if (isActive () != bool (contactNodes .size ()))
		isActive () = contactNodes .size ();

	if (not intersectionNodes .empty ())
		intersections () = std::move (intersectionNodes);

	if (not contactNodes .empty ())
		contacts () = std::move (contactNodes);
}

} // X3D
} // titania

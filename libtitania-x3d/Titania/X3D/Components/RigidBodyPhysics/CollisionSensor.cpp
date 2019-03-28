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

const X3DPtr <Contact> &
CollisionSensor::getContact (const size_t index) const
{
	if (index < contactCache .size ())
		return contactCache [index];

	auto contactNode = MakePtr <Contact> (getExecutionContext ());

	contactNode -> setup ();
	contactCache .emplace_back (std::move (contactNode));

	return contactCache .back ();
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
	const auto & collidableNodes      = colliderNode -> getCollidables ();
	auto         collidableNodesIndex = std::map <const btCollisionObject*, X3DNBodyCollidableNode*> ();
	auto         collisionWorlds      = std::set <std::shared_ptr <btDiscreteDynamicsWorld>> ();

	for (const auto & collidableNode : collidableNodes)
	{
		const auto & bodyNode = collidableNode -> getBody ();

		if (bodyNode)
		{
			collidableNodesIndex .emplace (bodyNode -> getRigidBody () .get (), collidableNode);

			const auto collection = bodyNode -> getCollection ();

			if (collection)
				collisionWorlds .emplace (collection -> getDynamicsWorld ());
		}
	}

	// Check collisions and create Contact nodes.

	std::set <X3DNBodyCollidableNode*> intersectionNodes;
	X3DPtrArray <Contact>              contactNodes;

	for (const auto & collisionWorld : collisionWorlds)
	{
		collisionWorld -> performDiscreteCollisionDetection ();

		const auto dispatcher   = collisionWorld -> getDispatcher ();
		const auto numManifolds = dispatcher -> getNumManifolds ();

		for (int32_t i = 0; i < numManifolds; ++ i)
		{
			const auto contactManifold = dispatcher -> getManifoldByIndexInternal (i);
			const auto numContacts     = contactManifold -> getNumContacts ();

			for (int32_t j = 0; j < numContacts; ++ j)
			{
				const auto & pt = contactManifold -> getContactPoint (j);

				if (pt .getDistance () <= 0)
				{
					const auto collidableNode1 = collidableNodesIndex .find (contactManifold -> getBody0 ());
					const auto collidableNode2 = collidableNodesIndex .find (contactManifold -> getBody1 ());

					if (collidableNode1 == collidableNodesIndex .end () and collidableNode2 == collidableNodesIndex .end ())
						continue;

					const auto & contactNode     = getContact (contactNodes .size ());
					const auto   btPosition      = pt .getPositionWorldOnA ();
					const auto   btContactNormal = pt .m_normalWorldOnB;

					contactNode -> position ()                 = Vector3f (btPosition .x (), btPosition .y (), btPosition .z ());
					contactNode -> contactNormal ()            = Vector3f (btContactNormal .x (), btContactNormal .y (), btContactNormal .z ());
					contactNode -> depth ()                    = -pt .getDistance ();
					//contactNode -> frictionDirection ()        = context .frictionDirection;
					contactNode -> appliedParameters ()        = colliderNode -> appliedParameters ();
					contactNode -> bounce ()                   = colliderNode -> bounce ();
					contactNode -> minBounceSpeed ()           = colliderNode -> minBounceSpeed ();
					contactNode -> frictionCoefficients ()     = colliderNode -> frictionCoefficients ();
					contactNode -> surfaceSpeed ()             = colliderNode -> surfaceSpeed ();
					contactNode -> slipCoefficients ()         = colliderNode -> slipFactors ();
					contactNode -> softnessConstantForceMix () = colliderNode -> softnessConstantForceMix ();
					contactNode -> softnessErrorCorrection ()  = colliderNode -> softnessErrorCorrection ();

					if (collidableNode1 not_eq collidableNodesIndex .end ())
					{
						intersectionNodes .emplace (collidableNode1 -> second);

						contactNode -> geometry1 () = collidableNode1 -> second;
						contactNode -> body1 ()     = collidableNode1 -> second -> getBody ();
					}

					if (collidableNode2 not_eq collidableNodesIndex .end ())
					{
						intersectionNodes .emplace (collidableNode2 -> second);

						contactNode -> geometry2 () = collidableNode2 -> second;
						contactNode -> body2 ()     = collidableNode2 -> second -> getBody ();
					}

					contactNodes .emplace_back (contactNode);
				}
			}
		}
	}

	// Send events

	if (isActive () != bool (contactNodes .size ()))
		isActive () = contactNodes .size ();

	if (not intersectionNodes .empty ())
		intersections () .assign (intersectionNodes .begin (), intersectionNodes .end ());

	if (not contactNodes .empty ())
		contacts () = std::move (contactNodes);
}

} // X3D
} // titania

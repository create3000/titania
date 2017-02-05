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

#include "RigidBodyCollection.h"

#include "../../Browser/Core/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/CollisionCollection.h"
#include "../RigidBodyPhysics/CollisionSensor.h"
#include "../RigidBodyPhysics/Contact.h"
#include "../RigidBodyPhysics/RigidBody.h"

namespace titania {
namespace X3D {

const ComponentType RigidBodyCollection::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   RigidBodyCollection::typeName       = "RigidBodyCollection";
const std::string   RigidBodyCollection::containerField = "children";

RigidBodyCollection::Fields::Fields () :
	                enabled (new SFBool (true)),
	           set_contacts (new MFNode ()),
	                gravity (new SFVec3f (0, -9.8, 0)),
	         preferAccuracy (new SFBool ()),
	        errorCorrection (new SFFloat (0.8)),
	             iterations (new SFInt32 (10)),
	       constantForceMix (new SFFloat (0.0001)),
	     maxCorrectionSpeed (new SFFloat (-1)),
	contactSurfaceThickness (new SFFloat ()),
	            autoDisable (new SFBool ()),
	            disableTime (new SFFloat ()),
	     disableLinearSpeed (new SFFloat ()),
	    disableAngularSpeed (new SFFloat ()),
	               collider (new SFNode ()),
	                 joints (new MFNode ()),
	                 bodies (new MFNode ())
{ }

RigidBodyCollection::RigidBodyCollection (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       X3DChildNode (),
	             fields (),
	          bodyNodes (),
	collisionSensorNode (new CollisionSensor (getExecutionContext ()))
{
	addType (X3DConstants::RigidBodyCollection);

	addField (inputOutput,    "metadata",                metadata ());
	addField (inputOutput,    "enabled",                 enabled ());
	addField (inputOnly,      "set_contacts",            set_contacts ());
	addField (inputOutput,    "gravity",                 gravity ());

	addField (inputOutput,    "preferAccuracy",          preferAccuracy ());
	addField (inputOutput,    "errorCorrection",         errorCorrection ());
	addField (inputOutput,    "iterations",              iterations ());

	addField (inputOutput,    "constantForceMix",        constantForceMix ());
	addField (inputOutput,    "maxCorrectionSpeed",      maxCorrectionSpeed ());
	addField (inputOutput,    "contactSurfaceThickness", contactSurfaceThickness ());

	addField (inputOutput,    "autoDisable",             autoDisable ());
	addField (inputOutput,    "disableTime",             disableTime ());
	addField (inputOutput,    "disableLinearSpeed",      disableLinearSpeed ());
	addField (inputOutput,    "disableAngularSpeed",     disableAngularSpeed ());

	addField (initializeOnly, "collider",                collider ());
	addField (inputOutput,    "joints",                  joints ());
	addField (inputOutput,    "bodies",                  bodies ());

	addChildObjects (bodyNodes, collisionSensorNode);
}

X3DBaseNode*
RigidBodyCollection::create (X3DExecutionContext* const executionContext) const
{
	return new RigidBodyCollection (executionContext);
}

void
RigidBodyCollection::initialize ()
{
	X3DChildNode::initialize ();

	set_contacts () .addInterest (&RigidBodyCollection::set_contacts_, this);
	gravity ()      .addInterest (&RigidBodyCollection::set_gravity, this);
	collider ()     .addInterest (&RigidBodyCollection::set_collider, this);
	bodies ()       .addInterest (&RigidBodyCollection::set_bodies, this);

	collisionSensorNode -> contacts () .addInterest (&RigidBodyCollection::set_contacts_, this);
	collisionSensorNode -> setup ();

	set_collider ();
	set_bodies ();
}

void
RigidBodyCollection::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	collisionSensorNode -> setExecutionContext (executionContext);

	X3DChildNode::setExecutionContext (executionContext);
}

void
RigidBodyCollection::set_contacts_ ()
{
	__LOG__ << set_contacts () .size () << std::endl;

	std::multimap <RigidBody*, Contact*> contactIndex;

	for (const auto & node : set_contacts ())
	{
		const auto contactNode = x3d_cast <Contact*> (node);

		if (contactNode)
		{
			const auto bodyNode = x3d_cast <RigidBody*> (contactNode -> body1 ());

			if (bodyNode)
			{
				contactIndex .emplace (bodyNode, contactNode);
			}
		}
	}

	for (const auto & bodyNode : bodyNodes)
	{
		if (not bodyNode -> enabled ())
			continue;

		const auto range = contactIndex .equal_range (bodyNode);

		for (const auto & pair : range)
		{
			const auto & contactNode    = pair .second;
			const auto & contactNormal  = contactNode -> contactNormal () .getValue ();
			const auto & linearVelocity = bodyNode -> linearVelocity () .getValue ();

			__LOG__ << contactNode -> geometry1 () -> getName () << " : " << bodyNode .getValue () << std::endl;
			__LOG__ << contactNormal << std::endl;
			__LOG__ << linearVelocity << std::endl;
			__LOG__ << dot (normalize (linearVelocity), contactNormal) << std::endl;

			if (dot (normalize (linearVelocity), contactNormal) < 0)
				continue;

			__LOG__ << reflect (negate (linearVelocity), contactNormal) << std::endl;
			__LOG__ << reflect (linearVelocity, contactNormal) << std::endl;

			bodyNode -> linearVelocity () = reflect (linearVelocity, contactNormal);
		}
	}
}

void
RigidBodyCollection::set_gravity ()
{
	for (const auto & bodyNode : bodyNodes)
		bodyNode -> setGravity (gravity ());
}

void
RigidBodyCollection::set_collider ()
{
	collisionSensorNode -> collider () = x3d_cast <CollisionCollection*> (collider ());
	collisionSensorNode -> enabled ()  = collisionSensorNode -> collider ();
}

void
RigidBodyCollection::set_bodies ()
{
	std::vector <RigidBody*> value;

	for (const auto & node : bodies ())
	{
		const auto bodyNode = x3d_cast <RigidBody*> (node);
		
		if (bodyNode)
			value .emplace_back (bodyNode);
	}

	bodyNodes .set (value .begin (), value .end ());

	set_gravity ();
}

} // X3D
} // titania

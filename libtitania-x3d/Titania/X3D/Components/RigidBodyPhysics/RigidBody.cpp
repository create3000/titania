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

#include "RigidBody.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/X3DNBodyCollidableNode.h"

namespace titania {
namespace X3D {

const ComponentType RigidBody::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   RigidBody::typeName       = "RigidBody";
const std::string   RigidBody::containerField = "bodies";

RigidBody::Fields::Fields () :
	             enabled (new SFBool (true)),
	               fixed (new SFBool ()),
	            position (new SFVec3f ()),
	         orientation (new SFRotation ()),
	      linearVelocity (new SFVec3f ()),
	     angularVelocity (new SFVec3f ()),
	   useFiniteRotation (new SFBool ()),
	  finiteRotationAxis (new SFVec3f ()),
	            autoDamp (new SFBool ()),
	 linearDampingFactor (new SFFloat (0.001)),
	angularDampingFactor (new SFFloat (0.001)),
	                mass (new SFFloat (1)),
	        centerOfMass (new SFVec3f ()),
	    massDensityModel (new SFNode ()),
	    useGlobalGravity (new SFBool (true)),
	              forces (new MFVec3f ()),
	             torques (new MFVec3f ()),
	             inertia (new SFMatrix3f ()),
	         autoDisable (new SFBool ()),
	         disableTime (new SFFloat ()),
	  disableLinearSpeed (new SFFloat ()),
	 disableAngularSpeed (new SFFloat ()),
	            geometry (new MFNode ())
{ }

RigidBody::RigidBody (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	      X3DNode (),
	       fields (),
	      gravity (),
	        force (),
	geometryNodes ()
{
	addType (X3DConstants::RigidBody);

	addField (inputOutput, "metadata",             metadata ());
	addField (inputOutput, "enabled",              enabled ());
	addField (inputOutput, "fixed",                fixed ());

	addField (inputOutput, "position",             position ());
	addField (inputOutput, "orientation",          orientation ());

	addField (inputOutput, "linearVelocity",       linearVelocity ());
	addField (inputOutput, "angularVelocity",      angularVelocity ());

	addField (inputOutput, "useFiniteRotation",    useFiniteRotation ());
	addField (inputOutput, "finiteRotationAxis",   finiteRotationAxis ());

	addField (inputOutput, "autoDamp",             autoDamp ());
	addField (inputOutput, "linearDampingFactor",  linearDampingFactor ());
	addField (inputOutput, "angularDampingFactor", angularDampingFactor ());

	addField (inputOutput, "mass",                 mass ());
	addField (inputOutput, "inertia",              inertia ());
	addField (inputOutput, "centerOfMass",         centerOfMass ());
	addField (inputOutput, "massDensityModel",     massDensityModel ());

	addField (inputOutput, "useGlobalGravity",     useGlobalGravity ());
	addField (inputOutput, "forces",               forces ());
	addField (inputOutput, "torques",              torques ());

	addField (inputOutput, "autoDisable",          autoDisable ());
	addField (inputOutput, "disableTime",          disableTime ());
	addField (inputOutput, "disableLinearSpeed",   disableLinearSpeed ());
	addField (inputOutput, "disableAngularSpeed",  disableAngularSpeed ());

	addField (inputOutput, "geometry",             geometry ());

	addChildObjects (geometryNodes);
}

X3DBaseNode*
RigidBody::create (X3DExecutionContext* const executionContext) const
{
	return new RigidBody (executionContext);
}

void
RigidBody::initialize ()
{
	X3DNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (this, &RigidBody::set_fixed);
	isLive () .addInterest (this, &RigidBody::set_fixed);

	shutdown () .addInterest (this, &RigidBody::set_shutdown);

	fixed ()    .addInterest (this, &RigidBody::set_fixed);
	position () .addInterest (this, &RigidBody::set_position);
	forces ()   .addInterest (this, &RigidBody::set_forces);
	geometry () .addInterest (this, &RigidBody::set_geometry);

	set_fixed ();
	set_forces ();
	set_geometry ();
}

void
RigidBody::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (isInitialized ())
	{
		getBrowser () -> removeCollidableNodes (geometryNodes);

		getBrowser () -> sensors () .removeInterest (this, &RigidBody::update);
		getExecutionContext () -> isLive () .removeInterest (this, &RigidBody::set_fixed);
	}

	X3DNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (this, &RigidBody::set_fixed);
	
		set_fixed ();
		set_geometry ();
	}
}

void
RigidBody::set_fixed ()
{
	if (getExecutionContext () -> isLive () and isLive () and not fixed ())
		getBrowser () -> sensors () .addInterest (this, &RigidBody::update);
	else
		getBrowser () -> sensors () .removeInterest (this, &RigidBody::update);
}

void
RigidBody::set_position ()
{
	for (const auto & geometryNode : geometryNodes)
		geometryNode -> translation () = position ();
}

void
RigidBody::set_forces ()
{
	force = std::accumulate (forces () .begin (),
	                         forces () .end (),
	                         Vector3f (),
	                         [ ] (const Vector3f & a, const Vector3f & b) { return a + b; });
}

void
RigidBody::set_geometry ()
{
	// Remove collidable n-body nodes from global CollisionCollection.

	getBrowser () -> removeCollidableNodes (geometryNodes);

	// Sort out X3DNBodyCollidableNode nodes.

	std::vector <X3DNBodyCollidableNode*> value;

	for (const auto & node : geometry ())
	{
		const auto geometryNode = x3d_cast <X3DNBodyCollidableNode*> (node);
		
		if (geometryNode)
			value .emplace_back (geometryNode);
	}

	geometryNodes .set (value .begin (), value .end ());

	// Set body.

	for (const auto & geometryNode : geometryNodes)
		geometryNode -> setBody (X3DPtr <RigidBody> (this));

	// Register collidable n-body nodes for global CollisionCollection.

	getBrowser () -> addCollidableNodes (geometryNodes);

	// Update geometry nodes translaton and rotation.

	set_position ();
}

void
RigidBody::update ()
{
	const float currentFrameRate   = getBrowser () -> getCurrentFrameRate ();
	const auto  linearDamping      = linearVelocity () * linearDampingFactor () .getValue ();
	auto        linearAcceleration = useGlobalGravity () ? gravity : Vector3f ();

	if (mass ())
		linearAcceleration += force / mass () .getValue ();
	
	linearVelocity () = linearVelocity () + linearAcceleration / currentFrameRate;
	
	if (autoDamp ())
		linearVelocity () -= linearDamping;

	position () = position () + linearVelocity () / currentFrameRate;
}

void
RigidBody::set_shutdown ()
{
	getBrowser () -> removeCollidableNodes (geometryNodes);
}

} // X3D
} // titania

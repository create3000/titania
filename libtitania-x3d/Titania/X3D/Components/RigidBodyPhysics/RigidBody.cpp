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

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"
#include "../RigidBodyPhysics/X3DNBodyCollidableNode.h"

namespace titania {
namespace X3D {

const Component   RigidBody::component      = Component ("RigidBodyPhysics", 2);
const std::string RigidBody::typeName       = "RigidBody";
const std::string RigidBody::containerField = "bodies";

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
	        collectionNode (),
	         geometryNodes (),
	    otherGeometryNodes (),
	         compoundShape (new btCompoundShape ()),
	           motionState (new btDefaultMotionState ()),
	             rigidBody (new btRigidBody (btRigidBody::btRigidBodyConstructionInfo (0, motionState .get (), compoundShape .get ()))),
	             transform (),
	                matrix (),
	                 force (),
	                torque (),
	       initialPosition (),
	    initialOrientation (),
	 initialLinearVelocity (),
	initialAngularVelocity ()
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

	addChildObjects (collectionNode, geometryNodes, otherGeometryNodes, transform);

	// Units

	position ()            .setUnit (UnitCategory::LENGTH);
	linearVelocity ()      .setUnit (UnitCategory::SPEED);
	angularVelocity ()     .setUnit (UnitCategory::ANGULAR_RATE);
	mass ()                .setUnit (UnitCategory::MASS);
	forces ()              .setUnit (UnitCategory::FORCE);
	torques ()             .setUnit (UnitCategory::FORCE);
	disableLinearSpeed ()  .setUnit (UnitCategory::SPEED);
	disableAngularSpeed () .setUnit (UnitCategory::ANGULAR_RATE);

	// Manage gravity by ourself, otherwise useGlobalGravity cannot be toggled dynamically.
	rigidBody -> setFlags (rigidBody -> getFlags () & ~BT_DISABLE_WORLD_GRAVITY);

	// gContactAddedCallback, search for conveyor belt
	//rigidBody -> setCollisionFlags (rigidBody -> getCollisionFlags () | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
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

	getExecutionContext () -> isLive () .addInterest (&RigidBody::set_live, this);

	linearVelocity ()       .addInterest (&RigidBody::set_linearVelocity,     this);
	angularVelocity ()      .addInterest (&RigidBody::set_angularVelocity,    this);
	useFiniteRotation ()    .addInterest (&RigidBody::set_finiteRotationAxis, this);
	finiteRotationAxis ()   .addInterest (&RigidBody::set_finiteRotationAxis, this);
	autoDamp ()             .addInterest (&RigidBody::set_damping,            this);
	linearDampingFactor ()  .addInterest (&RigidBody::set_damping,            this);
	angularDampingFactor () .addInterest (&RigidBody::set_damping,            this);
	forces ()               .addInterest (&RigidBody::set_forces,             this);
	torques ()              .addInterest (&RigidBody::set_torques,            this);
	autoDisable ()          .addInterest (&RigidBody::set_disable,            this);
	disableTime ()          .addInterest (&RigidBody::set_disable,            this);
	disableLinearSpeed ()   .addInterest (&RigidBody::set_disable,            this);
	disableAngularSpeed ()  .addInterest (&RigidBody::set_disable,            this);
	geometry ()             .addInterest (&RigidBody::set_geometry,           this);

	fixed ()   .addInterest (&RigidBody::set_massProps, this);
	mass ()    .addInterest (&RigidBody::set_massProps, this);
	inertia () .addInterest (&RigidBody::set_massProps, this);

	transform .addInterest (&RigidBody::set_transform, this);

	set_forces ();
	set_torques ();
	set_geometry ();

	// Editing support

	initialPosition        = position ();
	initialOrientation     = orientation ();
	initialLinearVelocity  = linearVelocity ();
	initialAngularVelocity = angularVelocity ();
}

void
RigidBody::set_live ()
{
	// Editing support

	if (not getExecutionContext () -> isLive ())
	{
		position ()        = initialPosition;
		orientation ()     = initialOrientation;
		linearVelocity ()  = initialLinearVelocity;
		angularVelocity () = initialAngularVelocity;
	}
}

void
RigidBody::set_position ()
{
	for (const auto & geometryNode : geometryNodes)
		geometryNode -> translation () = position ();
}

void
RigidBody::set_orientation ()
{
	for (const auto & geometryNode : geometryNodes)
		geometryNode -> rotation () = orientation ();
}

void
RigidBody::set_transform ()
{
	auto t = btTransform ();

	matrix .set (position () .getValue (), orientation () .getValue ());
	t .setFromOpenGLMatrix (matrix [0] .data ());

	auto it = btTransform ();
	auto im = inverse (matrix);

	it .setFromOpenGLMatrix (im [0] .data ());

	for (int32_t i = 0, size = compoundShape -> getNumChildShapes (); i < size; ++ i)
		compoundShape -> updateChildTransform (i, it, false);

	compoundShape -> recalculateLocalAabb ();
	motionState -> setWorldTransform (t);

	rigidBody -> setMotionState (motionState .get ());

	// Editing support

	if (not getExecutionContext () -> isLive ())
	{
		initialPosition    = position ();
		initialOrientation = orientation ();
	}
}

void
RigidBody::set_linearVelocity ()
{
	rigidBody -> setLinearVelocity (btVector3 (linearVelocity () .getX (), linearVelocity () .getY (), linearVelocity () .getZ ()));
	rigidBody -> activate ();

	// Editing support

	if (not getExecutionContext () -> isLive ())
		initialLinearVelocity = linearVelocity ();
}

void
RigidBody::set_angularVelocity ()
{
	rigidBody -> setAngularVelocity (btVector3 (angularVelocity () .getX (), angularVelocity () .getY (), angularVelocity () .getZ ()));
	rigidBody -> activate ();

	// Editing support

	if (not getExecutionContext () -> isLive ())
		initialAngularVelocity = angularVelocity ();
}

void
RigidBody::set_finiteRotationAxis ()
{
	if (useFiniteRotation ())
		rigidBody -> setAngularFactor (btVector3 (finiteRotationAxis () .getX (), finiteRotationAxis () .getY (), finiteRotationAxis () .getZ ()));
	else
		rigidBody -> setAngularFactor (btVector3 (1, 1, 1));
}

void
RigidBody::set_damping ()
{
	if (autoDamp ())
		rigidBody -> setDamping (linearDampingFactor (), angularDampingFactor ());
	else
		rigidBody -> setDamping (0, 0);

	rigidBody -> activate ();
}

void
RigidBody::set_centerOfMass ()
{
	rigidBody -> setCenterOfMassTransform (btTransform (btQuaternion (0, 0, 0, 1), btVector3 (centerOfMass () .getX (), centerOfMass () .getY (), centerOfMass () .getZ ())));
}

void
RigidBody::set_massProps ()
{
	auto localInertia = btVector3 (inertia () [0] + inertia () [1] + inertia () [2],
	                               inertia () [3] + inertia () [4] + inertia () [5],
	                               inertia () [6] + inertia () [7] + inertia () [8]);

	compoundShape -> calculateLocalInertia (fixed () ? 0.0f : mass (), localInertia);

	rigidBody -> setMassProps (fixed () ? 0.0f : mass (), localInertia);
}

void
RigidBody::set_forces ()
{
	force = std::accumulate (forces () .cbegin (), forces () .cend (), Vector3f (), std::plus <Vector3f> ());

	rigidBody -> activate ();
}

void
RigidBody::set_torques ()
{
	torque = std::accumulate (torques () .cbegin (), torques () .cend (), Vector3f (), std::plus <Vector3f> ());

	rigidBody -> activate ();
}

void
RigidBody::set_disable ()
{
	if (autoDisable ())
	{
		rigidBody -> setSleepingThresholds (disableLinearSpeed (), disableAngularSpeed ());
	}
	else
	{
		rigidBody -> setSleepingThresholds (0, 0);
	}
}

void
RigidBody::set_geometry ()
{
	for (const auto & geometryNode : geometryNodes)
	{
		geometryNode -> setBody (nullptr);

		geometryNode -> translation () .removeInterest (transform);
		geometryNode -> rotation ()    .removeInterest (transform);

		geometryNode -> translation () .removeInterest (position ());
		geometryNode -> rotation ()    .removeInterest (orientation ());

		position ()    .removeInterest (geometryNode -> translation ());
		orientation () .removeInterest (geometryNode -> rotation ());
	}

	for (const auto & geometryNode : otherGeometryNodes)
		geometryNode -> getBody () .removeInterest (&RigidBody::set_geometry, this);

	// Sort out X3DNBodyCollidableNode nodes.

	std::vector <X3DNBodyCollidableNode*> value;

	for (const auto & node : geometry ())
	{
		const auto geometryNode = x3d_cast <X3DNBodyCollidableNode*> (node);
		
		if (not geometryNode)
			continue;

		if (geometryNode -> getBody ())
		{
			geometryNode -> getBody () .addInterest (&RigidBody::set_geometry, this);
			otherGeometryNodes .emplace_back (geometryNode);
			continue;
		}

		geometryNode -> setBody (this);

		value .emplace_back (geometryNode);
	}

	geometryNodes .set (value .cbegin (), value .cend ());

	for (const auto & geometryNode : geometryNodes)
	{
		geometryNode -> translation () .addInterest (transform);
		geometryNode -> rotation ()    .addInterest (transform);

		geometryNode -> translation () .addInterest (position ());
		geometryNode -> rotation ()    .addInterest (orientation ());

		position ()    .addInterest (geometryNode -> translation ());
		orientation () .addInterest (geometryNode -> rotation ());
	}

	set_compoundShape ();
}

void
RigidBody::set_compoundShape ()
{
	for (int32_t i = compoundShape -> getNumChildShapes () - 1; i >= 0; -- i)
		compoundShape -> removeChildShapeByIndex (i);

	for (const auto & geometryNode : geometryNodes)
		compoundShape -> addChildShape (btTransform (), geometryNode -> getCompoundShape () .get ());

	set_position ();
	set_orientation ();
	set_transform ();
	set_linearVelocity ();
	set_angularVelocity ();
	set_finiteRotationAxis ();
	set_damping ();
	set_centerOfMass ();
	set_massProps ();
	set_disable ();
}

void
RigidBody::applyForces (const Vector3f & gravity)
{
	if (fixed ())
		return;

	rigidBody -> setGravity (useGlobalGravity () ? btVector3 (gravity .x (), gravity .y (), gravity .z ()) : btVector3 (0, 0, 0));
	rigidBody -> applyForce (btVector3 (force .x (), force .y (), force .z ()), btVector3 (0, 0, 0));
	rigidBody -> applyTorque (btVector3 (torque .x (), torque .y (), torque .z ()));
}

void
RigidBody::update ()
{
	btTransform transform;

	motionState -> getWorldTransform (transform);

	const auto btOrigin          = transform .getOrigin ();
	const auto btQuaternion      = transform .getRotation ();
	const auto btLinearVeloctity = rigidBody -> getLinearVelocity ();
	const auto btAngularVelocity = rigidBody -> getAngularVelocity ();

	position ()        = Vector3f (btOrigin .x (), btOrigin .y (), btOrigin .z ());
	orientation ()     = Rotation4d (Quaternion4d (btQuaternion .x (), btQuaternion .y (), btQuaternion .z (), btQuaternion .w ()));
	linearVelocity ()  = Vector3f (btLinearVeloctity .x (), btLinearVeloctity .y (), btLinearVeloctity .z ());
	angularVelocity () = Vector3f (btAngularVelocity .x (), btAngularVelocity .y (), btAngularVelocity .z ());
}

RigidBody::~RigidBody ()
{ }

} // X3D
} // titania

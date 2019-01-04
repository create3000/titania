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

#include "SliderJoint.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/RigidBody.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"

namespace titania {
namespace X3D {

const std::string SliderJoint::componentName  = "RigidBodyPhysics";
const std::string SliderJoint::typeName       = "SliderJoint";
const std::string SliderJoint::containerField = "joints";

SliderJoint::Fields::Fields () :
	               axis (new SFVec3f (0, 1, 0)),
	      minSeparation (new SFFloat ()),
	      maxSeparation (new SFFloat (1)),
	        sliderForce (new SFFloat ()),
	         stopBounce (new SFFloat ()),
	stopErrorCorrection (new SFFloat (1)),
 	         separation (new SFFloat ()),
	     separationRate (new SFFloat ())
{ }

SliderJoint::SliderJoint (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DRigidJointNode (),
	           fields (),
	          outputs (),
	            joint ()
{
	addType (X3DConstants::SliderJoint);

	addField (inputOutput, "metadata",            metadata ());
	addField (inputOutput, "forceOutput",         forceOutput ());
	addField (inputOutput, "axis",                axis ());
	addField (inputOutput, "minSeparation",       minSeparation ());
	addField (inputOutput, "maxSeparation",       maxSeparation ());
	addField (inputOutput, "sliderForce",         sliderForce ());
	addField (inputOutput, "stopBounce",          stopBounce ());
	addField (inputOutput, "stopErrorCorrection", stopErrorCorrection ());
	addField (outputOnly,  "separation",          separation ());
	addField (outputOnly,  "separationRate",      separationRate ());
	addField (inputOutput, "body1",               body1 ());
	addField (inputOutput, "body2",               body2 ());

	// Units

	minSeparation ()  .setUnit (UnitCategory::LENGTH);
	maxSeparation ()  .setUnit (UnitCategory::LENGTH);
	sliderForce ()    .setUnit (UnitCategory::FORCE);
	separation ()     .setUnit (UnitCategory::FORCE);
	separationRate () .setUnit (UnitCategory::SPEED);
}

X3DBaseNode*
SliderJoint::create (X3DExecutionContext* const executionContext) const
{
	return new SliderJoint (executionContext);
}

void
SliderJoint::initialize ()
{
	X3DRigidJointNode::initialize ();

	axis ()          .addInterest (&SliderJoint::set_joint,       this);
	minSeparation () .addInterest (&SliderJoint::set_separation,  this);
	maxSeparation () .addInterest (&SliderJoint::set_separation,  this);
}

void
SliderJoint::addJoint ()
{
	/*
	 * The X-Axis is the translation axis. Just produce a matrix ( coordinate system, relative to the object coordinate
	 * system ) in your app where the X-Axis points along the translation direction and where the position is located where
	 * the origin of the translation is ( as measured later on using the limits ). FrameA becomes this coordinate system you
	 * defined. I then create a second matrix ( frameB ) which represents the same matrix as before but relative to the
	 * parent object. Set the boolean parameter to "true" to use frameA as the master frame. Doing so things should work out
	 * correctly, at least it worked for me so far.
	*/

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

	const auto axisRotation = Rotation4d (Vector3f (1, 0, 0), axis () .getValue ());

	Matrix4f matrixA;
	Matrix4f matrixB;

	matrixA .set (getBody1 () -> position () .getValue (), getBody1 () -> orientation () * axisRotation);
	matrixB .set (getBody1 () -> position () .getValue (), getBody1 () -> orientation () * axisRotation);

	//matrixB = matrixA;

	btTransform frameInA;
	btTransform frameInB;

	frameInA .setFromOpenGLMatrix (matrixA [0] .data ());
	frameInB .setFromOpenGLMatrix (matrixB [0] .data ());

	joint .reset (new btSliderConstraint (*getBody1 () -> getRigidBody (),
	                                      *getBody2 () -> getRigidBody (),
	                                      frameInA,
	                                      frameInB,
	                                      true));

	joint -> setLowerAngLimit (0);
	joint -> setUpperAngLimit (0);

	set_separation ();

	getCollection () -> getDynamicsWorld () -> addConstraint (joint .get (), true);
}

void
SliderJoint::removeJoint ()
{
	if (not joint)
		return;

	if (getCollection ())
		getCollection () -> getDynamicsWorld () -> removeConstraint (joint .get ());

	joint .reset ();
}

void
SliderJoint::set_forceOutput ()
{
	const std::map <std::string, OutputType> outputTypes = {
		std::pair ("separation",     OutputType::separation),
		std::pair ("separationRate", OutputType::separationRate),
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
SliderJoint::set_separation ()
{
	if (not joint)
		return;

	joint -> setLowerLinLimit (minSeparation ());
	joint -> setUpperLinLimit (maxSeparation ());
}

void
SliderJoint::update1 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize1 ();
	set_joint ();
}

void
SliderJoint::update2 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize2 ();
	set_joint ();
}

} // X3D
} // titania

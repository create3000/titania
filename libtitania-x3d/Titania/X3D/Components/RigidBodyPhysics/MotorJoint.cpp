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

#include "MotorJoint.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../RigidBodyPhysics/RigidBody.h"
#include "../RigidBodyPhysics/RigidBodyCollection.h"

namespace titania {
namespace X3D {

const ComponentType MotorJoint::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   MotorJoint::typeName       = "MotorJoint";
const std::string   MotorJoint::containerField = "joints";

MotorJoint::Fields::Fields () :
	            autoCalc (new SFBool ()),
	         enabledAxes (new SFInt32 (1)),
	          motor1Axis (new SFVec3f ()),
	          motor2Axis (new SFVec3f ()),
	          motor3Axis (new SFVec3f ()),
	          axis1Angle (new SFFloat ()),
	          axis2Angle (new SFFloat ()),
	          axis3Angle (new SFFloat ()),
	         axis1Torque (new SFFloat ()),
	         axis2Torque (new SFFloat ()),
	         axis3Torque (new SFFloat ()),
	         stop1Bounce (new SFFloat ()),
	         stop2Bounce (new SFFloat ()),
	         stop3Bounce (new SFFloat ()),
	stop1ErrorCorrection (new SFFloat (0.8)),
	stop2ErrorCorrection (new SFFloat (0.8)),
	stop3ErrorCorrection (new SFFloat (0.8)),
	         motor1Angle (new SFFloat ()),
	         motor2Angle (new SFFloat ()),
	         motor3Angle (new SFFloat ()),
	     motor1AngleRate (new SFFloat ()),
	     motor2AngleRate (new SFFloat ()),
	     motor3AngleRate (new SFFloat ())
{ }

MotorJoint::MotorJoint (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DRigidJointNode (),
	           fields (),
	          outputs (),
	            joint ()
{
	addType (X3DConstants::MotorJoint);

	addField (inputOutput,    "metadata",             metadata ());
	addField (inputOutput,    "forceOutput",          forceOutput ());
	addField (initializeOnly, "autoCalc",             autoCalc ());
	addField (inputOutput,    "enabledAxes",          enabledAxes ());
	addField (inputOutput,    "motor1Axis",           motor1Axis ());
	addField (inputOutput,    "motor2Axis",           motor2Axis ());
	addField (inputOutput,    "motor3Axis",           motor3Axis ());
	addField (inputOutput,    "axis1Angle",           axis1Angle ());
	addField (inputOutput,    "axis2Angle",           axis2Angle ());
	addField (inputOutput,    "axis3Angle",           axis3Angle ());
	addField (inputOutput,    "axis1Torque",          axis1Torque ());
	addField (inputOutput,    "axis2Torque",          axis2Torque ());
	addField (inputOutput,    "axis3Torque",          axis3Torque ());
	addField (inputOutput,    "stop1Bounce",          stop1Bounce ());
	addField (inputOutput,    "stop2Bounce",          stop2Bounce ());
	addField (inputOutput,    "stop3Bounce",          stop3Bounce ());
	addField (inputOutput,    "stop1ErrorCorrection", stop1ErrorCorrection ());
	addField (inputOutput,    "stop2ErrorCorrection", stop2ErrorCorrection ());
	addField (inputOutput,    "stop3ErrorCorrection", stop3ErrorCorrection ());
	addField (outputOnly,     "motor1Angle",          motor1Angle ());
	addField (outputOnly,     "motor2Angle",          motor2Angle ());
	addField (outputOnly,     "motor3Angle",          motor3Angle ());
	addField (outputOnly,     "motor1AngleRate",      motor1AngleRate ());
	addField (outputOnly,     "motor2AngleRate",      motor2AngleRate ());
	addField (outputOnly,     "motor3AngleRate",      motor3AngleRate ());
	addField (inputOutput,    "body1",                body1 ());
	addField (inputOutput,    "body2",                body2 ());

	// Units

	axis1Angle ()      .setUnit (UnitCategory::ANGLE);
	axis2Angle ()      .setUnit (UnitCategory::ANGLE);
	axis3Angle ()      .setUnit (UnitCategory::ANGLE);
	motor1Angle ()     .setUnit (UnitCategory::ANGLE);
	motor2Angle ()     .setUnit (UnitCategory::ANGLE);
	motor3Angle ()     .setUnit (UnitCategory::ANGLE);
	motor1AngleRate () .setUnit (UnitCategory::ANGULAR_RATE);
	motor2AngleRate () .setUnit (UnitCategory::ANGULAR_RATE);
	motor3AngleRate () .setUnit (UnitCategory::ANGULAR_RATE);
}

X3DBaseNode*
MotorJoint::create (X3DExecutionContext* const executionContext) const
{
	return new MotorJoint (executionContext);
}

void
MotorJoint::initialize ()
{
	X3DRigidJointNode::initialize ();

	autoCalc ()    .addInterest (&MotorJoint::set_axes,        this);
	enabledAxes () .addInterest (&MotorJoint::set_axes,        this);
	motor1Axis ()  .addInterest (&MotorJoint::set_axes,        this);
	motor2Axis ()  .addInterest (&MotorJoint::set_axes,        this);
	motor3Axis ()  .addInterest (&MotorJoint::set_axes,        this);
	axis1Torque () .addInterest (&MotorJoint::set_axes,        this);
	axis2Torque () .addInterest (&MotorJoint::set_axes,        this);
	axis3Torque () .addInterest (&MotorJoint::set_axes,        this);
	axis1Angle ()  .addInterest (&MotorJoint::set_axis1Angle,  this);
	axis2Angle ()  .addInterest (&MotorJoint::set_axis2Angle,  this);
	axis3Angle ()  .addInterest (&MotorJoint::set_axis3Angle,  this);
}

void
MotorJoint::set_forceOutput ()
{
	const std::map <std::string, OutputType> outputTypes = {
		std::pair ("motor1Angle",     OutputType::motor1Angle),
		std::pair ("motor2Angle",     OutputType::motor2Angle),
		std::pair ("motor3Angle",     OutputType::motor3Angle),
		std::pair ("motor1AngleRate", OutputType::motor1AngleRate),
		std::pair ("motor2AngleRate", OutputType::motor2AngleRate),
		std::pair ("motor3AngleRate", OutputType::motor3AngleRate),
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
MotorJoint::set_axes ()
{
	if (not joint)
		return;

	const auto rotationalLimitMotor0 = joint -> getRotationalLimitMotor (0);
	const auto rotationalLimitMotor1 = joint -> getRotationalLimitMotor (1);
	const auto rotationalLimitMotor2 = joint -> getRotationalLimitMotor (2);

	if (autoCalc () and motor3Axis () .getX ())
	{
		rotationalLimitMotor0 -> m_enableMotor    = true;
		rotationalLimitMotor0 -> m_targetVelocity = axis1Torque ();
		rotationalLimitMotor0 -> m_maxMotorForce  = 100;

		joint -> setLimit (0, -math::pi <float>, math::pi <float>);
	}
	else
	{
		rotationalLimitMotor0-> m_enableMotor    = false;
		rotationalLimitMotor0-> m_targetVelocity = 0;
		rotationalLimitMotor0-> m_maxMotorForce  = 0;

		joint -> setLimit (0, 0, 0);
	}

	if (autoCalc () and motor3Axis () .getY ())
	{
		rotationalLimitMotor1 -> m_enableMotor    = true;
		rotationalLimitMotor1 -> m_targetVelocity = axis2Torque ();
		rotationalLimitMotor1 -> m_maxMotorForce  = 100;

		joint -> setLimit (1, -math::pi <float>, math::pi <float>);
	}
	else
	{
		rotationalLimitMotor1 -> m_enableMotor    = false;
		rotationalLimitMotor1 -> m_targetVelocity = 0;
		rotationalLimitMotor1 -> m_maxMotorForce  = 0;

		joint -> setLimit (1, 0, 0);
	}

	if (autoCalc () and motor3Axis () .getZ ())
	{
		rotationalLimitMotor2 -> m_enableMotor    = true;
		rotationalLimitMotor2 -> m_targetVelocity = axis3Torque ();
		rotationalLimitMotor2 -> m_maxMotorForce  = 100;

		joint -> setLimit (2, -math::pi <float>, math::pi <float>);
	}
	else
	{
		rotationalLimitMotor2 -> m_enableMotor    = false;
		rotationalLimitMotor2 -> m_targetVelocity = 0;
		rotationalLimitMotor2 -> m_maxMotorForce  = 0;

		joint -> setLimit (2, 0, 0);
	}
}

void
MotorJoint::set_axis1Angle ()
{
}

void
MotorJoint::set_axis2Angle ()
{
}

void
MotorJoint::set_axis3Angle ()
{
}

void
MotorJoint::addJoint ()
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

	Matrix4f matrixA;
	Matrix4f matrixB;

	matrixA .set (getBody1 () -> position () .getValue (), getBody1 () -> orientation () .getValue ());
	matrixB .set (getBody2 () -> position () .getValue (), getBody2 () -> orientation () .getValue ());

	matrixB .inverse ();
	matrixB .mult_right (matrixA);

	btTransform frameInA;
	btTransform frameInB;

	frameInA .setIdentity ();
	frameInB .setFromOpenGLMatrix (matrixB [0] .data ());

   joint .reset (new btGeneric6DofConstraint (*getBody1 () -> getRigidBody (),
	                                           *getBody2 () -> getRigidBody (),
	                                            frameInA,
	                                            frameInB,
	                                            true));

	joint -> enableFeedback (true);

	set_axes ();

	getCollection () -> getDynamicsWorld () -> addConstraint (joint .get (), true);
}

void
MotorJoint::removeJoint ()
{
	if (not joint)
		return;

	if (getCollection ())
		getCollection () -> getDynamicsWorld () -> removeConstraint (joint .get ());

	joint .reset ();
}

void
MotorJoint::update1 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize1 ();
	set_joint ();
}

void
MotorJoint::update2 ()
{
	// Editing support.

	if (getExecutionContext () -> isLive ())
		return;

	initialize2 ();
	set_joint ();
}

} // X3D
} // titania

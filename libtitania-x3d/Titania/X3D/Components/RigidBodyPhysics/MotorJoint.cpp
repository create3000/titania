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

namespace titania {
namespace X3D {

const ComponentType MotorJoint::component      = ComponentType::RIGID_BODY_PHYSICS;
const std::string   MotorJoint::typeName       = "MotorJoint";
const std::string   MotorJoint::containerField = "joints";

MotorJoint::Fields::Fields () :
	axis1Angle (new SFFloat ()),
	axis1Torque (new SFFloat ()),
	axis2Angle (new SFFloat ()),
	axis2Torque (new SFFloat ()),
	axis3Angle (new SFFloat ()),
	axis3Torque (new SFFloat ()),
	enabledAxes (new SFInt32 (1)),
	motor1Axis (new SFVec3f ()),
	motor2Axis (new SFVec3f ()),
	motor3Axis (new SFVec3f ()),
	stop1Bounce (new SFFloat ()),
	stop1ErrorCorrection (new SFFloat (0.8)),
	stop2Bounce (new SFFloat ()),
	stop2ErrorCorrection (new SFFloat (0.8)),
	stop3Bounce (new SFFloat ()),
	stop3ErrorCorrection (new SFFloat (0.8)),
	motor1Angle (new SFFloat ()),
	motor1AngleRate (new SFFloat ()),
	motor2Angle (new SFFloat ()),
	motor2AngleRate (new SFFloat ()),
	motor3Angle (new SFFloat ()),
	motor3AngleRate (new SFFloat ()),
	autoCalc (new SFBool ())
{ }

MotorJoint::MotorJoint (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DRigidJointNode (),
	           fields ()
{
	addType (X3DConstants::MotorJoint);

	addField (inputOutput,    "metadata",             metadata ());
	addField (inputOutput,    "body1",                body1 ());
	addField (inputOutput,    "body2",                body2 ());
	addField (inputOutput,    "forceOutput",          forceOutput ());
	addField (inputOutput,    "axis1Angle",           axis1Angle ());
	addField (inputOutput,    "axis1Torque",          axis1Torque ());
	addField (inputOutput,    "axis2Angle",           axis2Angle ());
	addField (inputOutput,    "axis2Torque",          axis2Torque ());
	addField (inputOutput,    "axis3Angle",           axis3Angle ());
	addField (inputOutput,    "axis3Torque",          axis3Torque ());
	addField (inputOutput,    "enabledAxes",          enabledAxes ());
	addField (inputOutput,    "motor1Axis",           motor1Axis ());
	addField (inputOutput,    "motor2Axis",           motor2Axis ());
	addField (inputOutput,    "motor3Axis",           motor3Axis ());
	addField (inputOutput,    "stop1Bounce",          stop1Bounce ());
	addField (inputOutput,    "stop1ErrorCorrection", stop1ErrorCorrection ());
	addField (inputOutput,    "stop2Bounce",          stop2Bounce ());
	addField (inputOutput,    "stop2ErrorCorrection", stop2ErrorCorrection ());
	addField (inputOutput,    "stop3Bounce",          stop3Bounce ());
	addField (inputOutput,    "stop3ErrorCorrection", stop3ErrorCorrection ());
	addField (outputOnly,     "motor1Angle",          motor1Angle ());
	addField (outputOnly,     "motor1AngleRate",      motor1AngleRate ());
	addField (outputOnly,     "motor2Angle",          motor2Angle ());
	addField (outputOnly,     "motor2AngleRate",      motor2AngleRate ());
	addField (outputOnly,     "motor3Angle",          motor3Angle ());
	addField (outputOnly,     "motor3AngleRate",      motor3AngleRate ());
	addField (initializeOnly, "autoCalc",             autoCalc ());
}

X3DBaseNode*
MotorJoint::create (X3DExecutionContext* const executionContext) const
{
	return new MotorJoint (executionContext);
}

void
MotorJoint::addJoint ()
{
}

void
MotorJoint::removeJoint ()
{
}

void
MotorJoint::update ()
{
}

} // X3D
} // titania

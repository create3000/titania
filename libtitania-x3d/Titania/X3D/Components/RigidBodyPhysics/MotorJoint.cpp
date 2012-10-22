/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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
 ******************************************************************************/

#include "MotorJoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

MotorJoint::MotorJoint (X3DExecutionContext* const executionContext) :
	        X3DBasicNode (executionContext -> getBrowser (), executionContext),
	   X3DRigidJointNode (),                                 
	          axis1Angle (),                                 // SFFloat [in,out] axis1Angle            0            [-π,π]
	         axis1Torque (),                                 // SFFloat [in,out] axis1Torque           0            (-∞,∞)
	          axis2Angle (),                                 // SFFloat [in,out] axis2Angle            0            [-π,π]
	         axis2Torque (),                                 // SFFloat [in,out] axis2Torque           0            (-∞,∞)
	          axis3Angle (),                                 // SFFloat [in,out] axis3Angle            0            [-π,π]
	         axis3Torque (),                                 // SFFloat [in,out] axis3Torque           0            (-∞,∞)
	         enabledAxes (1),                                // SFInt32 [in,out] enabledAxes           1            [0,3]
	          motor1Axis (),                                 // SFVec3f [in,out] motor1Axis            0 0 0
	          motor2Axis (),                                 // SFVec3f [in,out] motor2Axis            0 0 0
	          motor3Axis (),                                 // SFVec3f [in,out] motor3Axis            0 0 0
	         stop1Bounce (),                                 // SFFloat [in,out] stop1Bounce           0            [0,1]
	stop1ErrorCorrection (0.8),                              // SFFloat [in,out] stop1ErrorCorrection  0.8          [0,1]
	         stop2Bounce (),                                 // SFFloat [in,out] stop2Bounce           0            [0,1]
	stop2ErrorCorrection (0.8),                              // SFFloat [in,out] stop2ErrorCorrection  0.8          [0,1]
	         stop3Bounce (),                                 // SFFloat [in,out] stop3Bounce           0            [0,1]
	stop3ErrorCorrection (0.8),                              // SFFloat [in,out] stop3ErrorCorrection  0.8          [0,1]
	         motor1Angle (),                                 // SFFloat [out]    motor1Angle
	     motor1AngleRate (),                                 // SFFloat [out]    motor1AngleRate
	         motor2Angle (),                                 // SFFloat [out]    motor2Angle
	     motor2AngleRate (),                                 // SFFloat [out]    motor2AngleRate
	         motor3Angle (),                                 // SFFloat [out]    motor3Angle
	     motor3AngleRate (),                                 // SFFloat [out]    motor3AngleRate
	            autoCalc ()                                  // SFBool  [ ]      autoCalc              FALSE
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("MotorJoint");

	appendField (inputOutput,    "metadata",             metadata);
	appendField (inputOutput,    "body1",                body1);
	appendField (inputOutput,    "body2",                body2);
	appendField (inputOutput,    "forceOutput",          forceOutput);
	appendField (inputOutput,    "axis1Angle",           axis1Angle);
	appendField (inputOutput,    "axis1Torque",          axis1Torque);
	appendField (inputOutput,    "axis2Angle",           axis2Angle);
	appendField (inputOutput,    "axis2Torque",          axis2Torque);
	appendField (inputOutput,    "axis3Angle",           axis3Angle);
	appendField (inputOutput,    "axis3Torque",          axis3Torque);
	appendField (inputOutput,    "enabledAxes",          enabledAxes);
	appendField (inputOutput,    "motor1Axis",           motor1Axis);
	appendField (inputOutput,    "motor2Axis",           motor2Axis);
	appendField (inputOutput,    "motor3Axis",           motor3Axis);
	appendField (inputOutput,    "stop1Bounce",          stop1Bounce);
	appendField (inputOutput,    "stop1ErrorCorrection", stop1ErrorCorrection);
	appendField (inputOutput,    "stop2Bounce",          stop2Bounce);
	appendField (inputOutput,    "stop2ErrorCorrection", stop2ErrorCorrection);
	appendField (inputOutput,    "stop3Bounce",          stop3Bounce);
	appendField (inputOutput,    "stop3ErrorCorrection", stop3ErrorCorrection);
	appendField (outputOnly,     "motor1Angle",          motor1Angle);
	appendField (outputOnly,     "motor1AngleRate",      motor1AngleRate);
	appendField (outputOnly,     "motor2Angle",          motor2Angle);
	appendField (outputOnly,     "motor2AngleRate",      motor2AngleRate);
	appendField (outputOnly,     "motor3Angle",          motor3Angle);
	appendField (outputOnly,     "motor3AngleRate",      motor3AngleRate);
	appendField (initializeOnly, "autoCalc",             autoCalc);
}

X3DBasicNode*
MotorJoint::create (X3DExecutionContext* const executionContext) const
{
	return new MotorJoint (executionContext);
}

} // X3D
} // titania

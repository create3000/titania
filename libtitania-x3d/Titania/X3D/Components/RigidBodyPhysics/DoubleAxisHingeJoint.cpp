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

#include "DoubleAxisHingeJoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

DoubleAxisHingeJoint::DoubleAxisHingeJoint (X3DExecutionContext* const executionContext) :
	             X3DBasicNode (executionContext -> getBrowser (), executionContext),
	        X3DRigidJointNode (),                                 
	              anchorPoint (),                                 // SFVec3f [in,out] anchorPoint                0 0 0
	                    axis1 (),                                 // SFVec3f [in,out] axis1                      0 0 0
	                    axis2 (),                                 // SFVec3f [in,out] axis2                      0 0 0
	  desiredAngularVelocity1 (),                                 // SFFloat [in,out] desiredAngularVelocity1    0            (-∞,∞)
	  desiredAngularVelocity2 (),                                 // SFFloat [in,out] desiredAngularVelocity2    0            (-∞,∞)
	                maxAngle1 (),                                 // SFFloat [in,out] maxAngle1                  π            [-π,π]
	               maxTorque1 (),                                 // SFFloat [in,out] maxTorque1                 0            (-∞,∞)
	               maxTorque2 (),                                 // SFFloat [in,out] maxTorque2                 0            (-∞,∞)
	                minAngle1 (),                                 // SFFloat [in,out] minAngle1                  -π           [-π,π]
	              stopBounce1 (),                                 // SFFloat [in,out] stopBounce1                0            [0,1]
	    stopConstantForceMix1 (0.001),                            // SFFloat [in,out] stopConstantForceMix1      0.001        [0,∞)
	     stopErrorCorrection1 (0.8),                              // SFFloat [in,out] stopErrorCorrection1       0.8          [0,1]
	suspensionErrorCorrection (0.8),                              // SFFloat [in,out] suspensionErrorCorrection  0.8          [0,1]
	          suspensionForce (),                                 // SFFloat [in,out] suspensionForce            0            [0,∞)
	         body1AnchorPoint (),                                 // SFVec3f [out]    body1AnchorPoint
	                body1Axis (),                                 // SFVec3f [out]    body1Axis
	         body2AnchorPoint (),                                 // SFVec3f [out]    body2AnchorPoint
	                body2Axis (),                                 // SFVec3f [out]    body2Axis
	              hinge1Angle (),                                 // SFFloat [out]    hinge1Angle
	          hinge1AngleRate (),                                 // SFFloat [out]    hinge1AngleRate
	              hinge2Angle (),                                 // SFFloat [out]    hinge2Angle
	          hinge2AngleRate ()                                  // SFFloat [out]    hinge2AngleRate
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("DoubleAxisHingeJoint");

	appendField (inputOutput, "metadata",                  metadata);
	appendField (inputOutput, "body1",                     body1);
	appendField (inputOutput, "body2",                     body2);
	appendField (inputOutput, "forceOutput",               forceOutput);
	appendField (inputOutput, "anchorPoint",               anchorPoint);
	appendField (inputOutput, "axis1",                     axis1);
	appendField (inputOutput, "axis2",                     axis2);
	appendField (inputOutput, "desiredAngularVelocity1",   desiredAngularVelocity1);
	appendField (inputOutput, "desiredAngularVelocity2",   desiredAngularVelocity2);
	appendField (inputOutput, "maxAngle1",                 maxAngle1);
	appendField (inputOutput, "maxTorque1",                maxTorque1);
	appendField (inputOutput, "maxTorque2",                maxTorque2);
	appendField (inputOutput, "minAngle1",                 minAngle1);
	appendField (inputOutput, "stopBounce1",               stopBounce1);
	appendField (inputOutput, "stopConstantForceMix1",     stopConstantForceMix1);
	appendField (inputOutput, "stopErrorCorrection1",      stopErrorCorrection1);
	appendField (inputOutput, "suspensionErrorCorrection", suspensionErrorCorrection);
	appendField (inputOutput, "suspensionForce",           suspensionForce);
	appendField (outputOnly,  "body1AnchorPoint",          body1AnchorPoint);
	appendField (outputOnly,  "body1Axis",                 body1Axis);
	appendField (outputOnly,  "body2AnchorPoint",          body2AnchorPoint);
	appendField (outputOnly,  "body2Axis",                 body2Axis);
	appendField (outputOnly,  "hinge1Angle",               hinge1Angle);
	appendField (outputOnly,  "hinge1AngleRate",           hinge1AngleRate);
	appendField (outputOnly,  "hinge2Angle",               hinge2Angle);
	appendField (outputOnly,  "hinge2AngleRate",           hinge2AngleRate);
}

X3DBasicNode*
DoubleAxisHingeJoint::create (X3DExecutionContext* const executionContext) const
{
	return new DoubleAxisHingeJoint (executionContext);
}

} // X3D
} // titania

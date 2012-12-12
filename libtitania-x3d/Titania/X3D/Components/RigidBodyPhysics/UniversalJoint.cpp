/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "UniversalJoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

UniversalJoint::UniversalJoint (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	   X3DRigidJointNode (),                                                    
	         anchorPoint (),                                                    // SFVec3f [in,out] anchorPoint           0 0 0
	               axis1 (),                                                    // SFVec3f [in,out] axis1                 0 0 0
	               axis2 (),                                                    // SFVec3f [in,out] axis2                 0 0 0
	         stopBounce1 (),                                                    // SFFloat [in,out] stopBounce1           0            [0,1]
	stop1ErrorCorrection (0.8),                                                 // SFFloat [in,out] stop1ErrorCorrection  0.8          [0,1]
	         stop2Bounce (),                                                    // SFFloat [in,out] stop2Bounce           0            [0,1]
	stop2ErrorCorrection (0.8),                                                 // SFFloat [in,out] stop2ErrorCorrection  0.8          [0,1]
	    body1AnchorPoint (),                                                    // SFVec3f [out]    body1AnchorPoint
	           body1Axis (),                                                    // SFVec3f [out]    body1Axis
	    body2AnchorPoint (),                                                    // SFVec3f [out]    body2AnchorPoint
	           body2Axis ()                                                     // SFVec3f [out]    body2Axis
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("UniversalJoint");

	addField (inputOutput, "metadata",             metadata);
	addField (inputOutput, "body1",                body1);
	addField (inputOutput, "body2",                body2);
	addField (inputOutput, "forceOutput",          forceOutput);
	addField (inputOutput, "anchorPoint",          anchorPoint);
	addField (inputOutput, "axis1",                axis1);
	addField (inputOutput, "axis2",                axis2);
	addField (inputOutput, "stopBounce1",          stopBounce1);
	addField (inputOutput, "stop1ErrorCorrection", stop1ErrorCorrection);
	addField (inputOutput, "stop2Bounce",          stop2Bounce);
	addField (inputOutput, "stop2ErrorCorrection", stop2ErrorCorrection);
	addField (outputOnly,  "body1AnchorPoint",     body1AnchorPoint);
	addField (outputOnly,  "body1Axis",            body1Axis);
	addField (outputOnly,  "body2AnchorPoint",     body2AnchorPoint);
	addField (outputOnly,  "body2Axis",            body2Axis);
}

X3DBaseNode*
UniversalJoint::create (X3DExecutionContext* const executionContext) const
{
	return new UniversalJoint (executionContext);
}

} // X3D
} // titania

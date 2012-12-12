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

#include "SliderJoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

SliderJoint::SliderJoint (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	  X3DRigidJointNode (),                                                    
	               axis (0, 1, 0),                                             // SFVec3f [in,out] axis                 0 1 0
	      maxSeparation (1),                                                   // SFFloat [in,out] maxSeparation        1            [0,∞)
	      minSeparation (),                                                    // SFFloat [in,out] minSeparation        0            [0,∞)
	         stopBounce (),                                                    // SFFloat [in,out] stopBounce           0            [0,1]
	stopErrorCorrection (1),                                                   // SFFloat [in,out] stopErrorCorrection  1            [0,1]
	         separation (),                                                    // SFFloat [out]    separation
	     separationRate ()                                                     // SFFloat [out]    separationRate
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("SliderJoint");

	addField (inputOutput, "metadata",            metadata);
	addField (inputOutput, "body1",               body1);
	addField (inputOutput, "body2",               body2);
	addField (inputOutput, "forceOutput",         forceOutput);
	addField (inputOutput, "axis",                axis);
	addField (inputOutput, "maxSeparation",       maxSeparation);
	addField (inputOutput, "minSeparation",       minSeparation);
	addField (inputOutput, "stopBounce",          stopBounce);
	addField (inputOutput, "stopErrorCorrection", stopErrorCorrection);
	addField (outputOnly,  "separation",          separation);
	addField (outputOnly,  "separationRate",      separationRate);
}

X3DBaseNode*
SliderJoint::create (X3DExecutionContext* const executionContext) const
{
	return new SliderJoint (executionContext);
}

} // X3D
} // titania

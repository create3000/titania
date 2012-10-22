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

#include "Contact.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

Contact::Contact (X3DExecutionContext* const executionContext) :
	            X3DBasicNode (executionContext -> getBrowser (), executionContext),
	                 X3DNode (),                                 
	       appliedParameters ({ "BOUNCE" }),                     // MFString [in,out] appliedParameters         "BOUNCE"        [ ]
	                   body1 (),                                 // SFNode   [in,out] body1                     NULL            [RigidBody]
	                   body2 (),                                 // SFNode   [in,out] body2                     NULL            [RigidBody]
	                  bounce (),                                 // SFFloat  [in,out] bounce                    0               [0,1]
	           contactNormal (0, 1, 0),                          // SFVec3f  [in,out] contactNormal             0 1 0           (-∞,∞)
	                   depth (),                                 // SFFloat  [in,out] depth                     0               (-∞,∞)
	    frictionCoefficients (),                                 // SFVec2f  [in,out] frictionCoefficients      0 0             [0,∞)
	       frictionDirection (0, 1, 0),                          // SFVec3f  [in,out] frictionDirection         0 1 0           (-∞,∞)
	               geometry1 (),                                 // SFNode   [in,out] geometry1                 NULL            [X3DNBodyCollidableNode]
	               geometry2 (),                                 // SFNode   [in,out] geometry2                 NULL            [X3DNBodyCollidableNode]
	          minbounceSpeed (),                                 // SFFloat  [in,out] minbounceSpeed            0               [0,∞)
	                position (),                                 // SFVec3f  [in,out] position                  0 0 0           (-∞,∞)
	        slipCoefficients (),                                 // SFVec2f  [in,out] slipCoefficients          0 0             (-∞,∞)
	softnessConstantForceMix (0.0001),                           // SFFloat  [in,out] softnessConstantForceMix  0.0001          [0,1]
	 softnessErrorCorrection (0.8),                              // SFFloat  [in,out] softnessErrorCorrection   0.8             [0,1]
	            surfaceSpeed ()                                  // SFVec2f  [in,out] surfaceSpeed              0 0             (-∞,∞)
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("Contact");

	appendField (inputOutput, "metadata",                 metadata);
	appendField (inputOutput, "appliedParameters",        appliedParameters);
	appendField (inputOutput, "body1",                    body1);
	appendField (inputOutput, "body2",                    body2);
	appendField (inputOutput, "bounce",                   bounce);
	appendField (inputOutput, "contactNormal",            contactNormal);
	appendField (inputOutput, "depth",                    depth);
	appendField (inputOutput, "frictionCoefficients",     frictionCoefficients);
	appendField (inputOutput, "frictionDirection",        frictionDirection);
	appendField (inputOutput, "geometry1",                geometry1);
	appendField (inputOutput, "geometry2",                geometry2);
	appendField (inputOutput, "minbounceSpeed",           minbounceSpeed);
	appendField (inputOutput, "position",                 position);
	appendField (inputOutput, "slipCoefficients",         slipCoefficients);
	appendField (inputOutput, "softnessConstantForceMix", softnessConstantForceMix);
	appendField (inputOutput, "softnessErrorCorrection",  softnessErrorCorrection);
	appendField (inputOutput, "surfaceSpeed",             surfaceSpeed);
}

X3DBasicNode*
Contact::create (X3DExecutionContext* const executionContext) const
{
	return new Contact (executionContext);
}

} // X3D
} // titania

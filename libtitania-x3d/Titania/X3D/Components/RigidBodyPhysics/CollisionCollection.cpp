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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "CollisionCollection.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

CollisionCollection::CollisionCollection (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	            X3DChildNode (),                                                    
	       appliedParameters ({ "BOUNCE" }),                                        // MFString [in,out] appliedParameters         "BOUNCE"        [ ]
	                  bounce (),                                                    // SFFloat  [in,out] bounce                    0               [0,1]
	             collidables (),                                                    // MFNode   [in,out] collidables               NULL            [X3DNBodyCollisionSpaceNode|X3DNBodyCollidableNode]
	                 enabled (),                                                    // SFBool   [in,out] enabled                   TRUE
	    frictionCoefficients (),                                                    // SFVec2f  [in,out] frictionCoefficients      0 0             [0,∞)
	          minBounceSpeed (),                                                    // SFFloat  [in,out] minBounceSpeed            0.1             [0,∞)
	             slipFactors (),                                                    // SFVec2f  [in,out] slipFactors               0 0             (-∞,∞)
	softnessConstantForceMix (),                                                    // SFFloat  [in,out] softnessConstantForceMix  0.0001          [0,1]
	 softnessErrorCorrection (),                                                    // SFFloat  [in,out] softnessErrorCorrection   0.8             [0,1]
	            surfaceSpeed ()                                                     // SFVec2f  [in,out] surfaceSpeed              0 0             (-∞,∞)
{
	setComponent ("RigidBodyPhysics");
	setTypeName ("CollisionCollection");

	addField (inputOutput, "metadata",                 metadata);
	addField (inputOutput, "appliedParameters",        appliedParameters);
	addField (inputOutput, "bounce",                   bounce);
	addField (inputOutput, "collidables",              collidables);
	addField (inputOutput, "enabled",                  enabled);
	addField (inputOutput, "frictionCoefficients",     frictionCoefficients);
	addField (inputOutput, "minBounceSpeed",           minBounceSpeed);
	addField (inputOutput, "slipFactors",              slipFactors);
	addField (inputOutput, "softnessConstantForceMix", softnessConstantForceMix);
	addField (inputOutput, "softnessErrorCorrection",  softnessErrorCorrection);
	addField (inputOutput, "surfaceSpeed",             surfaceSpeed);
}

X3DBaseNode*
CollisionCollection::create (X3DExecutionContext* const executionContext) const
{
	return new CollisionCollection (executionContext);
}

} // X3D
} // titania

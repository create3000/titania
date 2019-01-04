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

#include "Contact.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string Contact::componentName  = "RigidBodyPhysics";
const std::string Contact::typeName       = "Contact";
const std::string Contact::containerField = "children";

Contact::Fields::Fields () :
	                position (new SFVec3f ()),
	           contactNormal (new SFVec3f (0, 1, 0)),
	                   depth (new SFFloat ()),
	       frictionDirection (new SFVec3f (0, 1, 0)),
	       appliedParameters (new MFString ({ "BOUNCE" })),
	                  bounce (new SFFloat ()),
	          minBounceSpeed (new SFFloat ()),
	    frictionCoefficients (new SFVec2f ()),
	            surfaceSpeed (new SFVec2f ()),
	        slipCoefficients (new SFVec2f ()),
	softnessConstantForceMix (new SFFloat (0.0001)),
	 softnessErrorCorrection (new SFFloat (0.8)),
	               geometry1 (new SFNode ()),
	               geometry2 (new SFNode ()),
	                   body1 (new SFNode ()),
	                   body2 (new SFNode ())
{ }

Contact::Contact (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DNode (),
	     fields ()
{
	addType (X3DConstants::Contact);

	addField (inputOutput, "metadata",                 metadata ());
	addField (inputOutput, "position",                 position ());
	addField (inputOutput, "contactNormal",            contactNormal ());
	addField (inputOutput, "depth",                    depth ());
	addField (inputOutput, "frictionDirection",        frictionDirection ());

	addField (inputOutput, "appliedParameters",        appliedParameters ());
	
	addField (inputOutput, "bounce",                   bounce ());
	addField (inputOutput, "minBounceSpeed",           minBounceSpeed ());
	addField (inputOutput, "frictionCoefficients",     frictionCoefficients ());
	addField (inputOutput, "surfaceSpeed",             surfaceSpeed ());

	addField (inputOutput, "slipCoefficients",         slipCoefficients ());
	addField (inputOutput, "softnessConstantForceMix", softnessConstantForceMix ());
	addField (inputOutput, "softnessErrorCorrection",  softnessErrorCorrection ());

	addField (inputOutput, "geometry1",                geometry1 ());
	addField (inputOutput, "geometry2",                geometry2 ());
	addField (inputOutput, "body1",                    body1 ());
	addField (inputOutput, "body2",                    body2 ());

	// Units

	position ()                 .setUnit (UnitCategory::LENGTH);
	depth ()                    .setUnit (UnitCategory::LENGTH);
	minBounceSpeed ()           .setUnit (UnitCategory::SPEED);
	surfaceSpeed ()             .setUnit (UnitCategory::SPEED);
	softnessConstantForceMix () .setUnit (UnitCategory::FORCE);
}

X3DBaseNode*
Contact::create (X3DExecutionContext* const executionContext) const
{
	return new Contact (executionContext);
}

} // X3D
} // titania

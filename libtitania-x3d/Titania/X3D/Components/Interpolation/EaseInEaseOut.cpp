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

#include "EaseInEaseOut.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   EaseInEaseOut::component      = Component ("Interpolation", 4);
const std::string EaseInEaseOut::typeName       = "EaseInEaseOut";
const std::string EaseInEaseOut::containerField = "children";

EaseInEaseOut::Fields::Fields () :
	           easeInEaseOut (new MFVec2f ()),
	modifiedFraction_changed (new SFFloat ())
{ }

EaseInEaseOut::EaseInEaseOut (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DInterpolatorNode (),
	             fields ()
{
	addType (X3DConstants::EaseInEaseOut);

	addField (inputOutput, "metadata",                 metadata ());
	addField (inputOnly,   "set_fraction",             set_fraction ());
	addField (inputOutput, "key",                      key ());
	addField (inputOutput, "easeInEaseOut",            easeInEaseOut ());
	addField (outputOnly,  "modifiedFraction_changed", modifiedFraction_changed ());
}

X3DBaseNode*
EaseInEaseOut::create (X3DExecutionContext* const executionContext) const
{
	return new EaseInEaseOut (executionContext);
}

void
EaseInEaseOut::initialize ()
{
	X3DInterpolatorNode::initialize ();

	easeInEaseOut () .addInterest (&EaseInEaseOut::set_keyValue, this);
}

void
EaseInEaseOut::set_keyValue ()
{
	if (easeInEaseOut () .size () < key () .size ())
		easeInEaseOut () .resize (key () .size (), easeInEaseOut () .size () ? easeInEaseOut () .back () : Vector2f ());
}

void
EaseInEaseOut::interpolate (size_t index0, size_t index1, const float weight)
{
	float       easeOut = easeInEaseOut () .get1Value (index0) .y ();
	float       easeIn  = easeInEaseOut () .get1Value (index1) .x ();
	const float sum     = easeOut + easeIn;

	if (sum < 0)
	{
		modifiedFraction_changed () = weight;
	}
	else
	{
		if (sum > 1)
		{
			easeIn  /= sum;
			easeOut /= sum;
		}

		const float t = 1 / (2 - easeOut - easeIn);

		if (weight < easeOut)
		{
			modifiedFraction_changed () = (t / easeOut) * math::sqr (weight);
		}
		else if (weight <= 1 - easeIn) // Spec says (weight < 1 - easeIn), but then we get a NaN below if easeIn == 0.
		{
			modifiedFraction_changed () = t * (2 * weight - easeOut);
		}
		else
		{
			modifiedFraction_changed () = 1 - ((t * math::sqr (1 - weight)) / easeIn);
		}
	}
}

} // X3D
} // titania

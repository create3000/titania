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

#include "PositionChaser2D.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   PositionChaser2D::component      = Component ("Followers", 1);
const std::string PositionChaser2D::typeName       = "PositionChaser2D";
const std::string PositionChaser2D::containerField = "children";

PositionChaser2D::Fields::Fields () :
	         set_value (new SFVec2f ()),
	   set_destination (new SFVec2f ()),
	      initialValue (new SFVec2f ()),
	initialDestination (new SFVec2f ()),
	     value_changed (new SFVec2f ())
{ }

PositionChaser2D::PositionChaser2D (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DChaserNode (),
	       fields (),
	bufferEndTime (0),
	previousValue (),
	       buffer ()
{
	addType (X3DConstants::PositionChaser2D);

	addField (inputOutput,    "metadata",           metadata ());
	addField (inputOnly,      "set_value",          set_value ());
	addField (inputOnly,      "set_destination",    set_destination ());
	addField (initializeOnly, "initialValue",       initialValue ());
	addField (initializeOnly, "initialDestination", initialDestination ());
	addField (initializeOnly, "duration",           duration ());
	addField (outputOnly,     "isActive",           isActive ());
	addField (outputOnly,     "value_changed",      value_changed ());
}

X3DBaseNode*
PositionChaser2D::create (X3DExecutionContext* const executionContext) const
{
	return new PositionChaser2D (executionContext);
}

void
PositionChaser2D::initialize ()
{
	X3DChaserNode::initialize ();

	set_value ()       .addInterest (&PositionChaser2D::set_value_, this);
	set_destination () .addInterest (&PositionChaser2D::set_destination_, this);

	bufferEndTime = getCurrentTime ();
	previousValue = initialValue ();

	buffer .resize (getNumBuffers (), initialValue ());
	buffer [0] = initialDestination ();

	set_destination () .set (initialDestination ());

	if (equals (initialDestination (), initialValue (), getTolerance ()))
		value_changed () = initialDestination ();

	else
		set_active (true);
}

bool
PositionChaser2D::equals (const Vector2f & lhs, const Vector2f & rhs, const float tolerance) const
{
	return abs (lhs - rhs) < tolerance;
}

void
PositionChaser2D::set_value_ ()
{
	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	for (auto & value : buffer)
		value = set_value ();

	previousValue = set_value ();

	value_changed () = set_value ();

	set_active (true);
}

void
PositionChaser2D::set_destination_ ()
{
	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	set_active (true);
}

void
PositionChaser2D::prepareEvents ()
{
	const float fraction = updateBuffer ();
	auto        output   = lerp (previousValue, buffer [buffer .size () - 1], (float) stepResponse ((buffer .size () - 1 + fraction) * getStepTime ()));

	for (int32_t i = buffer .size () - 2; i >= 0; -- i)
	{
		const auto deltaIn  = buffer [i] - buffer [i + 1];
		const auto deltaOut = deltaIn * (float) stepResponse ((i + fraction) * getStepTime ());

		output += deltaOut;
	}

	value_changed () = output;

	if (equals (output, set_destination (), getTolerance ()))
		set_active (false);
}

float
PositionChaser2D::updateBuffer ()
{
	float fraction = (getCurrentTime () - bufferEndTime) / getStepTime ();

	if (fraction >= 1)
	{
		float seconds;
		fraction = std::modf (fraction, &seconds);

		if (seconds < buffer .size ())
		{
			previousValue = buffer [buffer .size () - seconds];

			for (int32_t i = buffer .size () - 1; i >= seconds; -- i)
			{
				buffer [i] = buffer [i - seconds];
			}

			for (size_t i = 0; i < seconds; ++ i)
			{
				const float alpha = i / seconds;

				buffer [i] = lerp (set_destination () .getValue (), buffer [seconds], alpha);
 			}
		}
		else
		{
			previousValue = seconds == buffer .size () ? buffer [0] : set_destination ();

			for (auto & value : buffer)
				value = set_destination ();
		}

		bufferEndTime += seconds * getStepTime ();
	}

	return fraction;
}

} // X3D
} // titania

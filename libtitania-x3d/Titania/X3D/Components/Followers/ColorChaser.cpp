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

#include "ColorChaser.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   ColorChaser::component      = Component ("Followers", 1);
const std::string ColorChaser::typeName       = "ColorChaser";
const std::string ColorChaser::containerField = "children";

ColorChaser::Fields::Fields () :
	         set_value (new SFColor ()),
	   set_destination (new SFColor ()),
	      initialValue (new SFColor (0.8, 0.8, 0.8)),
	initialDestination (new SFColor (0.8, 0.8, 0.8)),
	     value_changed (new SFColor ())
{ }

ColorChaser::ColorChaser (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DChaserNode (),
	       fields (),
	bufferEndTime (0),
	previousValue (),
	       buffer ()
{
	addType (X3DConstants::ColorChaser);

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
ColorChaser::create (X3DExecutionContext* const executionContext) const
{
	return new ColorChaser (executionContext);
}

void
ColorChaser::initialize ()
{
	X3DChaserNode::initialize ();

	set_value ()       .addInterest (&ColorChaser::set_value_,       this);
	set_destination () .addInterest (&ColorChaser::set_destination_, this);

	bufferEndTime = getCurrentTime ();
	previousValue = initialValue () .getHSV ();

	buffer .resize (getNumBuffers (), previousValue);
	buffer [0] = initialDestination () .getHSV ();

	set_destination () .set (initialDestination ());

	if (equals (initialDestination () .getHSV (), initialValue () .getHSV (), getTolerance ()))
		value_changed () = initialDestination ();

	else
		set_active (true);
}

bool
ColorChaser::equals (const Vector3f & lhs, const Vector3f & rhs, const float tolerance) const
{
	return abs (lhs - rhs) < tolerance;
}

void
ColorChaser::set_value_ ()
{
	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	const auto hsv = set_value () .getHSV ();

	for (auto & value : buffer)
		value = hsv;

	previousValue = hsv;

	value_changed () = set_value ();

	set_active (true);
}

void
ColorChaser::set_destination_ ()
{
	bufferEndTime = getCurrentTime ();

	set_active (true);
}

void
ColorChaser::prepareEvents ()
{
	const float fraction = updateBuffer ();

	auto output = hsv_lerp <float> (previousValue, buffer [buffer .size () - 1], stepResponse ((buffer .size () - 1 + fraction) * getStepTime ()));

	for (int32_t i = buffer .size () - 2; i >= 0; -- i)
	{
		const float t    = stepResponse ((i + fraction) * getStepTime ());
		const auto  dhsv = output + buffer [i] - buffer [i + 1];

		output = hsv_lerp (output, Vector3f (interval (dhsv [0], 0.0f, 2 * pi <float>), dhsv [1], dhsv [2]), t);
	}

	value_changed () .setHSV (output);

	if (equals (output, set_destination () .getHSV (), getTolerance ()))
		set_active (false);
}

float
ColorChaser::updateBuffer ()
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

				buffer [i] = hsv_lerp (set_destination () .getHSV (), buffer [seconds], alpha);
 			}
		}
		else
		{
			previousValue = seconds == buffer .size () ? buffer [0] : set_destination () .getHSV ();

			const auto hsv = set_destination () .getHSV ();

			for (auto & value : buffer)
				value = hsv;
		}

		bufferEndTime += seconds * getStepTime ();
	}

	return fraction;
}

} // X3D
} // titania

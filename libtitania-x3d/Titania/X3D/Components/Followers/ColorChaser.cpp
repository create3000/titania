/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "ColorChaser.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string ColorChaser::componentName  = "Followers";
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

	set_value ()       .addInterest (this, &ColorChaser::_set_value);
	set_destination () .addInterest (this, &ColorChaser::_set_destination);

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
ColorChaser::equals (const Color3f & lhs, const Color3f & rhs, float tolerance) const
{
	Vector3f a (lhs .r (), lhs .g (), lhs .b ());
	Vector3f b (rhs .r (), rhs .g (), rhs .b ());

	return abs (a - b) < tolerance;
}

void
ColorChaser::_set_value ()
{
	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	for (auto & value : basic::adapter (buffer .begin () + 1, buffer .end ()))
		value = set_value ();

	previousValue = set_value ();

	value_changed () = set_value ();

	set_active (true);
}

void
ColorChaser::_set_destination ()
{
	bufferEndTime = getCurrentTime ();

	set_active (true);
}

template <class Type>
static
void
hsv_lerp (const Type & a_h, const Type & a_s, const Type & a_v,
          const Type & b_h, const Type & b_s, const Type & b_v,
          const Type & t,
          Type & r_h, Type & r_s, Type & r_v)
{
	Type range = std::abs (b_h - a_h);

	if (range <= M_PI)
	{
		r_h = math::lerp (a_h, b_h, t);
		r_s = math::lerp (a_s, b_s, t);
		r_v = math::lerp (a_v, b_v, t);
	}
	else
	{
		Type step = (M_PI2 - range) * t;
		Type h    = a_h < b_h ? a_h - step : a_h + step;

		if (h < 0)
			h += M_PI2;

		else if (h > M_PI2)
			h -= M_PI2;

		r_h = h;
		r_s = math::lerp (a_s, b_s, t);
		r_v = math::lerp (a_v, b_v, t);
	}
}

void
ColorChaser::prepareEvents ()
{
	float fraction = updateBuffer ();

	auto output = clerp (previousValue, buffer [buffer .size () - 1], stepResponse ((buffer .size () - 1 + fraction) * getStepTime ()));
	
	float ho, so, vo;
	
	output .getHSV (ho, so, vo);

	for (int32_t i = buffer .size () - 2; i >= 0; -- i)
	{
		float h, s, v, h1, s1, v1;

		buffer [i] .getHSV (h, s, v);
		buffer [i + 1] .getHSV (h1, s1, v1);
		
		float t = stepResponse ((i + fraction) * getStepTime ());
	
		hsv_lerp (ho, so, vo,
		          interval (ho + h - h1, 0.0f, float (2 * M_PI)),
		          so + s - s1,
		          vo + v - v1,
		          t,
		          ho, so, vo);
	}

	value_changed () .setHSV (ho, so, vo);

	if (equals (output, set_destination (), getTolerance ()))
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
				float alpha = i / seconds;

				buffer [i] = clerp (set_destination () .getValue (), buffer [seconds], alpha);
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

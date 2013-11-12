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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "ScalarChaser.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string ScalarChaser::componentName  = "Followers";
const std::string ScalarChaser::typeName       = "ScalarChaser";
const std::string ScalarChaser::containerField = "children";

ScalarChaser::Fields::Fields () :
	         set_value (new SFFloat ()),
	   set_destination (new SFFloat ()),
	      initialValue (new SFFloat ()),
	initialDestination (new SFFloat ()),
	     value_changed (new SFFloat ())
{ }

ScalarChaser::ScalarChaser (X3DExecutionContext* const executionContext) :
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
ScalarChaser::create (X3DExecutionContext* const executionContext) const
{
	return new ScalarChaser (executionContext);
}

void
ScalarChaser::initialize ()
{
	X3DChaserNode::initialize ();

	set_value ()       .addInterest (this, &ScalarChaser::_set_value);
	set_destination () .addInterest (this, &ScalarChaser::_set_destination);

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
ScalarChaser::equals (const float & lhs, const float & rhs, float tolerance) const
{
	return abs (lhs - rhs) < tolerance;
}

void
ScalarChaser::_set_value ()
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
ScalarChaser::_set_destination ()
{
	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	set_active (true);
}

void
ScalarChaser::prepareEvents ()
{
	float fraction = updateBuffer ();

	auto output = lerp (previousValue, buffer [buffer .size () - 1], stepResponse ((buffer .size () - 1 + fraction) * getStepTime ()));

	for (int32_t i = buffer .size () - 2; i >= 0; -- i)
	{
		auto deltaIn = buffer [i] - buffer [i + 1];

		auto deltaOut = deltaIn * stepResponse ((i + fraction) * getStepTime ());

		output += deltaOut;
	}

	value_changed () = output;

	if (equals (output, set_destination (), getTolerance ()))
		set_active (false);
}

float
ScalarChaser::updateBuffer ()
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

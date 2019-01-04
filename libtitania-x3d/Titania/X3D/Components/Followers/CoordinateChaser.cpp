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

#include "CoordinateChaser.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string CoordinateChaser::componentName  = "Followers";
const std::string CoordinateChaser::typeName       = "CoordinateChaser";
const std::string CoordinateChaser::containerField = "children";

CoordinateChaser::Fields::Fields () :
	         set_value (new MFVec3f ()),
	   set_destination (new MFVec3f ()),
	      initialValue (new MFVec3f ({ SFVec3f () })),
	initialDestination (new MFVec3f ({ SFVec3f () })),
	     value_changed (new MFVec3f ())
{ }

CoordinateChaser::CoordinateChaser (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DChaserNode (),
	       fields (),
	bufferEndTime (0),
	previousValue (),
	       buffer ()
{
	addType (X3DConstants::CoordinateChaser);

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
CoordinateChaser::create (X3DExecutionContext* const executionContext) const
{
	return new CoordinateChaser (executionContext);
}

void
CoordinateChaser::initialize ()
{
	X3DChaserNode::initialize ();

	set_value ()       .addInterest (&CoordinateChaser::set_value_, this);
	set_destination () .addInterest (&CoordinateChaser::set_destination_, this);

	bufferEndTime = getCurrentTime ();
	previousValue .assign (initialValue () .begin (), initialValue () .end ());

	buffer .resize (getNumBuffers ());

	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
	{
		value .assign (initialValue () .cbegin (), initialValue () .cend ());
		value .resize (initialDestination () .size ());
	}

	buffer [0] .assign (initialDestination () .cbegin (), initialDestination () .cend ());

	set_destination () .set (initialDestination ());

	if (equals (initialDestination (), initialValue (), getTolerance ()))
		value_changed () = initialDestination ();

	else
		set_active (true);
}

void
CoordinateChaser::set_value_ ()
{
	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	for (auto & value : buffer)
		value .assign (set_value () .cbegin (), set_value () .cend ());

	previousValue .assign (set_value () .cbegin (), set_value () .cend ());

	value_changed () = set_value ();

	set_active (true);
}

void
CoordinateChaser::set_destination_ ()
{
	for (auto & value : buffer)
		value .resize (set_destination () .size ());

	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	set_active (true);
}

void
CoordinateChaser::prepareEvents ()
{
	std::vector <Vector3f> output (set_destination () .size ());

	const float fraction = updateBuffer ();

	const float alpha = stepResponse ((buffer .size () - 1 + fraction) * getStepTime ());

	for (size_t j = 0, size = set_destination () .size (); j < size; ++ j)
		output [j] = lerp (previousValue [j], buffer [buffer .size () - 1] [j], alpha);

	for (int32_t i = buffer .size () - 2; i >= 0; -- i)
	{
		const float alpha = stepResponse ((i + fraction) * getStepTime ());
	
		for (size_t j = 0, size = set_destination () .size (); j < size; ++ j)
		{
			const auto deltaIn = buffer [i] [j] - buffer [i + 1] [j];

			const auto deltaOut = deltaIn * alpha;

			output [j] += deltaOut;
		}
	}

	value_changed () .assign (output .cbegin (), output .cend ());

	if (equals (output, set_destination (), getTolerance ()))
		set_active (false);
}

float
CoordinateChaser::updateBuffer ()
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
				for (size_t j = 0, size = set_destination () .size (); j < size; ++ j)
				{
					buffer [i] [j] = buffer [i - seconds] [j];
				}
			}

			for (size_t i = 0; i < seconds; ++ i)
			{
				const float alpha = i / seconds;

				for (size_t j = 0, size = set_destination () .size (); j < size; ++ j)
				{
					buffer [i] [j] = lerp (set_destination () [j] .get (), buffer [seconds] [j], alpha);
				}
 			}
		}
		else
		{
			if (seconds == buffer .size ())
				previousValue = buffer [0];
			else
				previousValue .assign (set_destination () .cbegin (), set_destination () .cend ());

			for (auto & value : buffer)
				value .assign (set_destination () .cbegin (), set_destination () .cend ());
		}

		bufferEndTime += seconds * getStepTime ();
	}

	return fraction;
}

} // X3D
} // titania

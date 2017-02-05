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

#include "TexCoordChaser2D.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType TexCoordChaser2D::component      = ComponentType::FOLLOWERS;
const std::string   TexCoordChaser2D::typeName       = "TexCoordChaser2D";
const std::string   TexCoordChaser2D::containerField = "children";

TexCoordChaser2D::Fields::Fields () :
	         set_value (new MFVec2f ()),
	   set_destination (new MFVec2f ()),
	      initialValue (new MFVec2f ()),
	initialDestination (new MFVec2f ()),
	     value_changed (new MFVec2f ())
{ }

TexCoordChaser2D::TexCoordChaser2D (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DChaserNode (),
	       fields (),
	bufferEndTime (0),
	previousValue (),
	       buffer ()
{
	addType (X3DConstants::TexCoordChaser2D);

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
TexCoordChaser2D::create (X3DExecutionContext* const executionContext) const
{
	return new TexCoordChaser2D (executionContext);
}

void
TexCoordChaser2D::initialize ()
{
	X3DChaserNode::initialize ();

	set_value ()       .addInterest (&TexCoordChaser2D::set_value_, this);
	set_destination () .addInterest (&TexCoordChaser2D::set_destination_, this);

	bufferEndTime = getCurrentTime ();
	previousValue .assign (initialValue () .begin (), initialValue () .end ());

	buffer .resize (getNumBuffers ());

	for (auto & value : std::make_pair (buffer .begin () + 1, buffer .end ()))
	{
		value .assign (initialValue () .begin (), initialValue () .end ());
		value .resize (initialDestination () .size ());
	}

	buffer [0] .assign (initialDestination () .begin (), initialDestination () .end ());

	set_destination () .set (initialDestination ());

	if (equals (initialDestination (), initialValue (), getTolerance ()))
		value_changed () = initialDestination ();

	else
		set_active (true);
}

void
TexCoordChaser2D::set_value_ ()
{
	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	for (auto & value : std::make_pair (buffer .begin () + 1, buffer .end ()))
		value .assign (set_value () .begin (), set_value () .end ());

	buffer [0] .resize (set_value () .size ());

	previousValue .assign (set_value () .begin (), set_value () .end ());

	value_changed () = set_value ();

	set_active (true);
}

void
TexCoordChaser2D::set_destination_ ()
{
	for (auto & value : buffer)
		value .resize (set_destination () .size ());

	if (not isActive ())
		bufferEndTime = getCurrentTime ();

	set_active (true);
}

void
TexCoordChaser2D::prepareEvents ()
{
	std::vector <Vector2f> output (set_destination () .size ());

	const float fraction = updateBuffer ();
	const float alpha    = stepResponse ((buffer .size () - 1 + fraction) * getStepTime ());

	for (size_t j = 0, size = set_destination () .size (); j < size; ++ j)
		output [j] = lerp (previousValue [j], buffer [buffer .size () - 1] [j], alpha);

	for (int32_t i = buffer .size () - 2; i >= 0; -- i)
	{
		const float alpha = stepResponse ((i + fraction) * getStepTime ());
	
		for (size_t j = 0, size = set_destination () .size (); j < size; ++ j)
		{
			const auto deltaIn  = buffer [i] [j] - buffer [i + 1] [j];
			const auto deltaOut = deltaIn * alpha;

			output [j] += deltaOut;
		}
	}

	value_changed () .assign (output .begin (), output .end ());

	if (equals (output, set_destination (), getTolerance ()))
		set_active (false);
}

float
TexCoordChaser2D::updateBuffer ()
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
					buffer [i] [j] = lerp (set_destination () [j] .getValue (), buffer [seconds] [j], alpha);
				}
 			}
		}
		else
		{
			if (seconds == buffer .size ())
				previousValue = buffer [0];
			else
				previousValue .assign (set_destination () .begin (), set_destination () .end ());

			for (auto & value : buffer)
				value .assign (set_destination () .begin (), set_destination () .end ());
		}

		bufferEndTime += seconds * getStepTime ();
	}

	return fraction;
}

} // X3D
} // titania

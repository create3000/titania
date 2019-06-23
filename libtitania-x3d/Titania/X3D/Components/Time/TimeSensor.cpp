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

#include "TimeSensor.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include <iostream>

namespace titania {
namespace X3D {

const Component   TimeSensor::component      = Component ("Time", 2);
const std::string TimeSensor::typeName       = "TimeSensor";
const std::string TimeSensor::containerField = "children";

// The field range (min fraction, current, max fraction) is NOT PUBLIC; all values should be in the interval [0, 1].
// It constrains the output of fraction_changed to the interval [min fraction, max fraction] and the start point of
// fraction_changed within this interval is current, that mean current must be min fraction < current < max fraction.

TimeSensor::Fields::Fields () :
	   cycleInterval (new SFTime (1)),
	           range ({ 0, 0, 1 }),     // current, first, last (in fractions)
	       cycleTime (new SFTime ()),
	fraction_changed (new SFFloat ()),
	            time (new SFTime ())
{ }

TimeSensor::TimeSensor (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       X3DSensorNode (),
	X3DTimeDependentNode (),
	              fields (),
	               cycle (0),
	            interval (0),
	            fraction (0),
	               first (0),
	                last (1),
	               scale (1)
{
	addType (X3DConstants::TimeSensor);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "enabled",          enabled ());
	addField (inputOutput, "cycleInterval",    cycleInterval ());
	addField (inputOutput, "loop",             loop ());
	addField (inputOutput, "startTime",        startTime ());
	addField (inputOutput, "resumeTime",       resumeTime ());
	addField (inputOutput, "pauseTime",        pauseTime ());
	addField (inputOutput, "stopTime",         stopTime ());
	addField (outputOnly,  "isPaused",         isPaused ());
	addField (outputOnly,  "isActive",         isActive ());
	addField (outputOnly,  "cycleTime",        cycleTime ());
	addField (outputOnly,  "elapsedTime",      elapsedTime ());
	addField (outputOnly,  "fraction_changed", fraction_changed ());
	addField (outputOnly,  "time",             time ());

	addChildObjects (range ());
}

X3DBaseNode*
TimeSensor::create (X3DExecutionContext* const executionContext) const
{
	return new TimeSensor (executionContext);
}

void
TimeSensor::initialize  ()
{
	X3DTimeDependentNode::initialize  ();

	cycleInterval () .addInterest (&TimeSensor::set_cycleInterval, this);
	range ()         .addInterest (&TimeSensor::set_range,         this);
}

void
TimeSensor::setRange (const float currentFraction, const float firstFraction, const float lastFraction)
{
	first    = firstFraction;
	last     = lastFraction;
	scale    = last - first;
	interval = cycleInterval () * scale;
	fraction = fract ((currentFraction >= 1 ? 0 : currentFraction) + (getCurrentTime () - startTime ()) / interval);
	cycle    = getCurrentTime () - (fraction - first) * cycleInterval ();
}

void
TimeSensor::set_cycleInterval ()
{
	if (isActive ())
		setRange (fraction, range () [1], range () [2]);
}

void
TimeSensor::set_range ()
{
	if (isActive ())
	{
		setRange (range () [0], range () [1], range () [2]);

		if (not isPaused ())
			set_fraction ();
	}
}

void
TimeSensor::set_start ()
{
	setRange (range () [0], range () [1], range () [2]);

	fraction_changed () = fraction;
	time ()             = getCurrentTime ();
}

void
TimeSensor::set_pause ()
{ }

void
TimeSensor::set_resume (const time_type pauseInterval)
{
	setRange (fract (fraction - (getCurrentTime () - startTime ()) / interval), range () [1], range () [2]);
}

void
TimeSensor::set_stop ()
{ }

void
TimeSensor::set_fraction ()
{
	fraction_changed () = fraction = first + fract ((getCurrentTime () - cycle) / interval) * scale;
}

void
TimeSensor::set_time ()
{
	// The event order below is very important.

	if (getCurrentTime () - cycle >= interval)
	{
		if (loop ())
		{
			if (interval)
			{
				cycle += interval * std::floor ((getCurrentTime () - cycle) / interval);

				elapsedTime () = getElapsedTime ();
				cycleTime ()   = getCurrentTime ();

				set_fraction ();
			}
		}
		else
		{
			fraction_changed () = fraction = last;
			stop ();
		}
	}
	else
	{
		elapsedTime () = getElapsedTime ();

		set_fraction ();
	}

	time () = getCurrentTime ();
}

TimeSensor::~TimeSensor ()
{ }

} // X3D
} // titania

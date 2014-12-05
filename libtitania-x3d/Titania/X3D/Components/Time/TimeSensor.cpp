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

#include "TimeSensor.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include <iostream>

namespace titania {
namespace X3D {

const ComponentType TimeSensor::component      = ComponentType::TIME;
const std::string   TimeSensor::typeName       = "TimeSensor";
const std::string   TimeSensor::containerField = "children";

// The field range (min fraction, current, max fraction) is NOT PUBLIC; all values should be in the interval [0, 1].
// It constrains the output of fraction_changed to the interval [min fraction, max fraction] and the start point of
// fraction_changed within this interval is current, that mean current must be min fraction < current < max fraction.

TimeSensor::Fields::Fields () :
	   cycleInterval (new SFTime (1)),
	           range ({ 0, 0, 1 }),
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
	               first (0),
	                last (1),
	              offset (0),
	               scale (1)
{
	addType (X3DConstants::TimeSensor);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "enabled",          enabled ());
	addField (inputOutput, "cycleInterval",    cycleInterval ());
	addField (inputOutput, "loop",             loop ());
	addField (inputOutput, "startTime",        startTime ());
	addField (inputOutput, "pauseTime",        pauseTime ());
	addField (inputOutput, "resumeTime",       resumeTime ());
	addField (inputOutput, "stopTime",         stopTime ());
	addField (outputOnly,  "isPaused",         isPaused ());
	addField (outputOnly,  "isActive",         isActive ());
	addField (outputOnly,  "cycleTime",        cycleTime ());
	addField (outputOnly,  "elapsedTime",      elapsedTime ());
	addField (outputOnly,  "fraction_changed", fraction_changed ());
	addField (outputOnly,  "time",             time ());
}

X3DBaseNode*
TimeSensor::create (X3DExecutionContext* const executionContext) const
{
	return new TimeSensor (executionContext);
}

void
TimeSensor::prepareEvents ()
{
	// The event order below is very important.

	if (getCurrentTime () - cycle >= interval)
	{
		if (loop ())
		{
			cycle              += interval * std::floor ((getCurrentTime () - cycle) / interval);
			fraction_changed () = last;
			elapsedTime ()      = getElapsedTime ();
			cycleTime ()        = getCurrentTime ();
		}
		else
		{
			fraction_changed () = last;
			stop ();
		}
	}
	else
	{
		time_type intpart;

		fraction_changed () = first + std::modf ((getCurrentTime () - cycle) / interval, &intpart) * scale;
		elapsedTime ()      = getElapsedTime ();
	}

	time () = getCurrentTime ();
}

void
TimeSensor::set_start ()
{
	first  = range () [0];
	last   = range () [2];
	scale  = last - first;
	offset = (range () [1] - first) * cycleInterval ();

	interval = cycleInterval () * scale;
	cycle    = getCurrentTime () - offset;

	fraction_changed () = range () [1];
	time ()             = getCurrentTime ();
}

void
TimeSensor::set_pause ()
{ }

void
TimeSensor::set_resume (const time_type pauseInterval)
{
	cycle += pauseInterval;
}

void
TimeSensor::set_stop ()
{ }

} // X3D
} // titania

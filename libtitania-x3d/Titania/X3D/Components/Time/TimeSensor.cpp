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
 ******************************************************************************/

#include "TimeSensor.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include <gdk/gdk.h>
#include <glibmm/main.h>

#include <iostream>

namespace titania {
namespace X3D {

TimeSensor::TimeSensor (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DTimeDependentNode (),                                                    
	       X3DSensorNode (),                                                    
	       cycleInterval (1),                                                   // SFTime  [in,out] cycleInterval     1        (0,∞)
	           cycleTime (),                                                    // SFTime  [out]    cycleTime
	    fraction_changed (),                                                    // SFFloat [out]    fraction_changed
	                time (),                                                    // SFTime  [out]    time
	                stop (),                                                    
	               cycle (),                                                    
	            interval (),                                                    
	        startTimeout (),                                                    
	         stopTimeout ()                                                     
{
	setComponent ("Time");
	setTypeName ("TimeSensor");

	addField (inputOutput, "metadata",         metadata);
	addField (inputOutput, "enabled",          enabled);
	addField (inputOutput, "cycleInterval",    cycleInterval);
	addField (inputOutput, "loop",             loop);
	addField (inputOutput, "startTime",        startTime);
	addField (inputOutput, "stopTime",         stopTime);
	addField (inputOutput, "pauseTime",        pauseTime);
	addField (inputOutput, "resumeTime",       resumeTime);
	addField (outputOnly,  "elapsedTime",      elapsedTime);
	addField (outputOnly,  "isPaused",         isPaused);
	addField (outputOnly,  "isActive",         isActive);
	addField (outputOnly,  "cycleTime",        cycleTime);
	addField (outputOnly,  "fraction_changed", fraction_changed);
	addField (outputOnly,  "time",             time);
}

X3DBaseNode*
TimeSensor::create (X3DExecutionContext* const executionContext) const
{
	return new TimeSensor (executionContext);
}

void
TimeSensor::initialize ()
{
	X3DTimeDependentNode::initialize ();

	setChildren (stop);

	enabled   .addInterest (this, &TimeSensor::set_enabled);
	startTime .addInterest (this, &TimeSensor::set_startTime);
	stopTime  .addInterest (this, &TimeSensor::set_stopTime);
	stop      .addInterest (this, &TimeSensor::set_stop);

	if (enabled)
	{
		if ((loop or startTime <= getCurrentTime ()) and stopTime <= startTime)
			set_start ();
	}
}

void
TimeSensor::set_enabled ()
{
	if (enabled)
	{
		if (loop and stopTime <= startTime)
			set_start ();
	}
	else
	{
		if (isActive)
		{
			isActive = false;
			getBrowser () -> removeSensor (this);
		}
	}
}

void
TimeSensor::set_startTime ()
{
	if (not enabled)
		return;

	if (getCurrentTime () >= startTime)
		set_start ();

	else
		addTimeout (startTimeout, &TimeSensor::do_start, startTime);
}

bool
TimeSensor::do_start ()
{
	if (enabled)
		set_start ();

	return false;
}

void
TimeSensor::set_start ()
{
	if (not isActive)
	{
		cycle    = getCurrentTime ();
		interval = cycleInterval;

		isActive         = true;
		cycleTime        = getCurrentTime ();
		fraction_changed = 0;
		time             = getCurrentTime ();

		getBrowser () -> addSensor (this);
	}
}

void
TimeSensor::set_stopTime ()
{
	if (not enabled)
		return;

	if (stopTime <= getCurrentTime ())
	{
		if (stopTime > startTime)
			if (isActive)
				set_stop ();

	}
	else
		addTimeout (stopTimeout, &TimeSensor::do_stop, stopTime);
}

bool
TimeSensor::do_stop ()
{
	if (enabled and isActive)
		set_stop ();

	return false;
}

void
TimeSensor::set_stop ()
{
	isActive = false;
	getBrowser () -> removeSensor (this);
}

void
TimeSensor::update ()
{
	//std::clog << "###### " << getCurrentTime () - cycleTime << " " << interval << std::endl;

	if (getCurrentTime () - cycle >= interval)
	{
		if (loop)
		{
			cycle           += interval;
			cycleTime        = getCurrentTime ();
			fraction_changed = 1;
		}
		else
		{
			fraction_changed = 1;
			stop             = getCurrentTime ();
		}
	}
	else
	{
		static time_type intpart;
		fraction_changed = std::modf ((getCurrentTime () - cycle) / interval, &intpart);
	}

	time = getCurrentTime ();
}

void
TimeSensor::addTimeout (sigc::connection & timeout, TimeoutHandler callback, const time_type time)
{
	if (timeout .connected ())
		timeout .disconnect ();

	timeout = Glib::signal_timeout () .connect (sigc::mem_fun (*this, callback),
	                                            time - getCurrentTime () * 1000,
	                                            GDK_PRIORITY_REDRAW);
}

void
TimeSensor::dispose ()
{
	startTimeout .disconnect ();

	if (isActive)
		getBrowser () -> removeSensor (this);

	X3DTimeDependentNode::dispose ();
}

} // X3D
} // titania

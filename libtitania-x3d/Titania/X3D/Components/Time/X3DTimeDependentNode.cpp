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

#include "X3DTimeDependentNode.h"

#include "../../Browser/X3DBrowser.h"

#include <gdk/gdk.h>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

X3DTimeDependentNode::Fields::Fields () :
	       loop (new SFBool ()),
	  startTime (new SFTime ()),
	  pauseTime (new SFTime ()),
	 resumeTime (new SFTime ()),
	   stopTime (new SFTime ()),
	   isPaused (new SFBool ()),
	  cycleTime (new SFTime ()),
	elapsedTime (new SFTime ())
{ }

X3DTimeDependentNode::X3DTimeDependentNode () :
	   X3DChildNode (),
	         fields (),
	    initialized (),
	 startTimeValue (0),
	 pauseTimeValue (0),
	resumeTimeValue (0),
	  stopTimeValue (0),
	          start (0),
	          pause (0),
	  pauseInterval (0),
	   startTimeout (),
	   pauseTimeout (),
	  resumeTimeout (),
	    stopTimeout (),
	       disabled (false)
{
	addNodeType (X3DConstants::X3DTimeDependentNode);

	addChildren (initialized);
}

void
X3DTimeDependentNode::initialize ()
{
	X3DChildNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (this, &X3DTimeDependentNode::set_live);

	initialized   .addInterest (this, &X3DTimeDependentNode::set_loop);
	enabled ()    .addInterest (this, &X3DTimeDependentNode::set_enabled);
	loop ()       .addInterest (this, &X3DTimeDependentNode::set_loop);
	startTime ()  .addInterest (this, &X3DTimeDependentNode::set_startTime);
	pauseTime ()  .addInterest (this, &X3DTimeDependentNode::set_pauseTime);
	resumeTime () .addInterest (this, &X3DTimeDependentNode::set_resumeTime);
	stopTime ()   .addInterest (this, &X3DTimeDependentNode::set_stopTime);

	startTimeValue  = startTime ();
	pauseTimeValue  = pauseTime ();
	resumeTimeValue = resumeTime ();
	stopTimeValue   = stopTime ();

	initialized = getCurrentTime ();
}

time_type
X3DTimeDependentNode::getElapsedTime () const
{
	return getCurrentTime () - start - pauseInterval;
}

// Event callbacks

void
X3DTimeDependentNode::set_live ()
{
	if (getExecutionContext () -> isLive () and isEnabled ())
	{
		if (disabled)
		{
			disabled = false;

			if (isActive () and not isPaused ())
				real_resume ();
		}
	}
	else
	{
		if (not disabled and isActive () and not isPaused ())
		{
			disabled = true;
			real_pause ();
		}
	}
}

void
X3DTimeDependentNode::set_enabled ()
{
	if (enabled ())
		set_loop ();

	else
		stop ();
}

void
X3DTimeDependentNode::set_loop ()
{
	if (enabled ())
	{
		if (loop ())
		{
			if (stopTimeValue <= startTimeValue)
			{
				if (startTimeValue <= getCurrentTime ())
					do_start ();
			}
		}
	}
}

void
X3DTimeDependentNode::set_startTime ()
{
	startTimeValue = startTime ();

	if (enabled ())
	{
		startTimeout .disconnect ();

		if (startTimeValue <= getCurrentTime ())
			do_start ();

		else
			addTimeout (startTimeout, &X3DTimeDependentNode::do_start, startTimeValue);
	}
}

void
X3DTimeDependentNode::set_pauseTime ()
{
	pauseTimeValue = pauseTime ();

	if (enabled ())
	{
		pauseTimeout .disconnect ();

		if (pauseTimeValue <= resumeTimeValue)
			return;

		if (pauseTimeValue <= getCurrentTime ())
			do_pause ();

		else
			addTimeout (pauseTimeout, &X3DTimeDependentNode::do_pause, pauseTimeValue);
	}
}

void
X3DTimeDependentNode::set_resumeTime ()
{
	resumeTimeValue = resumeTime ();

	if (enabled ())
	{
		resumeTimeout .disconnect ();

		if (resumeTimeValue <= pauseTimeValue)
			return;

		if (resumeTimeValue <= getCurrentTime ())
			do_resume ();

		else
			addTimeout (resumeTimeout, &X3DTimeDependentNode::do_resume, resumeTimeValue);
	}
}

void
X3DTimeDependentNode::set_stopTime ()
{
	stopTimeValue = stopTime ();

	if (enabled ())
	{
		stopTimeout .disconnect ();

		if (stopTimeValue <= startTimeValue)
			return;

		if (stopTimeValue <= getCurrentTime ())
			do_stop ();

		else
			addTimeout (stopTimeout, &X3DTimeDependentNode::do_stop, stopTimeValue);
	}
}

// Wrapper functions

void
X3DTimeDependentNode::do_start ()
{
	if (not isActive ())
	{
		start         = getCurrentTime ();
		pauseInterval = 0;

		// The event order below is very important.

		isActive () = true;

		set_start ();

		if (getExecutionContext () -> isLive () and isEnabled ())
		{
			getBrowser () -> prepareEvents () .addInterest (this, &X3DTimeDependentNode::prepareEvents);
		}
		else if (not disabled)
		{
			disabled = true;
			real_pause ();
		}

		elapsedTime () = 0;
		cycleTime ()   = getCurrentTime ();
	}
}

void
X3DTimeDependentNode::do_pause ()
{
	if (isActive () and not isPaused ())
	{
		isPaused () = true;

		if (pauseTimeValue not_eq getCurrentTime ())
			pauseTimeValue = getCurrentTime ();

		if (getExecutionContext () -> isLive () and isEnabled ())
			real_pause ();
	}
}

void
X3DTimeDependentNode::real_pause ()
{
	pause = getCurrentTime ();

	set_pause ();

	getBrowser () -> prepareEvents () .removeInterest (this, &X3DTimeDependentNode::prepareEvents);
}

void
X3DTimeDependentNode::do_resume ()
{
	if (isActive () and isPaused ())
	{
		isPaused () = false;

		if (resumeTimeValue not_eq getCurrentTime ())
			resumeTimeValue = getCurrentTime ();

		if (getExecutionContext () -> isLive () and isEnabled ())
			real_resume ();
	}
}

void
X3DTimeDependentNode::real_resume ()
{
	const time_type interval = getCurrentTime () - pause;

	pauseInterval += interval;

	set_resume (interval);

	getBrowser () -> prepareEvents () .addInterest (this, &X3DTimeDependentNode::prepareEvents);
}

void
X3DTimeDependentNode::do_stop ()
{
	stop ();
}

void
X3DTimeDependentNode::stop ()
{
	if (isActive ())
	{
		// The event order below is very important.

		set_stop ();

		elapsedTime () = getElapsedTime ();

		if (isPaused ())
			isPaused () = false;

		isActive () = false;

		if (getExecutionContext () -> isLive () and isEnabled ())
			getBrowser () -> prepareEvents () .removeInterest (this, &X3DTimeDependentNode::prepareEvents);
	}
}

// Timeout

bool
X3DTimeDependentNode::timeout (TimeoutHandler handler)
{
	if (enabled ())
	{
		getBrowser () -> getClock () -> advance ();

		(this ->* handler)();
	}

	return false;
}

void
X3DTimeDependentNode::addTimeout (sigc::connection & timeout, TimeoutHandler callback, const time_type time)
{
	timeout .disconnect ();

	timeout = Glib::signal_timeout () .connect (sigc::bind (sigc::mem_fun (*this, &X3DTimeDependentNode::timeout), callback),
	                                            (time - getCurrentTime ()) * 1000,
	                                            GDK_PRIORITY_REDRAW); // GDK_PRIORITY_REDRAW is very important, see Capone
}

void
X3DTimeDependentNode::removeTimeouts ()
{
	startTimeout  .disconnect ();
	pauseTimeout  .disconnect ();
	resumeTimeout .disconnect ();
	stopTimeout   .disconnect ();
}

void
X3DTimeDependentNode::beginUpdate ()
throw (Error <DISPOSED>)
{
	if (isEnabled ())
		return;
	
	X3DChildNode::beginUpdate ();

	set_live ();
}

void
X3DTimeDependentNode::endUpdate ()
throw (Error <DISPOSED>)
{
	if (not isEnabled ())
		return;

	X3DChildNode::endUpdate ();

	set_live ();
}

// Destruction

void
X3DTimeDependentNode::dispose ()
{
	removeTimeouts ();

	X3DChildNode::dispose ();
}

} // X3D
} // titania

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

#include "X3DTimeDependentNode.h"

#include "../../Browser/X3DBrowser.h"

#include <gdk/gdk.h>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

X3DTimeDependentNode::Fields::Fields () :
	loop (new SFBool ()),
	startTime (new SFTime ()),
	stopTime (new SFTime ()),
	pauseTime (new SFTime ()),
	resumeTime (new SFTime ()),
	isPaused (new SFBool ()),
	cycleTime (new SFTime ()),
	elapsedTime (new SFTime ())
{ }

X3DTimeDependentNode::X3DTimeDependentNode () :
	 X3DChildNode (),  
	       fields (),  
	        pause (0), 
	 startTimeout (),  
	  stopTimeout (),  
	 pauseTimeout (),  
	resumeTimeout ()   
{
	addNodeType (X3DConstants::X3DTimeDependentNode);
}

void
X3DTimeDependentNode::initialize ()
{
	X3DChildNode::initialize ();

	initialized .addInterest (this, &X3DTimeDependentNode::set_initialized);

	loop ()       .addInterest (this, &X3DTimeDependentNode::set_loop);
	startTime ()  .addInterest (this, &X3DTimeDependentNode::set_startTime);
	stopTime ()   .addInterest (this, &X3DTimeDependentNode::set_stopTime);
	pauseTime ()  .addInterest (this, &X3DTimeDependentNode::set_pauseTime);
	resumeTime () .addInterest (this, &X3DTimeDependentNode::set_resumeTime);
}

void
X3DTimeDependentNode::set_initialized ()
{
	set_loop ();
}

time_type
X3DTimeDependentNode::getElapsedTime () const
{
	return getCurrentTime () - startTime () - pause;
}

void
X3DTimeDependentNode::set_loop ()
{
	if (isEnabled ())
	{
		if (loop () and stopTime () <= startTime ())
			set_start ();
	}
}

void
X3DTimeDependentNode::set_startTime ()
{
	if (not isEnabled ())
		return;

	if (getCurrentTime () >= startTime ())
		set_start ();

	else
		addTimeout (startTimeout, &TimeSensor::do_start, startTime ());
}

bool
X3DTimeDependentNode::do_start ()
{
	if (isEnabled ())
		set_start ();

	return false;
}

void
X3DTimeDependentNode::set_stopTime ()
{
	if (not isEnabled ())
		return;

	if (stopTime () <= getCurrentTime ())
	{
		if (stopTime () > startTime ())
			set_stop ();
	}
	else
		addTimeout (stopTimeout, &TimeSensor::do_stop, stopTime ());
}

bool
X3DTimeDependentNode::do_stop ()
{
	if (isEnabled ())
		set_stop ();

	return false;
}

void
X3DTimeDependentNode::set_pauseTime ()
{
	if (not isEnabled ())
		return;
}

bool
X3DTimeDependentNode::do_pause ()
{
	if (isEnabled ())
		set_pause ();

	return false;
}

void
X3DTimeDependentNode::set_resumeTime ()
{
	if (not isEnabled ())
		return;
}

bool
X3DTimeDependentNode::do_resume ()
{
	if (isEnabled ())
		set_resume ();

	return false;
}

void
X3DTimeDependentNode::addTimeout (sigc::connection & timeout, TimeoutHandler callback, const time_type time)
{
	if (timeout .connected ())
		timeout .disconnect ();

	timeout = Glib::signal_timeout () .connect (sigc::mem_fun (*this, callback),
	                                            (time - getCurrentTime ()) * 1000,
	                                            GDK_PRIORITY_REDRAW);
}

void
X3DTimeDependentNode::dispose ()
{
	startTimeout  .disconnect ();
	stopTimeout   .disconnect ();
	pauseTimeout  .disconnect ();
	resumeTimeout .disconnect ();

	X3DChildNode::dispose ();
}

} // X3D
} // titania

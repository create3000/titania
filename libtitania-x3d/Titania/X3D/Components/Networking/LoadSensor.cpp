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

#include "LoadSensor.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

#include <glibmm/main.h>
#include <gdk/gdk.h>

namespace titania {
namespace X3D {

const std::string LoadSensor::componentName  = "Networking";
const std::string LoadSensor::typeName       = "LoadSensor";
const std::string LoadSensor::containerField = "children";

LoadSensor::Fields::Fields () :
	  timeOut (new SFTime ()),
	 isLoaded (new SFBool ()),
	 progress (new SFFloat ()),
	 loadTime (new SFTime ()),
	watchList (new MFNode ())
{ }

LoadSensor::LoadSensor (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DNetworkSensorNode (),
	              fields (),
	          urlObjects (),
	           startTime (0),
	              loaded (),
	            complete (),
	  timeOut_connection ()
{
	addType (X3DConstants::LoadSensor);

	addField (inputOutput, "metadata",  metadata ());
	addField (inputOutput, "enabled",   enabled ());
	addField (inputOutput, "timeOut",   timeOut ());
	addField (outputOnly,  "isActive",  isActive ());
	addField (outputOnly,  "isLoaded",  isLoaded ());
	addField (outputOnly,  "progress",  progress ());
	addField (outputOnly,  "loadTime",  loadTime ());
	addField (inputOutput, "watchList", watchList ());

	addChildren (urlObjects);
}

X3DBaseNode*
LoadSensor::create (X3DExecutionContext* const executionContext) const
{
	return new LoadSensor (executionContext);
}

void
LoadSensor::initialize ()
{
	X3DNetworkSensorNode::initialize ();

	enabled ()   .addInterest (this, &LoadSensor::set_enabled);
	timeOut ()   .addInterest (this, &LoadSensor::set_timeOut);
	watchList () .addInterest (this, &LoadSensor::set_watchList);

	watchList () .addEvent ();
}

void
LoadSensor::set_enabled ()
{
	if (enabled ())
		reset ();

	else
	{
		abort ();
		remove ();
	}
}

void
LoadSensor::set_timeOut ()
{
	if (isActive ())
	{
		timeOut_connection .disconnect ();

		if (timeOut () > 0)
		{
			time_type duration = timeOut () - (getCurrentTime () - startTime);

			timeOut_connection = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &LoadSensor::abort),
			                                                       duration * 1000,
			                                                       GDK_PRIORITY_REDRAW); // GDK_PRIORITY_REDRAW is very important
		}
	}
}

void
LoadSensor::set_watchList ()
{
	reset ();
}

void
LoadSensor::set_loadState (X3DUrlObject* const urlObject)
{
	if (isActive ())
	{
		switch (urlObject -> checkLoadState ())
		{
			case COMPLETE_STATE:
			{
				complete .emplace (urlObject);

				progress () = float (complete .size ()) / float (urlObjects .size ());

				if (complete .size () == urlObjects .size ())
				{
					timeOut_connection .disconnect ();

					isActive () = false;
					isLoaded () = true;
					loadTime () = getCurrentTime ();
				}

				break;
			}
			case FAILED_STATE:
			{
				abort ();
				break;
			}
			default:
				break;
		}
	}
	else
	{
		if (startTime)
		{
			switch (urlObject -> checkLoadState ())
			{
				case COMPLETE_STATE:
				case FAILED_STATE:
				{
					if (not loaded .count (urlObject))
						break;
					// Proceed with next step
				}
				case IN_PROGRESS_STATE:
				{
					reset ();
					break;
				}
				default:
					break;
			}
		}
		else
		{
			switch (urlObject -> checkLoadState ())
			{
				case COMPLETE_STATE:
				{
					start ();
					set_loadState (urlObject);
					break;
				}
				case FAILED_STATE:
				{
					start ();
					abort ();
					break;
				}
				case IN_PROGRESS_STATE:
				{
					start ();			
					break;
				}
				default:
					break;
			}
		}
	}

	switch (urlObject -> checkLoadState ())
	{
		case COMPLETE_STATE:
		case FAILED_STATE:
		{
			loaded .emplace (urlObject);
			break;
		}
		default:
			break;
	}
}

void
LoadSensor::start ()
{
	isActive () = true;
	progress () = 0;
	
	startTime = getCurrentTime ();
	set_timeOut ();
}

bool
LoadSensor::abort ()
{
	timeOut_connection .disconnect ();

	isActive () = false;
	
	if (enabled ())
		isLoaded () = false;

	return false;
}

void
LoadSensor::reset ()
{
	remove ();

	if (enabled ())
	{
		startTime = 0;
		loaded   .clear ();
		complete .clear ();
		
		for (const auto & node : watchList ())
		{
			const auto urlObject = x3d_cast <X3DUrlObject*> (node);

			if (urlObject)
			{
				urlObjects .emplace_back (urlObject);

				urlObject -> checkLoadState () .addInterest (this, &LoadSensor::set_loadState, urlObject);
			}
		}

		for (const auto & node : urlObjects)
		{
			const auto urlObject = x3d_cast <X3DUrlObject*> (node);

			set_loadState (urlObject);

			if (not isActive ())
				break;
		}
	}
}

void
LoadSensor::remove ()
{
	timeOut_connection .disconnect ();

	for (const auto & node : urlObjects)
	{
		const auto urlObject = x3d_cast <X3DUrlObject*> (node);

		urlObject -> checkLoadState () .removeInterest (this, &LoadSensor::set_loadState);
	}

	urlObjects .clear ();
}

void
LoadSensor::dispose ()
{
	timeOut_connection .disconnect ();

	X3DNetworkSensorNode::dispose ();
}

} // X3D
} // titania

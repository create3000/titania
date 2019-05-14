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

#include "X3DEventObject.h"

#include "../Browser/X3DBrowser.h"
#include "../Routing/Router.h"

#include <cassert>

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

// XXX: Rename this class to EventObject!

X3DEventObject::X3DEventObject (X3DBrowser* const browser) :
	       X3DChildObject (),
	              browser (browser), // must be atomic
	extendedEventHandling (true),
	             parentId ({ 0 }),
	               events (),
	          initialized (false)
{
	assert (browser);

	setTainted (true);
}

/***
 *  Initializes this object: after setup all fields can process events.
 */
void
X3DEventObject::setup ()
{
	X3DChildObject::setup ();

	browser -> addParent (this);

	initialized = true;
}

///  Set the browser this node belongs to.
void
X3DEventObject::setBrowser (X3DBrowser* const value)
{
	if (value == browser)
		return;

	if (initialized)
	{
		// Collect and remove events.

		std::vector <ChildrenList::value_type> objects;

		for (const auto & event : events)
		{
			if (browser -> getRouter () -> isValid (event))
				objects .emplace_back (*event .iter);
		}

		const bool tainted = browser -> getRouter () -> isValid (parentId);

		removeEvents ();

		// Replace browser.

		value -> addParent (this);
		browser -> removeParent (this);

		browser = value;

		// Add events.

		setTainted (false);

		for (const auto & object : objects)
			addEventObject (object .first, object .second);

		if (tainted)
			addEvent ();
	}
	else
	{
		browser = value;
	}
}

/***
 *  Adds @a object to the router event queue.
 */
void
X3DEventObject::addEventObject (X3DChildObject* const object)
{
	object -> isSet (true);

	if (object -> getTainted ())
		return;

	object -> setTainted (true);

	addEventObject (object, std::make_shared <Event> (object));
}

/***
 *  Adds @a object to the router event queue.
 */
void
X3DEventObject::addEventObject (X3DChildObject* const object, const EventPtr & event)
{
//try
//{
// 	__LOG__ << object << " : " << object -> getName () << " : " << object -> getTypeName () << " : " << getName () << " : " << getTypeName () << " : " << this << std::endl;
//}
//catch (...)
//{ }

	object -> isSet (true);
	browser -> addEvent ();

	// Register for processEvent

	events .emplace_back (browser -> getRouter () -> addTaintedChild (object, event));

	// Register for eventsProcessed

	if (not getTainted ())
	{
		if (object -> isInput () or (extendedEventHandling and not object -> isOutput ()))
		{
			setTainted (true);
		}
	}

	// Register always on every first event.

	if (not parentId .time)
		parentId = browser -> getRouter () -> addTaintedParent (this);
}

/***
 *  Marks this node as changed. Call this function if you want to inform the requesters of this object about a change.
 */
void
X3DEventObject::addEvent ()
{
	if (not getTainted ())
	{
		setTainted (true);

		if (not parentId .time)
			parentId = browser -> getRouter () -> addTaintedParent (this);

		browser -> addEvent ();
	}
}

/***
 *  The eventsProcessed () function represents the service request that is called after some set of events has been received.
 *  Requester of this object are now informed about a change of this object.
 */
void
X3DEventObject::eventsProcessed ()
{
	events .clear ();
	parentId .time = 0;

	if (getTainted ())
	{
		setTainted (false);
		processInterests ();
	}
}

/***
 *  Removes all outstanding events from the event queue of the router.
 */
void
X3DEventObject::removeEvents ()
{
	for (const auto & event : events)
		browser -> getRouter () -> removeTaintedChild (event);

	events .clear ();

	if (parentId .time)
	{
		browser -> getRouter () -> removeTaintedParent (parentId);
		parentId .time = 0;
	}
}

/***
 *  Disposes this object.  Note: it is normaly not needed to call this function.
 */
void
X3DEventObject::dispose ()
{
try
{
	X3DChildObject::dispose ();

	if (isInitialized ())
	{
		removeEvents ();

		browser -> removeParent (this);
	}
}
catch (const std::exception & error)
{
	__LOG__ << error .what () << std::endl;
}
}

/***
 *  Destructs this object.
 */
X3DEventObject::~X3DEventObject ()
{ }

} // X3D
} // titania

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

#include "Router.h"

#include <Titania/Chrono.h>

namespace titania {
namespace X3D {

Router::Router () :
	   events (),
	    nodes (),
	eventTime (chrono::now ()),
	 nodeTime (chrono::now ()),
	    mutex ()
{ }

EventId
Router::addEvent (X3DChildObject* const object, const EventPtr & event)
{
	events .emplace_back (object, event);

	return { eventTime, -- events .end () };
}

void
Router::removeEvent (const EventId & event)
{
	if (event .time == eventTime)
		events .erase (event .iter);
}

EventList
Router::getEvents ()
{
	// Invalidate all iterators

	eventTime = chrono::now ();

	return std::move (events);
}

NodeId
Router::addNode (X3DBaseNode* node)
{
	nodes .emplace_back (node);

	return { nodeTime, -- nodes .end () };
}

void
Router::removeNode (const NodeId & node)
{
	if (node .time == nodeTime)
		nodes .erase (node .iter);
}

NodeList
Router::getNodes ()
{
	// Invalidate all iterators

	nodeTime = chrono::now ();

	return std::move (nodes);
}

void
Router::processEvents ()
{
	do
	{
		do
		{
			for (auto & event : getEvents ())
			{
				event .first -> processEvent (event .second);
			}
		}
		while (not empty ());

		eventsProcessed ();
	}
	while (not empty ());
}

void
Router::eventsProcessed ()
{
	do
	{
		for (const auto & node : getNodes ())
			node -> processEvents ();
	}
	while (not nodes .empty () and empty ());
}

bool
Router::empty () const
{
	return events .empty ();
}

size_t
Router::size () const
{
	return events .size ();
}

void
Router::debug ()
{
	for (auto & event : events)
	{
		__LOG__ << event .first -> getName () << " : " << event .first -> getTypeName () << std::endl;

		for (const auto & parent : event .first -> getParents ())
		{
			auto node = dynamic_cast <X3DBaseNode*> (parent);

			if (node)
			{
				__LOG__ << "\t" << node -> getTypeName () << std::endl;
			}
		}
	}
}

} // X3D
} // titania

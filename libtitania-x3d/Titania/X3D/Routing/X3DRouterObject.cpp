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

#include "X3DRouterObject.h"

#include <Titania/Chrono/now.h>

#include <cassert>
#include <thread>

namespace titania {
namespace X3D {

X3DRouterObject::X3DRouterObject () :
	X3DBaseNode (),
	     events (),
	      nodes (),
	  eventTime (chrono::now ()),
	   nodeTime (chrono::now ())
{ }

EventId
X3DRouterObject::addTaintedField (X3DChildObject* const object, const EventPtr & event)
{
	addEvent ();

	events .emplace_back (object, event);

	return EventId { eventTime, -- events .end () };
}

void
X3DRouterObject::removeTaintedField (const EventId & event)
{
	if (event .time == eventTime)
		events .erase (event .iter);
}

EventList
X3DRouterObject::getTaintedFields ()
{
	// Invalidate all iterators

	eventTime = chrono::now ();

	return std::move (events);
}

NodeId
X3DRouterObject::addTaintedNode (X3DBaseNode* const node)
{
	addEvent ();

	nodes .emplace_back (node);

	return NodeId { nodeTime, -- nodes .end () };
}

void
X3DRouterObject::removeTaintedNode (const NodeId & node)
{
	if (node .time == nodeTime)
		nodes .erase (node .iter);
}

NodeList
X3DRouterObject::getTaintedNodes ()
{
	// Invalidate all iterators

	nodeTime = chrono::now ();

	return std::move (nodes);
}

void
X3DRouterObject::processEvents ()
{
	__LOG__ << std::string (120, '#') << std::endl;
	__LOG__ << std::string (120, '#') << std::endl;
	__LOG__ << std::string (120, '#') << std::endl;
	__LOG__ << std::string (120, '#') << std::endl;
	__LOG__ << std::string (120, '#') << std::endl;
	__LOG__ << std::string (120, '#') << std::endl;
	__LOG__ << std::string (120, '#') << std::endl;
	__LOG__ << std::string (120, '#') << std::endl;

	do
	{
		do
		{
			for (auto & event : getTaintedFields ())
			{
				__LOG__ << std::this_thread::get_id() << " : " << event .first << std::endl;
				event .first -> processEvent (event .second);
				__LOG__ << event .first << std::endl;
			}
		}
		while (not empty ());

		eventsProcessed ();
	}
	while (not empty ());
}

void
X3DRouterObject::eventsProcessed ()
{
	do
	{
		for (const auto & node : getTaintedNodes ())
			node -> eventsProcessed ();
	}
	while (not nodes .empty () and empty ());
}

bool
X3DRouterObject::empty () const
{
	return events .empty ();
}

void
X3DRouterObject::debugRouter () const
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

	assert (events .empty ());
}

} // X3D
} // titania

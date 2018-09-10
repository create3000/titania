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

#include "Router.h"

#include "../Basic/X3DBaseNode.h"
#include "../Fields/SFTime.h"

#include <cassert>

namespace titania {
namespace X3D {

Router::Router () :
	    children (),
	     parents (),
	childrenTime (SFTime::now ()),
	  parentTime (SFTime::now ())
{ }

ChildId
Router::addTaintedChild (X3DChildObject* const child, const EventPtr & event)
{
	children .emplace_back (child, event);

	return ChildId { childrenTime, -- children .end () };
}

void
Router::removeTaintedChild (const ChildId & childId)
{
	if (isValid (childId))
		children .erase (childId .iter);
}

bool
Router::isValid (const ChildId & childId) const
{
	return childId .time == childrenTime;
}

ChildrenList
Router::getTaintedChildren ()
{
	// Invalidate all iterators

	childrenTime = SFTime::now ();

	return std::move (children);
}

ParentId
Router::addTaintedParent (X3DParentObject* const parent)
{
	parents .emplace_back (parent);

	return ParentId { parentTime, -- parents .end () };
}

void
Router::removeTaintedParent (const ParentId & parentId)
{
	if (isValid (parentId))
		parents .erase (parentId .iter);
}

bool
Router::isValid (const ParentId & parentId) const
{
	return parentId .time == parentTime;
}

ParentList
Router::getTaintedParents ()
{
	// Invalidate all iterators

	parentTime = SFTime::now ();

	return std::move (parents);
}

void
Router::processEvents ()
{
	do
	{
		do
		{
			for (const auto & event : getTaintedChildren ())
			{
				try
				{
					event .first -> processEvent (event .second);
				}
				catch (const std::exception & error)
				{
					__LOG__ << "Unhandled exception in " << event .first -> getTypeName () << " named '" << event .first -> getName () << "'." << std::endl;
					__LOG__ << error .what () << std::endl;
				}
				catch (...)
				{
					__LOG__ << "Unhandled exception in " << event .first -> getTypeName () << " named '" << event .first -> getName () << "'." << std::endl;
				}
			}
		}
		while (not isEmpty ());

		eventsProcessed ();
	}
	while (not isEmpty ());
}

void
Router::eventsProcessed ()
{
	do
	{
		for (const auto & parent : getTaintedParents ())
			parent -> eventsProcessed ();
	}
	while (not parents .empty () and isEmpty ());
}

bool
Router::isEmpty () const
{
	return children .empty ();
}

size_t
Router::getSize () const
{
	return children .size ();
}

void
Router::debug () const
{
	for (auto & event : children)
	{
		const auto field = dynamic_cast <X3DFieldDefinition*> (event .first);

		if (field)
			__LOG__ << field -> getName () << " : " << field -> getTypeName () << std::endl;

		for (const auto & parent : event .first -> getParents ())
		{
			const auto node = dynamic_cast <X3DBaseNode*> (parent);

			if (node)
			{
				__LOG__ << "\t" << node -> getTypeName () << std::endl;
			}
		}
	}

	assert (isEmpty ());
}

Router::~Router ()
{ }

} // X3D
} // titania

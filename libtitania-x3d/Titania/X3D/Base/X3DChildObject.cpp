// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#include "X3DChildObject.h"

#include "../Fields/SFNode.h"

#include <algorithm>


namespace titania {
namespace X3D {

X3DChildObject::X3DChildObject () :
	X3DObject (),
	  parents (),
	     root (nullptr),
	  tainted (false)
{ }

// Object

void
X3DChildObject::notify ()
{
	for (const auto & parent : parents)
		parent -> notify ();
}

void
X3DChildObject::addEvent ()
{
	for (const auto & parent : parents)
		parent -> addEvent (this);
}

void
X3DChildObject::addEvent (X3DChildObject* const, const EventPtr & event)
{
	for (const auto & parent : parents)
		parent -> addEvent (this, event);
}

// Child

void
X3DChildObject::addParent (X3DChildObject* const parent)
{
	parents  .emplace (parent);
}

void
X3DChildObject::removeParent (X3DChildObject* const parent)
{
	if (parents .erase (parent))
	{
		if (root == parent)
			root = nullptr;

		if (parents .empty ())
		{
			dispose ();

			getGarbageCollector () .addObject (this);

			return;
		}

		ChildObjectSet circle;

		if (hasRoots (circle))
			return;

		for (auto & child : circle)
			child -> parents .clear ();

		for (auto & child : circle)
			child -> dispose ();

		for (auto & child : circle)
			getGarbageCollector () .addObject (child);
	}
}

bool
X3DChildObject::hasRoots (ChildObjectSet & seen)
{
	if (parents .empty ())
		return true;
		
	if (seen .emplace (this) .second)
	{
		// First test the good way
	
		if (root)
		{
			if (root -> hasRoots (seen))
				return true;
			else
				root = nullptr;
		}
		
		// Test all other ways and save the good way.

		for (auto & parent : parents)
		{
			if (parent -> hasRoots (seen))
			{
				root = parent;					
				return true;
			}
		}
	}

	return false;
}

// Object

void
X3DChildObject::dispose ()
{
	parents .clear ();
	root = nullptr;

	X3DObject::dispose ();
}

X3DChildObject::~X3DChildObject ()
{ }

} // X3D
} // titania

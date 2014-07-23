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
	if (root)
	{
		// Best guess for the shortest way to a rooted object.

		if (parent -> getParents () .size () < root -> getParents () .size ())
			root = parent;
	}
	else
		root = parent;

	if (parents .emplace (parent) .second)
		reference (parent);
}

void
X3DChildObject::replaceParent (X3DChildObject* const parentToRemove, X3DChildObject* const parentToAdd)
{
	if (root == parentToRemove)
		root = parentToAdd;

	if (parents .erase (parentToRemove))
	{
		unreference (parentToRemove);

		if (parents .emplace (parentToAdd) .second)
			reference (parentToAdd);
	}
	else
		addParent (parentToAdd);
}

void
X3DChildObject::removeParent (X3DChildObject* const parent)
{
	if (parents .erase (parent))
	{
		if (root == parent)
			root = nullptr;

		unreference (parent);

		if (getReferenceCount () == 0)
		{
			parents .clear ();

			dispose ();

			addDisposedObject (this);
			return;
		}

		ChildObjectSet circle;

		if (hasRootedObjects (circle))
			return;

		for (auto & child : circle)
		{
			//child -> referenceCount = 0;
			child -> parents .clear ();
		}

		for (auto & child : circle)
			child -> dispose ();

		addDisposedObjects (circle .begin (), circle .end ());
	}
}

void
X3DChildObject::addWeakParent (X3DChildObject* const parent)
{
	parents .emplace (parent);
}

void
X3DChildObject::removeWeakParent (X3DChildObject* const parent)
{
	parents .erase (parent);
}

bool
X3DChildObject::hasRootedObjects (ChildObjectSet & seen)
{
	if (parents .empty ())
		return true;

	if (seen .emplace (this) .second)
	{
		// First test the proved way.

		if (root)
		{
			if (root -> hasRootedObjects (seen))
				return true;
		}

		// Test all other ways and save the good way.

		for (auto & parent : parents)
		{
			if (parent -> hasRootedObjects (seen))
			{
				root = parent;
				return true;
			}
		}

		// We have no root and must test the next time again as parents can change.

		root = nullptr;
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

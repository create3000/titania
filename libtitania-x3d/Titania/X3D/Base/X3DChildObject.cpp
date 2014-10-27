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
	X3DGarbageCollector (),
	            parents (),
	               root (nullptr),
	            tainted (false)
{ }

// Event handling

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

// Children handling

void
X3DChildObject::addParent (X3DChildObject* const parent)
{
	// Determine the best guess for the shortest way to a rooted object.

	if ((not root)
	    or (parent -> getParents () .size () < root -> getParents () .size ()))
	{
		root = parent;
	}

	// Add parent

	if (parents .emplace (parent) .second)
		addReference ();
}

void
X3DChildObject::replaceParent (X3DChildObject* const parentToRemove, X3DChildObject* const parentToAdd)
{
	// Determine the best guess for the shortest way to a rooted object.

	if ((not root)
	    or (root == parentToRemove)
	    or (parentToAdd -> getParents () .size () < root -> getParents () .size ()))
	{
		root = parentToAdd;
	}

	// Replace parent

	if (parents .erase (parentToRemove))
	{
		if (parents .emplace (parentToAdd) .second)
			addReference ();

		removeReference ();
	}
	else
		addParent (parentToAdd);
}

/***
 *  This is the main function that resolves circular references.
 *
 *  If a parent is removed the function checks if the reference count is 0 and then the object will be disposed and
 *  garbage collected.  If the reference count is not 0 after parent remove, the function looks if the object has a
 *  rooted objects in the hierarchy.  If not, probably circular references are found and the object and the circular
 *  references will be disposed and garbage collected.
 */
void
X3DChildObject::removeParent (X3DChildObject* const parent)
{
	if (parents .erase (parent))
	{
		if (root == parent)
			root = nullptr;

		removeReference ();

		if (getReferenceCount () == 0)
		{
			unReference ();
			parents .clear ();

			processShutdown ();
			dispose ();

			addDisposedObject (this);
			return;
		}

		ChildObjectSet circle;

		if (hasRootedObjects (circle))
			return;

		// We have found circular references.

		for (const auto & child : circle)
		{
			child -> unReference ();
			child -> parents .clear ();
		}

		for (const auto & child : circle)
			child -> processShutdown ();

		for (const auto & child : circle)
			child -> dispose ();

		addDisposedObjects (circle .begin (), circle .end ());
	}
}

void
X3DChildObject::addWeakParent (X3DChildObject* const parent)
{
	// addWeakParent and removeWeakParent ensure that weak pointers can be found during traversal, for instance this is important
	// for X3DBaseNode::replace.

	// Add parent

	parents .emplace (parent);
}

void
X3DChildObject::removeWeakParent (X3DChildObject* const parent)
{
	if (root == parent)
		root = nullptr;

	parents .erase (parent);
}

/***
 *  This is the main function that determines if the object has a rooted object in the hierarchy of its parents.  An
 *  object is a root object if it has no parents.  If we haven't found rooted object then we have found circular
 *  references, that are returned in @a seen.
 */
bool
X3DChildObject::hasRootedObjects (ChildObjectSet & seen)
{
	if (parents .empty ())
		return true;

	if (seen .emplace (this) .second)
	{
		// First test way with the »best guess parent«, it is named here »root«.

		if (root)
		{
			if (root -> hasRootedObjects (seen))
				return true;
		}

		// Test all other ways and save the good way.

		for (const auto & parent : parents)
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

	X3DObject::dispose ();
}

X3DChildObject::~X3DChildObject ()
{ }

} // X3D
} // titania

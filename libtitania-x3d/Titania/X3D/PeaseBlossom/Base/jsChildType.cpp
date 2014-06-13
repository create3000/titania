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

#include "jsChildType.h"

namespace titania {
namespace pb {

jsChildType::jsChildType () :
	jsGarbageCollector (),
	    referenceCount (0),
	           parents (),
	              root (nullptr)
{ }

// Child

void
jsChildType::addParent (jsChildType* const parent)
{
	if (root)
	{
		if (parent -> getParents () .size () < root -> getParents () .size ())
			root = parent;
	}
	else
		root = parent;

	if (parents .emplace (parent) .second)
		++ referenceCount;
}

void
jsChildType::replaceParent (jsChildType* const parentToRemove, jsChildType* const parentToAdd)
{
	if (root == parentToRemove)
		root = parentToAdd;

	if (parents .erase (parentToRemove))
	{
		if (not parents .emplace (parentToAdd) .second)
			-- referenceCount;
	}
	else
		addParent (parentToAdd);
}

void
jsChildType::removeParent (jsChildType* const parent)
{
	if (parents .erase (parent))
	{
		if (root == parent)
			root = nullptr;

		-- referenceCount;

		if (referenceCount == 0)
		{
			parents .clear ();

			dispose ();

			addDisposedObject (this);

			return;
		}

		ChildObjectSet circle;

		if (hasRoots (circle))
			return;

		for (auto & child : circle)
		{
			child -> referenceCount = 0;
			child -> parents .clear ();
		}

		for (auto & child : circle)
			child -> dispose ();

		addDisposedObjects (circle .begin (), circle .end ());
	}
}

void
jsChildType::addWeakParent (jsChildType* const parent)
{
	parents .emplace (parent);
}

void
jsChildType::removeWeakParent (jsChildType* const parent)
{
	parents .erase (parent);
}

bool
jsChildType::hasRoots (ChildObjectSet & seen)
{
	if (parents .empty ())
		return true;

	if (seen .emplace (this) .second)
	{
		// First test the proved way.

		if (root)
		{
			if (root -> hasRoots (seen))
				return true;
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

		// We have no root and must test the next time again as parents can change.

		root = nullptr;
	}

	return false;
}

// Object

void
jsChildType::dispose ()
{
	parents .clear ();
	root = nullptr;
}

jsChildType::~jsChildType ()
{ }

} // pb
} // titania

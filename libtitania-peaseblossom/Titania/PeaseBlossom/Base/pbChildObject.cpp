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

#include "pbChildObject.h"

#include "../Base/GarbageCollector.h"

namespace titania {
namespace pb {

pbChildObject::pbChildObject () :
	            pbBase (),
	           parents (),
	              bestParent (nullptr),
	          children ()
{ }

void
pbChildObject::addChild (const pbChildObject & child)
{
	assert (&child not_eq this);

	children .emplace_back (const_cast <pbChildObject*> (&child));

	const_cast <pbChildObject &> (child) .addParent (this);
}

//void
//pbChildObject::removeChild (const pbChildObject & child)
//{
//	children .erase (const_cast <pbChildObject*> (&child));
//
//	const_cast <pbChildObject &> (child) .removeParent (this);
//}

std::list <pbChildObject*>::iterator
pbChildObject::addParent (pbChildObject* const parent)
{
	// Determine the best guess for the shortest way to a rooted object.

	if ((not bestParent)
	    or (parent -> getParents () .size () < bestParent -> getParents () .size ()))
	{
		bestParent = parent;
	}

	// Add parent

	parents .emplace_back (parent);
	
	return -- parents .end ();
}

const std::list <pbChildObject*>::iterator &
pbChildObject::replaceParent (const std::list <pbChildObject*>::iterator & parentToRemove, pbChildObject* const parentToAdd)
{
	// Determine the best guess for the shortest way to a rooted object.

	if ((not bestParent)
	    or (bestParent == *parentToRemove)
	    or (parentToAdd -> getParents () .size () < bestParent -> getParents () .size ()))
	{
		bestParent = parentToAdd;
	}

	// Replace parent
	
	*parentToRemove = parentToAdd;
	
	return parentToRemove;
}

void
pbChildObject::removeParent (const std::list <pbChildObject*>::iterator & iter)
{
	if (parents .empty ())
		return;

	const auto parent = *iter;

	parents .erase (iter);

	if (bestParent == parent)
		bestParent = nullptr;

	if (getReferenceCount () == 0)
	{
		parents .clear ();

		dispose ();

		//GarbageCollector::addDisposedObject (this);
		recycle ();
		return;
	}

	std::set <pbChildObject*> circle;

	if (hasRootedObjects (circle))
		return;

	// We have found circular references.

	for (const auto & child : circle)
		child -> parents .clear ();

	for (const auto & child : circle)
		child -> dispose ();

	//GarbageCollector::addDisposedObjects (circle .begin (), circle .end ());
	for (const auto & child : circle)
		child -> recycle ();
}

bool
pbChildObject::hasRootedObjects (std::set <pbChildObject*> & seen)
{
	if (parents .empty ())
		return true;

	if (seen .emplace (this) .second)
	{
		// First test the proved way.

		if (bestParent)
		{
			if (bestParent -> hasRootedObjects (seen))
				return true;
		}

		// Test all other ways and save the good way.

		for (auto & parent : parents)
		{
			if (parent -> hasRootedObjects (seen))
			{
				bestParent = parent;
				return true;
			}
		}

		// We have no root and must test the next time again as parents can change.

		bestParent = nullptr;
	}

	return false;
}

bool
pbChildObject::hasRootedObjectsDontCollectObject (std::set <pbChildObject*> & seen)
{
	if (getParents () .empty ())
		return true;

	for (auto & parent : getParents ())
	{
		if (parent -> hasRootedObjects (seen))
			return true;
	}

	return false;
}

// Object

void
pbChildObject::dispose ()
{
	//__LOG__ << this << " : " << getTypeName () << std::endl;

	parents .clear ();
	bestParent = nullptr;

	for (const auto & child : children)
		child -> dispose ();

	children .clear ();
}

void
pbChildObject::recycle ()
{
	GarbageCollector::addDisposedObject (this);
}

pbChildObject::~pbChildObject ()
{ }

} // pb
} // titania

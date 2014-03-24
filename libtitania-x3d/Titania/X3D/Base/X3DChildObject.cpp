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
	if (not root)
		root = parent;

	parents  .emplace (parent);
}

//void
//X3DChildObject::replaceParent (X3DChildObject* const parentToRemove, X3DChildObject* const parentToAdd)
//{
//	parents .erase (parentToRemove)
//	parents .emplace (parentToAdd);
//}

//	///  Move constructor.
//	X3DArrayField (X3DArrayField && field) :
//		X3DField <internal_type> (field .get ())
//	{
//		for (auto & value : get ())
//			value -> replaceParent (&field, this);
//
//		field .get () .clear ();
//		field .addEvent ();
//	}

//	X3DArrayField &
//	operator = (X3DArrayField && field)
//	{
//		if (&field == this)
//			return *this;
//
//		clear (); // addEvent ();
//
//		get () = field .get ();
//
//		for (auto & value : get ())
//			value -> replaceParent (&field, this);
//
//		field .get () .clear ();
//		field .addEvent ();
//
//		return *this;
//	}

//	X3DSFNode (X3DSFNode && field) :
//		X3DField <ValueType*> (field .getValue ())
//	{
//		if (getValue ())
//		{
//			getValue () -> replaceParent (&field, this);
//			field .set (nullptr);
//			field .addEvent ();
//		}
//	}

//template <class ValueType>
//template <class Up>
//inline
//X3DSFNode <ValueType> &
//X3DSFNode <ValueType>::operator = (X3DSFNode <Up> && field)
//{
// if (&field == this)
//    return;
//
// removeNode (getValue ());
//
//	X3DField <ValueType*>::set (dynamic_cast <ValueType*> (field .getValue ()));
//
//	if (getValue ())
//	{
//		getValue () -> replaceParent (&field, this);
//		field .set (nullptr);
//		field .addEvent ();
//	}
//
// addEvent ();
//
//	return *this;
//}

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

		getGarbageCollector () .addObjects (circle .begin (), circle .end ());
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

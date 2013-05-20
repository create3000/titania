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
	  tainted (false) 
{ }

// Object

void
X3DChildObject::notifyParents ()
{
	for (const auto & parent : getParents ())
		parent -> registerEvent (this);
}

void
X3DChildObject::registerEvent (X3DChildObject* const)
{
	notifyParents ();
}

void
X3DChildObject::registerEvent (X3DChildObject*, const Event & event)
{
	for (const auto & parent : getParents ())
		parent -> registerEvent (this, event);
}

// Child

bool
X3DChildObject::addParent (X3DChildObject* const parent)
{
	return parents .emplace (parent) .second;
}

static X3DChildObject* root = NULL;

bool
X3DChildObject::removeParent (X3DChildObject* const parent)
{
	if (parents .erase (parent))
	{
		if (parents .size ())
		{
			ChildObjectSet circle;
			
			root = NULL;

			if (hasRoots (circle))
			{
				if (root)
				{
					//__LOG__ << "############################################ " << this -> getTypeName () << std::endl;
					//__LOG__ << "############################################   " << root -> getTypeName () << " : " << root -> getParents () .size () << std::endl;
				}
				
				return false;
			}

			for (auto & child : circle)
				child -> parents .clear ();

			for (auto & child : circle)
			{
				child -> dispose ();
				
				getGarbageCollector () .addObject (child);
			}

			return true;
		}

		dispose ();
		
		getGarbageCollector () .addObject (this);

		return true;
	}

	return false;
}

const ChildObjectSet &
X3DChildObject::getParents () const
{
	return parents;
}

bool
X3DChildObject::hasRoots (ChildObjectSet & seen)
{
	if (getParents () .size ())
	{
		if (seen .emplace (this) .second)
		{
			for (auto & parent : getParents ())
			{
				if (parent -> hasRoots (seen))
					return true;
			}

			return false;
		}

		return false;
	}
	
	root = this;

	// this is a root node
	return true;
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

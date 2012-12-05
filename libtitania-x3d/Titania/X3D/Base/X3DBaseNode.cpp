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

#include "X3DBaseNode.h"

#include <algorithm>

namespace titania {
namespace X3D {

X3DBaseNode::X3DBaseNode () :
	X3DObject (), 
	  parents (), 
	 children ()  
{ }

// Object

void
X3DBaseNode::notify (X3DObject* const)
{
	notifyParents ();
}

void
X3DBaseNode::notifyParents ()
{
	for (const auto & parent : getParents ())
		parent -> notify (this);
}

// Child

bool
X3DBaseNode::addParent (X3DBaseNode* const parent)
{
	return parents .insert (parent) .second;
}

bool
X3DBaseNode::removeParent (X3DBaseNode* const parent)
{
	if (parents .erase (parent))
	{
		if (parents .size ())
		{
			BaseNodeSet circle;

			if (hasRoots (circle))
				return false;

			for (auto & child : circle)
				child -> parents .clear ();

			for (auto & child : circle)
				child -> dispose ();

			return false;
		}

		dispose ();

		return false;
	}

	return false;
}

const BaseNodeSet &
X3DBaseNode::getParents () const
{
	return parents;
}

bool
X3DBaseNode::hasRoots (BaseNodeSet & seen)
{
	if (getParents () .size ())
	{
		if (seen .insert (this) .second)
		{
			for (auto & parent : getParents ())
				if (parent -> hasRoots (seen))
					return true;

			return false;

			return std::any_of (getParents () .cbegin (),
			                    getParents () .cend (),
			                    std::bind (std::mem_fn (&X3DBaseNode::hasRoots),
			                               std::placeholders::_1,
			                               std::ref (seen)));
		}

		return false;
	}

	// this is a root node
	return true;
}

// Node:
void
X3DBaseNode::setChild (X3DBaseNode* const child)
{
	child -> addParent (this);
	children .insert (child);
}

void
X3DBaseNode::setChild (X3DBaseNode & child)
{
	child .addParent (this);
	children .insert (&child);
}

const BaseNodeSet &
X3DBaseNode::getChildren () const
{
	return children;
}

// Object

void
X3DBaseNode::dispose ()
{
	parents .clear ();

	for (auto & child : children)
		child -> dispose ();

	children .clear ();

	X3DObject::dispose ();
}

X3DBaseNode::~X3DBaseNode ()
{ }

} // X3D
} // titania

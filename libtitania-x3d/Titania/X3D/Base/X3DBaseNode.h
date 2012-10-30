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
 ******************************************************************************/

#ifndef __TITANIA_X3D_BASE_X3DBASE_NODE_H__
#define __TITANIA_X3D_BASE_X3DBASE_NODE_H__

#include "../Base/BaseNodeSet.h"
#include "../Base/X3DObject.h"

#include <Titania/Bits/Utility/Pass.h>

namespace titania {
namespace X3D {

class X3DBaseNode :
	virtual public X3DObject
{
public:

	//@{
	bool
	addParent (X3DBaseNode* const);

	bool
	removeParent (X3DBaseNode* const);

	const BaseNodeSet &
	getParents () const;

	virtual
	bool
	hasRoots (BaseNodeSet &);

	template <class Type>
	std::deque <Type*>
	findClosestParents () const;
	//@}

	//@{
	template <typename ... Args>
	void
	setChildren (Args & ...);

	void
	setChild (X3DBaseNode &);

	template <typename ... Args>
	void
	addChildren (Args & ...);

	void
	addChild (X3DBaseNode &);

	const BaseNodeSet &
	getChildren () const;
	//@}

	// Object:
	virtual
	void
	dispose ();


protected:

	X3DBaseNode ();

	// Object:
	//@{
	virtual
	void
	notify (X3DObject* const);

	virtual
	void
	notifyParents ();
	//@}

	virtual
	~X3DBaseNode ();


private:

	template <class Type>
	void
	findClosestParents (std::deque <Type*> &, BaseNodeSet &);

	BaseNodeSet parents;
	BaseNodeSet children;

};

template <typename ... Args>
inline
void
X3DBaseNode::setChildren (Args & ... args)
{
	basic::pass ((setChild (args), 1) ...);
}

template <typename ... Args>
inline
void
X3DBaseNode::addChildren (Args & ... args)
{
	basic::pass ((addChild (args), 1) ...);
}

inline
void
X3DBaseNode::addChild (X3DBaseNode & child)
{
	child .addParent (this);
}

template <class Type>
std::deque <Type*>
X3DBaseNode::findClosestParents () const
{
	BaseNodeSet seen;

	std::deque <Type*> parents;

	for (const auto & object : getParents ())
		object -> findClosestParents (parents, seen);

	return parents;
}

template <class Type>
void
X3DBaseNode::findClosestParents (std::deque <Type*> & parents, BaseNodeSet & seen)
{
	if (not seen .insert (this) .second)
		return;

	Type* parent = dynamic_cast <Type*> (this);

	if (parent)
	{
		parents .push_back (parent);
		return;
	}

	for (const auto & object : getParents ())
		object -> findClosestParents (parents, seen);
}

} // X3D
} // titania

#endif

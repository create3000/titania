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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_BASE_X3DCHILD_OBJECT_H__
#define __TITANIA_X3D_BASE_X3DCHILD_OBJECT_H__

#include "../Base/ChildObjectSet.h"
#include "../Base/Event.h"
#include "../Base/X3DObject.h"

#include <Titania/Utility/Pass.h>
#include <deque>

namespace titania {
namespace X3D {

class X3DChildObject :
	public X3DObject
{
public:

	///  @name Parent handling

	void
	addParent (X3DChildObject* const);

	void
	removeParent (X3DChildObject* const);

	const ChildObjectSet &
	getParents () const
	{ return parents; }

	virtual
	bool
	hasRoots (ChildObjectSet &);

	template <class Root, class Type>
	std::deque <Type*>
	findParents () const;

	///  @name Event Handling

	void
	isTainted (bool value)
	{ tainted = value; }

	bool
	isTainted ()
	{ return tainted; }

	virtual
	void
	write (const X3DChildObject &)
	{ }

	void
	addEvent ();

	// Only used in X3DFieldDefinition.
	virtual
	void
	processEvent (Event &)
	{ }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DChildObject ();


protected:

	X3DChildObject ();

	///  @name Children handling

	template <typename ... Args>
	void
	addChildren (Args & ... args)
	{ basic::pass ((addChild (args), 1) ...); }

	void
	addChild (X3DChildObject & child)
	{ child .addParent (this); }

	template <typename ... Args>
	void
	removeChildren (Args & ... args)
	{ basic::pass ((removeChild (args), 1) ...); }

	void
	removeChild (X3DChildObject & child)
	{ child .removeParent (this); }

	///  @name Event handling

	virtual
	void
	addEvent (X3DChildObject* const)
	{ addEvent (); }

	virtual
	void
	addEvent (X3DChildObject* const, const Event &);


private:

	typedef std::vector <X3DChildObject*> ChildObjectArray;

	template <class Root, class Type>
	void
	findParents (std::deque <Type*> &, ChildObjectSet &);

	ChildObjectSet  parents;
	X3DChildObject* root;
	bool            tainted;

};

template <class Root, class Type>
std::deque <Type*>
X3DChildObject::findParents () const
{
	std::deque <Type*> parents;
	ChildObjectSet     seen;

	for (const auto & object : getParents ())
		object -> findParents <Root, Type> (parents, seen);

	return parents;
}

template <class Root, class Type>
void
X3DChildObject::findParents (std::deque <Type*> & parents, ChildObjectSet & seen)
{
	if (dynamic_cast <Root*> (this))
		return;

	if (not seen .insert (this) .second)
		return;

	Type* parent = dynamic_cast <Type*> (this);

	if (parent)
	{
		parents .push_back (parent);
		return;
	}

	for (const auto & object : getParents ())
		object -> findParents <Root, Type> (parents, seen);
}

} // X3D
} // titania

#endif

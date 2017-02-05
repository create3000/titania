/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_X3D_EXECUTION_BINDABLE_NODE_LIST_H__
#define __TITANIA_X3D_EXECUTION_BINDABLE_NODE_LIST_H__

#include "../Basic/X3DBaseNode.h"

#include <vector>

namespace titania {
namespace X3D {

template <class Type>
class X3DBindableNodeList :
	virtual public X3DBaseNode
{
public:

	typedef Type*                              pointer_type;
	typedef std::vector <pointer_type>         list_type;
	typedef typename list_type::const_iterator const_iterator;
	typedef typename list_type::size_type      size_type;

	/// @name Constructors

	///  Default constructor.
	X3DBindableNodeList (X3DExecutionContext* const executionContext) :
		X3DBaseNode (executionContext -> getBrowser (), executionContext),
		       list ()
	{ }

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override
	{ return new X3DBindableNodeList (executionContext); }

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	/// @name Iterators

	const_iterator
	begin () const
	{ return list .cbegin (); }

	const_iterator
	cbegin () const
	{ return list .cbegin (); }

	const_iterator
	end () const
	{ return list .cend (); }

	const_iterator
	cend () const
	{ return list .cend (); }

	///  @name Member access

	const pointer_type &
	operator [ ] (const size_type & index) const
	{ return list [index]; }

	const pointer_type &
	at (const size_type & index) const
	throw (std::out_of_range)
	{ return list .at (index); }

	/// @name Capacity

	bool
	empty () const
	{ return list .empty (); }

	size_type
	size () const
	{ return list .size (); }

	size_type
	max_size () const
	{ return list .max_size (); }

	/// @name Modifiers

	const pointer_type &
	bound () const
	{
	   if (not list .empty ())
	   {
			for (const auto & node : std::make_pair (list .begin () + 1, list .end ()))
			{
				if (node -> isBound ())
					return node;
			}
		}

		try
		{
			return at (1);
		}
		catch (const std::out_of_range &)
		{
			return at (0);
		}
	}

	void
	push_back (const pointer_type & node)
	{ temp .emplace_back (node); }

	void
	update ()
	{
		if (temp not_eq list)
		{
			for (auto & node : list)
				node -> shutdown () .removeInterest (&X3DBindableNodeList::erase, this);

			replace (temp);

			list = std::move (temp);

			for (auto & node : list)
				node -> shutdown () .addInterest (&X3DBindableNodeList::erase, this, node);

			addEvent ();
		}
		else
			temp .clear ();
	}

	void
	erase (const pointer_type & node)
	{
		auto end = std::remove (list .begin (), list .end (), node);

		if (end not_eq list .end ())
		{
			list .erase (end, list .end ());
			addEvent ();
		}
	}


private:

	///  @name Operations
	
	void
	replace (const list_type &)
	{ }

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	list_type temp;
	list_type list;

};

template <class Type>
const ComponentType X3DBindableNodeList <Type>::component = ComponentType::TITANIA;

template <class Type>
const std::string X3DBindableNodeList <Type>::typeName = "X3DBindableNodeList";

template <class Type>
const std::string   X3DBindableNodeList <Type>::containerField = "bindableNodeList";

class X3DViewpointNode;

template <>
void
X3DBindableNodeList <X3DViewpointNode>::replace (const list_type &);

} // X3D
} // titania

#endif

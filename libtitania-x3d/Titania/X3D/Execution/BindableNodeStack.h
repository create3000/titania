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

#ifndef __TITANIA_X3D_EXECUTION_BINDABLE_NODE_STACK_H__
#define __TITANIA_X3D_EXECUTION_BINDABLE_NODE_STACK_H__

#include "../Basic/X3DBaseNode.h"

#include <Titania/Basic/BindStack.h>
#include <initializer_list>

namespace titania {
namespace X3D {

template <class Type>
class X3DBindableNodeStack :
	virtual public X3DBaseNode
{
public:

	///  @name Member types

	using pointer_type = Type*;
	using stack_type   = basic::bind_stack <pointer_type>;
	using size_type    = typename stack_type::size_type;

	///  @name Construction

	X3DBindableNodeStack (X3DExecutionContext* const executionContext, const pointer_type & node) :
		X3DBaseNode (executionContext -> getBrowser (), executionContext),
		      stack ({ node })
	{
		node -> shutdown () .addInterest (&X3DBindableNodeStack::erase, this, node);
	}

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override
	{ return new X3DBindableNodeStack (executionContext, getBottom ()); }

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Member access

	const pointer_type &
	getTop () const
	{ return stack .top (); }

	const pointer_type &
	getBottom () const
	{ return stack .bottom (); }

	size_type
	getSize () const
	{ return stack .size (); }

	void
	pushOnTop (const pointer_type & node, const bool force = false)
	{
		if (force)
		{
			node -> isBound ()  = true;
			node -> bindTime () = getCurrentTime ();
		}

		if (stack .empty ())
			return;

		if (stack .top () not_eq node)
		{
			if (stack .top () -> isBound ())
			{
				stack .top () -> set_bind () = false;
				stack .top () -> isBound ()  = false;
			}

			if (not node -> isBound ())
			{
				node -> isBound ()  = true;
				node -> bindTime () = getCurrentTime ();
				node -> transitionStart (stack .top ());
			}

			if (stack .push (node))
				node -> shutdown () .addInterest (&X3DBindableNodeStack::erase, this, node);

			addEvent ();
		}
	}

	bool
	pop (const pointer_type & node)
	{
		if (stack .empty ())
			return false;

		if (stack .top () == node)
		{
			if (node -> isBound ())
				node -> isBound () = false;

			node -> shutdown () .removeInterest (&X3DBindableNodeStack::erase, this);

			stack .pop ();

			if (stack .empty ())
				return true;

			if (not stack .top () -> isBound ())
			{
				stack .top () -> set_bind () = true;
				stack .top () -> isBound ()  = true;
				stack .top () -> bindTime () = getCurrentTime ();
				stack .top () -> transitionStart (node);
			}

			addEvent ();

			return true;
		}

		return false;
	}

	void
	erase (const pointer_type & node)
	{
		if (not pop (node))
			stack .erase (node);
	}


private:

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	stack_type stack;

};

template <class Type>
const Component X3DBindableNodeStack <Type>::component = Component ("Titania", 1);

template <class Type>
const std::string X3DBindableNodeStack <Type>::typeName = "X3DBindableNodeList";

template <class Type>
const std::string   X3DBindableNodeStack <Type>::containerField = "bindableNodeList";

} // X3D

} // titania

#endif

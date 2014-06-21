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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_VS_BLOCK_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_VS_BLOCK_H__

#include "../Base/vsChildObject.h"
#include "../Primitives/Undefined.h"
#include "../Primitives/array.h"

namespace titania {
namespace pb {

class vsBlock :
	virtual public vsChildObject,
	virtual public vsOutputStreamObject
{
public:

	/// @name Member access

	///  Returns an array with all local root expressions.
	array &
	getExpressions ()
	{ return expressions; }

	///  Returns an array with all local root expressions.
	const array &
	getExpressions () const
	{ return expressions; }

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () override
	{
		expressions .clear ();

		vsChildObject::dispose ();
	}

protected:

	///  @name Construction

	///  Constructs new vsBlock.
	vsBlock () :
		       vsChildObject (),
		vsOutputStreamObject (),
		         expressions (),
		           undefined (new Undefined ())
	{ construct (); }

	/// @name Operations

	///  Imports all expressions from @a block into @a executionContext.
	void
	import (const vsBlock* const block, vsExecutionContext* const executionContext)
	{
		for (const auto & expression : block -> getExpressions ())
			getExpressions () .emplace_back (expression -> copy (executionContext));
	}

	///  Executes the associated expessions of this context.
	virtual
	var
	run ()
	{
		for (const auto & expression : expressions)
			expression -> toValue ();

		return undefined;
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (expressions, undefined); }

	/// @name Members

	array expressions; // Use deque to keep iters when inserting value.
	var   undefined;

};

} // pb
} // titania

#endif

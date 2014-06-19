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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_ADDITION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_ADDITION_H__

#include "../Expressions/vsExpression.h"
#include "../Primitives/Number.h"
#include "../Primitives/String.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript addition expression.
 */
class Addition :
	public vsExpression
{
public:

	///  @name Construction

	///  Constructs new Addition expression.
	Addition (var && lhs, var && rhs) :
		vsExpression (),
		         lhs (std::move (lhs)),
		         rhs (std::move (rhs))
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	var
	copy (vsExecutionContext* const executionContext) const final override
	{ return make_var <Addition> (lhs -> copy (executionContext), rhs -> copy (executionContext)); }

	///  @name Common members

	///  Returns the type of the value. For this expression this is »ADDITION«.
	virtual
	ValueType
	getType () const final override
	{ return ADDITION; }

	///  @name Operations

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	toValue () const final override
	{ return var (evaluate (lhs, rhs)); }

	static
	vsValue*
	evaluate (const var & lhs, const var & rhs)
	{
		if (lhs -> getType () == STRING or rhs -> getType () == STRING)
			return new String (lhs -> toString () + rhs -> toString ());

		return new Number (lhs -> toNumber () + rhs -> toNumber ());
	}


private:

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (lhs, rhs); }

	///  @name Members

	const var lhs;
	const var rhs;

};

///  @relates Addition
///  @name Construction

///  Constructs new Addition expression.
inline
vsValue*
createAddition (var && lhs, var && rhs)
{
	if (lhs -> isPrimitive () and rhs -> isPrimitive ())
		return Addition::evaluate (lhs, rhs);

	return new Addition (std::move (lhs), std::move (rhs));
}

} // pb
} // titania

#endif

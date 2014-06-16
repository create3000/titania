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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_LEFT_SHIFT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_LEFT_SHIFT_H__

#include "../Expressions/jsExpression.h"
#include "../Primitives/Number.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a JavaScript division expression.
 */
class LeftShift :
	public jsExpression
{
public:

	///  @name Member access

	///  Returns the type of the value. For expressions this is »DIVISION«.
	virtual
	ValueType
	getType () const final override
	{ return LEFT_SHIFT; }

	///  @name Operations

	///  Converts its input argument to either Primitive or Object type.
	virtual
	bool
	isPrimitive () const final override
	{ return lhs -> isPrimitive () and rhs -> isPrimitive (); }

	///  Converts its argument to a value of type Boolean.
	virtual
	bool
	toBoolean () const final override
	{ return toInt32 (); }

	///  Converts its argument to an integral unsigned value of 16 bit.
	virtual
	uint16_t
	toUInt16 () const final override
	{ return toInt32 (); }

	///  Converts its argument to an integral signed value of 32 bit.
	virtual
	int32_t
	toInt32 () const final override
	{ return lhs -> toInt32 () << (rhs -> toUInt32 () & 0x1f); }

	///  Converts its argument to an integral unsigned value of 32 bit.
	virtual
	uint32_t
	toUInt32 () const final override
	{ return toInt32 (); }

	///  Converts its arguments to a value of type Number.
	virtual
	double
	toNumber () const final override
	{ return toInt32 (); }

	///  Converts its input argument to a non-Object type.
	virtual
	var
	toValue () const final override
	{ return make_var <Number> (toInt32 ()); }


protected:

	///  @name Friends

	friend
	var
	make_left_shift (var &&, var &&);

	///  @name Construction

	LeftShift (var && lhs, var && rhs) :
		jsExpression (),
		         lhs (std::move (lhs)),
		         rhs (std::move (rhs))
	{ addChildren (this -> lhs, this -> rhs); }


private:

	///  @name Members

	const var lhs;
	const var rhs;

};

///  @relates Division
///  @name division.

inline
var
make_left_shift (var && lhs, var && rhs)
{
	const var expression (new LeftShift (std::move (lhs), std::move (rhs)));

	if (expression -> isPrimitive ())
		return expression -> toPrimitive ();

	return expression;
}

} // pb
} // titania

#endif

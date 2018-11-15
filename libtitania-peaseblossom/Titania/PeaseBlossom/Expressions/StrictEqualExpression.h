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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_STRICT_EQUAL_EXPRESSION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_STRICT_EQUAL_EXPRESSION_H__

#include "../Expressions/pbStatement.h"
#include "../Expressions/PrimitiveExpression.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript strict equal expression.
 */
class StrictEqualExpression :
	public pbStatement
{
public:

	///  @name Construction

	///  Constructs new StrictEqualExpression expression.
	StrictEqualExpression (ptr <pbStatement> && lhs, ptr <pbStatement> && rhs) :
		pbStatement (StatementType::STRICT_EQUAL_EXPRESSION),
		         lhs (std::move (lhs)),
		         rhs (std::move (rhs))
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbStatement>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{ return new StrictEqualExpression (lhs -> copy (executionContext), rhs -> copy (executionContext)); }

	///  @name Operations

	///  Converts its argument to a value of type Boolean.
	///  throws pbError
	virtual
	CompletionType
	getValue () const final override
	{
		return evaluate (lhs, rhs);
	}

	///  Performs strict equal comparision to its arguments
	///  throws pbError
	static
	bool
	evaluate (const ptr <pbStatement> & lhs, const ptr <pbStatement> & rhs)
	{
		const auto x = lhs -> getValue ();
		const auto y = rhs -> getValue ();

		return evaluate (x, y);
	}

	/// Performs strict equal comparision to its arguments
	static
	bool
	evaluate (const var & x, const var & y)
	{
		if (x .getType () not_eq y .getType ())
			return false;

		switch (x .getType ())
		{
			case UNDEFINED:
				return true;

			case NULL_OBJECT:
				return true;

			case NUMBER:
				return x .getNumber () == y .getNumber ();

			case STRING:
				return x .getString () == y .getString ();

			case BOOLEAN:
				return x .getBoolean () == y .getBoolean ();

			case OBJECT:
				return x .getObject () == y .getObject ();
		}

		return false;
	}

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		ostream
			<< pb::toStream (this, lhs)
			<< Generator::TidySpace
			<< "==="
			<< Generator::TidySpace
			<< pb::toStream (this, rhs);
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (lhs, rhs); }

	///  @name Members

	const ptr <pbStatement> lhs;
	const ptr <pbStatement> rhs;

};

///  @relates StrictEqualExpression
///  @name Construction

///  Constructs new StrictEqualExpression expression.
inline
ptr <pbStatement>
createStrictEqualExpression (ptr <pbStatement> && lhs, ptr <pbStatement> && rhs)
{
	if (lhs -> isPrimitive () and rhs -> isPrimitive ())
		return new PrimitiveExpression (StrictEqualExpression (std::move (lhs), std::move (rhs)) .getValue (), StatementType::BOOLEAN);

	return new StrictEqualExpression (std::move (lhs), std::move (rhs));
}

} // pb
} // titania

#endif

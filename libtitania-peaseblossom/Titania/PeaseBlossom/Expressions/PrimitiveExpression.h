/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_PRIMITIVE_EXPRESSION_H__
#define __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_PRIMITIVE_EXPRESSION_H__

#include "../Expressions/PrimitiveExpressionType.h"
#include "../Expressions/pbExpression.h"
#include "../Primitives/var.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript primitive expression.
 */
class PrimitiveExpression :
	public pbExpression
{
public:

	///  @name Construction

	///  Constructs new PrimitiveExpression expression.
	PrimitiveExpression (const var & value, const PrimitiveExpressionType type) :
		pbExpression (ExpressionType::PRIMITIVE_EXPRESSION),
		       value (value),
		        type (type)
	{ }

	///  Constructs new PrimitiveExpression expression.
	PrimitiveExpression (var && value, const PrimitiveExpressionType type) :
		pbExpression (ExpressionType::PRIMITIVE_EXPRESSION),
		       value (std::move (value)),
		        type (type)
	{ }

	///  Creates a copy of this object.
	virtual
	ptr <pbExpression>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{ return new PrimitiveExpression (value, type); }

	///  @name Operations
	
	PrimitiveExpressionType
	getPrimitiveType () const
	{ return type; }

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	getValue () const
	throw (pbException,
	       pbControlFlowException) final override
	{ return value; }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		switch (type)
		{
			case PrimitiveExpressionType::NUMBER:
				ostream << std::dec << value;
				break;

			case PrimitiveExpressionType::BINARY_NUMBER:
				ostream << std::hex << value;
				break;

			case PrimitiveExpressionType::OCTAL_NUMBER:
				ostream << std::oct << value;
				break;

			case PrimitiveExpressionType::HEXAL_NUMBER:
				ostream << std::hex << value;
				break;

			case PrimitiveExpressionType::SINGLE_QUOTED_STRING:
				ostream << '\'' << value << '\'';
				break;

			case PrimitiveExpressionType::DOUBLE_QUOTED_STRING:
				ostream << '"' << value << '"';
				break;

			default:
				ostream << value;
				break;
		}
	}

private:

	///  @name Members

	const var                     value;
	const PrimitiveExpressionType type;

};

} // pb
} // titania

#endif

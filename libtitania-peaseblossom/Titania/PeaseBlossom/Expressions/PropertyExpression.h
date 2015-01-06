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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_PROPERTY_EXPRESSION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_PROPERTY_EXPRESSION_H__

#include "../Expressions/pbExpression.h"
#include "../Objects/pbObject.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript addition expression.
 */
class PropertyExpression :
	public pbExpression
{
public:

	///  @name Construction

	///  Constructs new AdditionExpression expression.
	PropertyExpression (ptr <pbExpression> && expression, std::string && identifier) :
		pbExpression (ExpressionType::PROPERTY_EXPRESSION),
		  expression (std::move (expression)),
		  identifier (std::move (identifier))
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbExpression>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{ return new PropertyExpression (expression -> copy (executionContext), std::string (identifier .getName ())); }

	///  @name Operations

	virtual
	void
	setValue (const var & value) const
	throw (pbException) final override
	{
		const auto lhs = expression -> getValue ();

		if (lhs .isObject ())
		{
			pbObject* const object = dynamic_cast <pbObject*> (lhs .getObject () .get ());
			
			if (object)
			{
				try
				{
					object -> setProperty (identifier, value);
				}
				catch (const std::out_of_range &)
				{
					object -> addPropertyDescriptor (identifier, value);
				}
			}
		}
	}

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	getValue () const
	throw (pbException,
	       pbControlFlowException) final override
	{
		const auto lhs = expression -> getValue ();

		if (lhs .isObject ())
		{
			pbObject* const object = dynamic_cast <pbObject*> (lhs .getObject () .get ());
			
			if (object)
			{
				try
				{
					return object -> getProperty (identifier);
				}
				catch (const std::out_of_range &)
				{ }
			}
		}

		return Undefined;
	}

	virtual
	var
	call (const ptr <pbExecutionContext> & ec, const std::vector <var> & arguments) const
	throw (pbException) final override
	{
		const auto lhs = expression -> getValue ();

		switch (lhs .getType ())
		{
			case UNDEFINED:
			{
				throw TypeError ("undefined has no properties");
			}
			case BOOLEAN:
			{
				// toString
				// lhs .toObject (ec) -> call (id, arguments)
				break;		
			}
			case NUMBER:
			{
				// toPrecision
				// toString
				// ...
				// lhs .toObject (ec) -> call (id, arguments)
				break;
			}
			case STRING:
			{
				// toString
				// ...
				// lhs .toObject (ec) -> call (id, arguments)
				break;
			}
			case NULL_OBJECT:
			{
				throw TypeError ("null has no properties");
			}
			case OBJECT:
			{
				pbObject* const object = static_cast <pbObject*> (lhs .getObject () .get ());

				return object -> apply (identifier, lhs, arguments);
			}
		}

		throw TypeError ("'" + lhs .toString () + "." + identifier .getName () + "' is not a function");
	}

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		ostream
			<< expression
			<< Generator::TidySpace
			<< '.'
			<< identifier;
	}


private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (expression); }

	///  @name Members

	const ptr <pbExpression> expression;
	const Identifier         identifier;

};

} // pb
} // titania

#endif

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

#ifndef __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_COMPUTED_MEMBER_ACCESS_EXPRESSION_H__
#define __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_COMPUTED_MEMBER_ACCESS_EXPRESSION_H__

#include "../Expressions/pbStatement.h"
#include "../Objects/pbObject.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript array index expression.
 */
class ComputedMemberAccessExpression :
	public pbStatement
{
public:

	///  @name Construction

	///  Constructs new AdditionExpression expression.
	ComputedMemberAccessExpression (pbExecutionContext* const executionContext, ptr <pbStatement> && expression, ptr <pbStatement> && identifierExpression) :
		         pbStatement (StatementType::COMPUTED_MEMBER_ACCESS_EXPRESSION),
		    executionContext (executionContext),
		          expression (std::move (expression)),
		identifierExpression (std::move (identifierExpression))
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbStatement>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{
		return new ComputedMemberAccessExpression (executionContext,
		                                           expression -> copy (executionContext),
		                                           identifierExpression -> copy (executionContext));
	}

	///  @name Operations

	///  throws pbError
	virtual
	void
	putValue (const var & value) const final override
	{
		const auto base       = expression -> getValue ();
		const auto identifier = Identifier (identifierExpression -> getValue () .toString ());

		switch (base .getType ())
		{
			case UNDEFINED:
			{
				throw TypeError ("undefined has no properties.");
			}
			case BOOLEAN:
			{
				// toString
				// base .toObject (ec) -> call (id, arguments)
				break;
			}
			case NUMBER:
			{
				// toPrecision
				// toString
				// ...
				// base .toObject (ec) -> call (id, arguments)
				break;
			}
			case STRING:
			{
				const auto index = identifier .toUInt32 ();
				
				if (index == PROPERTY)
					make_ptr <String> (executionContext, base .getString ()) -> put (identifier, value, false);
			}
			case NULL_OBJECT:
			{
				throw TypeError ("null has no properties.");
			}
			case OBJECT:
			{
				base .getObject () -> put (identifier, value, false);
			}
		}
	}

	///  Converts its input argument to either Primitive or Object type.
	///  throws pbError
	virtual
	CompletionType
	getValue () const final override
	{
		const auto base       = expression -> getValue ();
		const auto identifier = Identifier (identifierExpression -> getValue () .toString ());

		try
		{
			switch (base .getType ())
			{
				case UNDEFINED:
				{
					throw TypeError ("undefined has no properties.");
				}
				case BOOLEAN:
				{
					// toString
					// base .toObject (ec) -> call (id, arguments)
					break;
				}
				case NUMBER:
				{
					// toPrecision
					// toString
					// ...
					// base .toObject (ec) -> call (id, arguments)
					break;
				}
				case STRING:
				{
					const auto index = identifier .toUInt32 ();
					
					if (index == PROPERTY)
						return make_ptr <String> (executionContext, base .getString ()) -> get (identifier);
					
					if (index >= base .getString () .length ())
						break;

					return Glib::ustring (1, base .getString () [index]);
				}
				case NULL_OBJECT:
				{
					throw TypeError ("null has no properties.");
				}
				case OBJECT:
				{
					return base .getObject () -> get (identifier);
				}
			}
		}
		catch (const std::out_of_range &)
		{ }

		return undefined;
	}

	///  throws pbError
	virtual
	var
	call (const ptr <pbExecutionContext> & ec, const std::vector <var> & arguments) const final override
	{
		const auto base       = expression -> getValue ();
		const auto identifier = Identifier (identifierExpression -> getValue () .toString ());

		try
		{
			switch (base .getType ())
			{
				case UNDEFINED:
				{
					throw TypeError ("undefined has no properties");
				}
				case BOOLEAN:
				{
					// toString
					// base .toObject (ec) -> call (id, arguments)
					break;
				}
				case NUMBER:
				{
					// toPrecision
					// toString
					// ...
					// base .toObject (ec) -> call (id, arguments)
					break;
				}
				case STRING:
				{
					const auto index = identifier .toUInt32 ();
					
					if (index == PROPERTY)
						return make_ptr <String> (executionContext, base .getString ()) -> call (identifier, arguments);
				}
				case NULL_OBJECT:
				{
					throw TypeError ("null has no properties");
				}
				case OBJECT:
				{
					return base .getObject () -> call (identifier, arguments);
				}
			}
		}
		catch (const std::out_of_range &)
		{ }
		catch (const std::invalid_argument &)
		{ }

		throw TypeError ("'" + base .toString () + "['" + identifier .getString () + "'] is not a function");
	}

	///  throws pbError
	virtual
	bool
	deleteProperty () const final override
	{
		const auto base     = expression -> getValue ();
		const auto property = identifierExpression -> getValue ();
		const auto name     = property .toString ();

		switch (base .getType ())
		{
			case UNDEFINED:
				throw TypeError ("undefined has no properties");
			case BOOLEAN:
				return true;
			case NUMBER:
				return true;
			case STRING:
				return true;
			case NULL_OBJECT:
				throw TypeError ("null has no properties");
			case OBJECT:
			{
				try
				{
					base .getObject () -> deleteOwnProperty (name, false);
				}
				catch (const std::out_of_range &)
				{ }

				return true;
			}
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
			<< expression
			<< Generator::TidySpace
			<< '['
			<< identifierExpression
			<< ']';
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (executionContext, expression, identifierExpression); }

	///  @name Members

	const ptr <pbExecutionContext> executionContext;
	const ptr <pbStatement>        expression;
	const ptr <pbStatement>        identifierExpression;

};

} // pb
} // titania

#endif

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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_OBJECT_LITERAL_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_OBJECT_LITERAL_H__

#include "../Expressions/pbExpression.h"
#include "../Objects/Object.h"
#include "../Objects/pbFunction.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript object literal expression.
 */
class ObjectLiteral :
	public pbExpression
{
public:

	///  @name Construction

	///  Constructs new ObjectLiteral expression.
	ObjectLiteral (pbExecutionContext* const executionContext) :
		    pbExpression (ExpressionType::OBJECT_LITERAL),
		executionContext (executionContext),
		      properties ()
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbExpression>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{
		const auto copy = new ObjectLiteral (executionContext);
		
		for (const auto & pair : properties)
		{
			const auto & property = pair .second;

			copy -> defineOwnProperty (Identifier (property -> identifier),
			                           property -> value ? property -> value -> copy (executionContext) : nullptr,
			                           property -> attributes,
			                           property -> getter ? property -> getter -> copy (executionContext) : nullptr,
			                           property -> setter ? property -> setter -> copy (executionContext) : nullptr);
		}

		return copy;
	}

	///  @name Operations

	void
	defineOwnProperty (Identifier && identifier,
	                   ptr <pbExpression> && value,
	                   const AttributeType attributes,
	                   ptr <Function> && getter = nullptr,
	                   ptr <Function> && setter = nullptr)
	throw (SyntaxError)
	{
		auto & property = properties [identifier .getId ()];

		if (not property)
			property .reset (new PropertyDescriptor (properties .size ()));

		property -> identifier = std::move (identifier);
		
		if (value)
		{
			if (property -> getter)
				throw SyntaxError ("Object literal may not have data and accessor property with the same name.");

			property -> value = std::move (value);
		}

		if (getter)
		{
			if (property -> attributes & WRITABLE)
				throw SyntaxError ("Object literal may not have data and accessor property with the same name.");

			property -> getter = std::move (getter);
		}

		if (setter)
			property -> setter = std::move (setter);

		property -> attributes = attributes;

		sorted .emplace (property -> index, property);

		addChildren (property -> value,
		             property -> getter,
		             property -> setter);
	}

	///  Converts its input argument to either Primitive or Object type.
	virtual
	CompletionType
	getValue () const
	throw (pbError) final override
	{
		const auto object = new Object (executionContext .get ());

		for (const auto & property : sorted)
		{
			object -> addOwnProperty (property .second -> identifier,
			                          property .second -> value
			                          ? property .second -> value -> getValue ()
			                          : undefined,
			                          property .second -> attributes,
			                          property .second -> getter,
			                          property .second -> setter);
		}

		return object;
	}

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (properties .empty ())
		{
			ostream
				<< '{'
				<< Generator::TidySpace
				<< '}';
		}
		else
		{
			ostream
				<< '{'
				<< Generator::IncIndent
				<< Generator::TidyBreak;

			for (const auto & property : std::make_pair (sorted .begin (), -- sorted .end ()))
			{
				toStream (ostream, property .second);
	
				ostream
					<< ','
					<< Generator::Break;
			}

			toStream (ostream, (-- sorted .end ()) -> second);

			ostream
				<< Generator::Break
				<< Generator::DecIndent
				<< Generator::Indent
				<< '}';
		}
	}

private:

	///  @name Member types

	struct PropertyDescriptor
	{
		PropertyDescriptor (const size_t index) :
			attributes (NONE),
			index (index)
		{ }
	
		Identifier         identifier;
		ptr <pbExpression> value;
		AttributeType  attributes;
		ptr <Function>     getter;
		ptr <Function>     setter;
		size_t             index;
	};

	using PropertyDescriptorPtr = std::shared_ptr <PropertyDescriptor>;
	using PropertyIndex         = std::map <size_t, PropertyDescriptorPtr>;

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (executionContext); }

	///  @name Input/Output

	void
	toStream (std::ostream & ostream, const PropertyDescriptorPtr & property) const
	{
		bool line = false;
	
		if (property -> value)
		{
			ostream
				<< Generator::Indent
				<< property -> identifier
				<< ':'
				<< Generator::TidySpace
				<< property -> value;

			line = true;
		}
		
		if (property -> getter)
		{
			if (line)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}

			ostream
				<< Generator::Indent
				<< "get"
				<< Generator::Space
				<< property -> identifier
				<< Generator::TidySpace
				<< "()";

			if (property -> getter -> getExpressions () .empty ())
			{
				ostream
					<< Generator::TidySpace
					<< '{'
					<< Generator::TidySpace
					<< '}';
			}
			else
			{
				ostream
					<< Generator::TidyBreak
					<< Generator::Indent
					<< Generator::IncIndent
					<< '{';

				property -> getter -> pbExecutionContext::toStream (ostream);

				ostream
					<< Generator::DecIndent
					<< Generator::Indent
					<< '}';
			}

			line = true;
		}

		if (property -> setter)
		{
			if (line)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}

			ostream
				<< Generator::Indent
				<< "set"
				<< Generator::Space
				<< property -> identifier
				<< Generator::TidySpace
				<< '(';

			const auto & formalParameters = property -> setter -> getFormalParameters ();

			if (not formalParameters .empty ())
			{
				for (const auto parameter : std::make_pair (formalParameters .begin (), formalParameters .end () - 1))
				{
					ostream
						<< parameter
						<< ','
						<< Generator::TidySpace;
				}

				ostream << formalParameters .back ();
			}

			ostream << ')';
				
			if (property -> setter -> getExpressions () .empty ())
			{
				ostream
					<< Generator::TidySpace
					<< '{'
					<< Generator::TidySpace
					<< '}';
			}
			else
			{
				ostream
					<< Generator::TidyBreak
					<< Generator::Indent
					<< Generator::IncIndent
					<< '{';

				property -> setter -> pbExecutionContext::toStream (ostream);

				ostream
					<< Generator::DecIndent
					<< Generator::Indent
					<< '}';
			}
		}
	}

	///  @name Members

	const ptr <pbExecutionContext> executionContext;
	PropertyIndex                  properties;
	PropertyIndex                  sorted;

};

} // pb
} // titania

#endif

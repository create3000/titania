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
		
		for (const auto & property : properties)
		{
			copy -> updatePropertyDescriptor (std::string (property .second .identifier),
			                                  property .second .value  ? property .second .value  -> copy (executionContext) : nullptr,
			                                  property .second .getter ? property .second .getter -> copy (executionContext) : nullptr,
			                                  property .second .setter ? property .second .setter -> copy (executionContext) : nullptr);
		}

		return copy;
	}

	///  @name Operations

	void
	updatePropertyDescriptor (std::string && identifier,
	                          ptr <pbExpression> && value,
	                          ptr <pbFunction> && getter = nullptr,
	                          ptr <pbFunction> && setter = nullptr)
	{
		auto & property = properties [getId (identifier)];

		if (value)
			property .identifier = std::move (identifier);

		if (value)
			property .value = std::move (value);

		if (getter)
			property .getter = std::move (getter);

		if (setter)
			property .setter = std::move (setter);

		addChildren (property .value,
		             property .getter,
		             property .setter);
	}

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	getValue () const
	throw (pbException,
	       pbControlFlowException) final override
	{
		const auto object = new Object (executionContext .get ());

		for (const auto & property : properties)
		{
			try
			{
				object -> updatePropertyDescriptor (property .first,
				                                    property .second .identifier,
				                                    property .second .value ? property .second .value -> getValue () : Undefined (),
				                                    DEFAULT,
				                                    property .second .getter,
				                                    property .second .setter);
			}
			catch (const std::invalid_argument &)
			{ }
		}

		return object;
	}

private:

	///  @name Member types

	struct PropertyDescriptor
	{
		std::string        identifier;
		ptr <pbExpression> value;
		ptr <pbFunction>   getter;
		ptr <pbFunction>   setter;
	};

	using PropertyIndex = std::unordered_map <size_t, PropertyDescriptor>;

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (executionContext); }

	///  @name Members

	const ptr <pbExecutionContext> executionContext;
	PropertyIndex                  properties;

};

} // pb
} // titania

#endif

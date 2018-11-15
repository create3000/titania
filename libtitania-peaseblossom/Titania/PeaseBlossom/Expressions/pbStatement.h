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

#ifndef __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_PB_STATEMENT_H__
#define __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_PB_STATEMENT_H__

#include "../Base/pbChildObject.h"
#include "../Base/pbOutputStreamObject.h"
#include "../Execution/CompletionType.h"
#include "../Expressions/StatementType.h"
#include "../InputOutput/Generator.h"
#include "../Primitives/array.h"
#include "../Primitives/var.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript value. This is the base class for all ECMAScript values.
 */
class pbStatement :
	virtual public pbChildObject,
	virtual public pbOutputStreamObject
{
public:

	enum CompileType :
	uint8_t
	{
		PUT,
		GET
	};

	///  @name Construction

	virtual
	ptr <pbStatement>
	copy (pbExecutionContext* const) const
	noexcept (true) = 0;

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const
	noexcept (true) final override
	{ return typeName; }

	///  Returns the type  of this object.
	StatementType
	getType () const
	{ return type; }

	///  Returns wheter this expression is a primitive expression.
	bool
	isPrimitive () const;

	///  @name Conversion operations

	///  throws pbError
	virtual
	void
	putValue (const var &) const
	{ throw ReferenceError ("Invalid assignment left-hand side."); }

	///  throws pbError
	virtual
	CompletionType
	getValue () const = 0;

	///  throws pbError
	virtual
	var
	call (const ptr <pbExecutionContext> &, const std::vector <var> &) const;

	///  throws pbError
	virtual
	bool
	deleteProperty () const;


protected:

	///  @name Construction

	///  Constructs new pbStatement.
	pbStatement (const StatementType type) :
		       pbChildObject (),
		pbOutputStreamObject (),
		                type (type)
	{ }


private:

	///  @name Static members

	static const std::string typeName;

	///  @name Members

	const StatementType type;

};

///  @relates pbStatement
///  @name Construction

struct StatementsOutputType { const array <ptr <pbStatement>> & statements; };

///  Function to insert a array <ptr <pbStatement>> into an output stream.
inline
StatementsOutputType
toStream (const array <ptr <pbStatement>> & statements)
{
	return StatementsOutputType { statements };
}

///  Insertion operator for StatementType.
std::ostream &
operator << (std::ostream & ostream, const StatementsOutputType & value);

//

struct StatementOutputType { const ptr <pbStatement> & statement; const bool indent; };

///  Function to insert a array <ptr <pbStatement>> into an output stream.
inline
StatementOutputType
toStream (const ptr <pbStatement> & statement, const bool indent = false)
{
	return StatementOutputType { statement, indent };
}

///  Insertion operator for StatementType.
std::ostream &
operator << (std::ostream & ostream, const StatementOutputType & value);

//

struct ExpressionOutputType { const pbStatement* const parent; const ptr <pbStatement> & statement; };

///  Function to insert a array <ptr <pbStatement>> into an output stream.
inline
ExpressionOutputType
toStream (const pbStatement* const parent, const ptr <pbStatement> & statement)
{
	return ExpressionOutputType { parent, statement };
}

///  Insertion operator for StatementType.
std::ostream &
operator << (std::ostream & ostream, const ExpressionOutputType & value);

} // pb
} // titania

#endif

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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_PB_EXECUTION_CONTEXT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_PB_EXECUTION_CONTEXT_H__

#include "../Base/pbUserData.h"
#include "../Base/pbInputStreamObject.h"
#include "../Bits/Exception.h"
#include "../Objects/pbFunction.h"
#include "../Objects/pbObject.h"
#include "../Primitives/array.h"
#include "../Standard/StandardClassType.h"
#include "../Base/pbChildObject.h"
#include "../Base/pbOutputStreamObject.h"
#include "../Expressions/pbStatement.h"
#include "../Primitives/ptr.h"

namespace titania {
namespace pb {

class Function;
class Object;
class NativeFunction;
class VariableDeclaration;

class pbExecutionContext :
	virtual public pbChildObject,
	virtual public pbUserData,
	virtual public pbInputStreamObject,
	virtual public pbOutputStreamObject
{
public:

	/// @name Member access

	///  Set strict mode.
	void
	isStrict (const bool value)
	noexcept (true)
	{ strict = value; }

	///  Get strict mode.
	bool
	isStrict () const
	noexcept (true)
	{ return true; }

	///  Returns true if this execution context belongs to no other context otherwise false;
	bool
	isRootContext () const
	noexcept (true)
	{ return getExecutionContext () .get () == this; }

	virtual
	const ptr <pbExecutionContext> &
	getRootContext () const
	noexcept (true)
	{ return getExecutionContext () -> getRootContext (); }

	///  Returns the execution context this objects belongs to.
	const ptr <pbExecutionContext> &
	getExecutionContext () const
	noexcept (true)
	{ return executionContext; }

	///  Returns the global objects.
	const ptr <pbObject> &
	getGlobalObject () const
	noexcept (true)
	{ return variableObjects .back (); }

	virtual
	const ptr <pbObject> &
	getStandardObject () const
	noexcept (true)
	{ return getRootContext () -> getStandardObject (); }

	virtual
	const ptr <pbFunction> &
	getStandardFunction () const
	noexcept (true)
	{ return getRootContext () -> getStandardFunction (); }

	///  throws std::out_of_range
	virtual
	const ptr <pbFunction> &
	getStandardClass (const StandardClassType type) const
	{ return getRootContext () -> getStandardClass (type); }

	/// @name Function handling

	///  Checks wehter the global object has a function @a name.
	bool
	hasFunctionDeclaration (const Identifier & identifier) const
	noexcept (true)
	{ return functionDeclarations .count (identifier .getId ()); }

	///  Returns @a name local function, throws std::invalid_argument if function .name is empty or a function with
	///  name not exists.
	///  throws std::out_of_range
	const ptr <pbFunction> &
	getFunctionDeclaration (const Identifier & identifier) const
	{ return functionDeclarations .at (identifier .getId ()); }

	const std::map <size_t, ptr <pbFunction>> &
	getFunctionDeclarations () const
	noexcept (true)
	{ return functionDeclarations; }

	/// @name Statement handling

	///  Add an statement to the list of statements.
	void
	addStatement (ptr <pbStatement> && value)
	{ statements .emplace_back (std::move (value)); }

	///  Returns an array with all local root statements.
	const array <ptr <pbStatement>> &
	getStatements () const
	{ return statements; }

	/// @name Execution

	///  Executes the associated expessions of this context.
	///  throws pbError
	virtual
	var
	run ()
	{ return undefined; }

	/// @name Input/Output

	///  Parses @a istream and adds the expressions to this context. The stream must contain valid ECMAScript content
	///  otherwise an exception is thrown.
	///  throws pbError
	virtual
	void
	fromStream (std::istream & istream) final override;

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const override;

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () override;


protected:

	///  @name Friends

	friend class VariableDeclaration;
	friend class VariableExpression;
	friend class Function;
	friend class Parser;

	///  @name Construction

	///  Constructs new pbExecutionContext.
	pbExecutionContext (pbExecutionContext* const executionContext);

	///  @name Function handling

	///  Adds a function to this context, throws std::invalid_argument if a function with function .name already exists
	///  or function .name is empty.
	///  throws std::invalid_argumen
	void
	addFunctionDeclaration (ptr <pbFunction> && function);

	///  Updates a global function, throws std::invalid_argument if function .name is empty.
	///  throws std::invalid_argumen
	void
	updateFunctionDeclaration (ptr <pbFunction> && function);
	///  Removes the function identified by @a name from this execution context.
	void
	removeFunctionDeclaration (const Identifier & identifier)
	noexcept (true);

	///  @name Variable handling

	void
	addVariableDeclaration (ptr <VariableDeclaration> && variable)
	noexcept (true);

	const array <ptr <VariableDeclaration>> &
	getVariableDeclarations () const
	noexcept (true)
	{ return variableDeclarations; }

	///  @name Member access

	///  Returns the variable object.  That is the object where variable declarations are added.
	virtual
	const ptr <pbObject> &
	getVariableObject () const
	noexcept (true)
	{ return variableObjects .front (); }

	///  Returns the local objects.
	array <ptr <pbObject>> &
	getVariableObjects ()
	noexcept (true)
	{ return variableObjects; }

	///  Returns the local objects.
	const array <ptr <pbObject>> &
	getVariableObjects () const
	noexcept (true)
	{ return variableObjects; }

	//	///  Returns the lexical object.  That is the object where const and let declarations are added.
	//	virtual
	//	const ptr <pbObject> &
	//	getLexicalObject () const
	//	noexcept (true)
	//	{ return lexicalObjects .front (); }

	/// @name Operations

	///  Imports all function declarations and expressions from @a executionContext into this execution context.
	///  throws pbError
	void
	import (const pbExecutionContext* const executionContext);


private:

	/// @name Members

	const ptr <pbExecutionContext>       executionContext;
	std::map <size_t, ptr <pbFunction>>  functionDeclarations;
	array <ptr <VariableDeclaration>>    variableDeclarations;
	array <ptr <pbObject>>               variableObjects;
	array <ptr <pbStatement>>            statements;
	bool                                 strict;

};

} // pb
} // titania

#endif

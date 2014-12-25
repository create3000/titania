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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_VS_EXECUTION_CONTEXT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_VS_EXECUTION_CONTEXT_H__

#include "../Base/pbInputStreamObject.h"
#include "../Base/pbOutputStreamObject.h"
#include "../Bits/Exception.h"
#include "../Execution/pbBlock.h"
#include "../Objects/pbFunction.h"
#include "../Objects/pbObject.h"
#include "../Primitives/array.h"

namespace titania {
namespace pb {

class Function;

class pbExecutionContext :
	virtual public pbBlock,
	virtual public pbInputStreamObject
{
public:

	/// @name Member access

	///  Set strict mode.
	void
	isStrict (const bool value)
	{ strict = value; }

	///  Get strict mode.
	bool
	isStrict () const
	{ return true; }

	///  Returns true if this execution context belongs to no other context otherwise false;
	bool
	isRootContext () const
	{ return getExecutionContext () .get () == this; }

	///  Returns the execution context this objects belongs to.
	const ptr <pbExecutionContext> &
	getExecutionContext () const
	{ return executionContext; }

	///  Returns the global objects.
	const ptr <pbObject> &
	getGlobalObject () const
	{ return globalObject; }

	/// @name Operations

	///  Checks wehter the global object has a function @a name.
	bool
	hasFunctionDeclaration (const std::string & name) const
	noexcept (true)
	{ return functions .count (name); }

	///  Adds a function to this context, throws std::invalid_argument if a function with function .name already exists
	///  or function .name is empty.
	virtual
	void
	addFunctionDeclaration (const ptr <pbFunction> & function)
	throw (std::invalid_argument);

	///  Updates a global function, throws std::invalid_argument if function .name is empty.
	virtual
	void
	updateFunctionDeclaration (const ptr <pbFunction> & function)
	throw (std::invalid_argument);

	///  Removes the function identified by @a name from this execution context.
	virtual
	void
	removeFunctionDeclaration (const std::string & name)
	noexcept (true);

	///  Returns @a name local function, throws std::invalid_argument if function .name is empty or a function with
	///  name not exists.
	virtual
	const ptr <pbFunction> &
	getFunctionDeclaration (const std::string & name) const
	throw (std::out_of_range)
	{ return functions .at (name); }

	const std::map <std::string, ptr <pbFunction>> &
	getFunctionDeclarations () const
	{ return functions; }

	/// @name Input/Output

	///  Parses @a istream and adds the expressions to this context. The stream must contain valid ECMAScript content
	///  otherwise an exception is thrown.
	virtual
	void
	fromStream (std::istream & istream)
	throw (SyntaxError,
	       ReferenceError) final override;

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () override;


protected:

	///  @name Friends

	friend class Function;
	friend class VariableDeclaration;
	friend class VariableExpression;

	///  @name Construction

	///  Constructs new pbExecutionContext.
	pbExecutionContext (pbExecutionContext* const executionContext, const ptr <pbObject> & globalObject);

	/// @name Member access

	///  Replaces the current execution context by @a executionContext.
	void
	setExecutionContext (const ptr <pbExecutionContext> & value)
	{ executionContext = value; }

	///  Returns the local objects.
	ptr <pbObject> &
	getLocalObject ()
	{ return localObject; }

	///  Returns the local objects.
	const ptr <pbObject> &
	getLocalObject () const
	{ return localObject; }

	///  Returns the default objects stack.
	array <ptr <pbObject>> &
	getDefaultObjects ()
	{ return defaultObjects; }

	///  Returns the default objects stack.
	const array <ptr <pbObject>> &
	getDefaultObjects () const
	{ return defaultObjects; }

	/// @name Operations

	///  Imports all function declarations and expressions from @a executionContext into this execution context.
	void
	import (const pbExecutionContext* const executionContext);

	///  Executes the associated expessions of this context.
	virtual
	var
	run ();


private:

	///  @name Construction

	void
	construct ();

	/// @name Members

	bool                                      strict;
	ptr <pbExecutionContext>                  executionContext;
	ptr <pbObject>                            globalObject;
	ptr <pbObject>                            localObject;
	array <ptr <pbObject>>                    defaultObjects; // Use deque to keep iters when inserting values.
	std::map <std::string, ptr <pbFunction>>  functions;

};

} // pb
} // titania

#endif

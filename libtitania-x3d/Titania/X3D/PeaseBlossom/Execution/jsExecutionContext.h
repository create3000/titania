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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_JS_EXECUTION_CONTEXT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXECUTION_JS_EXECUTION_CONTEXT_H__

#include "../Base/jsChildObject.h"
#include "../Base/jsInputStreamObject.h"
#include "../Base/jsOutputStreamObject.h"
#include "../Bits/Exception.h"
#include "../Objects/Object.h"
#include "../Objects/jsFunction.h"

#include <Titania/Utility/Adapter.h>

#include <vector>

namespace titania {
namespace pb {

class Function;

class jsExecutionContext :
	virtual public jsChildObject,
	virtual public jsInputStreamObject,
	virtual public jsOutputStreamObject
{
public:

	/// @name Member access

	///  Returns the global objects.
	const basic_ptr <jsObject> &
	getGlobalObject () const
	{ return globalObject; }

	/// @name Input/Output

	///  Parses @a istream and adds the expressions to this context. The stream must contain valid JavaScript content
	///  otherwise an exception is thrown.
	virtual
	void
	fromStream (std::istream & istream)
	throw (SyntaxError) final override;

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing of an object does not remove object itself.
	virtual
	void
	dispose () override;


protected:

	friend class Parser;
	friend class Identifier;
	friend class VariableDeclaration;

	///  @name Construction

	///  Constructs new jsExecutionContext.
	jsExecutionContext (jsExecutionContext* const executionContext, const basic_ptr <jsObject> & globalObject);

	/// @name Member access

	///  Set strict mode.
	void
	isStrict (const bool value)
	{ strict = value; }

	///  Get strict mode.
	bool
	isStrict () const
	{ return true; }

	///  Returns the execution context this objects belongs to.
	const basic_ptr <jsExecutionContext> &
	getExecutionContext () const
	{ return executionContext; }

	/// @name Operations

	///  Pushs an object to the default object stack.
	void
	addDefaultObject (const basic_ptr <jsObject> & object);

	///  Removes the current default object from the default object stack.
	void
	removeDefaultObject ()
	{ return defaultObjects .pop_back (); }

	///  Gets the current default object.
	const basic_ptr <jsObject> &
	getDefaultObject ()
	{ return defaultObjects .back (); }

	///  Returns a property descriptor for a named property on an object in the scope hierarchy.
	const PropertyDescriptor &
	getPropertyDescriptor (const std::string & name) const
	throw (ReferenceError);

	///  Adds an expression to this context.
	void
	addExpression (var &&);

	///  Adds a function to this context.
	virtual
	void
	defineFunction (const basic_ptr <jsFunction> & function);

	///  @name Operations

	///  Executes the associated expessions of this context.
	virtual
	var
	run ();


private:

	/// @name Members

	bool                                             strict;
	const basic_ptr <jsExecutionContext>            executionContext;
	basic_ptr <jsObject>                            globalObject;
	std::deque <basic_ptr <jsObject>>               defaultObjects; // Use deque to keep iters when inserting value.
	std::deque <var>                                 expressions;    // Use deque to keep iters when inserting value.
	std::map <std::string, basic_ptr <jsFunction>>  functions;

};

} // pb
} // titania

#endif

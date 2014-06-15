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
#include "../Primitives/Object.h"
#include "../Primitives/jsFunction.h"
#include "../Values/var.h"

#include <Titania/Utility/Adapter.h>

#include <vector>

namespace titania {
namespace pb {

class jsExecutionContext :
	virtual public jsChildObject,
	virtual public jsInputStreamObject,
	virtual public jsOutputStreamObject
{
public:

	/// @name Global object access

	const basic_ptr <jsObject> &
	getGlobalObject () const
	{ return globalObject; }

	/// @name Function services

	void
	replaceFunction (const basic_ptr <jsFunction> & function);

	/// @name Input/Output

	virtual
	void
	fromStream (std::istream & istream)
	throw (SyntaxError) final override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	friend class Parser;
	friend class Identifier;
	friend class VariableDeclaration;

	///  @name Construction

	jsExecutionContext (jsExecutionContext* const executionContext, const basic_ptr <jsObject> & globalObject);

	/// @name Default object services

	void
	addDefaultObject (const basic_ptr <jsObject> & object);

	void
	removeDefaultObject ()
	{ return defaultObjects .pop_back (); }

	const basic_ptr <jsObject> &
	getDefaultObject ()
	{ return defaultObjects .back (); }

	/// @name Property access

	const var &
	getProperty (const std::string & name) const
	throw (ReferenceError);

	/// @name Expression access

	template <class ... Args>
	void
	addExpression (Args && ... args)
	{
		expressions .emplace_back (std::forward <Args> (args) ...);
		expressions .back () .addParent (this);
	}

	///  @name Operations

	virtual
	var
	run ();

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const override
	{ ostream << "[program Program]"; }


private:

	/// @name Members

	const basic_ptr <jsExecutionContext>            executionContext;
	std::map <std::string, basic_ptr <jsFunction>>  functions;
	basic_ptr <jsObject>                            globalObject;
	std::deque <basic_ptr <jsObject>>               defaultObjects; // Use deque to keep iters when inserting value.
	std::deque <var>                                expressions;    // Use deque to keep iters when inserting value.

};

} // pb
} // titania

#endif

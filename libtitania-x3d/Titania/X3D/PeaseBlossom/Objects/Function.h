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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_FUNCTION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_FUNCTION_H__

#include "../Execution/pbExecutionContext.h"
#include "../Objects/pbFunction.h"

#include <atomic>

namespace titania {
namespace pb {

/**
 *  Class to represent a scripted ECMAScript function.
 */
class Function :
	public pbFunction,
	public pbExecutionContext
{
public:

	///  @name Construction

	///  Constructs new Function.
	Function (pbExecutionContext* const, const std::string & = "", std::vector <std::string> && = { });

	///  Creates a new default object.
	virtual
	ptr <pbFunction>
	copy (pbExecutionContext* const) const
	noexcept (true) final override;

	///  @name Operations

	///  Executes this function.
	virtual
	var
	apply (const var &, const std::vector <var> & = { })
	throw (pbException) final override;

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future.  If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () final override;

	///  Sets the recusion limit to @a value.  The default value for the recursion limit is 100,000.
	static
	void
	setRecursionLimit (const size_t value)
	{ recursionLimit = value; }

	///  Returns the recursion limit.
	static
	size_t
	getRecursionLimit ()
	{ return recursionLimit; }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream &) const final override;


protected:

	///  @name Friends

	friend class Program;

	///  @name Construction

	///  Constructs new standard Function.
	Function (pbExecutionContext* const, const std::nullptr_t);

	///  @name Operations

	///  Constructs new object of this class.
	virtual
	var
	construct (const ptr <pbExecutionContext> &, const var &, const std::vector <var> & = { })
	throw (pbException) final override;


private:

	///  @name Member types

	class StackGuard
	{
	public:

		StackGuard (Function* const function, pbObject* const localObject) :
			function (function)
		{ function -> push (localObject); }

		~StackGuard ()
		{ function -> pop (); }


	private:

		Function* const function;

	};

	///  @name Operations
	
	void
	addLocalObjects (const ptr <pbExecutionContext> &);

	///  Set @a localObject as local object and pushes all default objects to the default object stack if an recursion is
	///  detected.
	void
	push (pbObject* const);

	///  Reverses the effect of pop.
	void
	pop ();

	///  @name Static members

	static std::atomic <size_t> recursionLimit;

	///  @name Member access

	const std::vector <std::string> formalParameters;
	array <ptr <pbObject>>          localObjects;
	array <ptr <pbObject>>          localObjectsStack;
	size_t                          recursionDepth;

};

} // pb
} // titania

#endif

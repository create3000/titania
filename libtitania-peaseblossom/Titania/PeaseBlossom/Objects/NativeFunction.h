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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_NATIVE_FUNCTION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_NATIVE_FUNCTION_H__

#include "../Execution/pbExecutionContext.h"
#include "../Objects/pbFunction.h"

namespace titania {
namespace pb {

using FunctionType = std::function <var (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & arguments)>;

/**
 *  Class to represent a native ECMAScript function.
 */
class NativeFunction :
	public pbFunction
{
public:

	///  @name Construction

	///  Constructs new Function.
	NativeFunction (pbExecutionContext* const executionContext,
	                const Identifier & name,
	                const FunctionType & function,
	                const size_t length);

	///  Constructs new Function.
	NativeFunction (pbExecutionContext* const executionContext,
	                const Identifier & name,
	                const FunctionType & constructor,
	                const FunctionType & function,
	                const size_t length);

	///  @name Common members

	virtual
	const std::string &
	getTypeName () const
	noexcept (true) final override
	{ return getName () .getString (); }

	///  @name Operations

	///  throws pbError
	virtual
	var
	call (const var & object, const std::vector <var> & arguments = { }) final override;

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream &) const final override;


protected:

	///  @name operations

	///  throws pbError
	virtual
	var
	construct (const var & object, const std::vector <var> & arguments = { }) final override;


private:

	///  @name Operations
	
	FunctionType
	getDefaultConstructor (const Identifier & name) const;

	FunctionType
	getDefaultFunction (const Identifier & name) const;

	///  @name Member access

	const ptr <pbExecutionContext> executionContext;
	const FunctionType             constructor;
	const FunctionType             function;

};

} // pb
} // titania

#endif

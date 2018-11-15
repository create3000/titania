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

#ifndef __TITANIA_PEASE_BLOSSOM_EXECUTION_PROGRAM_H__
#define __TITANIA_PEASE_BLOSSOM_EXECUTION_PROGRAM_H__

#include "../Execution/pbExecutionContext.h"

namespace titania {
namespace pb {

class Program :
	public pbExecutionContext
{
public:

	using pbExecutionContext::run;

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const
	noexcept (true) override
	{ return typeName; }

	///  @name Member access

	virtual
	const ptr <pbExecutionContext> &
	getRootContext () const
	noexcept (true) final override
	{ return getExecutionContext (); }

	virtual
	const ptr <pbObject> &
	getStandardObject () const
	noexcept (true) final override
	{ return standardObject; }

	virtual
	const ptr <pbFunction> &
	getStandardFunction () const
	noexcept (true) final override
	{ return standardFunction; }

	///  throws std::out_of_range
	virtual
	const ptr <pbFunction> &
	getStandardClass (const StandardClassType type) const;

	/// @name Execution

	///  throws pbError
	virtual
	var
	run () final override;

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ ostream << "[program Program]"; }

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () final override;


protected:

	///  @name Friends

	friend
	ptr <Program>
	createProgram ();

	///  @name Construction

	///  Constructs new Program.
	Program ();

	///  Adds the standard classes and properties to the global object.
	void
	initialize ()
	noexcept (true);


private:

	///  @name Operations

	bool
	resolve (const Identifier & identifier);

	///  @name Static members

	static const std::string typeName;

	///  @name Members

	ptr <pbObject>                    standardObject;
	ptr <pbFunction>                  standardFunction;
	mutable array <ptr <pbFunction>>  standardClasses;

};

///  @relates Program
///  @name Construction

///  Constructs new Program.
ptr <Program>
createProgram ();

} // pb
} // titania

#endif

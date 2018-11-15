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

#ifndef __TITANIA_PEASE_BLOSSOM_OBJECTS_STRING_H__
#define __TITANIA_PEASE_BLOSSOM_OBJECTS_STRING_H__

#include "../Objects/pbObject.h"
#include "../Standard/String.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a »string« object.
 */
class String :
	public pbObject
{
public:

	///  @name Construction

	///  Constructs new String.
	String (pbExecutionContext* const executionContext, const Glib::ustring & value = "");

	///  @name Array access

	///  Sets the value of the property for @a identifier.
	///  throws pbError, std::out_of_range
	virtual
	void
	put (const Identifier & identifier, const var & value, const bool throw_ = false) final override;

	///  Returns the value of the property for @a identifier.
	///  throws pbError, std::out_of_range
	virtual
	var
	get (const Identifier & identifier) const final override;

	///  @name Member access

	/// Sets the value of this String object.
	void
	setValue (const Glib::ustring & string)
	{ value = string; }

	/// Returns the value associated.
	const Glib::ustring &
	getValue () const
	{ return value; }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ ostream << value; }


protected:

	///  @name Friends

	friend
	ptr <NativeFunction>
	Standard::String::initialize (pbExecutionContext* const);

	///  @name Construction

	///  Constructs new String.
	String (pbExecutionContext* const executionContext, const nullptr_t);


private:

	///  @name Construction

	void
	addProperties (pbExecutionContext* const executionContext);

	///  @name Properties

	static
	pb::var
	getLength (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Members

	Glib::ustring value;

};

} // pb
} // titania

#endif

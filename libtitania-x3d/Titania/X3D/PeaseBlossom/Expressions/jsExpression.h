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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_JS_EXPRESSION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_JS_EXPRESSION_H__

#include "../Primitives/jsValue.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript value. This is the base class for all ECMAScript values.
 */
class jsExpression :
	public jsValue
{
public:

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const override
	{ return typeName; }

	///  Returns the a default of its input argument type.
	virtual
	var
	getDefaultValue () const final override
	{ return toValue () -> getDefaultValue (); }

	///  @name Common operations

	///  Returns true if the input argument is a non-Object type otherwise false.
	virtual
	bool
	isPrimitive () const final override
	{ return false; }

	///  Converts its input argument to a non-Object type.
	virtual
	var
	toPrimitive () const final override
	{ return toValue () -> toPrimitive (); }

	///  Converts its argument to a value of type Boolean.
	virtual
	bool
	toBoolean () const override
	{ return toValue () -> toBoolean (); }

	///  Converts its argument to an integral unsigned value of 16 bit.
	virtual
	uint16_t
	toUInt16 () const override
	{ return toValue () -> toUInt16 (); }

	///  Converts its argument to an integral signed value of 32 bit.
	virtual
	int32_t
	toInt32 () const override
	{ return toValue () -> toInt32 (); }

	///  Converts its argument to an integral unsigned value of 32 bit.
	virtual
	uint32_t
	toUInt32 () const override
	{ return toValue () -> toUInt32 (); }

	///  Converts its argument to a value of type Number.
	virtual
	double
	toNumber () const override
	{ return toValue () -> toNumber (); }

	///  Converts its argument to a value of type Object.
	virtual
	var
	toObject () const
	throw (TypeError) final override
	{ return toValue () -> toObject (); }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ toValue () -> toStream (ostream); }


protected:

	///  @name Construction

	///  Constructs new jsExpression.
	jsExpression () :
		jsValue ()
	{ }


private:

	///  @name Static members

	static const std::string typeName;

};

} // pb
} // titania

#endif

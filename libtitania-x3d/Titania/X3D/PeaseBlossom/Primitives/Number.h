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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_NUMBER_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_NUMBER_H__

#include "../Primitives/jsNumber.h"

#include <iomanip>

namespace titania {
namespace pb {

/**
 *  Class to represent a »number« value.
 */
class Number :
	public jsNumber
{
public:

	///  @name Construction

	Number () :
		jsNumber (),
		  number (0)
	{ }

	explicit
	Number (const double value) :
		jsNumber (),
		  number (value)
	{ }

	explicit
	Number (const var & value) :
		jsNumber (),
		  number (value -> toNumber ())
	{ }

	///  @name Common members

	///  Returns the a default of its input argument type.
	virtual
	var
	getDefaultValue () const final override
	{ return defaultValue; }

	///  @name Common operations

	///  Converts its input argument to a non-Object type.
	virtual
	var
	toPrimitive () const final override
	{ return var (const_cast <Number*> (this)); }

	///  Converts its argument to a value of type Boolean.
	virtual
	bool
	toBoolean () const override
	{ return number; }

	///  Converts its argument to an integral unsigned value of 16 bit.
	virtual
	uint16_t
	toUInt16 () const override
	{ return toInteger (); }

	///  Converts its argument to an integral unsigned value of 32 bit.
	virtual
	uint32_t
	toUInt32 () const override
	{ return toInteger (); }

	///  Converts its argument to a value of type Number.
	virtual
	double
	toNumber () const override
	{ return number; }

	///  Converts its argument to a value of type Object.
	virtual
	var
	toObject () const
	throw (TypeError) final override;

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const override;


private:

	///  @name Static members

	static const var defaultValue;

	///  @name Members

	const double number;

};

inline
void
Number::toStream (std::ostream & ostream) const
{
	if (std::isnan (number))
		ostream << "NaN";

	else if (number == NEGATIVE_INFINITY ())
		ostream << "-Infinity";

	else if (number == POSITIVE_INFINITY ())
		ostream << "Infinity";

	else
		ostream << std::setprecision (std::numeric_limits <double>::digits10) << number;
}

} // pb
} // titania

#endif

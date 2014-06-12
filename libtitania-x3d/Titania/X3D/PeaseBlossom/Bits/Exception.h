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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_BITS_EXCEPTION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_BITS_EXCEPTION_H__

#include "../Base/jsOutputStreamType.h"

#include <exception>
#include <map>
#include <sstream>
#include <string>

namespace titania {
namespace pb {

enum ExceptionType
{
	ERROR,
	EVAL_ERROR,
	RANGE_ERROR,
	REFERENCE_ERROR,
	SYNTAX_ERROR,
	TYPE_ERROR,
	URI_ERROR

};

///  @relates AssignmentOperatorType
///  @name Input/Output operators.

template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const ExceptionType value)
noexcept (true)
{
	static const std::map <ExceptionType, std::string> exceptionTypes = {
		std::make_pair (ExceptionType::ERROR,           "Error"),
		std::make_pair (ExceptionType::EVAL_ERROR,      "EvalError"),
		std::make_pair (ExceptionType::RANGE_ERROR,     "RangeError"),
		std::make_pair (ExceptionType::REFERENCE_ERROR, "ReferenceError"),
		std::make_pair (ExceptionType::SYNTAX_ERROR,    "SyntaxError"),
		std::make_pair (ExceptionType::TYPE_ERROR,      "TypeError"),
		std::make_pair (ExceptionType::URI_ERROR,       "URIError")
	};

	return ostream << exceptionTypes .at (value);
}

inline
std::string
to_string (const ExceptionType type)
noexcept (true)
{
	std::ostringstream osstream;

	osstream << type;

	return osstream .str ();
}

/**
 *  Base class to represent a JavaScript exception.
 */
class jsException :
	public std::exception,
	public jsOutputStreamType
{
public:

	///  @name Member access

	virtual
	const char*
	what () const
	noexcept (true) final override
	{ return message .c_str (); }

	virtual
	ExceptionType
	getType () const
	noexcept (true) = 0;

	const std::string &
	getMessage () const
	noexcept (true)
	{ return message; }

	virtual
	void
	toStream (std::ostream & ostream) const
	noexcept (true) final override
	{ ostream << getType () << ": " << message; }

	///  @name Destruction

	virtual
	~jsException ()
	noexcept (true)
	{ }


protected:

	///  @name Construction

	explicit
	jsException (const std::string & message) :
		      std::exception (),
		jsOutputStreamType (),
		             message (message)
	{ }


private:

	///  @name Members

	const std::string message;

};

/**
 *  Template to represent a JavaScript exception.
 *
 *  @param  ExceptionType  Type of exeception.
 */
template <ExceptionType type>
class Exception :
	public jsException
{
public:

	explicit
	Exception (const std::string & message) :
		jsException (message)
	{ }

	virtual
	ExceptionType
	getType () const
	noexcept (true) final override
	{ return type; }

};

} // pb
} // titania

#endif

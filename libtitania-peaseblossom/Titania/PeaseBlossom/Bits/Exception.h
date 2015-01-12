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

#ifndef __TITANIA_PEASE_BLOSSOM_BITS_EXCEPTION_H__
#define __TITANIA_PEASE_BLOSSOM_BITS_EXCEPTION_H__

#include "../Base/pbOutputStreamObject.h"

#include <exception>
#include <sstream>
#include <string>

namespace titania {
namespace pb {

/**
 *  Base class to represent a control flow exception.
 */
class pbException :
	public std::exception
{
public:

	pbException ()
	noexcept (true) :
		std::exception ()
	{ }

	virtual
	~pbException ()
	noexcept (true)
	{ }

};

enum ErrorType
{
	ERROR,
	EVAL_ERROR,
	RANGE_ERROR,
	REFERENCE_ERROR,
	SYNTAX_ERROR,
	TYPE_ERROR,
	URI_ERROR,
	RUNTIME_ERROR

};

///  @relates AssignmentOperatorType
///  @name Input/Output operators.

template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const ErrorType type)
noexcept (true)
{
	switch (type)
	{
		case ERROR:           ostream << "Error";          break;
		case EVAL_ERROR:      ostream << "EvalError";      break;
		case RANGE_ERROR:     ostream << "RangeError";     break;
		case REFERENCE_ERROR: ostream << "ReferenceError"; break;
		case SYNTAX_ERROR:    ostream << "SyntaxError";    break;
		case TYPE_ERROR:      ostream << "TypeError";      break;
		case URI_ERROR:       ostream << "URIError";       break;
		case RUNTIME_ERROR:   ostream << "RuntimeError";   break;
	}

	return ostream;
}

inline
std::string
to_string (const ErrorType type)
noexcept (true)
{
	std::ostringstream osstream;

	osstream << type;

	return osstream .str ();
}

/**
 *  Base class to represent a ECMAScript exception.
 */
class pbError :
	public pbException,
	public pbOutputStreamObject
{
public:

	///  @name Member access

	virtual
	const char*
	what () const
	noexcept (true) final override
	{ return message .c_str (); }

	virtual
	ErrorType
	getType () const
	noexcept (true) = 0;

	const std::string &
	getMessage () const
	noexcept (true)
	{ return message; }

	void
	setFilename (const std::string & value)
	noexcept (true)
	{ filename = value; }

	const std::string &
	getFilename () const
	noexcept (true)
	{ return filename; }

	void
	setLineNumber (const size_t value)
	noexcept (true)
	{ lineNumber = value; }

	size_t
	getLineNumber () const
	noexcept (true)
	{ return lineNumber; }

	void
	setColumn (const size_t value)
	noexcept (true)
	{ column = value; }

	size_t
	getColumn () const
	noexcept (true)
	{ return column; }

	void
	setSourceLine (const std::string & value)
	noexcept (true)
	{ sourceLine = value; }

	const std::string &
	getSourceLine () const
	noexcept (true)
	{ return sourceLine; }

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const
	noexcept (true) final override
	{ ostream << getType () << ": " << message; }

	///  @name Destruction

	///  Destructs the pbError.
	virtual
	~pbError ()
	noexcept (true)
	{ }


protected:

	///  @name Construction

	///  Constructs new pbError.
	explicit
	pbError (const std::string & message) :
		         pbException (),
		pbOutputStreamObject (),
		             message (message),
		            filename (),
		          lineNumber (0),
		              column (0),
		          sourceLine ()
	{ }


private:

	///  @name Members

	const std::string message;
	std::string       filename;
	size_t            lineNumber;
	size_t            column;
	std::string       sourceLine;

};

/**
 *  Template to represent a ECMAScript exception.
 *
 *  @param  ErrorType  Type of exeception.
 */
template <ErrorType type>
class Exception :
	public pbError
{
public:

	explicit
	Exception (const std::string & message) :
		pbError (message)
	{ }

	virtual
	ErrorType
	getType () const
	noexcept (true) final override
	{ return type; }

};

using Error          = Exception <ERROR>;
using EvalError      = Exception <EVAL_ERROR>;
using RangeError     = Exception <RANGE_ERROR>;
using ReferenceError = Exception <REFERENCE_ERROR>;
using SyntaxError    = Exception <SYNTAX_ERROR>;
using TypeError      = Exception <TYPE_ERROR>;
using URIError       = Exception <URI_ERROR>;
using RuntimeError   = Exception <RUNTIME_ERROR>;

} // pb
} // titania

#endif

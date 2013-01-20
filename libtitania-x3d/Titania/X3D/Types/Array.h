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

#ifndef __TITANIA_X3D_TYPES_ARRAY_H__
#define __TITANIA_X3D_TYPES_ARRAY_H__

#include "../InputOutput/Generator.h"
#include <deque>
#include <istream>
#include <ostream>

namespace titania {
namespace X3D {

template <class Type, template <class, class Allocator = std::allocator <Type>> class ArrayType = std::deque>
class Array :
	public ArrayType <Type>
{
public:

	typedef Type                                        value_type;
	typedef ArrayType <Type>                            array_type;
	typedef typename array_type::iterator               iterator;
	typedef typename array_type::const_iterator         const_iterator;
	typedef typename array_type::reverse_iterator       reverse_iterator;
	typedef typename array_type::const_reverse_iterator const_reverse_iterator;
	typedef typename array_type::difference_type        difference_type;
	typedef typename array_type::size_type              size_type;

	///  The default constructor returns an empty Array.
	Array () :
		ArrayType <Type> ()
	{ }

	///  Copy constructor.
	Array (const Array & value) :
		ArrayType <Type> (value)
	{ }

	///  Move constructor.
	Array (Array && value) :
		ArrayType <Type> (value)
	{ }

	template <class InputIterator>
	Array (InputIterator first, InputIterator last) :
		ArrayType <Type> (first, last)
	{ }

	Array &
	operator = (const Array & value)
	{
		ArrayType <Type>::operator = (value);
		return *this;
	}

	Array &
	operator = (Array && value)
	{
		ArrayType <Type>::operator = (value);
		return *this;
	}

	~Array ()
	{ }

};

template <class CharT, class Traits, class Type, template <class, class Allocator> class ArrayType>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, Array <Type, ArrayType> & array)
{
	array .clear ();

	Type value;

	while (istream >> value)
		array .push_back (value);

	return istream;
}

template <class CharT, class Traits, class Type, template <class, class Allocator> class ArrayType>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const Array <Type, ArrayType> & array)
{
	if (array .size () > 1)
	{
		ostream << Generator::OpenBracket;

		std::copy (array .begin (),
		           array .end () - 1,
		           Generator::ListSeparator <Type> (ostream));

		ostream << array .back () << Generator::CloseBracket;

		return ostream;
	}

	if (array .size () == 1)
	{
		ostream << array .front ();
		return ostream;
	}

	return ostream << Generator::EmptyBrackets;
}

} // X3D
} // titania

#endif

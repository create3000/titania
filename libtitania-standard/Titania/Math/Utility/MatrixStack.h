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

#ifndef __TITANIA_MATH_UTILITY_MATRIX4STACK_H__
#define __TITANIA_MATH_UTILITY_MATRIX4STACK_H__

#include "../Numbers/Matrix3.h"
#include "../Numbers/Matrix4.h"
#include <vector>

#include "../../LOG.h"

namespace titania {
namespace math {

/**
 *  Template to represent a matrix4 stack.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class matrix_stack
{
public:

	///  Value typedef.
	using value_type = Type;

	///  Container typedef.
	using container_type = std::vector <value_type>;

	///  Translation typedef.
	using translation_type = typename value_type::translation_type;

	///  Rotation typedef.
	using rotation_type = typename value_type::rotation_type;

	///  Scale typedef.
	using scale_type = typename value_type::scale_type;

	///  @name Constructors

	///  Default constructor.
	constexpr
	matrix_stack () :
		stack (1)
	{ }

	///  Constructs the underlying container with the contents of @a container.
	matrix_stack (const container_type & container) :
		stack (container)
	{ }

	///  Copy constructor.
	matrix_stack (const matrix_stack &) = delete;

	///  Default assignment operator.
	matrix_stack &
	operator = (const matrix_stack &) = delete;

	///  @name Element access
	
	void
	set (const value_type & value)
	{ stack .back () = value; }

	const value_type &
	get () const
	{ return stack .back (); }

	typename value_type::value_type*
	data ()
	{ return stack .back () .data (); }

	const typename value_type::value_type*
	data () const
	{ return stack .back () .data (); }

	///  @name Operations
	
	void
	push ()
	{ stack .emplace_back (stack .back ()); }

	void
	push (const value_type & value)
	{ stack .emplace_back (value); }

	void
	pop ()
	{ stack .pop_back (); }
	
	///  @name Capacity

	bool
	empty () const
	{ return stack .empty (); }

	typename container_type::size_type
	size () const
	{ return stack .size (); }

	///  @name Arithmetic operations

	void
	identity ()
	{ stack .back () .set (); }

	void
	mult_left (const value_type & value)
	{ stack .back () .mult_left (value); }

	void
	translate (const translation_type & value)
	{ stack .back () .translate (value); }

	void
	rotate (const rotation_type & value)
	{ stack .back () .rotate (value); }

	void
	scale (const scale_type & value)
	{ stack .back () .scale (value); }


private:

	container_type stack;

};

///  @relates matrix_stack
///  @name Input/Output operations

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const matrix_stack <Type> & stack)
{
	return ostream << stack .get ();
}

extern template class matrix_stack <matrix3 <float>>;
extern template class matrix_stack <matrix3 <double>>;
extern template class matrix_stack <matrix3 <long double>>;

extern template class matrix_stack <matrix4 <float>>;
extern template class matrix_stack <matrix4 <double>>;
extern template class matrix_stack <matrix4 <long double>>;

//
extern template std::ostream & operator << (std::ostream &, const matrix_stack <matrix3 <float>> &);
extern template std::ostream & operator << (std::ostream &, const matrix_stack <matrix3 <double>> &);
extern template std::ostream & operator << (std::ostream &, const matrix_stack <matrix3 <long double>> &);

extern template std::ostream & operator << (std::ostream &, const matrix_stack <matrix4 <float>> &);
extern template std::ostream & operator << (std::ostream &, const matrix_stack <matrix4 <double>> &);
extern template std::ostream & operator << (std::ostream &, const matrix_stack <matrix4 <long double>> &);

} // math
} // titania

#endif

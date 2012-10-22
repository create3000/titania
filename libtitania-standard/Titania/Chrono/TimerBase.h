/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#ifndef __TITANIA_CHRONO_TIMER_H__
#define __TITANIA_CHRONO_TIMER_H__

#include <complex>

namespace titania {
namespace chrono {

template <class Type>
class timer_base
{
public:

	typedef Type value_type;

	///  Get the current cycle of this clock.
	constexpr Type
	cycle () const { return value .real (); }

	///  Assignment operator for clocks.
	timer_base &
	operator = (const timer_base & clock)
	{
		value = clock .value;
		return *this;
	}

	///  Compares if the cycle of this clock is before the cycle of @a clock.
	constexpr bool
	before (const timer_base & clock) const { return before (clock .cycle ()); }

	///  Compares if the cycle of this clock is before @a value.
	virtual
	bool
	before (const Type & value) const { return cycle () < value; }

	///  Get the last interval of this clock.  The interval is the amount of time between two cycles.
	///  A steady clock will alway have a constant interval.
	constexpr Type
	interval () const { return value .imag (); }

	///  Advance this clock.  This will set the cycle of this clock to the new count.
	virtual
	void
	advance () { cycle (count ()); }

	virtual
	~timer_base () { }

protected:

	///  Component constructor.  Sets the value for this clock to @a cycle and @a interval.
	constexpr
	timer_base (const Type & cycle, const Type & interval) :
		value (cycle, interval) { }

	///  Copy constructor.
	constexpr
	timer_base (const timer_base & clock) :
		value (clock .value) { }

	///  Set the last interval of this clock.
	void
	interval (const Type & interval) { value .imag (interval); }


private:

	///  Set the cycle of this clock.
	void
	cycle (const Type & cycle) { return value .real (cycle); }

	///  Get the count of this clock.
	virtual
	Type
	count () const = 0;

	std::complex <Type> value;

};

template <class Type>
inline
constexpr bool
operator < (const timer_base <Type> & a, const timer_base <Type> & b)
{
	return a .before (b);
}

template <class Type>
inline
constexpr bool
operator < (const timer_base <Type> & a, const Type & b)
{
	return a .before (b);
}

template <class Type>
inline
constexpr bool
operator > (const timer_base <Type> & a, const timer_base <Type> & b)
{
	return not a .before (b);
}

template <class Type>
inline
constexpr bool
operator > (const timer_base <Type> & a, const Type & b)
{
	return not a .before (b);
}

///  Extraction operator for clock values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, timer_base <Type> & clock)
{
	return istream;
}

///  Insertion operator for clock values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const timer_base <Type> & clock)
{
	return ostream << clock .cycle ();
}

extern template class timer_base <double>;
extern template std::istream & operator >> (std::istream &, timer_base <double> &);
extern template std::ostream & operator << (std::ostream &, const timer_base <double> &);

} // basic
} // titania

#endif

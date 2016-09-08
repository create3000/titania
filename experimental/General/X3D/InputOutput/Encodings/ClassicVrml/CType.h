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

#ifndef __GENERAL_X3D_INPUT_OUTPUT_ENCODINGS_CLASSIC_VRML_CTYPE_H__
#define __GENERAL_X3D_INPUT_OUTPUT_ENCODINGS_CLASSIC_VRML_CTYPE_H__

#include <locale>
#include <vector>

namespace titania {
namespace X3D {
namespace Encodings {
namespace ClassicVrml {

template <class CharT>
class CType :
	public std::ctype <CharT>
{
public:

	explicit
	CType (size_t = 0);

	virtual
	bool
	do_is (std::ctype_base::mask, CharT) const;

	//	const CharT*
	//	do_is (const CharT* low, const CharT* high, std::ctype_base::mask* vec) const;

	static const std::ctype_base::mask*
	table ();

};

///////////////////////////////////////////////////////////////////////////////

template <>
class CType <char>:
	public std::ctype <char>
{
public:

	typedef typename std::ctype <char>::mask mask;

	explicit
	CType (size_t = 0);


protected:

	static bool
	is_space (char);

	//	static bool
	//	is_id_first_char (char);
	//
	//	static bool
	//	is_id_rest_char (char);


private:

	static std::vector <mask>
	create_table ();

	static const std::vector <mask> table;

	template <class CharT>
	friend class CType;

};

///////////////////////////////////////////////////////////////////////////////

template <class CharT>
CType <CharT>::CType (size_t refs) :
	std::ctype <CharT> (refs)
{ }

template <class CharT>
bool
CType <CharT>::do_is (std::ctype_base::mask mask, CharT c) const
{
	static constexpr size_t MAX_ASCII_CHAR = 0x7F;

	if (static_cast <size_t> (c) >= 0 and static_cast <size_t> (c) <= MAX_ASCII_CHAR)
		return mask & table () [c];

	return std::ctype <CharT>::do_is (mask, c);
}

template <class CharT>
inline
const std::ctype_base::mask*
CType <CharT>::table ()
{
	return CType <char>::table .data ();
}

///////////////////////////////////////////////////////////////////////////////

extern template class CType <char>;
extern template class CType <wchar_t>;

} // ClassicVrml
} // Encodings
} // X3D
} // titania

#endif

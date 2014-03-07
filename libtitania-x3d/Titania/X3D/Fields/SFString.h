/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_FIELDS_SFSTRING_H__
#define __TITANIA_X3D_FIELDS_SFSTRING_H__

#include "../Basic/X3DField.h"
#include "../Types/String.h"

namespace titania {
namespace X3D {

extern template class X3DField <String>;

class SFString :
	public X3DField <String>
{
public:

	typedef std::string::value_type   char_type;
	typedef internal_type::value_type value_type;
	typedef internal_type::size_type  size_type;

	using X3DField <String>::addInterest;
	using X3DField <String>::operator =;

	SFString ();

	SFString (const SFString &);

	explicit
	SFString (const String &);

	explicit
	SFString (const char_type* const);

	virtual
	SFString*
	create () const final override
	{ return new SFString (); }

	virtual
	SFString*
	clone () const
	throw (Error <NOT_SUPPORTED>) final override
	{ return new SFString (*this); }

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const SFString &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFString &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  Functions

	SFString &
	operator = (const char_type* const);

	operator const std::string & () const;

	const std::string &
	str () const;

	const char_type*
	c_str () const;

	bool
	empty () const;

	size_type
	length () const;

	///  @name Input operator.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Output operator.
	virtual
	void
	toStream (std::ostream &) const final override;

};

//@{
inline
bool
operator == (const SFString & lhs, const char* rhs)
{
	return lhs .getValue () == rhs;
}

inline
bool
operator == (const char* lhs, const SFString & rhs)
{
	return lhs == rhs .getValue ();
}
//@}

} // X3D
} // titania

#endif

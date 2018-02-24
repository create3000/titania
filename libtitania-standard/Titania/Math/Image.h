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

#ifndef __TITANIA_MATH_IMAGE_H__
#define __TITANIA_MATH_IMAGE_H__

#include <cstddef>
#include <cstdint>
#include <utility>

namespace titania {
namespace math {

template <class Array>
class image
{
public:

	///  @name Member types

	using array_type = Array;
	using size_type  = size_t;

	///  @name Construction

	image () :
		     m_width (0),
		    m_height (0),
		m_components (0),
		     m_array ()
	{ }

	image (const image & img) :
		     m_width (img .width ()),
		    m_height (img .height ()),
		m_components (img .components ()),
		     m_array (img .array ())
	{ }

	image (image && img) :
		     m_width (img .width ()),
		    m_height (img .height ()),
		m_components (img .components ()),
		     m_array (std::move (img .array ()))
	{
		img .check_resize ();
	}

	image (const size_type width, const size_type height, const size_type components, const Array & array) :
		     m_width (width),
		    m_height (height),
		m_components (components),
		     m_array (array)
	{ check_resize (); }

	image (const size_type width, const size_type height, const size_type components, Array && array) :
		     m_width (width),
		    m_height (height),
		m_components (components),
		     m_array (std::move (array))
	{ check_resize (); }

	///  @name Assignment operators

	image &
	operator = (const image & other);

	image &
	operator = (image && other);

	///  @name Member access

	void
	width (const size_type value);

	size_type
	width () const
	{ return m_width; }

	void
	height (const size_type value);

	size_type
	height () const
	{ return m_height; }

	void
	components (const size_type components)
	{ m_components = components; }

	size_type
	components () const
	{ return m_components; }

	void
	array (const Array & array);

	Array &
	array ()
	{ return m_array; }

	const Array &
	array () const
	{ return m_array; }

	void
	set (const size_type width, const size_type height, const size_type components, const Array & array);

	void
	set (const size_type width, const size_type height, const size_type components, Array && array);

	void
	get (size_type & width, size_type & height, size_type & components, Array & array) const;


private:

	///  @name Operations

	void
	check_resize ()
	{ m_array .resize (m_width * m_height); }

	///  @name Member access

	size_type m_width;
	size_type m_height;
	size_type m_components;
	Array     m_array;

};

template <class Array>
image <Array> &
image <Array>::operator = (const image <Array> & img)
{
	m_width      = img .width ();
	m_height     = img .height ();
	m_components = img .components ();
	m_array      = img .array ();

	return *this;
}

template <class Array>
image <Array> &
image <Array>::operator = (image <Array> && img)
{
	m_width      = img .width ();
	m_height     = img .height ();
	m_components = img .components ();
	m_array      = std::move (img .array ());

	img .m_width      = 0;
	img .m_height     = 0;
	img .m_components = 0;

	img .check_resize ();

	return *this;
}

template <class Array>
void
image <Array>::set (const size_type width, const size_type height, const size_type components, const Array & array)
{
	m_width      = width;
	m_height     = height;
	m_components = components;
	m_array      = array;

	check_resize ();
}

template <class Array>
void
image <Array>::set (const size_type width, const size_type height, const size_type components, Array && array)
{
	m_width      = width;
	m_height     = height;
	m_components = components;
	m_array      = std::move (array);

	check_resize ();
}

template <class Array>
void
image <Array>::get (size_type & width, size_type & height, size_type & components, Array & array) const
{
	width      = m_width;
	height     = m_height;
	components = m_components;
	array      = m_array;
}

template <class Array>
void
image <Array>::width (const size_type width)
{
	m_width = width;

	check_resize ();
}

template <class Array>
void
image <Array>::height (const size_type height)
{
	m_height = height;

	check_resize ();
}

template <class Array>
void
image <Array>::array (const Array & array)
{
	m_array = array;

	check_resize ();
}

template <class Array>
bool
operator == (const image <Array> & a, const image <Array> & b)
{
	return
	   a .width  () == b .width  () and
	   a .height () == b .height () and
	   a .components () == b .components () and
	   a .array  () == b .array  ();
}

template <class Array>
inline
bool
operator not_eq (const image <Array> & a, const image <Array> & b)
{
	return not (a == b);
}

} // math
} // titania

#endif

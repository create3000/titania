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
 ******************************************************************************/

#ifndef __TITANIA_MATH_IMAGE_H__
#define __TITANIA_MATH_IMAGE_H__

#include <cstddef>
#include <cstdint>
#include <valarray>

namespace titania {
namespace math {

template <class Array>
class image
{
public:

	typedef Array  array_type;
	typedef size_t size_type;

	image () :
		value  ({ 0, 0, 0, Array () })
	{ }

	image (const image & img) :
		value  (img .value)
	{ }

	image (const size_type width, const size_type height, const size_type components, const Array & array) :
		value  ({ width, height, components, array })
	{
		resize ();
	}

	void
	width (const size_type);

	size_type
	width () const { return value .width; }

	void
	height (const size_type);

	size_type
	height () const { return value .height; }

	void
	components (const size_type components) { value .components = components; }

	size_type
	components () const { return value .components; }

	void
	array (const Array &);

	Array &
	array () { return value .array; }

	const Array &
	array () const { return value .array; }

	void
	set (const size_type, const size_type, const size_type, const Array &);

	void
	get (size_type &, size_type &, size_type &, Array &) const;

	image &
	operator = (const image &);


private:

	void
	resize ()
	{
		value .array .resize (value .width * value .height);
	}

	struct Value
	{
		size_type width;
		size_type height;
		size_type components;

		Array array;
	};

	Value value;

};

template <class Array>
void
image <Array>::width (const size_type width)
{
	value .width = width;

	resize ();
}

template <class Array>
void
image <Array>::height (const size_type height)
{
	value .height = height;

	resize ();
}

template <class Array>
void
image <Array>::array (const Array & array)
{
	value .array = array;

	resize ();
}

template <class Array>
void
image <Array>::set (const size_type width, const size_type height, const size_type components, const Array & array)
{
	value .width      = width;
	value .height     = height;
	value .components = components;
	value .array      = array;

	resize ();
}

template <class Array>
void
image <Array>::get (size_type & width, size_type & height, size_type & components, Array & array) const
{
	width      = value .width;
	height     = value .height;
	components = value .components;
	array      = value .array;
}

template <class Array>
image <Array> &
image <Array>::operator = (const image  <Array> & img)
{
	value .width      = img .width  ();
	value .height     = img .height ();
	value .components = img .components ();
	value .array      = img .array  ();

	return *this;
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

extern template class image <std::valarray <int32_t>>;

} // math
} // titania

#endif

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

#ifndef __TITANIA_MATH_NUMBERS_HASH_H__
#define __TITANIA_MATH_NUMBERS_HASH_H__

#include "Color3.h"
#include "Color4.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Rotation4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace std {

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <class Type>
struct array_hash { };

template <>
struct array_hash <double>
{
	using result_type = std::size_t;

	template <class Iterator>
	result_type
	operator () (const Iterator & first, const Iterator & last) const
	{
		union IF
		{
			uint64_t i;
			double f;
		};

		result_type h = 1;

		for (const auto & value : std::pair (first, last))
		{
			IF f = { .f = value };

			h = 31 * h + f .i;
		}

		h ^= (h >> 20) ^ (h >> 12);
		h ^= (h >> 7) ^ (h >> 4);

		return h;
	}

};

template <>
struct array_hash <float>
{
	using result_type = std::size_t;

	template <class Iterator>
	result_type
	operator () (const Iterator & first, const Iterator & last) const
	{
		union IF
		{
			uint32_t i;
			float f;
		};

		result_type h = 1;

		for (const auto & value : std::pair (first, last))
		{
			IF f = { .f = value };

			h = 31 * h + f .i;
		}

		h ^= (h >> 20) ^ (h >> 12);
		h ^= (h >> 7) ^ (h >> 4);

		return h;
	}

};

template <>
struct array_hash <int32_t>
{
	using result_type = std::size_t;

	template <class Iterator>
	result_type
	operator () (const Iterator & first, const Iterator & last) const
	{
		union IF
		{
			uint32_t i;
			int32_t f;
		};

		result_type h = 1;

		for (const auto & value : std::pair (first, last))
		{
			IF f = { .f = value };

			h = 31 * h + f .i;
		}

		h ^= (h >> 20) ^ (h >> 12);
		h ^= (h >> 7) ^ (h >> 4);

		return h;
	}

};

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

template <>
struct hash <titania::math::color3 <double>> 
{
	using argument_type = titania::math::color3 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::color3 <float>> 
{
	using argument_type = titania::math::color3 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::color4 <double>> 
{
	using argument_type = titania::math::color4 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::color4 <float>> 
{
	using argument_type = titania::math::color4 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::matrix2 <double>> 
{
	using argument_type = titania::math::matrix2 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type::value_type> () (array .front () .data (), array .front () .data () + (array .rows () * array .columns ()));
	}

};

template <>
struct hash <titania::math::matrix2 <float>> 
{
	using argument_type = titania::math::matrix2 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type::value_type> () (array .front () .data (), array .front () .data () + (array .rows () * array .columns ()));
	}

};

template <>
struct hash <titania::math::matrix3 <double>> 
{
	using argument_type = titania::math::matrix3 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type::value_type> () (array .front () .data (), array .front () .data () + (array .rows () * array .columns ()));
	}

};

template <>
struct hash <titania::math::matrix3 <float>> 
{
	using argument_type = titania::math::matrix3 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type::value_type> () (array .front () .data (), array .front () .data () + (array .rows () * array .columns ()));
	}

};

template <>
struct hash <titania::math::matrix4 <double>> 
{
	using argument_type = titania::math::matrix4 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type::value_type> () (array .front () .data (), array .front () .data () + (array .rows () * array .columns ()));
	}

};

template <>
struct hash <titania::math::matrix4 <float>> 
{
	using argument_type = titania::math::matrix4 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type::value_type> () (array .front () .data (), array .front () .data () + (array .rows () * array .columns ()));
	}

};

template <>
struct hash <titania::math::quaternion <double>> 
{
	using argument_type = titania::math::quaternion <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::quaternion <float>> 
{
	using argument_type = titania::math::quaternion <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::vector2 <double>> 
{
	using argument_type = titania::math::vector2 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::vector2 <float>> 
{
	using argument_type = titania::math::vector2 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::vector3 <double>> 
{
	using argument_type = titania::math::vector3 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::vector3 <float>> 
{
	using argument_type = titania::math::vector3 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::vector4 <double>> 
{
	using argument_type = titania::math::vector4 <double>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

template <>
struct hash <titania::math::vector4 <float>> 
{
	using argument_type = titania::math::vector4 <float>;
	using result_type   = std::size_t;

	result_type
	operator () (const argument_type & array) const
	{
		return array_hash <typename argument_type::value_type> () (array .begin (), array .end ());
	}

};

} // std

#endif

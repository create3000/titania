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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_X3DSCALAR_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_X3DSCALAR_H__

#include "../String.h"

#include "../../../Fields/SFString.h"
#include "../../../Fields/SFTime.h"
#include "../../../Fields/X3DScalar.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

// Template class for JavaScript build-in field types.

template <class InternalType>
class X3DScalar
{
public:

	///  @name Member types

	using internal_type = InternalType;

	///  @name Construction

	static
	JS::Value
	create (JSContext* const cx, const internal_type* const field)
	{
		throw std::runtime_error ("X3DScalar::create");
	}

};

template <>
inline
JS::Value
X3DScalar <X3D::SFBool>::create (JSContext* const cx, const internal_type* const field)
{
	return JS::BooleanValue (field -> getValue ());
}

template <>
inline
JS::Value
X3DScalar <X3D::SFDouble>::create (JSContext* const cx, const internal_type* const field)
{
	return JS::DoubleValue (field -> getValue ());
}

template <>
inline
JS::Value
X3DScalar <X3D::SFFloat>::create (JSContext* const cx, const internal_type* const field)
{
	return JS::DoubleValue (field -> getValue ());
}

template <>
inline
JS::Value
X3DScalar <X3D::SFInt32>::create (JSContext* const cx, const internal_type* const field)
{
	return JS::Int32Value (field -> getValue ());
}

template <>
inline
JS::Value
X3DScalar <X3D::SFString>::create (JSContext* const cx, const internal_type* const field)
{
	return StringValue (cx, field -> getValue ());
}

template <>
inline
JS::Value
X3DScalar <X3D::SFTime>::create (JSContext* const cx, const internal_type* const field)
{
	return JS::DoubleValue (field -> getValue ());
}

using SFBool   = X3DScalar <X3D::SFBool>;
using SFDouble = X3DScalar <X3D::SFDouble>;
using SFFloat  = X3DScalar <X3D::SFFloat>;
using SFInt32  = X3DScalar <X3D::SFInt32>;
using SFString = X3DScalar <X3D::SFString>;
using SFTime   = X3DScalar <X3D::SFTime>;

} // spidermonkey
} // X3D
} // titania

#endif

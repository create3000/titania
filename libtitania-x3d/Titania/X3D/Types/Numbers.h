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

#ifndef __TITANIA_X3D_TYPES_NUMBERS_H__
#define __TITANIA_X3D_TYPES_NUMBERS_H__

#include "../Types/Time.h"
#include <Titania/Math/Numbers/Color3.h>
#include <Titania/Math/Numbers/Color4.h>
#include <Titania/Math/Numbers/Matrix3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Quaternion.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <cstdint>
#include <complex>
#include <string>

namespace titania {
namespace X3D {

using namespace titania::math;

using Color3d = color3 <double>;
using Color3f = color3 <float>;

using Color4d = color4 <double>;
using Color4f = color4 <float>;

// Image is definied in Image.h because it needs MFInt32

using Matrix3d = matrix3 <double>;
using Matrix3f = matrix3 <float>;

using Matrix4d = matrix4 <double>;
using Matrix4f = matrix4 <float>;

using Quaternion4d = quaternion <double>;
using Quaternion4f = quaternion <float>;

using Rotation4d = rotation4 <double>;
using Rotation4f = rotation4 <float>;

// time_type is defined in Time.h
// String is defined in String.h

using Vector2d = vector2 <double>;
using Vector2f = vector2 <float>;
using Vector2i = vector2 <int32_t>;

using Vector3d = vector3 <double>;
using Vector3f = vector3 <float>;
using Vector3i = vector3 <int32_t>;

using Vector4d = vector4 <double>;
using Vector4f = vector4 <float>;
using Vector4i = vector4 <int32_t>;

// Templates

template <class Type>
using Matrix3 = matrix3 <Type>;

template <class Type>
using Matrix4 = matrix4 <Type>;

template <class Type>
using Vector2 = vector2 <Type>;

template <class Type>
using Vector3 = vector3 <Type>;

template <class Type>
using Vector4 = vector4 <Type>;

template <class T>
std::complex <T>
polar (const T & r, const T & theta = T ())
{
	return std::complex (r * std::cos (theta), r * std::sin (theta));
}

} // X3D
} // titania

#endif

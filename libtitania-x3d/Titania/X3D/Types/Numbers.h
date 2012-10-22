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

#ifndef __TITANIA_X3D_TYPES_NUMBERS_H__
#define __TITANIA_X3D_TYPES_NUMBERS_H__

#include "../Types/Time.h"
#include <Titania/Math/Numbers/Color3.h>
#include <Titania/Math/Numbers/Color4.h>
#include <Titania/Math/Numbers/Matrix3.h>
#include <Titania/Math/Numbers/Matrix4.h>
#include <Titania/Math/Numbers/Rotation4.h>
#include <Titania/Math/Numbers/Vector2.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>
#include <cstdint>
#include <string>

namespace titania {
namespace X3D {

typedef bool         Bool;
typedef double       Double;
typedef float        Float;
typedef std::int32_t Int32;

typedef math::color3  <Double> Color3d;
typedef math::color3  <Float>  Color3f;

typedef math::color4 <Double> Color4d;
typedef math::color4 <Float>  Color4f;

// Image is definied in Image.h because it needs MFInt32

typedef math::matrix3 <Double> Matrix3d;
typedef math::matrix3 <Float>  Matrix3f;

typedef math::matrix4 <Double> Matrix4d;
typedef math::matrix4 <Float>  Matrix4f;

typedef math::rotation4 <Double> Rotation4d;
typedef math::rotation4 <Float>  Rotation4f;

typedef std::string String;

// time_type is defined in Time.h

typedef math::vector2 <Double> Vector2d;
typedef math::vector2 <Float>  Vector2f;

typedef math::vector3 <Double> Vector3d;
typedef math::vector3 <Float>  Vector3f;

typedef math::vector4 <Double> Vector4d;
typedef math::vector4 <Float>  Vector4f;

} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_FIELDS_NATIVE_ARRAY_FIELDS_H__
#define __TITANIA_X3D_FIELDS_NATIVE_ARRAY_FIELDS_H__

#include "../Basic/X3DNativeArrayField.h"
#include "../Types/Numbers.h"
#include "../Types/String.h"
#include "../Types/Time.h"

namespace titania {
namespace X3D {

#ifdef __APPLE__
// typeName

template <>
const std::string X3DField <Array <bool>>::typeName;

template <>
const std::string X3DField <Array <Color3f>>::typeName;

template <>
const std::string X3DField <Array <Color4f>>::typeName;

template <>
const std::string X3DField <Array <double>>::typeName;

template <>
const std::string X3DField <Array <float>>::typeName;

//template <>
//const std::string X3DField <Array <SFImage*>>::typeName;

template <>
const std::string X3DField <Array <Matrix3d>>::typeName;

template <>
const std::string X3DField <Array <Matrix3f>>::typeName;

template <>
const std::string X3DField <Array <Matrix4d>>::typeName;

template <>
const std::string X3DField <Array <Matrix4f>>::typeName;

template <>
const std::string X3DField <Array <Rotation4d>>::typeName;

template <>
const std::string X3DField <Array <String>>::typeName;

template <>
const std::string X3DField <Array <Vector2d>>::typeName;

template <>
const std::string X3DField <Array <Vector2f>>::typeName;

template <>
const std::string X3DField <Array <Vector3d>>::typeName;

template <>
const std::string X3DField <Array <Vector3f>>::typeName;

template <>
const std::string X3DField <Array <Vector4d>>::typeName;

template <>
const std::string X3DField <Array <Vector4f>>::typeName;

// type

template <>
const FieldType X3DField <Array <bool>>::type;

template <>
const FieldType X3DField <Array <Color3f>>::type;

template <>
const FieldType X3DField <Array <Color4f>>::type;

template <>
const FieldType X3DField <Array <double>>::type;

template <>
const FieldType X3DField <Array <float>>::type;

//template <>
//const FieldType X3DField <Array <SFImage*>>::type;

template <>
const FieldType X3DField <Array <Matrix3d>>::type;

template <>
const FieldType X3DField <Array <Matrix3f>>::type;

template <>
const FieldType X3DField <Array <Matrix4d>>::type;

template <>
const FieldType X3DField <Array <Matrix4f>>::type;

template <>
const FieldType X3DField <Array <Rotation4d>>::type;

template <>
const FieldType X3DField <Array <String>>::type;

template <>
const FieldType X3DField <Array <Vector2d>>::type;

template <>
const FieldType X3DField <Array <Vector2f>>::type;

template <>
const FieldType X3DField <Array <Vector3d>>::type;

template <>
const FieldType X3DField <Array <Vector3f>>::type;

template <>
const FieldType X3DField <Array <Vector4d>>::type;

template <>
const FieldType X3DField <Array <Vector4f>>::type;
#endif

// Inhibit implicit instantiation for required instantiations.
extern template class X3DField <Array <bool>> ;
extern template class X3DField <Array <Color3f>> ;
extern template class X3DField <Array <Color4f>> ;
extern template class X3DField <Array <double>> ;
extern template class X3DField <Array <float>> ;
//extern template class X3DField <Array <SFImage*>> ;
extern template class X3DField <Array <Matrix3d>> ;
extern template class X3DField <Array <Matrix3f>> ;
extern template class X3DField <Array <Matrix4d>> ;
extern template class X3DField <Array <Matrix4f>> ;
extern template class X3DField <Array <Rotation4d>> ;
extern template class X3DField <Array <String>> ;
extern template class X3DField <Array <Vector2d>> ;
extern template class X3DField <Array <Vector2f>> ;
extern template class X3DField <Array <Vector3d>> ;
extern template class X3DField <Array <Vector3f>> ;
extern template class X3DField <Array <Vector4d>> ;
extern template class X3DField <Array <Vector4f>> ;

template <>
void
X3DNativeArrayField <String>::toXMLStream (std::ostream &) const;

// Inhibit implicit instantiation for required instantiations.
extern template class X3DNativeArrayField <bool>;
extern template class X3DNativeArrayField <Color3f>;
extern template class X3DNativeArrayField <Color4f>;
extern template class X3DNativeArrayField <double>;
extern template class X3DNativeArrayField <float>;
//extern template class X3DNativeArrayField <SFImage>;
extern template class X3DNativeArrayField <Matrix3d>;
extern template class X3DNativeArrayField <Matrix3f>;
extern template class X3DNativeArrayField <Matrix4d>;
extern template class X3DNativeArrayField <Matrix4f>;
extern template class X3DNativeArrayField <Rotation4d>;
extern template class X3DNativeArrayField <String>;
extern template class X3DNativeArrayField <Vector2d>;
extern template class X3DNativeArrayField <Vector2f>;
extern template class X3DNativeArrayField <Vector3d>;
extern template class X3DNativeArrayField <Vector3f>;
extern template class X3DNativeArrayField <Vector4d>;
extern template class X3DNativeArrayField <Vector4f>;

// Typedefs for standard X3DNativeArrayFields
using MFBool      = X3DNativeArrayField <bool>;
using MFColor     = X3DNativeArrayField <Color3f>;
using MFColorRGBA = X3DNativeArrayField <Color4f>;
using MFDouble    = X3DNativeArrayField <double>;
using MFFloat     = X3DNativeArrayField <float>;
////using MFImage     = X3DNativeArrayField <SFImage>;
using MFMatrix3d  = X3DNativeArrayField <Matrix3d>;
using MFMatrix3f  = X3DNativeArrayField <Matrix3f>;
using MFMatrix4d  = X3DNativeArrayField <Matrix4d>;
using MFMatrix4f  = X3DNativeArrayField <Matrix4f>;
using MFRotation  = X3DNativeArrayField <Rotation4d>;
using MFString    = X3DNativeArrayField <String>;
using MFVec2d     = X3DNativeArrayField <Vector2d>;
using MFVec2f     = X3DNativeArrayField <Vector2f>;
using MFVec3d     = X3DNativeArrayField <Vector3d>;
using MFVec3f     = X3DNativeArrayField <Vector3f>;
using MFVec4d     = X3DNativeArrayField <Vector4d>;
using MFVec4f     = X3DNativeArrayField <Vector4f>;

template <class Type>
using MFEnum = X3DNativeArrayField <Type>;

} // X3D
} // titania

#endif

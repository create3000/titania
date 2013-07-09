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

#ifndef __TITANIA_X3D_BITS_TYPE_H__
#define __TITANIA_X3D_BITS_TYPE_H__

#include "../Bits/X3DConstants.h"
#include "../Bits/Error.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

template <class FieldType, class ValueType>
void
X3DFieldDefinition::setValue (const ValueType & value)
{
	if (getType () == Type <FieldType> ())
		static_cast <FieldType*> (this) -> setValue (value);

	else
		throw Error <INVALID_FIELD> ("X3DFieldDefinition: Couldn't set value, field types does not match.");
}

template <class FieldType, class ValueType>
const typename FieldType::value_type &
X3DFieldDefinition::getValue () const
{
	if (getType () == Type <FieldType> ())
		return static_cast <FieldType*> (this) -> getValue ();

	else
		throw Error <INVALID_FIELD> ("X3DFieldDefinition: Couldn't get value, field types does not match.");
}

class X3DBaseNode;

template <class Type>
class X3DScalar;

class SFColor;
class SFColorRGBA;
class SFImage;

template <class Type>
class SFMatrix3;

template <class Type>
class SFMatrix4;

template <class Type>
class SFNode;

template <class Type>
class SFRotation4;

class SFString;
class SFTime;

template <class Type>
class SFVec2;

template <class Type>
class SFVec3;

template <class Type>
class SFVec4;

template <class Type>
class X3DArrayField;

template <class T>
inline
X3DConstants::FieldType
Type ()
{
	throw Error <INVALID_FIELD> ("Type <T> () is not supported!");
}

template <>
inline
X3DConstants::FieldType
Type <X3DScalar <bool>> ()
{
	return X3DConstants::SFBool;
}

template <>
inline
X3DConstants::FieldType
Type <SFColor> ()
{
	return X3DConstants::SFColor;
}

template <>
inline
X3DConstants::FieldType
Type <SFColorRGBA> ()
{
	return X3DConstants::SFColorRGBA;
}

template <>
inline
X3DConstants::FieldType
Type <X3DScalar <double>> ()
{
	return X3DConstants::SFDouble;
}

template <>
inline
X3DConstants::FieldType
Type <X3DScalar <float>> ()
{
	return X3DConstants::SFFloat;
}

template <>
inline
X3DConstants::FieldType
Type <SFImage> ()
{
	return X3DConstants::SFImage;
}

template <>
inline
X3DConstants::FieldType
Type <X3DScalar <int32_t>> ()
{
	return X3DConstants::SFInt32;
}

template <>
inline
X3DConstants::FieldType
Type <SFMatrix3 <Matrix3d>> ()
{
	return X3DConstants::SFMatrix3d;
}

template <>
inline
X3DConstants::FieldType
Type <SFMatrix3 <Matrix3f>> ()
{
	return X3DConstants::SFMatrix3f;
}

template <>
inline
X3DConstants::FieldType
Type <SFMatrix4 <Matrix4d>> ()
{
	return X3DConstants::SFMatrix4d;
}

template <>
inline
X3DConstants::FieldType
Type <SFMatrix4 <Matrix4f>> ()
{
	return X3DConstants::SFMatrix4f;
}

template <>
inline
X3DConstants::FieldType
Type <SFNode <X3DBaseNode>> ()
{
	return X3DConstants::SFNode;
}

template <>
inline
X3DConstants::FieldType
Type <SFRotation4 <Rotation4f>> ()
{
	return X3DConstants::SFRotation;
}

template <>
inline
X3DConstants::FieldType
Type <SFString> ()
{
	return X3DConstants::SFString;
}

template <>
inline
X3DConstants::FieldType
Type <SFTime> ()
{
	return X3DConstants::SFTime;
}

template <>
inline
X3DConstants::FieldType
Type <SFVec2 <Vector2d>> ()
{
	return X3DConstants::SFVec2d;
}

template <>
inline
X3DConstants::FieldType
Type <SFVec2 <Vector2f>> ()
{
	return X3DConstants::SFVec2f;
}

template <>
inline
X3DConstants::FieldType
Type <SFVec3 <Vector3d>> ()
{
	return X3DConstants::SFVec3d;
}

template <>
inline
X3DConstants::FieldType
Type <SFVec3 <Vector3f>> ()
{
	return X3DConstants::SFVec3f;
}

template <>
inline
X3DConstants::FieldType
Type <SFVec4 <Vector4d>> ()
{
	return X3DConstants::SFVec4d;
}

template <>
inline
X3DConstants::FieldType
Type <SFVec4 <Vector4f>> ()
{
	return X3DConstants::SFVec4f;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <X3DScalar <bool>>> ()
{
	return X3DConstants::MFBool;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <SFColor>> ()
{
	return X3DConstants::MFColor;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <SFColorRGBA>> ()
{
	return X3DConstants::MFColorRGBA;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <X3DScalar <double>>> ()
{
	return X3DConstants::MFDouble;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <X3DScalar <float>>> ()
{
	return X3DConstants::MFFloat;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <SFImage>> ()
{
	return X3DConstants::MFImage;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <X3DScalar <int32_t>>> ()
{
	return X3DConstants::MFInt32;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Matrix3d>> ()
{
	return X3DConstants::MFMatrix3d;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Matrix3f>> ()
{
	return X3DConstants::MFMatrix3f;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Matrix4d>> ()
{
	return X3DConstants::MFMatrix4d;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Matrix4f>> ()
{
	return X3DConstants::MFMatrix4f;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <SFNode <X3DBaseNode>>> ()
{
	return X3DConstants::MFNode;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <SFRotation4 <Rotation4f>>> ()
{
	return X3DConstants::MFRotation;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <SFString>> ()
{
	return X3DConstants::MFString;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <SFTime>> ()
{
	return X3DConstants::MFTime;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Vector2d>> ()
{
	return X3DConstants::MFVec2d;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Vector2f>> ()
{
	return X3DConstants::MFVec2f;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Vector3d>> ()
{
	return X3DConstants::MFVec3d;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Vector3f>> ()
{
	return X3DConstants::MFVec3f;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Vector4d>> ()
{
	return X3DConstants::MFVec4d;
}

template <>
inline
X3DConstants::FieldType
Type <X3DArrayField <Vector4f>> ()
{
	return X3DConstants::MFVec4f;
}

} // X3D
} // titania

#endif

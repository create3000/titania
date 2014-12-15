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

#include "ArrayFields.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

// typeName

template <>
const std::string X3DObject <X3D::MFBool>::typeName = "MFBool";

template <>
const std::string X3DObject <X3D::MFColor>::typeName = "MFColor";

template <>
const std::string X3DObject <X3D::MFColorRGBA>::typeName = "MFColorRGBA";

template <>
const std::string X3DObject <X3D::MFDouble>::typeName = "MFDouble";

template <>
const std::string X3DObject <X3D::MFFloat>::typeName = "MFFloat";

template <>
const std::string X3DObject <X3D::MFImage>::typeName = "MFImage";

template <>
const std::string X3DObject <X3D::MFInt32>::typeName = "MFInt32";

template <>
const std::string X3DObject <X3D::MFMatrix3d>::typeName = "MFMatrix3d";

template <>
const std::string X3DObject <X3D::MFMatrix3f>::typeName = "MFMatrix3f";

template <>
const std::string X3DObject <X3D::MFMatrix4d>::typeName = "MFMatrix4d";

template <>
const std::string X3DObject <X3D::MFMatrix4f>::typeName = "MFMatrix4f";

template <>
const std::string X3DObject <X3D::MFNode>::typeName = "MFNode";

template <>
const std::string X3DObject <X3D::MFRotation>::typeName = "MFRotation";

template <>
const std::string X3DObject <X3D::MFString>::typeName = "MFString";

template <>
const std::string X3DObject <X3D::MFTime>::typeName = "MFTime";

template <>
const std::string X3DObject <X3D::MFVec2d>::typeName = "MFVec2d";

template <>
const std::string X3DObject <X3D::MFVec2f>::typeName = "MFVec2f";

template <>
const std::string X3DObject <X3D::MFVec3d>::typeName = "MFVec3d";

template <>
const std::string X3DObject <X3D::MFVec3f>::typeName = "MFVec3f";

template <>
const std::string X3DObject <X3D::MFVec4d>::typeName = "MFVec4d";

template <>
const std::string X3DObject <X3D::MFVec4f>::typeName = "MFVec4f";

// type

template <>
const ObjectType X3DObject <X3D::MFBool>::type = ObjectType::MFBool;

template <>
const ObjectType X3DObject <X3D::MFColor>::type = ObjectType::MFColor;

template <>
const ObjectType X3DObject <X3D::MFColorRGBA>::type = ObjectType::MFColorRGBA;

template <>
const ObjectType X3DObject <X3D::MFDouble>::type = ObjectType::MFDouble;

template <>
const ObjectType X3DObject <X3D::MFFloat>::type = ObjectType::MFFloat;

template <>
const ObjectType X3DObject <X3D::MFImage>::type = ObjectType::MFImage;

template <>
const ObjectType X3DObject <X3D::MFInt32>::type = ObjectType::MFInt32;

template <>
const ObjectType X3DObject <X3D::MFMatrix3d>::type = ObjectType::MFMatrix3d;

template <>
const ObjectType X3DObject <X3D::MFMatrix3f>::type = ObjectType::MFMatrix3f;

template <>
const ObjectType X3DObject <X3D::MFMatrix4d>::type = ObjectType::MFMatrix4d;

template <>
const ObjectType X3DObject <X3D::MFMatrix4f>::type = ObjectType::MFMatrix4f;

template <>
const ObjectType X3DObject <X3D::MFNode>::type = ObjectType::MFNode;

template <>
const ObjectType X3DObject <X3D::MFRotation>::type = ObjectType::MFRotation;

template <>
const ObjectType X3DObject <X3D::MFString>::type = ObjectType::MFString;

template <>
const ObjectType X3DObject <X3D::MFTime>::type = ObjectType::MFTime;

template <>
const ObjectType X3DObject <X3D::MFVec2d>::type = ObjectType::MFVec2d;

template <>
const ObjectType X3DObject <X3D::MFVec2f>::type = ObjectType::MFVec2f;

template <>
const ObjectType X3DObject <X3D::MFVec3d>::type = ObjectType::MFVec3d;

template <>
const ObjectType X3DObject <X3D::MFVec3f>::type = ObjectType::MFVec3f;

template <>
const ObjectType X3DObject <X3D::MFVec4d>::type = ObjectType::MFVec4d;

template <>
const ObjectType X3DObject <X3D::MFVec4f>::type = ObjectType::MFVec4f;

// set1Value

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFBool, SFBool>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		getObject (info) -> set1Value (index, value -> ToBoolean () -> Value ());

		return value;
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFFloat, SFFloat>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		getObject (info) -> set1Value (index, value -> ToNumber () -> Value ());

		return value;
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFDouble, SFDouble>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		getObject (info) -> set1Value (index, value -> ToNumber () -> Value ());

		return value;
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFInt32, SFInt32>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		getObject (info) -> set1Value (index, value -> ToInt32 () -> Value ());

		return value;
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFString, SFString>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		getObject (info) -> set1Value (index, to_string (value));

		return value;
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFTime, SFTime>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		getObject (info) -> set1Value (index, value -> ToNumber () -> Value ());

		return value;
	}
	catch (const std::bad_alloc &)
	{
		return v8::ThrowException (String ("RuntimeError: out of memory."));
	}
}

// Extern instantiations

template class X3DArrayField <X3D::MFBool, SFBool>;
template class X3DArrayField <X3D::MFColor, SFColor>;
template class X3DArrayField <X3D::MFColorRGBA, SFColorRGBA>;
template class X3DArrayField <X3D::MFDouble, SFDouble>;
template class X3DArrayField <X3D::MFFloat, SFFloat>;
template class X3DArrayField <X3D::MFImage, SFImage>;
template class X3DArrayField <X3D::MFInt32, SFInt32>;
template class X3DArrayField <X3D::MFMatrix3d, SFMatrix3d>;
template class X3DArrayField <X3D::MFMatrix3f, SFMatrix3f>;
template class X3DArrayField <X3D::MFMatrix4d, SFMatrix4d>;
template class X3DArrayField <X3D::MFMatrix4f, SFMatrix4f>;
template class X3DArrayField <X3D::MFNode, SFNode>;
template class X3DArrayField <X3D::MFRotation, SFRotation>;
template class X3DArrayField <X3D::MFString, SFString>;
template class X3DArrayField <X3D::MFTime, SFTime>;
template class X3DArrayField <X3D::MFVec2d, SFVec2d>;
template class X3DArrayField <X3D::MFVec2f, SFVec2f>;
template class X3DArrayField <X3D::MFVec3d, SFVec3d>;
template class X3DArrayField <X3D::MFVec3f, SFVec3f>;
template class X3DArrayField <X3D::MFVec4d, SFVec4d>;
template class X3DArrayField <X3D::MFVec4f, SFVec4f>;

} // GoogleV8
} // X3D
} // titania

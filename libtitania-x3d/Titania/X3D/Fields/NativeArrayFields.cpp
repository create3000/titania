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

#include "NativeArrayFields.h"

namespace titania {
namespace X3D {

// typeName

template <>
const std::string X3DField <Array <bool>>::typeName = "MFBool";

template <>
const std::string X3DField <Array <Color3f>>::typeName = "MFColor";

template <>
const std::string X3DField <Array <Color4f>>::typeName = "MFColorRGBA";

template <>
const std::string X3DField <Array <double>>::typeName = "MFDouble";

template <>
const std::string X3DField <Array <float>>::typeName = "MFFloat";

//template <>
//const std::string X3DField <Array <SFImage*>>::typeName = "MFImage";

template <>
const std::string X3DField <Array <Matrix3d>>::typeName = "MFMatrix3d";

template <>
const std::string X3DField <Array <Matrix3f>>::typeName = "MFMatrix3f";

template <>
const std::string X3DField <Array <Matrix4d>>::typeName = "MFMatrix4d";

template <>
const std::string X3DField <Array <Matrix4f>>::typeName = "MFMatrix4f";

template <>
const std::string X3DField <Array <Rotation4d>>::typeName = "MFRotation";

template <>
const std::string X3DField <Array <String>>::typeName = "MFString";

template <>
const std::string X3DField <Array <Vector2d>>::typeName = "MFVec2d";

template <>
const std::string X3DField <Array <Vector2f>>::typeName = "MFVec2f";

template <>
const std::string X3DField <Array <Vector3d>>::typeName = "MFVec3d";

template <>
const std::string X3DField <Array <Vector3f>>::typeName = "MFVec3f";

template <>
const std::string X3DField <Array <Vector4d>>::typeName = "MFVec4d";

template <>
const std::string X3DField <Array <Vector4f>>::typeName = "MFVec4f";

// type

template <>
const FieldType X3DField <Array <bool>>::type = X3DConstants::MFBool;

template <>
const FieldType X3DField <Array <Color3f>>::type = X3DConstants::MFColor;

template <>
const FieldType X3DField <Array <Color4f>>::type = X3DConstants::MFColorRGBA;

template <>
const FieldType X3DField <Array <double>>::type = X3DConstants::MFDouble;

template <>
const FieldType X3DField <Array <float>>::type = X3DConstants::MFFloat;

//template <>
//const FieldType X3DField <Array <SFImage*>>::type = X3DConstants::MFImage;

template <>
const FieldType X3DField <Array <Matrix3d>>::type = X3DConstants::MFMatrix3d;

template <>
const FieldType X3DField <Array <Matrix3f>>::type = X3DConstants::MFMatrix3f;

template <>
const FieldType X3DField <Array <Matrix4d>>::type = X3DConstants::MFMatrix4d;

template <>
const FieldType X3DField <Array <Matrix4f>>::type = X3DConstants::MFMatrix4f;

template <>
const FieldType X3DField <Array <Rotation4d>>::type = X3DConstants::MFRotation;

template <>
const FieldType X3DField <Array <String>>::type = X3DConstants::MFString;

template <>
const FieldType X3DField <Array <Vector2d>>::type = X3DConstants::MFVec2d;

template <>
const FieldType X3DField <Array <Vector2f>>::type = X3DConstants::MFVec2f;

template <>
const FieldType X3DField <Array <Vector3d>>::type = X3DConstants::MFVec3d;

template <>
const FieldType X3DField <Array <Vector3f>>::type = X3DConstants::MFVec3f;

template <>
const FieldType X3DField <Array <Vector4d>>::type = X3DConstants::MFVec4d;

template <>
const FieldType X3DField <Array <Vector4f>>::type = X3DConstants::MFVec4f;

// MFString

template <>
void
X3DNativeArrayField <String>::toXMLStream (std::ostream & ostream) const
{
	if (not empty ())
	{
		for (const auto & value : std::pair (cbegin (), cend () - 1))
		{
			ostream
				<< '"';

			XMLGenerator::Encode (ostream, value, getUnit ());

			ostream
				<< '"'
				<< X3DGenerator::Comma
				<< X3DGenerator::Space;
		}

		ostream
			<< '"'
			<< XMLEncode (back ())
			<< '"';
	}
}

// Explicit instantiation for required instantiations.
template class X3DField <Array <bool>>;
template class X3DField <Array <Color3f>>;
template class X3DField <Array <Color4f>>;
template class X3DField <Array <double>>;
template class X3DField <Array <float>>;
////template class X3DField <Array <SFImage*>>;
template class X3DField <Array <Matrix3d>>;
template class X3DField <Array <Matrix3f>>;
template class X3DField <Array <Matrix4d>>;
template class X3DField <Array <Matrix4f>>;
template class X3DField <Array <Rotation4d>>;
template class X3DField <Array <String>>;
template class X3DField <Array <Vector2d>>;
template class X3DField <Array <Vector2f>>;
template class X3DField <Array <Vector3d>>;
template class X3DField <Array <Vector3f>>;
template class X3DField <Array <Vector4d>>;
template class X3DField <Array <Vector4f>>;

// Explicit instantiation for required instantiations.
template class X3DNativeArrayField <bool>;
template class X3DNativeArrayField <Color3f>;
template class X3DNativeArrayField <Color4f>;
template class X3DNativeArrayField <double>;
template class X3DNativeArrayField <float>;
////template class X3DNativeArrayField <SFImage>;
template class X3DNativeArrayField <Matrix3d>;
template class X3DNativeArrayField <Matrix3f>;
template class X3DNativeArrayField <Matrix4d>;
template class X3DNativeArrayField <Matrix4f>;
template class X3DNativeArrayField <Rotation4d>;
template class X3DNativeArrayField <String>;
template class X3DNativeArrayField <Vector2d>;
template class X3DNativeArrayField <Vector2f>;
template class X3DNativeArrayField <Vector3d>;
template class X3DNativeArrayField <Vector3f>;
template class X3DNativeArrayField <Vector4d>;
template class X3DNativeArrayField <Vector4f>;

} // X3D
} // titania

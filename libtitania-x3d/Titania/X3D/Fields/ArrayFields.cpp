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

#include "ArrayFields.h"

// Explicit instantiation for required instantiations.
template class std::vector <titania::X3D::SFBool*>;
template class std::vector <titania::X3D::SFColor*>;
template class std::vector <titania::X3D::SFColorRGBA*>;
template class std::vector <titania::X3D::SFDouble*>;
template class std::vector <titania::X3D::SFFloat*>;
template class std::vector <titania::X3D::SFImage*>;
template class std::vector <titania::X3D::SFMatrix3d*>;
template class std::vector <titania::X3D::SFMatrix3f*>;
template class std::vector <titania::X3D::SFMatrix4d*>;
template class std::vector <titania::X3D::SFMatrix4f*>;
template class std::vector <titania::X3D::SFRotation*>;
template class std::vector <titania::X3D::SFString*>;
template class std::vector <titania::X3D::SFTime*>;
template class std::vector <titania::X3D::SFVec2d*>;
template class std::vector <titania::X3D::SFVec2f*>;
template class std::vector <titania::X3D::SFVec3d*>;
template class std::vector <titania::X3D::SFVec3f*>;
template class std::vector <titania::X3D::SFVec4d*>;
template class std::vector <titania::X3D::SFVec4f*>;

namespace titania {
namespace X3D {

// typeName

template <>
const std::string X3DField <Array <SFBool>>::typeName ("MFBool");

template <>
const std::string X3DField <Array <SFColor>>::typeName ("MFColor");

template <>
const std::string X3DField <Array <SFColorRGBA>>::typeName ("MFColorRGBA");

template <>
const std::string X3DField <Array <SFDouble>>::typeName ("MFDouble");

template <>
const std::string X3DField <Array <SFFloat>>::typeName ("MFFloat");

template <>
const std::string X3DField <Array <SFImage>>::typeName ("MFImage");

template <>
const std::string X3DField <Array <SFMatrix3d>>::typeName ("MFMatrix3d");

template <>
const std::string X3DField <Array <SFMatrix3f>>::typeName ("MFMatrix3f");

template <>
const std::string X3DField <Array <SFMatrix4d>>::typeName ("MFMatrix4d");

template <>
const std::string X3DField <Array <SFMatrix4f>>::typeName ("MFMatrix4f");

template <>
const std::string X3DField <Array <SFRotation>>::typeName ("MFRotation");

template <>
const std::string X3DField <Array <SFString>>::typeName ("MFString");

template <>
const std::string X3DField <Array <SFTime>>::typeName ("MFTime");

template <>
const std::string X3DField <Array <SFVec2d>>::typeName ("MFVec2d");

template <>
const std::string X3DField <Array <SFVec2f>>::typeName ("MFVec2f");

template <>
const std::string X3DField <Array <SFVec3d>>::typeName ("MFVec3d");

template <>
const std::string X3DField <Array <SFVec3f>>::typeName ("MFVec3f");

template <>
const std::string X3DField <Array <SFVec4d>>::typeName ("MFVec4d");

template <>
const std::string X3DField <Array <SFVec4f>>::typeName ("MFVec4f");

// type

template <>
const X3DConstants::FieldType X3DField <Array <SFBool>>::type = X3DConstants::MFBool;

template <>
const X3DConstants::FieldType X3DField <Array <SFColor>>::type = X3DConstants::MFColor;

template <>
const X3DConstants::FieldType X3DField <Array <SFColorRGBA>>::type = X3DConstants::MFColorRGBA;

template <>
const X3DConstants::FieldType X3DField <Array <SFDouble>>::type = X3DConstants::MFDouble;

template <>
const X3DConstants::FieldType X3DField <Array <SFFloat>>::type = X3DConstants::MFFloat;

template <>
const X3DConstants::FieldType X3DField <Array <SFImage>>::type = X3DConstants::MFImage;

template <>
const X3DConstants::FieldType X3DField <Array <SFMatrix3d>>::type = X3DConstants::MFMatrix3d;

template <>
const X3DConstants::FieldType X3DField <Array <SFMatrix3f>>::type = X3DConstants::MFMatrix3f;

template <>
const X3DConstants::FieldType X3DField <Array <SFMatrix4d>>::type = X3DConstants::MFMatrix4d;

template <>
const X3DConstants::FieldType X3DField <Array <SFMatrix4f>>::type = X3DConstants::MFMatrix4f;

template <>
const X3DConstants::FieldType X3DField <Array <SFRotation>>::type = X3DConstants::MFRotation;

template <>
const X3DConstants::FieldType X3DField <Array <SFString>>::type = X3DConstants::MFString;

template <>
const X3DConstants::FieldType X3DField <Array <SFTime>>::type = X3DConstants::MFTime;

template <>
const X3DConstants::FieldType X3DField <Array <SFVec2d>>::type = X3DConstants::MFVec2d;

template <>
const X3DConstants::FieldType X3DField <Array <SFVec2f>>::type = X3DConstants::MFVec2f;

template <>
const X3DConstants::FieldType X3DField <Array <SFVec3d>>::type = X3DConstants::MFVec3d;

template <>
const X3DConstants::FieldType X3DField <Array <SFVec3f>>::type = X3DConstants::MFVec3f;

template <>
const X3DConstants::FieldType X3DField <Array <SFVec4d>>::type = X3DConstants::MFVec4d;

template <>
const X3DConstants::FieldType X3DField <Array <SFVec4f>>::type = X3DConstants::MFVec4f;

// MFString

template <>
void
X3DArrayField <SFString>::toXMLStream (std::ostream & ostream) const
{
	if (not empty ())
	{
		for (const auto & value : basic::adapter (cbegin (), cend () - 1))
		{
			ostream
				<< '"'
				<< XMLEncode (value)
				<< '"'
				<< Generator::Comma
				<< Generator::Space;
		}

		ostream
			<< '"'
			<< XMLEncode (back ())
			<< '"';
	}
}

// Explicit instantiation for required instantiations.
template class X3DField <Array <SFBool>>;
template class X3DField <Array <SFColor>>;
template class X3DField <Array <SFColorRGBA>>;
template class X3DField <Array <SFDouble>>;
template class X3DField <Array <SFFloat>>;
template class X3DField <Array <SFImage>>;
template class X3DField <Array <SFMatrix3d>>;
template class X3DField <Array <SFMatrix3f>>;
template class X3DField <Array <SFMatrix4d>>;
template class X3DField <Array <SFMatrix4f>>;
template class X3DField <Array <SFRotation>>;
template class X3DField <Array <SFString>>;
template class X3DField <Array <SFTime>>;
template class X3DField <Array <SFVec2d>>;
template class X3DField <Array <SFVec2f>>;
template class X3DField <Array <SFVec3d>>;
template class X3DField <Array <SFVec3f>>;
template class X3DField <Array <SFVec4d>>;
template class X3DField <Array <SFVec4f>>;

// Explicit instantiation for required instantiations.
template class X3DArrayField <SFBool>;
template class X3DArrayField <SFColor>;
template class X3DArrayField <SFColorRGBA>;
template class X3DArrayField <SFDouble>;
template class X3DArrayField <SFFloat>;
template class X3DArrayField <SFImage>;
template class X3DArrayField <SFMatrix3d>;
template class X3DArrayField <SFMatrix3f>;
template class X3DArrayField <SFMatrix4d>;
template class X3DArrayField <SFMatrix4f>;
template class X3DArrayField <SFRotation>;
template class X3DArrayField <SFString>;
template class X3DArrayField <SFTime>;
template class X3DArrayField <SFVec2d>;
template class X3DArrayField <SFVec2f>;
template class X3DArrayField <SFVec3d>;
template class X3DArrayField <SFVec3f>;
template class X3DArrayField <SFVec4d>;
template class X3DArrayField <SFVec4f>;

} // X3D
} // titania

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

#include "ArrayFields.h"

namespace titania {
namespace X3D {

template <>
const Type X3DField <Array <SFBool>>::type ("MFBool");

template <>
const Type X3DField <Array <SFColor>>::type ("MFColor");

template <>
const Type X3DField <Array <SFColorRGBA>>::type ("MFColorRGBA");

template <>
const Type X3DField <Array <SFDouble>>::type ("MFDouble");

template <>
const Type X3DField <Array <SFFloat>>::type ("MFFloat");

template <>
const Type X3DField <Array <SFImage>>::type ("MFImage");

template <>
const Type X3DField <Array <SFMatrix3d>>::type ("MFMatrix3d");

template <>
const Type X3DField <Array <SFMatrix3f>>::type ("MFMatrix3f");

template <>
const Type X3DField <Array <SFMatrix4d>>::type ("MFMatrix4d");

template <>
const Type X3DField <Array <SFMatrix4f>>::type ("MFMatrix4f");

template <>
const Type X3DField <Array <SFRotation>>::type ("MFRotation");

template <>
const Type X3DField <Array <SFString>>::type ("MFString");

template <>
const Type X3DField <Array <SFTime>>::type ("MFTime");

template <>
const Type X3DField <Array <SFVec2d>>::type ("MFVec2d");

template <>
const Type X3DField <Array <SFVec2f>>::type ("MFVec2f");

template <>
const Type X3DField <Array <SFVec3d>>::type ("MFVec3d");

template <>
const Type X3DField <Array <SFVec3f>>::type ("MFVec3f");

template <>
const Type X3DField <Array <SFVec4d>>::type ("MFVec4d");

template <>
const Type X3DField <Array <SFVec4f>>::type ("MFVec4f");

// Explicit instantiation for required instantiations.
template class Array <SFBool>;
template class Array <SFColor>;
template class Array <SFColorRGBA>;
template class Array <SFDouble>;
template class Array <SFFloat>;
template class Array <SFImage>;
template class Array <SFMatrix3d>;
template class Array <SFMatrix3f>;
template class Array <SFMatrix4d>;
template class Array <SFMatrix4f>;
template class Array <SFRotation>;
template class Array <SFString>;
template class Array <SFTime>;
template class Array <SFVec2d>;
template class Array <SFVec2f>;
template class Array <SFVec3d>;
template class Array <SFVec3f>;
template class Array <SFVec4d>;
template class Array <SFVec4f>;

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

}  // X3D

// Explicit instantiation for required instantiations.
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFBool> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFColor> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFColorRGBA> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFDouble> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFFloat> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFImage> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFMatrix3d> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFMatrix3f> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFMatrix4d> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFMatrix4f> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFRotation> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFString> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFTime> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFVec2d> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFVec2f> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFVec3d> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFVec3f> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFVec4d> &);
template std::ostream & X3D::operator << (std::ostream &, const X3D::Array <X3D::SFVec4f> &);

} // titania

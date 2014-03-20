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

#ifndef __TITANIA_X3D_FIELDS_ARRAY_FIELDS_H__
#define __TITANIA_X3D_FIELDS_ARRAY_FIELDS_H__

#include "../Fields/SFColor.h"
#include "../Fields/SFColorRGBA.h"
#include "../Fields/SFImage.h"
#include "../Fields/SFMatrix3.h"
#include "../Fields/SFMatrix4.h"
#include "../Fields/SFRotation4.h"
#include "../Fields/SFString.h"
#include "../Fields/SFTime.h"
#include "../Fields/SFVec2.h"
#include "../Fields/SFVec3.h"
#include "../Fields/SFVec4.h"
#include "../Fields/X3DScalar.h"

#include "../Basic/X3DArrayField.h"

namespace titania {
namespace X3D {

template <>
void
X3DArrayField <SFString>::toXMLStream (std::ostream &) const;

//  Inhibit implicit instantiation for required instantiations.
//extern template class Array <SFBool>;
//extern template class Array <SFColor>;
//extern template class Array <SFColorRGBA>;
//extern template class Array <SFDouble>;
//extern template class Array <SFFloat>;
//extern template class Array <SFImage>;
//extern template class Array <SFMatrix3d>;
//extern template class Array <SFMatrix3f>;
//extern template class Array <SFMatrix4d>;
//extern template class Array <SFMatrix4f>;
//extern template class Array <SFRotation>;
//extern template class Array <SFString>;
//extern template class Array <SFTime>;
//extern template class Array <SFVec2d>;
//extern template class Array <SFVec2f>;
//extern template class Array <SFVec3d>;
//extern template class Array <SFVec3f>;
//extern template class Array <SFVec4d>;
//extern template class Array <SFVec4f>;

// Inhibit implicit instantiation for required instantiations.
extern template class X3DField <Array <SFBool>>;
extern template class X3DField <Array <SFColor>>;
extern template class X3DField <Array <SFColorRGBA>>;
extern template class X3DField <Array <SFDouble>>;
extern template class X3DField <Array <SFFloat>>;
extern template class X3DField <Array <SFImage>>;
extern template class X3DField <Array <SFMatrix3d>>;
extern template class X3DField <Array <SFMatrix3f>>;
extern template class X3DField <Array <SFMatrix4d>>;
extern template class X3DField <Array <SFMatrix4f>>;
extern template class X3DField <Array <SFRotation>>;
extern template class X3DField <Array <SFString>>;
extern template class X3DField <Array <SFTime>>;
extern template class X3DField <Array <SFVec2d>>;
extern template class X3DField <Array <SFVec2f>>;
extern template class X3DField <Array <SFVec3d>>;
extern template class X3DField <Array <SFVec3f>>;
extern template class X3DField <Array <SFVec4d>>;
extern template class X3DField <Array <SFVec4f>>;

// Inhibit implicit instantiation for required instantiations.
extern template class X3DArrayField <SFBool>;
extern template class X3DArrayField <SFColor>;
extern template class X3DArrayField <SFColorRGBA>;
extern template class X3DArrayField <SFDouble>;
extern template class X3DArrayField <SFFloat>;
extern template class X3DArrayField <SFImage>;
extern template class X3DArrayField <SFMatrix3d>;
extern template class X3DArrayField <SFMatrix3f>;
extern template class X3DArrayField <SFMatrix4d>;
extern template class X3DArrayField <SFMatrix4f>;
extern template class X3DArrayField <SFRotation>;
extern template class X3DArrayField <SFString>;
extern template class X3DArrayField <SFTime>;
extern template class X3DArrayField <SFVec2d>;
extern template class X3DArrayField <SFVec2f>;
extern template class X3DArrayField <SFVec3d>;
extern template class X3DArrayField <SFVec3f>;
extern template class X3DArrayField <SFVec4d>;
extern template class X3DArrayField <SFVec4f>;

// Typedefs for standard X3DArrayFields
typedef X3DArrayField <SFBool>      MFBool;
typedef X3DArrayField <SFColor>     MFColor;
typedef X3DArrayField <SFColorRGBA> MFColorRGBA;
typedef X3DArrayField <SFDouble>    MFDouble;
typedef X3DArrayField <SFFloat>     MFFloat;
typedef X3DArrayField <SFImage>     MFImage;
typedef X3DArrayField <SFMatrix3d>  MFMatrix3d;
typedef X3DArrayField <SFMatrix3f>  MFMatrix3f;
typedef X3DArrayField <SFMatrix4d>  MFMatrix4d;
typedef X3DArrayField <SFMatrix4f>  MFMatrix4f;
typedef X3DArrayField <SFRotation>  MFRotation;
typedef X3DArrayField <SFString>    MFString;
typedef X3DArrayField <SFTime>      MFTime;
typedef X3DArrayField <SFVec2d>     MFVec2d;
typedef X3DArrayField <SFVec2f>     MFVec2f;
typedef X3DArrayField <SFVec3d>     MFVec3d;
typedef X3DArrayField <SFVec3f>     MFVec3f;
typedef X3DArrayField <SFVec4d>     MFVec4d;
typedef X3DArrayField <SFVec4f>     MFVec4f;

} // X3D
} // titania

#endif

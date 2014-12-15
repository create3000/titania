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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_ARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_FIELDS_ARRAY_FIELD_H__

#include "../X3DArrayField.h"

#include "SFColor.h"
#include "SFColorRGBA.h"
#include "SFImage.h"
#include "SFMatrix3.h"
#include "SFMatrix4.h"
#include "SFNode.h"
#include "SFRotation4.h"
#include "SFVec2.h"
#include "SFVec3.h"
#include "SFVec4.h"
#include "X3DScalar.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFBool, SFBool>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info);

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFDouble, SFDouble>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info);

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFFloat, SFFloat>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info);

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFInt32, SFInt32>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info);

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFString, SFString>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info);

template <>
v8::Handle <v8::Value>
X3DArrayField <X3D::MFTime, SFTime>::set1Value (uint32_t index, v8::Local <v8::Value> value, const v8::AccessorInfo & info);

extern template class X3DArrayField <X3D::MFBool, SFBool>;
extern template class X3DArrayField <X3D::MFColor, SFColor>;
extern template class X3DArrayField <X3D::MFColorRGBA, SFColorRGBA>;
extern template class X3DArrayField <X3D::MFDouble, SFDouble>;
extern template class X3DArrayField <X3D::MFFloat, SFFloat>;
extern template class X3DArrayField <X3D::MFImage, SFImage>;
extern template class X3DArrayField <X3D::MFInt32, SFInt32>;
extern template class X3DArrayField <X3D::MFMatrix3d, SFMatrix3d>;
extern template class X3DArrayField <X3D::MFMatrix3f, SFMatrix3f>;
extern template class X3DArrayField <X3D::MFMatrix4d, SFMatrix4d>;
extern template class X3DArrayField <X3D::MFMatrix4f, SFMatrix4f>;
extern template class X3DArrayField <X3D::MFNode, SFNode>;
extern template class X3DArrayField <X3D::MFRotation, SFRotation>;
extern template class X3DArrayField <X3D::MFString, SFString>;
extern template class X3DArrayField <X3D::MFTime, SFTime>;
extern template class X3DArrayField <X3D::MFVec2d, SFVec2d>;
extern template class X3DArrayField <X3D::MFVec2f, SFVec2f>;
extern template class X3DArrayField <X3D::MFVec3d, SFVec3d>;
extern template class X3DArrayField <X3D::MFVec3f, SFVec3f>;
extern template class X3DArrayField <X3D::MFVec4d, SFVec4d>;
extern template class X3DArrayField <X3D::MFVec4f, SFVec4f>;

using MFBool      = X3DArrayField <X3D::MFBool, SFBool>;
using MFColor     = X3DArrayField <X3D::MFColor, SFColor>;
using MFColorRGBA = X3DArrayField <X3D::MFColorRGBA, SFColorRGBA>;
using MFDouble    = X3DArrayField <X3D::MFDouble, SFDouble>;
using MFFloat     = X3DArrayField <X3D::MFFloat, SFFloat>;
using MFImage     = X3DArrayField <X3D::MFImage, SFImage>;
using MFInt32     = X3DArrayField <X3D::MFInt32, SFInt32>;
using MFMatrix3d  = X3DArrayField <X3D::MFMatrix3d, SFMatrix3d>;
using MFMatrix3f  = X3DArrayField <X3D::MFMatrix3f, SFMatrix3f>;
using MFMatrix4d  = X3DArrayField <X3D::MFMatrix4d, SFMatrix4d>;
using MFMatrix4f  = X3DArrayField <X3D::MFMatrix4f, SFMatrix4f>;
using MFNode      = X3DArrayField <X3D::MFNode, SFNode>;
using MFRotation  = X3DArrayField <X3D::MFRotation, SFRotation>;
using MFString    = X3DArrayField <X3D::MFString, SFString>;
using MFTime      = X3DArrayField <X3D::MFTime, SFTime>;
using MFVec2d     = X3DArrayField <X3D::MFVec2d, SFVec2d>;
using MFVec2f     = X3DArrayField <X3D::MFVec2f, SFVec2f>;
using MFVec3d     = X3DArrayField <X3D::MFVec3d, SFVec3d>;
using MFVec3f     = X3DArrayField <X3D::MFVec3f, SFVec3f>;
using MFVec4d     = X3DArrayField <X3D::MFVec4d, SFVec4d>;
using MFVec4f     = X3DArrayField <X3D::MFVec4f, SFVec4f>;

} // GoogleV8
} // X3D
} // titania

#endif

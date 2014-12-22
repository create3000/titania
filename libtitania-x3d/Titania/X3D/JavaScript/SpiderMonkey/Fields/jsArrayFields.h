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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_ARRAY_FIELDS_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_ARRAY_FIELDS_H__

#include "../../../Fields/ArrayFields.h"
#include "../jsX3DArrayField.h"

#include "jsSFColor.h"
#include "jsSFColorRGBA.h"
#include "jsSFImage.h"
#include "jsSFMatrix3.h"
#include "jsSFMatrix4.h"
#include "jsSFNode.h"
#include "jsSFRotation.h"
#include "jsSFVec2.h"
#include "jsSFVec3.h"
#include "jsSFVec4.h"
#include "jsX3DScalar.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <>
constexpr ObjectType
jsX3DArrayField <jsSFBool, X3D::MFBool>::getId ()
{ return ObjectType::MFBool; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFColor, X3D::MFColor>::getId ()
{ return ObjectType::MFColor; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFColorRGBA, X3D::MFColorRGBA>::getId ()
{ return ObjectType::MFColorRGBA; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFDouble, X3D::MFDouble>::getId ()
{ return ObjectType::MFDouble; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFFloat, X3D::MFFloat>::getId ()
{ return ObjectType::MFFloat; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFInt32, X3D::MFInt32>::getId ()
{ return ObjectType::MFInt32; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFImage, X3D::MFImage>::getId ()
{ return ObjectType::MFImage; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFMatrix3d, X3D::MFMatrix3d>::getId ()
{ return ObjectType::MFMatrix3d; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFMatrix3f, X3D::MFMatrix3f>::getId ()
{ return ObjectType::MFMatrix3f; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFMatrix4d, X3D::MFMatrix4d>::getId ()
{ return ObjectType::MFMatrix4d; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFMatrix4f, X3D::MFMatrix4f>::getId ()
{ return ObjectType::MFMatrix4f; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFNode, X3D::MFNode>::getId ()
{ return ObjectType::MFNode; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFRotation, X3D::MFRotation>::getId ()
{ return ObjectType::MFRotation; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFString, X3D::MFString>::getId ()
{ return ObjectType::MFString; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFTime, X3D::MFTime>::getId ()
{ return ObjectType::MFTime; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFVec2d, X3D::MFVec2d>::getId ()
{ return ObjectType::MFVec2d; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFVec2f, X3D::MFVec2f>::getId ()
{ return ObjectType::MFVec2f; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFVec3d, X3D::MFVec3d>::getId ()
{ return ObjectType::MFVec3d; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFVec3f, X3D::MFVec3f>::getId ()
{ return ObjectType::MFVec3f; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFVec4d, X3D::MFVec4d>::getId ()
{ return ObjectType::MFVec4d; }

template <>
constexpr ObjectType
jsX3DArrayField <jsSFVec4f, X3D::MFVec4f>::getId ()
{ return ObjectType::MFVec4f; }

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::construct (JSContext*, unsigned, JS::Value*);

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::set1Value (JSContext*, JS::HandleObject, JS::HandleId, JSBool, JS::MutableHandleValue);

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::unshift (JSContext*, unsigned, JS::Value*);

template <>
JSBool
jsX3DArrayField <jsSFNode, X3D::MFNode>::push (JSContext*, unsigned, JS::Value*);

using jsMFBool      = jsX3DArrayField <jsSFBool, X3D::MFBool>;
using jsMFColor     = jsX3DArrayField <jsSFColor, X3D::MFColor>;
using jsMFColorRGBA = jsX3DArrayField <jsSFColorRGBA, X3D::MFColorRGBA>;
using jsMFDouble    = jsX3DArrayField <jsSFDouble, X3D::MFDouble>;
using jsMFFloat     = jsX3DArrayField <jsSFFloat, X3D::MFFloat>;
using jsMFInt32     = jsX3DArrayField <jsSFInt32, X3D::MFInt32>;
using jsMFImage     = jsX3DArrayField <jsSFImage, X3D::MFImage>;
using jsMFMatrix3d  = jsX3DArrayField <jsSFMatrix3d, X3D::MFMatrix3d>;
using jsMFMatrix3f  = jsX3DArrayField <jsSFMatrix3f, X3D::MFMatrix3f>;
using jsMFMatrix4d  = jsX3DArrayField <jsSFMatrix4d, X3D::MFMatrix4d>;
using jsMFMatrix4f  = jsX3DArrayField <jsSFMatrix4f, X3D::MFMatrix4f>;
using jsMFNode      = jsX3DArrayField <jsSFNode, X3D::MFNode>;
using jsMFRotation  = jsX3DArrayField <jsSFRotation, X3D::MFRotation>;
using jsMFString    = jsX3DArrayField <jsSFString, X3D::MFString>;
using jsMFTime      = jsX3DArrayField <jsSFTime, X3D::MFTime>;
using jsMFVec2d     = jsX3DArrayField <jsSFVec2d, X3D::MFVec2d>;
using jsMFVec2f     = jsX3DArrayField <jsSFVec2f, X3D::MFVec2f>;
using jsMFVec3d     = jsX3DArrayField <jsSFVec3d, X3D::MFVec3d>;
using jsMFVec3f     = jsX3DArrayField <jsSFVec3f, X3D::MFVec3f>;
using jsMFVec4d     = jsX3DArrayField <jsSFVec4d, X3D::MFVec4d>;
using jsMFVec4f     = jsX3DArrayField <jsSFVec4f, X3D::MFVec4f>;

extern template class jsX3DArrayField <jsSFBool, X3D::MFBool>;
extern template class jsX3DArrayField <jsSFColor, X3D::MFColor>;
extern template class jsX3DArrayField <jsSFColorRGBA, X3D::MFColorRGBA>;
extern template class jsX3DArrayField <jsSFDouble, X3D::MFDouble>;
extern template class jsX3DArrayField <jsSFFloat, X3D::MFFloat>;
extern template class jsX3DArrayField <jsSFInt32, X3D::MFInt32>;
extern template class jsX3DArrayField <jsSFImage, X3D::MFImage>;
extern template class jsX3DArrayField <jsSFMatrix3d, X3D::MFMatrix3d>;
extern template class jsX3DArrayField <jsSFMatrix3f, X3D::MFMatrix3f>;
extern template class jsX3DArrayField <jsSFMatrix4d, X3D::MFMatrix4d>;
extern template class jsX3DArrayField <jsSFMatrix4f, X3D::MFMatrix4f>;
extern template class jsX3DArrayField <jsSFNode, X3D::MFNode>;
extern template class jsX3DArrayField <jsSFRotation, X3D::MFRotation>;
extern template class jsX3DArrayField <jsSFString, X3D::MFString>;
extern template class jsX3DArrayField <jsSFTime, X3D::MFTime>;
extern template class jsX3DArrayField <jsSFVec2d, X3D::MFVec2d>;
extern template class jsX3DArrayField <jsSFVec2f, X3D::MFVec2f>;
extern template class jsX3DArrayField <jsSFVec3d, X3D::MFVec3d>;
extern template class jsX3DArrayField <jsSFVec3f, X3D::MFVec3f>;
extern template class jsX3DArrayField <jsSFVec4d, X3D::MFVec4d>;
extern template class jsX3DArrayField <jsSFVec4f, X3D::MFVec4f>;

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

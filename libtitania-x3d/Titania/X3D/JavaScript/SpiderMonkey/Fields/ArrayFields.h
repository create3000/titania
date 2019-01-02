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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_ARRAY_FIELDS_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_ARRAY_FIELDS_H__

#include "SFColor.h"
#include "SFColorRGBA.h"
#include "SFImage.h"
#include "SFMatrix3.h"
#include "SFMatrix4.h"
#include "SFNode.h"
#include "SFRotation.h"
#include "SFVec2.h"
#include "SFVec3.h"
#include "SFVec4.h"
#include "X3DScalar.h"

#include "../../../Fields.h"
#include "../X3DArrayField.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

using MFBool      = X3DArrayFieldTemplate <SFBool, X3D::MFBool>;
using MFColor     = X3DArrayFieldTemplate <SFColor, X3D::MFColor>;
using MFColorRGBA = X3DArrayFieldTemplate <SFColorRGBA, X3D::MFColorRGBA>;
using MFDouble    = X3DArrayFieldTemplate <SFDouble, X3D::MFDouble>;
using MFFloat     = X3DArrayFieldTemplate <SFFloat, X3D::MFFloat>;
using MFImage     = X3DArrayFieldTemplate <SFImage, X3D::MFImage>;
using MFInt32     = X3DArrayFieldTemplate <SFInt32, X3D::MFInt32>;
using MFMatrix3d  = X3DArrayFieldTemplate <SFMatrix3d, X3D::MFMatrix3d>;
using MFMatrix3f  = X3DArrayFieldTemplate <SFMatrix3f, X3D::MFMatrix3f>;
using MFMatrix4d  = X3DArrayFieldTemplate <SFMatrix4d, X3D::MFMatrix4d>;
using MFMatrix4f  = X3DArrayFieldTemplate <SFMatrix4f, X3D::MFMatrix4f>;
using MFRotation  = X3DArrayFieldTemplate <SFRotation, X3D::MFRotation>;
using MFString    = X3DArrayFieldTemplate <SFString, X3D::MFString>;
using MFTime      = X3DArrayFieldTemplate <SFTime, X3D::MFTime>;
using MFVec2d     = X3DArrayFieldTemplate <SFVec2d, X3D::MFVec2d>;
using MFVec2f     = X3DArrayFieldTemplate <SFVec2f, X3D::MFVec2f>;
using MFVec3d     = X3DArrayFieldTemplate <SFVec3d, X3D::MFVec3d>;
using MFVec3f     = X3DArrayFieldTemplate <SFVec3f, X3D::MFVec3f>;
using MFVec4d     = X3DArrayFieldTemplate <SFVec4d, X3D::MFVec4d>;
using MFVec4f     = X3DArrayFieldTemplate <SFVec4f, X3D::MFVec4f>;

template <>
constexpr
ObjectType
MFBool::getId ()
{ return ObjectType::MFBool; }

template <>
constexpr
ObjectType
MFColor::getId ()
{ return ObjectType::MFColor; }

template <>
constexpr
ObjectType
MFColorRGBA::getId ()
{ return ObjectType::MFColorRGBA; }

template <>
constexpr
ObjectType
MFDouble::getId ()
{ return ObjectType::MFDouble; }

template <>
constexpr
ObjectType
MFFloat::getId ()
{ return ObjectType::MFFloat; }

template <>
constexpr
ObjectType
MFImage::getId ()
{ return ObjectType::MFImage; }

template <>
constexpr
ObjectType
MFInt32::getId ()
{ return ObjectType::MFInt32; }

template <>
constexpr
ObjectType
MFMatrix3d::getId ()
{ return ObjectType::MFMatrix3d; }

template <>
constexpr
ObjectType
MFMatrix3f::getId ()
{ return ObjectType::MFMatrix3f; }

template <>
constexpr
ObjectType
MFMatrix4d::getId ()
{ return ObjectType::MFMatrix4d; }

template <>
constexpr
ObjectType
MFMatrix4f::getId ()
{ return ObjectType::MFMatrix4f; }

template <>
constexpr
ObjectType
MFRotation::getId ()
{ return ObjectType::MFRotation; }

template <>
constexpr
ObjectType
MFString::getId ()
{ return ObjectType::MFString; }

template <>
constexpr
ObjectType
MFTime::getId ()
{ return ObjectType::MFTime; }

template <>
constexpr
ObjectType
MFVec2d::getId ()
{ return ObjectType::MFVec2d; }

template <>
constexpr
ObjectType
MFVec2f::getId ()
{ return ObjectType::MFVec2f; }

template <>
constexpr
ObjectType
MFVec3d::getId ()
{ return ObjectType::MFVec3d; }

template <>
constexpr
ObjectType
MFVec3f::getId ()
{ return ObjectType::MFVec3f; }

template <>
constexpr
ObjectType
MFVec4d::getId ()
{ return ObjectType::MFVec4d; }

template <>
constexpr
ObjectType
MFVec4f::getId ()
{ return ObjectType::MFVec4f; }

} // spidermonkey
} // X3D
} // titania

#endif

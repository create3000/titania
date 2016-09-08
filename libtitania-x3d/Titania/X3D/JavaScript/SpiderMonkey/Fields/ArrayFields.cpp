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

#include "ArrayFields.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <>
JSClass MFBool::static_class = {
	"MFBool", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFBool>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFColor::static_class = {
	"MFColor", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFColor>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFColorRGBA::static_class = {
	"MFColorRGBA", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFColorRGBA>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFDouble::static_class = {
	"MFDouble", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFDouble>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFFloat::static_class = {
	"MFFloat", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFFloat>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFImage::static_class = {
	"MFImage", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFImage>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFInt32::static_class = {
	"MFInt32", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFInt32>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFMatrix3d::static_class = {
	"MFMatrix3d", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFMatrix3d>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFMatrix3f::static_class = {
	"MFMatrix3f", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFMatrix3f>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFMatrix4d::static_class = {
	"MFMatrix4d", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFMatrix4d>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFMatrix4f::static_class = {
	"MFMatrix4f", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFMatrix4f>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFRotation::static_class = {
	"MFRotation", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFRotation>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFString::static_class = {
	"MFString", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFString>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFTime::static_class = {
	"MFTime", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFTime>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFVec2d::static_class = {
	"MFVec2d", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFVec2d>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFVec2f::static_class = {
	"MFVec2f", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFVec2f>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFVec3d::static_class = {
	"MFVec3d", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFVec3d>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFVec3f::static_class = {
	"MFVec3f", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFVec3f>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFVec4d::static_class = {
	"MFVec4d", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFVec4d>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSClass MFVec4f::static_class = {
	"MFVec4f", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <MFVec4f>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template class X3DArrayField <SFBool, X3D::MFBool>;
template class X3DArrayField <SFColor, X3D::MFColor>;
template class X3DArrayField <SFColorRGBA, X3D::MFColorRGBA>;
template class X3DArrayField <SFDouble, X3D::MFDouble>;
template class X3DArrayField <SFFloat, X3D::MFFloat>;
template class X3DArrayField <SFImage, X3D::MFImage>;
template class X3DArrayField <SFInt32, X3D::MFInt32>;
template class X3DArrayField <SFMatrix3d, X3D::MFMatrix3d>;
template class X3DArrayField <SFMatrix3f, X3D::MFMatrix3f>;
template class X3DArrayField <SFMatrix4d, X3D::MFMatrix4d>;
template class X3DArrayField <SFMatrix4f, X3D::MFMatrix4f>;
template class X3DArrayField <SFRotation, X3D::MFRotation>;
template class X3DArrayField <SFString, X3D::MFString>;
template class X3DArrayField <SFTime, X3D::MFTime>;
template class X3DArrayField <SFVec2d, X3D::MFVec2d>;
template class X3DArrayField <SFVec2f, X3D::MFVec2f>;
template class X3DArrayField <SFVec3d, X3D::MFVec3d>;
template class X3DArrayField <SFVec3f, X3D::MFVec3f>;
template class X3DArrayField <SFVec4d, X3D::MFVec4d>;
template class X3DArrayField <SFVec4f, X3D::MFVec4f>;

} // spidermonkey
} // X3D
} // titania

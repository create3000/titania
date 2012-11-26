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
 ******************************************************************************/

#include "jsFields.h"
#include "jsfield.h"
#include "jsstring.h"

namespace titania {
namespace X3D {

JSBool
JS_ValueToField (JSContext* context, X3DFieldDefinition* field, jsval* vp)
{
	switch (field -> getType ())
	{
		case SFBoolType: {
			JSBool value;

			if (not JS_ValueToBoolean (context, *vp, &value))
				return JS_FALSE;

			*(SFBool*) field = value;
			break;
		}
		case SFColorType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFColor::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFColor, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFColor*) field = *(SFColor*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFColorRGBAType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFColorRGBA::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFColorRGBA, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFColorRGBA*) field = *(SFColorRGBA*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFDoubleType: {
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*(SFDouble*) field = value;
			break;
		}
		case SFFloatType: {
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*(SFFloat*) field = value;
			break;
		}
		case SFInt32Type: {
			int32 value;

			if (not JS_ValueToECMAInt32 (context, *vp, &value))
				return JS_FALSE;

			*(SFInt32*) field = value;
			break;
		}
		case SFImageType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFImage::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFImage, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFImage*) field = *(SFImage*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFMatrix3fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFMatrix3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix3f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFMatrix3f*) field = *(SFMatrix3f*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFMatrix3dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFMatrix3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix3d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFMatrix3d*) field = *(SFMatrix3d*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFMatrix4dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFMatrix4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix4d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFMatrix4d*) field = *(SFMatrix4d*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFMatrix4fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFMatrix4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix4f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFMatrix4f*) field = *(SFMatrix4f*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFNodeType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFNode::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFNode, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFNode*) field = *(SFNode*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFRotationType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFRotation::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFRotation, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFRotation*) field = *(SFRotation*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFStringType: {
			*(SFString*) field = std::string (JS_GetStringBytes (JS_ValueToString (context, *vp)));
			break;
		}
		case SFTimeType: {
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*(SFTime*) field = value;
			break;
		}
		case SFVec2dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFVec2d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec2d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFVec2d*) field = *(SFVec2d*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFVec2fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFVec2f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec2f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFVec2f*) field = *(SFVec2f*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFVec3dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFVec3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec3d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFVec3d*) field = *(SFVec3d*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFVec3fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFVec3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec3f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFVec3f*) field = *(SFVec3f*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFVec4dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFVec4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec4d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFVec4d*) field = *(SFVec4d*) JS_GetPrivate (context, obj2);
			break;
		}
		case SFVec4fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFVec4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec4f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(SFVec4f*) field = *(SFVec4f*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFBoolType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFBool::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFBool, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFBool*) field = *(MFBool*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFColorType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFColor::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFColor, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFColor*) field = *(MFColor*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFColorRGBAType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFColorRGBA::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFColorRGBA, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFColorRGBA*) field = *(MFColorRGBA*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFDoubleType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFDouble::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFDouble, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFDouble*) field = *(MFDouble*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFFloatType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFFloat::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFFloat, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFFloat*) field = *(MFFloat*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFImageType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFImage::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFImage, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFImage*) field = *(MFImage*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFInt32Type: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFInt32::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFInt32, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFInt32*) field = *(MFInt32*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFMatrix3dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFMatrix3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix3d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFMatrix3d*) field = *(MFMatrix3d*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFMatrix3fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFMatrix3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix3f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFMatrix3f*) field = *(MFMatrix3f*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFMatrix4dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFMatrix4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix4d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFMatrix4d*) field = *(MFMatrix4d*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFMatrix4fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFMatrix4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix4f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFMatrix4f*) field = *(MFMatrix4f*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFNodeType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFNode::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFNode, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFNode*) field = *(MFNode*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFRotationType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFRotation::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFRotation, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFRotation*) field = *(MFRotation*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFStringType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFString::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFString, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFString*) field = *(MFString*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFTimeType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFTime::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFTime, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFTime*) field = *(MFTime*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFVec2dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFVec2d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec2d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFVec2d*) field = *(MFVec2d*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFVec2fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFVec2f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec2f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFVec2f*) field = *(MFVec2f*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFVec3dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFVec3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec3d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFVec3d*) field = *(MFVec3d*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFVec3fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFVec3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec3f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFVec3f*) field = *(MFVec3f*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFVec4dType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFVec4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec4d, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFVec4d*) field = *(MFVec4d*) JS_GetPrivate (context, obj2);
			break;
		}
		case MFVec4fType: {
			JSObject* obj2;

			if (not JS_ValueToObject (context, *vp, &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsMFVec4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec4f, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			*(MFVec4f*) field = *(MFVec4f*) JS_GetPrivate (context, obj2);
			break;
		}
	}

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

JSBool
JS_NewFieldValue (JSContext* context, X3DFieldDefinition* field, jsval* vp, const bool seal)
{
	switch (field -> getType ())
	{
		case SFBoolType:
			*vp = *(SFBool*) field ? JSVAL_TRUE : JSVAL_FALSE;
			return JS_TRUE;
		case SFColorType:
			return jsSFColor::create (context, (SFColor*) field, vp, seal);
		case SFColorRGBAType:
			return jsSFColorRGBA::create (context, (SFColorRGBA*) field, vp, seal);
		case SFDoubleType:
			return JS_NewDoubleValue (context, ((SFDouble*) field) -> getValue (), vp);
		case SFFloatType:
			return JS_NewDoubleValue (context, ((SFFloat*) field) -> getValue (), vp);
		case SFInt32Type:
			return JS_NewDoubleValue (context, ((SFInt32*) field) -> getValue (), vp);
		case SFImageType:
			return jsSFImage::create (context, (SFImage*) field, vp, seal);
		case SFMatrix3fType:
			return jsSFMatrix3f::create (context, (SFMatrix3f*) field, vp, seal);
		case SFMatrix3dType:
			return jsSFMatrix3d::create (context, (SFMatrix3d*) field, vp, seal);
		case SFMatrix4dType:
			return jsSFMatrix4d::create (context, (SFMatrix4d*) field, vp, seal);
		case SFMatrix4fType:
			return jsSFMatrix4f::create (context, (SFMatrix4f*) field, vp, seal);
		case SFNodeType:
			return jsSFNode::create (context, (SFNode*) field, vp, seal);
		case SFRotationType:
			return jsSFRotation::create (context, (SFRotation*) field, vp, seal);
		case SFStringType:
			return JS_NewStringValue (context, ((SFString*) field) -> getValue (), vp);
		case SFTimeType:
			return JS_NewDoubleValue (context, ((SFTime*) field) -> getValue (), vp);
		case SFVec2dType:
			return jsSFVec2d::create (context, (SFVec2d*) field, vp, seal);
		case SFVec2fType:
			return jsSFVec2f::create (context, (SFVec2f*) field, vp, seal);
		case SFVec3dType:
			return jsSFVec3d::create (context, (SFVec3d*) field, vp, seal);
		case SFVec3fType:
			return jsSFVec3f::create (context, (SFVec3f*) field, vp, seal);
		case SFVec4dType:
			return jsSFVec4d::create (context, (SFVec4d*) field, vp, seal);
		case SFVec4fType:
			return jsSFVec4f::create (context, (SFVec4f*) field, vp, seal);
		case MFBoolType:
			return jsMFBool::create (context, (MFBool*) field, vp, seal);
		case MFColorType:
			return jsMFColor::create (context, (MFColor*) field, vp, seal);
		case MFColorRGBAType:
			return jsMFColorRGBA::create (context, (MFColorRGBA*) field, vp, seal);
		case MFDoubleType:
			return jsMFDouble::create (context, (MFDouble*) field, vp, seal);
		case MFFloatType:
			return jsMFFloat::create (context, (MFFloat*) field, vp, seal);
		case MFImageType:
			return jsMFImage::create (context, (MFImage*) field, vp, seal);
		case MFInt32Type:
			return jsMFInt32::create (context, (MFInt32*) field, vp, seal);
		case MFMatrix3dType:
			return jsMFMatrix3d::create (context, (MFMatrix3d*) field, vp, seal);
		case MFMatrix3fType:
			return jsMFMatrix3f::create (context, (MFMatrix3f*) field, vp, seal);
		case MFMatrix4dType:
			return jsMFMatrix4d::create (context, (MFMatrix4d*) field, vp, seal);
		case MFMatrix4fType:
			return jsMFMatrix4f::create (context, (MFMatrix4f*) field, vp, seal);
		case MFNodeType:
			return jsMFNode::create (context, (MFNode*) field, vp, seal);
		case MFRotationType:
			return jsMFRotation::create (context, (MFRotation*) field, vp, seal);
		case MFStringType:
			return jsMFString::create (context, (MFString*) field, vp, seal);
		case MFTimeType:
			return jsMFTime::create (context, (MFTime*) field, vp, seal);
		case MFVec2dType:
			return jsMFVec2d::create (context, (MFVec2d*) field, vp, seal);
		case MFVec2fType:
			return jsMFVec2f::create (context, (MFVec2f*) field, vp, seal);
		case MFVec3dType:
			return jsMFVec3d::create (context, (MFVec3d*) field, vp, seal);
		case MFVec3fType:
			return jsMFVec3f::create (context, (MFVec3f*) field, vp, seal);
		case MFVec4dType:
			return jsMFVec4d::create (context, (MFVec4d*) field, vp, seal);
		case MFVec4fType:
			return jsMFVec4f::create (context, (MFVec4f*) field, vp, seal);
	}

	return JS_TRUE;
}

} // X3D
} // titania

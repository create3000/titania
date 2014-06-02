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

#include "jsFields.h"
#include "jsString.h"
#include "jsfield.h"
#include "jsError.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSBool
JS_ValueToField (JSContext* const context, X3DFieldDefinition* const field, jsval* const vp)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
		{
			JSBool value;

			if (not JS_ValueToBoolean (context, *vp, &value))
				return JS_FALSE;

			*static_cast <SFBool*> (field) = value;

			break;
		}
		case X3DConstants::SFColor:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFColor::getClass ()))
				return JS_FALSE;

			*static_cast <SFColor*> (field) = *(SFColor*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFColorRGBA:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFColorRGBA::getClass ()))
				return JS_FALSE;

			*static_cast <SFColorRGBA*> (field) = *(SFColorRGBA*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFDouble:
		{
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*static_cast <SFDouble*> (field) = value;

			break;
		}
		case X3DConstants::SFFloat:
		{
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*static_cast <SFFloat*> (field) = value;

			break;
		}
		case X3DConstants::SFInt32:
		{
			int32 value;

			if (not JS_ValueToECMAInt32 (context, *vp, &value))
				return JS_FALSE;

			*static_cast <SFInt32*> (field) = value;

			break;
		}
		case X3DConstants::SFImage:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFImage::getClass ()))
				return JS_FALSE;

			*static_cast <SFImage*> (field) = *(SFImage*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix3d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFMatrix3d::getClass ()))
				return JS_FALSE;

			*static_cast <SFMatrix3d*> (field) = *(SFMatrix3d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix3f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFMatrix3f::getClass ()))
				return JS_FALSE;

			*static_cast <SFMatrix3f*> (field) = *(SFMatrix3f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix4d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFMatrix4d::getClass ()))
				return JS_FALSE;

			*static_cast <SFMatrix4d*> (field) = *(SFMatrix4d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix4f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFMatrix4f::getClass ()))
				return JS_FALSE;

			*static_cast <SFMatrix4f*> (field) = *(SFMatrix4f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFNode:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;
				
			if (value)
			{
				if (JS_InstanceOfError (context, value, jsSFNode::getClass ()))
					return JS_FALSE;

				*static_cast <SFNode*> (field) = *(SFNode*) JS_GetPrivate (context, value);
			}
			else
				*static_cast <SFNode*> (field) = nullptr;

			break;
		}
		case X3DConstants::SFRotation:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFRotation::getClass ()))
				return JS_FALSE;

			*static_cast <SFRotation*> (field) = *(SFRotation*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFString:
		{
			*static_cast <SFString*> (field) = JS_GetString (context, *vp);

			break;
		}
		case X3DConstants::SFTime:
		{
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*static_cast <SFTime*> (field) = value;

			break;
		}
		case X3DConstants::SFVec2d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFVec2d::getClass ()))
				return JS_FALSE;

			*static_cast <SFVec2d*> (field) = *(SFVec2d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec2f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFVec2f::getClass ()))
				return JS_FALSE;

			*static_cast <SFVec2f*> (field) = *(SFVec2f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec3d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFVec3d::getClass ()))
				return JS_FALSE;

			*static_cast <SFVec3d*> (field) = *(SFVec3d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec3f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFVec3f::getClass ()))
				return JS_FALSE;

			*static_cast <SFVec3f*> (field) = *(SFVec3f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec4d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFVec4d::getClass ()))
				return JS_FALSE;

			*static_cast <SFVec4d*> (field) = *(SFVec4d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec4f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsSFVec4f::getClass ()))
				return JS_FALSE;

			*static_cast <SFVec4f*> (field) = *(SFVec4f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFBool:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFBool::getClass ()))
				return JS_FALSE;

			*static_cast <MFBool*> (field) = *(MFBool*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFColor:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFColor::getClass ()))
				return JS_FALSE;

			*static_cast <MFColor*> (field) = *(MFColor*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFColorRGBA:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFColorRGBA::getClass ()))
				return JS_FALSE;

			*static_cast <MFColorRGBA*> (field) = *(MFColorRGBA*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFDouble:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFDouble::getClass ()))
				return JS_FALSE;

			*static_cast <MFDouble*> (field) = *(MFDouble*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFFloat:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFFloat::getClass ()))
				return JS_FALSE;

			*static_cast <MFFloat*> (field) = *(MFFloat*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFImage:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFImage::getClass ()))
				return JS_FALSE;

			*static_cast <MFImage*> (field) = *(MFImage*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFInt32:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFInt32::getClass ()))
				return JS_FALSE;

			*static_cast <MFInt32*> (field) = *(MFInt32*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFMatrix3d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFMatrix3d::getClass ()))
				return JS_FALSE;

			*static_cast <MFMatrix3d*> (field) = *(MFMatrix3d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFMatrix3f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFMatrix3f::getClass ()))
				return JS_FALSE;

			*static_cast <MFMatrix3f*> (field) = *(MFMatrix3f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFMatrix4d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFMatrix4d::getClass ()))
				return JS_FALSE;

			*static_cast <MFMatrix4d*> (field) = *(MFMatrix4d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFMatrix4f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;
			
			if (JS_InstanceOfError (context, value, jsMFMatrix4f::getClass ()))
				return JS_FALSE;

			*static_cast <MFMatrix4f*> (field) = *(MFMatrix4f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFNode:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFNode::getClass ()))
				return JS_FALSE;

			*static_cast <MFNode*> (field) = *(MFNode*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFRotation:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFRotation::getClass ()))
				return JS_FALSE;

			*static_cast <MFRotation*> (field) = *(MFRotation*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFString:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFString::getClass ()))
				return JS_FALSE;

			*static_cast <MFString*> (field) = *(MFString*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFTime:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFTime::getClass ()))
				return JS_FALSE;

			*static_cast <MFTime*> (field) = *(MFTime*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFVec2d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFVec2d::getClass ()))
				return JS_FALSE;

			*static_cast <MFVec2d*> (field) = *(MFVec2d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFVec2f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFVec2f::getClass ()))
				return JS_FALSE;

			*static_cast <MFVec2f*> (field) = *(MFVec2f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFVec3d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFVec3d::getClass ()))
				return JS_FALSE;

			*static_cast <MFVec3d*> (field) = *(MFVec3d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFVec3f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFVec3f::getClass ()))
				return JS_FALSE;

			*static_cast <MFVec3f*> (field) = *(MFVec3f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFVec4d:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFVec4d::getClass ()))
				return JS_FALSE;

			*static_cast <MFVec4d*> (field) = *(MFVec4d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFVec4f:
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (JS_InstanceOfError (context, value, jsMFVec4f::getClass ()))
				return JS_FALSE;

			*static_cast <MFVec4f*> (field) = *(MFVec4f*) JS_GetPrivate (context, value);

			break;
		}
	}

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

JSBool
JS_NewFieldValue (JSContext* const context, X3DFieldDefinition* const field, jsval* const vp)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
			return jsSFBool::create (context, static_cast <SFBool*> (field), vp);

		case X3DConstants::SFColor:
			return jsSFColor::create (context, static_cast <SFColor*> (field), vp);

		case X3DConstants::SFColorRGBA:
			return jsSFColorRGBA::create (context, static_cast <SFColorRGBA*> (field), vp);

		case X3DConstants::SFDouble:
			return jsSFDouble::create (context, static_cast <SFDouble*> (field), vp);

		case X3DConstants::SFFloat:
			return jsSFFloat::create (context, static_cast <SFFloat*> (field), vp);

		case X3DConstants::SFInt32:
			return jsSFInt32::create (context, static_cast <SFInt32*> (field), vp);

		case X3DConstants::SFImage:
			return jsSFImage::create (context, static_cast <SFImage*> (field), vp);

		case X3DConstants::SFMatrix3d:
			return jsSFMatrix3d::create (context, static_cast <SFMatrix3d*> (field), vp);

		case X3DConstants::SFMatrix3f:
			return jsSFMatrix3f::create (context, static_cast <SFMatrix3f*> (field), vp);

		case X3DConstants::SFMatrix4d:
			return jsSFMatrix4d::create (context, static_cast <SFMatrix4d*> (field), vp);

		case X3DConstants::SFMatrix4f:
			return jsSFMatrix4f::create (context, static_cast <SFMatrix4f*> (field), vp);

		case X3DConstants::SFNode:
			return jsSFNode::create (context, static_cast <SFNode*> (field), vp);

		case X3DConstants::SFRotation:
			return jsSFRotation::create (context, static_cast <SFRotation*> (field), vp);

		case X3DConstants::SFString:
			return jsSFString::create (context, static_cast <SFString*> (field), vp);

		case X3DConstants::SFTime:
			return jsSFTime::create (context, static_cast <SFTime*> (field), vp);

		case X3DConstants::SFVec2d:
			return jsSFVec2d::create (context, static_cast <SFVec2d*> (field), vp);

		case X3DConstants::SFVec2f:
			return jsSFVec2f::create (context, static_cast <SFVec2f*> (field), vp);

		case X3DConstants::SFVec3d:
			return jsSFVec3d::create (context, static_cast <SFVec3d*> (field), vp);

		case X3DConstants::SFVec3f:
			return jsSFVec3f::create (context, static_cast <SFVec3f*> (field), vp);

		case X3DConstants::SFVec4d:
			return jsSFVec4d::create (context, static_cast <SFVec4d*> (field), vp);

		case X3DConstants::SFVec4f:
			return jsSFVec4f::create (context, static_cast <SFVec4f*> (field), vp);

		case X3DConstants::MFBool:
			return jsMFBool::create (context, static_cast <MFBool*> (field), vp);

		case X3DConstants::MFColor:
			return jsMFColor::create (context, static_cast <MFColor*> (field), vp);

		case X3DConstants::MFColorRGBA:
			return jsMFColorRGBA::create (context, static_cast <MFColorRGBA*> (field), vp);

		case X3DConstants::MFDouble:
			return jsMFDouble::create (context, static_cast <MFDouble*> (field), vp);

		case X3DConstants::MFFloat:
			return jsMFFloat::create (context, static_cast <MFFloat*> (field), vp);

		case X3DConstants::MFImage:
			return jsMFImage::create (context, static_cast <MFImage*> (field), vp);

		case X3DConstants::MFInt32:
			return jsMFInt32::create (context, static_cast <MFInt32*> (field), vp);

		case X3DConstants::MFMatrix3d:
			return jsMFMatrix3d::create (context, static_cast <MFMatrix3d*> (field), vp);

		case X3DConstants::MFMatrix3f:
			return jsMFMatrix3f::create (context, static_cast <MFMatrix3f*> (field), vp);

		case X3DConstants::MFMatrix4d:
			return jsMFMatrix4d::create (context, static_cast <MFMatrix4d*> (field), vp);

		case X3DConstants::MFMatrix4f:
			return jsMFMatrix4f::create (context, static_cast <MFMatrix4f*> (field), vp);

		case X3DConstants::MFNode:
			return jsMFNode::create (context, static_cast <MFNode*> (field), vp);

		case X3DConstants::MFRotation:
			return jsMFRotation::create (context, static_cast <MFRotation*> (field), vp);

		case X3DConstants::MFString:
			return jsMFString::create (context, static_cast <MFString*> (field), vp);

		case X3DConstants::MFTime:
			return jsMFTime::create (context, static_cast <MFTime*> (field), vp);

		case X3DConstants::MFVec2d:
			return jsMFVec2d::create (context, static_cast <MFVec2d*> (field), vp);

		case X3DConstants::MFVec2f:
			return jsMFVec2f::create (context, static_cast <MFVec2f*> (field), vp);

		case X3DConstants::MFVec3d:
			return jsMFVec3d::create (context, static_cast <MFVec3d*> (field), vp);

		case X3DConstants::MFVec3f:
			return jsMFVec3f::create (context, static_cast <MFVec3f*> (field), vp);

		case X3DConstants::MFVec4d:
			return jsMFVec4d::create (context, static_cast <MFVec4d*> (field), vp);

		case X3DConstants::MFVec4f:
			return jsMFVec4f::create (context, static_cast <MFVec4f*> (field), vp);
	}

	return JS_TRUE;
}

} // MozillaSpiderMonkey
} // X3D
} // titania

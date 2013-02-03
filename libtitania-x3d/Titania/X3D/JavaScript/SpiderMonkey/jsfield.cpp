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

#include "String.h"
#include "jsFields.h"
#include "jsfield.h"

namespace titania {
namespace X3D {

JSBool
JS_ValueToField (JSContext* context, X3DFieldDefinition* field, jsval* vp)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
		{
			JSBool value;

			if (not JS_ValueToBoolean (context, *vp, &value))
				return JS_FALSE;

			*(SFBool*) field = value;

			break;
		}
		case X3DConstants::SFColor:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFColor::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFColor, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFColor*) field = *(SFColor*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFColorRGBA:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFColorRGBA::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFColorRGBA, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFColorRGBA*) field = *(SFColorRGBA*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFDouble:
		{
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*(SFDouble*) field = value;

			break;
		}
		case X3DConstants::SFFloat:
		{
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*(SFFloat*) field = value;

			break;
		}
		case X3DConstants::SFInt32:
		{
			int32 value;

			if (not JS_ValueToECMAInt32 (context, *vp, &value))
				return JS_FALSE;

			*(SFInt32*) field = value;

			break;
		}
		case X3DConstants::SFImage:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFImage::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFImage, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFImage*) field = *(SFImage*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix3f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFMatrix3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix3f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFMatrix3f*) field = *(SFMatrix3f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix3d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFMatrix3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix3d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFMatrix3d*) field = *(SFMatrix3d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix4d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFMatrix4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix4d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFMatrix4d*) field = *(SFMatrix4d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFMatrix4f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFMatrix4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFMatrix4f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFMatrix4f*) field = *(SFMatrix4f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFNode:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFNode::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFNode, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(X3DField <X3DBaseNode*>*)field = *(SFNode <X3DBaseNode>*)JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFRotation:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFRotation::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFRotation, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFRotation*) field = *(SFRotation*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFString:
		{
			*(SFString*) field = JS_GetString (context, *vp);

			break;
		}
		case X3DConstants::SFTime:
		{
			jsdouble value;

			if (not JS_ValueToNumber (context, *vp, &value))
				return JS_FALSE;

			*(SFTime*) field = value;

			break;
		}
		case X3DConstants::SFVec2d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFVec2d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec2d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFVec2d*) field = *(SFVec2d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec2f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFVec2f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec2f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFVec2f*) field = *(SFVec2f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec3d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFVec3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec3d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFVec3d*) field = *(SFVec3d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec3f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFVec3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec3f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFVec3f*) field = *(SFVec3f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec4d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFVec4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec4d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFVec4d*) field = *(SFVec4d*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::SFVec4f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFVec4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be SFVec4f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			*(SFVec4f*) field = *(SFVec4f*) JS_GetPrivate (context, value);

			break;
		}
		case X3DConstants::MFBool:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFBool::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFBool, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFColor:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFColor::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFColor, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFColorRGBA:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFColorRGBA::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFColorRGBA, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFDouble:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFDouble::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFDouble, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFFloat:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFFloat::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFFloat, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFImage:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFImage::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFImage, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFInt32:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFInt32::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFInt32, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();
			
			break;
		}
		case X3DConstants::MFMatrix3d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFMatrix3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix3d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFMatrix3f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFMatrix3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix3f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFMatrix4d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFMatrix4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix4d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFMatrix4f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFMatrix4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFMatrix4f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFNode:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFNode::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFNode, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFRotation:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFRotation::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFRotation, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFString:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFString::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFString, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFTime:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFTime::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFTime, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFVec2d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFVec2d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec2d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFVec2f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFVec2f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec2f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFVec3d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFVec3d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec3d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFVec3f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFVec3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec3f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFVec4d:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFVec4d::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec4d, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

			break;
		}
		case X3DConstants::MFVec4f:
		{
			JSObject* value;

			if (not JS_ValueToObject (context, *vp, &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsMFVec4f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument is invalid - should be MFVec4f, is %s", JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> write (*dynamic_cast <X3DChildObject*> ((X3DArray*) JS_GetPrivate (context, value)));
			field -> notifyParents ();

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
		case X3DConstants::SFBool:
		{
			*vp = *(SFBool*) field ? JSVAL_TRUE : JSVAL_FALSE;
			return JS_TRUE;
		}

		case X3DConstants::SFColor:
			return jsSFColor::create (context, (SFColor*) field, vp, seal);

		case X3DConstants::SFColorRGBA:
			return jsSFColorRGBA::create (context, (SFColorRGBA*) field, vp, seal);

		case X3DConstants::SFDouble:
			return JS_NewNumberValue (context, ((SFDouble*) field) -> getValue (), vp);

		case X3DConstants::SFFloat:
			return JS_NewNumberValue (context, ((SFFloat*) field) -> getValue (), vp);

		case X3DConstants::SFInt32:
			return JS_NewNumberValue (context, ((SFInt32*) field) -> getValue (), vp);

		case X3DConstants::SFImage:
			return jsSFImage::create (context, (SFImage*) field, vp, seal);

		case X3DConstants::SFMatrix3f:
			return jsSFMatrix3f::create (context, (SFMatrix3f*) field, vp, seal);

		case X3DConstants::SFMatrix3d:
			return jsSFMatrix3d::create (context, (SFMatrix3d*) field, vp, seal);

		case X3DConstants::SFMatrix4d:
			return jsSFMatrix4d::create (context, (SFMatrix4d*) field, vp, seal);

		case X3DConstants::SFMatrix4f:
			return jsSFMatrix4f::create (context, (SFMatrix4f*) field, vp, seal);

		case X3DConstants::SFNode:
			return jsSFNode::create (context, (X3DField <X3DBaseNode*>*)field, vp, seal);

		case X3DConstants::SFRotation:
			return jsSFRotation::create (context, (SFRotation*) field, vp, seal);

		case X3DConstants::SFString:
			return JS_NewStringValue (context, ((SFString*) field) -> getValue (), vp);

		case X3DConstants::SFTime:
			return JS_NewNumberValue (context, ((SFTime*) field) -> getValue (), vp);

		case X3DConstants::SFVec2d:
			return jsSFVec2d::create (context, (SFVec2d*) field, vp, seal);

		case X3DConstants::SFVec2f:
			return jsSFVec2f::create (context, (SFVec2f*) field, vp, seal);

		case X3DConstants::SFVec3d:
			return jsSFVec3d::create (context, (SFVec3d*) field, vp, seal);

		case X3DConstants::SFVec3f:
			return jsSFVec3f::create (context, (SFVec3f*) field, vp, seal);

		case X3DConstants::SFVec4d:
			return jsSFVec4d::create (context, (SFVec4d*) field, vp, seal);

		case X3DConstants::SFVec4f:
			return jsSFVec4f::create (context, (SFVec4f*) field, vp, seal);

		case X3DConstants::MFBool:
			return jsMFBool::create (context, (MFBool*) field, vp, seal);

		case X3DConstants::MFColor:
			return jsMFColor::create (context, (MFColor*) field, vp, seal);

		case X3DConstants::MFColorRGBA:
			return jsMFColorRGBA::create (context, (MFColorRGBA*) field, vp, seal);

		case X3DConstants::MFDouble:
			return jsMFDouble::create (context, (MFDouble*) field, vp, seal);

		case X3DConstants::MFFloat:
			return jsMFFloat::create (context, (MFFloat*) field, vp, seal);

		case X3DConstants::MFImage:
			return jsMFImage::create (context, (MFImage*) field, vp, seal);

		case X3DConstants::MFInt32:
			return jsMFInt32::create (context, (MFInt32*) field, vp, seal);

		case X3DConstants::MFMatrix3d:
			return jsMFMatrix3d::create (context, (MFMatrix3d*) field, vp, seal);

		case X3DConstants::MFMatrix3f:
			return jsMFMatrix3f::create (context, (MFMatrix3f*) field, vp, seal);

		case X3DConstants::MFMatrix4d:
			return jsMFMatrix4d::create (context, (MFMatrix4d*) field, vp, seal);

		case X3DConstants::MFMatrix4f:
			return jsMFMatrix4f::create (context, (MFMatrix4f*) field, vp, seal);

		case X3DConstants::MFNode:
			return jsMFNode::create (context, dynamic_cast <X3DArray*> (field), vp, seal);

		case X3DConstants::MFRotation:
			return jsMFRotation::create (context, (MFRotation*) field, vp, seal);

		case X3DConstants::MFString:
			return jsMFString::create (context, (MFString*) field, vp, seal);

		case X3DConstants::MFTime:
			return jsMFTime::create (context, (MFTime*) field, vp, seal);

		case X3DConstants::MFVec2d:
			return jsMFVec2d::create (context, (MFVec2d*) field, vp, seal);

		case X3DConstants::MFVec2f:
			return jsMFVec2f::create (context, (MFVec2f*) field, vp, seal);

		case X3DConstants::MFVec3d:
			return jsMFVec3d::create (context, (MFVec3d*) field, vp, seal);

		case X3DConstants::MFVec3f:
			return jsMFVec3f::create (context, (MFVec3f*) field, vp, seal);

		case X3DConstants::MFVec4d:
			return jsMFVec4d::create (context, (MFVec4d*) field, vp, seal);

		case X3DConstants::MFVec4f:
			return jsMFVec4f::create (context, (MFVec4f*) field, vp, seal);
	}

	return JS_TRUE;
}

} // X3D
} // titania

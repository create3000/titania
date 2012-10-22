/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#include "jsVrmlMatrix.h"

#include "jsSFRotation.h"
#include "jsSFVec3f.h"

namespace Titania {
namespace X3D {

const size_t jsVrmlMatrix::size = 16;

JSClass jsVrmlMatrix::static_class = {
	"VrmlMatrix", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	(JSEnumerateOp) enumerate, resolve, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSFunctionSpec jsVrmlMatrix::functions [ ] = {
	{ "setTransform",  setTransform,  5, 0, 0 },
	{ "getTransform",  getTransform,  3, 0, 0 },
	{ "inverse",       inverse,       0, 0, 0 },
	{ "transpose",     transpose,     0, 0, 0 },
	{ "multLeft",      multLeft,      1, 0, 0 },
	{ "multRight",     multRight,     1, 0, 0 },
	{ "multVecMatrix", multVecMatrix, 1, 0, 0 },
	{ "multMatrixVec", multMatrixVec, 1, 0, 0 },
	{ "toString",      toString,      0, 0, 0 },
	{ 0, 0, 0, 0, 0 }

};

void
jsVrmlMatrix::init (JSContext* context, JSObject* global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, NULL, functions, NULL, NULL);
}

JSBool
jsVrmlMatrix::create (JSContext* context, SFMatrix4f* field, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, field);

	if (seal)
		JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsVrmlMatrix::construct (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		JS_SetPrivate (context, obj, new SFMatrix4f ());
		return JS_TRUE;
	}
	else if (argc == size)
	{
		jsdouble m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;

		if (not JS_ConvertArguments (context, argc, argv, "dddddddddddddddd",
		                             &m11, &m12, &m13, &m14, &m21, &m22, &m23, &m24, &m31, &m32, &m33, &m34, &m41, &m42, &m43, &m44))
			return JS_FALSE;

		JS_SetPrivate (context, obj, new SFMatrix4f (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44));

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::get1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

	return JS_NewDoubleValue (context, sfmatrix4f -> get1Value (JSVAL_TO_INT (id)), vp);
}

JSBool
jsVrmlMatrix::set1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	sfmatrix4f -> set1Value (JSVAL_TO_INT (id), value);

	return JS_TRUE;
}

JSBool
jsVrmlMatrix::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	if (not JS_GetPrivate (context, obj))
	{
		*statep = JSVAL_NULL;
		return JS_TRUE;
	}

	size_t* index;

	switch (enum_op)
	{
		case JSENUMERATE_INIT:
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				*idp = INT_TO_JSVAL (size);

			break;
		case JSENUMERATE_NEXT:
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < size)
			{
				if (idp)
					*idp = INT_TO_JSVAL (*index);

				*index = *index + 1;
				break;
			}

		//else done -- cleanup.
		case JSENUMERATE_DESTROY:
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);
			delete index;
			*statep = JSVAL_NULL;
	}

	return JS_TRUE;
}

JSBool
jsVrmlMatrix::resolve (JSContext* context, JSObject* obj, jsval id)
{
	if (not JSVAL_IS_INT (id))
		return JS_ResolveStub (context, obj, id);

	int32 index = JSVAL_TO_INT (id);

	if (index >= 0 and index < (int32) size)
	{
		// Define the indexed property
		JS_DefineProperty (context, obj, (char*) index, NULL, get1Value, set1Value,
		                   JSPROP_PERMANENT | JSPROP_INDEX);

		return JS_TRUE;
	}

	JS_ReportError (context, "index out of range");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::setTransform (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc >= 0 or argc <= 5)
	{
		SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

		JSObject* translationObj, * rotationObj, * scaleObj, * scaleOrientationObj, * centerObj;

		if (not JS_ConvertArguments (context, argc, argv, "/ooooo", &translationObj, &rotationObj, &scaleObj, &scaleOrientationObj, &centerObj))
			return JS_FALSE;

		if (argc >= 1)
		{
			if (not JS_InstanceOf (context, translationObj, jsSFVec3f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, translationObj) -> name);
				return JS_FALSE;
			}

			SFVec3f* translation = (SFVec3f*) JS_GetPrivate (context, translationObj);

			if (argc >= 2)
			{
				if (not JS_InstanceOf (context, rotationObj, jsSFRotation::getClass (), NULL))
				{
					JS_ReportError (context, "Type of argument 2 is invalid - should be SFRotation, is %s", JS_GetClass (context, rotationObj) -> name);
					return JS_FALSE;
				}

				SFRotation* rotation = (SFRotation*) JS_GetPrivate (context, rotationObj);

				if (argc >= 3)
				{
					if (not JS_InstanceOf (context, scaleObj, jsSFVec3f::getClass (), NULL))
					{
						JS_ReportError (context, "Type of argument 3 is invalid - should be SFVec3f, is %s", JS_GetClass (context, scaleObj) -> name);
						return JS_FALSE;
					}

					SFVec3f* scale = (SFVec3f*) JS_GetPrivate (context, scaleObj);

					if (argc >= 4)
					{
						if (not JS_InstanceOf (context, scaleOrientationObj, jsSFRotation::getClass (), NULL))
						{
							JS_ReportError (context, "Type of argument 4 is invalid - should be SFRotation, is %s", JS_GetClass (context, scaleOrientationObj) -> name);
							return JS_FALSE;
						}

						SFRotation* scaleOrientation = (SFRotation*) JS_GetPrivate (context, scaleOrientationObj);

						if (argc >= 5)
						{
							if (not JS_InstanceOf (context, centerObj, jsSFVec3f::getClass (), NULL))
							{
								JS_ReportError (context, "Type of argument 5 is invalid - should be SFVec3f, is %s", JS_GetClass (context, centerObj) -> name);
								return JS_FALSE;
							}

							SFVec3f* center = (SFVec3f*) JS_GetPrivate (context, centerObj);

							sfmatrix4f -> setTransform (*translation, *rotation, *scale, *scaleOrientation, *center);
						}
						else
						{
							sfmatrix4f -> setTransform (*translation, *rotation, *scale, *scaleOrientation);
						}
					}
					else
					{
						sfmatrix4f -> setTransform (*translation, *rotation, *scale);
					}
				}
				else
				{
					sfmatrix4f -> setTransform (*translation, *rotation);
				}
			}
			else
			{
				sfmatrix4f -> setTransform (*translation);
			}
		}
		else
		{
			sfmatrix4f -> setTransform ();
		}

		*rval = JSVAL_VOID;

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::getTransform (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 3)
	{
		SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

		JSObject* translationObj, * rotationObj, * scaleObj;

		if (not JS_ConvertArguments (context, argc, argv, "ooo", &translationObj, &rotationObj, &scaleObj))
			return JS_FALSE;

		if (not JS_InstanceOf (context, translationObj, jsSFVec3f::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, translationObj) -> name);
			return JS_FALSE;
		}

		if (not JS_InstanceOf (context, rotationObj, jsSFRotation::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFRotation, is %s", JS_GetClass (context, rotationObj) -> name);
			return JS_FALSE;
		}

		if (not JS_InstanceOf (context, scaleObj, jsSFVec3f::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, scaleObj) -> name);
			return JS_FALSE;
		}

		SFVec3f*    translation = (SFVec3f*) JS_GetPrivate (context, translationObj);
		SFRotation* rotation    = (SFRotation*) JS_GetPrivate (context, rotationObj);
		SFVec3f*    scale       = (SFVec3f*) JS_GetPrivate (context, scaleObj);

		sfmatrix4f -> getTransform (*translation, *rotation, *scale);

		*rval = JSVAL_VOID;

		return JS_TRUE;
	}

	return JS_TRUE;
}

JSBool
jsVrmlMatrix::inverse (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

		return create (context, sfmatrix4f -> inverse (), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::transpose (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

		return create (context, sfmatrix4f -> transpose (), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::multLeft (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFMatrix4f* sfmatrix4f1 = (SFMatrix4f*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be VrmlMatrix, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFMatrix4f* sfmatrix4f2 = (SFMatrix4f*) JS_GetPrivate (context, obj2);

		return create (context, sfmatrix4f1 -> multLeft (*sfmatrix4f2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::multRight (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFMatrix4f* sfmatrix4f1 = (SFMatrix4f*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be VrmlMatrix, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFMatrix4f* sfmatrix4f2 = (SFMatrix4f*) JS_GetPrivate (context, obj2);

		return create (context, sfmatrix4f1 -> multRight (*sfmatrix4f2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::multVecMatrix (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, jsSFVec3f::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3f* vec = (SFVec3f*) JS_GetPrivate (context, obj2);

		return jsSFVec3f::create (context, sfmatrix4f -> multVecMatrix (*vec), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsVrmlMatrix::multMatrixVec (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFMatrix4f* sfmatrix4f = (SFMatrix4f*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, jsSFVec3f::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3f* vec = (SFVec3f*) JS_GetPrivate (context, obj2);

		return jsSFVec3f::create (context, sfmatrix4f -> multMatrixVec (*vec), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // Titania

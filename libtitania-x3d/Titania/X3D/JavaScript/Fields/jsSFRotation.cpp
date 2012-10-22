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

#include "jsSFRotation.h"

#include "jsSFVec3f.h"

namespace Titania {
namespace X3D {

const size_t jsSFRotation::size = 4;

JSClass jsSFRotation::static_class = {
	"SFRotation", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsSFRotation::properties [ ] = {
	{ "x",     X,     JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "y",     Y,     JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "z",     Z,     JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "angle", ANGLE, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

JSFunctionSpec jsSFRotation::functions [ ] = {
	{ "getAxis",   getAxis,  0, 0, 0 },
	{ "setAxis",   getAxis,  0, 0, 0 },
	{ "inverse",   inverse,  0, 0, 0 },
	{ "multiply",  multiply, 1, 0, 0 },
	{ "multVec",   multVec,  1, 0, 0 },
	{ "slerp",     slerp,    2, 0, 0 },
	{ "toString",  toString, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }

};

void
jsSFRotation::init (JSContext* context, JSObject* global)
{
	JSObject* proto = JS_InitClass (context, global, NULL, &static_class, construct,
	                                0, properties, functions, NULL, NULL);

	JS_DefineProperty (context, proto, (char*) X,     NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Y,     NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Z,     NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) ANGLE, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
}

JSBool
jsSFRotation::create (JSContext* context, SFRotation* field, jsval* vp, const bool seal)
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
jsSFRotation::construct (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		JS_SetPrivate (context, obj, new SFRotation ());
		return JS_TRUE;
	}
	else if (argc == 2)
	{
		JSObject* obj2;

		if (not JS_ValueToObject (context, argv [0], &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, jsSFVec3f::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		JSObject* obj3;

		if (not JS_ValueToObject (context, argv [1], &obj3))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj3, jsSFVec3f::getClass (), NULL))
		{
			jsdouble angle;

			if (not JS_ValueToNumber (context, argv [1], &angle))
			{
				JS_ReportError (context, "Type of argument 2 is invalid - should be SFVec3f or Number, is %s", JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			SFVec3f* vector = (SFVec3f*) JS_GetPrivate (context, obj2);

			JS_SetPrivate (context, obj, new SFRotation (*vector, angle));

			return JS_TRUE;
		}

		SFVec3f* fromVec = (SFVec3f*) JS_GetPrivate (context, obj2);
		SFVec3f* toVec   = (SFVec3f*) JS_GetPrivate (context, obj3);

		JS_SetPrivate (context, obj, new SFRotation (*fromVec, *toVec));

		return JS_TRUE;
	}
	else if (argc == 4)
	{
		jsdouble x, y, z, angle;

		if (not JS_ConvertArguments (context, argc, argv, "dddd", &x, &y, &z, &angle))
			return JS_FALSE;

		JS_SetPrivate (context, obj, new SFRotation (x, y, z, angle));

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFRotation::get1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFRotation* sfrotation = (SFRotation*) JS_GetPrivate (context, obj);

	return JS_NewDoubleValue (context, sfrotation -> get1Value (JSVAL_TO_INT (id)), vp);
}

JSBool
jsSFRotation::set1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFRotation* sfrotation = (SFRotation*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	sfrotation -> set1Value (JSVAL_TO_INT (id), value);

	return JS_TRUE;
}

JSBool
jsSFRotation::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
jsSFRotation::getAxis (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFRotation* sfrotation = (SFRotation*) JS_GetPrivate (context, obj);

		return jsSFVec3f::create (context, sfrotation -> getAxis (), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFRotation::setAxis (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFRotation* sfrotation = (SFRotation*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, jsSFVec3f::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3f* sfvec3f = (SFVec3f*) JS_GetPrivate (context, obj2);

		sfrotation -> setAxis (*sfvec3f);

		*rval = JSVAL_VOID;

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFRotation::inverse (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFRotation* sfrotation = (SFRotation*) JS_GetPrivate (context, obj);

		return create (context, sfrotation -> inverse (), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFRotation::multiply (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFRotation* sfrotation1 = (SFRotation*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFRotation, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFRotation* sfrotation2 = (SFRotation*) JS_GetPrivate (context, obj2);

		return create (context, sfrotation1 -> multiply (*sfrotation2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFRotation::multVec (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFRotation* sfrotation = (SFRotation*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, jsSFVec3f::getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3f, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3f* sfvec3f = (SFVec3f*) JS_GetPrivate (context, obj2);

		return jsSFVec3f::create (context, sfrotation -> multVec (*sfvec3f), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFRotation::slerp (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 2)
	{
		SFRotation* sfrotation = (SFRotation*) JS_GetPrivate (context, obj);

		JSObject* obj2;
		jsdouble  t;

		if (not JS_ConvertArguments (context, argc, argv, "od", &obj2, &t))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFRotation, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFRotation* dest = (SFRotation*) JS_GetPrivate (context, obj2);

		return create (context, sfrotation -> slerp (*dest, t), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // Titania

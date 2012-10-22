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

#include "jsSFVec3d.h"

namespace Titania {
namespace X3D {

const size_t jsSFVec3d::size = 3;

JSClass jsSFVec3d::static_class = {
	"SFVec3d", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsSFVec3d::properties [ ] = {
	{ "x", X, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "y", Y, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "z", Z, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

JSFunctionSpec jsSFVec3d::functions [ ] = {
	{ "negate",    normalize, 0, 0, 0 },
	{ "add",       add,       1, 0, 0 },
	{ "subtract",  subtract,  1, 0, 0 },
	{ "multiply",  multiply,  1, 0, 0 },
	{ "divide",    divide,    1, 0, 0 },
	{ "normalize", normalize, 0, 0, 0 },
	{ "dot",       dot,       1, 0, 0 },
	{ "cross",     cross,     1, 0, 0 },
	{ "length",    length,    0, 0, 0 },
	{ "toString",  toString,  0, 0, 0 },
	{ 0, 0, 0, 0, 0 }

};

void
jsSFVec3d::init (JSContext* context, JSObject* global)
{
	JSObject* proto = JS_InitClass (context, global, NULL, &static_class, construct,
	                                0, properties, functions, NULL, NULL);

	JS_DefineProperty (context, proto, (char*) X, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Y, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Z, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
}

JSBool
jsSFVec3d::create (JSContext* context, SFVec3d* field, jsval* vp, const bool seal)
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
jsSFVec3d::construct (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	jsdouble x = 0;
	jsdouble y = 0;
	jsdouble z = 0;

	if (argc == 1)
	{
		if (not JS_ConvertArguments (context, argc, argv, "d", &x))
			return JS_FALSE;
	}
	else if (argc == 2)
	{
		if (not JS_ConvertArguments (context, argc, argv, "dd", &x, &y))
			return JS_FALSE;
	}
	else if (argc > 2)
	{
		if (not JS_ConvertArguments (context, argc, argv, "ddd", &x, &y, &z))
			return JS_FALSE;
	}

	JS_SetPrivate (context, obj, new SFVec3d (x, y, z));

	return JS_TRUE;
}

JSBool
jsSFVec3d::get1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFVec3d* sfvec3d = (SFVec3d*) JS_GetPrivate (context, obj);

	return JS_NewDoubleValue (context, sfvec3d -> get1Value (JSVAL_TO_INT (id)), vp);
}

JSBool
jsSFVec3d::set1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFVec3d* sfvec3d = (SFVec3d*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	sfvec3d -> set1Value (JSVAL_TO_INT (id), value);

	return JS_TRUE;
}

JSBool
jsSFVec3d::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
jsSFVec3d::negate (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFVec3d* sfvec3d = (SFVec3d*) JS_GetPrivate (context, obj);

		return create (context, sfvec3d -> negate (), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::add (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFVec3d* sfvec3d1 = (SFVec3d*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3d, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3d* sfvec3d2 = (SFVec3d*) JS_GetPrivate (context, obj2);

		return create (context, sfvec3d1 -> add (*sfvec3d2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::subtract (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFVec3d* sfvec3d1 = (SFVec3d*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3d, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3d* sfvec3d2 = (SFVec3d*) JS_GetPrivate (context, obj2);

		return create (context, sfvec3d1 -> subtract (*sfvec3d2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::multiply (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFVec3d* sfvec3d1 = (SFVec3d*) JS_GetPrivate (context, obj);

		jsdouble sfvec3d2;

		if (not JS_ConvertArguments (context, argc, argv, "d", &sfvec3d2))
			return JS_FALSE;

		return create (context, sfvec3d1 -> multiply (sfvec3d2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::divide (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFVec3d* sfvec3d1 = (SFVec3d*) JS_GetPrivate (context, obj);

		jsdouble sfvec3d2;

		if (not JS_ConvertArguments (context, argc, argv, "d", &sfvec3d2))
			return JS_FALSE;

		return create (context, sfvec3d1 -> divide (sfvec3d2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::dot (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFVec3d* sfvec3d1 = (SFVec3d*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3d, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3d* sfvec3d2 = (SFVec3d*) JS_GetPrivate (context, obj2);

		return JS_NewDoubleValue (context, sfvec3d1 -> dot (*sfvec3d2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::cross (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 1)
	{
		SFVec3d* sfvec3d1 = (SFVec3d*) JS_GetPrivate (context, obj);

		JSObject* obj2;

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec3d, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec3d* sfvec3d2 = (SFVec3d*) JS_GetPrivate (context, obj2);

		return create (context, sfvec3d1 -> cross (*sfvec3d2), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::normalize (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFVec3d* sfvec3d = (SFVec3d*) JS_GetPrivate (context, obj);

		return create (context, sfvec3d -> normalize (), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec3d::length (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFVec3d* sfvec3d = (SFVec3d*) JS_GetPrivate (context, obj);

		return JS_NewDoubleValue (context, sfvec3d -> length (), rval);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // Titania

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

#include "jsSFVec4d.h"

namespace titania {
namespace X3D {

const size_t jsSFVec4d::size = 4;

JSClass jsSFVec4d::static_class = {
	"SFVec4d", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsSFVec4d::properties [ ] = {
	{ "x", X, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "y", Y, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "z", Z, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "w", W, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

JSFunctionSpec jsSFVec4d::functions [ ] = {
	{ "getName",     getName,     0, 0 },
	{ "getTypeName", getTypeName, 0, 0 },
	{ "getType",     getType,     0, 0 },
	
	{ "negate",      normalize,   0, 0 },
	{ "add",         add,         1, 0 },
	{ "subtract",    subtract,    1, 0 },
	{ "multiply",    multiply,    1, 0 },
	{ "divide",      divide,      1, 0 },
	{ "normalize",   normalize,   0, 0 },
	{ "dot",         dot,         1, 0 },
	{ "length",      length,      0, 0 },
						  			      
	{ "toString",    toString,    0, 0 },
	
	{ 0 }

};

void
jsSFVec4d::init (JSContext* context, JSObject* global)
{
	JSObject* proto = JS_InitClass (context, global, NULL, &static_class, construct,
	                                0, properties, functions, NULL, NULL);

	JS_DefineProperty (context, proto, (char*) X, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Y, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Z, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) W, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
}

JSBool
jsSFVec4d::create (JSContext* context, SFVec4d* field, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, field);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsSFVec4d::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new SFVec4d (), &JS_RVAL (context, vp));
	}
	else if (argc == 4)
	{
		jsdouble x = 0;
		jsdouble y = 0;
		jsdouble z = 0;
		jsdouble w = 0;
	
		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "dddd", &x, &y, &z, &w))
			return JS_FALSE;
			
		return create (context, new SFVec4d (x, y, z, w), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
		case JSENUMERATE_INIT_ALL:
		{
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				*idp = INT_TO_JSVAL (size);

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < size)
			{
				if (idp)
					*idp = INT_TO_JSVAL (*index);

				*index = *index + 1;
				break;
			}

		//else done -- cleanup.
		}
		case JSENUMERATE_DESTROY:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);
			delete index;
			*statep = JSVAL_NULL;
		}
	}

	return JS_TRUE;
}

JSBool
jsSFVec4d::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	SFVec4d* sfvec4d = (SFVec4d*) JS_GetPrivate (context, obj);

	return JS_NewNumberValue (context, sfvec4d -> get1Value (JSID_TO_INT (id)), vp);
}

JSBool
jsSFVec4d::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	SFVec4d* sfvec4d = (SFVec4d*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	sfvec4d -> set1Value (JSID_TO_INT (id), value);

	return JS_TRUE;
}

JSBool
jsSFVec4d::negate (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFVec4d* sfvec4d = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));
		
		return create (context, sfvec4d -> negate (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::add (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		SFVec4d* sfvec4d1 = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* obj2;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec4d, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec4d* sfvec4d2 = (SFVec4d*) JS_GetPrivate (context, obj2);

		return create (context, sfvec4d1 -> add (*sfvec4d2), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::subtract (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		SFVec4d* sfvec4d1 = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* obj2;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec4d, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec4d* sfvec4d2 = (SFVec4d*) JS_GetPrivate (context, obj2);

		return create (context, sfvec4d1 -> subtract (*sfvec4d2), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::multiply (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		SFVec4d* sfvec4d1 = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		jsdouble value;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "d", &value))
			return JS_FALSE;

		return create (context, sfvec4d1 -> multiply (value), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::divide (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		SFVec4d* sfvec4d1 = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		jsdouble value;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "d", &value))
			return JS_FALSE;

		return create (context, sfvec4d1 -> divide (value), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::dot (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		SFVec4d* sfvec4d1 = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* obj2;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &obj2))
			return JS_FALSE;

		if (not JS_InstanceOf (context, obj2, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFVec4d, is %s", JS_GetClass (context, obj2) -> name);
			return JS_FALSE;
		}

		SFVec4d* sfvec4d2 = (SFVec4d*) JS_GetPrivate (context, obj2);

		return JS_NewNumberValue (context, sfvec4d1 -> dot (*sfvec4d2), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::normalize (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFVec4d* sfvec4d = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return create (context, sfvec4d -> normalize (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFVec4d::length (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFVec4d* sfvec4d = (SFVec4d*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewNumberValue (context, sfvec4d -> length (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // titania

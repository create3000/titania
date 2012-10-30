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
 ******************************************************************************/

#include "jsSFColorRGBA.h"

namespace Titania {
namespace X3D {

const size_t jsSFColorRGBA::size = 4;

JSClass jsSFColorRGBA::static_class = {
	"SFColorRGBA", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsSFColorRGBA::properties [ ] = {
	{ "r", R, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "g", G, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "b", B, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "a", A, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

JSFunctionSpec jsSFColorRGBA::functions [ ] = {
	{ "getHSV",   getHSV,   0, 0, 0 },
	{ "setHSV",   setHSV,   3, 0, 0 },
	{ "toString", toString, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }

};

void
jsSFColorRGBA::init (JSContext* context, JSObject* global)
{
	JSObject* proto = JS_InitClass (context, global, NULL, &static_class, construct,
	                                0, properties, functions, NULL, NULL);

	JS_DefineProperty (context, proto, (char*) R, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) G, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) B, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) A, NULL, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
}

JSBool
jsSFColorRGBA::create (JSContext* context, SFColorRGBA* field, jsval* vp, const bool seal)
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
jsSFColorRGBA::construct (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		JS_SetPrivate (context, obj, new SFColorRGBA ());
		return JS_TRUE;
	}
	else if (argc == size)
	{
		jsdouble r, g, b, a;

		if (not JS_ConvertArguments (context, argc, argv, "dddd", &r, &g, &b, &a))
			return JS_FALSE;

		JS_SetPrivate (context, obj, new SFColorRGBA (r, g, b, a));

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFColorRGBA::get1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFColorRGBA* sfcolorrgba = (SFColorRGBA*) JS_GetPrivate (context, obj);

	return JS_NewDoubleValue (context, sfcolorrgba -> get1Value (JSVAL_TO_INT (id)), vp);
}

JSBool
jsSFColorRGBA::set1Value (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	SFColorRGBA* sfcolorrgba = (SFColorRGBA*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	sfcolorrgba -> set1Value (JSVAL_TO_INT (id), value);

	return JS_TRUE;
}

JSBool
jsSFColorRGBA::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
jsSFColorRGBA::getHSV (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 0)
	{
		SFColorRGBA* sfcolorrgba = (SFColorRGBA*) JS_GetPrivate (context, obj);

		float h, s, v;

		sfcolorrgba -> getHSV (h, s, v);

		jsval vector [3];

		if (not JS_NewDoubleValue (context, h, &vector [0]))
			return JS_FALSE;

		if (not JS_NewDoubleValue (context, s, &vector [1]))
			return JS_FALSE;

		if (not JS_NewDoubleValue (context, v, &vector [2]))
			return JS_FALSE;

		JSObject* result = JS_NewArrayObject (context, 3, vector);

		*rval = OBJECT_TO_JSVAL (result);

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFColorRGBA::setHSV (JSContext* context, JSObject* obj, uintN argc, jsval* argv, jsval* rval)
{
	if (argc == 3)
	{
		SFColorRGBA* sfcolorrgba = (SFColorRGBA*) JS_GetPrivate (context, obj);

		jsdouble h, s, v;

		if (not JS_ConvertArguments (context, argc, argv, "ddd", &h, &s, &v))
			return JS_FALSE;

		sfcolorrgba -> setHSV (h, s, v);

		*rval = JSVAL_VOID;

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // Titania

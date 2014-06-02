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

#include "jsSFColor.h"

#include "../jsContext.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

const size_t jsSFColor::size = 3;

JSClass jsSFColor::static_class = {
	"SFColor", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsSFColor::properties [ ] = {
	{ "r", R, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "g", G, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "b", B, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

JSFunctionSpec jsSFColor::functions [ ] = {
	{ "getName",     getName <X3DChildObject>,     0, 0 },
	{ "getTypeName", getTypeName <X3DChildObject>, 0, 0 },
	{ "getType",     getType,                      0, 0 },
	{ "isReadable",  isReadable,                   0, 0 },
	{ "isWritable",  isWritable,                   0, 0 },

	{ "getHSV",      getHSV,   0, 0 },
	{ "setHSV",      setHSV,   3, 0 },

	{ "toString",    toString <X3DChildObject>, 0, 0 },

	{ 0 }

};

void
jsSFColor::init (JSContext* context, JSObject* global)
{
	JSObject* proto = JS_InitClass (context, global, NULL, &static_class, construct,
	                                0, properties, functions, NULL, NULL);

	JS_DefineProperty (context, proto, (char*) R, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) G, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) B, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
}

JSBool
jsSFColor::create (JSContext* const context, SFColor* const field, jsval* const vp)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

	try
	{
		*vp = OBJECT_TO_JSVAL (javaScript -> getObject (field));
	}
	catch (const std::out_of_range &)
	{
		JSObject* const result = JS_NewObject (context, &static_class, NULL, NULL);

		if (result == NULL)
			return JS_FALSE;

		JS_SetPrivate (context, result, field);

		javaScript -> addObject (field, result);

		*vp = OBJECT_TO_JSVAL (result);
	}

	return JS_TRUE;
}

JSBool
jsSFColor::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new SFColor (), &JS_RVAL (context, vp));
	}
	else if (argc == size)
	{
		jsdouble r, g, b;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "ddd", &r, &g, &b))
			return JS_FALSE;

		return create (context, new SFColor (r, g, b), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFColor::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
				*idp = INT_TO_JSID (size);

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < size)
			{
				if (idp)
					*idp = INT_TO_JSID (*index);

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
jsSFColor::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	SFColor* const sfcolor = (SFColor*) JS_GetPrivate (context, obj);

	return JS_NewNumberValue (context, sfcolor -> get1Value (JSID_TO_INT (id)), vp);
}

JSBool
jsSFColor::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	SFColor* const sfcolor = (SFColor*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	sfcolor -> set1Value (JSID_TO_INT (id), value);

	return JS_TRUE;
}

JSBool
jsSFColor::getHSV (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		SFColor* const sfcolor = (SFColor*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		float h, s, v;

		sfcolor -> getHSV (h, s, v);

		jsval vector [3];

		if (not JS_NewNumberValue (context, h, &vector [0]))
			return JS_FALSE;

		if (not JS_NewNumberValue (context, s, &vector [1]))
			return JS_FALSE;

		if (not JS_NewNumberValue (context, v, &vector [2]))
			return JS_FALSE;

		JSObject* const result = JS_NewArrayObject (context, 3, vector);

		JS_SET_RVAL (context, vp, OBJECT_TO_JSVAL (result));

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsSFColor::setHSV (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 3)
	{
		SFColor* const sfcolor = (SFColor*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		jsdouble h, s, v;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "ddd", &h, &s, &v))
			return JS_FALSE;

		sfcolor -> setHSV (h, s, v);

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // MozillaSpiderMonkey
} // X3D
} // titania

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

#include "jsMFVec2f.h"

namespace titania {
namespace X3D {

JSClass jsMFVec2f::static_class = {
	"MFVec2f", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

void
jsMFVec2f::init (JSContext* context, JSObject* global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, properties, functions, NULL, NULL);
}

JSBool
jsMFVec2f::create (JSContext* context, MFVec2f* field, jsval* vp, const bool seal)
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
jsMFVec2f::construct (JSContext* context, uintN argc, jsval* vp)
{
	JSObject* obj = JS_THIS_OBJECT (context, vp);

	if (argc == 0)
	{
		JS_SetPrivate (context, obj, new MFVec2f ());
		return JS_TRUE;
	}
	else
	{
		SFVec2f values [argc];

		jsval* argv = JS_ARGV (context, vp);

		for (uintN i = 0; i < argc; ++ i)
		{
			JSObject* obj2;

			if (not JS_ValueToObject (context, argv [i], &obj2))
				return JS_FALSE;

			if (not JS_InstanceOf (context, obj2, jsSFVec2f::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument %d is invalid - should be SFVec2f, is %s", i, JS_GetClass (context, obj2) -> name);
				return JS_FALSE;
			}

			values [i] = *(SFVec2f*) JS_GetPrivate (context, obj2);
		}

		JS_SetPrivate (context, obj, new MFVec2f (values, values + argc));

		return JS_TRUE;
	}

	return JS_FALSE;
}

JSBool
jsMFVec2f::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	MFVec2f* x3darrayfield = (MFVec2f*) JS_GetPrivate (context, obj);

	if (not x3darrayfield)
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
				JS_ValueToId (context, INT_TO_JSVAL (x3darrayfield -> size ()), idp);

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < x3darrayfield -> size ())
			{
				JS_DefineProperty (context,
				                   obj, (char*) *index,
				                   JSVAL_VOID,
				                   get1Value, set1Value,
				                   JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT);

				if (idp)
					JS_ValueToId (context, INT_TO_JSVAL (*index), idp);

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
jsMFVec2f::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	if (not JSVAL_IS_INT (id))
		return JS_PropertyStub (context, obj, id, vp);

	int32 index = JSVAL_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	MFVec2f* mfvec2f = (MFVec2f*) JS_GetPrivate (context, obj);

	return jsSFVec2f::create (context, &mfvec2f -> get1Value (index), vp);
}

JSBool
jsMFVec2f::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (not JSVAL_IS_INT (id))
		return JS_PropertyStub (context, obj, id, vp);

	int32 index = JSVAL_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	MFVec2f* mfvec2f = (MFVec2f*) JS_GetPrivate (context, obj);

	JSObject* obj2;

	if (not JS_ValueToObject (context, *vp, &obj2))
		return JS_FALSE;

	if (not JS_InstanceOf (context, obj2, jsSFVec2f::getClass (), NULL))
	{
		JS_ReportError (context, "Type of argument is invalid - should be SFVec2f, is %s", JS_GetClass (context, obj2) -> name);
		return JS_FALSE;
	}

	mfvec2f -> set1Value (index, *(SFVec2f*) JS_GetPrivate (context, obj2));

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

} // X3D
} // titania

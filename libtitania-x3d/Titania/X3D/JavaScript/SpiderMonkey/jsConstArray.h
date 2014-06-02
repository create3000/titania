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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_CONST_ARRAY_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_CONST_ARRAY_H__

#include <jsapi.h>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type, class ValueType>
class jsConstArray
{
public:

	static
	void
	init (JSContext* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, const Type* const, jsval* const);

	static
	JSClass*
	getClass () { return &static_class; }


private:

	enum Property {LENGTH};

	static
	JSBool
	enumerate (JSContext *, JSObject *, JSIterateOp, jsval *, jsid*);

	static JSBool get1Value (JSContext *, JSObject *, jsid, jsval*);

	static JSBool length (JSContext *, JSObject *, jsid, jsval*);

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class Type, class ValueType>
JSClass jsConstArray <Type, ValueType>::static_class = {
	"jsConstArray", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type, class ValueType>
JSPropertySpec jsConstArray <Type, ValueType>::properties [ ] = {
	{ "length", LENGTH, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, length, NULL },
	{ 0 }

};

template <class Type, class ValueType>
JSFunctionSpec jsConstArray <Type, ValueType>::functions [ ] = {
	{ 0, 0, 0, 0 }

};

template <class Type, class ValueType>
void
jsConstArray <Type, ValueType>::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, NULL,
	              0, properties, functions, NULL, NULL);
}

template <class Type, class ValueType>
JSBool
jsConstArray <Type, ValueType>::create (JSContext* const context, const Type* const array, jsval* const vp)
{
	JSObject* const result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, const_cast <Type*> (array));

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

template <class Type, class ValueType>
JSBool
jsConstArray <Type, ValueType>::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	const auto array = static_cast <Type*> (JS_GetPrivate (context, obj));

	if (not array)
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
				*idp = INT_TO_JSID (array -> size ());

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < array -> size ())
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

template <class Type, class ValueType>
JSBool
jsConstArray <Type, ValueType>::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	const int32 index = JSID_TO_INT (id);
	const auto  array = static_cast <Type*> (JS_GetPrivate (context, obj));

	if (index < 0 and index >= (int32) array -> size ())
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	return ValueType::create (context, array -> at (index), vp);
}

template <class Type, class ValueType>
JSBool
jsConstArray <Type, ValueType>::length (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	const auto array = static_cast <Type*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, array -> size (), vp);
}

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC4_H__

#include "../../../Fields/SFVec4.h"
#include "../jsX3DField.h"
#include "../jsContext.h"

namespace titania {
namespace X3D {

template <class Type>
class jsSFVec4 :
	public jsX3DField
{
public:

	typedef Type field_type;

	static
	void
	init (JSContext*, JSObject*);

	static
	JSBool
	create (JSContext*, Type*, jsval*, const bool = false);

	static
	JSClass*
	getClass () { return &static_class; }


private:

	enum Property {X, Y, Z, W};

	static JSBool construct (JSContext *, uintN, jsval*);
	static JSBool enumerate (JSContext *, JSObject *, JSIterateOp, jsval *, jsid*);

	static JSBool get1Value (JSContext *, JSObject *, jsid, jsval*);
	static JSBool set1Value (JSContext *, JSObject *, jsid, JSBool, jsval*);

	static JSBool negate    (JSContext *, uintN, jsval*);
	static JSBool add       (JSContext *, uintN, jsval*);
	static JSBool subtract  (JSContext *, uintN, jsval*);
	static JSBool multiply  (JSContext *, uintN, jsval*);
	static JSBool divide    (JSContext *, uintN, jsval*);
	static JSBool dot       (JSContext *, uintN, jsval*);
	static JSBool normalize (JSContext *, uintN, jsval*);
	static JSBool length    (JSContext *, uintN, jsval*);

	static const size_t   size;
	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class Type>
const size_t jsSFVec4 <Type>::size = 4;

template <class Type>
JSClass jsSFVec4 <Type>::static_class = {
	"SFVec4", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type>
JSPropertySpec jsSFVec4 <Type>::properties [ ] = {
	{ "x", X, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "y", Y, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "z", Z, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "w", W, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

template <class Type>
JSFunctionSpec jsSFVec4 <Type>::functions [ ] = {
	{ "getName",     getName <X3DChildObject>,     0, 0 },
	{ "getTypeName", getTypeName <X3DChildObject>, 0, 0 },
	{ "getType",     getType,                      0, 0 },
	{ "isReadable",  isReadable,                   0, 0 },
	{ "isWritable",  isWritable,                   0, 0 },

	{ "negate",      negate,      0, 0 },
	{ "add",         add,         1, 0 },
	{ "subtract",    subtract,    1, 0 },
	{ "multiply",    multiply,    1, 0 },
	{ "divide",      divide,      1, 0 },
	{ "normalize",   normalize,   0, 0 },
	{ "dot",         dot,         1, 0 },
	{ "length",      length,      0, 0 },

	{ "toString",    toString <X3DChildObject>, 0, 0 },

	{ 0 }

};

template <class Type>
void
jsSFVec4 <Type>::init (JSContext* context, JSObject* global)
{
	JSObject* proto = JS_InitClass (context, global, NULL, &static_class, construct,
	                                0, properties, functions, NULL, NULL);

	JS_DefineProperty (context, proto, (char*) X, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Y, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) Z, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (context, proto, (char*) W, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
}

template <class Type>
JSBool
jsSFVec4 <Type>::create (JSContext* context, Type* field, jsval* vp, const bool seal)
{
	auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

	try
	{
		*vp = OBJECT_TO_JSVAL (javaScript -> getObject (field));
	}
	catch (const std::out_of_range &)
	{
		JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

		if (result == NULL)
			return JS_FALSE;

		JS_SetPrivate (context, result, field);

		//if (seal)
		//	JS_SealObject (context, result, JS_FALSE);

		javaScript -> addObject (field, result);

		*vp = OBJECT_TO_JSVAL (result);
	}

	return JS_TRUE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new Type (), &JS_RVAL (context, vp));
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

		return create (context, new Type (x, y, z, w), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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

template <class Type>
JSBool
jsSFVec4 <Type>::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	Type* self = (Type*) JS_GetPrivate (context, obj);

	return JS_NewNumberValue (context, self -> get1Value (JSID_TO_INT (id)), vp);
}

template <class Type>
JSBool
jsSFVec4 <Type>::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	Type* self = (Type*) JS_GetPrivate (context, obj);

	jsdouble value;

	if (not JS_ValueToNumber (context, *vp, &value))
		return JS_FALSE;

	self -> set1Value (JSID_TO_INT (id), value);

	return JS_TRUE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::negate (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return create (context, self -> negate (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::add (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (not JS_InstanceOf (context, rhs, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be  %s, is %s", getClass () -> name, JS_GetClass (context, rhs) -> name);
			return JS_FALSE;
		}

		Type* vector = (Type*) JS_GetPrivate (context, rhs);

		return create (context, self -> add (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::subtract (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (not JS_InstanceOf (context, rhs, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be  %s, is %s", getClass () -> name, JS_GetClass (context, rhs) -> name);
			return JS_FALSE;
		}

		Type* vector = (Type*) JS_GetPrivate (context, rhs);

		return create (context, self -> subtract (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::multiply (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		jsdouble value;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "d", &value))
			return JS_FALSE;

		return create (context, self -> multiply (value), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::divide (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		jsdouble value;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "d", &value))
			return JS_FALSE;

		return create (context, self -> divide (value), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::dot (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		JSObject* rhs;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &rhs))
			return JS_FALSE;

		if (not JS_InstanceOf (context, rhs, getClass (), NULL))
		{
			JS_ReportError (context, "Type of argument 1 is invalid -  %s, is %s", getClass () -> name, JS_GetClass (context, rhs) -> name);
			return JS_FALSE;
		}

		Type* vector = (Type*) JS_GetPrivate (context, rhs);

		return JS_NewNumberValue (context, self -> dot (*vector), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::normalize (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return create (context, self -> normalize (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type>
JSBool
jsSFVec4 <Type>::length (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		Type* self = (Type*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		return JS_NewNumberValue (context, self -> length (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

extern template class jsSFVec4 <SFVec4d>;
extern template class jsSFVec4 <SFVec4f>;

typedef jsSFVec4 <SFVec4d> jsSFVec4d;
typedef jsSFVec4 <SFVec4f> jsSFVec4f;

} // X3D
} // titania

#endif

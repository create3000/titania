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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__

#include "jsContext.h"
#include "jsString.h"
#include "jsX3DField.h"
#include "jsError.h"

namespace titania {
namespace X3D {

template <class Type, class FieldType>
class jsX3DArrayField :
	public jsX3DField
{
public:

	typedef Type                           value_type;
	typedef FieldType                      field_type;
	typedef typename FieldType::value_type field_value_type;

	static
	void
	init (JSContext* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, FieldType* const, jsval* const, const bool = false);

	static
	JSClass*
	getClass ()
	{ return &static_class; }


private:

	enum Property {LENGTH};

	static JSBool construct (JSContext *, uintN, jsval*);

	static
	JSBool
	enumerate (JSContext *, JSObject *, JSIterateOp, jsval *, jsid*);

	static JSBool get1Value (JSContext *, JSObject *, jsid, jsval*);
	static JSBool set1Value (JSContext *, JSObject *, jsid, JSBool, jsval*);

	static JSBool unshift (JSContext *, uintN, jsval*);
	static JSBool push    (JSContext *, uintN, jsval*);
	static JSBool shift   (JSContext *, uintN, jsval*);
	static JSBool pop     (JSContext *, uintN, jsval*);

	static JSBool length (JSContext *, JSObject *, jsid, jsval*);
	static JSBool length (JSContext *, JSObject *, jsid, JSBool, jsval*);

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class Type, class FieldType>
JSClass jsX3DArrayField <Type, FieldType>::static_class = {
	"X3DArrayField", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type, class FieldType>
JSPropertySpec jsX3DArrayField <Type, FieldType>::properties [ ] = {
	{ "length", LENGTH, JSPROP_SHARED | JSPROP_PERMANENT, length, length },
	{ 0 }

};

template <class Type, class FieldType>
JSFunctionSpec jsX3DArrayField <Type, FieldType>::functions [ ] = {
	{ "getName",     getName <X3DChildObject>,     0, 0 },
	{ "getTypeName", getTypeName <X3DChildObject>, 0, 0 },
	{ "getType",     getType,                      0, 0 },
	{ "isReadable",  isReadable,                   0, 0 },
	{ "isWritable",  isWritable,                   0, 0 },

	{ "unshift",     unshift, 1, 0 },
	{ "push",        push,    1, 0 },
	{ "shift",       shift,   0, 0 },
	{ "pop",         pop,     0, 0 },

	{ "toString",    toString <X3DChildObject>,    0, 0 },

	{ 0 }

};

template <class Type, class FieldType>
void
jsX3DArrayField <Type, FieldType>::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, properties, functions, NULL, NULL);
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::create (JSContext* const context, FieldType* const field, jsval* const vp, const bool seal)
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

		//if (seal)
		//	JS_SealObject (context, result, JS_FALSE);

		javaScript -> addObject (field, result);

		*vp = OBJECT_TO_JSVAL (result);
	}

	return JS_TRUE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new field_type (), &JS_RVAL (context, vp));
	}
	else
	{
		field_type* const field = new field_type ();

		jsval* const argv = JS_ARGV (context, vp);

		for (uintN i = 0; i < argc; ++ i)
		{
			JSObject* value = nullptr;

			if (not JS_ValueToObject (context, argv [i], &value))
				return JS_FALSE;
			
			if (JS_InstanceOfError (context, value, value_type::getClass ()))
				return JS_FALSE;

			field -> emplace_back (*(field_value_type*) JS_GetPrivate (context, value));
		}

		return create (context, field, &JS_RVAL (context, vp));
	}

	return JS_FALSE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	FieldType* const field = static_cast <FieldType*> (JS_GetPrivate (context, obj));

	if (not field)
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
				*idp = INT_TO_JSID (field -> size ());

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < field -> size ())
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

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	const int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "%s .get1Value: index out of range.", getClass () -> name);
		return JS_FALSE;
	}

	FieldType* field = (FieldType*) JS_GetPrivate (context, obj);

	return value_type::create (context, field -> get1Value (index), vp);
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	const int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	JSObject* value = nullptr;

	if (not JS_ValueToObject (context, *vp, &value))
		return JS_FALSE;
		
	if (JS_InstanceOfError (context, value, value_type::getClass ()))
		return JS_FALSE;

	FieldType* const field = (FieldType*) JS_GetPrivate (context, obj);

	field -> set1Value (index, *(field_value_type*) JS_GetPrivate (context, value));

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::unshift (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		JSObject* value = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &value))
			return JS_FALSE;
			
		if (JS_InstanceOfError (context, value, value_type::getClass ()))
			return JS_FALSE;

		FieldType* const field = (FieldType*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		field -> emplace_front (*(field_value_type*) JS_GetPrivate (context, value));

		return JS_NewNumberValue (context, field -> size (), vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::push (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		JSObject* value = nullptr;

		jsval* const argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "o", &value))
			return JS_FALSE;

		if (JS_InstanceOfError (context, value, value_type::getClass ()))
			return JS_FALSE;

		FieldType* const field = (FieldType*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		field -> emplace_back (*(field_value_type*) JS_GetPrivate (context, value));

		return JS_NewNumberValue (context, field -> size (), vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::shift (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		FieldType* const field = (FieldType*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		if (field -> empty ())
		{
			*vp = JSVAL_VOID;

			return JS_TRUE;
		}

		const auto value = new field_value_type (field -> front ());

		field -> pop_front ();

		return value_type::create (context, value, vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::pop (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		FieldType* const field = (FieldType*) JS_GetPrivate (context, JS_THIS_OBJECT (context, vp));

		if (field -> empty ())
		{
			*vp = JSVAL_VOID;

			return JS_TRUE;
		}

		const auto value = new field_value_type (field -> back ());

		field -> pop_back ();

		return value_type::create (context, value, vp);
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::length (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	FieldType* const field = static_cast <FieldType*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, field -> size (), vp);
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::length (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	FieldType* const field = static_cast <FieldType*> (JS_GetPrivate (context, obj));

	uint32 value;

	if (not JS_ValueToECMAUint32 (context, *vp, &value))
		return JS_FALSE;

	field -> resize (value);

	return JS_TRUE;
}

} // X3D
} // titania

#endif

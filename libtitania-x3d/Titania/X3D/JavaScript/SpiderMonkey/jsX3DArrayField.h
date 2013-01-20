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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__

#include "String.h"
#include "jsX3DField.h"

namespace titania {
namespace X3D {

template <class Type, class FieldType>
class jsX3DArrayField :
	public jsX3DField
{
public:

	typedef Type                            value_type;
	typedef FieldType                       field_type;
	typedef typename FieldType::scalar_type field_value_type;

	static
	void
	init (JSContext*, JSObject*);

	static
	JSBool
	create (JSContext*, X3DArray*, jsval*, const bool = false);

	static
	JSClass*
	getClass () { return &static_class; }


private:

	enum Property {LENGTH};

	static JSBool construct (JSContext *, uintN, jsval*);

	static
	JSBool
	enumerate (JSContext *, JSObject *, JSIterateOp, jsval *, jsid*);

	static JSBool get1Value (JSContext *, JSObject *, jsid, jsval*);
	static JSBool set1Value (JSContext *, JSObject *, jsid, JSBool, jsval*);

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
	{ "getName",     getName <X3DArray>,     0, 0 },
	{ "getTypeName", getTypeName <X3DArray>, 0, 0 },
	{ "getType",     getType <X3DArray>,     0, 0 },

	{ "toString",    toString <X3DArray>,    0, 0 },

	{ 0 }

};

template <class Type, class FieldType>
void
jsX3DArrayField <Type, FieldType>::init (JSContext* context, JSObject* global)
{
	JS_InitClass (context, global, NULL, &static_class, construct,
	              0, properties, functions, NULL, NULL);
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::create (JSContext* context, X3DArray* field, jsval* vp, const bool seal)
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
		field_type* field = new field_type ();

		jsval* argv = JS_ARGV (context, vp);

		for (uintN i = 0; i < argc; ++ i)
		{
			JSObject* value;

			if (not JS_ValueToObject (context, argv [i], &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, value_type::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument %d is invalid - should be %s, is %s",
				                i,
				                value_type::getClass () -> name,
				                JS_GetClass (context, value) -> name);

				return JS_FALSE;
			}

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
	X3DArray* field = static_cast <X3DArray*> (JS_GetPrivate (context, obj));

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

	int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	X3DArray* field = (X3DArray*) JS_GetPrivate (context, obj);

	return value_type::create (context, (field_value_type*) &field -> get1Value (index), vp);
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	JSObject* value;

	if (not JS_ValueToObject (context, *vp, &value))
		return JS_FALSE;

	if (not JS_InstanceOf (context, value, value_type::getClass (), NULL))
	{
		JS_ReportError (context, "Type of argument is invalid - should be %s, is %s",
		                value_type::getClass () -> name,
		                JS_GetClass (context, value) -> name);

		return JS_FALSE;
	}

	X3DArray* field = (X3DArray*) JS_GetPrivate (context, obj);

	field -> set1Value (index, *(field_value_type*) JS_GetPrivate (context, value));

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::length (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DArray* field = static_cast <X3DArray*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, field -> size (), vp);
}

template <class Type, class FieldType>
JSBool
jsX3DArrayField <Type, FieldType>::length (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	X3DArray* field = static_cast <X3DArray*> (JS_GetPrivate (context, obj));

	uint32 value;

	if (not JS_ValueToECMAUint32 (context, *vp, &value))
		return JS_FALSE;

	field -> resize (value);

	return JS_TRUE;
}

} // X3D
} // titania

#endif

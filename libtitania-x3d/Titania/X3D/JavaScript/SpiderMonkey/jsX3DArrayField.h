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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_JAEGER_MONKEY_JS_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_JAEGER_MONKEY_JS_X3DARRAY_FIELD_H__

#include "../../Fields/ArrayFields.h"
#include "jsX3DField.h"
#include "String.h"

namespace titania {
namespace X3D {

template <class Type>
class jsX3DArrayField :
	public jsX3DField
{
protected:

	static
	JSBool
	create (JSContext*, JSClass*, X3DArray*, jsval*, const bool);

	static
	JSBool
	enumerate (JSContext*, JSObject*, JSIterateOp, jsval*, jsid*);

	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];


private:

	enum Property {LENGTH};

	static JSBool length (JSContext*, JSObject*, jsid, jsval*);
	static JSBool length (JSContext*, JSObject*, jsid, JSBool, jsval*);
	
};

template <class Type>
JSPropertySpec jsX3DArrayField <Type>::properties [ ] = {
	{ "length", LENGTH, JSPROP_SHARED | JSPROP_PERMANENT, length, length },
	{ 0 }
};

template <class Type>
JSFunctionSpec jsX3DArrayField <Type>::functions [ ] = {
	{ "getName",     getName <X3DArray>,     0, 0 },
	{ "getTypeName", getTypeName <X3DArray>, 0, 0 },
	{ "getType",     getType <X3DArray>,     0, 0 },
	
	{ "toString",    toString <X3DArray>,    0, 0 },
	
	{ 0 }
};

template <class Type>
JSBool
jsX3DArrayField <Type>::create (JSContext* context, JSClass* static_class, X3DArray* field, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, field);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

template <class Type>
JSBool
jsX3DArrayField <Type>::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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

template <class Type>
JSBool
jsX3DArrayField <Type>::length (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DArray* field = static_cast <X3DArray*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, field -> size (), vp);
}

template <class Type>
JSBool
jsX3DArrayField <Type>::length (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
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

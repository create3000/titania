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

#include "jsFieldDefinitionArray.h"
#include "jsX3DFieldDefinition.h"

namespace titania {
namespace X3D {

JSClass jsFieldDefinitionArray::static_class = {
	"FieldDefinitionArray", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsFieldDefinitionArray::properties [ ] = {
	{ "length", LENGTH, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, getLength, setLength },
	{ 0 }

};

void
jsFieldDefinitionArray::initObject (JSContext* context, JSObject* obj)
{
	FieldDefinitionArray* array = (FieldDefinitionArray*) JS_GetPrivate (context, obj);

	if (array)
	{
		for (size_t i = 0; i < array -> size (); ++ i)
			JS_DefineProperty (context, obj, (char*) i, JSVAL_VOID, get1Value, set1Value,
			                   JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	}

	JS_DefineProperties (context, obj, properties);
}

JSBool
jsFieldDefinitionArray::create (JSContext* context, const FieldDefinitionArray* fieldDefinitionArray, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, const_cast <FieldDefinitionArray*> (fieldDefinitionArray));

	initObject (context, result);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsFieldDefinitionArray::enumerate (JSContext* context, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	FieldDefinitionArray* array = (FieldDefinitionArray*) JS_GetPrivate (context, obj);

	if (not array)
	{
		*statep = JSVAL_NULL;
		return JS_TRUE;
	}

	FieldDefinitionArray::size_type size = array -> size ();

	size_t* index;

	switch (enum_op)
	{
		case JSENUMERATE_INIT:
		case JSENUMERATE_INIT_ALL:
		{
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				JS_ValueToId (context, INT_TO_JSVAL (size), idp);

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < size)
			{
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
jsFieldDefinitionArray::get1Value (JSContext *context, JSObject *obj, jsid id, jsval *vp)
{
	if (not JSVAL_IS_INT (id))
		return JS_PropertyStub (context, obj, id, vp);

	FieldDefinitionArray* array = (FieldDefinitionArray*) JS_GetPrivate (context, obj);

	int32 index = JSVAL_TO_INT (id);

	if (index >= 0 and index < (int32) array -> size ())
		return jsX3DFieldDefinition::create (context, array -> at (index), vp);

	return JS_FALSE;
}

JSBool
jsFieldDefinitionArray::set1Value (JSContext *context, JSObject *obj, jsid id, JSBool strict, jsval *vp)
{
	if (not JSVAL_IS_INT (id))
		return JS_PropertyStub (context, obj, id, vp);

	return JS_TRUE;
}

JSBool
jsFieldDefinitionArray::getLength (JSContext *context, JSObject *obj, jsid id, jsval *vp)
{
	FieldDefinitionArray* array = (FieldDefinitionArray*) JS_GetPrivate (context, obj);

	return JS_NewNumberValue (context, array ? array -> size () : 0, vp);
}

JSBool
jsFieldDefinitionArray::setLength (JSContext *context, JSObject *obj, jsid id, JSBool strict, jsval *vp)
{
	return JS_TRUE;
}

} // X3D
} // titania

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

#include "jsContext.h"
#include "jsString.h"
#include "jsX3DFieldDefinition.h"

namespace titania {
namespace X3D {

JSClass jsX3DFieldDefinition::static_class = {
	"X3DFieldDefinition", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DFieldDefinition::properties [ ] = {
	{ "name",       NAME,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, name,       NULL },
	{ "accessType", ACCESSTYPE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, accessType, NULL },
	{ "dataType",   DATATYPE,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, dataType,   NULL },
	{ 0 }

};

void
jsX3DFieldDefinition::initObject (JSContext* context, JSObject* object)
{
	JS_DefineProperties (context, object, properties);
}

JSBool
jsX3DFieldDefinition::create (JSContext* context, X3DFieldDefinition* field, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, field);

	initObject (context, result);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsX3DFieldDefinition::name (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto field = static_cast <X3DFieldDefinition*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, field -> getName (), vp);
}

JSBool
jsX3DFieldDefinition::accessType (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto field = static_cast <X3DFieldDefinition*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, field -> getAccessType (), vp);
}

JSBool
jsX3DFieldDefinition::dataType (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto field = static_cast <X3DFieldDefinition*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, field -> getType (), vp);
}

void
jsX3DFieldDefinition:: finalize (JSContext* context, JSObject* obj)
{
	auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));
	auto field      = static_cast <X3DFieldDefinition*> (JS_GetPrivate (context, obj));

	if (field) // XXX there are sometimes objects without field
	{
		javaScript -> removeObject (field);
	}
}

} // X3D
} // titania

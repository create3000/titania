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

#include "jsX3DFieldDefinition.h"

#include "../../Fields/X3DScalar.h"
#include "jsContext.h"
#include "jsString.h"

namespace titania {
namespace X3D {

using jsX3DFieldDefinitionPrivate = X3DScalar <const X3DFieldDefinition*>;

JSClass jsX3DFieldDefinition::static_class = {
	"X3DFieldDefinition", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, privateFinalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DFieldDefinition::properties [ ] = {
	{ "name",       NAME,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, name,       NULL },
	{ "accessType", ACCESSTYPE, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, accessType, NULL },
	{ "dataType",   DATATYPE,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, dataType,   NULL },
	{ 0 }

};

void
jsX3DFieldDefinition::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, NULL,
	              0, properties, NULL, NULL, NULL);
}

JSBool
jsX3DFieldDefinition::create (JSContext* const context, const X3DFieldDefinition* const field, jsval* const vp)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

	JSObject* const result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	const auto privateField = new jsX3DFieldDefinitionPrivate (field);

	privateField -> addParent (javaScript);
	const_cast <X3DFieldDefinition*> (field) -> addParent (privateField);

	JS_SetPrivate (context, result, privateField);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsX3DFieldDefinition::name (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	const auto field = static_cast <jsX3DFieldDefinitionPrivate*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, field -> getValue () -> getName (), vp);
}

JSBool
jsX3DFieldDefinition::accessType (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	const auto field = static_cast <jsX3DFieldDefinitionPrivate*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, field -> getValue () -> getAccessType (), vp);
}

JSBool
jsX3DFieldDefinition::dataType (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	const auto field = static_cast <jsX3DFieldDefinitionPrivate*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, field -> getValue () -> getType (), vp);
}

void
jsX3DFieldDefinition::privateFinalize (JSContext* context, JSObject* obj)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));
	const auto field      = static_cast <jsX3DFieldDefinitionPrivate*> (JS_GetPrivate (context, obj));

	// Proto objects have no private

	if (field)
	{
		const_cast <X3DFieldDefinition*> (field -> getValue ()) -> removeParent (field);
		field -> removeParent (javaScript);
	}
}

// This function is for concrete fields only.

void
jsX3DFieldDefinition::finalize (JSContext* context, JSObject* obj)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));
	const auto field      = static_cast <X3DFieldDefinition*> (JS_GetPrivate (context, obj));

	// Proto objects have no private

	if (field)
		javaScript -> removeObject (field);
}

} // X3D
} // titania

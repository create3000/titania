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

#include "jsProfileInfo.h"

#include "jsComponentInfoArray.h"
#include "jsString.h"

namespace titania {
namespace X3D {

JSClass jsProfileInfo::static_class = {
	"ProfileInfo", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsProfileInfo::properties [ ] = {
	{ "name",        NAME,         JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, name,        NULL },
	{ "title",       TITLE,        JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, title,       NULL },
	{ "providerUrl", PROVIDER_URL, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, providerUrl, NULL },
	{ "components",  COMPONENTS,   JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, components,  NULL },
	{ 0 }

};

JSFunctionSpec jsProfileInfo::functions [ ] = {
	{ 0, 0, 0, 0 }

};

void
jsProfileInfo::init (JSContext* const context, JSObject* const global)
{
	JS_InitClass (context, global, NULL, &static_class, NULL,
	              0, properties, functions, NULL, NULL);
}

JSBool
jsProfileInfo::create (JSContext* const context, const ProfileInfo* const profileInfo, jsval* const vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, const_cast <ProfileInfo*> (profileInfo));

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsProfileInfo::name (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto profileInfo = static_cast <ProfileInfo*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, profileInfo -> getName (), vp);
}

JSBool
jsProfileInfo::title (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto profileInfo = static_cast <ProfileInfo*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, profileInfo -> getTitle (), vp);
}

JSBool
jsProfileInfo::providerUrl (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto profileInfo = static_cast <ProfileInfo*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, profileInfo -> getProviderUrl (), vp);
}

JSBool
jsProfileInfo::components (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto profileInfo = static_cast <ProfileInfo*> (JS_GetPrivate (context, obj));

	return jsComponentInfoArray::create (context, &profileInfo -> getComponents (), vp);
}

} // X3D
} // titania

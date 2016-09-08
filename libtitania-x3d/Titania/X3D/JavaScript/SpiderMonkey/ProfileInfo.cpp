/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "ProfileInfo.h"

#include "ComponentInfoArray.h"
#include "String.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSClass ProfileInfo::static_class = {
	"ProfileInfo", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec ProfileInfo::properties [ ] = {
	{ "name",        NAME,         JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, name,        nullptr },
	{ "title",       TITLE,        JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, title,       nullptr },
	{ "providerUrl", PROVIDER_URL, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, providerUrl, nullptr },
	{ "components",  COMPONENTS,   JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, components,  nullptr },
	{ 0 }

};

JSObject*
ProfileInfo::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, nullptr, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JSBool
ProfileInfo::create (JSContext* const cx, const X3D::ProfileInfoPtr & profileInfo, jsval* const vp)
{
	const auto result = JS_NewObject (cx, &static_class, nullptr, nullptr);

	if (result == nullptr)
		return ThrowException (cx, "out of memory");

	JS_SetPrivate (cx, result, new X3D::ProfileInfoPtr (profileInfo));

	*vp = OBJECT_TO_JSVAL (result);

	return true;
}

JSBool
ProfileInfo::name (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & profileInfo = *getThis <ProfileInfo> (cx, obj);

		return JS_NewStringValue (cx, profileInfo -> getName (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .name: %s.", getClass () -> name, error .what ());
	}
}

JSBool
ProfileInfo::title (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & profileInfo = *getThis <ProfileInfo> (cx, obj);

		return JS_NewStringValue (cx, profileInfo -> getTitle (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .title: %s.", getClass () -> name, error .what ());
	}
}

JSBool
ProfileInfo::providerUrl (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & profileInfo = *getThis <ProfileInfo> (cx, obj);

		return JS_NewStringValue (cx, profileInfo -> getProviderUrl (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .providerUrl: %s.", getClass () -> name, error .what ());
	}
}

JSBool
ProfileInfo::components (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & profileInfo = *getThis <ProfileInfo> (cx, obj);

		return ComponentInfoArray::create (cx, &profileInfo -> getComponents (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .components: %s.", getClass () -> name, error .what ());
	}
}

void
ProfileInfo::finalize (JSContext* cx, JSObject* obj)
{
	const auto profileInfo = getObject <X3D::ProfileInfoPtr*> (cx, obj);

	if (profileInfo)
		delete profileInfo;
}

} // spidermonkey
} // X3D
} // titania

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

#include "X3DExternProtoDeclaration.h"

#include "../../Components/Core/X3DPrototypeInstance.h"
#include "Fields/SFNode.h"
#include "Context.h"
#include "FieldDefinitionArray.h"
#include "String.h"
#include "value.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSClass X3DExternProtoDeclaration::static_class = {
	"X3DExternProtoDeclaration", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec X3DExternProtoDeclaration::properties [ ] = {
	{ "name",          NAME,           JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, name,          nullptr },
	{ "fields",        FIELDS,         JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, fields,        nullptr },
	{ "urls",          URLS,           JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, urls,          nullptr },
	{ "isExternProto", IS_EXTERNPROTO, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, isExternProto, nullptr },
	{ "loadState",     LOAD_STATE,     JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, loadState,     nullptr },
	{ 0 }

};

JSFunctionSpec X3DExternProtoDeclaration::functions [ ] = {
	{ "newInstance", newInstance, 0, 0 },
	{ "loadNow",     loadNow,     0, 0 },
	{ 0, 0, 0, 0 }

};

JSObject*
X3DExternProtoDeclaration::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JSBool
X3DExternProtoDeclaration::create (JSContext* const cx, const X3D::ExternProtoDeclarationPtr & externproto, jsval* const vp)
{
	const auto result = JS_NewObject (cx, &static_class, nullptr, nullptr);

	if (result == nullptr)
		return ThrowException (cx, "out of memory");

	const auto field = new X3D::ExternProtoDeclarationPtr (externproto);

	JS_SetPrivate (cx, result, field);

	getContext (cx) -> addObject (field, field, result);

	*vp = OBJECT_TO_JSVAL (result);

	return true;
}

// Properties

JSBool
X3DExternProtoDeclaration::name (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & externproto = *getThis <X3DExternProtoDeclaration> (cx, obj);

		return JS_NewStringValue (cx, externproto -> getName (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .name: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DExternProtoDeclaration::fields (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & externproto = *getThis <X3DExternProtoDeclaration> (cx, obj);

		return FieldDefinitionArray::create (cx, &externproto -> getFieldDefinitions (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .fields: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DExternProtoDeclaration::urls (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & externproto = *getThis <X3DExternProtoDeclaration> (cx, obj);

		return getValue (cx, &externproto -> url (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .urls: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DExternProtoDeclaration::isExternProto (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & externproto = *getThis <X3DExternProtoDeclaration> (cx, obj);

		*vp = externproto -> isExternproto () ? JSVAL_TRUE : JSVAL_FALSE;
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isExternProto: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DExternProtoDeclaration::loadState (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & externproto = *getThis <X3DExternProtoDeclaration> (cx, obj);

		return JS_NewNumberValue (cx, externproto -> checkLoadState (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .loadState: %s.", getClass () -> name, error .what ());
	}
}

// Functions

JSBool
X3DExternProtoDeclaration::newInstance (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .newInstance: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto & externproto = *getThis <X3DExternProtoDeclaration> (cx, vp);
		X3D::SFNode  instance    = externproto -> createInstance ();

		instance -> setup ();

		return X3DField::get <SFNode> (cx, &instance, &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .newInstance: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DExternProtoDeclaration::loadNow (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .loadNow: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto & externproto = *getThis <X3DExternProtoDeclaration> (cx, vp);

		externproto -> requestImmediateLoad ();

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .loadNow: %s.", getClass () -> name, error .what ());
	}
}

void
X3DExternProtoDeclaration::finalize (JSContext* cx, JSObject* obj)
{
	const auto externproto = getObject <ExternProtoDeclarationPtr*> (cx, obj);

	if (externproto)
		getContext (cx) -> removeObject (externproto, externproto);
}

} // spidermonkey
} // X3D
} // titania

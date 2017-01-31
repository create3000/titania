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

#include "X3DProtoDeclaration.h"

#include "../../Components/Core/X3DPrototypeInstance.h"
#include "Fields/SFNode.h"
#include "Context.h"
#include "FieldDefinitionArray.h"
#include "String.h"
#include "value.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSClass X3DProtoDeclaration::static_class = {
	"X3DProtoDeclaration", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec X3DProtoDeclaration::properties [ ] = {
	{ "name",          NAME,           JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, name,          nullptr },
	{ "fields",        FIELDS,         JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, fields,        nullptr },
	{ "isExternProto", IS_EXTERNPROTO, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, isExternProto, nullptr },
	{ 0 }

};

JSFunctionSpec X3DProtoDeclaration::functions [ ] = {
	{ "newInstance", newInstance, 0, 0 },

	{ "toVRMLString", toVRMLString, 0, 0 },
	{ "toXMLString",  toXMLString,  0, 0 },

	{ 0, 0, 0, 0 }

};

JSObject*
X3DProtoDeclaration::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JSBool
X3DProtoDeclaration::create (JSContext* const cx, const X3D::ProtoDeclarationPtr & proto, jsval* const vp)
{
	JSObject* const result = JS_NewObject (cx, &static_class, nullptr, nullptr);

	if (result == nullptr)
		return ThrowException (cx, "out of memory");

	const auto field = new X3D::ProtoDeclarationPtr (proto);

	JS_SetPrivate (cx, result, field);

	getContext (cx) -> addObject (field, field, result);

	*vp = OBJECT_TO_JSVAL (result);

	return true;
}

// Properties

JSBool
X3DProtoDeclaration::name (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & proto = *getThis <X3DProtoDeclaration> (cx, obj);

		return JS_NewStringValue (cx, proto -> getName (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .name: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DProtoDeclaration::fields (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & proto = *getThis <X3DProtoDeclaration> (cx, obj);

		return FieldDefinitionArray::create (cx, &proto -> getFieldDefinitions (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .fields: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DProtoDeclaration::isExternProto (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & proto = *getThis <X3DProtoDeclaration> (cx, obj);

		*vp = proto -> isExternproto () ? JSVAL_TRUE : JSVAL_FALSE;
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isExternProto: %s.", getClass () -> name, error .what ());
	}
}

// Functions

JSBool
X3DProtoDeclaration::newInstance (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .newInstance: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto & proto = *getThis <X3DProtoDeclaration> (cx, vp);

		X3D::SFNode instance (proto -> createInstance ());

		instance -> setup ();

		return X3DField::get <SFNode> (cx, &instance, &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .newInstance: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DProtoDeclaration::toVRMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toVRMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto context = getContext (cx);
		const auto lhs     = getThis <X3DProtoDeclaration> (cx, vp);
		const auto version = context -> getExecutionContext () -> getSpecificationVersion ();

		Generator::SpecificationVersion (version);
		Generator::NicestStyle ();

		return JS_NewStringValue (cx, lhs -> toString (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toVRMLString: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DProtoDeclaration::toXMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toXMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto context = getContext (cx);
		const auto lhs     = getThis <X3DProtoDeclaration> (cx, vp);
		auto       version = context -> getExecutionContext () -> getSpecificationVersion ();

		if (version == VRML_V2_0)
			version = LATEST_VERSION;

		Generator::SpecificationVersion (version);
		Generator::NicestStyle ();

		return JS_NewStringValue (cx, lhs -> toXMLString (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toXMLString: %s.", getClass () -> name, error .what ());
	}
}

// Destruction

void
X3DProtoDeclaration::finalize (JSContext* cx, JSObject* obj)
{
	const auto proto = getObject <X3D::ProtoDeclarationPtr*> (cx, obj);

	if (proto)
		getContext (cx) -> removeObject (proto, proto);
}

} // spidermonkey
} // X3D
} // titania

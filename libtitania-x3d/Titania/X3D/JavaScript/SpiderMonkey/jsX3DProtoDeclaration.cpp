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

#include "jsX3DProtoDeclaration.h"

#include "../../Components/Core/X3DPrototypeInstance.h"
#include "Fields/jsSFNode.h"
#include "jsContext.h"
//#include "jsFieldDefinitionArray.h"
#include "jsString.h"
#include "jsfield.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsX3DProtoDeclaration::static_class = {
	"X3DProtoDeclaration",
	JSCLASS_HAS_PRIVATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	JS_PropertyStub,
	JS_StrictPropertyStub,
	JS_EnumerateStub,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DProtoDeclaration::properties [ ] = {
	JS_PSG ("name",          getName,       JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSG ("fields",        getFields,     JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSG ("isExternProto", isExternProto, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END

};

JSFunctionSpec jsX3DProtoDeclaration::functions [ ] = {
	JS_FS ("newInstance", newInstance, 0, JSPROP_PERMANENT),
	JS_FS_END

};

JSObject*
jsX3DProtoDeclaration::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JS::Value
jsX3DProtoDeclaration::create (JSContext* const cx, const ProtoDeclarationPtr & proto)
throw (std::invalid_argument)
{
	JSObject* const result = JS_NewObject (cx, &static_class, nullptr, nullptr);

	if (result == nullptr)
		throw std::invalid_argument ("out of memory");

	const auto field = new X3D::ProtoDeclarationPtr (proto);

	JS_SetPrivate (result, field);

	getContext (cx) -> addObject (field, result);

	return JS::ObjectValue (*result);
}

// Properties

JSBool
jsX3DProtoDeclaration::getName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args  = JS::CallArgsFromVp (argc, vp);
		const auto & proto = *getThis <jsX3DProtoDeclaration> (cx, args);

		args .rval () .set (StringValue (cx, proto -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .name: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsX3DProtoDeclaration::getFields (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args  = JS::CallArgsFromVp (argc, vp);
		const auto & proto = *getThis <jsX3DProtoDeclaration> (cx, args);

		//args .rval () .set (jsFieldDefinitionArray::create (cx, &proto -> getFieldDefinitions ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .fields: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsX3DProtoDeclaration::isExternProto (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args  = JS::CallArgsFromVp (argc, vp);
		const auto & proto = *getThis <jsX3DProtoDeclaration> (cx, args);

		args .rval () .setBoolean (proto -> isExternproto ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isExternProto: %s.", getClass () -> name, error .what ());
	}
}

// Functions

JSBool
jsX3DProtoDeclaration::newInstance (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .newInstance: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto   args     = JS::CallArgsFromVp (argc, vp);
		const auto & proto    = *getThis <jsX3DProtoDeclaration> (cx, args);
		auto         instance = proto -> createInstance ();

		instance -> setup ();

		args .rval () .set (jsSFNode::create (cx, new SFNode (std::move (instance))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .newInstance: %s.", getClass () -> name, error .what ());
	}
}

// Destruction

void
jsX3DProtoDeclaration::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto proto = getObject <X3D::ProtoDeclarationPtr*> (obj);

	if (proto)
		getContext (fop -> runtime ()) -> removeObject (proto);
}

} // MozillaSpiderMonkey
} // X3D
} // titania

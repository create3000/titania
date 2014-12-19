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
#include "jsFieldDefinitionArray.h"
#include "jsString.h"
#include "jsfield.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsX3DProtoDeclaration::static_class = {
	"X3DProtoDeclaration", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DProtoDeclaration::properties [ ] = {
	{ "name",          NAME,           JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, name,          NULL },
	{ "fields",        FIELDS,         JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, fields,        NULL },
	{ "isExternProto", IS_EXTERNPROTO, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, isExternProto, NULL },
	{ 0 }

};

JSFunctionSpec jsX3DProtoDeclaration::functions [ ] = {
	{ "newInstance", newInstance, 0, 0 },
	{ 0, 0, 0, 0 }

};

void
jsX3DProtoDeclaration::init (JSContext* const cx, JSObject* const global)
{
	const auto proto = JS_InitClass (cx, global, NULL, &static_class, NULL, 0, properties, functions, NULL, NULL);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
}

JSBool
jsX3DProtoDeclaration::create (JSContext* const cx, const ProtoDeclarationPtr & proto, jsval* const vp)
{
	JSObject* const result = JS_NewObject (cx, &static_class, NULL, NULL);

	if (result == NULL)
		return false;

	const auto field = new ProtoDeclarationPtr (proto);

	JS_SetPrivate (cx, result, field);

	getContext (cx) -> addObject (field, result);

	*vp = OBJECT_TO_JSVAL (result);

	return true;
}

// Properties

JSBool
jsX3DProtoDeclaration::name (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto & proto = *static_cast <ProtoDeclarationPtr*> (JS_GetPrivate (cx, obj));

	return JS_NewStringValue (cx, proto -> getName (), vp);
}

JSBool
jsX3DProtoDeclaration::fields (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	auto & proto = *static_cast <ProtoDeclarationPtr*> (JS_GetPrivate (cx, obj));

	return jsFieldDefinitionArray::create (cx, &proto -> getFieldDefinitions (), vp);
}

JSBool
jsX3DProtoDeclaration::isExternProto (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto & proto = *static_cast <ProtoDeclarationPtr*> (JS_GetPrivate (cx, obj));

	*vp = proto -> isExternproto () ? JSVAL_TRUE : JSVAL_FALSE;

	return true;
}

// Functions

JSBool
jsX3DProtoDeclaration::newInstance (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto & proto    = *static_cast <ProtoDeclarationPtr*> (JS_GetPrivate (cx, JS_THIS_OBJECT (cx, vp)));
	auto         instance = proto -> createInstance ();

	instance -> setup ();

	return jsSFNode::create (cx, new SFNode (std::move (instance)), &JS_RVAL (cx, vp));
}

void
jsX3DProtoDeclaration::finalize (JSContext* cx, JSObject* obj)
{
	const auto proto = static_cast <ProtoDeclarationPtr*> (JS_GetPrivate (cx, obj));

	if (proto)
		getContext (cx) -> removeObject (proto);
}

} // MozillaSpiderMonkey
} // X3D
} // titania

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

#include "jsX3DExternProtoDeclaration.h"

#include "../../Components/Core/X3DPrototypeInstance.h"
#include "Fields/jsSFNode.h"
#include "jsContext.h"
#include "jsFieldDefinitionArray.h"
#include "jsString.h"
#include "jsfield.h"

namespace titania {
namespace X3D {

JSClass jsX3DExternProtoDeclaration::static_class = {
	"X3DExternProtoDeclaration", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DExternProtoDeclaration::properties [ ] = {
	{ "name",          NAME,           JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, name,          NULL },
	{ "fields",        FIELDS,         JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, fields,        NULL },
	{ "urls",          URLS,           JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, urls,          NULL },
	{ "isExternProto", IS_EXTERNPROTO, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, isExternProto, NULL },
	{ "loadState",     LOAD_STATE,     JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, loadState,     NULL },
	{ 0 }

};

JSFunctionSpec jsX3DExternProtoDeclaration::functions [ ] = {
	{ "newInstance", newInstance, 0, 0 },
	{ "loadNow",     loadNow,     0, 0 },
	{ 0, 0, 0, 0 }

};

void
jsX3DExternProtoDeclaration::initObject (JSContext* context, JSObject* object)
{
	JS_DefineProperties (context, object, properties);
	JS_DefineFunctions (context, object, functions);
}

JSBool
jsX3DExternProtoDeclaration::create (JSContext* context, const SFNode <ExternProto> & externproto, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	initObject (context, result);

	auto field = new SFNode <ExternProto> (externproto);

	JS_SetPrivate (context, result, field);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	static_cast <jsContext*> (JS_GetContextPrivate (context)) -> addObject (field, result);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

// Properties

JSBool
jsX3DExternProtoDeclaration::name (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & externproto = *static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, externproto -> getName (), vp);
}

JSBool
jsX3DExternProtoDeclaration::fields (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & externproto = *static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, obj));

	return jsFieldDefinitionArray::create (context, &externproto -> getFieldDefinitions (), vp);
}

JSBool
jsX3DExternProtoDeclaration::urls (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & externproto = *static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, obj));

	return JS_NewFieldValue (context, &externproto -> url (), vp);
}

JSBool
jsX3DExternProtoDeclaration::isExternProto (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & externproto = *static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, obj));

	*vp = externproto -> isExternproto () ? JSVAL_TRUE : JSVAL_FALSE;

	return JS_TRUE;
}

JSBool
jsX3DExternProtoDeclaration::loadState (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & externproto = *static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, obj));

	return JS_NewNumberValue (context, externproto -> checkLoadState (), vp);
}

// Functions

JSBool
jsX3DExternProtoDeclaration::newInstance (JSContext* context, uintN argc, jsval* vp)
{
	auto & externproto = *static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

	SFNode <X3DPrototypeInstance> instance = externproto -> createInstance ();

	instance -> setup ();

	return jsSFNode::create (context, new SFNode <X3DBaseNode> (instance), &JS_RVAL (context, vp));
}

JSBool
jsX3DExternProtoDeclaration::loadNow (JSContext* context, uintN argc, jsval* vp)
{
	auto & externproto = *static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

	externproto -> requestImmediateLoad ();

	JS_SET_RVAL (context, vp, JSVAL_VOID);

	return JS_TRUE;
}

void
jsX3DExternProtoDeclaration::finalize (JSContext* context, JSObject* obj)
{
	auto externproto = static_cast <SFNode <ExternProto>*> (JS_GetPrivate (context, obj));

	static_cast <jsContext*> (JS_GetContextPrivate (context)) -> removeObject (externproto);
}

} // X3D
} // titania

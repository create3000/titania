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

#include "jsX3DScene.h"

#include "Fields/jsMFNode.h"
#include "Fields/jsSFNode.h"
#include "jsContext.h"
#include "jsError.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsX3DScene::static_class = {
	"X3DScene", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DScene::properties [ ] = {
	{ "rootNodes", ROOT_NODES, JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT, rootNodes, rootNodes },
	{ 0 }
};

JSFunctionSpec jsX3DScene::functions [ ] = {
	{ "setMetaData", setMetaData, 2, 0 },
	{ "getMetaData", getMetaData, 1, 0 },

	{ "addExportedNode",    addExportedNode,    2, 0 },
	{ "removeExportedNode", removeExportedNode, 1, 0 },
	{ "updateExportedNode", updateExportedNode, 2, 0 },
	{ "getExportedNode",    getExportedNode,    1, 0 },

	{ 0 }

};

void
jsX3DScene::init (JSContext* const cx, JSObject* const global, JSObject* const executionContext)
{
	const auto proto = JS_InitClass (cx, global, executionContext, &static_class, NULL, 0, properties, functions, NULL, NULL);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
}

JSBool
jsX3DScene::create (JSContext* const cx, X3DScene* const scene, jsval* const vp)
{
	JSObject* const result = JS_NewObject (cx, &static_class, NULL, NULL);

	if (result == NULL)
		return false;

	const auto context = getContext (cx);

	if (scene not_eq context -> getExecutionContext ())
		scene -> addParent (context);

	JS_SetPrivate (cx, result, scene);

	*vp = OBJECT_TO_JSVAL (result);

	return true;
}

JSBool
jsX3DScene::rootNodes (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	// Differs from X3DExecutionContext rootNodes!!!

	const auto scene = getObject <X3DScene*> (cx, obj);

	return jsMFNode::create (cx, &scene -> getRootNodes (), vp);
}

JSBool
jsX3DScene::rootNodes (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	const auto scene = getObject <X3DScene*> (cx, obj);

	JSObject* omfnode = nullptr;

	if (not JS_ValueToObject (cx, *vp, &omfnode))
		return false;

	if (JS_InstanceOfError (cx, omfnode, jsMFNode::getClass ()))
		return false;

	scene -> getRootNodes () = *getObject <MFNode*> (cx, omfnode);

	return true;
}

JSBool
jsX3DScene::setMetaData (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2)
	{
		JSString* key   = nullptr;
		JSString* value = nullptr;

		jsval* argv = JS_ARGV (cx, vp);

		if (not JS_ConvertArguments (cx, argc, argv, "SS", &key, &value))
			return false;

		const auto scene = getObject <X3DScene*> (cx, JS_THIS_OBJECT (cx, vp));

		scene -> setMetaData (JS_GetString (cx, key), JS_GetString (cx, value));

		JS_SET_RVAL (cx, vp, JSVAL_VOID);

		return true;
	}

	JS_ReportError (cx, "wrong number of arguments");

	return false;
}

JSBool
jsX3DScene::getMetaData (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		JSString* key = nullptr;

		jsval* argv = JS_ARGV (cx, vp);

		if (not JS_ConvertArguments (cx, argc, argv, "S", &key))
			return false;

		try
		{
			const auto scene = getObject <X3DScene*> (cx, JS_THIS_OBJECT (cx, vp));

			return JS_NewStringValue (cx, scene -> getMetaData (JS_GetString (cx, key)), &JS_RVAL (cx, vp));
		}
		catch (const Error <INVALID_NAME> &)
		{
			return JS_NewStringValue (cx, "", &JS_RVAL (cx, vp));
		}
	}

	JS_ReportError (cx, "wrong number of arguments");

	return false;
}

JSBool
jsX3DScene::addExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2)
	{
		JSString* exportedName = nullptr;
		JSObject* oNode        = nullptr;

		const auto argv = JS_ARGV (cx, vp);

		if (not JS_ConvertArguments (cx, argc, argv, "So", &exportedName, &oNode))
			return false;

		if (JS_InstanceOfError (cx, oNode, jsSFNode::getClass ()))
			return false;

		auto & node = *getObject <SFNode*> (cx, oNode);

		try
		{
			const auto scene = getObject <X3DScene*> (cx, JS_THIS_OBJECT (cx, vp));

			scene -> addExportedNode (JS_GetString (cx, exportedName), node);

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "wrong number of arguments");

	return false;
}

JSBool
jsX3DScene::removeExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		JSString* exportedName = nullptr;

		jsval* argv = JS_ARGV (cx, vp);

		if (not JS_ConvertArguments (cx, argc, argv, "S", &exportedName))
			return false;

		try
		{
			const auto scene = getObject <X3DScene*> (cx, JS_THIS_OBJECT (cx, vp));

			scene -> removeImportedNode (JS_GetString (cx, exportedName));

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "wrong number of arguments");

	return false;
}

JSBool
jsX3DScene::updateExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2)
	{
		JSString* exportedName = nullptr;
		JSObject* oNode        = nullptr;

		const auto argv = JS_ARGV (cx, vp);

		if (not JS_ConvertArguments (cx, argc, argv, "So", &exportedName, &oNode))
			return false;

		if (JS_InstanceOfError (cx, oNode, jsSFNode::getClass ()))
			return false;

		const auto & node = *getObject <SFNode*> (cx, oNode);

		try
		{
			const auto scene = getObject <X3DScene*> (cx, JS_THIS_OBJECT (cx, vp));

			scene -> updateExportedNode (JS_GetString (cx, exportedName), node);

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "wrong number of arguments");

	return false;
}

JSBool
jsX3DScene::getExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		JSString* exportedName = nullptr;

		const auto argv = JS_ARGV (cx, vp);

		if (not JS_ConvertArguments (cx, argc, argv, "S", &exportedName))
			return false;

		try
		{
			const auto scene     = getObject <X3DScene*> (cx, JS_THIS_OBJECT (cx, vp));
			const auto namedNode = scene -> getExportedNode (JS_GetString (cx, exportedName));

			return jsSFNode::create (cx, new SFNode (std::move (namedNode)), &JS_RVAL (cx, vp));
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "wrong number of arguments");

	return false;
}

void
jsX3DScene::finalize (JSContext* cx, JSObject* obj)
{
	const auto context = getContext (cx);
	const auto scene   = getObject <X3DScene*> (cx, obj);

	if (scene)
	{
		if (scene not_eq context -> getExecutionContext ())
			scene -> removeParent (context);
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

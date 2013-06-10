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

#include "jsX3DScene.h"

#include "jsContext.h"
#include "Fields/jsSFNode.h"

namespace titania {
namespace X3D {

JSClass jsX3DScene::static_class = {
	"X3DScene", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DScene::properties [ ] = {
	{ 0 }

};

JSFunctionSpec jsX3DScene::functions [ ] = {
	{ "setMetaData", setMetaData, 2, 0 },
	{ "getMetaData", getMetaData, 1, 0 },

	{ "addExportedNode",    addExportedNode,    2, 0 },
	{ "removeExportedNode", removeExportedNode, 1, 0 },
	{ "updateExportedNode", updateExportedNode, 2, 0 },
	{ "getExportedNode",    getExportedNode,    1, 0 },

	{ 0, 0, 0, 0 }

};

void
jsX3DScene::initObject (JSContext* context, JSObject* object)
{
	jsX3DExecutionContext::initObject (context, object);

	JS_DefineProperties (context, object, properties);
	JS_DefineFunctions (context, object, functions);
}

JSBool
jsX3DScene::create (JSContext* context, Scene* scene, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	initObject (context, result);
	
	JS_SetPrivate (context, result, scene);

	auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));
	
	if (scene not_eq javaScript -> getExecutionContext ())
		scene -> addParent (javaScript);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsX3DScene::setMetaData (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 2)
	{
		JSString* key;
		JSString* value;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "SS", &key, &value))
			return JS_FALSE;

		auto scene = static_cast <Scene*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

		scene -> setMetaData (JS_GetString (context, key), JS_GetString (context, value));

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsX3DScene::getMetaData (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		JSString* key;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "S", &key))
			return JS_FALSE;
			
		try
		{
			auto scene = static_cast <Scene*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

			return JS_NewStringValue (context, scene -> getMetaData (JS_GetString (context, key)), &JS_RVAL (context, vp));
		}
		catch (const Error <INVALID_NAME> &)
		{
			return JS_NewStringValue (context, "", &JS_RVAL (context, vp));
		}
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsX3DScene::addExportedNode (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 2)
	{
		JSString* exportedName;
		JSObject* oNode;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "So", &exportedName, &oNode))
			return JS_FALSE;

		if (JS_GetClass (context, oNode) not_eq jsSFNode::getClass ())
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFNode, is %s", JS_GetClass (context, oNode) -> name);
			return JS_FALSE;
		}

		auto & node = *static_cast <SFNode <X3DBaseNode>*> (JS_GetPrivate (context, oNode));

		try
		{
			auto scene = static_cast <Scene*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

			scene -> addExportedNode (JS_GetString (context, exportedName), node);

			JS_SET_RVAL (context, vp, JSVAL_VOID);

			return JS_TRUE;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (context, exception .what ());
			return JS_FALSE;
		}
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsX3DScene::removeExportedNode (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		JSString* exportedName;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "S", &exportedName))
			return JS_FALSE;
		
		try
		{
			auto scene = static_cast <Scene*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));
		
			scene -> removeImportedNode (JS_GetString (context, exportedName));
			
			JS_SET_RVAL (context, vp, JSVAL_VOID);

			return JS_TRUE;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (context, exception .what ());
			return JS_FALSE;
		}
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsX3DScene::updateExportedNode (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 2)
	{
		JSString* exportedName;
		JSObject* oNode;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "So", &exportedName, &oNode))
			return JS_FALSE;

		if (JS_GetClass (context, oNode) not_eq jsSFNode::getClass ())
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFNode, is %s", JS_GetClass (context, oNode) -> name);
			return JS_FALSE;
		}

		auto & node = *static_cast <SFNode <X3DBaseNode>*> (JS_GetPrivate (context, oNode));

		try
		{
			auto scene = static_cast <Scene*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));

			scene -> updateExportedNode (JS_GetString (context, exportedName), node);

			JS_SET_RVAL (context, vp, JSVAL_VOID);

			return JS_TRUE;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (context, exception .what ());
			return JS_FALSE;
		}
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsX3DScene::getExportedNode (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		JSString* exportedName;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "S", &exportedName))
			return JS_FALSE;
		
		try
		{
			auto scene = static_cast <Scene*> (JS_GetPrivate (context, JS_THIS_OBJECT (context, vp)));
		
			const auto & namedNode = scene -> getExportedNode (JS_GetString (context, exportedName));
			
			return jsSFNode::create (context, new SFNode <X3DBaseNode> (namedNode), &JS_RVAL (context, vp));
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (context, exception .what ());
			return JS_FALSE;
		}
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

void
jsX3DScene::finalize (JSContext* context, JSObject* obj)
{
	auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));
	auto scene      = static_cast <Scene*> (JS_GetPrivate (context, obj));

	if (scene not_eq javaScript -> getExecutionContext ())
		scene -> removeParent (javaScript);
}


} // X3D
} // titania

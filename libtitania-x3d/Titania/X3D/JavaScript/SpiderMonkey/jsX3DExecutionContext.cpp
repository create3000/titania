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

#include "jsX3DExecutionContext.h"

#include "../../Bits/Cast.h"
#include "../../Bits/Error.h"
#include "../../Components/Core/X3DPrototypeInstance.h"
#include "../../Components/Networking/Inline.h"
#include "../../Execution/X3DScene.h"
#include "Fields/jsMFNode.h"
#include "Fields/jsSFNode.h"
#include "jsComponentInfoArray.h"
#include "jsError.h"
#include "jsExternProtoDeclarationArray.h"
#include "jsProfileInfo.h"
#include "jsProtoDeclarationArray.h"
#include "jsRouteArray.h"
#include "jsString.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsX3DExecutionContext::static_class = {
	"X3DExecutionContext", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DExecutionContext::properties [ ] = {
	{ "specificationVersion", SPECIFICATION_VERSION, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, specificationVersion, NULL },
	{ "encoding",             ENCODING,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, encoding,             NULL },
	{ "worldURL",             WORLD_URL,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, worldURL,             NULL },

	{ "profile",              PROFILE,               JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, profile,     NULL },
	{ "components",           COMPONENTS,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, components,  NULL },

	{ "externprotos",         EXTERNPROTOS,          JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, externprotos, NULL },
	{ "protos",               PROTOS,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, protos,       NULL },
	{ "rootNodes",            ROOT_NODES,            JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, rootNodes,    NULL },
	{ "routes",               ROUTES,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, routes,       NULL },
	{ 0 }

};

JSFunctionSpec jsX3DExecutionContext::functions [ ] = {
	{ "createNode",  createNode,  1, 0 },
	{ "createProto", createProto, 1, 0 },

	{ "addNamedNode",    addNamedNode,    2, 0 },
	{ "removeNamedNode", removeNamedNode, 1, 0 },
	{ "updateNamedNode", updateNamedNode, 2, 0 },
	{ "getNamedNode",    getNamedNode,    1, 0 },

	{ "addImportedNode",    addImportedNode,    2, 0 },
	{ "removeImportedNode", removeImportedNode, 1, 0 },
	{ "updateImportedNode", updateImportedNode, 2, 0 },
	{ "getImportedNode",    getImportedNode,    1, 0 },

	{ "addRoute",    addRoute,    4, 0 },
	{ "deleteRoute", deleteRoute, 1, 0 },

	{ "toVRMLString", toVRMLString, 0, 0 },
	{ "toXMLString",  toXMLString,  0, 0 },

	{ 0 }

};

JSObject*
jsX3DExecutionContext::init (JSContext* const cx, JSObject* const global)
{
	const auto proto = JS_InitClass (cx, global, NULL, &static_class, NULL, 0, properties, functions, NULL, NULL);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JSBool
jsX3DExecutionContext::create (JSContext* const cx, X3DExecutionContext* const executionContext, jsval* const vp)
{
	JSObject* const result = JS_NewObject (cx, &static_class, NULL, NULL);

	if (result == NULL)
		return false;

	JS_SetPrivate (cx, result, executionContext);

	*vp = OBJECT_TO_JSVAL (result);
	return true;
}

// Properties

JSBool
jsX3DExecutionContext::specificationVersion (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);

	return JS_NewStringValue (cx, XMLEncode (executionContext -> getSpecificationVersion ()), vp);
}

JSBool
jsX3DExecutionContext::encoding (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);

	return JS_NewStringValue (cx, executionContext -> getEncoding (), vp);
}

JSBool
jsX3DExecutionContext::worldURL (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);

	return JS_NewStringValue (cx, executionContext -> getWorldURL (), vp);
}

JSBool
jsX3DExecutionContext::profile (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);
	const auto profile          = executionContext -> getProfile ();

	if (profile)
		return jsProfileInfo::create (cx, profile, vp);

	JS_SET_RVAL (cx, vp, JSVAL_VOID);

	return true;
}

JSBool
jsX3DExecutionContext::components (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);

	return jsComponentInfoArray::create (cx, &executionContext -> getComponents (), vp);
}

JSBool
jsX3DExecutionContext::externprotos (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);

	return jsExternProtoDeclarationArray::create (cx, &executionContext -> getExternProtoDeclarations (), vp);
}

JSBool
jsX3DExecutionContext::protos (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);

	return jsProtoDeclarationArray::create (cx, &executionContext -> getProtoDeclarations (), vp);
}

JSBool
jsX3DExecutionContext::rootNodes (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);
	const auto rootNodes        = new MFNode (executionContext -> getRootNodes ());

	rootNodes -> setName ("rootNodes");
	rootNodes -> setAccessType (initializeOnly);

	return jsMFNode::create (cx, rootNodes, vp);
}

JSBool
jsX3DExecutionContext::routes (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto executionContext = getObject <X3DExecutionContext*> (cx, obj);

	return jsRouteArray::create (cx, &executionContext -> getRoutes (), vp);
}

// Functions

JSBool
jsX3DExecutionContext::createNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		try
		{
			const auto argv = JS_ARGV (cx, vp);

			JSString* name = nullptr;

			if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
				return false;

			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));
			auto       node             = executionContext -> createNode (JS_GetString (cx, name));

			node -> setup ();

			return jsSFNode::create (cx, new SFNode (std::move (node)), &JS_RVAL (cx, vp));
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .createNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::createProto (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		try
		{
			const auto argv = JS_ARGV (cx, vp);

			JSString* name = nullptr;

			if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
				return false;

			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));
			auto       node             = executionContext -> createPrototypeInstance (JS_GetString (cx, name));

			node -> setup ();

			return jsSFNode::create (cx, new SFNode (std::move (node)), &JS_RVAL (cx, vp));
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .createProto: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::addNamedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSString* name  = nullptr;
		JSObject* oNode = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "So", &name, &oNode))
			return false;

		if (JS_InstanceOfError (cx, oNode, jsSFNode::getClass ()))
			return false;

		const auto & node = *getObject <SFNode*> (cx, oNode);

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

			executionContext -> addNamedNode (JS_GetString (cx, name), node);

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .addNamedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::removeNamedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSString* name = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
			return false;

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

			executionContext -> removeNamedNode (JS_GetString (cx, name));

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .removeNamedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::updateNamedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSString* name  = nullptr;
		JSObject* oNode = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "So", &name, &oNode))
			return false;

		if (JS_InstanceOfError (cx, oNode, jsSFNode::getClass ()))
			return false;

		const auto & node = *getObject <SFNode*> (cx, oNode);

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

			executionContext -> updateNamedNode (JS_GetString (cx, name), node);

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .updateNamedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::getNamedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSString* name = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
			return false;

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));
			auto       namedNode        = executionContext -> getNamedNode (JS_GetString (cx, name));

			return jsSFNode::create (cx, new SFNode (std::move (namedNode)), &JS_RVAL (cx, vp));
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .getNamedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::addImportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2 or argc == 3)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSObject* oInline      = nullptr;
		JSString* exportedName = nullptr;
		JSString* importedName = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "oS/S", &oInline, &exportedName, &importedName))
			return false;

		if (JS_InstanceOfError (cx, oInline, jsSFNode::getClass ()))
			return false;

		const auto & node       = *getObject <SFNode*> (cx, oInline);
		const auto   inlineNode = x3d_cast <Inline*> (node);

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

			if (argc == 3)
				executionContext -> addImportedNode (inlineNode, JS_GetString (cx, exportedName), JS_GetString (cx, importedName));

			else
				executionContext -> addImportedNode (inlineNode, JS_GetString (cx, exportedName));

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .addImportedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::removeImportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSString* importedName = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &importedName))
			return false;

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

			executionContext -> removeImportedNode (JS_GetString (cx, importedName));

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .removeImportedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::updateImportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2 or argc == 3)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSObject* oInline      = nullptr;
		JSString* exportedName = nullptr;
		JSString* importedName = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "oS/S", &oInline, &exportedName, &importedName))
			return false;

		if (JS_InstanceOfError (cx, oInline, jsSFNode::getClass ()))
			return false;

		const auto & node       = *getObject <SFNode*> (cx, oInline);
		const auto   inlineNode = x3d_cast <Inline*> (node);

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

			if (argc == 3)
				executionContext -> updateImportedNode (inlineNode, JS_GetString (cx, exportedName), JS_GetString (cx, importedName));

			else
				executionContext -> updateImportedNode (inlineNode, JS_GetString (cx, exportedName));

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & exception)
		{
			JS_ReportError (cx, exception .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .updateImportedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::getImportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto argv = JS_ARGV (cx, vp);

		JSString* importedName = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &importedName))
			return false;

		try
		{
			const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));
			auto       importedNode     = executionContext -> getImportedNode (JS_GetString (cx, importedName));

			return jsSFNode::create (cx, new SFNode (std::move (importedNode)), &JS_RVAL (cx, vp));
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .getImportedNode: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::addRoute (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 4)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSObject* ofromNode    = nullptr;
		JSObject* otoNode      = nullptr;
		JSString* fromEventOut = nullptr;
		JSString* toEventIn    = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "oSoS", &ofromNode, &fromEventOut, &otoNode, &toEventIn))
			return false;

		if (JS_InstanceOfError (cx, ofromNode, jsSFNode::getClass ()))
			return false;

		if (JS_InstanceOfError (cx, otoNode, jsSFNode::getClass ()))
			return false;

		const auto & fromNode = *getObject <SFNode*> (cx, ofromNode);
		const auto & toNode   = *getObject <SFNode*> (cx, otoNode);

		try
		{
			const RoutePtr & route = script -> getExecutionContext () -> addRoute (fromNode, JS_GetString (cx, fromEventOut),
			                                                                       toNode,   JS_GetString (cx, toEventIn));

			return jsX3DRoute::create (cx, route, &JS_RVAL (cx, vp));
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .addRoute: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::deleteRoute (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSObject* oRoute = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "o", &oRoute))
			return false;

		if (JS_InstanceOfError (cx, oRoute, jsX3DRoute::getClass ()))
			return false;

		const auto & route = *getObject <RoutePtr*> (cx, oRoute);

		try
		{
			script -> getExecutionContext () -> deleteRoute (route);

			JS_SET_RVAL (cx, vp, JSVAL_VOID);

			return true;
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "X3DExecutionContext .deleteRoute: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::toVRMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

		std::ostringstream ostream;

		Generator::NicestStyle ();

		executionContext -> X3DExecutionContext::toStream (ostream);

		return JS_NewStringValue (cx, ostream .str (), &JS_RVAL (cx, vp));
	}

	JS_ReportError (cx, "X3DExecutionContext .toVRMLString: wrong number of arguments");

	return false;
}

JSBool
jsX3DExecutionContext::toXMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto executionContext = getObject <X3DExecutionContext*> (cx, JS_THIS_OBJECT (cx, vp));

		std::ostringstream ostream;

		Generator::NicestStyle ();

		executionContext -> X3DExecutionContext::toXMLStream (ostream);

		return JS_NewStringValue (cx, ostream .str (), &JS_RVAL (cx, vp));
	}

	JS_ReportError (cx, "X3DExecutionContext .toXMLString: wrong number of arguments");

	return false;
}

} // MozillaSpiderMonkey
} // X3D
} // titania

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

#include "jsBrowser.h"

#include "../../Browser/BrowserOptions.h"
#include "../../Browser/BrowserProperties.h"
#include "../../Browser/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Scripting/Script.h"
#include "../../InputOutput/Generator.h"
#include "../../InputOutput/Loader.h"
#include "../../Thread/SceneLoader.h"
#include "Fields/jsMFNode.h"
#include "Fields/jsMFString.h"
#include "Fields/jsSFNode.h"
#include "jsComponentInfoArray.h"
#include "jsContext.h"
#include "jsError.h"
#include "jsProfileInfoArray.h"
#include "jsString.h"
#include "jsX3DExecutionContext.h"
#include "jsX3DScene.h"
#include "jsfield.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsBrowser::static_class = {
	"Browser", 0,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

// X3D properties
JSPropertySpec jsBrowser::properties [ ] = {
	{ "name",                NAME,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, name,                nullptr },
	{ "version",             VERSION,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, version,             nullptr },
	{ "currentSpeed",        CURRENT_SPEED,        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentSpeed,        nullptr },
	{ "currentFrameRate",    CURRENT_FRAMERATE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentFrameRate,    nullptr },
	{ "description",         DESCRIPTION,          JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT,                   description,         description },
	{ "supportedComponents", SUPPORTED_COMPONENTS, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, supportedComponents, nullptr },
	{ "supportedProfiles",   SUPPORTED_PROFILES,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, supportedProfiles,   nullptr },
	{ "currentScene",        CURRENT_SCENE,        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentScene,        nullptr },
	{ 0 }

};

JSFunctionSpec jsBrowser::functions [ ] = {
	// X3D functions
	{ "replaceWorld",         replaceWorld,         1, 0 },
	{ "createX3DFromString",  createX3DFromString,  1, 0 },
	{ "createX3DFromURL",     createX3DFromURL,     1, 0 },
	{ "loadURL",              loadURL,              2, 0 },
	{ "getRenderingProperty", getRenderingProperty, 1, 0 },
	{ "getBrowserProperty",   getBrowserProperty,   1, 0 },
	{ "getBrowserOption",     getBrowserOption,     1, 0 },
	{ "setBrowserOption",     setBrowserOption,     2, 0 },

	{ "firstViewpoint",       firstViewpoint,       0, 0 },
	{ "previousViewpoint",    previousViewpoint,    0, 0 },
	{ "nextViewpoint",        nextViewpoint,        0, 0 },
	{ "lastViewpoint",        lastViewpoint,        0, 0 },

	{ "print",                print,                0, 0 },
	{ "println",              println,              0, 0 },

	// VRML functions
	{ "getName",              getName,              0, 0 },
	{ "getVersion",           getVersion,           0, 0 },
	{ "getCurrentSpeed",      getCurrentSpeed,      0, 0 },
	{ "getCurrentFrameRate",  getCurrentFrameRate,  0, 0 },
	{ "getWorldURL",          getWorldURL,          0, 0 },
	{ "setDescription",       setDescription,       1, 0 },
	{ "createVrmlFromString", createVrmlFromString, 1, 0 },
	{ "createVrmlFromURL",    createVrmlFromURL,    3, 0 },
	{ "addRoute",             addRoute,             4, 0 },
	{ "deleteRoute",          deleteRoute,          4, 0 },

	{ 0 }

};

JSObject*
jsBrowser::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineObject (cx, global, "Browser", &static_class, nullptr, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT);
	
	return proto;
}

// X3D properties

JSBool
jsBrowser::name (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	return JS_NewStringValue (cx, script -> getBrowser () -> getName (), vp);
}

JSBool
jsBrowser::version (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	return JS_NewStringValue (cx, script -> getBrowser () -> getVersion (), vp);
}

JSBool
jsBrowser::currentSpeed (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	return JS_NewNumberValue (cx, script -> getBrowser () -> getCurrentSpeed (), vp);
}

JSBool
jsBrowser::currentFrameRate (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	return JS_NewNumberValue (cx, script -> getBrowser () -> getCurrentFrameRate (), vp);
}

JSBool
jsBrowser::description (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	return JS_NewStringValue (cx, script -> getBrowser () -> getDescription (), vp);
}

JSBool
jsBrowser::description (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	script -> getBrowser () -> setDescription (to_string (cx, *vp));

	return true;
}

JSBool
jsBrowser::supportedComponents (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	return jsComponentInfoArray::create (cx, &script -> getBrowser () -> getSupportedComponents (), vp);
}

JSBool
jsBrowser::supportedProfiles (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();

	return jsProfileInfoArray::create (cx, &script -> getBrowser () -> getSupportedProfiles (), vp);
}

JSBool
jsBrowser::currentScene (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	const auto script = getContext (cx) -> getScriptNode ();
	const auto scene  = dynamic_cast <X3DScene*> (script -> getExecutionContext ());

	if (scene)
		return jsX3DScene::create (cx, scene, vp);

	return jsX3DExecutionContext::create (cx, script -> getExecutionContext (), vp);
}

// X3D functions

JSBool
jsBrowser::replaceWorld (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .replaceWorld: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv   = JS_ARGV (cx, vp);
		const auto script = getContext (cx) -> getScriptNode ();

		if (JSVAL_IS_NULL (argv [0]))
			script -> getBrowser () -> replaceWorld (nullptr);

		else
		{
			const X3D::X3DScenePtr scene = getArgument <jsX3DScene> (cx, argv, 0);

			script -> getBrowser () -> replaceWorld (scene);
		}

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::createX3DFromString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .createX3DFromString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv      = JS_ARGV (cx, vp);
		const auto script    = getContext (cx) -> getScriptNode ();
		const auto x3dSyntax = getArgument <std::string> (cx, argv, 0);
		const auto scene     = X3D::Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (x3dSyntax);

		return jsX3DScene::create (cx, scene, vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::createX3DFromURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc > 3)
		return ThrowException (cx, "%s .createX3DFromURL: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv    = JS_ARGV (cx, vp);
		const auto context = getContext (cx);
		const auto script  = context -> getScriptNode ();
		const auto url     = getArgument <jsMFString> (cx, argv, 0);

		if (argc > 1)
		{
			const auto event = getArgument <std::string> (cx, argv, 1);

			try
			{
				if (argc == 3)
				{
					const auto node = getArgument <jsSFNode> (cx, argv, 2);

					if (not node -> getValue ())
						return ThrowException (cx, "Browser .createX3DFromURL: node is NULL.");

					try
					{
						const auto field = node -> getValue () -> getField (event);

						if (not field -> isInput ())
							return ThrowException (cx, "Browser .createX3DFromURL: field '%s' is not an eventIn.", event .c_str ());

						if (field -> getType () not_eq X3D::X3DConstants::MFNode)
							return ThrowException (cx, "Browser .createX3DFromURL: field '%s' is not a MFNode.", event .c_str ());

						try
						{
							const auto scene = X3D::Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);

							field -> set (scene -> getRootNodes ());
							field -> addEvent ();
						}
						catch (const X3D::X3DError & error)
						{
							field -> set (MFNode ());
							field -> addEvent ();

							script -> getBrowser () -> print (error .what ());
						}

						JS_SET_RVAL (cx, vp, JSVAL_VOID);
						return true;
					}
					catch (const X3D::Error <X3D::INVALID_NAME> &)
					{
						return ThrowException (cx, "Browser .createX3DFromURL: no such field '%s' in node %s.", event .c_str (), node -> getValue () -> getTypeName () .c_str ());
					}
				}
			}
			catch (const std::exception &)
			{ }

			const auto scene   = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);
			auto       nodeObj = argc == 2 ? context -> getGlobal () : JSVAL_TO_OBJECT (argv [1]);

			if (not nodeObj)
				nodeObj = context -> getGlobal ();

			jsval argv [2];

			jsX3DScene::create (cx, scene, &argv [0]);
			JS_NewNumberValue (cx, script -> getCurrentTime (), &argv [1]);

			jsval rval;

			if (not JS_CallFunctionName (cx, nodeObj, event .c_str (), 2, argv, &rval))
			{
				return ThrowException (cx, "Browser .createX3DFromURL: Couldn't call function '%s'.", event .c_str ());
			}

			JS_SET_RVAL (cx, vp, JSVAL_VOID);
			return true;
		}
		else
		{
			const auto scene = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);

			return jsX3DScene::create (cx, scene, vp);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::loadURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .loadURL: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv      = JS_ARGV (cx, vp);
		const auto script    = getContext (cx) -> getScriptNode ();
		const auto url       = getArgument <jsMFString> (cx, argv, 0);
		const auto parameter = getArgument <jsMFString> (cx, argv, 1);

		Loader (script -> getExecutionContext (), script -> getWorldURL ()) .loadURL (*url, *parameter);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::getRenderingProperty (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getRenderingProperty: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv    = JS_ARGV (cx, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, argv, 0);

		try
		{
			const auto field = browser -> getRenderingProperties () -> getField (name);

			return getValue (cx, field, vp);
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			return ThrowException (cx, "Browser .getRenderingProperty: unknown property '%s'.", name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::getBrowserProperty (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getBrowserProperty: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv    = JS_ARGV (cx, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, argv, 0);

		try
		{
			const auto field = browser -> getBrowserProperties () -> getField (name);

			return getValue (cx, field, vp);
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			return ThrowException (cx, "Browser .getBrowserProperty: unknown property '%s'.", name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::getBrowserOption (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getBrowserOption: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv    = JS_ARGV (cx, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, argv, 0);

		try
		{
			const auto field = browser -> getBrowserOptions () -> getField (name);

			return getValue (cx, field, vp);
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			return ThrowException (cx, "Browser .getBrowserOption: unknown property '%s'.", name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::setBrowserOption (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .setBrowserOption: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv    = JS_ARGV (cx, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, argv, 0);

		try
		{
			const auto field = browser -> getBrowserOptions () -> getField (name);

			if (not setValue (cx, field, &argv [1]))
				return ThrowException (cx, "Browser .setBrowserOption: couldn't set value for option '%s'.", name .c_str ());

			JS_SET_RVAL (cx, vp, JSVAL_VOID);
			return true;
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			return ThrowException (cx, "Browser .setBrowserOption: unknown property '%s'.", name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::firstViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .firstViewpoint: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	browser -> firstViewpoint ();

	return true;
}

JSBool
jsBrowser::previousViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .previousViewpoint: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	browser -> previousViewpoint ();

	return true;
}

JSBool
jsBrowser::nextViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .nextViewpoint: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	browser -> nextViewpoint ();

	return true;
}

JSBool
jsBrowser::lastViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .lastViewpoint: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	browser -> lastViewpoint ();

	return true;
}

JSBool
jsBrowser::print (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto browser = getContext (cx) -> getBrowser ();
	const auto argv    = JS_ARGV (cx, vp);

	for (uint32_t i = 0; i < argc; ++ i)
		browser -> print (to_string (cx, argv [i]));

	JS_SET_RVAL (cx, vp, JSVAL_VOID);
	return true;
}

JSBool
jsBrowser::println (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto browser = getContext (cx) -> getBrowser ();

	print (cx, argc, vp);

	browser -> print ('\n');

	return true;
}

// VRML97 functions

JSBool
jsBrowser::getName (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getName: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	return JS_NewStringValue (cx, browser -> getName (), &JS_RVAL (cx, vp));
}

JSBool
jsBrowser::getVersion (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getVersion: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	return JS_NewStringValue (cx, browser -> getVersion (), &JS_RVAL (cx, vp));
}

JSBool
jsBrowser::getCurrentSpeed (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getCurrentSpeed: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	return JS_NewNumberValue (cx, browser -> getCurrentSpeed (), &JS_RVAL (cx, vp));
}

JSBool
jsBrowser::getCurrentFrameRate (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getCurrentFrameRate: wrong number of arguments.", getClass () -> name);

	const auto browser = getContext (cx) -> getBrowser ();

	return JS_NewNumberValue (cx, browser -> getCurrentFrameRate (), &JS_RVAL (cx, vp));
}

JSBool
jsBrowser::getWorldURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getWorldURL: wrong number of arguments.", getClass () -> name);

	const auto script = getContext (cx) -> getScriptNode ();

	return JS_NewStringValue (cx, script -> getExecutionContext () -> getWorldURL (), &JS_RVAL (cx, vp));
}

JSBool
jsBrowser::createVrmlFromString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .createVrmlFromString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv       = JS_ARGV (cx, vp);
		const auto script     = getContext (cx) -> getScriptNode ();
		const auto vrmlSyntax = getArgument <std::string> (cx, argv, 0);

		try
		{
			const auto scene = X3D::Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);

			return jsMFNode::create (cx, new X3D::MFNode (scene -> getRootNodes ()), &JS_RVAL (cx, vp));
		}
		catch (const X3DError & error)
		{
			script -> getBrowser () -> print (error .what ());

			return jsMFNode::create (cx, new X3D::MFNode (), &JS_RVAL (cx, vp));
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

//createVrmlFromURL(MFString url, Node node, String event)
JSBool
jsBrowser::createVrmlFromURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 3)
		return ThrowException (cx, "%s .createVrmlFromURL: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv    = JS_ARGV (cx, vp);
		const auto context = getContext (cx);
		const auto script  = context -> getScriptNode ();
		const auto url     = getArgument <jsMFString> (cx, argv, 0);
		const auto node    = getArgument <jsSFNode> (cx, argv, 1);
		const auto event   = getArgument <std::string> (cx, argv, 2);

		if (not node -> getValue ())
			return ThrowException (cx, "Browser .createVrmlFromURL: node is NULL.");

		try
		{
			const auto field = node -> getValue () -> getField (event);

			if (not field -> isInput ())
				return ThrowException (cx, "Browser .createVrmlFromURL: field '%s' is not an eventIn.", event .c_str ());

			if (field -> getType () not_eq X3D::X3DConstants::MFNode)
				return ThrowException (cx, "Browser .createVrmlFromURL: field '%s' is not a MFNode.", event .c_str ());

			if (context -> getFuture ())
				context -> getFuture () -> wait ();

			using namespace std::placeholders;

			context -> getFuture () .reset (new X3D::SceneLoader (script -> getExecutionContext (),
			                                                      *url,
			                                                      std::bind (&jsBrowser::setSceneAsync,
			                                                                 X3D::SFNode (script),
			                                                                 *node,
			                                                                 static_cast <X3D::MFNode*> (field),
			                                                                 _1)));

			JS_SET_RVAL (cx, vp, JSVAL_VOID);
			return true;
		}
		catch (const Error <INVALID_NAME> &)
		{
			return ThrowException (cx, "Browser .createVrmlFromURL: no such field '%s' in node %s.", event .c_str (), node -> getValue () -> getTypeName () .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

void
jsBrowser::setSceneAsync (const X3D::SFNode & script, const X3D::SFNode & node, X3D::MFNode* const field, X3D::X3DScenePtr && scene)
{
	if (scene)
	{
		const auto executionContext = script -> getExecutionContext ();

		executionContext -> isLive () .addInterest (scene -> isLive ());

		scene -> isLive () = executionContext -> isLive ();
		scene -> setup ();

		*field = scene -> getRootNodes ();
	}
}

JSBool
jsBrowser::addRoute (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 4)
		return ThrowException (cx, "%s .addRoute: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto script       = getContext (cx) -> getScriptNode ();
		const auto fromNode     = getArgument <jsSFNode> (cx, argv, 0);
		const auto fromEventOut = getArgument <std::string> (cx, argv, 1);
		const auto toNode       = getArgument <jsSFNode> (cx, argv, 2);
		const auto toEventIn    = getArgument <std::string> (cx, argv, 3);

		script -> getExecutionContext ()  -> addRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .addRoute: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsBrowser::deleteRoute (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 4)
		return ThrowException (cx, "%s .deleteRoute: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto script       = getContext (cx) -> getScriptNode ();
		const auto fromNode     = getArgument <jsSFNode> (cx, argv, 0);
		const auto fromEventOut = getArgument <std::string> (cx, argv, 1);
		const auto toNode       = getArgument <jsSFNode> (cx, argv, 2);
		const auto toEventIn    = getArgument <std::string> (cx, argv, 3);

		script -> getExecutionContext () -> deleteRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::setDescription (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .setDescription: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv        = JS_ARGV (cx, vp);
		const auto script      = getContext (cx) -> getScriptNode ();
		const auto description = getArgument <std::string> (cx, argv, 0);

		script -> getBrowser () -> setDescription (description);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

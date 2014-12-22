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
#include "Fields/jsArrayFields.h"
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
	JS_PropertyStub,
	JS_DeletePropertyStub,
	JS_PropertyStub, 
	JS_StrictPropertyStub,
	JS_EnumerateStub,
	JS_ResolveStub,
	JS_ConvertStub,
	nullptr,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

// X3D properties
JSPropertySpec jsBrowser::properties [ ] = {
	JS_PSG  ("name",                getName,                                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSG  ("version",             getVersion,                             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSG  ("currentSpeed",        getCurrentSpeed,                        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSG  ("currentFrameRate",    getCurrentFrameRate,                    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSGS ("description",         getDescription,         setDescription, JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT                  ),
	JS_PSG  ("supportedComponents", getSupportedComponents,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSG  ("supportedProfiles",   getSupportedProfiles,                   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSG  ("currentScene",        getCurrentScene,                        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PS_END

};

JSFunctionSpec jsBrowser::functions [ ] = {
	// X3D functions
	JS_FS ("replaceWorld",         replaceWorld,         1, JSPROP_PERMANENT),
	JS_FS ("createX3DFromString",  createX3DFromString,  1, JSPROP_PERMANENT),
	JS_FS ("createX3DFromURL",     createX3DFromURL,     1, JSPROP_PERMANENT),
	JS_FS ("loadURL",              loadURL,              2, JSPROP_PERMANENT),
	JS_FS ("getRenderingProperty", getRenderingProperty, 1, JSPROP_PERMANENT),
	JS_FS ("getBrowserProperty",   getBrowserProperty,   1, JSPROP_PERMANENT),
	JS_FS ("getBrowserOption",     getBrowserOption,     1, JSPROP_PERMANENT),
	JS_FS ("setBrowserOption",     setBrowserOption,     2, JSPROP_PERMANENT),

	JS_FS ("firstViewpoint",       firstViewpoint,       0, JSPROP_PERMANENT),
	JS_FS ("previousViewpoint",    previousViewpoint,    0, JSPROP_PERMANENT),
	JS_FS ("nextViewpoint",        nextViewpoint,        0, JSPROP_PERMANENT),
	JS_FS ("lastViewpoint",        lastViewpoint,        0, JSPROP_PERMANENT),

	JS_FS ("print",                print,                0, JSPROP_PERMANENT),
	JS_FS ("println",              println,              0, JSPROP_PERMANENT),

	// VRML functions
	JS_FS ("getName",              getName,              0, JSPROP_PERMANENT),
	JS_FS ("getVersion",           getVersion,           0, JSPROP_PERMANENT),
	JS_FS ("getCurrentSpeed",      getCurrentSpeed,      0, JSPROP_PERMANENT),
	JS_FS ("getCurrentFrameRate",  getCurrentFrameRate,  0, JSPROP_PERMANENT),
	JS_FS ("getWorldURL",          getWorldURL,          0, JSPROP_PERMANENT),
	JS_FS ("setDescription",       setDescription,       1, JSPROP_PERMANENT),
	JS_FS ("createVrmlFromString", createVrmlFromString, 1, JSPROP_PERMANENT),
	JS_FS ("createVrmlFromURL",    createVrmlFromURL,    3, JSPROP_PERMANENT),
	JS_FS ("addRoute",             addRoute,             4, JSPROP_PERMANENT),
	JS_FS ("deleteRoute",          deleteRoute,          4, JSPROP_PERMANENT),

	JS_FS_END

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
jsBrowser::getName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	args .rval () .set (StringValue (cx, browser -> getName ()));
	return true;
}

JSBool
jsBrowser::getVersion (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	args .rval () .set (StringValue (cx, browser -> getVersion ()));
	return true;
}

JSBool
jsBrowser::getCurrentSpeed (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	args .rval () .setDouble (browser -> getCurrentSpeed ());
	return true;
}

JSBool
jsBrowser::getCurrentFrameRate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	args .rval () .setDouble (browser -> getCurrentFrameRate ());
	return true;
}

JSBool
jsBrowser::setDescription (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		browser -> setDescription (getArgument <std::string> (cx, args, 0));

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::getDescription (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	args .rval () .set (StringValue (cx, browser -> getDescription ()));
	return true;
}

JSBool
jsBrowser::getSupportedComponents (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	args .rval () .set (jsComponentInfoArray::create (cx, &browser -> getSupportedComponents ()));
	return true;
}

JSBool
jsBrowser::getSupportedProfiles (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	args .rval () .set (jsProfileInfoArray::create (cx, &browser -> getSupportedProfiles ()));
	return true;
}

JSBool
jsBrowser::getCurrentScene (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args             = JS::CallArgsFromVp (argc, vp);
	const auto executionContext = getContext (cx) -> getExecutionContext ();
	const auto scene            = dynamic_cast <X3DScene*> (executionContext);

	if (scene)
		args .rval () .set (jsX3DScene::create (cx, scene));
	else
		args .rval () .set (jsX3DExecutionContext::create (cx, executionContext));

	return true;
}

// X3D functions

JSBool
jsBrowser::replaceWorld (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .replaceWorld: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto script = getContext (cx) -> getScriptNode ();

		try
		{
			const X3D::X3DScenePtr scene = getArgument <jsX3DScene> (cx, args, 0);

			script -> getBrowser () -> replaceWorld (scene);
		}
		catch (const std::domain_error &)
		{
			script -> getBrowser () -> replaceWorld (nullptr);
		}

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::createX3DFromString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .createX3DFromString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args      = JS::CallArgsFromVp (argc, vp);
		const auto script    = getContext (cx) -> getScriptNode ();
		const auto x3dSyntax = getArgument <std::string> (cx, args, 0);
		const auto scene     = X3D::Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (x3dSyntax);

		args .rval () .set (jsX3DScene::create (cx, scene));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::createX3DFromURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc > 3)
		return ThrowException (cx, "%s .createX3DFromURL: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto context = getContext (cx);
		const auto script  = context -> getScriptNode ();
		const auto url     = getArgument <jsMFString> (cx, args, 0);

		if (argc > 1)
		{
			const auto event = getArgument <std::string> (cx, args, 1);

			try
			{
				if (argc == 3)
				{
					const auto node = getArgument <jsSFNode> (cx, args, 2);

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

						args .rval () .setUndefined ();
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
			auto       nodeObj = argc == 2 or not args [1] .isObject () ? context -> getGlobal () : args [1] .toObjectOrNull ();

			if (not nodeObj)
				nodeObj = context -> getGlobal ();

			JS::Value fargs [2] = {
				jsX3DScene::create (cx, scene),
				JS::DoubleValue (script -> getCurrentTime ())
			};

			JS::Value rval;

			if (not JS_CallFunctionName (cx, nodeObj, event .c_str (), 2, fargs, &rval))
			{
				return ThrowException (cx, "Browser .createX3DFromURL: Couldn't call function '%s'.", event .c_str ());
			}

			args .rval () .setUndefined ();
			return true;
		}
		else
		{
			const auto scene = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);

			args .rval () .set (jsX3DScene::create (cx, scene));
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::loadURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .loadURL: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args      = JS::CallArgsFromVp (argc, vp);
		const auto script    = getContext (cx) -> getScriptNode ();
		const auto url       = getArgument <jsMFString> (cx, args, 0);
		const auto parameter = getArgument <jsMFString> (cx, args, 1);

		Loader (script -> getExecutionContext (), script -> getWorldURL ()) .loadURL (*url, *parameter);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

JSBool
jsBrowser::getRenderingProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getRenderingProperty: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getRenderingProperties () -> getField (name);

			args .rval () .set (getValue (cx, field));
			return true;
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
jsBrowser::getBrowserProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getBrowserProperty: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getBrowserProperties () -> getField (name);

			args .rval () .set (getValue (cx, field));
			return true;
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
jsBrowser::getBrowserOption (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getBrowserOption: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getBrowserOptions () -> getField (name);

			args .rval () .set (getValue (cx, field));
			return true;
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
jsBrowser::setBrowserOption (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .setBrowserOption: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getBrowserOptions () -> getField (name);

			setValue (cx, field, &args [1]);

			args .rval () .setUndefined ();
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
jsBrowser::firstViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .firstViewpoint: wrong number of arguments.", getClass () -> name);

	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	browser -> firstViewpoint ();

	args .rval () .setUndefined ();
	return true;
}

JSBool
jsBrowser::previousViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .previousViewpoint: wrong number of arguments.", getClass () -> name);

	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	browser -> previousViewpoint ();

	args .rval () .setUndefined ();
	return true;
}

JSBool
jsBrowser::nextViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .nextViewpoint: wrong number of arguments.", getClass () -> name);

	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	browser -> nextViewpoint ();

	args .rval () .setUndefined ();
	return true;
}

JSBool
jsBrowser::lastViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .lastViewpoint: wrong number of arguments.", getClass () -> name);

	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	browser -> lastViewpoint ();

	args .rval () .setUndefined ();
	return true;
}

JSBool
jsBrowser::print (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	for (uint32_t i = 0; i < argc; ++ i)
		browser -> print (to_string (cx, args [i]));

	args .rval () .setUndefined ();
	return true;
}

JSBool
jsBrowser::println (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto browser = getContext (cx) -> getBrowser ();

	print (cx, argc, vp);

	browser -> print ('\n');

	return true;
}

// VRML97 functions

JSBool
jsBrowser::getWorldURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getWorldURL: wrong number of arguments.", getClass () -> name);

	const auto args   = JS::CallArgsFromVp (argc, vp);
	const auto script = getContext (cx) -> getScriptNode ();

	args .rval () .set (StringValue (cx, script -> getExecutionContext () -> getWorldURL ()));
	return true;
}

JSBool
jsBrowser::createVrmlFromString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .createVrmlFromString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args       = JS::CallArgsFromVp (argc, vp);
		const auto script     = getContext (cx) -> getScriptNode ();
		const auto vrmlSyntax = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto scene = X3D::Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);

			args .rval () .set (jsMFNode::create (cx, new X3D::MFNode (scene -> getRootNodes ())));
			return true;
		}
		catch (const X3DError & error)
		{
			script -> getBrowser () -> print (error .what ());

			args .rval () .set (jsMFNode::create (cx, new X3D::MFNode ()));
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, error .what ());
	}
}

//createVrmlFromURL(MFString url, Node node, String event)
JSBool
jsBrowser::createVrmlFromURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 3)
		return ThrowException (cx, "%s .createVrmlFromURL: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto context = getContext (cx);
		const auto script  = context -> getScriptNode ();
		const auto url     = getArgument <jsMFString> (cx, args, 0);
		const auto node    = getArgument <jsSFNode> (cx, args, 1);
		const auto event   = getArgument <std::string> (cx, args, 2);

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

			args .rval () .setUndefined ();
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
jsBrowser::addRoute (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 4)
		return ThrowException (cx, "%s .addRoute: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args         = JS::CallArgsFromVp (argc, vp);
		const auto script       = getContext (cx) -> getScriptNode ();
		const auto fromNode     = getArgument <jsSFNode> (cx, args, 0);
		const auto fromEventOut = getArgument <std::string> (cx, args, 1);
		const auto toNode       = getArgument <jsSFNode> (cx, args, 2);
		const auto toEventIn    = getArgument <std::string> (cx, args, 3);

		script -> getExecutionContext ()  -> addRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .addRoute: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsBrowser::deleteRoute (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 4)
		return ThrowException (cx, "%s .deleteRoute: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args         = JS::CallArgsFromVp (argc, vp);
		const auto script       = getContext (cx) -> getScriptNode ();
		const auto fromNode     = getArgument <jsSFNode> (cx, args, 0);
		const auto fromEventOut = getArgument <std::string> (cx, args, 1);
		const auto toNode       = getArgument <jsSFNode> (cx, args, 2);
		const auto toEventIn    = getArgument <std::string> (cx, args, 3);

		script -> getExecutionContext () -> deleteRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		args .rval () .setUndefined ();
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

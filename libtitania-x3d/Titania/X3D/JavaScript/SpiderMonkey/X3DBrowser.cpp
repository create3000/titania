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

#include "X3DBrowser.h"

#include "ComponentInfoArray.h"
#include "Context.h"
#include "Error.h"
#include "Fields/ArrayFields.h"
#include "Fields/MFNode.h"
#include "Fields/SFNode.h"
#include "ProfileInfoArray.h"
#include "String.h"
#include "X3DExecutionContext.h"
#include "X3DScene.h"

#include "../../Browser/Core/BrowserOptions.h"
#include "../../Browser/Core/BrowserProperties.h"
#include "../../Browser/Core/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Scripting/Script.h"
#include "../../InputOutput/Generator.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Thread/SceneFuture.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DBrowser::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	nullptr, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass X3DBrowser::static_class = {
	"X3DBrowser",
	JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec X3DBrowser::properties [ ] = {
	JS_PSGS ("name",                getName,                nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("version",             getVersion,             nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("currentSpeed",        getCurrentSpeed,        nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("currentFrameRate",    getCurrentFrameRate,    nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("description",         getDescription,         setDescription, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("supportedComponents", getSupportedComponents, nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("supportedProfiles",   getSupportedProfiles,   nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("currentScene",        getCurrentScene,        nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

const JSFunctionSpec X3DBrowser::functions [ ] = {
//	// X3D functions
//
//	JS_FS ("replaceWorld",         replaceWorld,         1, JSPROP_PERMANENT),
//	JS_FS ("createX3DFromString",  createX3DFromString,  1, JSPROP_PERMANENT),
//	JS_FS ("createX3DFromURL",     createX3DFromURL,     1, JSPROP_PERMANENT),
//	JS_FS ("loadURL",              loadURL,              2, JSPROP_PERMANENT),
//	JS_FS ("getRenderingProperty", getRenderingProperty, 1, JSPROP_PERMANENT),
//	JS_FS ("getBrowserProperty",   getBrowserProperty,   1, JSPROP_PERMANENT),
//	JS_FS ("getBrowserOption",     getBrowserOption,     1, JSPROP_PERMANENT),
//	JS_FS ("setBrowserOption",     setBrowserOption,     2, JSPROP_PERMANENT),
//
//	JS_FS ("firstViewpoint",       firstViewpoint,       0, JSPROP_PERMANENT),
//	JS_FS ("previousViewpoint",    previousViewpoint,    0, JSPROP_PERMANENT),
//	JS_FS ("nextViewpoint",        nextViewpoint,        0, JSPROP_PERMANENT),
//	JS_FS ("lastViewpoint",        lastViewpoint,        0, JSPROP_PERMANENT),
//
//	JS_FS ("print",                print,                1, JSPROP_PERMANENT),
//	JS_FS ("println",              println,              1, JSPROP_PERMANENT),
//
//	// VRML functions

	JS_FS ("getName",              getName,              0, JSPROP_PERMANENT),
//	JS_FS ("getVersion",           getVersion,           0, JSPROP_PERMANENT),
//	JS_FS ("getCurrentSpeed",      getCurrentSpeed,      0, JSPROP_PERMANENT),
//	JS_FS ("getCurrentFrameRate",  getCurrentFrameRate,  0, JSPROP_PERMANENT),
//	JS_FS ("getWorldURL",          getWorldURL,          0, JSPROP_PERMANENT),
//	JS_FS ("setDescription",       setDescription,       1, JSPROP_PERMANENT),
//	JS_FS ("createVrmlFromString", createVrmlFromString, 1, JSPROP_PERMANENT),
//	JS_FS ("createVrmlFromURL",    createVrmlFromURL,    3, JSPROP_PERMANENT),
//	JS_FS ("addRoute",             addRoute,             4, JSPROP_PERMANENT),
//	JS_FS ("deleteRoute",          deleteRoute,          4, JSPROP_PERMANENT),
	JS_FS_END
};

JSObject*
X3DBrowser::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

bool
X3DBrowser::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

JSObject*
X3DBrowser::create (JSContext* const cx)
{
	const auto context = getContext (cx);
	const auto obj     = JS_NewObjectWithGivenProto (cx, getClass (), context -> getProto (getId ()));

	return obj;
}

// X3D properties

bool
X3DBrowser::getName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .name: wrong number of arguments.", getClass () -> name);
	
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		args .rval () .set (StringValue (cx, browser -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .name: %s.", error .what ());
	}
}

//bool
//X3DBrowser::getVersion (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	return JS_NewStringValue (cx, script -> getBrowser () -> getVersion (), vp);
//}
//
//bool
//X3DBrowser::getCurrentSpeed (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	return JS_NewNumberValue (cx, script -> getBrowser () -> getCurrentSpeed (), vp);
//}
//
//bool
//X3DBrowser::getCurrentFrameRate (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	return JS_NewNumberValue (cx, script -> getBrowser () -> getCurrentFrameRate (), vp);
//}
//
//bool
//X3DBrowser::setDescription (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	script -> getBrowser () -> setDescription (to_string (cx, *vp));
//
//	return true;
//}
//
//bool
//X3DBrowser::getDescription (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	return JS_NewStringValue (cx, script -> getBrowser () -> getDescription (), vp);
//}
//
//bool
//X3DBrowser::getSupportedComponents (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	return ComponentInfoArray::create (cx, &script -> getBrowser () -> getSupportedComponents (), vp);
//}
//
//bool
//X3DBrowser::getSupportedProfiles (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	return ProfileInfoArray::create (cx, &script -> getBrowser () -> getSupportedProfiles (), vp);
//}

bool
X3DBrowser::getCurrentScene (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args             = JS::CallArgsFromVp (argc, vp);
		const auto & script           = getContext (cx) -> getScriptNode ();
		const auto   executionContext = script -> getExecutionContext ();
		const auto   scene            = dynamic_cast <X3D::X3DScene*> (executionContext);

		if (scene)
			args .rval () .set (X3DScene::create (cx, scene));
		else
			args .rval () .set (X3DExecutionContext::create (cx, executionContext));

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, error .what ());
	}
}

// X3D functions

//bool
//X3DBrowser::replaceWorld (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 1)
//			return ThrowException <JSProto_Error> (cx, "%s .replaceWorld: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv   = JS_ARGV (cx, vp);
//		const auto & script = getContext (cx) -> getScriptNode ();
//
//		if (JSVAL_IS_NULL (argv [0]))
//			script -> getBrowser () -> replaceWorld (nullptr);
//
//		else
//		{
//			const X3D::X3DScenePtr scene (getArgument <X3DScene> (cx, argv, 0));
//
//			script -> getBrowser () -> replaceWorld (scene);
//		}
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::createX3DFromString (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 1)
//			return ThrowException <JSProto_Error> (cx, "%s .createX3DFromString: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv      = JS_ARGV (cx, vp);
//		const auto & script    = getContext (cx) -> getScriptNode ();
//		const auto   x3dSyntax = getArgument <std::string> (cx, argv, 0);
//		const auto   scene     = X3D::FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (x3dSyntax);
//
//		return X3DScene::create (cx, scene, vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::createX3DFromURL (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc > 3)
//			return ThrowException <JSProto_Error> (cx, "%s .createX3DFromURL: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv    = JS_ARGV (cx, vp);
//		const auto   context = getContext (cx);
//		const auto & script  = context -> getScriptNode ();
//		const auto   url     = getArgument <MFString> (cx, argv, 0);
//
//		if (argc > 1)
//		{
//			const auto event = getArgument <std::string> (cx, argv, 1);
//
//			try
//			{
//				if (argc == 3)
//				{
//					const auto node = getArgument <SFNode> (cx, argv, 2);
//
//					if (not node -> getValue ())
//						return ThrowException <JSProto_Error> (cx, "Browser .createX3DFromURL: node is NULL.");
//
//					try
//					{
//						const auto field = node -> getValue () -> getField (event);
//
//						if (not field -> isInput ())
//							return ThrowException <JSProto_Error> (cx, "Browser .createX3DFromURL: field '%s' is not an eventIn.", event .c_str ());
//
//						if (field -> getType () not_eq X3D::X3DConstants::MFNode)
//							return ThrowException <JSProto_Error> (cx, "Browser .createX3DFromURL: field '%s' is not a MFNode.", event .c_str ());
//
//						try
//						{
//							const auto scene = X3D::FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);
//
//							field -> set (scene -> getRootNodes ());
//							field -> addEvent ();
//						}
//						catch (const X3D::X3DError & error)
//						{
//							script -> getBrowser () -> getConsole () -> error (error .what (), "\n");
//
//							field -> set (X3D::MFNode ());
//							field -> addEvent ();
//						}
//
//						JS_SET_RVAL (cx, vp, JSVAL_VOID);
//						return true;
//					}
//					catch (const Error <X3D::INVALID_NAME> &)
//					{
//						return ThrowException <JSProto_Error> (cx, "Browser .createX3DFromURL: no such field '%s' in node %s.", event .c_str (), node -> getValue () -> getTypeName () .c_str ());
//					}
//				}
//			}
//			catch (const std::exception &)
//			{ }
//
//			const auto scene   = FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);
//			auto       nodeObj = argc == 2 ? context -> getGlobal () : JSVAL_TO_OBJECT (argv [1]);
//
//			if (not nodeObj)
//				nodeObj = context -> getGlobal ();
//
//			jsval argv [2];
//
//			X3DScene::create (cx, scene, &argv [0]);
//			JS_NewNumberValue (cx, script -> getCurrentTime (), &argv [1]);
//
//			jsval rval;
//
//			if (not JS_CallFunctionName (cx, nodeObj, event .c_str (), 2, argv, &rval))
//			{
//				return ThrowException <JSProto_Error> (cx, "Browser .createX3DFromURL: Couldn't call function '%s'.", event .c_str ());
//			}
//
//			JS_SET_RVAL (cx, vp, JSVAL_VOID);
//			return true;
//		}
//		else
//		{
//			const auto scene = FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);
//
//			return X3DScene::create (cx, scene, vp);
//		}
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::loadURL (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 2)
//			return ThrowException <JSProto_Error> (cx, "%s .loadURL: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv      = JS_ARGV (cx, vp);
//		const auto & script    = getContext (cx) -> getScriptNode ();
//		const auto   url       = getArgument <MFString> (cx, argv, 0);
//		const auto   parameter = getArgument <MFString> (cx, argv, 1);
//
//		FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .loadURL (*url, *parameter);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::getRenderingProperty (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 1)
//			return ThrowException <JSProto_Error> (cx, "%s .getRenderingProperty: wrong number of arguments.", getClass () -> name);
//	
//		const auto argv    = JS_ARGV (cx, vp);
//		const auto browser = getContext (cx) -> getBrowser ();
//		const auto name    = getArgument <std::string> (cx, argv, 0);
//
//		try
//		{
//			const auto field = browser -> getRenderingProperties () -> getField (name);
//
//			return getValue (cx, field, vp);
//		}
//		catch (const Error <X3D::INVALID_NAME> &)
//		{
//			return ThrowException <JSProto_Error> (cx, "Browser .getRenderingProperty: unknown property '%s'.", name .c_str ());
//		}
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::getBrowserProperty (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 1)
//			return ThrowException <JSProto_Error> (cx, "%s .getBrowserProperty: wrong number of arguments.", getClass () -> name);
//	
//		const auto argv    = JS_ARGV (cx, vp);
//		const auto browser = getContext (cx) -> getBrowser ();
//		const auto name    = getArgument <std::string> (cx, argv, 0);
//
//		try
//		{
//			const auto field = browser -> getBrowserProperties () -> getField (name);
//
//			return getValue (cx, field, vp);
//		}
//		catch (const Error <X3D::INVALID_NAME> &)
//		{
//			return ThrowException <JSProto_Error> (cx, "Browser .getBrowserProperty: unknown property '%s'.", name .c_str ());
//		}
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::getBrowserOption (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 1)
//			return ThrowException <JSProto_Error> (cx, "%s .getBrowserOption: wrong number of arguments.", getClass () -> name);
//	
//		const auto argv    = JS_ARGV (cx, vp);
//		const auto browser = getContext (cx) -> getBrowser ();
//		const auto name    = getArgument <std::string> (cx, argv, 0);
//
//		try
//		{
//			const auto field = browser -> getBrowserOptions () -> getField (name);
//
//			return getValue (cx, field, vp);
//		}
//		catch (const Error <X3D::INVALID_NAME> &)
//		{
//			return ThrowException <JSProto_Error> (cx, "Browser .getBrowserOption: unknown property '%s'.", name .c_str ());
//		}
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::setBrowserOption (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 2)
//			return ThrowException <JSProto_Error> (cx, "%s .setBrowserOption: wrong number of arguments.", getClass () -> name);
//	
//		const auto argv    = JS_ARGV (cx, vp);
//		const auto browser = getContext (cx) -> getBrowser ();
//		const auto name    = getArgument <std::string> (cx, argv, 0);
//
//		try
//		{
//			const auto field = browser -> getBrowserOptions () -> getField (name);
//
//			if (not setValue (cx, field, &argv [1]))
//				return ThrowException <JSProto_Error> (cx, "Browser .setBrowserOption: couldn't set value for option '%s'.", name .c_str ());
//
//			JS_SET_RVAL (cx, vp, JSVAL_VOID);
//			return true;
//		}
//		catch (const Error <X3D::INVALID_NAME> &)
//		{
//			return ThrowException <JSProto_Error> (cx, "Browser .setBrowserOption: unknown property '%s'.", name .c_str ());
//		}
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
//bool
//X3DBrowser::firstViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .firstViewpoint: wrong number of arguments.", getClass () -> name);
//
//	const auto browser = getContext (cx) -> getBrowser ();
//
//	browser -> firstViewpoint ();
//
//	return true;
//}
//
//bool
//X3DBrowser::previousViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .previousViewpoint: wrong number of arguments.", getClass () -> name);
//
//	const auto browser = getContext (cx) -> getBrowser ();
//
//	browser -> previousViewpoint ();
//
//	return true;
//}
//
//bool
//X3DBrowser::nextViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .nextViewpoint: wrong number of arguments.", getClass () -> name);
//
//	const auto browser = getContext (cx) -> getBrowser ();
//
//	browser -> nextViewpoint ();
//
//	return true;
//}
//
//bool
//X3DBrowser::lastViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .lastViewpoint: wrong number of arguments.", getClass () -> name);
//
//	const auto browser = getContext (cx) -> getBrowser ();
//
//	browser -> lastViewpoint ();
//
//	return true;
//}
//
//bool
//X3DBrowser::print (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto browser = getContext (cx) -> getBrowser ();
//	const auto argv    = JS_ARGV (cx, vp);
//
//	for (uint32_t i = 0; i < argc; ++ i)
//		browser -> print (to_string (cx, argv [i]));
//
//	JS_SET_RVAL (cx, vp, JSVAL_VOID);
//	return true;
//}
//
//bool
//X3DBrowser::println (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	const auto browser = getContext (cx) -> getBrowser ();
//
//	print (cx, argc, vp);
//
//	browser -> println ();
//
//	return true;
//}
//
//// VRML97 functions
//
//bool
//X3DBrowser::getWorldURL (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .getWorldURL: wrong number of arguments.", getClass () -> name);
//
//	const auto & script = getContext (cx) -> getScriptNode ();
//
//	return JS_NewStringValue (cx, script -> getExecutionContext () -> getWorldURL (), &JS_RVAL (cx, vp));
//}
//
//bool
//X3DBrowser::createVrmlFromString (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 1)
//			return ThrowException <JSProto_Error> (cx, "%s .createVrmlFromString: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv       = JS_ARGV (cx, vp);
//		const auto & script     = getContext (cx) -> getScriptNode ();
//		const auto   vrmlSyntax = getArgument <std::string> (cx, argv, 0);
//
//		try
//		{
//			const auto scene = X3D::FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);
//
//			return X3DField::create <MFNode> (cx, new X3D::MFNode (scene -> getRootNodes ()), &JS_RVAL (cx, vp));
//		}
//		catch (const X3DError & error)
//		{
//			script -> getBrowser () -> getConsole () -> error (error .what (), "\n");
//
//			return X3DField::create <MFNode> (cx, new X3D::MFNode (), &JS_RVAL (cx, vp));
//		}
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
////createVrmlFromURL(MFString url, Node node, String event)
//bool
//X3DBrowser::createVrmlFromURL (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 3)
//			return ThrowException <JSProto_Error> (cx, "%s .createVrmlFromURL: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv    = JS_ARGV (cx, vp);
//		const auto   context = getContext (cx);
//		const auto & script  = context -> getScriptNode ();
//		const auto   url     = getArgument <MFString> (cx, argv, 0);
//		const auto   node    = getArgument <SFNode> (cx, argv, 1);
//		const auto   event   = getArgument <std::string> (cx, argv, 2);
//
//		if (not node -> getValue ())
//			return ThrowException <JSProto_Error> (cx, "Browser .createVrmlFromURL: node is NULL.");
//
//		try
//		{
//			const auto field = node -> getValue () -> getField (event);
//
//			if (not field -> isInput ())
//				return ThrowException <JSProto_Error> (cx, "Browser .createVrmlFromURL: field '%s' is not an eventIn.", event .c_str ());
//
//			if (field -> getType () not_eq X3D::X3DConstants::MFNode)
//				return ThrowException <JSProto_Error> (cx, "Browser .createVrmlFromURL: field '%s' is not a MFNode.", event .c_str ());
//
//			if (context -> getFuture ())
//				context -> getFuture () -> wait ();
//
//			using namespace std::placeholders;
//
//			context -> getFuture () .setValue (new X3D::SceneFuture (script -> getExecutionContext (),
//			                                                         *url,
//			                                                         false,
//			                                                         std::bind (&X3DBrowser::setSceneAsync,
//			                                                                    X3D::SFNode (script),
//			                                                                    *node,
//			                                                                    static_cast <X3D::MFNode*> (field),
//			                                                                    _1)));
//
//			JS_SET_RVAL (cx, vp, JSVAL_VOID);
//			return true;
//		}
//		catch (const Error <INVALID_NAME> &)
//		{
//			return ThrowException <JSProto_Error> (cx, "Browser .createVrmlFromURL: no such field '%s' in node %s.", event .c_str (), node -> getValue () -> getTypeName () .c_str ());
//		}
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}
//
////  XXX: use FieldPtr here for "X3D::MFNode* const field"
//void
//X3DBrowser::setSceneAsync (const X3D::SFNode & script, const X3D::SFNode & node, X3D::MFNode* const field, X3D::X3DScenePtr && scene)
//{
//	if (scene)
//	{
//		const auto executionContext = script -> getExecutionContext ();
//
//		executionContext -> isLive () .addInterest (scene -> isLive ());
//
//		scene -> isLive () = executionContext -> isLive ();
//		scene -> setup ();
//
//		*field = scene -> getRootNodes ();
//	}
//}
//
//bool
//X3DBrowser::addRoute (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 4)
//			return ThrowException <JSProto_Error> (cx, "%s .addRoute: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv         = JS_ARGV (cx, vp);
//		const auto & script       = getContext (cx) -> getScriptNode ();
//		const auto   fromNode     = getArgument <SFNode> (cx, argv, 0);
//		const auto   fromEventOut = getArgument <std::string> (cx, argv, 1);
//		const auto   toNode       = getArgument <SFNode> (cx, argv, 2);
//		const auto   toEventIn    = getArgument <std::string> (cx, argv, 3);
//
//		script -> getExecutionContext ()  -> addRoute (*fromNode, fromEventOut, *toNode, toEventIn);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .addRoute: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DBrowser::deleteRoute (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		if (argc not_eq 4)
//			return ThrowException <JSProto_Error> (cx, "%s .deleteRoute: wrong number of arguments.", getClass () -> name);
//	
//		const auto   argv         = JS_ARGV (cx, vp);
//		const auto & script       = getContext (cx) -> getScriptNode ();
//		const auto   fromNode     = getArgument <SFNode> (cx, argv, 0);
//		const auto   fromEventOut = getArgument <std::string> (cx, argv, 1);
//		const auto   toNode       = getArgument <SFNode> (cx, argv, 2);
//		const auto   toEventIn    = getArgument <std::string> (cx, argv, 3);
//
//		script -> getExecutionContext () -> deleteRoute (*fromNode, fromEventOut, *toNode, toEventIn);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, error .what ());
//	}
//}

} // spidermonkey
} // X3D
} // titania

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
#include "value.h"
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
	nullptr, // enumerate
	nullptr, // newEnumerate
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
	JS_PSGS ("version",             getVersion,             nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("currentSpeed",        getCurrentSpeed,        nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("currentFrameRate",    getCurrentFrameRate,    nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("description",         getDescription,         setDescription, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("supportedComponents", getSupportedComponents, nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("supportedProfiles",   getSupportedProfiles,   nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("currentScene",        getCurrentScene,        nullptr,        JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

const JSFunctionSpec X3DBrowser::functions [ ] = {
	// X3D functions

	JS_FN ("replaceWorld",         replaceWorld,         1, JSPROP_PERMANENT),
	JS_FN ("createX3DFromString",  createX3DFromString,  1, JSPROP_PERMANENT),
	JS_FN ("createX3DFromURL",     createX3DFromURL,     1, JSPROP_PERMANENT),
	JS_FN ("loadURL",              loadURL,              2, JSPROP_PERMANENT),
	JS_FN ("getRenderingProperty", getRenderingProperty, 1, JSPROP_PERMANENT),
	JS_FN ("getBrowserProperty",   getBrowserProperty,   1, JSPROP_PERMANENT),
	JS_FN ("getBrowserOption",     getBrowserOption,     1, JSPROP_PERMANENT),
	JS_FN ("setBrowserOption",     setBrowserOption,     2, JSPROP_PERMANENT),

	JS_FN ("firstViewpoint",       firstViewpoint,       0, JSPROP_PERMANENT),
	JS_FN ("previousViewpoint",    previousViewpoint,    0, JSPROP_PERMANENT),
	JS_FN ("nextViewpoint",        nextViewpoint,        0, JSPROP_PERMANENT),
	JS_FN ("lastViewpoint",        lastViewpoint,        0, JSPROP_PERMANENT),

	JS_FN ("print",                print,                1, JSPROP_PERMANENT),
	JS_FN ("println",              println,              1, JSPROP_PERMANENT),

	// VRML functions

	JS_FN ("getName",              getName,              0, JSPROP_PERMANENT),
	JS_FN ("getVersion",           getVersion,           0, JSPROP_PERMANENT),
	JS_FN ("getCurrentSpeed",      getCurrentSpeed,      0, JSPROP_PERMANENT),
	JS_FN ("getCurrentFrameRate",  getCurrentFrameRate,  0, JSPROP_PERMANENT),
	JS_FN ("getWorldURL",          getWorldURL,          0, JSPROP_PERMANENT),
	JS_FN ("setDescription",       setDescription,       1, JSPROP_PERMANENT),
	JS_FN ("createVrmlFromString", createVrmlFromString, 1, JSPROP_PERMANENT),
	JS_FN ("createVrmlFromURL",    createVrmlFromURL,    3, JSPROP_PERMANENT),
	JS_FN ("addRoute",             addRoute,             4, JSPROP_PERMANENT),
	JS_FN ("deleteRoute",          deleteRoute,          4, JSPROP_PERMANENT),
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
		return ThrowException <JSProto_Error> (cx, "%s .prototype .name: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getVersion (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .version: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		args .rval () .set (StringValue (cx, browser -> getVersion ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .version: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getCurrentSpeed (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .currentSpeed: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		args .rval () .setNumber (browser -> getCurrentSpeed ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .currentSpeed: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getCurrentFrameRate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .currentFrameRate: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		args .rval () .setNumber (browser -> getCurrentFrameRate ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .currentFrameRate: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::setDescription (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .description: wrong number of arguments.", getClass () -> name);

		const auto args        = JS::CallArgsFromVp (argc, vp);
		const auto browser     = getContext (cx) -> getBrowser ();
		const auto description = getArgument <std::string> (cx, args, 0);

		browser -> setDescription (description);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .description: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getDescription (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .description: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		args .rval () .set (StringValue (cx, browser -> getDescription ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .description: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getSupportedComponents (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		args .rval () .set (ComponentInfoArray::create (cx, const_cast <X3D::ComponentInfoArray*> (&browser -> getSupportedComponents ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .supportedComponents: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getSupportedProfiles (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		args .rval () .set (ProfileInfoArray::create (cx, const_cast <X3D::ProfileInfoArray*> (&browser -> getSupportedProfiles ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .supportedProfiles: %s.", getClass () -> name, error .what ());
	}
}

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
		return ThrowException <JSProto_Error> (cx, "%s .prototype .currentScene: %s.", getClass () -> name, error .what ());
	}
}

// X3D functions

bool
X3DBrowser::replaceWorld (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .replaceWorld: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		try
		{
			const auto scene = X3D::X3DScenePtr (*getArgument <X3DScene> (cx, args, 0));

			browser -> replaceWorld (scene);
		}
		catch (const std::domain_error & error)
		{
			browser -> replaceWorld (nullptr);
		}

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .replaceWorld: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::createX3DFromString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromString: wrong number of arguments.", getClass () -> name);

		const auto   args      = JS::CallArgsFromVp (argc, vp);
		const auto & script    = getContext (cx) -> getScriptNode ();
		const auto   x3dSyntax = getArgument <std::string> (cx, args, 0);
		const auto   scene     = X3D::FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (x3dSyntax);

		args .rval () .set (X3DScene::create (cx, scene));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromString: %s.", getClass () -> name, error .what ());
	}
}

// createX3DFromURL (url, event, node);
bool
X3DBrowser::createX3DFromURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1 and argc not_eq 3)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromURL: wrong number of arguments.", getClass () -> name);

		const auto   args    = JS::CallArgsFromVp (argc, vp);
		const auto   context = getContext (cx);
		const auto & script  = context -> getScriptNode ();
		const auto   url     = getArgument <MFString> (cx, args, 0);

		if (argc == 1)
		{
			const auto scene = FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);

			args .rval () .set (X3DScene::create (cx, scene));
			return true;
		}
		else
		{
			const auto event = getArgument <std::string> (cx, args, 1);

			try
			{
				const auto node = getArgument <SFNode> (cx, args, 2);

				if (not node -> getValue ())
					return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromURL: node is NULL.", getClass () -> name);

				try
				{
					const auto field = node -> getValue () -> getField (event);

					if (not field -> isInput ())
						return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromURL: field '%s' is not an eventIn.", getClass () -> name, event .c_str ());

					if (field -> getType () not_eq X3D::X3DConstants::MFNode)
						return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromURL: field '%s' is not a MFNode.", getClass () -> name, event .c_str ());

					using namespace std::placeholders;

					context -> addFuture (X3D::MakePtr <X3D::SceneFuture> (script -> getExecutionContext (),
					                                                       *url,
					                                                       false,
					                                                       std::bind (&X3DBrowser::setSceneAsync,
					                                                                  X3D::SFNode (script),
					                                                                  *node,
					                                                                  static_cast <X3D::MFNode*> (field),
					                                                                  _1)));

					args .rval () .setUndefined ();
					return true;
				}
				catch (const Error <INVALID_NAME> &)
				{
					return ThrowException <JSProto_Error> (cx, "%s .createX3DFromURL: no such field '%s' in node %s.", getClass () -> name, event .c_str (), node -> getValue () -> getTypeName () .c_str ());
				}
			}
			catch (const std::exception &)
			{ }

			const auto scene   = FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);
			auto       nodeObj = argc == 2 or not args [2] .isObject () ? context -> getGlobal () : args [2] .toObjectOrNull ();

			if (not nodeObj)
				nodeObj = context -> getGlobal ();

			JS::RootedValue     rval (cx);
			JS::AutoValueVector argv (cx);

			argv .append (X3DScene::create (cx, scene));
			argv .append (JS::DoubleValue (script -> getCurrentTime ()));

			JS::RootedObject rooted (cx, nodeObj);

			if (not JS_CallFunctionName (cx, rooted, event .c_str (), argv, &rval))
			{
				return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromURL: Couldn't call function '%s'.", getClass () -> name, event .c_str ());
			}

			args .rval () .setUndefined ();
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .createX3DFromURL: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::loadURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .loadURL: wrong number of arguments.", getClass () -> name);

		const auto   args      = JS::CallArgsFromVp (argc, vp);
		const auto & script    = getContext (cx) -> getScriptNode ();
		const auto   url       = getArgument <MFString> (cx, args, 0);
		const auto   parameter = getArgument <MFString> (cx, args, 1);

		FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .loadURL (*url, *parameter);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .loadURL: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getRenderingProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getRenderingProperty: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getRenderingProperties () -> getField (name);

			args .rval () .set (getValue (cx, field));
			return true;
		}
		catch (const Error <X3D::INVALID_NAME> & error)
		{
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getRenderingProperty: unknown property '%s'.", getClass () -> name, name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getRenderingProperty: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getBrowserProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getBrowserProperty: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getBrowserProperties () -> getField (name);

			args .rval () .set (getValue (cx, field));
			return true;
		}
		catch (const Error <X3D::INVALID_NAME> &)
		{
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getBrowserProperty: unknown property '%s'.", getClass () -> name, name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getBrowserProperty: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::getBrowserOption (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getBrowserOption: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getBrowserOptions () -> getField (name);

			args .rval () .set (getValue (cx, field));
			return true;
		}
		catch (const Error <X3D::INVALID_NAME> &)
		{
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getBrowserOption: unknown property '%s'.", getClass () -> name, name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getBrowserOption: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::setBrowserOption (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .setBrowserOption: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();
		const auto name    = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto field = browser -> getBrowserOptions () -> getField (name);

			setValue (cx, field, args [1]);

			args .rval () .setUndefined ();
			return true;
		}
		catch (const Error <X3D::INVALID_NAME> &)
		{
			return ThrowException <JSProto_Error> (cx, "%s .prototype .setBrowserOption: unknown property '%s'.", getClass () -> name, name .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .setBrowserOption: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::firstViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .firstViewpoint: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		browser -> firstViewpoint ();

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .firstViewpoint: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::previousViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .previousViewpoint: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		browser -> previousViewpoint ();

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .previousViewpoint: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::nextViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .nextViewpoint: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		browser -> nextViewpoint ();

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .nextViewpoint: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::lastViewpoint (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .lastViewpoint: wrong number of arguments.", getClass () -> name);

		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		browser -> lastViewpoint ();

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .lastViewpoint: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::print (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto browser = getContext (cx) -> getBrowser ();

		for (uint32_t i = 0; i < argc; ++ i)
			browser -> print (getArgument <std::string> (cx, args, i));

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .print: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::println (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto browser = getContext (cx) -> getBrowser ();

		print (cx, argc, vp);

		browser -> println ();

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .println: %s.", getClass () -> name, error .what ());
	}
}

// VRML97 functions

bool
X3DBrowser::getWorldURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getWorldURL: wrong number of arguments.", getClass () -> name);

		const auto args             = JS::CallArgsFromVp (argc, vp);
		const auto executionContext = getContext (cx) -> getExecutionContext ();

		args .rval () .set (StringValue (cx, executionContext -> getWorldURL ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getWorldURL: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::createVrmlFromString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .createVrmlFromString: wrong number of arguments.", getClass () -> name);

		const auto   args       = JS::CallArgsFromVp (argc, vp);
		const auto & script     = getContext (cx) -> getScriptNode ();
		const auto   vrmlSyntax = getArgument <std::string> (cx, args, 0);

		try
		{
			const auto scene = X3D::FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);

			args .rval () .set (MFNode::create (cx, new X3D::MFNode (scene -> getRootNodes ())));
			return true;
		}
		catch (const X3DError & error)
		{
			script -> getBrowser () -> getConsole () -> error (error .what (), "\n");

			args .rval () .set (MFNode::create (cx, new X3D::MFNode ()));
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .createVrmlFromString: %s.", getClass () -> name, error .what ());
	}
}

//createVrmlFromURL(MFString url, Node node, String event)
bool
X3DBrowser::createVrmlFromURL (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 3)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .createVrmlFromURL: wrong number of arguments.", getClass () -> name);

		const auto   args    = JS::CallArgsFromVp (argc, vp);
		const auto   context = getContext (cx);
		const auto & script  = context -> getScriptNode ();
		const auto   url     = getArgument <MFString> (cx, args, 0);
		const auto   node    = getArgument <SFNode> (cx, args, 1);
		const auto   event   = getArgument <std::string> (cx, args, 2);

		if (not node -> getValue ())
			return ThrowException <JSProto_Error> (cx, "%s .prototype .createVrmlFromURL: node is NULL.", getClass () -> name);

		try
		{
			const auto field = node -> getValue () -> getField (event);

			if (not field -> isInput ())
				return ThrowException <JSProto_Error> (cx, "%s .prototype .createVrmlFromURL: field '%s' is not an eventIn.", getClass () -> name, event .c_str ());

			if (field -> getType () not_eq X3D::X3DConstants::MFNode)
				return ThrowException <JSProto_Error> (cx, "%s .prototype .createVrmlFromURL: field '%s' is not a MFNode.", getClass () -> name, event .c_str ());

			using namespace std::placeholders;

			context -> addFuture (X3D::MakePtr <X3D::SceneFuture> (script -> getExecutionContext (),
			                                                       *url,
			                                                       false,
			                                                       std::bind (&X3DBrowser::setSceneAsync,
			                                                                  X3D::SFNode (script),
			                                                                  *node,
			                                                                  static_cast <X3D::MFNode*> (field),
			                                                                  _1)));

			args .rval () .setUndefined ();
			return true;
		}
		catch (const Error <INVALID_NAME> &)
		{
			return ThrowException <JSProto_Error> (cx, "%s .createVrmlFromURL: no such field '%s' in node %s.", getClass () -> name, event .c_str (), node -> getValue () -> getTypeName () .c_str ());
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .createVrmlFromURL: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::addRoute (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 4)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .addRoute: wrong number of arguments.", getClass () -> name);

		const auto   args         = JS::CallArgsFromVp (argc, vp);
		const auto & script       = getContext (cx) -> getScriptNode ();
		const auto   fromNode     = getArgument <SFNode> (cx, args, 0);
		const auto   fromEventOut = getArgument <std::string> (cx, args, 1);
		const auto   toNode       = getArgument <SFNode> (cx, args, 2);
		const auto   toEventIn    = getArgument <std::string> (cx, args, 3);

		script -> getExecutionContext ()  -> addRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .addRoute: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DBrowser::deleteRoute (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 4)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .deleteRoute: wrong number of arguments.", getClass () -> name);

		const auto   args         = JS::CallArgsFromVp (argc, vp);
		const auto & script       = getContext (cx) -> getScriptNode ();
		const auto   fromNode     = getArgument <SFNode> (cx, args, 0);
		const auto   fromEventOut = getArgument <std::string> (cx, args, 1);
		const auto   toNode       = getArgument <SFNode> (cx, args, 2);
		const auto   toEventIn    = getArgument <std::string> (cx, args, 3);

		script -> getExecutionContext () -> deleteRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .deleteRoute: %s.", getClass () -> name, error .what ());
	}
}

//  XXX: use FieldPtr here for "X3D::MFNode* const field"
void
X3DBrowser::setSceneAsync (const X3D::SFNode & script, const X3D::SFNode & node, X3D::MFNode* const field, X3D::X3DScenePtr && scene)
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

} // spidermonkey
} // X3D
} // titania

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
	{ "name",                NAME,                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, name,                NULL },
	{ "version",             VERSION,              JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, version,             NULL },
	{ "currentSpeed",        CURRENT_SPEED,        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentSpeed,        NULL },
	{ "currentFrameRate",    CURRENT_FRAMERATE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentFrameRate,    NULL },
	{ "description",         DESCRIPTION,          JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT,                   description,         description },
	{ "supportedComponents", SUPPORTED_COMPONENTS, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, supportedComponents, NULL },
	{ "supportedProfiles",   SUPPORTED_PROFILES,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, supportedProfiles,   NULL },
	{ "currentScene",        CURRENT_SCENE,        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentScene,        NULL },
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

	{ "print",                print,                1, 0 },
	{ "println",              println,              1, 0 },

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
	{ 0, 0, 0, 0 }

};

void
jsBrowser::init (JSContext* const cx, JSObject* const global)
{
	const auto proto = JS_InitClass (cx, global, NULL, &static_class, NULL, 0, properties, functions, NULL, NULL);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineObject (cx, global, "Browser", &static_class, NULL, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT);
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

	script -> getBrowser () -> setDescription (JS_GetString (cx, *vp));

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
	if (argc == 1)
	{
		try
		{
			const auto script = getContext (cx) -> getScriptNode ();
			const auto argv   = JS_ARGV (cx, vp);

			if (JSVAL_IS_NULL (argv [0]))
				script -> getBrowser () -> replaceWorld (nullptr);

			else
			{
				JSObject* scene = nullptr;
				
				if (not JS_ConvertArguments (cx, argc, argv, "o", &scene))
					return false;

				if (JS_InstanceOfError (cx, scene, jsX3DScene::getClass ()))
					return false;

				script -> getBrowser () -> replaceWorld (X3DScenePtr (getObject <X3DScene*> (cx, scene)));
			}

			JS_SET_RVAL (cx, vp, JSVAL_VOID);
			return true;
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "Browser .replaceWorld: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::createX3DFromString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		try
		{
			const auto script = getContext (cx) -> getScriptNode ();
			const auto argv   = JS_ARGV (cx, vp);

			JSString* x3dSyntax = nullptr;

			if (not JS_ConvertArguments (cx, argc, argv, "S", &x3dSyntax))
				return false;

			const X3DScenePtr scene = Loader (script -> getExecutionContext (),
			                                  script -> getWorldURL ()) .createX3DFromString (JS_GetString (cx, x3dSyntax));

			return jsX3DScene::create (cx, scene, vp);
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "Browser .createX3DFromString: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::createX3DFromURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc > 0 and argc < 4)
	{
		const auto context = getContext (cx);
		const auto script     = context -> getScriptNode ();

		JSObject* ourl  = nullptr;
		JSObject* onode = nullptr;
		JSString* event = nullptr;

		const auto argv = JS_ARGV (cx, vp);

		if (not JS_ConvertArguments (cx, argc, argv, "o/So", &ourl, &event, &onode))
			return false;

		if (JS_InstanceOfError (cx, ourl, jsMFString::getClass ()))
			return false;

		const auto url = getObject <MFString*> (cx, ourl);

		if (argc > 1)
		{
			if (argc == 3 and JS_GetClass (cx, onode) == jsSFNode::getClass ())
			{
				const auto sfnode = getObject <SFNode*> (cx, onode);

				if (*sfnode)
				{
					try
					{
						const auto field = sfnode -> getValue () -> getField (JS_GetString (cx, event));

						if (field -> isInput ())
						{
							if (field -> getType () == X3DConstants::MFNode)
							{
								try
								{
									const auto scene = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);

									field -> set (scene -> getRootNodes ());
									field -> addEvent ();
								}
								catch (const X3DError & error)
								{
									field -> set (MFNode ());
									field -> addEvent ();

									script -> getBrowser () -> print (error .what ());
								}

								JS_SET_RVAL (cx, vp, JSVAL_VOID);
								return true;
							}
							else
								JS_ReportError (cx, ("Browser .createX3DFromURL: field '" + JS_GetString (cx, event) + "' is not a MFNode") .c_str ());
						}
						else
							JS_ReportError (cx, ("Browser .createX3DFromURL: field '" + JS_GetString (cx, event) + "' is not an eventIn") .c_str ());
					}
					catch (const Error <INVALID_NAME> &)
					{
						JS_ReportError (cx, ("Browser .createX3DFromURL: no such field '" + JS_GetString (cx, event) + "'") .c_str ());
					}
				}
				else
					JS_ReportError (cx, "Browser .createX3DFromURL: node is NULL");

				return false;
			}
			else
			{
				try
				{
					const auto scene = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);

					if (argc == 2)
						onode = context -> getGlobal ();

					jsval argv [2];

					jsX3DScene::create (cx, scene, &argv [0]);
					JS_NewNumberValue (cx, script -> getCurrentTime (), &argv [1]);

					jsval rval;

					if (not JS_CallFunctionName (cx, onode, JS_GetString (cx, event) .c_str (), 2, argv, &rval))
					{
						JS_ReportError (cx, "Browser .createX3DFromURL: Couldn't call function '%s'.", JS_GetString (cx, event) .c_str ());
						return false;
					}

					JS_SET_RVAL (cx, vp, JSVAL_VOID);
					return true;
				}
				catch (const X3DError & error)
				{
					JS_ReportError (cx, error .what ());
					return false;
				}
			}
		}
		else
		{
			try
			{
				const auto scene = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromURL (*url);

				return jsX3DScene::create (cx, scene, vp);
			}
			catch (const X3DError & error)
			{
				JS_ReportError (cx, error .what ());
				return false;
			}
		}
	}

	JS_ReportError (cx, "Browser .createX3DFromURL: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::loadURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2)
	{
		try
		{
			const auto script = getContext (cx) -> getScriptNode ();
			const auto argv   = JS_ARGV (cx, vp);

			JSObject* ourl       = nullptr;
			JSObject* oparameter = nullptr;

			if (not JS_ConvertArguments (cx, argc, argv, "oo", &ourl, &oparameter))
				return false;

			if (JS_InstanceOfError (cx, ourl, jsMFString::getClass ()))
				return false;

			if (JS_InstanceOfError (cx, oparameter, jsMFString::getClass ()))
				return false;
				
			const auto url       = getObject <MFString*> (cx, ourl);
			const auto parameter = getObject <MFString*> (cx, oparameter);

			Loader (script -> getExecutionContext (), script -> getWorldURL ()) .loadURL (*url, *parameter);

			JS_SET_RVAL (cx, vp, JSVAL_VOID);
			return true;
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "Browser .loadURL: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::getRenderingProperty (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* name = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
			return false;

		try
		{
			const auto field = script -> getBrowser () -> getRenderingProperties () -> getField (JS_GetString (cx, name));

			return getValue (cx, field, vp);
		}
		catch (const Error <INVALID_NAME> &)
		{
			JS_ReportError (cx, "Browser .getRenderingProperty: unknown property '%s'.", JS_GetString (cx, name) .c_str ());
		}
	}
	else
		JS_ReportError (cx, "Browser .getRenderingProperty: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::getBrowserProperty (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* name = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
			return false;

		try
		{
			const auto field = script -> getBrowser () -> getBrowserProperties () -> getField (JS_GetString (cx, name));

			return getValue (cx, field, vp);
		}
		catch (const Error <INVALID_NAME> &)
		{
			JS_ReportError (cx, "Browser .getBrowserProperty: unknown property '%s'.", JS_GetString (cx, name) .c_str ());
		}
	}
	else
		JS_ReportError (cx, "Browser .getBrowserProperty: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::getBrowserOption (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* name = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
			return false;

		try
		{
			const auto field = script -> getBrowser () -> getBrowserOptions () -> getField (JS_GetString (cx, name));

			return getValue (cx, field, vp);
		}
		catch (const Error <INVALID_NAME> &)
		{
			JS_ReportError (cx, "Browser .getBrowserOption: unknown option '%s'.", JS_GetString (cx, name) .c_str ());
		}
	}
	else
		JS_ReportError (cx, "Browser .getBrowserOption: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::setBrowserOption (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 2)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* name = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &name))
			return false;

		try
		{
			const auto field = script -> getBrowser () -> getBrowserOptions () -> getField (JS_GetString (cx, name));

			if (setValue (cx, field, &argv [1]))
			{
				JS_SET_RVAL (cx, vp, JSVAL_VOID);

				return true;
			}
			else
				JS_ReportError (cx, "Browser .setBrowserOption: couldn't set value for option '%s'.", JS_GetString (cx, name) .c_str ());
		}
		catch (const Error <INVALID_NAME> &)
		{
			JS_ReportError (cx, "Browser .setBrowserOption: unknown option '%s'.", JS_GetString (cx, name) .c_str ());
		}
	}
	else
		JS_ReportError (cx, "Browser .setBrowserOption: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::firstViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		script -> getBrowser () -> firstViewpoint ();

		return true;
	}

	JS_ReportError (cx, "Browser .firstViewpoint: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::previousViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		script -> getBrowser () -> previousViewpoint ();

		return true;
	}

	JS_ReportError (cx, "Browser .previousViewpoint: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::nextViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		script -> getBrowser () -> nextViewpoint ();

		return true;
	}

	JS_ReportError (cx, "Browser .nextViewpoint: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::lastViewpoint (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		script -> getBrowser () -> lastViewpoint ();

		return true;
	}

	JS_ReportError (cx, "Browser .lastViewpoint: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::print (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* object = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &object))
			return false;

		script -> getBrowser () -> print (JS_GetString (cx, object));

		return true;
	}

	JS_ReportError (cx, "Browser .print: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::println (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* object = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &object))
			return false;

		script -> getBrowser () -> println (JS_GetString (cx, object));

		return true;
	}

	JS_ReportError (cx, "Browser .println: wrong number of arguments");

	return false;
}

// VRML97 functions

JSBool
jsBrowser::getName (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		return JS_NewStringValue (cx, script -> getBrowser () -> getName (), &JS_RVAL (cx, vp));
	}

	JS_ReportError (cx, "Browser .getName: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::getVersion (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		return JS_NewStringValue (cx, script -> getBrowser () -> getVersion (), &JS_RVAL (cx, vp));
	}

	JS_ReportError (cx, "Browser .getVersion: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::getCurrentSpeed (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		return JS_NewNumberValue (cx, script -> getBrowser () -> getCurrentSpeed (), &JS_RVAL (cx, vp));
	}

	JS_ReportError (cx, "Browser .getCurrentSpeed: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::getCurrentFrameRate (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		return JS_NewNumberValue (cx, script -> getBrowser () -> getCurrentFrameRate (), &JS_RVAL (cx, vp));
	}

	JS_ReportError (cx, "Browser .getCurrentFrameRate: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::getWorldURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 0)
	{
		const auto script = getContext (cx) -> getScriptNode ();

		return JS_NewStringValue (cx, script -> getBrowser () -> getWorldURL (), &JS_RVAL (cx, vp));
	}

	JS_ReportError (cx, "Browser .getWorldURL: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::createVrmlFromString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* vrmlSyntax = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &vrmlSyntax))
			return false;

		try
		{
			const auto scene = Loader (script -> getExecutionContext (),
			                           script -> getWorldURL ()) .createX3DFromString (JS_GetString (cx, vrmlSyntax));

			return jsMFNode::create (cx, new MFNode (scene -> getRootNodes ()), &JS_RVAL (cx, vp));
		}
		catch (const X3DError & error)
		{
			script -> getBrowser () -> print (error .what ());

			return jsMFNode::create (cx, new MFNode (), &JS_RVAL (cx, vp));
		}
	}

	JS_ReportError (cx, "Browser .createVrmlFromString: wrong number of arguments");

	return false;
}

//createVrmlFromURL(MFString url, Node node, String event)
JSBool
jsBrowser::createVrmlFromURL (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 3)
	{
		const auto context = getContext (cx);
		const auto script     = context -> getScriptNode ();
		const auto argv       = JS_ARGV (cx, vp);

		JSObject* ourl  = nullptr;
		JSObject* onode = nullptr;
		JSString* event = nullptr;

		if (not JS_ConvertArguments (cx, argc, argv, "ooS", &ourl, &onode, &event))
			return false;

		if (JS_InstanceOfError (cx, ourl, jsMFString::getClass ()))
			return false;

		if (JS_InstanceOfError (cx, onode, jsSFNode::getClass ()))
			return false;

		const auto url    = getObject <MFString*> (cx, ourl);
		const auto sfnode = getObject <SFNode*> (cx, onode);

		if (*sfnode)
		{
			try
			{
				const auto field = sfnode -> getValue () -> getField (JS_GetString (cx, event));

				if (field -> isInput ())
				{
					if (field -> getType () == X3DConstants::MFNode)
					{
						if (context -> getFuture ())
							context -> getFuture () -> wait ();

						using namespace std::placeholders;

						context -> getFuture () .reset (new SceneLoader (script -> getExecutionContext (),
						                                                    *url,
						                                                    std::bind (&jsBrowser::setSceneAsync,
						                                                               SFNode (script),
						                                                               *sfnode,
						                                                               static_cast <MFNode*> (field),
						                                                               _1)));

						JS_SET_RVAL (cx, vp, JSVAL_VOID);
						return true;
					}
					else
						JS_ReportError (cx, ("Browser .createVrmlFromURL: field '" + JS_GetString (cx, event) + "' is not a MFNode") .c_str ());
				}
				else
					JS_ReportError (cx, ("Browser .createVrmlFromURL: field '" + JS_GetString (cx, event) + "' is not an eventIn") .c_str ());
			}
			catch (const Error <INVALID_NAME> &)
			{
				JS_ReportError (cx, ("Browser .createVrmlFromURL: no such field '" + JS_GetString (cx, event) + "'") .c_str ());
			}
		}
		else
			JS_ReportError (cx, "Browser .createVrmlFromURL: node is NULL");
	}
	else
		JS_ReportError (cx, "Browser .createVrmlFromURL: wrong number of arguments");

	return false;
}

void
jsBrowser::setSceneAsync (const SFNode & script, const SFNode & node, MFNode* const field, X3DScenePtr && scene)
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
	if (jsX3DExecutionContext::addRoute (cx, argc, vp))
	{
		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}

	return false;
}

JSBool
jsBrowser::deleteRoute (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 4)
	{
		try
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

			script -> getExecutionContext () -> deleteRoute (fromNode, JS_GetString (cx, fromEventOut),
			                                                 toNode,   JS_GetString (cx, toEventIn));

			JS_SET_RVAL (cx, vp, JSVAL_VOID);
			return true;
		}
		catch (const X3DError & error)
		{
			JS_ReportError (cx, error .what ());
			return false;
		}
	}

	JS_ReportError (cx, "Browser .deleteRoute: wrong number of arguments");

	return false;
}

JSBool
jsBrowser::setDescription (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc == 1)
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto argv   = JS_ARGV (cx, vp);

		JSString* description;

		if (not JS_ConvertArguments (cx, argc, argv, "S", &description))
			return false;

		script -> getBrowser () -> setDescription (JS_GetString (cx, description));

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}

	JS_ReportError (cx, "Browser .setDescription: wrong number of arguments");

	return false;
}

} // MozillaSpiderMonkey
} // X3D
} // titania

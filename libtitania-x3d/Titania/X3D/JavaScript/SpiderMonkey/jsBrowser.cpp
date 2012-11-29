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
 ******************************************************************************/

#include "jsBrowser.h"

#include "../../Browser/X3DBrowser.h"
#include "../../InputOutput/Generator.h"
#include "Fields/jsMFNode.h"
#include "Fields/jsMFString.h"
#include "Fields/jsSFNode.h"
#include "jsComponentInfoArray.h"
#include "jsProfileInfoArray.h"
#include "jsX3DExecutionContext.h"
#include "jsX3DScene.h"
#include "String.h"

namespace titania {
namespace X3D {

JSClass jsBrowser::static_class = {
	"Browser", 0,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsBrowser::properties [ ] = {
	{ "name",                NAME,                JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, name,                NULL },
	{ "version",             VERSION,             JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, version,             NULL },
	{ "currentSpeed",        CURRENTSPEED,        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentSpeed,        NULL },
	{ "currentFrameRate",    CURRENTFRAMERATE,    JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentFrameRate,    NULL },
	{ "description",         DESCRIPTION,         JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT,                   description,         description },
	{ "supportedComponents", SUPPORTEDCOMPONENTS, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, supportedComponents, NULL },
	{ "supportedProfiles",   SUPPORTEDPROFILES,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, supportedProfiles,   NULL },
	{ "currentScene",        CURRENTSCENE,        JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, currentScene,        NULL },
	{ 0 }

};

JSFunctionSpec jsBrowser::functions [ ] = {
	{ "getName",              getName,              0, 0 }, // VRML97
	{ "getVersion",           getVersion,           0, 0 }, // VRML97
	{ "getCurrentSpeed",      getCurrentSpeed,      0, 0 }, // VRML97
	{ "getCurrentFrameRate",  getCurrentFrameRate,  0, 0 }, // VRML97
	{ "getWorldURL",          getWorldURL,          0, 0 }, // VRML97
	{ "setDescription",       setDescription,       1, 0 }, // VRML97
	{ "replaceWorld",         replaceWorld,         1, 0 }, // VRML97
	{ "createVrmlFromString", createVrmlFromString, 1, 0 }, // VRML97
	{ "createVrmlFromURL",    createVrmlFromURL,    3, 0 }, // VRML97
	{ "addRoute",             addRoute,             4, 0 }, // VRML97
	{ "deleteRoute",          deleteRoute,          4, 0 }, // VRML97
	{ "loadURL",              loadURL,              2, 0 }, // VRML97
	{ 0, 0, 0, 0 }

};

void
jsBrowser::defineObject (JSContext* context, JSObject* global)
{
	JSObject* obj = JS_DefineObject (context, global, "Browser", &static_class, NULL,
	                                 JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT);

	JS_DefineProperties (context, obj, properties);
	JS_DefineFunctions (context, obj, functions);
}

// X3D properties

JSBool
jsBrowser::name (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	return JS_NewStringValue (context, node -> getBrowser () -> getName (), vp);
}

JSBool
jsBrowser::version (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	return JS_NewStringValue (context, node -> getBrowser () -> getVersion (), vp);
}

JSBool
jsBrowser::currentSpeed (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	return JS_NewNumberValue (context, node -> getBrowser () -> getCurrentSpeed (), vp);
}

JSBool
jsBrowser::currentFrameRate (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	return JS_NewNumberValue (context, node -> getBrowser () -> getCurrentFrameRate (), vp);
}

JSBool
jsBrowser::description (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	return JS_NewStringValue (context, node -> getBrowser () -> getDescription (), vp);
}

JSBool
jsBrowser::description (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	node -> getBrowser () -> setDescription (JS_GetString (context, *vp));
	return JS_TRUE;
}

JSBool
jsBrowser::supportedComponents (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	//node -> getBrowser() -> getSupportedComponents()
	return jsComponentInfoArray::create (context, NULL, vp);
}

JSBool
jsBrowser::supportedProfiles (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	//node -> getBrowser() -> getSupportedProfiles();
	return jsProfileInfoArray::create (context, NULL, vp);
}

JSBool
jsBrowser::currentScene (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	Scene* scene = dynamic_cast <Scene*> (node -> getExecutionContext ());

	if (scene)
		return jsX3DScene::create (context, scene, vp);

	X3DExecutionContext* executionContext = dynamic_cast <X3DExecutionContext*> (node -> getExecutionContext ());

	if (executionContext)
		return jsX3DExecutionContext::create (context, executionContext, vp);

	return JS_FALSE;
}

// VRML97 functions

JSBool
jsBrowser::getName (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);
		return JS_NewStringValue (context, node -> getBrowser () -> getName (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::getVersion (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);
		return JS_NewStringValue (context, node -> getBrowser () -> getVersion (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::getCurrentSpeed (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);
		return JS_NewNumberValue (context, node -> getBrowser () -> getCurrentSpeed (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::getCurrentFrameRate (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);
		return JS_NewNumberValue (context, node -> getBrowser () -> getCurrentFrameRate (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::getWorldURL (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);
		return JS_NewStringValue (context, node -> getBrowser () -> getWorldURL (), &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::replaceWorld (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		//X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::createVrmlFromString (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		JSString* vrmlSyntax;

		jsval* argv = JS_ARGV (context, vp);
		
		if (not JS_ConvertArguments (context, argc, argv, "S", &vrmlSyntax))
			return JS_FALSE;

		SFNode <Scene>          scene;
		MFNode <X3DBasicNode>*  mfnode = new MFNode <X3DBasicNode> ();

		try
		{
			scene = node -> getBrowser () -> createX3DFromString (JS_GetString (context, vrmlSyntax));
		}
		catch (const X3DError & error)
		{
			std::cerr << "Warning Browser: " << std::endl << error .what () << std::endl;
		}

		if (scene)
			*mfnode = scene -> getRootNodes ();

		return jsMFNode::create (context, mfnode, &JS_RVAL (context, vp));
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

//createVrmlFromURL(MFString url, Node node, String event)
JSBool
jsBrowser::createVrmlFromURL (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 3)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		JSObject* ourl;
		JSObject* onode;
		JSString* event;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "ooS", &ourl, &onode, &event))
			return JS_FALSE;

		if (JS_GetClass (context, ourl) not_eq jsMFString::getClass ())
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be MFString, is %s", JS_GetClass (context, ourl) -> name);
			return JS_FALSE;
		}

		MFString* url = (MFString*) JS_GetPrivate (context, ourl);

		if (JS_GetClass (context, onode) not_eq jsSFNode::getClass ())
		{
			JS_ReportError (context, "Type of argument 2 is invalid - should be SFNode, is %s", JS_GetClass (context, onode) -> name);
			return JS_FALSE;
		}

		X3DField <X3DBasicNode*>* sfnode = (X3DField <X3DBasicNode*>*) JS_GetPrivate (context, onode);

		if (*sfnode)
		{
			X3DFieldDefinition* field = sfnode -> getValue () -> getField (JS_GetString (context, event));

			if (field)
			{
				if (field -> isInput ())
				{
					if (field -> getType () == X3DConstants::MFNode)
					{
						SFNode <Scene> scene = node -> getBrowser () -> createX3DFromURL (*url);

						if (scene)
							field -> write (scene -> getRootNodes ());

						//std::cout << "createVrmlFromURL " << *url << std::endl;
						JS_SET_RVAL (context, vp, JSVAL_VOID);

						return JS_TRUE;
					}
					else
						JS_ReportError (context, (std::string ("Browser .createVrmlFromURL: field '") + JS_GetString (context, event) + "' is not a MFNode") .c_str ());
				}
				else
					JS_ReportError (context, (std::string ("Browser .createVrmlFromURL: field '") + JS_GetString (context, event) + "' is not an eventIn") .c_str ());
			}
			else
				JS_ReportError (context, (std::string ("Browser .createVrmlFromURL: no such field '") + JS_GetString (context, event) + "'") .c_str ());
		}
		else
			JS_ReportError (context, "Browser .createVrmlFromURL: node is null");
	}
	else
		JS_ReportError (context, "Browser .createVrmlFromURL: wrong number of arguments");

	return JS_FALSE;
}

//void addRoute(SFNode fromNode, String fromEventOut, SFNode toNode, String toEventIn)
JSBool
jsBrowser::addRoute (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 4)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		JSObject* ofromNode;
		JSObject* otoNode;
		JSString* fromEventOut;
		JSString* toEventIn;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "oSoS", &ofromNode, &fromEventOut, &otoNode, &toEventIn))
			return JS_FALSE;

		if (JS_GetClass (context, ofromNode) not_eq jsSFNode::getClass ())
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFNode, is %s", JS_GetClass (context, ofromNode) -> name);
			return JS_FALSE;
		}

		X3DField <X3DBasicNode*>* fromNode = (X3DField <X3DBasicNode*>*) JS_GetPrivate (context, ofromNode);

		if (JS_GetClass (context, otoNode) not_eq jsSFNode::getClass ())
		{
			JS_ReportError (context, "Type of argument 3 is invalid - should be SFNode, is %s", JS_GetClass (context, otoNode) -> name);
			return JS_FALSE;
		}

		X3DField <X3DBasicNode*>* toNode = (X3DField <X3DBasicNode*>*) JS_GetPrivate (context, otoNode);

		try
		{
			node -> getExecutionContext () -> addRoute (fromNode -> getValue (), JS_GetString (context, fromEventOut), 
			                                            toNode -> getValue (),   JS_GetString (context, toEventIn));
			
			JS_SET_RVAL (context, vp, JSVAL_VOID);
			
			return JS_TRUE;
		}
		catch (const X3DError & error)
		{
			JS_ReportError (context, error .what ());
		}
	}
	else
		JS_ReportError (context, "Browser .addRoute: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::deleteRoute (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 4)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		JSObject* ofromNode;
		JSObject* otoNode;
		JSString* fromEventOut;
		JSString* toEventIn;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "oSoS", &ofromNode, &fromEventOut, &otoNode, &toEventIn))
			return JS_FALSE;

		if (JS_GetClass (context, ofromNode) not_eq jsSFNode::getClass ())
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be SFNode, is %s", JS_GetClass (context, ofromNode) -> name);
			return JS_FALSE;
		}

		X3DField <X3DBasicNode*>* fromNode = (X3DField <X3DBasicNode*>*) JS_GetPrivate (context, ofromNode);

		if (JS_GetClass (context, otoNode) not_eq jsSFNode::getClass ())
		{
			JS_ReportError (context, "Type of argument 3 is invalid - should be SFNode, is %s", JS_GetClass (context, otoNode) -> name);
			return JS_FALSE;
		}

		X3DField <X3DBasicNode*>* toNode = (X3DField <X3DBasicNode*>*) JS_GetPrivate (context, otoNode);

		try
		{
			node -> getExecutionContext () -> deleteRoute (fromNode -> getValue (), JS_GetString (context, fromEventOut),
			                                               toNode -> getValue (),   JS_GetString (context, toEventIn));
			
			JS_SET_RVAL (context, vp, JSVAL_VOID);
			
			return JS_TRUE;
		}
		catch (const X3DError & error)
		{
			JS_ReportError (context, error .what ());
		}
	}
	else
		JS_ReportError (context, "Browser .deleteRoute: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::loadURL (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 2)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		JSObject* ourl;
		JSObject* oparameter;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "oo", &ourl, &oparameter))
			return JS_FALSE;

		if (JS_GetClass (context, ourl) not_eq jsMFString::getClass ())
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be MFString, is %s", JS_GetClass (context, ourl) -> name);
			return JS_FALSE;
		}

		MFString* url = (MFString*) JS_GetPrivate (context, ourl);

		if (JS_GetClass (context, oparameter) not_eq jsMFString::getClass ())
		{
			JS_ReportError (context, "Type of argument 1 is invalid - should be MFString, is %s", JS_GetClass (context, oparameter) -> name);
			return JS_FALSE;
		}

		MFString* parameter = (MFString*) JS_GetPrivate (context, oparameter);

		node -> getBrowser () -> loadURL (*url, *parameter);

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}
	else
		JS_ReportError (context, "Browser .loadURL: wrong number of arguments");

	return JS_FALSE;
}

JSBool
jsBrowser::setDescription (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		JSString* description;

		jsval* argv = JS_ARGV (context, vp);

		if (not JS_ConvertArguments (context, argc, argv, "S", &description))
			return JS_FALSE;

		node -> getBrowser () -> setDescription (JS_GetString (context, description));

		JS_SET_RVAL (context, vp, JSVAL_VOID);

		return JS_TRUE;
	}
	else
		JS_ReportError (context, "Browser .setDescription: wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // titania

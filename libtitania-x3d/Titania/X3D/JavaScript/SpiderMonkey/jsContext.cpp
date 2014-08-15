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

#include "jsContext.h"

#include "../../Browser/X3DBrowser.h"
#include "../../InputOutput/Loader.h"
#include "../../Thread/SceneLoader.h"

#include "jsGlobals.h"
#include "jsX3DConstants.h"

#include "jsBrowser.h"
#include "jsX3DExecutionContext.h"
#include "jsX3DScene.h"

#include "jsComponentInfo.h"
#include "jsProfileInfo.h"
#include "jsX3DExternProtoDeclaration.h"
#include "jsX3DFieldDefinition.h"
#include "jsX3DProtoDeclaration.h"
#include "jsX3DRoute.h"

#include "jsComponentInfoArray.h"
#include "jsExternProtoDeclarationArray.h"
#include "jsFieldDefinitionArray.h"
#include "jsProfileInfoArray.h"
#include "jsProtoDeclarationArray.h"
#include "jsRouteArray.h"

#include "jsFields.h"

#include "jsString.h"
#include "jsfield.h"

#include <cassert>
#include <thread>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

// mutex test: http://www.web3d.org/x3d/content/examples/Basic/StudentProjects/DeadReckoningComparisons.x3d

const std::string jsContext::componentName  = "Browser";
const std::string jsContext::typeName       = "jsContext";
const std::string jsContext::containerField = "context";

JSClass jsContext::GlobalClass = {
	"global",
	JSCLASS_GLOBAL_FLAGS,
	JS_PropertyStub,
	JS_PropertyStub,
	JS_PropertyStub,
	JS_StrictPropertyStub,
	JS_EnumerateStub,
	JS_ResolveStub,
	JS_ConvertStub,
	JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

jsContext::jsContext (Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3DJavaScriptContext (script, ecmascript),
	             runtime (nullptr),
	             context (nullptr),
	         globalClass (GlobalClass),
	              global (nullptr),
	            worldURL ({ uri }),
	        initializeFn (),
	     prepareEventsFn (),
	   eventsProcessedFn (),
	          shutdownFn (),
	              fields (),
	           functions (),
	             objects (),
	               files (),
	              future ()
{
	// Get a JS runtime.
	runtime .reset (JS_NewRuntime (64 * 1024 * 1024)); // 64 MB runtime memory

	if (runtime == nullptr)
		throw std::invalid_argument ("Couldn't not create JavaScript runtime.");

	// Create a context.
	context .reset (JS_NewContext (runtime .get (), 512));

	if (context == nullptr)
		throw std::invalid_argument ("Couldn't not create JavaScript context.");

	JS_SetCStringsAreUTF8 ();
	JS_SetOptions (context .get (), JSOPTION_ATLINE | JSOPTION_VAROBJFIX | JSOPTION_JIT | JSOPTION_METHODJIT);
	JS_SetVersion (context .get (), JSVERSION_LATEST);
	JS_SetErrorReporter (context .get (), error);

	// Create the global object.
	global = JS_NewCompartmentAndGlobalObject (context .get (), &globalClass, nullptr);

	if (global == nullptr)
		throw std::invalid_argument ("Couldn't not create JavaScript global object.");

	setContext ();

	setFields ();
}

X3DBaseNode*
jsContext::create (X3DExecutionContext* const) const
{
	return new jsContext (getScriptNode (), getECMAScript (), worldURL .front ());
}

void
jsContext::setContext ()
{
	// Populate the global object with the standard globals, like Object and Array.
	if (not JS_InitStandardClasses (context .get (), global))
		return;

	JS_SetContextPrivate (context .get (), this);

	jsBrowser::init (context .get (), global);
	jsX3DConstants::init (context .get (), global);

	JSObject* executionContext = jsX3DExecutionContext::init (context .get (), global);
	jsX3DScene::init (context .get (), global, executionContext);

	jsComponentInfo::init             (context .get (), global);
	jsProfileInfo::init               (context .get (), global);
	jsX3DFieldDefinition::init        (context .get (), global);
	jsX3DExternProtoDeclaration::init (context .get (), global);
	jsX3DProtoDeclaration::init       (context .get (), global);
	jsX3DRoute::init                  (context .get (), global);

	jsFieldDefinitionArray::init        (context .get (), global);
	jsComponentInfoArray::init          (context .get (), global);
	jsProfileInfoArray::init            (context .get (), global);
	jsExternProtoDeclarationArray::init (context .get (), global);
	jsProtoDeclarationArray::init       (context .get (), global);
	jsRouteArray::init                  (context .get (), global);

	jsSFColor::init     (context .get (), global);
	jsSFColorRGBA::init (context .get (), global);
	jsSFImage::init     (context .get (), global);
	jsSFMatrix3d::init  (context .get (), global);
	jsSFMatrix3f::init  (context .get (), global);
	jsSFMatrix4d::init  (context .get (), global);
	jsSFMatrix4f::init  (context .get (), global);
	jsSFNode::init      (context .get (), global);
	jsSFRotation::init  (context .get (), global);
	jsSFVec2d::init     (context .get (), global);
	jsSFVec2f::init     (context .get (), global);
	jsSFVec3d::init     (context .get (), global);
	jsSFVec3f::init     (context .get (), global);
	jsSFVec4d::init     (context .get (), global);
	jsSFVec4f::init     (context .get (), global);
	jsVrmlMatrix::init  (context .get (), global);

	jsMFBool::init      (context .get (), global);
	jsMFColor::init     (context .get (), global);
	jsMFColorRGBA::init (context .get (), global);
	jsMFDouble::init    (context .get (), global);
	jsMFFloat::init     (context .get (), global);
	jsMFImage::init     (context .get (), global);
	jsMFInt32::init     (context .get (), global);
	jsMFMatrix3d::init  (context .get (), global);
	jsMFMatrix3f::init  (context .get (), global);
	jsMFMatrix4d::init  (context .get (), global);
	jsMFMatrix4f::init  (context .get (), global);
	jsMFNode::init      (context .get (), global);
	jsMFRotation::init  (context .get (), global);
	jsMFString::init    (context .get (), global);
	jsMFTime::init      (context .get (), global);
	jsMFVec2d::init     (context .get (), global);
	jsMFVec2f::init     (context .get (), global);
	jsMFVec3d::init     (context .get (), global);
	jsMFVec3f::init     (context .get (), global);
	jsMFVec4d::init     (context .get (), global);
	jsMFVec4f::init     (context .get (), global);

	jsGlobals::init (context .get (), global);
}

void
jsContext::setFields ()
{
	for (auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		switch (field -> getAccessType ())
		{
			case initializeOnly :
			case outputOnly     :
				{
					addUserDefinedField (field);
					defineProperty (context .get (), global, field, field -> getName (), JSPROP_ENUMERATE);
					break;
				}
			case inputOnly:
				break;
			case inputOutput:
			{
				addUserDefinedField (field);
				defineProperty (context .get (), global, field, field -> getName (),              JSPROP_ENUMERATE);
				defineProperty (context .get (), global, field, field -> getName () + "_changed", 0);
				break;
			}
		}
	}
}

void
jsContext::addUserDefinedField (X3DFieldDefinition* const field)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
		case X3DConstants::SFDouble:
		case X3DConstants::SFFloat:
		case X3DConstants::SFInt32:
		case X3DConstants::SFString:
		case X3DConstants::SFTime:
			break;
		default:
		{
			jsval vp;

			if (JS_NewFieldValue (context .get (), field, &vp))
			{
				fields [field -> getName ()] = vp;
				JS_AddValueRoot (context .get (), &fields [field -> getName ()]);
			}

			break;
		}
	}
}

void
jsContext::defineProperty (JSContext* const context,
                           JSObject* const obj,
                           X3DFieldDefinition* const field,
                           const std::string & name,
                           const uintN attrs)
{
	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
		case X3DConstants::SFDouble:
		case X3DConstants::SFFloat:
		case X3DConstants::SFInt32:
		case X3DConstants::SFString:
		case X3DConstants::SFTime:
		{
			JS_DefineProperty (context,
			                   obj,
			                   name .c_str (),
			                   JSVAL_VOID,
			                   getBuildInProperty,
			                   setProperty,
			                   JSPROP_PERMANENT | JSPROP_SHARED | attrs);
			break;
		}
		default:
		{
			JS_DefineProperty (context,
			                   obj,
			                   name .c_str (),
			                   JSVAL_VOID,
			                   getProperty,
			                   setProperty,
			                   JSPROP_PERMANENT | JSPROP_SHARED | attrs);
			break;
		}
	}
}

JSBool
jsContext::getBuildInProperty (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	jsval name;

	if (JS_IdToValue (context, id, &name))
	{
		jsContext* const          javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));
		Script* const             script     = javaScript -> getScriptNode ();
		X3DFieldDefinition* const field      = script -> getField (JS_GetString (context, name));

		return JS_NewFieldValue (context, field, vp);
	}

	return JS_TRUE;
}

JSBool
jsContext::getProperty (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	jsval name;

	if (JS_IdToValue (context, id, &name))
	{
		jsContext* const javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

		*vp = javaScript -> fields [JS_GetString (context, name)];
		return JS_TRUE;
	}

	return JS_TRUE;
}

JSBool
jsContext::setProperty (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	jsval name;

	if (JS_IdToValue (context, id, &name))
	{
		Script* const script = static_cast <jsContext*> (JS_GetContextPrivate (context)) -> getScriptNode ();

		X3DFieldDefinition* const field = script -> getField (JS_GetString (context, name));

		return JS_ValueToField (context, field, vp);
	}

	return JS_TRUE;
}

JSBool
jsContext::require (const basic::uri & uri, jsval & rval)
{
	try
	{
		// Resolve uri

		const basic::uri resolvedURL = worldURL .back () .transform (uri);

		// Get cached result

		const auto file = files .find (resolvedURL);

		if (file not_eq files .end ())
		{
			rval = file -> second;
			return JS_TRUE;
		}

		// Load document

		const std::string document = Loader (getExecutionContext ()) .loadDocument (resolvedURL);

		// Evaluate script

		worldURL .emplace_back (resolvedURL);

		const auto success = evaluate (document, resolvedURL, rval);

		worldURL .pop_back ();

		// Cache result

		if (success)
		{
			files .emplace (resolvedURL, rval);

			JS_AddValueRoot (context .get (), &files [resolvedURL]);

			return JS_TRUE;
		}
	}
	catch (const X3DError &)
	{ }

	return JS_FALSE;
}

JSBool
jsContext::evaluate (const std::string & string, const std::string & filename)
{
	jsval rval;

	return evaluate (string, filename, rval);
}

JSBool
jsContext::evaluate (const std::string & string, const std::string & filename, jsval & rval)
{
	glong   items_read    = 0;
	glong   items_written = 0;
	GError* error         = nullptr;

	gunichar2* utf16_string = g_utf8_to_utf16 (string .c_str (), string .length (), &items_read, &items_written, &error);

	if (error)
	{
		JS_ReportError (context .get (), "jsContext::evaluate: %s: %d: %s.", g_quark_to_string (error -> domain), error -> code, error -> message);
		return JS_FALSE;
	}

	const JSBool retval = JS_EvaluateUCScript (context .get (), global,
	                                           utf16_string, items_written,
	                                           filename .empty () ? nullptr : filename .c_str (),
	                                           1,
	                                           &rval);

	g_free (utf16_string);

	return retval;
}

void
jsContext::initialize ()
{
	X3DJavaScriptContext::initialize ();

	if (not evaluate (getECMAScript (), worldURL .front () == getExecutionContext () -> getWorldURL () ? "" : worldURL .front () .str ()))
		throw std::invalid_argument ("Couldn't evaluate script.");

	setEventHandler ();

	getExecutionContext () -> isLive () .addInterest (this, &jsContext::set_live);
	isLive () .addInterest (this, &jsContext::set_live);

	set_live ();

	if (not JSVAL_IS_VOID (initializeFn))
		callFunction (initializeFn);
}

void
jsContext::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getExecutionContext () -> isLive () .removeInterest (this, &jsContext::set_live);
	executionContext -> isLive () .addInterest (this, &jsContext::set_live);

	X3DJavaScriptContext::setExecutionContext (executionContext);

	set_live ();
}

void
jsContext::setEventHandler ()
{
	initializeFn      = getFunction ("initialize");
	prepareEventsFn   = getFunction ("prepareEvents");
	eventsProcessedFn = getFunction ("eventsProcessed");
	shutdownFn        = getFunction ("shutdown");

	if (not JSVAL_IS_VOID (shutdownFn))
		shutdown () .addInterest (this, &jsContext::set_shutdown);

	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		switch (field -> getAccessType ())
		{
			case inputOnly   :
			case inputOutput :
				{
					const jsval function = field -> getAccessType () == inputOnly
					                       ? getFunction (field -> getName ())
												  : getFunction ("set_" + field -> getName ());

					if (not JSVAL_IS_VOID (function))
						functions [field] = function;

					break;
				}
			default :
				break;
		}
	}
}

void
jsContext::set_live ()
{
	if (getExecutionContext () -> isLive () and isLive ())
	{
		if (not JSVAL_IS_VOID (prepareEventsFn))
			getBrowser () -> prepareEvents () .addInterest (this, &jsContext::prepareEvents);

		if (not JSVAL_IS_VOID (eventsProcessedFn))
			getScriptNode () -> addInterest (this, &jsContext::eventsProcessed);

		getScriptNode () -> addInterest (this, &jsContext::finish);

		for (const auto & field: getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly:
				case inputOutput:
				{
					if (functions .count (field))
						field -> addInterest (this, &jsContext::set_field, field, std::ref (functions [field]));

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		if (not JSVAL_IS_VOID (prepareEventsFn))
			getBrowser () -> prepareEvents () .removeInterest (this, &jsContext::prepareEvents);

		if (not JSVAL_IS_VOID (eventsProcessedFn))
			getScriptNode () -> removeInterest (this, &jsContext::eventsProcessed);

		getScriptNode () -> removeInterest (this, &jsContext::finish);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly   :
				case inputOutput :
					{
						if (functions .count (field))
							field -> removeInterest (this, &jsContext::set_field);

						break;
					}
				default:
					break;
			}
		}
	}
}

void
jsContext::prepareEvents ()
{
	callFunction (prepareEventsFn);
}

void
jsContext::set_field (X3DFieldDefinition* const field, const jsval & function)
{
	field -> isTainted (true);

	jsval argv [2];

	JS_NewFieldValue (context .get (), field, &argv [0]);
	JS_NewNumberValue (context .get (), getCurrentTime (), &argv [1]);

	jsval rval;
	JS_CallFunctionValue (context .get (), global, function, 2, argv, &rval);

	field -> isTainted (false);
}

void
jsContext::eventsProcessed ()
{
	callFunction (eventsProcessedFn);
}

void
jsContext::set_shutdown ()
{
	callFunction (shutdownFn);
}

void
jsContext::finish ()
{
	JS_GC (context .get ());
}

jsval
jsContext::getFunction (const std::string & name) const
{
	jsval     function = JSVAL_VOID;
	JSObject* objp     = nullptr;

	JSBool result = JS_GetMethod (context .get (), global, name .c_str (), &objp, &function);

	if (result)
		return function;

	return JSVAL_VOID;
}

void
jsContext::callFunction (const std::string & name) const
{
	jsval     function = JSVAL_VOID;
	JSObject* objp     = nullptr;

	JSBool result = JS_GetMethod (context .get (), global, name .c_str (), &objp, &function);

	if (not result or JSVAL_IS_VOID (function))
		return;

	callFunction (function);
}

void
jsContext::callFunction (jsval function) const
{
	jsval rval;

	JS_CallFunctionValue (context .get (), global, function, 0, nullptr, &rval);
}

void
jsContext::addObject (X3DFieldDefinition* const field, JSObject* const object)
throw (Error <INVALID_FIELD>)
{
	if (not objects .emplace (field, object) .second)
		throw Error <INVALID_FIELD> ("Object already exists in jsContext.");

	field -> addParent (this);
}

void
jsContext::removeObject (X3DFieldDefinition* const field)
{
	if (objects .erase (field))
		field -> removeParent (this);
}

JSObject*
jsContext::getObject (X3DFieldDefinition* const field)
throw (std::out_of_range)
{
	return objects .at (field);
}

void
jsContext::error (JSContext* context, const char* message, JSErrorReport* report)
{
	const auto javaScript = static_cast <jsContext*> (JS_GetContextPrivate (context));

	javaScript -> X3DJavaScriptContext::error (message,
	                                           report -> filename ? report -> filename : "<inline>",
	                                           report -> lineno,
	                                           report -> tokenptr ? report -> tokenptr - report -> linebuf : -1,
	                                           report -> linebuf ? report -> linebuf : "");
}

void
jsContext::dispose ()
{
	if (future)
		future -> dispose ();

	for (auto & field : fields)
		JS_RemoveValueRoot (context .get (), &field .second);

	for (auto & file : files)
		JS_RemoveValueRoot (context .get (), &file .second);

	// Cleanup.
	context .reset ();
	runtime .reset ();

	assert (objects .empty ());

	X3DJavaScriptContext::dispose ();
}

jsContext::~jsContext ()
{ }

} // MozillaSpiderMonkey
} // X3D
} // titania

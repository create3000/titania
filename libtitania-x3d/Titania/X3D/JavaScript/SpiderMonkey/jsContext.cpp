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
#include "../../Miscellaneous/Random.h"
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

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

// mutex test: http://www.web3d.org/x3d/content/examples/Basic/StudentProjects/DeadReckoningComparisons.x3d

const ComponentType jsContext::component      = ComponentType::TITANIA;
const std::string   jsContext::typeName       = "jsContext";
const std::string   jsContext::containerField = "cx";

JSClass jsContext::globalClass = {
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
	                  rt (nullptr),
	                  cx (nullptr),
	              global (nullptr),
	            worldURL ({ uri }),
	        initializeFn (),
	     prepareEventsFn (),
	   eventsProcessedFn (),
	          shutdownFn (),
	          	  protos (size_t (ObjectType::SIZE)),
	              fields (),
	           functions (),
	             objects (),
	               files (),
	              future (),
	               frame (0)
{
	rt = JS_NewRuntime (64 * 1024 * 1024); // 64 MB runtime memory

	if (rt == nullptr)
		throw std::runtime_error ("Couldn't create JavaScript runtime.");

	cx = JS_NewContext (rt, 8192);

	if (cx == nullptr)
		throw std::runtime_error ("Couldn't create JavaScript context.");

	global = JS_NewCompartmentAndGlobalObject (cx, &globalClass, nullptr);

	if (global == nullptr)
		throw std::runtime_error ("Couldn't create JavaScript global object.");

	JS_SetCStringsAreUTF8 ();
	JS_SetVersion (cx, JSVERSION_LATEST);
	JS_SetOptions (cx, JSOPTION_ATLINE | JSOPTION_VAROBJFIX | JSOPTION_JIT | JSOPTION_METHODJIT);
	JS_SetErrorReporter (cx, error);

	addClasses ();
	addUserDefinedFields ();
}

X3DBaseNode*
jsContext::create (X3DExecutionContext* const) const
{
	return new jsContext (getScriptNode (), getECMAScript (), worldURL .front ());
}

void
jsContext::addClasses ()
throw (std::runtime_error)
{
	// Populate the global object with the standard globals, like Object and Array.
	if (not JS_InitStandardClasses (cx, global))
		throw std::runtime_error ("Couldn't create JavaScript standard classes.");

	JS_SetContextPrivate (cx, this);

	jsGlobals::init (cx, global);

	addProto (jsX3DConstants::getId (), jsX3DConstants::init (cx, global, nullptr));
	addProto (jsBrowser::getId (),      jsBrowser::init      (cx, global, nullptr));

	addProto (jsX3DExecutionContext::getId (), jsX3DExecutionContext::init (cx, global, nullptr));
	addProto (jsX3DScene::getId (),            jsX3DScene::init            (cx, global, getProto (jsX3DExecutionContext::getId ())));

	addProto (jsComponentInfo::getId (),             jsComponentInfo::init             (cx, global, nullptr));
	addProto (jsProfileInfo::getId (),               jsProfileInfo::init               (cx, global, nullptr));
	addProto (jsX3DFieldDefinition::getId (),        jsX3DFieldDefinition::init        (cx, global, nullptr));
	addProto (jsX3DExternProtoDeclaration::getId (), jsX3DExternProtoDeclaration::init (cx, global, nullptr));
	addProto (jsX3DProtoDeclaration::getId (),       jsX3DProtoDeclaration::init       (cx, global, nullptr));
	addProto (jsX3DRoute::getId (),                  jsX3DRoute::init                  (cx, global, nullptr));

	addProto (jsFieldDefinitionArray::getId (),        jsFieldDefinitionArray::init        (cx, global, nullptr));
	addProto (jsComponentInfoArray::getId (),          jsComponentInfoArray::init          (cx, global, nullptr));
	addProto (jsProfileInfoArray::getId (),            jsProfileInfoArray::init            (cx, global, nullptr));
	addProto (jsExternProtoDeclarationArray::getId (), jsExternProtoDeclarationArray::init (cx, global, nullptr));
	addProto (jsProtoDeclarationArray::getId (),       jsProtoDeclarationArray::init       (cx, global, nullptr));
	addProto (jsRouteArray::getId (),                  jsRouteArray::init                  (cx, global, nullptr));

	addProto (jsX3DField::getId (),    jsX3DField::init    (cx, global, nullptr));

	addProto (jsSFColor::getId (),     jsSFColor::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFColorRGBA::getId (), jsSFColorRGBA::init (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFImage::getId (),     jsSFImage::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix3d::getId (),  jsSFMatrix3d::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix3f::getId (),  jsSFMatrix3f::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix4d::getId (),  jsSFMatrix4d::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix4f::getId (),  jsSFMatrix4f::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFNode::getId (),      jsSFNode::init      (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFRotation::getId (),  jsSFRotation::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec2d::getId (),     jsSFVec2d::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec2f::getId (),     jsSFVec2f::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec3d::getId (),     jsSFVec3d::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec3f::getId (),     jsSFVec3f::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec4d::getId (),     jsSFVec4d::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec4f::getId (),     jsSFVec4f::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsVrmlMatrix::getId (),  jsVrmlMatrix::init  (cx, global, getProto (jsX3DField::getId ())));

	addProto (jsMFBool::getId (),      jsMFBool::init      (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFColor::getId (),     jsMFColor::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFColorRGBA::getId (), jsMFColorRGBA::init (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFDouble::getId (),    jsMFDouble::init    (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFFloat::getId (),     jsMFFloat::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFImage::getId (),     jsMFImage::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFInt32::getId (),     jsMFInt32::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix3d::getId (),  jsMFMatrix3d::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix3f::getId (),  jsMFMatrix3f::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix4d::getId (),  jsMFMatrix4d::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix4f::getId (),  jsMFMatrix4f::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFNode::getId (),      jsMFNode::init      (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFRotation::getId (),  jsMFRotation::init  (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFString::getId (),    jsMFString::init    (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFTime::getId (),      jsMFTime::init      (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec2d::getId (),     jsMFVec2d::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec2f::getId (),     jsMFVec2f::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec3d::getId (),     jsMFVec3d::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec3f::getId (),     jsMFVec3f::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec4d::getId (),     jsMFVec4d::init     (cx, global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec4f::getId (),     jsMFVec4f::init     (cx, global, getProto (jsX3DField::getId ())));
}

void
jsContext::addUserDefinedFields ()
{
	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		addUserDefinedField (field);

		switch (field -> getAccessType ())
		{
			case initializeOnly:
			case outputOnly:
			{
				defineProperty (global, field, field -> getName (), JSPROP_ENUMERATE);
				break;
			}
			case inputOnly:
				break;
			case inputOutput:
			{
				defineProperty (global, field, field -> getName (),              JSPROP_ENUMERATE);
				defineProperty (global, field, field -> getName () + "_changed", 0);
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

			if (getValue (cx, field, &vp))
			{
				auto & value = fields .emplace (field -> getName (), vp) .first -> second;

				JS_AddValueRoot (cx, &value);
			}

			break;
		}
	}
}

void
jsContext::defineProperty (JSObject* const obj,
                           X3DFieldDefinition* const field,
                           const std::string & name,
                           const uint32_t attrs)
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
			JS_DefineProperty (cx,
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
			JS_DefineProperty (cx,
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
jsContext::setProperty (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto & script = getContext (cx) -> getScriptNode ();
		const auto   field  = script -> getField (to_string (cx, id));

		return setValue (cx, field, vp);
	}
	catch (std::exception &)
	{
		return ThrowException (cx, "user-defined field not found");
	}
}

JSBool
jsContext::getBuildInProperty (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto & script = getContext (cx) -> getScriptNode ();
		const auto   field  = script -> getField (to_string (cx, id));

		return getValue (cx, field, vp);
	}
	catch (std::exception &)
	{
		return ThrowException (cx, "user-defined field not found");
	}
}

JSBool
jsContext::getProperty (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		*vp = getContext (cx) -> fields .at (to_string (cx, id));
		return true;
	}
	catch (std::exception &)
	{
		return ThrowException (cx, "user-defined field not found");
	}
}

void
jsContext::addObject (X3D::X3DFieldDefinition* const field, JSObject* const object)
throw (Error <INVALID_FIELD>)
{
	if (not objects .emplace (field, object) .second)
		throw Error <INVALID_FIELD> ("Object already exists in jsContext.");

	field -> addParent (this);
}

void
jsContext::removeObject (X3D::X3DFieldDefinition* const field)
{
	if (objects .erase (field))
		field -> removeParent (this);
	else
		__LOG__ << field -> getName () << " : " << field -> getTypeName () << std::endl;
}

JSBool
jsContext::require (const basic::uri & uri, jsval & rval)
{
	try
	{
		// Resolve uri and get cached result.

		const auto resolvedURL = worldURL .back () .transform (uri);
		const auto file        = files .find (resolvedURL);

		if (file not_eq files .end ())
		{
			rval = file -> second;
			return true;
		}

		// Load document.

		const auto document = Loader (getExecutionContext ()) .loadDocument (resolvedURL);

		// Evaluate script.

		worldURL .emplace_back (resolvedURL);

		const auto success = evaluate (document, resolvedURL, rval);

		worldURL .pop_back ();

		// Cache result.

		if (success)
		{
			auto & value = files .emplace (resolvedURL, rval) .first -> second;

			JS_AddValueRoot (cx, &value);

			return true;
		}
	}
	catch (const X3DError &)
	{ }

	return false;
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
		JS_ReportError (cx, "jsContext::evaluate: %s: %d: %s.", g_quark_to_string (error -> domain), error -> code, error -> message);
		return false;
	}

	const auto script = JS_CompileUCScript (cx, global, utf16_string, items_written, filename .empty () ? nullptr : filename .c_str (), 1);
	bool       retval = false;

	if (script)
		retval = JS_ExecuteScript (cx, global, script, &rval);

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

	if (future)
		future -> setExecutionContext (executionContext);

	X3DJavaScriptContext::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (this, &jsContext::set_live);

		set_live ();
	}
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
					const auto function = field -> getAccessType () == inputOnly
					                      ? getFunction (field -> getName ())
												 : getFunction ("set_" + field -> getName ());

					if (not JSVAL_IS_VOID (function))
						functions .emplace (field, function);

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
	field -> isTainted (cx);

	jsval argv [2];

	getValue (cx, field, &argv [0]);
	JS_NewNumberValue (cx, getCurrentTime (), &argv [1]);

	jsval rval;
	JS_CallFunctionValue (cx, global, function, 2, argv, &rval);

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
	static constexpr double GC_INTERVAL       = 12;  // in seconds
	static constexpr double TARGET_FRAME_RATE = 60;  // in frames per second
	static constexpr size_t MAX_OBJECTS       = 512; // number of cached objects

	const auto variation   = TARGET_FRAME_RATE * random1 ();
	const auto targetFrame = (GC_INTERVAL * 60 * TARGET_FRAME_RATE) / getBrowser () -> getCurrentFrameRate () + variation;

	if (++ frame < targetFrame and objects .size () < MAX_OBJECTS)
		return;

	frame = 0;

	__LOG__ << this << " : " << objects .size () << std::endl;

	JS_GC (cx);
}

jsval
jsContext::getFunction (const std::string & name) const
{
	jsval     function = JSVAL_VOID;
	JSObject* obj      = nullptr;

	const auto result = JS_GetMethod (cx, global, name .c_str (), &obj, &function);

	if (result)
		return function;

	return JSVAL_VOID;
}

void
jsContext::callFunction (const std::string & name) const
{
	jsval function = getFunction (name);

	if (JSVAL_IS_VOID (function))
		return;

	callFunction (function);
}

void
jsContext::callFunction (jsval function) const
{
	jsval rval;

	JS_CallFunctionValue (cx, global, function, 0, nullptr, &rval);
}

void
jsContext::error (JSContext* cx, const char* message, JSErrorReport* report)
{
	const auto context = getContext (cx);

	context -> setError (message,
	                     report -> filename ? report -> filename : "<inline>",
	                     report -> lineno,
	                     report -> tokenptr ? report -> tokenptr - report -> linebuf : -1,
	                     report -> linebuf ? report -> linebuf : "");
}

void
jsContext::dispose ()
{
	if (future)
	{
		future -> dispose ();
		future .reset (); // XXX: See Inline
	}

	for (auto & field : fields)
		JS_RemoveValueRoot (cx, &field .second);

	for (auto & file : files)
		JS_RemoveValueRoot (cx, &file .second);

	// Cleanup.
	JS_DestroyContext (cx);
	JS_DestroyRuntime (rt);

	assert (objects .empty ());

	X3DJavaScriptContext::dispose ();
}

jsContext::~jsContext ()
{ }

} // MozillaSpiderMonkey
} // X3D
} // titania

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
//#include "jsX3DConstants.h"

//#include "jsBrowser.h"
//#include "jsX3DExecutionContext.h"
//#include "jsX3DScene.h"

#include "jsComponentInfo.h"
#include "jsProfileInfo.h"
#include "jsX3DExternProtoDeclaration.h"
#include "jsX3DFieldDefinition.h"
#include "jsX3DProtoDeclaration.h"
#include "jsX3DRoute.h"

//#include "jsComponentInfoArray.h"
//#include "jsExternProtoDeclarationArray.h"
//#include "jsFieldDefinitionArray.h"
//#include "jsProfileInfoArray.h"
//#include "jsProtoDeclarationArray.h"
//#include "jsRouteArray.h"

#include "jsFields.h"

#include "jsString.h"
#include "jsError.h"
#include "jsfield.h"

#include <cassert>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

// mutex test: http://www.web3d.org/x3d/content/examples/Basic/StudentProjects/DeadReckoningComparisons.x3d

static size_t count = 0;

const ComponentType jsContext::component      = ComponentType::TITANIA;
const std::string   jsContext::typeName       = "jsContext";
const std::string   jsContext::containerField = "context";

JSClass jsContext::globalClass = {
	"global",
	JSCLASS_GLOBAL_FLAGS,
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

jsContext::jsContext (Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3DJavaScriptContext (script, ecmascript),
	                  rt (),
	                  cx (),
	              global (nullptr),
	            worldURL ({ uri }),
	          	  protos (size_t (ObjectType::SIZE)),
	              fields (),
	             objects (),
	               files (),
	              future (),
	               frame (0)
{
	rt .reset (JS_NewRuntime (64 * 1024 * 1024, JS_USE_HELPER_THREADS)); // 64 MB runtime memory

	if (not rt)
		throw std::runtime_error ("Couldn't create JavaScript runtime.");

	cx .reset (JS_NewContext (rt .get (), 8192));

	if (not cx)
		throw std::runtime_error ("Couldn't create JavaScript context.");

	const auto options = JS::CompartmentOptions () .setVersion (JSVERSION_LATEST);

	global = JS_NewGlobalObject (cx .get (), &globalClass, nullptr, options);

	if (not global)
		throw std::runtime_error ("Couldn't create JavaScript global object.");
	
	__LOG__ << this << " : " << ++ count << std::endl;

	JSAutoCompartment ac (cx .get (), global);

	JS_SetRuntimeThread (rt .get ());
	JS_SetOptions (cx .get (), JSOPTION_VAROBJFIX | JSOPTION_TYPE_INFERENCE | JSOPTION_COMPILE_N_GO);
	JS_SetErrorReporter (cx .get (), error);

	addClasses ();
	addUserDefinedFields ();
}

X3DBaseNode*
jsContext::create (X3DExecutionContext* const) const
{
	return new jsContext (getScriptNode (), getECMAScript (), worldURL .front ());
}

std::string
jsContext::getWorldUrl () const
{
	return worldURL .front () == getExecutionContext () -> getWorldURL () ? "<inline>" : worldURL .front ();
}

void
jsContext::addClasses ()
throw (std::runtime_error)
{
__LOG__ << this << std::endl;

	// Populate the global object with the standard globals, like Object and Array.
	if (not JS_InitStandardClasses (cx .get (), global))
		throw std::runtime_error ("Couldn't create JavaScript standard classes.");

	JS_SetRuntimePrivate (rt .get (), this);
	JS_SetContextPrivate (cx .get (), this);

	jsGlobals::init (cx .get (), global);

//	addProto (jsX3DConstants::getId (), jsX3DConstants::init (cx .get (), global, nullptr));
//	addProto (jsBrowser::getId (),      jsBrowser::init      (cx .get (), global, nullptr));

//	addProto (jsX3DExecutionContext::getId (), jsX3DExecutionContext::init (cx .get (), global, nullptr));
//	addProto (jsX3DScene::getId (),            jsX3DScene::init            (cx .get (), global, getProto (jsX3DExecutionContext::getId ())));

	addProto (jsComponentInfo::getId (),             jsComponentInfo::init             (cx .get (), global, nullptr));
	addProto (jsProfileInfo::getId (),               jsProfileInfo::init               (cx .get (), global, nullptr));
	addProto (jsX3DFieldDefinition::getId (),        jsX3DFieldDefinition::init        (cx .get (), global, nullptr));
	addProto (jsX3DExternProtoDeclaration::getId (), jsX3DExternProtoDeclaration::init (cx .get (), global, nullptr));
	addProto (jsX3DProtoDeclaration::getId (),       jsX3DProtoDeclaration::init       (cx .get (), global, nullptr));
	addProto (jsX3DRoute::getId (),                  jsX3DRoute::init                  (cx .get (), global, nullptr));

//	addProto (jsFieldDefinitionArray::getId (),        jsFieldDefinitionArray::init        (cx .get (), global, nullptr));
//	addProto (jsComponentInfoArray::getId (),          jsComponentInfoArray::init          (cx .get (), global, nullptr));
//	addProto (jsProfileInfoArray::getId (),            jsProfileInfoArray::init            (cx .get (), global, nullptr));
//	addProto (jsExternProtoDeclarationArray::getId (), jsExternProtoDeclarationArray::init (cx .get (), global, nullptr));
//	addProto (jsProtoDeclarationArray::getId (),       jsProtoDeclarationArray::init       (cx .get (), global, nullptr));
//	addProto (jsRouteArray::getId (),                  jsRouteArray::init                  (cx .get (), global, nullptr));

	addProto (jsX3DField::getId (),    jsX3DField::init    (cx .get (), global, nullptr));

	addProto (jsSFColor::getId (),     jsSFColor::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFColorRGBA::getId (), jsSFColorRGBA::init (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFImage::getId (),     jsSFImage::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix3d::getId (),  jsSFMatrix3d::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix3f::getId (),  jsSFMatrix3f::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix4d::getId (),  jsSFMatrix4d::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFMatrix4f::getId (),  jsSFMatrix4f::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFNode::getId (),      jsSFNode::init      (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFRotation::getId (),  jsSFRotation::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec2d::getId (),     jsSFVec2d::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec2f::getId (),     jsSFVec2f::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec3d::getId (),     jsSFVec3d::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec3f::getId (),     jsSFVec3f::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec4d::getId (),     jsSFVec4d::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsSFVec4f::getId (),     jsSFVec4f::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsVrmlMatrix::getId (),  jsVrmlMatrix::init  (cx .get (), global, getProto (jsX3DField::getId ())));

	addProto (jsMFBool::getId (),      jsMFBool::init      (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFColor::getId (),     jsMFColor::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFColorRGBA::getId (), jsMFColorRGBA::init (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFDouble::getId (),    jsMFDouble::init    (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFFloat::getId (),     jsMFFloat::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFImage::getId (),     jsMFImage::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFInt32::getId (),     jsMFInt32::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix3d::getId (),  jsMFMatrix3d::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix3f::getId (),  jsMFMatrix3f::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix4d::getId (),  jsMFMatrix4d::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFMatrix4f::getId (),  jsMFMatrix4f::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFNode::getId (),      jsMFNode::init      (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFRotation::getId (),  jsMFRotation::init  (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFString::getId (),    jsMFString::init    (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFTime::getId (),      jsMFTime::init      (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec2d::getId (),     jsMFVec2d::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec2f::getId (),     jsMFVec2f::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec3d::getId (),     jsMFVec3d::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec3f::getId (),     jsMFVec3f::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec4d::getId (),     jsMFVec4d::init     (cx .get (), global, getProto (jsX3DField::getId ())));
	addProto (jsMFVec4f::getId (),     jsMFVec4f::init     (cx .get (), global, getProto (jsX3DField::getId ())));
}

void
jsContext::addUserDefinedFields ()
{
	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
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
				fields .emplace (field -> getName (), RootedValue (cx .get (), getValue (cx .get (), field)));
				break;
			}
		}

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
			JS_DefineProperty (cx .get (),
			                   obj,
			                   name .c_str (),
			                   JS::UndefinedValue (),
			                   getBuildInProperty,
			                   setProperty,
			                   JSPROP_PERMANENT | JSPROP_SHARED | attrs);
			break;
		}
		default:
		{
			JS_DefineProperty (cx .get (),
			                   obj,
			                   name .c_str (),
			                   JS::UndefinedValue (),
			                   getProperty,
			                   setProperty,
			                   JSPROP_PERMANENT | JSPROP_SHARED | attrs);
			break;
		}
	}
}

JSBool
jsContext::setProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	const auto name = to_string (cx, id);

	try
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto field  = script -> getField (name);

		setValue (cx, field, vp);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "Couldn't assing value to user-defined field '%s': %s.", name .c_str (), error .what ());
	}
}

JSBool
jsContext::getBuildInProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	const auto name = to_string (cx, id);

	try
	{
		const auto script = getContext (cx) -> getScriptNode ();
		const auto field  = script -> getField (name);

		vp .set (getValue (cx, field));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "Couldn't retrieve value of user-defined field '%s': %s.", name .c_str (), error .what ());
	}
}

JSBool
jsContext::getProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	const auto name = to_string (cx, id);

	try
	{
		vp .set (getContext (cx) -> fields .at (name));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "Couldn't retrieve value of user-defined field '%s': %s.", name .c_str (), error .what ());
	}
}

void
jsContext::addObject (X3DFieldDefinition* const field, JSObject* const object)
throw (std::invalid_argument)
{
	if (not objects .emplace (field, object) .second)
		throw std::invalid_argument ("addObject");

	field -> addParent (this);
}

void
jsContext::removeObject (X3DFieldDefinition* const field)
{
	__LOG__ << field -> getName () << " : " << field -> getTypeName () << std::endl;

	if (objects .erase (field))
		field -> removeParent (this);
	else
		__LOG__ << field -> getName () << " : " << field -> getTypeName () << std::endl;
}

JSBool
jsContext::require (const basic::uri & uri, JS::Value & rval)
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
			files .emplace (resolvedURL, RootedValue (cx .get (), rval));

		return success;
	}
	catch (const X3DError &)
	{
		return false;
	}
}

bool
jsContext::evaluate (const std::string & string, const std::string & filename)
{
	JS::Value rval;

	return evaluate (string, filename, rval);
}

bool
jsContext::evaluate (const std::string & string, const std::string & filename, JS::Value & rval)
{
	const auto options = JS::CompileOptions (cx .get (), JSVERSION_LATEST) .setUTF8 (true);
	const auto script  = JS::Compile (cx .get (), JS::RootedObject (rt .get (), global), options, string .c_str (), string .length ());

	if (script)
		return JS_ExecuteScript (cx .get (), global, script, &rval);

	return false;
}

void
jsContext::initialize ()
{
	X3DJavaScriptContext::initialize ();
	
	JSAutoCompartment ac (cx .get (), global);

	if (not evaluate (getECMAScript (), getWorldUrl ()))
		throw std::invalid_argument ("Couldn't evaluate script.");

	getExecutionContext () -> isLive () .addInterest (this, &jsContext::set_live);
	isLive () .addInterest (this, &jsContext::set_live);

	set_live ();

	call ("initialize");

	shutdown () .addInterest (this, &jsContext::set_shutdown);

	finish ();
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
jsContext::set_live ()
{
	if (getExecutionContext () -> isLive () and isLive ())
	{
		try
		{
			getBrowser () -> prepareEvents () .addInterest (this, &jsContext::prepareEvents, RootedValue (cx .get (), getFunction ("prepareEvents")));
		}
		catch (const std::invalid_argument &)
		{ }

		try
		{
			getScriptNode () -> addInterest (this, &jsContext::eventsProcessed, RootedValue (cx .get (), getFunction ("eventsProcessed")));
		}
		catch (const std::invalid_argument &)
		{ }

		getScriptNode () -> addInterest (this, &jsContext::finish);

		for (const auto & field: getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly:
				{
					try
					{
						field -> addInterest (this, &jsContext::set_field, field, RootedValue (cx .get (), getFunction (field -> getName ())));
					}
					catch (const std::invalid_argument &)
					{ }

					break;
				}
				case inputOutput:
				{
					try
					{
						field -> addInterest (this, &jsContext::set_field, field, RootedValue (cx .get (), getFunction ("set_" + field -> getName ())));
					}
					catch (const std::invalid_argument &)
					{ }

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		getBrowser () -> prepareEvents () .removeInterest (this, &jsContext::prepareEvents);
		getScriptNode () -> removeInterest (this, &jsContext::eventsProcessed);
		getScriptNode () -> removeInterest (this, &jsContext::finish);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly   :
				case inputOutput :
				{
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
jsContext::prepareEvents (const JS::Value & value)
{
	JSAutoCompartment ac (cx .get (), global);

	call (value);
}

void
jsContext::set_field (X3DFieldDefinition* const field, const JS::Value & function)
{
__LOG__ << this << " : " << field -> getName () << std::endl;

	field -> isTainted (true);

	JSAutoCompartment ac (cx .get (), global);

	JS::Value argv [2] = {
		getValue (cx .get (), field),
		JS::DoubleValue (getCurrentTime ())
	};

	JS::Value rval;
	JS_CallFunctionValue (cx .get (), global, function, 2, argv, &rval);

	field -> isTainted (false);
}

void
jsContext::eventsProcessed (const JS::Value & value)
{
	JSAutoCompartment ac (cx .get (), global);

	call (value);
}

void
jsContext::set_shutdown ()
{
	JSAutoCompartment ac (cx .get (), global);

	call ("shutdown");
}

void
jsContext::finish ()
{
__LOG__ << this << std::endl;

	JS_SetRuntimeThread (rt .get ());
	JS_GC (rt .get ());

//	static constexpr double GC_INTERVAL       = 12;  // in seconds
//	static constexpr double TARGET_FRAME_RATE = 60;  // in frames per second
//	static constexpr size_t MAX_OBJECTS       = 512; // number of cached objects
//
//	const auto variation   = TARGET_FRAME_RATE * random1 ();
//	const auto targetFrame = (GC_INTERVAL * 60 * TARGET_FRAME_RATE) / getBrowser () -> getCurrentFrameRate () + variation;
//
//	if (++ frame < targetFrame and objects .size () < MAX_OBJECTS)
//		return;
//
//	frame = 0;
//
//	__LOG__ << this << " : " << objects .size () << std::endl;
//
//	JS_GC (rt .get ());
}

JS::Value
jsContext::getFunction (const std::string & name) const
throw (std::invalid_argument)
{
	JS::Value value;

	if (JS_GetProperty (cx .get (), global, name .c_str (), &value))
	{
		if (value .isObjectOrNull ())
		{
			const auto obj = value .toObjectOrNull ();

			if (obj and JS_ObjectIsFunction (cx .get (), obj))
				return value;
		}
	}

	throw std::invalid_argument ("getFunction");
}

void
jsContext::call (const std::string & value)
{
	try
	{
		call (getFunction (value));
	}
	catch (const std::invalid_argument &)
	{ }
}

void
jsContext::call (const JS::Value & value)
{
	JS::Value rval;

	JS_CallFunctionValue (cx .get (), global, value, 0, nullptr, &rval);
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
	__LOG__ << this << " : " << -- count << std::endl;

	X3DJavaScriptContext::dispose ();
	
	// Cleanup after base dispose!!!

	if (future)
	{
		future -> dispose ();
		future .reset (); // XXX: See Inline
	}

	JS_SetRuntimeThread (rt .get ());

	fields .clear ();
	files  .clear ();

	cx .reset ();
	rt .reset ();

	assert (objects .empty ());

}

jsContext::~jsContext ()
{ }

} // MozillaSpiderMonkey
} // X3D
} // titania

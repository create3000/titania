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

#include "Context.h"

#include "../../Browser/X3DBrowser.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Thread/SceneFuture.h"

#include "Global.h"
#include "X3DConstants.h"

#include "Browser.h"
#include "X3DExecutionContext.h"
#include "X3DScene.h"

#include "ComponentInfo.h"
#include "ProfileInfo.h"
#include "X3DExternProtoDeclaration.h"
#include "X3DFieldDefinition.h"
#include "X3DProtoDeclaration.h"
#include "X3DRoute.h"

#include "ComponentInfoArray.h"
#include "ExternProtoDeclarationArray.h"
#include "FieldDefinitionArray.h"
#include "ProfileInfoArray.h"
#include "ProtoDeclarationArray.h"
#include "RouteArray.h"

#include "Fields.h"

#include "String.h"
#include "value.h"

#include <cassert>

namespace titania {
namespace X3D {
namespace spidermonkey {

// mutex test: http://www.web3d.org/x3d/content/examples/Basic/StudentProjects/DeadReckoningComparisons.x3d

const ComponentType Context::component      = ComponentType::TITANIA;
const std::string   Context::typeName       = "SpiderMonkeyContext";
const std::string   Context::containerField = "context";

JSClass Context::globalClass = {
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

Context::Context (X3D::Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3DJavaScriptContext (script, ecmascript),
	                  rt (nullptr),
	                  cx (nullptr),
	              global (nullptr),
	            worldURL ({ uri }),
	        initializeFn (),
	     prepareEventsFn (),
	   eventsProcessedFn (),
	              protos (size_t (ObjectType::SIZE)),
	              fields (),
	           functions (),
	             objects (),
	          references (),
	               files (),
	       callbackTimes (),
	              future (),
	               frame (0)
{
	addChildObjects (future);

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

	shutdown () .addInterest (&Context::set_shutdown, this);

	addClasses ();
	addUserDefinedFields ();
}

X3DBaseNode*
Context::create (X3D::X3DExecutionContext* const) const
{
	return new Context (getScriptNode (), getECMAScript (), worldURL .front ());
}

///  throws std::runtime_error
void
Context::addClasses ()
{
	// Populate the global object with the standard globals, like Object and Array.
	if (not JS_InitStandardClasses (cx, global))
		throw std::runtime_error ("Couldn't create JavaScript standard classes.");

	JS_SetContextPrivate (cx, this);

	Globals::init (cx, global);

	addProto (X3DConstants::getId (), X3DConstants::init (cx, global, nullptr));
	addProto (Browser::getId (),      Browser::init      (cx, global, nullptr));

	addProto (X3DExecutionContext::getId (), X3DExecutionContext::init (cx, global, nullptr));
	addProto (X3DScene::getId (),            X3DScene::init            (cx, global, getProto (X3DExecutionContext::getId ())));

	addProto (ComponentInfo::getId (),             ComponentInfo::init             (cx, global, nullptr));
	addProto (ProfileInfo::getId (),               ProfileInfo::init               (cx, global, nullptr));
	addProto (X3DFieldDefinition::getId (),        X3DFieldDefinition::init        (cx, global, nullptr));
	addProto (X3DExternProtoDeclaration::getId (), X3DExternProtoDeclaration::init (cx, global, nullptr));
	addProto (X3DProtoDeclaration::getId (),       X3DProtoDeclaration::init       (cx, global, nullptr));
	addProto (X3DRoute::getId (),                  X3DRoute::init                  (cx, global, nullptr));

	addProto (FieldDefinitionArray::getId (),        FieldDefinitionArray::init        (cx, global, nullptr));
	addProto (ComponentInfoArray::getId (),          ComponentInfoArray::init          (cx, global, nullptr));
	addProto (ProfileInfoArray::getId (),            ProfileInfoArray::init            (cx, global, nullptr));
	addProto (ExternProtoDeclarationArray::getId (), ExternProtoDeclarationArray::init (cx, global, nullptr));
	addProto (ProtoDeclarationArray::getId (),       ProtoDeclarationArray::init       (cx, global, nullptr));
	addProto (RouteArray::getId (),                  RouteArray::init                  (cx, global, nullptr));

	addProto (X3DField::getId (),    X3DField::init    (cx, global, nullptr));

	addProto (SFColor::getId (),     SFColor::init     (cx, global, getProto (X3DField::getId ())));
	addProto (SFColorRGBA::getId (), SFColorRGBA::init (cx, global, getProto (X3DField::getId ())));
	addProto (SFImage::getId (),     SFImage::init     (cx, global, getProto (X3DField::getId ())));
	addProto (SFMatrix3d::getId (),  SFMatrix3d::init  (cx, global, getProto (X3DField::getId ())));
	addProto (SFMatrix3f::getId (),  SFMatrix3f::init  (cx, global, getProto (X3DField::getId ())));
	addProto (SFMatrix4d::getId (),  SFMatrix4d::init  (cx, global, getProto (X3DField::getId ())));
	addProto (SFMatrix4f::getId (),  SFMatrix4f::init  (cx, global, getProto (X3DField::getId ())));
	addProto (SFNode::getId (),      SFNode::init      (cx, global, getProto (X3DField::getId ())));
	addProto (SFRotation::getId (),  SFRotation::init  (cx, global, getProto (X3DField::getId ())));
	addProto (SFVec2d::getId (),     SFVec2d::init     (cx, global, getProto (X3DField::getId ())));
	addProto (SFVec2f::getId (),     SFVec2f::init     (cx, global, getProto (X3DField::getId ())));
	addProto (SFVec3d::getId (),     SFVec3d::init     (cx, global, getProto (X3DField::getId ())));
	addProto (SFVec3f::getId (),     SFVec3f::init     (cx, global, getProto (X3DField::getId ())));
	addProto (SFVec4d::getId (),     SFVec4d::init     (cx, global, getProto (X3DField::getId ())));
	addProto (SFVec4f::getId (),     SFVec4f::init     (cx, global, getProto (X3DField::getId ())));
	addProto (VrmlMatrix::getId (),  VrmlMatrix::init  (cx, global, getProto (X3DField::getId ())));

	addProto (MFBool::getId (),      MFBool::init      (cx, global, getProto (X3DField::getId ())));
	addProto (MFColor::getId (),     MFColor::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFColorRGBA::getId (), MFColorRGBA::init (cx, global, getProto (X3DField::getId ())));
	addProto (MFDouble::getId (),    MFDouble::init    (cx, global, getProto (X3DField::getId ())));
	addProto (MFFloat::getId (),     MFFloat::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFImage::getId (),     MFImage::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFInt32::getId (),     MFInt32::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFMatrix3d::getId (),  MFMatrix3d::init  (cx, global, getProto (X3DField::getId ())));
	addProto (MFMatrix3f::getId (),  MFMatrix3f::init  (cx, global, getProto (X3DField::getId ())));
	addProto (MFMatrix4d::getId (),  MFMatrix4d::init  (cx, global, getProto (X3DField::getId ())));
	addProto (MFMatrix4f::getId (),  MFMatrix4f::init  (cx, global, getProto (X3DField::getId ())));
	addProto (MFNode::getId (),      MFNode::init      (cx, global, getProto (X3DField::getId ())));
	addProto (MFRotation::getId (),  MFRotation::init  (cx, global, getProto (X3DField::getId ())));
	addProto (MFString::getId (),    MFString::init    (cx, global, getProto (X3DField::getId ())));
	addProto (MFTime::getId (),      MFTime::init      (cx, global, getProto (X3DField::getId ())));
	addProto (MFVec2d::getId (),     MFVec2d::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFVec2f::getId (),     MFVec2f::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFVec3d::getId (),     MFVec3d::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFVec3f::getId (),     MFVec3f::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFVec4d::getId (),     MFVec4d::init     (cx, global, getProto (X3DField::getId ())));
	addProto (MFVec4f::getId (),     MFVec4f::init     (cx, global, getProto (X3DField::getId ())));
}

void
Context::addUserDefinedFields ()
{
	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		addUserDefinedField (field);

		switch (field -> getAccessType ())
		{
			case X3D::initializeOnly:
			case X3D::outputOnly:
			{
				defineProperty (global, field, field -> getName (), JSPROP_ENUMERATE);
				break;
			}
			case X3D::inputOnly:
				break;
			case X3D::inputOutput:
			{
				defineProperty (global, field, field -> getName (),              JSPROP_ENUMERATE);
				defineProperty (global, field, field -> getName () + "_changed", 0);
				break;
			}
		}
	}
}

void
Context::addUserDefinedField (X3D::X3DFieldDefinition* const field)
{
	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		case X3D::X3DConstants::SFDouble:
		case X3D::X3DConstants::SFFloat:
		case X3D::X3DConstants::SFInt32:
		case X3D::X3DConstants::SFNode:
		case X3D::X3DConstants::SFString:
		case X3D::X3DConstants::SFTime:
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
Context::defineProperty (JSObject* const obj,
                         X3D::X3DFieldDefinition* const field,
                         const std::string & name,
                         const uint32_t attrs)
{
	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFBool:
		case X3D::X3DConstants::SFDouble:
		case X3D::X3DConstants::SFFloat:
		case X3D::X3DConstants::SFInt32:
		case X3D::X3DConstants::SFNode:
		case X3D::X3DConstants::SFString:
		case X3D::X3DConstants::SFTime:
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
Context::setProperty (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
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
Context::getBuildInProperty (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
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
Context::getProperty (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
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
Context::addObject (X3D::X3DChildObject* const key, X3D::X3DFieldDefinition* const field, JSObject* const object)
{
	assert (objects .emplace (key -> getId (), object) .second);

	field -> addParent (this);

	//__LOG__ << field -> getTypeName () << " : " << field -> getName ()  << " : " << key << " : " << field << std::endl;
}

void
Context::removeObject (X3D::X3DChildObject* const key, X3D::X3DFieldDefinition* const field)
{
	if (objects .erase (key -> getId ()))
		field -> removeParent (this);

	else
		__LOG__ << field -> getTypeName () << " : " << field -> getName ()  << " : " << key << " : " << field << std::endl;
}

JSObject*
Context::getObject (X3D::X3DChildObject* const key) const
{
	const auto iter = objects .find (key -> getId ());

	if (iter not_eq objects .end ())
		return iter -> second;

	return nullptr;
}

void
Context::setReference (X3D::X3DFieldDefinition* const array, const size_t index, X3D::X3DFieldDefinition* const reference)
{
	const auto key  = std::pair (size_t (array), index);
	const auto iter = references .find (key);

	if (iter not_eq references .end ())
		iter -> second -> disposed () .removeInterest (&Context::removeReference, this);

	reference -> disposed () .addInterest (&Context::removeReference, this, array, index);

	references .emplace (key, reference);
}

void
Context::removeReference (X3D::X3DFieldDefinition* const array, const size_t index)
{
	const auto key = std::pair (size_t (array), index);
	
	references .erase (key);
}

X3D::X3DFieldDefinition*
Context::getReference (X3D::X3DFieldDefinition* const array, const size_t index)
{
	const auto key  = std::pair (size_t (array), index);
	const auto iter = references .find (key);

	if (iter not_eq references .end ())
		return iter -> second;

	return nullptr;
}

JSBool
Context::require (const basic::uri & uri, jsval & rval)
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

		const auto document = FileLoader (getExecutionContext ()) .loadDocument (resolvedURL);

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
Context::evaluate (const std::string & string, const std::string & filename)
{
	jsval rval;

	return evaluate (string, filename, rval);
}

JSBool
Context::evaluate (const std::string & string, const std::string & filename, jsval & rval)
{
	glong   items_read    = 0;
	glong   items_written = 0;
	GError* error         = nullptr;

	gunichar2* utf16_string = g_utf8_to_utf16 (string .c_str (), string .length (), &items_read, &items_written, &error);

	if (error)
	{
		JS_ReportError (cx, "Context::evaluate: %s: %d: %s.", g_quark_to_string (error -> domain), error -> code, error -> message);
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
Context::initialize ()
{
	X3DJavaScriptContext::initialize ();

	if (not evaluate (getECMAScript (), worldURL .front () == getExecutionContext () -> getWorldURL () ? "" : worldURL .front () .str ()))
		throw std::invalid_argument ("Couldn't evaluate script.");

	// Do not execute Script nodes within ProtoDeclarations.
	if (getExecutionContext () -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
		return;

	setEventHandler ();

	set_live ();

	if (not JSVAL_IS_VOID (initializeFn))
		callFunction (initializeFn);
}

void
Context::setExecutionContext (X3D::X3DExecutionContext* const executionContext)
{
	if (future)
		future -> setExecutionContext (executionContext);

	X3DJavaScriptContext::setExecutionContext (executionContext);
}

void
Context::setEventHandler ()
{
	initializeFn      = getFunction ("initialize");
	prepareEventsFn   = getFunction ("prepareEvents");
	eventsProcessedFn = getFunction ("eventsProcessed");

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
				{
					const auto iter = functions .emplace (field, function);

					JS_AddValueRoot (cx, &iter .first -> second);
				}

				break;
			}
			default :
				break;
		}
	}
}

void
Context::set_live ()
{
	if (not JSVAL_IS_VOID (prepareEventsFn))
		getBrowser () -> prepareEvents () .addInterest (&Context::prepareEvents, this);

	if (not JSVAL_IS_VOID (eventsProcessedFn))
		getScriptNode () -> addInterest (&Context::eventsProcessed, this);

	getScriptNode () -> addInterest (&Context::finish, this);

	for (const auto & field: getScriptNode () -> getUserDefinedFields ())
	{
		switch (field -> getAccessType ())
		{
			case inputOnly:
			case inputOutput:
			{
				if (functions .count (field))
					field -> addInterest (&Context::set_field, this, field, functions [field]);

				break;
			}
			default:
				break;
		}
	}
}

void
Context::prepareEvents ()
{
	callFunction (prepareEventsFn);
	getBrowser () -> addEvent ();
}

void
Context::set_field (X3D::X3DFieldDefinition* const field, const jsval & function)
{
	if (callbackTimes [field] == getCurrentTime ())
		return; // Event loop detected.
	else
		callbackTimes [field] = getCurrentTime ();

	field -> setTainted (true);

	jsval argv [2];

	getValue (cx, field, &argv [0]);
	JS_NewNumberValue (cx, getCurrentTime (), &argv [1]);

	jsval rval;
	JS_CallFunctionValue (cx, global, function, 2, argv, &rval);

	field -> setTainted (false);
}

void
Context::eventsProcessed ()
{
	callFunction (eventsProcessedFn);
}

void
Context::set_shutdown ()
{
	const auto shutdownFn = getFunction ("shutdown");

	if (not JSVAL_IS_VOID (shutdownFn))
		callFunction (shutdownFn);

	for (auto & field : fields)
		JS_RemoveValueRoot (cx, &field .second);

	for (auto & function : functions)
		JS_RemoveValueRoot (cx, &function .second);

	for (auto & file : files)
		JS_RemoveValueRoot (cx, &file .second);

	// Cleanup.
	JS_DestroyContext (cx);
	JS_DestroyRuntime (rt);

	assert (objects .empty ());
}

void
Context::finish ()
{
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

	//__LOG__ << this << " : " << objects .size () << std::endl;

	JS_GC (cx);
}

jsval
Context::getFunction (const std::string & name) const
{
	jsval     function = JSVAL_VOID;
	JSObject* obj      = nullptr;

	const auto result = JS_GetMethod (cx, global, name .c_str (), &obj, &function);

	if (result)
		return function;

	return JSVAL_VOID;
}

void
Context::callFunction (const std::string & name) const
{
	jsval function = getFunction (name);

	if (JSVAL_IS_VOID (function))
		return;

	callFunction (function);
}

void
Context::callFunction (jsval function) const
{
	jsval rval;

	JS_CallFunctionValue (cx, global, function, 0, nullptr, &rval);
}

void
Context::connectEventsProcessed ()
{
	getScriptNode () -> removeInterest (&Context::connectEventsProcessed, this);
	getScriptNode () -> addInterest (&Context::eventsProcessed, this);
}

void
Context::error (JSContext* cx, const char* message, JSErrorReport* report)
{
	const auto context = getContext (cx);

	context -> setError (message,
	                     report -> filename ? report -> filename : "<inline>",
	                     report -> lineno,
	                     report -> tokenptr ? report -> tokenptr - report -> linebuf : -1,
	                     report -> linebuf ? report -> linebuf : "");
}

void
Context::dispose ()
{
	future .setValue (nullptr);

	X3DJavaScriptContext::dispose ();
}

Context::~Context ()
{ }

} // spidermonkey
} // X3D
} // titania

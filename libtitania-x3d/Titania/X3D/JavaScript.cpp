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
 ******************************************************************************/

#include "JavaScript.h"

#include "Browser.h"
//#include "JavaScript/jsBrowser.h"
//#include "JavaScript/jsFields.h"
//#include "JavaScript/jsGlobals.h"
//#include "JavaScript/jsfield.h"

namespace titania {
namespace X3D {

JSClass JavaScript::global_class = {
	"global", JSCLASS_GLOBAL_FLAGS,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

std::string
JavaScript::getVersion ()
{
	return JS_VersionToString (JS_GetVersion (context));
}

std::string
JavaScript::getImplementationVersion ()
{
	return JS_GetImplementationVersion ();
}

/*
 * The error reporter callback.
 */

void
JavaScript::error (JSContext* context, const char* message, JSErrorReport* report)
{
	std::clog << "# Javascript: runtime error on line "
	          << (unsigned int) report -> lineno
	          << " in "
	          << (*report -> filename ? report -> filename : "<no filename>") << ": "
	          << message << std::endl;
}

JavaScript::JavaScript (X3DBasicNode* node, const std::string & ecmascript) :
	browser (node -> getBrowser ()), 
	   node (node)                   
{
	// Create a JS runtime.
	runtime = JS_NewRuntime (8L * 1024L * 1024L);

	if (runtime == NULL)
		return;

	// Create a context.
	context = JS_NewContext (runtime, 8192);

	if (context == NULL)
		return;

	JS_SetOptions (context, JSOPTION_VAROBJFIX);
	JS_SetVersion (context, JSVERSION_LATEST);
	JS_SetErrorReporter (context, error);

	// Create the global object.
	global = JS_NewObject (context, &global_class, NULL, NULL);

	if (global == NULL)
		return;

	initContext ();

	initNode ();

	evaluate (ecmascript, node -> getName ());
}

void
JavaScript::initContext ()
{
	// Populate the global object with the standard globals, like Object and Array.
	if (not JS_InitStandardClasses (context, global))
		return;

	JS_SetContextPrivate (context, node);

	jsGlobals::init (context, global);

	jsBrowser::defineObject (context, global);

	jsSFColor::init (context, global);
	jsSFColorRGBA::init (context, global);
	jsSFImage::init (context, global);
	jsSFMatrix3d::init (context, global);
	jsSFMatrix3f::init (context, global);
	jsSFMatrix4d::init (context, global);
	jsSFMatrix4f::init (context, global);
	jsSFNode <X3DBasicNode>::init (context, global);
	jsSFRotation::init (context, global);
	jsSFVec2d::init (context, global);
	jsSFVec2f::init (context, global);
	jsSFVec3d::init (context, global);
	jsSFVec3f::init (context, global);
	jsSFVec4d::init (context, global);
	jsSFVec4f::init (context, global);
	jsVrmlMatrix::init (context, global);

	jsMFBool::init (context, global);
	jsMFColor::init (context, global);
	jsMFColorRGBA::init (context, global);
	jsMFDouble::init (context, global);
	jsMFFloat::init (context, global);
	jsMFImage::init (context, global);
	jsMFInt32::init (context, global);
	jsMFMatrix3d::init (context, global);
	jsMFMatrix3f::init (context, global);
	jsMFMatrix4d::init (context, global);
	jsMFMatrix4f::init (context, global);
	jsMFNode::init (context, global);
	jsMFRotation::init (context, global);
	jsMFString::init (context, global);
	jsMFTime::init (context, global);
	jsMFVec2d::init (context, global);
	jsMFVec2f::init (context, global);
	jsMFVec3d::init (context, global);
	jsMFVec3f::init (context, global);
	jsMFVec4d::init (context, global);
	jsMFVec4f::init (context, global);
}

void
JavaScript::defineProperty (
   JSContext* context,
   JSObject* obj,
   const std::string & name,
   int8 tinyid,
   X3DFieldDefinition* field,
   JSPropertyOp sharedGetter,
   JSPropertyOp sharedSetter,
   JSPropertyOp setter,
   uintN attrs
   )
{
	switch (field -> getType ())
	{
		case SFBoolType:
		case SFDoubleType:
		case SFFloatType:
		case SFInt32Type:
		case SFStringType:
		case SFTimeType:
			JS_DefinePropertyWithTinyId (context, obj, name .c_str (), tinyid, JSVAL_VOID, sharedGetter, sharedSetter, JSPROP_SHARED | attrs);
			break;
		default:
			jsval value;
			JS_NewFieldValue (context, field, &value);
			JS_DefinePropertyWithTinyId (context, obj, name .c_str (), tinyid, value, NULL, setter, attrs);
			break;
	}
}

void
JavaScript::initNode ()
{
	size_t                               i = 0;
	FieldDefinitionArray::const_iterator field;

	for (field = node -> getFieldDefinitions () .begin (); field not_eq node -> getFieldDefinitions () .end (); ++ field, ++ i)
	{
		if (not (*field) -> getCustom ())
			continue;

		switch ((*field) -> getAccessType ())
		{
			case initializeOnly:
			case outputOnly:
			{
				defineProperty (context, global, (*field) -> getName (), i, *field, getSharedProperty, setSharedProperty, setProperty, JSPROP_ENUMERATE | JSPROP_PERMANENT);
				break;
			}
			case inputOnly:
			{
				field -> addInterest (this, &JavaScript::set_field);
				break;
			}
			case inputOutput:
			{
				defineProperty (context, global, (*field) -> getName (), i, *field, getSharedProperty, setSharedProperty, setProperty, JSPROP_ENUMERATE | JSPROP_PERMANENT);
				defineProperty (context, global, "set_" + (*field) -> getName (), i, *field, getSharedProperty, setSharedProperty, setProperty, JSPROP_PERMANENT);
				defineProperty (context, global, (*field) -> getName () + "_changed", i, *field, getSharedProperty, setSharedProperty, setProperty, JSPROP_PERMANENT);
				break;
			}
		}
	}
}

JSBool
JavaScript::getSharedProperty (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	X3DFieldDefinition* field = node -> getFieldDefinitions () .at (JSVAL_TO_INT (id));

	return JS_NewFieldValue (context, field, vp);
}

JSBool
JavaScript::setSharedProperty (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	X3DFieldDefinition* field = node -> getFieldDefinitions () .at (JSVAL_TO_INT (id));

	return JS_ValueToField (context, field, vp);
}

JSBool
JavaScript::setProperty (JSContext* context, JSObject* obj, jsval id, jsval* vp)
{
	X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

	X3DFieldDefinition* field = node -> getFieldDefinitions () .at (JSVAL_TO_INT (id));

	return
	   JS_ValueToField (context, field, vp) and
	   JS_LookupProperty (context, obj, field -> getName () .c_str (), vp); // JS_LookupPropertyById(context, obj, id, vp); (not jet released)
}

void
JavaScript::evaluate (const std::string & string, const std::string & filename)
{
	browser -> pushExecutionContext (node -> getExecutionContext ());

	jsval rval;
	JS_EvaluateScript (context, global, string .c_str (), string .length (), filename .c_str (), 1, &rval);

	browser -> popExecutionContext ();

	JS_GC (context);                                                        // Garbage Collector
}

void
JavaScript::callFunction (const std::string & function)
{
	jsval     func_val = JSVAL_VOID;
	JSObject* objp;

	JSBool result = JS_GetMethod (context, global, function .c_str (), &objp, &func_val);

	if (not result or JSVAL_IS_VOID (func_val))
		return;

	jsval rval;
	JS_CallFunctionValue (context, global, func_val, 0, NULL, &rval);

	JS_GC (context);
}

void
JavaScript::set_field (const X3DFieldDefinition & field)
{
	jsval     func_val = JSVAL_VOID;
	JSObject* objp;

	JSBool result = JS_GetMethod (context, global, field .getName () .c_str (), &objp, &func_val);

	if (not result or JSVAL_IS_VOID (func_val))
		return;

	jsval argv [2];
	JS_NewFieldValue (context, &field, &argv [0], true);
	JS_NewDoubleValue (context, X3D::Browser () -> getCurrentTime (), &argv [1]);

	jsval rval;
	JS_CallFunctionValue (context, global, func_val, 2, argv, &rval);

	JS_GC (context);
}

void
JavaScript::initialize ()
{
	callFunction ("initialize");
}

void
JavaScript::prepareEvents ()
{
	callFunction ("prepareEvents");
}

void
JavaScript::eventsProcessed ()
{
	callFunction ("eventsProcessed");
}

void
JavaScript::shutdown ()
{
	callFunction ("shutdown");
}

JavaScript::~JavaScript ()
{
	/* Cleanup. */
	JS_GC (context);
	JS_DestroyContext (context);
	JS_DestroyRuntime (runtime);
}

} // X3D
} // titania

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

#include "jsBrowser.h"
#include "jsFields.h"
#include "jsGlobals.h"
#include "jsfield.h"

namespace titania {
namespace X3D {

JSClass JavaScript::global_class = {
	"global", JSCLASS_GLOBAL_FLAGS,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

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

	JS_SetOptions (context, JSOPTION_VAROBJFIX | JSOPTION_METHODJIT);
	JS_SetVersion (context, JSVERSION_LATEST);
	JS_SetErrorReporter (context, error);

	// Create the global object.
	global = JS_NewCompartmentAndGlobalObject (context, &global_class, NULL);

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
	jsSFNode::init (context, global);
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
JavaScript::initNode ()
{
	for (auto & field : node -> getUserDefinedFields ())
	{
		switch (field -> getAccessType ())
		{
			case initializeOnly:
			case outputOnly:
			{
				defineProperty (context, global, field -> getName (), JSPROP_ENUMERATE);
				break;
			}
			case inputOnly:
			{
				field -> addInterest (this, &JavaScript::set_field, *field);
				break;
			}
			case inputOutput:
			{
				defineProperty (context, global, field -> getName (),              JSPROP_ENUMERATE);
				defineProperty (context, global, "set_" + field -> getName (),     0);
				defineProperty (context, global, field -> getName () + "_changed", 0);
				break;
			}
		}
	}
}

void
JavaScript::defineProperty (JSContext* context,
                            JSObject* obj,
                            const std::string & name,
                            uintN attrs)
{
	JS_DefineProperty (context,
	                   obj, name .c_str (),
	                   JSVAL_VOID,
	                   getProperty, setProperty,
	                   JSPROP_PERMANENT | attrs);
}

JSBool
JavaScript::getProperty (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	jsval name;
	
	if (JS_IdToValue (context, id, &name))
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		X3DFieldDefinition* field = node -> getField (JS_EncodeString (context, JSVAL_TO_STRING (name)));

		return JS_NewFieldValue (context, field, vp);
	}

	return JS_FALSE;
}

JSBool
JavaScript::setProperty (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	jsval name;
	
	if (JS_IdToValue (context, id, &name))
	{
		X3DBasicNode* node = (X3DBasicNode*) JS_GetContextPrivate (context);

		X3DFieldDefinition* field = node -> getField (JS_EncodeString (context, JSVAL_TO_STRING (name)));

		return JS_ValueToField (context, field, vp);
	}
	
	return JS_FALSE;
}

void
JavaScript::evaluate (const std::string & string, const std::string & filename)
{
	jsval rval;

	JS_EvaluateScript (context, global, string .c_str (), string .length (), filename .c_str (), 1, &rval);

	JS_GC (context); // Garbage Collector
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

	JSBool result = JS_GetMethod (context, global, field .getName () [0] .c_str (), &objp, &func_val);

	if (not result or JSVAL_IS_VOID (func_val))
		return;

	jsval argv [2];
	JS_NewFieldValue (context, const_cast <X3DFieldDefinition*> (&field), &argv [0], true);
	JS_NewNumberValue (context, 0 /* time */, &argv [1]);

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

void
JavaScript::error (JSContext* context, const char* message, JSErrorReport* report)
{
	std::clog << "# Javascript: runtime error on line "
	          << (unsigned int) report -> lineno
	          << " in "
	          << (*report -> filename ? report -> filename : "<no filename>") << ": "
	          << message << std::endl;
}

JavaScript::~JavaScript ()
{
	// Cleanup.
	JS_GC (context);
	JS_DestroyContext (context);
	JS_DestroyRuntime (runtime);
}

} // X3D
} // titania

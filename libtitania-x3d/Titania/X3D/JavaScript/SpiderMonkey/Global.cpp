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

#include "Global.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Scripting/Script.h"
#include "../../InputOutput/Generator.h"
#include "Arguments.h"
#include "Context.h"
#include "Error.h"
#include "String.h"

#include <Titania/LOG.h>

namespace titania {
namespace X3D {
namespace spidermonkey {

JSPropertySpec Globals::properties [ ] = {
	{ "NULL",  0,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT, null_,  nullptr },
	{ "FALSE", false, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT, false_, nullptr },
	{ "TRUE",  true,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT, true_,  nullptr },
	{ 0 }

};

JSFunctionSpec Globals::functions [ ] = {
	{ "print",   Globals::print,   0, 0 }, // VRML 2.0
	{ "trace",   Globals::print,   0, 0 }, // Non standard
	{ "require", Globals::require, 1, 0 }, // Non standard

	{ "SFBool",   Globals::SFBool,   1, 0 }, // Dummy native objects
	{ "SFDouble", Globals::SFDouble, 1, 0 }, // Dummy native objects
	{ "SFFloat",  Globals::SFFloat,  1, 0 }, // Dummy native objects
	{ "SFInt32",  Globals::SFInt32,  1, 0 }, // Dummy native objects
	{ "SFString", Globals::SFString, 1, 0 }, // Dummy native objects
	{ "SFTime",   Globals::SFTime,   1, 0 }, // Dummy native objects
	{ 0 }

};

void
Globals::init (JSContext* const cx, JSObject* const global)
{
	JS_DefineProperties (cx, global, properties);
	JS_DefineFunctions (cx, global, functions);
}

JSBool
Globals::null_ (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	*vp = JSVAL_NULL;
	return true;
}

JSBool
Globals::false_ (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	*vp = JSVAL_FALSE;
	return true;
}

JSBool
Globals::true_ (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	*vp = JSVAL_TRUE;
	return true;
}

JSBool
Globals::print (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto browser = getContext (cx) -> getBrowser ();
	const auto argv    = JS_ARGV (cx, vp);

	for (uint32_t i = 0; i < argc; ++ i)
		browser -> print (to_string (cx, argv [i]));

	browser -> print ('\n');

	JS_SET_RVAL (cx, vp, JSVAL_VOID);
	return true;
}

// Non standard

JSBool
Globals::require (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto context = getContext (cx);
	const auto argv    = JS_ARGV (cx, vp);
	auto       success = false;
	auto       rval    = JSVAL_VOID;

	for (uint32_t i = 0; i < argc; ++ i)
	{
		if (context -> require (to_string (cx, argv [i]), rval))
		{
			success = true;
			break;
		}
	}

	JS_SET_RVAL (cx, vp, rval);
	return success;
}

// Dummy native objects

JSBool
Globals::SFBool (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto context = getContext (cx);
	const auto argv    = JS_ARGV (cx, vp);

	JSObject* Boolean = nullptr;

	JS_GetClassObject (cx, context -> getGlobal (), JSProto_Boolean, &Boolean);

	const auto object = JS_New (cx, Boolean, argc, argv);

	JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (object));
	return true;
}

JSBool
Globals::SFDouble (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto context = getContext (cx);
	const auto argv    = JS_ARGV (cx, vp);

	JSObject* Number = nullptr;

	JS_GetClassObject (cx, context -> getGlobal (), JSProto_Number, &Number);

	const auto object = JS_New (cx, Number, argc, argv);

	JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (object));
	return true;
}

JSBool
Globals::SFFloat (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto context = getContext (cx);
	const auto argv    = JS_ARGV (cx, vp);

	JSObject* Number = nullptr;

	JS_GetClassObject (cx, context -> getGlobal (), JSProto_Number, &Number);

	const auto object = JS_New (cx, Number, argc, argv);

	JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (object));
	return true;
}

JSBool
Globals::SFInt32 (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto context = getContext (cx);
	auto       argv    = JS_ARGV (cx, vp);

	for (uint32_t i = 0; i < argc; ++ i)
		JS_NewNumberValue (cx, getArgument <int32_t> (cx, argv, i), &argv [i]);

	JSObject* Number = nullptr;

	JS_GetClassObject (cx, context -> getGlobal (), JSProto_Number, &Number);

	const auto object = JS_New (cx, Number, argc, argv);

	JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (object));
	return true;
}

JSBool
Globals::SFString (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto context = getContext (cx);
	const auto argv    = JS_ARGV (cx, vp);

	JSObject* String = nullptr;

	JS_GetClassObject (cx, context -> getGlobal (), JSProto_String, &String);

	const auto object = JS_New (cx, String, argc, argv);

	JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (object));
	return true;
}

JSBool
Globals::SFTime (JSContext* cx, uint32_t argc, jsval* vp)
{
	const auto context = getContext (cx);
	const auto argv    = JS_ARGV (cx, vp);

	JSObject* Number = nullptr;

	JS_GetClassObject (cx, context -> getGlobal (), JSProto_Number, &Number);

	const auto object = JS_New (cx, Number, argc, argv);

	JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (object));
	return true;
}

} // spidermonkey
} // X3D
} // titania

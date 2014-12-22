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

#include "jsGlobals.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Scripting/Script.h"
#include "../../InputOutput/Generator.h"
#include "jsContext.h"
#include "jsString.h"

#include <Titania/LOG.h>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSPropertySpec jsGlobals::properties [ ] = {
	JS_PSG ("NULL",  null_,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT),
	JS_PSG ("FALSE", false_, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT),
	JS_PSG ("TRUE",  true_,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT),
	JS_PS_END

};

JSFunctionSpec jsGlobals::functions [ ] = {
	JS_FS ("print",   jsGlobals::print,   0, JSPROP_PERMANENT), // VRML 2.0
	JS_FS ("trace",   jsGlobals::print,   0, JSPROP_PERMANENT), // Non standard
	JS_FS ("require", jsGlobals::require, 1, JSPROP_PERMANENT), // Non standard
	JS_FS_END

};

void
jsGlobals::init (JSContext* const cx, JSObject* const global)
{
	JS_DefineProperties (cx, global, properties);
	JS_DefineFunctions (cx, global, functions);
}

JSBool
jsGlobals::null_ (JSContext* cx, unsigned argc, JS::Value* vp)
{
	vp -> setNull ();
	return true;
}

JSBool
jsGlobals::false_ (JSContext* cx, unsigned argc, JS::Value* vp)
{
	vp -> setBoolean (false);
	return true;
}

JSBool
jsGlobals::true_ (JSContext* cx, unsigned argc, JS::Value* vp)
{
	vp -> setBoolean (true);
	return true;
}

JSBool
jsGlobals::print (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto browser = getContext (cx) -> getBrowser ();

	for (unsigned i = 0; i < argc; ++ i)
		browser -> print (to_string (cx, args [i]));

	browser -> print ('\n');

	args .rval () .setUndefined ();
	return true;
}

// Non standard

JSBool
jsGlobals::require (JSContext* cx, unsigned argc, JS::Value* vp)
{
	const auto args    = JS::CallArgsFromVp (argc, vp);
	const auto context = getContext (cx);
	auto       success = false;
	auto       rval    = JS::UndefinedValue ();

	for (unsigned i = 0; i < argc; ++ i)
	{
		if (context -> require (to_string (cx, args [i]), rval))
		{
			success = true;
			break;
		}
	}

	args .rval () .set (rval);
	return success;
}

} // MozillaSpiderMonkey
} // X3D
} // titania

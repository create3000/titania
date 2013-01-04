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

#include "jsGlobals.h"

#include "../../InputOutput/Generator.h"
#include "Fields/jsSFNode.h"
#include "String.h"

#include <Titania/LOG.h>

namespace titania {
namespace X3D {

JSPropertySpec jsGlobals::properties [ ] = {
	{ "NULL",  0,     JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT, _null,  NULL },
	{ "FALSE", false, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT, _false, NULL },
	{ "TRUE",  true,  JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_PERMANENT, _true,  NULL },
	{ 0 }

};

JSFunctionSpec jsGlobals::functions [ ] = {
	{ "include", jsGlobals::include, 1, 0 },
	{ "print",   jsGlobals::print,   0, 0 },
	{ 0 }

};

jsval jsGlobals::X3D_JS_NULL = JSVAL_VOID;

void
jsGlobals::init (JSContext* context, JSObject* global)
{
	JS_DefineProperties (context, global, properties);
	JS_DefineFunctions (context, global, functions);

	jsSFNode::create (context, new SFNode <X3DBaseNode> (), &X3D_JS_NULL);
}

JSBool
jsGlobals::_null (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	*vp = X3D_JS_NULL;
	return JS_TRUE;
}

JSBool
jsGlobals::_false (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	*vp = JSVAL_FALSE;
	return JS_TRUE;
}

JSBool
jsGlobals::_true (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	*vp = JSVAL_TRUE;
	return JS_TRUE;
}

JSBool
jsGlobals::include (JSContext* context, uintN argc, jsval* vp)
{
	jsval* argv = JS_ARGV (context, vp);

	for (uintN i = 0; i < argc; ++ i)
		std::clog << JS_GetString (context, argv [i]);

	std::clog << std::endl;

	JS_SET_RVAL (context, vp, JSVAL_VOID);
	return JS_TRUE;
}

JSBool
jsGlobals::print (JSContext* context, uintN argc, jsval* vp)
{
	jsval* argv = JS_ARGV (context, vp);

	for (uintN i = 0; i < argc; ++ i)
		std::clog << JS_GetString (context, argv [i]);

	std::clog << std::endl;

	JS_SET_RVAL (context, vp, JSVAL_VOID);
	return JS_TRUE;
}

} // X3D
} // titania

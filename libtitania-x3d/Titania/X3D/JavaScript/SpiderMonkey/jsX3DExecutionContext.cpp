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

#include "../Error.h"
#include "../Scene.h"
#include "Fields/jsMFNode.h"
#include "Fields/jsSFNode.h"
#include "jsX3DExecutionContext.h"

namespace titania {
namespace X3D {

JSClass jsX3DExecutionContext::static_class = {
	"X3DExecutionContext", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DExecutionContext::properties [ ] = {
	{ "rootNodes", ROOTNODES, JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT, getRootNodes, setRootNodes },
	{ 0 }

};

JSFunctionSpec jsX3DExecutionContext::functions [ ] = {
	{ "createNode", createNode, 1, 0, 0 },
	{ 0, 0, 0, 0, 0 }

};

void
jsX3DExecutionContext::initObject (JSContext* context, JSObject* object)
{
	JS_DefineProperties (context, object, properties);
	JS_DefineFunctions (context, object, functions);
}

JSBool
jsX3DExecutionContext::create (JSContext* context, X3DExecutionContext* executionContext, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	JS_SetPrivate (context, result, executionContext);

	initObject (context, result);

	if (seal)
		JS_SealObject (context, result, JS_FALSE);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

JSBool
jsX3DExecutionContext::getRootNodes (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DExecutionContext* executionContext = (X3DExecutionContext*) JS_GetPrivate (context, obj);

	return jsMFNode::create (context, executionContext -> getRootNodes (), vp, not dynamic_cast <Scene*> (executionContext));
}

JSBool
jsX3DExecutionContext::setRootNodes (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	X3DExecutionContext* executionContext = (X3DExecutionContext*) JS_GetPrivate (context, obj);

	if (not dynamic_cast <Scene*> (executionContext))
		return JS_TRUE;

	JSObject* obj2;

	if (not JS_ValueToObject (context, *vp, &obj2))
		return JS_FALSE;

	if (JS_GetClass (context, obj2) not_eq jsMFNode::getClass ())
	{
		JS_ReportError (context, "Type of argument is invalid - should be MFNode, is %s", JS_GetClass (context, obj2) -> name);
		return JS_FALSE;
	}

	MFNode* mfnode = (MFNode*) JS_GetPrivate (context, obj2);

	*executionContext -> getRootNodes () = *mfnode;

	return JS_TRUE;
}

JSBool
jsX3DExecutionContext::createNode (JSContext* context, JSObject* obj, uintN argc, jsval* vp)
{
	if (argc == 1)
	{
		try
		{
			char* name;

			if (not JS_ConvertArguments (context, argc, argv, "s", &name))
				return JS_FALSE;

			X3DExecutionContext* executionContext = (X3DExecutionContext*) JS_GetPrivate (context, obj);

			SFNode* node = new SFNode (executionContext -> createNode (name));

			if (*node)
				node -> getValue () -> initialize ();

			return jsSFNode::create (context, node, rval);
		}
		catch (const Error & exception)
		{
			JS_ReportError (context, exception .what ());
			return JS_FALSE;
		}
	}

	JS_ReportError (context, "wrong number of arguments");

	return JS_FALSE;
}

} // X3D
} // titania

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

#include "jsX3DRoute.h"

#include "../../Components/Core/X3DPrototypeInstance.h"
#include "Fields/jsSFNode.h"
#include "jsContext.h"
#include "jsString.h"

namespace titania {
namespace X3D {

JSClass jsX3DRoute::static_class = {
	"X3DRoute", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DRoute::properties [ ] = {
	{ "sourceNode",       SOURCE_NODE,       JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, sourceNode,       NULL },
	{ "sourceField",      SOURCE_FIELD,      JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, sourceField,      NULL },
	{ "destinationNode",  DESTINATION_NODE,  JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, destinationNode,  NULL },
	{ "destinationField", DESTINATION_FIELD, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE, destinationField, NULL },
	{ 0 }

};

void
jsX3DRoute::init (JSContext* context, JSObject* global)
{
	JS_InitClass (context, global, NULL, &static_class, NULL,
	              0, properties, NULL, NULL, NULL);
}

JSBool
jsX3DRoute::create (JSContext* context, const X3DSFNode <Route> & route, jsval* vp, const bool seal)
{
	JSObject* result = JS_NewObject (context, &static_class, NULL, NULL);

	if (result == NULL)
		return JS_FALSE;

	auto field = new X3DSFNode <Route> (route);

	JS_SetPrivate (context, result, field);

	//if (seal)
	//	JS_SealObject (context, result, JS_FALSE);

	static_cast <jsContext*> (JS_GetContextPrivate (context)) -> addObject (field, result);

	*vp = OBJECT_TO_JSVAL (result);

	return JS_TRUE;
}

// Properties

JSBool
jsX3DRoute::sourceNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & route = *static_cast <X3DSFNode <Route>*> (JS_GetPrivate (context, obj));

	return jsSFNode::create (context, new SFNode (route -> getSourceNode ()), vp);
}

JSBool
jsX3DRoute::sourceField (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & route = *static_cast <X3DSFNode <Route>*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, route -> getSourceField (), vp);
}

JSBool
jsX3DRoute::destinationNode (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & route = *static_cast <X3DSFNode <Route>*> (JS_GetPrivate (context, obj));

	return jsSFNode::create (context, new SFNode (route -> getDestinationNode ()), vp);
}

JSBool
jsX3DRoute::destinationField (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	auto & route = *static_cast <X3DSFNode <Route>*> (JS_GetPrivate (context, obj));

	return JS_NewStringValue (context, route -> getDestinationField (), vp);
}

void
jsX3DRoute:: finalize (JSContext* context, JSObject* obj)
{
	auto route = static_cast <X3DSFNode <Route>*> (JS_GetPrivate (context, obj));

	if (route)
		static_cast <jsContext*> (JS_GetContextPrivate (context)) -> removeObject (route);
}

} // X3D
} // titania

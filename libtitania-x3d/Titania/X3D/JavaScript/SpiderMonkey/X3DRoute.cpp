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

#include "X3DRoute.h"

#include "Context.h"
#include "Error.h"
#include "Fields/SFNode.h"
#include "SlotType.h"
#include "String.h"

#include "../../Routing/Route.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DRoute::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass X3DRoute::static_class = {
	"X3DRoute",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec X3DRoute::properties [ ] = {
	JS_PSGS ("sourceNode",       getSourceNode,       nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("sourceField",      getSourceField,      nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("destinationNode",  getDestinationNode,  nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("destinationField", getDestinationField, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

const JSFunctionSpec X3DRoute::functions [ ] = {
	JS_FS_END
};

JSObject*
X3DRoute::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

bool
X3DRoute::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

JS::Value
X3DRoute::create (JSContext* const cx, const X3D::RoutePtr & route)
{
	const auto context = getContext (cx);
	const auto obj     = JS_NewObjectWithGivenProto (cx, getClass (), context -> getProto (getId ()));

	if (not obj)
		throw std::runtime_error ("out of memory");

	const auto self = new X3D::RoutePtr (route);

	setObject (obj, self);
	setContext (obj, context);

	self -> addParent (context);

	return JS::ObjectValue (*obj);
}

// Properties

bool
X3DRoute::getSourceNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args       = JS::CallArgsFromVp (argc, vp);
		const auto & self       = *getThis <X3DRoute> (cx, args);
		auto         sourceNode = self -> getSourceNode ();

		args .rval () .set (SFNode::create (cx, &sourceNode));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .sourceNode: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DRoute::getSourceField (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args = JS::CallArgsFromVp (argc, vp);
		const auto & self = *getThis <X3DRoute> (cx, args);

		args .rval () .set (StringValue (cx, self -> getSourceField ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .sourceField: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DRoute::getDestinationNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args            = JS::CallArgsFromVp (argc, vp);
		const auto & self            = *getThis <X3DRoute> (cx, args);
		auto         destinationNode = self -> getDestinationNode ();

		args .rval () .set (SFNode::create (cx, &destinationNode));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .destinationNode: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DRoute::getDestinationField (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args = JS::CallArgsFromVp (argc, vp);
		const auto & self = *getThis <X3DRoute> (cx, args);

		args .rval () .set (StringValue (cx, self -> getDestinationField ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .destinationField: %s.", getClass () -> name, error .what ());
	}
}

void
X3DRoute::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = getContext (obj);
	const auto self    = getObject <X3D::FieldPtr*> (obj);

	// Proto objects have no private

	if (self)
		self -> removeParent (context);
}

} // spidermonkey
} // X3D
} // titania

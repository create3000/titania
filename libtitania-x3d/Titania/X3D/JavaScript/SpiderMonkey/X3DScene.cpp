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

#include "X3DScene.h"

#include "Context.h"
#include "Error.h"
#include "Fields/MFNode.h"
#include "Fields/SFNode.h"
#include "SlotType.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DScene::class_ops = {
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

const JSClass X3DScene::static_class = {
	"X3DScene",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec X3DScene::properties [ ] = {
	JS_PSGS ("rootNodes", getRootNodes, setRootNodes, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

const JSFunctionSpec X3DScene::functions [ ] = {
	JS_FS ("setMetaData",        setMetaData,        2, JSPROP_PERMANENT),
	JS_FS ("getMetaData",        getMetaData,        1, JSPROP_PERMANENT),
	JS_FS ("addExportedNode",    addExportedNode,    2, JSPROP_PERMANENT),
	JS_FS ("removeExportedNode", removeExportedNode, 1, JSPROP_PERMANENT),
	JS_FS ("updateExportedNode", updateExportedNode, 2, JSPROP_PERMANENT),
	JS_FS ("getExportedNode",    getExportedNode,    1, JSPROP_PERMANENT),
	JS_FS ("addRootNode",        addRootNode,        1, JSPROP_PERMANENT),
	JS_FS ("removeRootNode",     removeRootNode,     1, JSPROP_PERMANENT),
	JS_FS ("setRootNodes",       setRootNodes,       1, JSPROP_PERMANENT),
	JS_FS ("getRootNodes",       getRootNodes,       0, JSPROP_PERMANENT),
	JS_FS_END
};

JSObject*
X3DScene::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

bool
X3DScene::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

JS::Value
X3DScene::create (JSContext* const cx, X3D::X3DScene* const scene)
{
	const auto context = getContext (cx);
	const auto key     = size_t (scene);
	const auto obj     = context -> getObject (key);

	if (obj)
	{
		return JS::ObjectValue (*obj);
	}
	else
	{
		const auto obj = JS_NewObjectWithGivenProto (cx, getClass (), context -> getProto (getId ()));
	
		if (not obj)
			throw std::runtime_error ("out of memory");
	
		const auto self = new internal_type (scene);
	
		setObject (obj, self);
		setContext (obj, context);
		setKey (obj, key);
	
		context -> addObject (key, self, obj);
	
		return JS::ObjectValue (*obj);
	}
}

bool
X3DScene::setRootNodes (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .rootNodes: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto scene = getExecutionContext <X3DScene> (cx, args);
		const auto value = getArgument <MFNode> (cx, args, 0);

		scene -> getRootNodes () = *value;

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .rootNodes: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::getRootNodes (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .rootNodes: wrong number of arguments.", getClass () -> name);
	
		// Differs from X3DExecutionContext rootNodes!!!

		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto scene = getExecutionContext <X3DScene> (cx, args);

		args .rval () .set (MFNode::create (cx, &scene -> getRootNodes ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .rootNodes: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::setMetaData (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .setMetaData: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto scene = getExecutionContext <X3DScene> (cx, args);
		const auto key   = getArgument <std::string> (cx, args, 0);
		const auto value = getArgument <std::string> (cx, args, 1);

		scene -> setMetaData (key, value);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .setMetaData: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::getMetaData (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getMetaData: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto scene = getExecutionContext <X3DScene> (cx, args);
		const auto key   = getArgument <std::string> (cx, args, 0);

		args .rval () .set (StringValue (cx, scene -> getMetaData (key)));
		return true;
	}
	catch (const Error <X3D::INVALID_NAME> &)
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getMetaData: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::addExportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .addExportedNode: wrong number of arguments.", getClass () -> name);
	
		const auto args         = JS::CallArgsFromVp (argc, vp);
		const auto scene        = getExecutionContext <X3DScene> (cx, args);
		const auto exportedName = getArgument <std::string> (cx, args, 0);
		const auto node         = getArgument <SFNode> (cx, args, 1);

		scene -> addExportedNode (exportedName, *node);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .addExportedNode: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::removeExportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .removeExportedNode: wrong number of arguments.", getClass () -> name);
	
		const auto args         = JS::CallArgsFromVp (argc, vp);
		const auto scene        = getExecutionContext <X3DScene> (cx, args);
		const auto exportedName = getArgument <std::string> (cx, args, 0);

		scene -> removeImportedNode (exportedName);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .removeExportedNode: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::updateExportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .updateExportedNode: wrong number of arguments.", getClass () -> name);
	
		const auto args         = JS::CallArgsFromVp (argc, vp);
		const auto scene        = getExecutionContext <X3DScene> (cx, args);
		const auto exportedName = getArgument <std::string> (cx, args, 0);
		const auto node         = getArgument <SFNode> (cx, args, 1);

		scene -> updateExportedNode (exportedName, *node);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .updateExportedNode: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::getExportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getExportedNode: wrong number of arguments.", getClass () -> name);
	
		const auto args         = JS::CallArgsFromVp (argc, vp);
		const auto scene        = getExecutionContext <X3DScene> (cx, args);
		const auto exportedName = getArgument <std::string> (cx, args, 0);
		auto       exportedNode = scene -> getExportedNode (exportedName);

		args .rval () .set (SFNode::create (cx, &exportedNode));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getExportedNode: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::addRootNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .addRootNode: wrong number of arguments.", getClass () -> name);
	
		const auto args     = JS::CallArgsFromVp (argc, vp);
		const auto scene    = getExecutionContext <X3DScene> (cx, args);
		const auto rootNode = getArgument <SFNode> (cx, args, 0);

		scene -> addRootNode (*rootNode);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .addRootNode: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DScene::removeRootNode (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .removeRootNode: wrong number of arguments.", getClass () -> name);
	
		const auto args     = JS::CallArgsFromVp (argc, vp);
		const auto scene    = getExecutionContext <X3DScene> (cx, args);
		const auto rootNode = getArgument <SFNode> (cx, args, 0);

		scene -> removeRootNode (*rootNode);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .removeRootNode: %s.", getClass () -> name, error .what ());
	}
}

// Destruction

void
X3DScene::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = getContext (obj);
	const auto self    = getObject <internal_type*> (obj);

	// Proto objects have no private.

	if (self)
		context -> removeObject (getKey (obj));
}

} // spidermonkey
} // X3D
} // titania

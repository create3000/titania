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

#include "Fields/MFNode.h"
#include "Fields/SFNode.h"
#include "Context.h"
#include "Error.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSClass X3DScene::static_class = {
	"X3DScene", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec X3DScene::properties [ ] = {
	{ "rootNodes", ROOT_NODES, JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT, rootNodes, rootNodes },
	{ 0 }
};

JSFunctionSpec X3DScene::functions [ ] = {
	{ "fromUnit", fromUnit, 2, 0 },
	{ "toUnit",   toUnit,   2, 0 },

	{ "setMetaData", setMetaData, 2, 0 },
	{ "getMetaData", getMetaData, 1, 0 },

	{ "addExportedNode",    addExportedNode,    2, 0 },
	{ "removeExportedNode", removeExportedNode, 1, 0 },
	{ "updateExportedNode", updateExportedNode, 2, 0 },
	{ "getExportedNode",    getExportedNode,    1, 0 },

	{ "addRootNode",    addRootNode,    1, 0 },
	{ "removeRootNode", removeRootNode, 1, 0 },
	{ "setRootNodes",   setRootNodes,   1, 0 },
	{ "getRootNodes",   getRootNodes,   0, 0 },

	{ "toVRMLString", toVRMLString, 0, 0 },
	{ "toXMLString",  toXMLString,  0, 0 },

	{ 0 }

};

JSObject*
X3DScene::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JSBool
X3DScene::create (JSContext* const cx, X3D::X3DScene* const scene, jsval* const vp)
{
	const auto result = JS_NewObject (cx, &static_class, nullptr, nullptr);

	if (result == nullptr)
		return ThrowException (cx, "out of memory");

	const auto context = getContext (cx);

	if (scene not_eq context -> getExecutionContext ())
		scene -> addParent (context);

	JS_SetPrivate (cx, result, scene);

	*vp = OBJECT_TO_JSVAL (result);

	return true;
}

JSBool
X3DScene::rootNodes (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto scene = getThis <X3DScene> (cx, obj);
		const auto value = getArgument <MFNode> (cx, vp, 0);

		scene -> getRootNodes () = *value;

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .rootNodes: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::rootNodes (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		// Differs from X3DExecutionContext rootNodes!!!

		const auto scene = getThis <X3DScene> (cx, obj);

		return X3DField::get <MFNode> (cx, scene -> getRootNodes (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .rootNodes: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::fromUnit (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .fromUnit: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto scene        = getThis <X3DScene> (cx, vp);
		const auto unitCategory = getArgument <int32_t> (cx, argv, 0);
		const auto value        = getArgument <double> (cx, argv, 1);

		if (unitCategories .count (unitCategory) == 0)
			return ThrowException (cx, "%s .fromUnit: unknown base unit.", getClass () -> name);

		return JS_NewNumberValue (cx, scene -> toUnit (X3D::UnitCategory (unitCategory), value), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .fromUnit: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::toUnit (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .toUnit: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto scene        = getThis <X3DScene> (cx, vp);
		const auto unitCategory = getArgument <int32_t> (cx, argv, 0);
		const auto value        = getArgument <double> (cx, argv, 1);

		if (unitCategories .count (unitCategory) == 0)
			return ThrowException (cx, "%s .toUnit: unknown base unit.", getClass () -> name);

		return JS_NewNumberValue (cx, scene -> toUnit (X3D::UnitCategory (unitCategory), value), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toUnit: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::setMetaData (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .setMetaData: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv  = JS_ARGV (cx, vp);
		const auto scene = getThis <X3DScene> (cx, vp);
		const auto key   = getArgument <std::string> (cx, argv, 0);
		const auto value = getArgument <std::string> (cx, argv, 1);

		scene -> setMetaData (key, value);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setMetaData: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::getMetaData (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getMetaData: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv  = JS_ARGV (cx, vp);
		const auto scene = getThis <X3DScene> (cx, vp);
		const auto key   = getArgument <std::string> (cx, argv, 0);

		return JS_NewStringValue (cx, scene -> getMetaData (key), &JS_RVAL (cx, vp));
	}
	catch (const Error <X3D::INVALID_NAME> &)
	{
		return JS_NewStringValue (cx, "", &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getMetaData: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::addExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .addExportedNode: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto scene        = getThis <X3DScene> (cx, vp);
		const auto exportedName = getArgument <std::string> (cx, argv, 0);
		const auto node         = getArgument <SFNode> (cx, argv, 1);

		scene -> addExportedNode (exportedName, *node);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .addExportedNode: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::removeExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .removeExportedNode: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto scene        = getThis <X3DScene> (cx, vp);
		const auto exportedName = getArgument <std::string> (cx, argv, 0);

		scene -> removeImportedNode (exportedName);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .removeExportedNode: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::updateExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .updateExportedNode: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto scene        = getThis <X3DScene> (cx, vp);
		const auto exportedName = getArgument <std::string> (cx, argv, 0);
		const auto node         = getArgument <SFNode> (cx, argv, 1);

		scene -> updateExportedNode (exportedName, *node);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .updateExportedNode: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::getExportedNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .getExportedNode: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv         = JS_ARGV (cx, vp);
		const auto scene        = getThis <X3DScene> (cx, vp);
		const auto exportedName = getArgument <std::string> (cx, argv, 0);
		auto       exportedNode = scene -> getExportedNode (exportedName);

		return X3DField::get <SFNode> (cx, exportedNode, &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getExportedNode: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::addRootNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .addRootNode: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv     = JS_ARGV (cx, vp);
		const auto scene    = getThis <X3DScene> (cx, vp);
		const auto rootNode = getArgument <SFNode> (cx, argv, 0);

		scene -> addRootNode (*rootNode);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .addRootNode: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::removeRootNode (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .removeRootNode: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv     = JS_ARGV (cx, vp);
		const auto scene    = getThis <X3DScene> (cx, vp);
		const auto rootNode = getArgument <SFNode> (cx, argv, 0);

		scene -> removeRootNode (*rootNode);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .removeRootNode: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::setRootNodes (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .setRootNodes: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv  = JS_ARGV (cx, vp);
		const auto scene = getThis <X3DScene> (cx, vp);
		const auto value = getArgument <MFNode> (cx, argv, 0);

		scene -> setRootNodes (*value);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setRootNodes: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::getRootNodes (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getRootNodes: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto scene = getThis <X3DScene> (cx, vp);

		return X3DField::get <MFNode> (cx, scene -> getRootNodes (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getRootNodes: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::toVRMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toVRMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto scene = getThis <X3DScene> (cx, vp);

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		Generator::NicestStyle (osstream);

		scene -> toStream (osstream);

		return JS_NewStringValue (cx, osstream .str (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toVRMLString: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DScene::toXMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toXMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto scene = getThis <X3DScene> (cx, vp);

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		Generator::NicestStyle (osstream);

		scene -> toXMLStream (osstream);

		return JS_NewStringValue (cx, osstream .str (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toXMLString: %s.", getClass () -> name, error .what ());
	}
}

void
X3DScene::finalize (JSContext* cx, JSObject* obj)
{
	const auto context = getContext (cx);
	const auto scene   = getObject <X3D::X3DScene*> (cx, obj);

	if (scene)
	{
		if (scene not_eq context -> getExecutionContext ())
			scene -> removeParent (context);
	}
}

} // spidermonkey
} // X3D
} // titania

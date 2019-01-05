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

#include "X3DExecutionContext.h"

#include "Arguments.h"
#include "ComponentInfoArray.h"
#include "Error.h"
#include "ExternProtoDeclarationArray.h"
#include "Fields/MFNode.h"
#include "Fields/SFNode.h"
#include "ProfileInfo.h"
#include "ProtoDeclarationArray.h"
#include "RouteArray.h"
#include "SlotType.h"
#include "String.h"

#include "../../Bits/Cast.h"
#include "../../Bits/Error.h"
#include "../../Components/Core/X3DPrototypeInstance.h"
#include "../../Components/Networking/Inline.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DExecutionContext::class_ops = {
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

const JSClass X3DExecutionContext::static_class = {
	"X3DExecutionContext",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec X3DExecutionContext::properties [ ] = {
	JS_PSGS ("specificationVersion", getSpecificationVersion, nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("encoding",             getEncoding,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("worldURL",             getWorldURL,             nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//
//	JS_PSGS ("profile",              getProfile,              nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("components",           getComponents,           nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//
//	JS_PSGS ("externprotos",         getExternprotos,         nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("protos",               getProtos,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("rootNodes",            getRootNodes,            nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
//	JS_PSGS ("routes",               getRoutes,               nullptr, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

const JSFunctionSpec X3DExecutionContext::functions [ ] = {
//	JS_FS ("fromUnit",           fromUnit,           2, JSPROP_PERMANENT),
//	JS_FS ("toUnit",             toUnit,             2, JSPROP_PERMANENT),
//	JS_FS ("createNode",         createNode,         1, JSPROP_PERMANENT),
//	JS_FS ("createProto",        createProto,        1, JSPROP_PERMANENT),
//	JS_FS ("addNamedNode",       addNamedNode,       2, JSPROP_PERMANENT),
//	JS_FS ("removeNamedNode",    removeNamedNode,    1, JSPROP_PERMANENT),
//	JS_FS ("updateNamedNode",    updateNamedNode,    2, JSPROP_PERMANENT),
//	JS_FS ("getNamedNode",       getNamedNode,       1, JSPROP_PERMANENT),
//	JS_FS ("addImportedNode",    addImportedNode,    2, JSPROP_PERMANENT),
//	JS_FS ("removeImportedNode", removeImportedNode, 1, JSPROP_PERMANENT),
//	JS_FS ("updateImportedNode", updateImportedNode, 2, JSPROP_PERMANENT),
//	JS_FS ("getImportedNode",    getImportedNode,    1, JSPROP_PERMANENT),
//	JS_FS ("getRootNodes",       getRootNodes,       0, JSPROP_PERMANENT),
//	JS_FS ("addRoute",           addRoute,           4, JSPROP_PERMANENT),
//	JS_FS ("deleteRoute",        deleteRoute,        1, JSPROP_PERMANENT),
//	JS_FS ("toVRMLString",       toVRMLString,       0, JSPROP_PERMANENT),
//	JS_FS ("toXMLString",        toXMLString,        0, JSPROP_PERMANENT),
	JS_FS_END
};

const std::set <int32_t> X3DExecutionContext::unitCategories = {
	int32_t (X3D::UnitCategory::ANGLE),
	int32_t (X3D::UnitCategory::FORCE),
	int32_t (X3D::UnitCategory::LENGTH),
	int32_t (X3D::UnitCategory::MASS),
};

JSObject*
X3DExecutionContext::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

bool
X3DExecutionContext::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

JS::Value
X3DExecutionContext::create (JSContext* const cx, X3D::X3DExecutionContext* const executionContext)
{
	const auto context = getContext (cx);
	const auto key     = size_t (executionContext);
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
	
		const auto self = new internal_type (executionContext);
	
		setObject (obj, self);
		setContext (obj, context);
		setKey (obj, key);
		setExecutionContext <X3DExecutionContext> (obj, executionContext);

		context -> addObject (key, self, obj);
	
		return JS::ObjectValue (*obj);
	}
}

// Properties

bool
X3DExecutionContext::getSpecificationVersion (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args             = JS::CallArgsFromVp (argc, vp);
		const auto executionContext = getExecutionContext <X3DExecutionContext> (cx, args);

		args .rval () .set (StringValue (cx, XMLEncode (executionContext -> getSpecificationVersion ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .specificationVersion: %s.", getClass () -> name, error .what ());
	}
}

//bool
//X3DExecutionContext::encoding (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//
//		return JS_NewStringValue (cx, to_string (executionContext -> getEncoding ()), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .encoding: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::worldURL (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//
//		return JS_NewStringValue (cx, executionContext -> getWorldURL (), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .worldURL: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::profile (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//		const auto profile          = executionContext -> getProfile ();
//
//		if (profile)
//			return ProfileInfo::create (cx, profile, vp);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .profile: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::components (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//
//		return ComponentInfoArray::create (cx, &executionContext -> getComponents (), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .components: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::externprotos (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//
//		return ExternProtoDeclarationArray::create (cx, &executionContext -> getExternProtoDeclarations (), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .externprotos: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::protos (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//
//		return ProtoDeclarationArray::create (cx, &executionContext -> getProtoDeclarations (), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .rootNodes: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::rootNodes (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//		const auto rootNodes        = new X3D::MFNode (executionContext -> getRootNodes ());
//
//		rootNodes -> setName ("rootNodes");
//		rootNodes -> setAccessType (initializeOnly);
//
//		return X3DField::create <MFNode> (cx, rootNodes, vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .rootNodes: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::routes (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, obj);
//
//		return RouteArray::create (cx, &executionContext -> getRoutes (), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .routes: %s.", getClass () -> name, error .what ());
//	}
//}
//
//// Functions
//
//bool
//X3DExecutionContext::fromUnit (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 2)
//		return ThrowException <JSProto_Error> (cx, "%s .fromUnit: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto unitCategory     = getArgument <int32_t> (cx, argv, 0);
//		const auto value            = getArgument <double> (cx, argv, 1);
//
//		if (unitCategories .count (unitCategory) == 0)
//			return ThrowException <JSProto_Error> (cx, "%s .fromUnit: unknown base unit.", getClass () -> name);
//
//		return JS_NewNumberValue (cx, executionContext -> toUnit (X3D::UnitCategory (unitCategory), value), &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .fromUnit: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::toUnit (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 2)
//		return ThrowException <JSProto_Error> (cx, "%s .toUnit: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto unitCategory     = getArgument <int32_t> (cx, argv, 0);
//		const auto value            = getArgument <double> (cx, argv, 1);
//
//		if (unitCategories .count (unitCategory) == 0)
//			return ThrowException <JSProto_Error> (cx, "%s .toUnit: unknown base unit.", getClass () -> name);
//
//		return JS_NewNumberValue (cx, executionContext -> toUnit (X3D::UnitCategory (unitCategory), value), &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .toUnit: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::createNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 1)
//		return ThrowException <JSProto_Error> (cx, "%s .createNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto name             = getArgument <std::string> (cx, argv, 0);
//		auto node                   = executionContext -> createNode (name);
//
//		return X3DField::get <SFNode> (cx, node, &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .createNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::createProto (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 1)
//		return ThrowException <JSProto_Error> (cx, "%s .createProto: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto  argv             = JS_ARGV (cx, vp);
//		const auto  executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto  name             = getArgument <std::string> (cx, argv, 0);
//		auto        node             = X3D::SFNode (executionContext -> createProto (name));
//
//		return X3DField::get <SFNode> (cx, node, &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .createProto: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::addNamedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 2)
//		return ThrowException <JSProto_Error> (cx, "%s .addNamedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto name             = getArgument <std::string> (cx, argv, 0);
//		const auto node             = getArgument <SFNode> (cx, argv, 1);
//
//		executionContext -> addNamedNode (name, *node);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .addNamedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::removeNamedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 1)
//		return ThrowException <JSProto_Error> (cx, "%s .removeNamedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto name             = getArgument <std::string> (cx, argv, 0);
//
//		executionContext -> removeNamedNode (name);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .removeNamedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::updateNamedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 2)
//		return ThrowException <JSProto_Error> (cx, "%s .updateNamedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto name             = getArgument <std::string> (cx, argv, 0);
//		const auto node             = getArgument <SFNode> (cx, argv, 1);
//
//		executionContext -> updateNamedNode (name, *node);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .updateNamedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::getNamedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 1)
//		return ThrowException <JSProto_Error> (cx, "%s .getNamedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto name             = getArgument <std::string> (cx, argv, 0);
//		auto namedNode              = executionContext -> getNamedNode (name);
//
//		return X3DField::get <SFNode> (cx, namedNode, &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .getNamedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::addImportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 2 and argc not_eq 3)
//		return ThrowException <JSProto_Error> (cx, "%s .addImportedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto node             = getArgument <SFNode> (cx, argv, 0);
//		const auto inlineNode       = X3D::X3DPtr <X3D::Inline> (*node);
//		const auto exportedName     = getArgument <std::string> (cx, argv, 1);
//
//		if (argc == 3)
//			executionContext -> addImportedNode (inlineNode, exportedName, getArgument <std::string> (cx, argv, 2));
//
//		else
//			executionContext -> addImportedNode (inlineNode, exportedName);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .addImportedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::removeImportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 1)
//		return ThrowException <JSProto_Error> (cx, "%s .removeImportedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto importedName     = getArgument <std::string> (cx, argv, 0);
//
//		executionContext -> removeImportedNode (importedName);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .removeImportedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::updateImportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 2 and argc not_eq 3)
//		return ThrowException <JSProto_Error> (cx, "%s .updateImportedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto node             = getArgument <SFNode> (cx, argv, 0);
//		const auto inlineNode       = X3D::X3DPtr <X3D::Inline> (*node);
//		const auto exportedName     = getArgument <std::string> (cx, argv, 1);
//
//		if (argc == 3)
//			executionContext -> updateImportedNode (inlineNode, exportedName, getArgument <std::string> (cx, argv, 2));
//
//		else
//			executionContext -> updateImportedNode (inlineNode, exportedName);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .updateImportedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::getImportedNode (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 1)
//		return ThrowException <JSProto_Error> (cx, "%s .getImportedNode: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto importedName     = getArgument <std::string> (cx, argv, 0);
//		auto       importedNode     = executionContext -> getImportedNode (importedName);
//
//		return X3DField::get <SFNode> (cx, importedNode, &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .getImportedNode: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::getRootNodes (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .getRootNodes: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto rootNodes        = new X3D::MFNode (executionContext -> getRootNodes ());
//
//		rootNodes -> setName ("rootNodes");
//		rootNodes -> setAccessType (initializeOnly);
//
//		return X3DField::create <MFNode> (cx, rootNodes, &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .getRootNodes: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::addRoute (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 4)
//		return ThrowException <JSProto_Error> (cx, "%s .addRoute: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto   argv             = JS_ARGV (cx, vp);
//		const auto   executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto   fromNode         = getArgument <SFNode> (cx, argv, 0);
//		const auto   fromEventOut     = getArgument <std::string> (cx, argv, 1);
//		const auto   toNode           = getArgument <SFNode> (cx, argv, 2);
//		const auto   toEventIn        = getArgument <std::string> (cx, argv, 3);
//		const auto & route            = executionContext -> addRoute (*fromNode, fromEventOut, *toNode, toEventIn);
//
//		return X3DRoute::create (cx, route, &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .addRoute: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::deleteRoute (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 1)
//		return ThrowException <JSProto_Error> (cx, "%s .deleteRoute: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv             = JS_ARGV (cx, vp);
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//		const auto route            = getArgument <X3DRoute> (cx, argv, 0);
//
//		executionContext -> deleteRoute (*route);
//
//		JS_SET_RVAL (cx, vp, JSVAL_VOID);
//		return true;
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .deleteRoute: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::toVRMLString (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .toVRMLString: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//
//		std::ostringstream osstream;
//
//		osstream .imbue (std::locale::classic ());
//
//		Generator::NicestStyle (osstream);
//
//		executionContext -> X3D::X3DExecutionContext::toStream (osstream);
//
//		return JS_NewStringValue (cx, osstream .str (), &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .toVRMLString: %s.", getClass () -> name, error .what ());
//	}
//}
//
//bool
//X3DExecutionContext::toXMLString (JSContext* cx, unsigned argc, JS::Value* vp)
//{
//	if (argc not_eq 0)
//		return ThrowException <JSProto_Error> (cx, "%s .toXMLString: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto executionContext = getThis <X3DExecutionContext> (cx, vp);
//
//		std::ostringstream osstream;
//
//		osstream .imbue (std::locale::classic ());
//
//		Generator::NicestStyle (osstream);
//
//		executionContext -> X3D::X3DExecutionContext::toXMLStream (osstream);
//
//		return JS_NewStringValue (cx, osstream .str (), &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException <JSProto_Error> (cx, "%s .toXMLString: %s.", getClass () -> name, error .what ());
//	}
//}

// Destruction

void
X3DExecutionContext::finalize (JSFreeOp* fop, JSObject* obj)
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

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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "jsSFNode.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Scripting/Script.h"
#include "../../../Execution/X3DScene.h"
#include "../../../InputOutput/Loader.h"
#include "../jsContext.h"
//#include "../jsFieldDefinitionArray.h"
#include "../jsString.h"
#include "../jsfield.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsSFNode::static_class = {
	"SFNode",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	getProperty,
	setProperty,
	(JSEnumerateOp) enumerate,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSFunctionSpec jsSFNode::functions [ ] = {
	{ "getNodeName",         getNodeName,         0, 0 },
	{ "getNodeType",         getNodeType,         0, 0 },
	{ "getFieldDefinitions", getFieldDefinitions, 0, 0 },

	{ "toVRMLString",        toVRMLString,        0, 0 },
	{ "toXMLString",         toXMLString,         0, 0 },
	{ "toString",            toString,            0, 0 },

	{ 0 }

};

JSObject*
jsSFNode::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, nullptr, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JS::Value
jsSFNode::create (JSContext* const cx, X3D::SFNode* const field)
throw (std::invalid_argument)
{
	return jsX3DField::create (cx, &static_class, field);
}

JSBool
jsSFNode::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new X3D::SFNode ()));
				return true;
			}
			case 1:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto vrmlSyntax = getArgument <std::string> (cx, args, 0);
				const auto script     = getContext (cx) -> getScriptNode ();
				const auto scene      = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);
				const auto result     = scene -> getRootNodes () .empty () ? new SFNode () : new SFNode (scene -> getRootNodes () [0]);

				args .rval () .set (create (cx, result));
				return true;
			}
			default:
				return ThrowException (cx, "%s .new: wrong number of arguments.", getClass () -> name);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::enumerate (JSContext* cx, JS::HandleObject obj, JSIterateOp enum_op, JS::MutableHandleValue statep, JS::MutableHandleId idp)
{
	try
	{
		const auto lhs  = getThis <jsSFNode> (cx, obj);
		const auto node = lhs -> getValue ();

		if (not node or node -> getFieldDefinitions () .empty ())
		{
			statep .setNull ();
			return true;
		}

		size_t* index = nullptr;

		switch (enum_op)
		{
			case JSENUMERATE_INIT:
			case JSENUMERATE_INIT_ALL:
			{
				index = new size_t (0);
				statep .set (PRIVATE_TO_JSVAL (index));
				idp .set (INT_TO_JSID (node -> getFieldDefinitions () .size ()));
				break;
			}
			case JSENUMERATE_NEXT:
			{
				index = (size_t*) JSVAL_TO_PRIVATE (statep);

				if (*index < node -> getFieldDefinitions () .size ())
				{
					const auto field = node -> getFieldDefinitions () [*index];
					const auto name  = StringValue (cx, field -> getName ());
					
					jsid id;
					JS_ValueToId (cx, name, &id);

					idp .set (id);
					*index = *index + 1;
					break;
				}

				//else done -- cleanup.
			}
			case JSENUMERATE_DESTROY:
			{
				index = (size_t*) JSVAL_TO_PRIVATE (statep);
				delete index;
				statep .setNull ();
			}
		}

		return true;
	}
	catch (const std::exception &)
	{
		statep .setNull ();
		return true;
	}
}

JSBool
jsSFNode::setProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	if (not JSID_IS_STRING (id))
		return true;

	const auto name = to_string (cx, id);

	try
	{
		const auto lhs = getThis <jsSFNode> (cx, obj);

		if (not lhs -> getValue ())
			return true;

		try
		{
			const auto field = lhs -> getValue () -> getField (name);

			if (field -> getAccessType () == initializeOnly or field -> getAccessType () == outputOnly)
				return true;

			setValue (cx, field, vp);
			return true;
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .%s: %s.", getClass () -> name, name .c_str (), error .what ());
	}
}

JSBool
jsSFNode::getProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	if (not JSID_IS_STRING (id))
		return true;

	const auto name = to_string (cx, id);

	try
	{
		const auto lhs = getThis <jsSFNode> (cx, obj);

		if (not lhs -> getValue ())
			return true;

		try
		{
			const auto field = lhs -> getValue () -> getField (name);

			if (field -> getAccessType () == initializeOnly or field -> getAccessType () == inputOnly)
				return true;

			vp .set (getValue (cx, field));
			return true;
		}
		catch (const Error <INVALID_NAME> &)
		{
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .%s: %s.", getClass () -> name, name .c_str (), error .what ());
	}
}

JSBool
jsSFNode::getNodeName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getNodeName: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFNode> (cx, args);

		args .rval () .set (StringValue (cx, *lhs ? lhs -> getValue () -> getName () : ""));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getNodeName: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::getNodeType (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getNodeType: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto lhs    = getThis <jsSFNode> (cx, args);
		JSObject*  result = nullptr;

		if (lhs -> getValue ())
		{
			const auto node = lhs -> getValue ();

			std::vector <JS::Value> array (node -> getType () .size ());

			for (const auto & type : node -> getType ())
				array .emplace_back (JS::Int32Value (type));

			result = JS_NewArrayObject (cx, array .size (), array .data ());
		}
		else
			result = JS_NewArrayObject (cx, 0, nullptr);

		if (result == nullptr)
			return ThrowException (cx, "out of memory");

		args .rval () .setObjectOrNull (result);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getNodeType: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::getFieldDefinitions (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getFieldDefinitions: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFNode> (cx, args);

//		if (lhs -> getValue ())
//			args .rval () .set (jsFieldDefinitionArray::create (cx, &lhs -> getValue () -> getFieldDefinitions ()));
//		else
//			args .rval () .set (jsFieldDefinitionArray::create (cx, nullptr));

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getFieldDefinitions: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::toVRMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toVRMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto lhs     = getThis <jsSFNode> (cx, args);
		const auto context = getContext (cx);
		const auto version = context -> getExecutionContext () -> getSpecificationVersion ();

		Generator::SpecificationVersion (version);
		Generator::NicestStyle ();

		args .rval () .set (StringValue (cx, lhs -> toString ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toVRMLString: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::toXMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toXMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args    = JS::CallArgsFromVp (argc, vp);
		const auto lhs     = getThis <jsSFNode> (cx, args);
		const auto context = getContext (cx);
		auto       version = context -> getExecutionContext () -> getSpecificationVersion ();

		if (version == VRML_V2_0)
			version = LATEST_VERSION;

		Generator::SpecificationVersion (version);
		Generator::NicestStyle ();

		args .rval () .set (StringValue (cx, lhs -> toXMLString ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toXMLString: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::toString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFNode> (cx, args);

		if (lhs -> getValue ())
			args .rval () .set (StringValue (cx, lhs -> getValue () -> getTypeName () + " { }"));
		else
			args .rval () .set (StringValue (cx, "NULL"));

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toString: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

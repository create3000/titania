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
#include "../jsFieldDefinitionArray.h"
#include "../jsFields.h"
#include "../jsString.h"
#include "../jsfield.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsSFNode::static_class = {
	"SFNode", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, getProperty, setProperty,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
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

JSBool
jsSFNode::create (JSContext* const cx, SFNode* const field, jsval* const vp)
{
	return jsX3DField::create (cx, &static_class, field, vp);
}

JSBool
jsSFNode::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create (cx, new SFNode (), &JS_RVAL (cx, vp));
			}
			case 1:
			{
				try
				{
					const auto argv       = JS_ARGV (cx, vp);
					const auto vrmlSyntax = getArgument <std::string> (cx, argv, 0);
					const auto script     = getContext (cx) -> getScriptNode ();
					const auto scene      = Loader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);

					return create (cx,
					               scene -> getRootNodes () .empty ()
					               ? new SFNode ()
										: new SFNode (scene -> getRootNodes () [0]),
					               &JS_RVAL (cx, vp));
				}
				catch (const X3DError & error)
				{
					JS_ReportError (cx, error .what ());
					return false;
				}
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
jsSFNode::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	try
	{
		const auto lhs  = getThis <jsSFNode> (cx, obj);
		const auto node = lhs -> getValue ();

		if (not node or node -> getFieldDefinitions () .empty ())
		{
			*statep = JSVAL_NULL;
			return true;
		}

		size_t* index;

		switch (enum_op)
		{
			case JSENUMERATE_INIT:
			case JSENUMERATE_INIT_ALL:
			{
				index   = new size_t (0);
				*statep = PRIVATE_TO_JSVAL (index);

				if (idp)
					*idp = INT_TO_JSID (node -> getFieldDefinitions () .size ());

				break;
			}
			case JSENUMERATE_NEXT:
			{
				index = (size_t*) JSVAL_TO_PRIVATE (*statep);

				if (*index < node -> getFieldDefinitions () .size ())
				{
					const auto   field = node -> getFieldDefinitions () [*index];
					const auto & name  = field -> getName ();

					jsval id;
					JS_NewStringValue (cx, name, &id);

					if (idp)
						JS_ValueToId (cx, id, idp);

					*index = *index + 1;
					break;
				}

				//else done -- cleanup.
			}
			case JSENUMERATE_DESTROY:
			{
				index = (size_t*) JSVAL_TO_PRIVATE (*statep);
				delete index;
				*statep = JSVAL_NULL;
			}
		}

		return true;
	}
	catch (const std::exception &)
	{
		*statep = JSVAL_NULL;
		return true;	
	}
}

JSBool
jsSFNode::getProperty (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		if (not JSID_IS_STRING (id))
			return true;

		const auto lhs  = getThis <jsSFNode> (cx, obj);
		const auto name = to_string (cx, id);

		if (not lhs -> getValue ())
			return true;

		try
		{
			const auto field = lhs -> getValue () -> getField (name);

			if (field -> getAccessType () == initializeOnly or field -> getAccessType () == inputOnly)
			{
				*vp = JSVAL_VOID;
				return true;
			}

			return getValue (cx, field, vp);
		}
		catch (const Error <INVALID_NAME> &)
		{
			return true;
		}
	}
	catch (const std::exception & error)
	{
		const auto name = to_string (cx, id);

		return ThrowException (cx, "%s .%s: %s.", getClass () -> name, name .c_str (), error .what ());
	}
}

JSBool
jsSFNode::setProperty (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		if (not JSID_IS_STRING (id))
			return true;

		const auto lhs  = getThis <jsSFNode> (cx, obj);
		const auto name = to_string (cx, id);

		if (not lhs -> getValue ())
			return true;

		try
		{
			const auto field = lhs -> getValue () -> getField (name);

			if (field -> getAccessType () == initializeOnly or field -> getAccessType () == outputOnly)
				return true;

			if (not setValue (cx, field, vp))
				return ThrowException (cx, "out of memory");

			*vp = JSVAL_VOID;
			return true;
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			return true;
		}
	}
	catch (const std::exception & error)
	{
		const auto name = to_string (cx, id);

		return ThrowException (cx, "%s .%s: %s.", getClass () -> name, name .c_str (), error .what ());
	}
}

JSBool
jsSFNode::getNodeName (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getNodeName: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFNode> (cx, vp);

		return JS_NewStringValue (cx, *lhs ? lhs -> getValue () -> getName () : "", &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getNodeName: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::getNodeType (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getNodeType: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs    = getThis <jsSFNode> (cx, vp);
		JSObject*  result = nullptr;

		if (lhs -> getValue ())
		{
			const auto node = lhs -> getValue ();

			std::vector <jsval> array (node -> getType () .size ());

			for (size_t i = 0, size = node -> getType () .size (); i < size; ++ i)
			{
				if (not JS_NewNumberValue (cx, size_t (node -> getType () [i]), &array [i]))
					return false;
			}

			result = JS_NewArrayObject (cx, array .size (), array .data ());
		}
		else
			result = JS_NewArrayObject (cx, 0, nullptr);

		if (result == nullptr)
			return ThrowException (cx, "out of memory");

		JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (result));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getNodeType: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::getFieldDefinitions (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getFieldDefinitions: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFNode> (cx, vp);

		if (lhs -> getValue ())
			return jsFieldDefinitionArray::create (cx, &lhs -> getValue () -> getFieldDefinitions (), &JS_RVAL (cx, vp));

		return jsFieldDefinitionArray::create (cx, nullptr, &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getFieldDefinitions: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::toVRMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toVRMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto context = getContext (cx);
		const auto lhs     = getThis <jsSFNode> (cx, vp);
		const auto version = context -> getExecutionContext () -> getSpecificationVersion ();

		Generator::SpecificationVersion (version);
		Generator::NicestStyle ();

		return JS_NewStringValue (cx, lhs -> toString (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toVRMLString: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::toXMLString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toXMLString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto context = getContext (cx);
		const auto lhs     = getThis <jsSFNode> (cx, vp);
		auto       version = context -> getExecutionContext () -> getSpecificationVersion ();

		if (version == VRML_V2_0)
			version = LATEST_VERSION;

		Generator::SpecificationVersion (version);
		Generator::NicestStyle ();

		return JS_NewStringValue (cx, lhs -> toXMLString (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toXMLString: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFNode::toString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFNode> (cx, vp);

		if (lhs -> getValue ())
			return JS_NewStringValue (cx, lhs -> getValue () -> getTypeName () + " { }", &JS_RVAL (cx, vp));

		return JS_NewStringValue (cx, "NULL", &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toString: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

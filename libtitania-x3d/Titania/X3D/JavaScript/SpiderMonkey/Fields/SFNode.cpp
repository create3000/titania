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

#include "SFNode.h"

//#include "../FieldDefinitionArray.h"
#include "../String.h"
#include "../value.h"

#include "../../../Components/Scripting/Script.h"
#include "../../../InputOutput/FileLoader.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps SFNode::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize <SFNode>, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass SFNode::static_class = {
	"SFNode",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec SFNode::properties [ ] = {
	JS_PS_END
};

const JSFunctionSpec SFNode::functions [ ] = {
	JS_FS ("getNodeName",         getNodeName,         0, JSPROP_PERMANENT),
	JS_FS ("getNodeType",         getNodeType,         0, JSPROP_PERMANENT),
	JS_FS ("getFieldDefinitions", getFieldDefinitions, 0, JSPROP_PERMANENT),

	JS_FS ("toVRMLString",        toVRMLString,        0, JSPROP_PERMANENT),
	JS_FS ("toXMLString",         toXMLString,         0, JSPROP_PERMANENT),
	JS_FS ("toString",            toString,            0, JSPROP_PERMANENT),
	JS_FS_END
};

JSObject*
SFNode::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

JS::Value
SFNode::create (JSContext* const cx, X3D::SFNode* const field)
{
	if (field -> getValue ())
	{
		const auto script           = getContext (cx) -> getScriptNode ();
		const auto [value, created] = X3DField::create <SFNode> (cx, field);

		if (created and script -> directOutput ())
		{
			const auto object = JS::RootedObject (cx, value .toObjectOrNull ());

			for (const auto fieldDefinition : field -> getValue () -> getFieldDefinitions ())
			{
				const auto & name = fieldDefinition -> getName ();
		
				JS_DefineProperty (cx,
				                   object,
				                   name .c_str (),
				                   JS::UndefinedHandleValue,
				                   JSPROP_PROPOP_ACCESSORS | JSPROP_PERMANENT | (fieldDefinition -> isInitializable () ? JSPROP_ENUMERATE : 0),
				                   JS_PROPERTYOP_GETTER (&SFNode::getProperty),
				                   JS_PROPERTYOP_SETTER (&SFNode::setProperty));
	
				if (fieldDefinition -> getAccessType () == X3D::inputOutput)
				{
					JS_DefineProperty (cx,
					                   object,
					                   ("set_" + name) .c_str (),
					                   JS::UndefinedHandleValue,
					                   JSPROP_PROPOP_ACCESSORS | JSPROP_PERMANENT,
					                   JS_PROPERTYOP_GETTER (&SFNode::getProperty),
					                   JS_PROPERTYOP_SETTER (&SFNode::setProperty));
		
					JS_DefineProperty (cx,
					                   object,
					                   (name + "_changed") .c_str (),
					                   JS::UndefinedHandleValue,
					                   JSPROP_PROPOP_ACCESSORS | JSPROP_PERMANENT,
					                   JS_PROPERTYOP_GETTER (&SFNode::getProperty),
					                   JS_PROPERTYOP_SETTER (&SFNode::setProperty));
				}
			}
		}

		return value;
	}
	else
	{
		return JS::NullValue ();
	}
}

bool
SFNode::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .setNull ();
				return true;
			}
			case 1:
			{
				const auto args       = JS::CallArgsFromVp (argc, vp);
				const auto script     = getContext (cx) -> getScriptNode ();
				const auto vrmlSyntax = getArgument <std::string> (cx, args, 0);
				const auto scene      = X3D::FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);

				if (scene -> getRootNodes () .empty ())
					args .rval () .setNull ();
				else
					args .rval () .set (create (cx, &scene -> getRootNodes () [0]));

				return true;
			}
			default:
				return ThrowException <JSProto_Error> (cx, "new %s: wrong number of arguments.", getClass () -> name);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, error .what ());
	}
}

bool
SFNode::setProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp, JS::ObjectOpResult & result)
{
	try
	{
		try
		{
			const auto self  = getThis <SFNode> (cx, obj);
			const auto name  = to_string (cx, id);
			const auto field = self -> getValue () -> getField (name);

			if (field -> getAccessType () not_eq X3D::outputOnly)
				setValue (cx, field, vp);

			result .succeed ();
			return true;
		}
		catch (const X3D::Error <X3D::INVALID_NAME> & error)
		{
			result .succeed ();
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .%s: %s.", getClass () -> name, to_string (cx, id) .c_str (), error .what ());
	}
}

bool
SFNode::getProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	try
	{
		try
		{
			const auto self  = getThis <SFNode> (cx, obj);
			const auto name  = to_string (cx, id);
			const auto field = self -> getValue () -> getField (name);

			if (field -> getAccessType () == X3D::inputOnly)
			{
				vp .setUndefined ();
			}
			else
			{
				vp .set (getValue (cx, field));
			}

			return true;
		}
		catch (const X3D::Error <X3D::INVALID_NAME> & error)
		{
			vp .setUndefined ();
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .%s: %s.", getClass () -> name, to_string (cx, id), error .what ());
	}
}

bool
SFNode::getNodeName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getNodeName: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFNode> (cx, args);

		args .rval () .set (StringValue (cx, self -> getValue () -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getNodeName: %s.", getClass () -> name, error .what ());
	}
}

bool
SFNode::getNodeType (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getNodeType: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFNode> (cx, args);
		const auto node = self -> getValue ();

		JS::AutoValueVector array (cx);

		for (const auto & type : node -> getType ())
			array .append (JS::Int32Value (type));

		const auto result = JS_NewArrayObject (cx, array);

		if (not result)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getNodeType: out of memory", getClass () -> name);

		args .rval () .setObjectOrNull (result);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getNodeType: %s.", getClass () -> name, error .what ());
	}
}

bool
SFNode::getFieldDefinitions (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getFieldDefinitions: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFNode> (cx, args);

//		args .rval () .set (FieldDefinitionArray::create (cx, &self -> getFieldDefinitions ()));

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getFieldDefinitions: %s.", getClass () -> name, error .what ());
	}
}

bool
SFNode::toVRMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toVRMLString: wrong number of arguments.", getClass () -> name);
	
		const auto args             = JS::CallArgsFromVp (argc, vp);
		const auto context          = getContext (cx);
		const auto executionContext = context -> getExecutionContext ();
		const auto self             = getThis <SFNode> (cx, args);
		const auto version          = executionContext -> getSpecificationVersion ();

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		Generator::SpecificationVersion (osstream, version);
		Generator::NicestStyle (osstream);
		Generator::PushExecutionContext (osstream, executionContext);

		self -> toStream (osstream);

		args .rval () .set (StringValue (cx, osstream .str ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toVRMLString: %s.", getClass () -> name, error .what ());
	}
}

bool
SFNode::toXMLString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toXMLString: wrong number of arguments.", getClass () -> name);
	
		const auto args             = JS::CallArgsFromVp (argc, vp);
		const auto context          = getContext (cx);
		const auto executionContext = context -> getExecutionContext ();
		const auto self             = getThis <SFNode> (cx, args);
		auto       version          = executionContext -> getSpecificationVersion ();

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		if (version == VRML_V2_0)
			version = X3D::LATEST_VERSION;

		Generator::SpecificationVersion (osstream, version);
		Generator::NicestStyle (osstream);
		Generator::PushExecutionContext (osstream, executionContext);

		self -> toXMLStream (osstream);

		args .rval () .set (StringValue (cx, osstream .str ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toXMLString: %s.", getClass () -> name, error .what ());
	}
}

bool
SFNode::toString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toString: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFNode> (cx, args);

		args .rval () .set (StringValue (cx, self -> getValue () -> getTypeName () + " { }"));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toString: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania

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

#include "../../../Execution/X3DExecutionContext.h"
#include "../../../InputOutput/Loader.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <>
const std::string X3DObject <X3D::SFNode>::typeName = "SFNode";

template <>
const ObjectType X3DObject <X3D::SFNode>::type = ObjectType::SFNode;

v8::Local <v8::FunctionTemplate>
SFNode::initialize (const v8::Local <v8::External> & context)
{
	const auto functionTemplate = createFunctionTemplate (context, construct);
	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetNamedPropertyHandler (getProperty, setProperty, hasProperty, nullptr, getPropertyNames);

	instanceTemplate -> Set (String ("getName"),             v8::FunctionTemplate::New (getName,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getTypeName"),         v8::FunctionTemplate::New (getTypeName,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getType"),             v8::FunctionTemplate::New (getType,      context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isReadable"),          v8::FunctionTemplate::New (isReadable,   context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isWritable"),          v8::FunctionTemplate::New (isWritable,   context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("getNodeName"),         v8::FunctionTemplate::New (getNodeName,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getNodeType"),         v8::FunctionTemplate::New (getNodeType,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getFieldDefinitions"), v8::FunctionTemplate::New (getFieldDefinitions,  context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("toVRMLString"),        v8::FunctionTemplate::New (toVRMLString, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("toXMLString"),         v8::FunctionTemplate::New (toXMLString,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("toString"),            v8::FunctionTemplate::New (toString,     context) -> GetFunction (), getFunctionAttributes ());

	return functionTemplate;
}

v8::Handle <v8::Value>
SFNode::construct (const v8::Arguments & args)
{
	if (args .IsConstructCall ())
	{
		const auto context = getContext (args);
		const auto object  = args .This ();

		switch (args .Length ())
		{
			case 0:
			{
				addObject (context, object, new T ());
				break;
			}
			case 1:
			{
				const auto arg = args [0];

				if (arg -> IsExternal ())
				{
					addProperty (context, object, getObject (arg));
					break;
				}

				try
				{
					auto       loader = X3D::Loader (context -> getExecutionContext (), context -> getScriptNode () -> getWorldURL ());
					const auto scene  = loader .createX3DFromString (to_string (arg -> ToString ()));

					addProperty (context, object, scene -> getRootNodes () .empty () ? new T () : new T (scene -> getRootNodes () [0]));
					break;
				}
				catch (const X3DError & error)
				{
					return v8::ThrowException (String (error .what ()));
				}
			}
			default:
				return v8::ThrowException (String ("RuntimeError: wrong number of arguments."));
		}

		return v8::Undefined ();
	}

	return v8::ThrowException (String ("RuntimeError: cannot call constructor as function."));
}

v8::Handle <v8::Integer>
SFNode::hasProperty (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	try
	{
		const auto lhs  = getObject (info);
		const auto node = lhs -> getValue ();

		if (not node)
			return v8::Handle <v8::Integer> ();

		const auto field = node -> getField (to_string (property));

		if (field -> getAccessType () == X3D::initializeOnly)
			return v8::Handle <v8::Integer> ();

		return v8::Integer::New (true);
	}
	catch (const std::exception &)
	{
		return v8::Handle <v8::Integer> ();
	}
}

v8::Handle <v8::Value>
SFNode::setProperty (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	try
	{
		const auto lhs  = getObject (info);
		const auto node = lhs -> getValue ();

		if (not node)
			return v8::Undefined ();

		const auto field = node -> getField (to_string (property));

		if (field -> getAccessType () == X3D::initializeOnly or field -> getAccessType () == X3D::outputOnly)
			return v8::Undefined ();

		//if (setValue (field, value))
		// return value;
		//
		//return v8::ThrowException (String (lhs -> getValue () -> getTypeName () + "." + to_string (property) + ": couldn't assign value."));

		return value;
	}
	catch (const std::exception &)
	{
		return v8::Handle <v8::Value> ();
	}
}

v8::Handle <v8::Value>
SFNode::getProperty (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	try
	{
		const auto lhs  = getObject (info);
		const auto node = lhs -> getValue ();

		if (not node)
			return v8::Undefined ();

		const auto field = node -> getField (to_string (property));

		if (field -> getAccessType () == X3D::initializeOnly or field -> getAccessType () == X3D::inputOnly)
			return v8::Undefined ();

		//return getValue (field);
		return v8::Boolean::New (true);
	}
	catch (const std::exception &)
	{
		return v8::Handle <v8::Value> ();
	}
}

v8::Handle <v8::Array>
SFNode::getPropertyNames (const v8::AccessorInfo & info)
{
	const auto lhs   = getObject (info);
	const auto node  = lhs -> getValue ();
	size_t     i     = 0;
	const auto names = v8::Array::New ();

	if (node)
	{
		for (const auto & field : node -> getFieldDefinitions ())
			names -> Set (i ++, String (field -> getName ()));
	}

	return names;
}

v8::Handle <v8::Value>
SFNode::getNodeName (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".getNodeName: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto node    = lhs -> getValue ();

		if (node)
			return String (node -> getName ());

		return String ("");
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

v8::Handle <v8::Value>
SFNode::getNodeType (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".getNodeType: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto node    = lhs -> getValue ();

		size_t     i     = 0;
		const auto array = v8::Array::New ();

		if (node)
		{
			for (const auto & type : node -> getType ())
				array -> Set (i ++, v8::Number::New (type));
		}

		return array;
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

v8::Handle <v8::Value>
SFNode::getFieldDefinitions (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".getFieldDefinitions: wrong number of arguments."));

		//const auto context = getContext (args);
		//const auto lhs     = getObject (context, args);
		//const auto node    = lhs -> getValue ();

		return v8::Undefined ();
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

v8::Handle <v8::Value>
SFNode::toVRMLString (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".toVRMLString: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);

		Generator::SpecificationVersion (context -> getExecutionContext () -> getSpecificationVersion ());
		Generator::NicestStyle ();

		return String (lhs -> toString ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

v8::Handle <v8::Value>
SFNode::toXMLString (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".toXMLString: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		auto       version = context -> getExecutionContext () -> getSpecificationVersion ();

		if (version == X3D::VRML_V2_0)
			version = X3D::LATEST_VERSION;

		Generator::SpecificationVersion (version);
		Generator::NicestStyle ();

		return String (lhs -> toXMLString ());
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

v8::Handle <v8::Value>
SFNode::toString (const v8::Arguments & args)
{
	try
	{
		if (args .Length () not_eq 0)
			return v8::ThrowException (String (TypeName () + ".toString: wrong number of arguments."));

		const auto context = getContext (args);
		const auto lhs     = getObject (context, args);
		const auto node    = lhs -> getValue ();

		if (node)
			return String (node -> getTypeName () + " { }");

		return String ("NULL");
	}
	catch (const std::exception & error)
	{
		return v8::ThrowException (String (error .what ()));
	}
}

} // GoogleV8
} // X3D
} // titania

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

#include "SFNode.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Scripting/Script.h"
#include "../../../Execution/X3DScene.h"
#include "../../../InputOutput/FileLoader.h"
#include "../value.h"

#include <sstream>

namespace titania {
namespace X3D {
namespace peaseblossom {

const std::string SFNode::typeName = "SFNode";

const pb::Callbacks SFNode::callbacks = {
	enumerate,
	hasProperty,
	getProperty,
	setProperty,
	pb::ResolveCallback (),
	dispose <SFNode>

};

pb::ptr <pb::NativeFunction>
SFNode::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 1);
	const auto prototype = context -> getClass (ObjectType::X3DField) -> createInstance (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	// Functions

	prototype -> addOwnProperty ("getNodeName",         new pb::NativeFunction (ec, "getNodeName",         getNodeName,         0), pb::NONE);
	prototype -> addOwnProperty ("getNodeType",         new pb::NativeFunction (ec, "getNodeType",         getNodeType,         0), pb::NONE);
	prototype -> addOwnProperty ("getFieldDefinitions", new pb::NativeFunction (ec, "getFieldDefinitions", getFieldDefinitions, 0), pb::NONE);
	prototype -> addOwnProperty ("toVRMLString",        new pb::NativeFunction (ec, "toVRMLString",        toVRMLString,        0), pb::NONE);
	prototype -> addOwnProperty ("toXMLString",         new pb::NativeFunction (ec, "toXMLString",         toXMLString,         0), pb::NONE);
	prototype -> addOwnProperty ("toString",            new pb::NativeFunction (ec, "toString",            toString,            0), pb::NONE);

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

pb::var
SFNode::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 1:
		{
			try
			{
				const auto   vrmlSyntax = get1Argument <std::string> (args, 0);
				const auto & script     = getContext (ec) -> getScriptNode ();
				const auto   scene      = FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);

				if (not scene -> getRootNodes () .empty () and scene -> getRootNodes () [0])
				{
					setUserData <SFNode> (ec, object, new X3D::SFNode (scene -> getRootNodes () [0]));
					break;
				}

				// Proceed with next case:
			}
			catch (const X3DError & error)
			{
				throw pb::Error (error .what ());
			}
		}
		default:
			throw pb::Error (getTypeName () + ".constructor: wrong number of arguments.");
	}

	return pb::undefined;
}

bool
SFNode::enumerate (pb::pbObject* const object, const pb::EnumerateType type, std::string & propertyName, void* & userData)
{
	const auto node = getObject <X3D::SFNode> (object);

	switch (type)
	{
		case pb::ENUMERATE_BEGIN:
		{
			if (not node -> getValue ())
				return false;

			if (node -> getValue () -> getFieldDefinitions () .empty ())
				return false;

			userData = new size_t (0);
			return true;
		}
		case pb::ENUMERATE:
		{
			auto & index = *static_cast <size_t*> (userData);

			if (index < node -> getValue () -> getFieldDefinitions () .size ())
			{
				propertyName = node -> getValue () -> getFieldDefinitions () [index] -> getName ();
				index       += 1;
				return true;
			}

			return false;
		}
		case pb::ENUMERATE_END:
		{
			delete static_cast <size_t*> (userData);
			return false;
		}
	}

	return false;
}

bool
SFNode::hasProperty (pb::pbObject* const object, const pb::Identifier & identifier)
{
	const auto lhs = getObject <X3D::SFNode> (object);

	if (lhs -> getValue ())
		return lhs -> getValue () -> hasField (identifier .getString ());

	return false;
}

void
SFNode::setProperty (pb::pbObject* const object, const pb::Identifier & identifier, const pb::var & value)
{
	const auto lhs = getObject <X3D::SFNode> (object);

	if (not lhs -> getValue ())
		throw std::out_of_range ("SFNode::setProperty");

	try
	{
		const auto field = lhs -> getValue () -> getField (identifier .getString ());

		if (field -> getAccessType () == X3D::outputOnly)
			return;

		setValue (field, value);
	}
	catch (const Error <X3D::INVALID_NAME> &)
	{
		throw std::out_of_range ("SFNode::setProperty");
	}
}

pb::var
SFNode::getProperty (pb::pbObject* const object, const pb::Identifier & identifier)
{
	const auto lhs = getObject <X3D::SFNode> (object);

	if (not lhs -> getValue ())
		throw std::out_of_range ("SFNode::getProperty");

	try
	{
		const auto context = getContext (object);
		const auto field   = lhs -> getValue () -> getField (identifier .getString ());

		if (field -> getAccessType () == X3D::initializeOnly or field -> getAccessType () == X3D::inputOnly)
			return pb::undefined;

		return getValue (context, field);
	}
	catch (const Error <X3D::INVALID_NAME> &)
	{
		throw std::out_of_range ("SFNode::getProperty");
	}
}

pb::var
SFNode::getNodeName (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.getNodeName: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFNode> (ec, object);

		if (lhs -> getValue ())
			return lhs -> getValue () -> getName ();

		return "";
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.getNodeName is not generic.");
	}
}

pb::var
SFNode::getNodeType (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.getNodeType: wrong number of arguments.");

	try
	{
		const auto lhs   = getThis <SFNode> (ec, object);
		const auto array = new pb::Array (ec);

		if (lhs -> getValue ())
		{
			size_t index = 0;

			for (const auto & type : lhs -> getValue () -> getType ())
				array -> put (basic::to_string (index ++, std::locale::classic ()), uint32_t (type));
		}

		return array;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.getNodeType is not generic.");
	}
}

pb::var
SFNode::getFieldDefinitions (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.getFieldDefinitions: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFNode> (ec, object);

		//		if (lhs -> getValue ())
		//			return FieldDefinitionArray::create (ec, &lhs -> getValue () -> getFieldDefinitions ());
		//
		//		return FieldDefinitionArray::create (ec, nullptr);
		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.getFieldDefinitions is not generic.");
	}
}

pb::var
SFNode::toVRMLString (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.toVRMLString: wrong number of arguments.");

	try
	{
		const auto context = getContext (ec);
		const auto lhs     = getThis <SFNode> (ec, object);
		auto       version = context -> getExecutionContext () -> getSpecificationVersion ();

		std::ostringstream osstream;

		Generator::SpecificationVersion (osstream, version);
		Generator::NicestStyle (osstream);

		lhs -> toStream (osstream);

		return osstream .str ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.toVRMLString is not generic.");
	}
}

pb::var
SFNode::toXMLString (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.toXMLString: wrong number of arguments.");

	try
	{
		const auto context = getContext (ec);
		const auto lhs     = getThis <SFNode> (ec, object);
		auto       version = context -> getExecutionContext () -> getSpecificationVersion ();

		std::ostringstream osstream;

		if (version == VRML_V2_0)
			version = LATEST_VERSION;

		Generator::SpecificationVersion (osstream, version);
		Generator::NicestStyle (osstream);

		lhs -> toXMLStream (osstream);

		return osstream .str ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.toXMLString is not generic.");
	}
}

pb::var
SFNode::toString (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.toString: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFNode> (ec, object);

		if (lhs -> getValue ())
			return lhs -> getValue () -> getTypeName () + " { }";

		return "NULL";
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.toString is not generic.");
	}
}

} // peaseblossom
} // X3D
} // titania

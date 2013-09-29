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

#include "ExternProto.h"

#include "../Bits/Error.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../InputOutput/Loader.h"
#include "../Parser/RegEx.h"

#include <iomanip>

namespace titania {
namespace X3D {

const std::string ExternProto::componentName  = "Browser";
const std::string ExternProto::typeName       = "ExternProto";
const std::string ExternProto::containerField = "externProto";

ExternProto::ExternProto (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DProto (),
	X3DUrlObject (),
	       scene (),
	       proto ()
{
	addField (inputOutput, "metadata", metadata ());

	addChildren (url (), scene, proto);

	//checkLoadState () .isTainted (true);
	url () .isTainted (true);
	scene .isTainted (true);
	proto .isTainted (true);
}

X3DBaseNode*
ExternProto::create (X3DExecutionContext* const executionContext) const
{
	return new ExternProto (executionContext);
}

ExternProto*
ExternProto::clone (X3DExecutionContext* const executionContext) const
{
	ExternProto* copy = dynamic_cast <ExternProto*> (X3DProto::clone (executionContext));

	copy -> url () = url ();

	return copy;
}

void
ExternProto::initialize ()
{
	X3DProto::initialize ();
	X3DUrlObject::initialize ();
	
	for (const auto & field : getFieldDefinitions ())
		field -> isTainted (true);
}

X3DPrototypeInstance*
ExternProto::createInstance (X3DExecutionContext* const executionContext)
{
	requestImmediateLoad ();

	auto instance = new X3DPrototypeInstance (executionContext, proto);

	return instance;
}

void
ExternProto::requestImmediateLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);
	
	Loader loader (getExecutionContext ());

	try
	{
		scene = getBrowser () -> createScene ();

		loader .parseIntoScene (scene, url ());
		
		for (const auto & uninitializedNode : scene -> getUninitializedNodes ())
			getExecutionContext () -> addUninitializedNode (uninitializedNode);

		scene -> setup ();
	}
	catch (const X3DError & error)
	{
		scene = nullptr;

		setLoadState (FAILED_STATE);

		throw Error <URL_UNAVAILABLE> ("Couldn't load any URL specified for EXTERNPROTO '" + getName () + "'\n" + error .what ());
	}

	std::string protoName = loader .getWorldURL () .fragment () .empty ()
	                        ? getName ()
									: loader .getWorldURL () .fragment ();

	proto = scene -> getProtoDeclaration (protoName);

	if (proto)
	{
		for (const auto & fieldDefinition : getUserDefinedFields ())
		{
			try
			{
				X3DFieldDefinition* protoField = proto -> getField (fieldDefinition -> getName ());

				if (protoField -> getAccessType () == fieldDefinition -> getAccessType ())
				{
					if (protoField -> getType () == fieldDefinition -> getType ())
					{
						fieldDefinition -> write (*protoField);
					}
					else
					{
						setLoadState (FAILED_STATE);
						throw Error <INVALID_FIELD> ("EXTERNPROTO '" + getName () + "' field '" + fieldDefinition -> getName () + "' and PROTO field have different types");
					}
				}
				else
				{
					setLoadState (FAILED_STATE);
					throw Error <INVALID_ACCESS_TYPE> ("EXTERNPROTO '" + getName () + "' field '" + fieldDefinition -> getName () + "' and PROTO field have different access types");
				}
			}
			catch (const Error <INVALID_NAME> &)
			{
				setLoadState (FAILED_STATE);
				throw Error <INVALID_NAME> ("EXTERNPROTO field '" + fieldDefinition -> getName () + "' not found in PROTO '" + proto -> getName () + "'");
			}
		}

		setLoadState (COMPLETE_STATE);
		return;
	}
	else
	{
		setLoadState (FAILED_STATE);
		throw Error <INVALID_NAME> ("No PROTO '" + protoName + "' found for EXTERNPROTO '" + getName () + "' in url '" + loader .getWorldURL () + "'");
	}
}

void
ExternProto::toStream (std::ostream & ostream) const
{
	if (not getComments () .empty ())
	{
		for (const auto & comment : getComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}

		ostream << Generator::TidyBreak;
	}

	ostream
		<< Generator::Indent
		<< "EXTERNPROTO"
		<< Generator::Space
		<< getName ()
		<< Generator::TidySpace
		<< '[';

	size_t typeLength       = 0;
	size_t accessTypeLength = 0;

	FieldDefinitionArray fields = getUserDefinedFields ();

	if (fields .empty ())
	{
		if (not getInterfaceComments () .empty ())
		{
			ostream
				<< Generator::TidyBreak
				<< Generator::IncIndent;

			for (const auto & comment : getInterfaceComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
			}

			ostream
				<< Generator::DecIndent
				<< Generator::Indent;
		}

		else
			ostream << Generator::TidySpace;
	}
	else
	{
		switch (Generator::Style ())
		{
			case Generator::SMALLEST:
			case Generator::SMALL:
			{
				break;
			}
			default:
			{
				for (const auto & field : fields)
				{
					typeLength = std::max (typeLength, field -> getTypeName () .length ());

					accessTypeLength = std::max (accessTypeLength, Generator::AccessTypes [field] .length ());
				}

				break;
			}
		}

		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & field : basic::adapter (fields .begin (), fields .end () - 1))
		{
			toStreamField (ostream, field, accessTypeLength, typeLength);
			ostream << Generator::Break;			
		}

		toStreamField (ostream, fields .back (), accessTypeLength, typeLength);
		ostream << Generator::TidyBreak;			

		for (const auto & comment : getInterfaceComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::Break;
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent;
	}

	ostream
		<< ']'
		<< Generator::TidyBreak;

	for (const auto & comment : getInnerComments ())
	{
		ostream
			<< Generator::Indent
			<< Generator::Comment
			<< comment
			<< Generator::Break;
	}

	ostream
		<< Generator::Indent
		<< url ();
}

void
ExternProto::toStreamField (std::ostream & ostream, X3DFieldDefinition* const field, size_t accessTypeLength, size_t typeLength) const
{
	for (const auto & comment : field -> getComments ())
	{
		ostream
			<< Generator::Indent
			<< Generator::Comment
			<< comment
			<< Generator::Break;
	}

	ostream
		<< Generator::Indent
		<< std::setiosflags (std::ios::left)
		<< std::setw (accessTypeLength);

	ostream << Generator::AccessTypes [field];

	ostream
		<< Generator::Space
		<< std::setiosflags (std::ios::left) << std::setw (typeLength) << field -> getTypeName ()
		<< Generator::Space
		<< field -> getName ();
}

void
ExternProto::dispose ()
{
	removeChildren (url ());

	scene .dispose ();
	proto .dispose ();

	X3DUrlObject::dispose ();
	X3DProto::dispose ();
}

ExternProto::~ExternProto ()
{ }

} // X3D
} // titania

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

#include "ExternProtoDeclaration.h"

#include "../Bits/Error.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Execution/X3DScene.h"
#include "../InputOutput/Loader.h"
#include "../Parser/RegEx.h"
#include "../Thread/SceneLoader.h"

#include <iomanip>

namespace titania {
namespace X3D {

static constexpr bool X3D_PARALLEL = true;

const ComponentType ExternProtoDeclaration::component      = ComponentType::TITANIA;
const std::string   ExternProtoDeclaration::typeName       = "EXTERNPROTO";
const std::string   ExternProtoDeclaration::containerField = "externProto";

ExternProtoDeclaration::ExternProtoDeclaration (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
   X3DProtoDeclarationNode (),
	X3DUrlObject (),
	       scene (),
	   prototype (),
	      future ()
{
	addType (X3DConstants::ExternProtoDeclaration);

	addField (inputOutput, "metadata", metadata ());
	url () .setName ("url");

	addChildren (url (), scene, prototype);
}

ExternProtoDeclaration*
ExternProtoDeclaration::create (X3DExecutionContext* const executionContext) const
{
	return new ExternProtoDeclaration (executionContext);
}

ExternProtoDeclaration*
ExternProtoDeclaration::copy (X3DExecutionContext* const executionContext, const CopyType type) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	switch (type)
	{
		case CLONE:
		{
			executionContext -> updateExternProtoDeclaration (this -> getName (), const_cast <ExternProtoDeclaration*> (this));

			return const_cast <ExternProtoDeclaration*> (this);
		}
		case COPY_OR_CLONE:
		{
//			try
//			{
//				const auto externProto = dynamic_cast <ExternProtoDeclaration*> (executionContext -> findProtoDeclaration (getName ()));
//
//				if (externProto)
//					return externProto;
//			}
//			catch (const X3D::X3DError &)
//			{ }
//
//			FieldDefinitionArray userDefinedFields;
//			
//			for (const auto & fieldDefinition : getUserDefinedFields ())
//			{
//				const auto field = fieldDefinition -> create ();
//
//				field -> setName (fieldDefinition -> getName ());
//				field -> setAccessType (fieldDefinition -> getAccessType ());
//
//				userDefinedFields .emplace_back (field);
//			}
//
//			const auto copy = executionContext -> createExternProtoDeclaration (getName (), userDefinedFields, url ());
//
//			executionContext -> addExternProtoDeclaration (getName (), copy);
//
//			transform (copy -> url (), getExecutionContext () -> getWorldURL (), executionContext -> getWorldURL ());
//
//			return copy;
		}
		case FLAT_COPY:
			break;
	}

	throw Error <NOT_SUPPORTED> ("Not supported.");
}

X3DPrototypeInstance*
ExternProtoDeclaration::createInstance (X3DExecutionContext* const executionContext)
// Spec says
//throw (Error <INVALID_NODE>,
//       Error <INVALID_OPERATION_TIMING>,
//       Error <DISPOSED>)
{
	return new X3DPrototypeInstance (executionContext, this);
}

void
ExternProtoDeclaration::initialize ()
{
	X3DProtoDeclarationNode::initialize ();
	X3DUrlObject::initialize ();

	getExecutionContext () -> isLive () .addInterest (this, &ExternProtoDeclaration::set_live);
	isLive () .addInterest (this, &ExternProtoDeclaration::set_live);

	url () .addInterest (this, &ExternProtoDeclaration::set_url);
}

void
ExternProtoDeclaration::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getExecutionContext () -> isLive () .removeInterest (this, &ExternProtoDeclaration::set_live);

	if (scene == getExecutionContext () -> getBrowser () -> getPrivateScene ())
		scene = executionContext -> getBrowser () -> getPrivateScene ();
	else if (scene)
		scene -> setExecutionContext (executionContext);

	if (future)
		future -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DProtoDeclarationNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (this, &ExternProtoDeclaration::set_live);

		set_live ();
	}
}

void
ExternProtoDeclaration::setProtodeclaration (ProtoDeclaration* value)
{
	prototype = value;

	if (prototype)
	{
		for (const auto & fieldDefinition : getFieldDefinitions ())
		{
			try
			{
				X3DFieldDefinition* const protoField = prototype -> getField (fieldDefinition -> getName ());

				if (protoField -> getAccessType () == fieldDefinition -> getAccessType ())
				{
					if (protoField -> getType () == fieldDefinition -> getType ())
					{
						fieldDefinition -> set (*protoField);
					}
					else
					{
						__LOG__ << ("EXTERNPROTO '" + getName () + "' field '" + fieldDefinition -> getName () + "' and PROTO field have different types") << std::endl;
					}
				}
				else
				{
					__LOG__ << ("EXTERNPROTO '" + getName () + "' field '" + fieldDefinition -> getName () + "' and PROTO field have different access types") << std::endl;
				}
			}
			catch (const Error <INVALID_NAME> &)
			{
				__LOG__ << ("EXTERNPROTO field '" + fieldDefinition -> getName () + "' not found in PROTO '" + prototype -> getName () + "'") << std::endl;
			}
		}
	}
}

ProtoDeclaration*
ExternProtoDeclaration::getProtoDeclaration ()
throw (Error <DISPOSED>)
{
	if (prototype)
		return prototype;
	
	throw Error <DISPOSED> ("No prototype declaration available.");
}

void
ExternProtoDeclaration::requestImmediateLoad ()
{
	if (X3D_PARALLEL and checkLoadState () == IN_PROGRESS_STATE)
	{
		future -> wait ();
		return;
	}

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;
	
	setLoadState (IN_PROGRESS_STATE);

	Loader loader (getExecutionContext ());

	try
	{
		scene = getBrowser () -> createScene ();

		loader .parseIntoScene (scene, url ());

		setScene (std::move (scene));
	}
	catch (const X3DError & error)
	{
		setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
	}
}

void
ExternProtoDeclaration::requestAsyncLoad (const std::function <void ()> & callback)
{
	using namespace std::placeholders;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
	{
		callback ();
		return;
	}

	setLoadState (IN_PROGRESS_STATE);

	if (future)
		future -> dispose ();

	future .reset (new SceneLoader (getExecutionContext (),
	                                url (),
	                                std::bind (&ExternProtoDeclaration::setSceneAsync, this, _1, callback)));
}

void
ExternProtoDeclaration::setSceneAsync (X3DScenePtr && value, const std::function <void ()> & callback)
{
	if (value)
	{
		setScene (std::move (value));
	}
	else
	{
		scene = getBrowser () -> getPrivateScene ();

		setProtodeclaration (nullptr);

		setLoadState (FAILED_STATE);
	}

	callback ();
}

void
ExternProtoDeclaration::setScene (X3DScenePtr && value)
{
	scene = value;

	try
	{
		scene -> isLive () = getExecutionContext () -> isLive () and isLive ();
		scene -> isPrivate (getRootContext () -> isPrivate ());
		scene -> setExecutionContext (getExecutionContext ());

		if (getExecutionContext () -> isInitialized ())
			scene -> setup ();

		else
			getExecutionContext () -> addUninitializedNode (scene);

		const std::string protoName = scene -> getWorldURL () .fragment () .empty ()
		                              ? getName ()
									       	: scene -> getWorldURL () .fragment ();

		setProtodeclaration (scene -> getProtoDeclaration (protoName));

		setLoadState (COMPLETE_STATE);
	}
	catch (const X3DError & error)
	{
		scene = getBrowser () -> getPrivateScene ();

		setProtodeclaration (nullptr);

		setLoadState (FAILED_STATE);
	}
}

void
ExternProtoDeclaration::set_live ()
{
	if (checkLoadState () == COMPLETE_STATE)
	{
		const bool value = getExecutionContext () -> isLive () and isLive ();
		
		if (value not_eq scene -> isLive ())
			scene -> isLive () = value;
	}
}

void
ExternProtoDeclaration::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	requestAsyncLoad ([ ] () { });
}

void
ExternProtoDeclaration::toStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

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

	const FieldDefinitionArray fields = getUserDefinedFields ();

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
					<< Generator::ForceBreak;
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

		for (const auto & field : std::make_pair (fields .begin (), fields .end () - 1))
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
				<< Generator::ForceBreak;
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
			<< Generator::ForceBreak;
	}

	ostream
		<< Generator::Indent
		<< url ();
}

void
ExternProtoDeclaration::toStreamField (std::ostream & ostream, X3DFieldDefinition* const field, size_t accessTypeLength, size_t typeLength) const
{
	for (const auto & comment : field -> getComments ())
	{
		ostream
			<< Generator::Indent
			<< Generator::Comment
			<< comment
			<< Generator::ForceBreak;
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
ExternProtoDeclaration::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	ostream
		<< Generator::Indent
		<< "<ExternProtoDeclare"
		<< Generator::Space
		<< "name='"
		<< XMLEncode (getName ())
		<< "'"
		<< Generator::Space
		<< "url='"
		<< XMLEncode (url ())
		<< "'"
		<< ">"
		<< Generator::Break
		<< Generator::IncIndent;

	const FieldDefinitionArray fields = getUserDefinedFields ();
	
	for (const auto & field : fields)
	{
		ostream
			<< Generator::Indent
			<< "<field"
			<< Generator::Space
			<< "accessType='"
			<< Generator::X3DAccessTypes [field]
			<< "'"
			<< Generator::Space
			<< "type='"
			<< field-> getTypeName ()
			<< "'"
			<< Generator::Space
			<< "name='"
			<< XMLEncode (field-> getName ())
			<< "'"
			<< "/>"
			<< Generator::Break;
	}

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</ExternProtoDeclare>";
}

void
ExternProtoDeclaration::dispose ()
{
	if (future)
	{
		future -> dispose ();
		future .reset ();
	}

	removeChildren (url ());

	X3DUrlObject::dispose ();
	X3DProtoDeclarationNode::dispose ();
}

ExternProtoDeclaration::~ExternProtoDeclaration ()
{ }

} // X3D
} // titania

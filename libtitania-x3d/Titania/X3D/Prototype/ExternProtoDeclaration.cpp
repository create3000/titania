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

#include "ExternProtoDeclaration.h"

#include "../Bits/Error.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Execution/X3DScene.h"
#include "../InputOutput/FileLoader.h"
#include "../Thread/SceneFuture.h"

#include <iomanip>

namespace titania {
namespace X3D {

static constexpr bool X3D_PARALLEL = true;

const Component   ExternProtoDeclaration::component      = Component ("Core", 2);
const std::string ExternProtoDeclaration::typeName       = "EXTERNPROTO";
const std::string ExternProtoDeclaration::containerField = "externProto";

ExternProtoDeclaration::ExternProtoDeclaration (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DProtoDeclarationNode (),
	           X3DUrlObject (),
	                  scene (),
	       protoDeclaration (),
	                 future ()
{
	addType (X3DConstants::ExternProtoDeclaration);

	addField (inputOutput, "metadata", metadata ());
	url () .setName ("url");

	addChildObjects (url (), scene, protoDeclaration, future);
}

ExternProtoDeclaration*
ExternProtoDeclaration::create (X3DExecutionContext* const executionContext) const
{
	return new ExternProtoDeclaration (executionContext);
}

///  throws Error <NOT_SUPPORTED>
ExternProtoDeclaration*
ExternProtoDeclaration::copy (X3DExecutionContext* const executionContext, const CopyType type) const
{
	switch (type)
	{
		case CLONE:
		{
			try
			{
				ExternProtoDeclaration* const externproto = dynamic_cast <ExternProtoDeclaration*> (executionContext -> findProtoDeclaration (getName (), AvailableType { }));
	
				if (externproto)
					return externproto;
			}
			catch (const X3DError & error)
			{
				executionContext -> updateExternProtoDeclaration (getName (), ExternProtoDeclarationPtr (const_cast <ExternProtoDeclaration*> (this)));

				return const_cast <ExternProtoDeclaration*> (this);
			}
		}
		case COPY_OR_CLONE:
		{
			try
			{
				return executionContext -> getExternProtoDeclaration (getName ()) .getValue ();
			}
			catch (const X3DError & error)
			{
				executionContext -> updateExternProtoDeclaration (getName (), ExternProtoDeclarationPtr (const_cast <ExternProtoDeclaration*> (this)));

				return const_cast <ExternProtoDeclaration*> (this);
			}
		}
		case FLAT_COPY:
		{
			break;
		}
	}

	throw Error <NOT_SUPPORTED> ("Not supported.");
}

void
ExternProtoDeclaration::initialize ()
{
	X3DProtoDeclarationNode::initialize ();
	X3DUrlObject::initialize ();

	getExecutionContext () -> isLive () .addInterest (&ExternProtoDeclaration::set_live, this);
	isLive () .addInterest (&ExternProtoDeclaration::set_live, this);

	url () .addInterest (&ExternProtoDeclaration::set_url, this);

	setLoadState (checkLoadState ());
}

void
ExternProtoDeclaration::setExecutionContext (X3DExecutionContext* const executionContext)
{
	getExecutionContext () -> isLive () .removeInterest (&ExternProtoDeclaration::set_live, this);

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
		getExecutionContext () -> isLive () .addInterest (&ExternProtoDeclaration::set_live, this);

		set_live ();
	}
}

// Spec says
//throw Error <INVALID_NODE>,
//      Error <INVALID_OPERATION_TIMING>,
//      Error <DISPOSED>
X3DPrototypeInstance*
ExternProtoDeclaration::createInstance (X3DExecutionContext* const executionContext)
{
	return new X3DPrototypeInstance (executionContext, X3DProtoDeclarationNodePtr (this));
}

void
ExternProtoDeclaration::addInstance (X3DPrototypeInstance* const instance)
{
	X3DProtoDeclarationNode::addInstance (instance);

	if (protoDeclaration)
		protoDeclaration -> addInstance (instance);
}

void
ExternProtoDeclaration::removeInstance (X3DPrototypeInstance* const instance)
{
	X3DProtoDeclarationNode::removeInstance (instance);

	if (protoDeclaration)
		protoDeclaration -> removeInstance (instance);
}

void
ExternProtoDeclaration::setProtoDeclaration (ProtoDeclaration* value)
{
	if (protoDeclaration)
	{
		for (const auto & instance : getInstances ())
			protoDeclaration -> removeInstance (instance);
	}

	protoDeclaration = value;

	if (protoDeclaration)
	{
		for (const auto & instance : getInstances ())
			protoDeclaration -> addInstance (instance);	

		if (getBrowser () -> isStrict ())
		{
			for (const auto & fieldDefinition : getFieldDefinitions ())
			{
				try
				{
					X3DFieldDefinition* const protoField = protoDeclaration -> getField (fieldDefinition -> getName ());
	
					if (protoField -> getAccessType () == fieldDefinition -> getAccessType ())
					{
						if (protoField -> getType () == fieldDefinition -> getType ())
							fieldDefinition -> set (*protoField);
						else
							getBrowser () -> getConsole () -> error ("EXTERNPROTO '", getName (), "' field '", fieldDefinition -> getName (), "' and PROTO field have different types.\n");
					}
					else
						getBrowser () -> getConsole () -> error ("EXTERNPROTO '", getName (), "' field '", fieldDefinition -> getName (), "' and PROTO field have different access types.\n");
				}
				catch (const Error <INVALID_NAME> &)
				{
					getBrowser () -> getConsole () -> error ("EXTERNPROTO field '", fieldDefinition -> getName (), "' not found in PROTO '", protoDeclaration -> getName (), "'.\n");
				}
			}
		}
		else
		{
			for (const auto & fieldDefinition : getUserDefinedFields ())
				removeField (fieldDefinition -> getName ());
	
			for (const auto & fieldDefinition : protoDeclaration -> getUserDefinedFields ())
				addUserDefinedField (fieldDefinition -> getAccessType (), fieldDefinition -> getName (), fieldDefinition);
		}
	}
}

///  throws Error <DISPOSED>
ProtoDeclaration*
ExternProtoDeclaration::getProtoDeclaration ()
{
	if (protoDeclaration)
		return protoDeclaration;
	
	throw Error <DISPOSED> ("No prototype declaration available.");
}

void
ExternProtoDeclaration::requestImmediateLoad ()
{
	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	if (future)
	{
		future -> wait ();
		return;
	}

	setLoadState (IN_PROGRESS_STATE);

	FileLoader loader (getExecutionContext ());

	try
	{
		auto scene = getBrowser () -> createScene (false);

		loader .parseIntoScene (scene, url ());

		setLoadState (COMPLETE_STATE);
		setLoadedUrl (scene -> getWorldURL ());
		setScene (std::move (scene));
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error (error .what (), "\n");

		setLoadState (FAILED_STATE);
		setLoadedUrl ("");
		setScene (X3DScenePtr (getBrowser () -> getPrivateScene ()));
	}
}

void
ExternProtoDeclaration::requestAsyncLoad ()
{
	using namespace std::placeholders;

	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	getScene () -> addExternProtoLoadCount (this);

	future .setValue (new SceneFuture (getExecutionContext (),
	                                   url (),
	                                   true,
	                                   std::bind (&ExternProtoDeclaration::setSceneAsync, this, _1)));
}

void
ExternProtoDeclaration::setSceneAsync (X3DScenePtr && value)
{
	getScene () -> removeExternProtoLoadCount (this);

	if (value)
	{
		setLoadState (COMPLETE_STATE);
		setLoadedUrl (value -> getWorldURL ());
		setScene (std::move (value));
	}
	else
	{
		setLoadState (FAILED_STATE);
		setLoadedUrl ("");

		scene = getBrowser () -> getPrivateScene ();

		setProtoDeclaration (nullptr);
	}
}

void
ExternProtoDeclaration::setScene (X3DScenePtr && value)
{
	scene = std::move (value);

	try
	{
		scene -> isLive () = getExecutionContext () -> isLive () and isLive ();
		scene -> setPrivate (getScene () -> getPrivate ());
		scene -> setExecutionContext (getExecutionContext ());

		if (getExecutionContext () -> isInitialized ())
			scene -> setup ();

		else
			getExecutionContext () -> addUninitializedNode (scene);

		if (scene -> getWorldURL () .fragment () .empty ())
		{
			setProtoDeclaration (scene -> getProtoDeclarations () .at (0));
		}
		else
		{
			const auto & protoName = scene -> getWorldURL () .fragment ();

			setProtoDeclaration (scene -> getProtoDeclaration (protoName));
		}
	}
	catch (const std::exception & error)
	{
		setLoadState (FAILED_STATE);

		getBrowser () -> getConsole () -> error (error .what (), "\n");

		scene = getBrowser () -> getPrivateScene ();

		setProtoDeclaration (nullptr);
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

	requestAsyncLoad ();
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
		switch (Generator::Style (ostream))
		{
			case Generator::StyleType::SMALLEST:
			case Generator::StyleType::SMALL:
			{
				break;
			}
			default:
			{
				for (const auto & field : fields)
				{
					typeLength = std::max (typeLength, field -> getTypeName () .length ());

					accessTypeLength = std::max (accessTypeLength, to_string (field -> getAccessType ()) .length ());
				}

				break;
			}
		}

		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & field : std::pair (fields .cbegin (), fields .cend () - 1))
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

	ostream << field -> getAccessType ();

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
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	const FieldDefinitionArray fields = getUserDefinedFields ();
	
	for (const auto & field : fields)
	{
		ostream
			<< Generator::Indent
			<< "<field"
			<< Generator::Space
			<< "accessType='"
			<< field -> getAccessType ()
			<< "'"
			<< Generator::Space
			<< "type='"
			<< field -> getTypeName ()
			<< "'"
			<< Generator::Space
			<< "name='"
			<< XMLEncode (field -> getName ())
			<< "'"
			<< "/>"
			<< Generator::TidyBreak;
	}

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</ExternProtoDeclare>";
}

void
ExternProtoDeclaration::toJSONStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	ostream
		<< '{'
		<< Generator::TidySpace
		<< '"'
		<< "ExternProtoDeclare"
		<< '"'
		<< ':'
		<< Generator::TidyBreak
		<< Generator::IncIndent
		<< Generator::Indent
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent
		<< Generator::Indent
		<< '"'
		<< "@name"
		<< '"'
		<< ':'
		<< SFString (getName ())
		<< ','
		<< Generator::TidyBreak;


	// Fields

	const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	if (not userDefinedFields .empty ())
	{
		ostream
			<< Generator::Indent
			<< '"'
			<< "field"
			<< '"'
			<< ':'
			<< Generator::TidySpace
			<< '['
			<< Generator::TidyBreak
			<< Generator::IncIndent;			

		for (const auto & field : userDefinedFields)
		{
			ostream
				<< Generator::Indent
				<< '{'
				<< Generator::TidyBreak
				<< Generator::IncIndent;	

			ostream
				<< Generator::Indent
				<< '"'
				<< "@accessType"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '"'
				<< field -> getAccessType ()
				<< '"'
				<< ','
				<< Generator::TidyBreak;	

			ostream
				<< Generator::Indent
				<< '"'
				<< "@type"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< '"'
				<< field -> getTypeName ()
				<< '"'
				<< ','
				<< Generator::TidyBreak;	

			ostream
				<< Generator::Indent
				<< '"'
				<< "@name"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< SFString (field -> getName ())
				<< Generator::TidyBreak;

			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< '}';

			if (field not_eq userDefinedFields .back ())
				ostream << ',';

			ostream << Generator::TidyBreak;
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< ']'
			<< ','
			<< Generator::TidyBreak;
	}


	// URL

	ostream
		<< Generator::Indent
		<< '"'
		<< "@url"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< JSONEncode (url ())
		<< Generator::TidyBreak;


	// End

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}'
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}';
}

void
ExternProtoDeclaration::dispose ()
{
	future .setValue (nullptr);

	getScene () -> removeExternProtoLoadCount (this);

	removeChildObjects (url ());

	X3DUrlObject::dispose ();
	X3DProtoDeclarationNode::dispose ();
}

ExternProtoDeclaration::~ExternProtoDeclaration ()
{ }

} // X3D
} // titania

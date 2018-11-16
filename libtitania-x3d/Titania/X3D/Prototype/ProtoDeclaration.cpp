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

#include "ProtoDeclaration.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Prototype/ExternProtoDeclaration.h"

#include <iomanip>

namespace titania {
namespace X3D {

const ComponentType ProtoDeclaration::component      = ComponentType::TITANIA;
const std::string   ProtoDeclaration::typeName       = "PROTO";
const std::string   ProtoDeclaration::containerField = "proto";

ProtoDeclaration::ProtoDeclaration (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DProtoDeclarationNode (),
	    X3DExecutionContext (),
	              loadState (NOT_STARTED_STATE)
{
	addType (X3DConstants::ProtoDeclaration);

	addField (inputOutput, "metadata", metadata ());

	addChildObjects (getRootNodes (), loadState);

	endUpdate ();
}

ProtoDeclaration*
ProtoDeclaration::create (X3DExecutionContext* const executionContext) const
{
	return new ProtoDeclaration (executionContext);
}

///  throws Error <NOT_SUPPORTED>
ProtoDeclaration*
ProtoDeclaration::copy (X3DExecutionContext* const executionContext, const CopyType type) const
{
	switch (type)
	{
		case CLONE:
		{
			ProtoDeclaration* const proto = dynamic_cast <ProtoDeclaration*> (executionContext -> findProtoDeclaration (getName (), AvailableType { }));

			if (proto)
				return proto;

			[[fallthrough]];
		}
		case COPY_OR_CLONE:
		{
			try
			{
				return executionContext -> getProtoDeclaration (getName ()) .getValue ();
			}
			catch (const X3DError & error)
			{
				executionContext -> updateProtoDeclaration (getName (), ProtoDeclarationPtr (const_cast <ProtoDeclaration*> (this)));
	
				return const_cast <ProtoDeclaration*> (this);
			}
		}
		case FLAT_COPY:
			break;
	}

	throw Error <NOT_SUPPORTED> ("Not supported.");
}

void
ProtoDeclaration::initialize ()
{
	X3DProtoDeclarationNode::initialize ();
	X3DExecutionContext::initialize ();

	// Prototypes shouldn't be live as they are not realized.

	// Prototypes must be setuped partially to ensure that routes are printed always, especially route from and to nodes
	// without names or hidden names, this is ok as setup can be called multiple times.

	for (const auto & proto : getProtoDeclarations ())
		proto -> setup ();

	for (const auto & route : getRoutes ())
		route -> setup ();

	loadState = COMPLETE_STATE;
}

void
ProtoDeclaration::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DExecutionContext::setExecutionContext (executionContext);
}

// Spec says
//throw Error <INVALID_NODE>,
//      Error <INVALID_OPERATION_TIMING>,
//      Error <DISPOSED>
X3DPrototypeInstance*
ProtoDeclaration::createInstance (X3DExecutionContext* const executionContext)
{
	return new X3DPrototypeInstance (executionContext, X3DProtoDeclarationNodePtr (this));
}

ExternProtoDeclarationPtr
ProtoDeclaration::createExternProtoDeclaration (X3DExecutionContext* const executionContext, const MFString & url)
{
	FieldDefinitionArray externInterfaceDeclarations;

	for (const auto & fieldDefinition : getFieldDefinitions ())
	{
	   externInterfaceDeclarations .emplace_back (fieldDefinition -> copy (FLAT_COPY));
		externInterfaceDeclarations .back () -> setAccessType (fieldDefinition -> getAccessType ());
		externInterfaceDeclarations .back () -> setName (fieldDefinition -> getName ());
	}

	const auto externproto = executionContext -> createExternProtoDeclaration (getName (), externInterfaceDeclarations, url);

	return externproto;
}

void
ProtoDeclaration::toStream (std::ostream & ostream) const
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
		<< "PROTO"
		<< Generator::Space
		<< getName ()
		<< Generator::TidySpace
		<< '[';

	Generator::EnterScope (ostream);

	const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	if (userDefinedFields .empty ())
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
		ostream
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		size_t typeLength       = 0;
		size_t accessTypeLength = 0;

		switch (Generator::Style (ostream))
		{
			case Generator::StyleType::SMALLEST:
			case Generator::StyleType::SMALL:
			{
				break;
			}
			default:
			{
				for (const auto & field : userDefinedFields)
				{
					typeLength = std::max (typeLength, field -> getTypeName () .length ());

					accessTypeLength = std::max (accessTypeLength, to_string (field -> getAccessType ()) .length ());
				}

				break;
			}
		}

		for (const auto & field : std::pair (userDefinedFields .cbegin (), userDefinedFields .cend () - 1))
		{
			toStreamField (ostream, field, accessTypeLength, typeLength);
			ostream << Generator::Break;
		}

		toStreamField (ostream, userDefinedFields .back (), accessTypeLength, typeLength);
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

	ostream << ']';

	Generator::LeaveScope (ostream);

	ostream << Generator::TidyBreak;

	ostream
		<< Generator::Indent
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	X3DExecutionContext::toStream (ostream);

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}';
}

void
ProtoDeclaration::toStreamField (std::ostream & ostream, X3DFieldDefinition* const field, const size_t accessTypeLength, const size_t typeLength) const
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

	if (field -> isInitializable ())
	{
		ostream
			<< Generator::Space
			<< *field;
	}
}

void
ProtoDeclaration::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	ostream
		<< Generator::Indent
		<< "<ProtoDeclare"
		<< Generator::Space
		<< "name='"
		<< XMLEncode (getName ())
		<< "'"
		<< ">"
		<< Generator::TidyBreak;

	// <ProtoInterface>

	Generator::EnterScope (ostream);

	const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	if (not userDefinedFields .empty ())
	{
		ostream
			<< Generator::IncIndent
			<< Generator::Indent
			<< "<ProtoInterface>"
			<< Generator::TidyBreak
			<< Generator::IncIndent;

		for (const auto & field : userDefinedFields)
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
				<< "'";

			if (field -> isDefaultValue ())
			{
				ostream
					<< "/>"
					<< Generator::TidyBreak;
			}
			else
			{
				switch (field -> getType ())
				{
					case X3DConstants::SFNode:
					case X3DConstants::MFNode:
					{
						Generator::PushContainerField (ostream, nullptr);

						ostream
							<< ">"
							<< Generator::TidyBreak
							<< Generator::IncIndent
							<< XMLEncode (field)
							<< Generator::TidyBreak
							<< Generator::DecIndent
							<< Generator::Indent
							<< "</field>"
							<< Generator::TidyBreak;

						Generator::PopContainerField (ostream);

						break;
					}
					default:
					{
						ostream
							<< Generator::Space
							<< "value='"
							<< XMLEncode (field)
							<< "'"
							<< "/>"
							<< Generator::TidyBreak;

						break;
					}
				}
			}
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< "</ProtoInterface>"
			<< Generator::TidyBreak
			<< Generator::DecIndent;
	}

	Generator::LeaveScope (ostream);

	// </ProtoInterface>

	// <ProtoBody>

	ostream
		<< Generator::IncIndent
		<< Generator::Indent
		<< "<ProtoBody>"
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	X3DExecutionContext::toXMLStream (ostream);

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</ProtoBody>"
		<< Generator::TidyBreak
		<< Generator::DecIndent;

	// </ProtoBody>

	ostream
		<< Generator::Indent
		<< "</ProtoDeclare>";
}

void
ProtoDeclaration::toJSONStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	ostream
		<< '{'
		<< Generator::TidySpace
		<< '"'
		<< "ProtoDeclare"
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

	ostream
		<< Generator::Indent
		<< '"'
		<< "ProtoInterface"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent;


	// Fields

	bool lastProperty = false;

	Generator::EnterScope (ostream);

	const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	if (not userDefinedFields .empty ())
	{
		if (lastProperty)
		{
			ostream
				<< ','
				<< Generator::TidyBreak;
		}

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
				<< SFString (field -> getName ());

			if (field -> isDefaultValue ())
			{
				ostream << Generator::TidyBreak;
			}
			else
			{
				ostream
					<< ','
					<< Generator::TidyBreak;

				// Output value
	
				switch (field -> getType ())
				{
					case X3DConstants::MFNode:
					{
						ostream
							<< Generator::Indent
							<< '"'
							<< "-children"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< JSONEncode (field)
							<< Generator::TidyBreak;
	
						break;
					}
					case X3DConstants::SFNode:
					{
						ostream
							<< Generator::Indent
							<< '"'
							<< "-children"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< '['
							<< Generator::TidyBreak
							<< Generator::IncIndent
							<< Generator::Indent
							<< JSONEncode (field)
							<< Generator::TidyBreak
							<< Generator::DecIndent
							<< Generator::Indent
							<< ']'
							<< Generator::TidyBreak;
	
						break;
					}
					default:
					{
						ostream
							<< Generator::Indent
							<< '"'
							<< "@value"
							<< '"'
							<< ':'
							<< Generator::TidySpace
							<< JSONEncode (field)
							<< Generator::TidyBreak;
	
						break;
					}
				}
			}

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
			<< ']';

		lastProperty = true;
	}

	ostream
		<< Generator::DecIndent
		<< Generator::TidyBreak
		<< Generator::Indent
		<< '}'
		<< ','
		<< Generator::TidyBreak;

	Generator::LeaveScope (ostream);


	// ProtoBody

	ostream
		<< Generator::Indent
		<< '"'
		<< "ProtoBody"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '{'
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	ostream
		<< Generator::Indent
		<< '"'
		<< "-children"
		<< '"'
		<< ':'
		<< Generator::TidySpace
		<< '['
		<< Generator::TidyBreak
		<< Generator::IncIndent;

	X3DExecutionContext::toJSONStream (ostream);

	ostream
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< ']'
		<< Generator::TidyBreak;

	// End

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}'
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}'
		<< Generator::TidyBreak
		<< Generator::DecIndent
		<< Generator::Indent
		<< '}';
}

void
ProtoDeclaration::dispose ()
{
	X3DExecutionContext::dispose ();
	X3DProtoDeclarationNode::dispose ();

	removeChildObjects (getRootNodes ());
}

} // X3D
} // titania

/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "ProtoDeclaration.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Execution/X3DExecutionContext.h"

#include <iomanip>

namespace titania {
namespace X3D {

const std::string ProtoDeclaration::componentName  = "Browser";
const std::string ProtoDeclaration::typeName       = "PROTO";
const std::string ProtoDeclaration::containerField = "proto";

ProtoDeclaration::ProtoDeclaration (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	            X3DNode (),
	X3DExecutionContext (),
	     X3DProtoObject ()
{
	addType (X3DConstants::ProtoDeclaration);

	addField (inputOutput, "metadata", metadata ());
	addChildren (getRootNodes ());

	setEncoding             (executionContext -> getEncoding ());
	setSpecificationVersion (executionContext -> getSpecificationVersion ());
	setCharacterEncoding    (executionContext -> getCharacterEncoding ());
	setComment              (executionContext -> getComment ());

	setProfile (executionContext -> getProfile ());

	for (const auto & component : executionContext -> getComponents ())
		addComponent (component);

	setUnits (executionContext -> getUnits ());
}

ProtoDeclaration*
ProtoDeclaration::create (X3DExecutionContext* const executionContext) const
{
	return new ProtoDeclaration (executionContext);
}

ProtoDeclaration*
ProtoDeclaration::copy (X3DExecutionContext* const executionContext, const CopyType type) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	switch (type)
	{
		case CLONE:
		{
			executionContext -> updateProtoDeclaration (this -> getName (), const_cast <ProtoDeclaration*> (this));

			return const_cast <ProtoDeclaration*> (this);
		}
		case COPY_OR_CLONE:
		{
			try
			{
				const auto proto = dynamic_cast <ProtoDeclaration*> (executionContext -> findProtoObject (getName ()));

				if (proto)
					return proto;
			}
			catch (const X3D::X3DError &)
			{ }

			FieldDefinitionArray userDefinedFields;
			
			for (const auto & fieldDefinition : getUserDefinedFields ())
				userDefinedFields .emplace_back (fieldDefinition -> copy (executionContext, COPY_OR_CLONE));

			const auto copy = executionContext -> createProtoDeclaration (getName (), userDefinedFields);
	
			metadata () .copy (executionContext, &copy -> metadata (), COPY_OR_CLONE);

			executionContext -> addProtoDeclaration (getName (), copy);

			copy -> importExternProtos (this);
			copy -> importProtos (this);
			copy -> importRootNodes (this);
			copy -> importImportedNodes (this);
			copy -> importRoutes (this);

			return copy;
		}
		case FLAT_COPY:
			break;
	}

	throw Error <NOT_SUPPORTED> ("Not supported.");
}

X3DPrototypeInstance*
ProtoDeclaration::createInstance (X3DExecutionContext* const executionContext)
// Spec says
//throw (Error <INVALID_NODE>,
//       Error <INVALID_OPERATION_TIMING>,
//       Error <DISPOSED>)
{
	return new X3DPrototypeInstance (executionContext, this);
}

void
ProtoDeclaration::initialize ()
{
	X3DNode::initialize ();
	X3DExecutionContext::initialize ();
	X3DProtoObject::initialize ();

	// Prototypes shouldn't be live as they are not realized.
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

	Generator::PushContext ();

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

		switch (Generator::Style ())
		{
			case Generator::SMALLEST:
			case Generator::SMALL:
			{
				break;
			}
			default:
			{
				for (const auto & field : userDefinedFields)
				{
					typeLength = std::max (typeLength, field -> getTypeName () .length ());

					accessTypeLength = std::max (accessTypeLength, Generator::AccessTypes [field] .length ());
				}

				break;
			}
		}

		for (const auto & field : std::make_pair (userDefinedFields .begin (), userDefinedFields .end () - 1))
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

	Generator::PopContext ();

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

	ostream << Generator::AccessTypes [field];

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
		<< Generator::Break;

	// <ProtoInterface>

	Generator::PushContext ();

	const FieldDefinitionArray userDefinedFields = getUserDefinedFields ();

	if (not userDefinedFields .empty ())
	{
		ostream
			<< Generator::IncIndent
			<< Generator::Indent
			<< "<ProtoInterface>"
			<< Generator::Break
			<< Generator::IncIndent;

		for (const auto & field : userDefinedFields)
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
					<< Generator::Break;
			}
			else
			{
				switch (field -> getType ())
				{
					case X3DConstants::SFNode:
					case X3DConstants::MFNode:
					{
						Generator::PushContainerField (nullptr);

						ostream
							<< ">"
							<< Generator::Break
							<< Generator::IncIndent
							<< XMLEncode (field)
							<< Generator::Break
							<< Generator::DecIndent
							<< Generator::Indent
							<< "</field>"
							<< Generator::Break;

						Generator::PopContainerField ();

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
							<< Generator::Break;

						break;
					}
				}
			}
		}

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< "</ProtoInterface>"
			<< Generator::Break
			<< Generator::DecIndent;
	}

	Generator::PopContext ();

	// </ProtoInterface>

	// <ProtoBody>

	ostream
		<< Generator::IncIndent
		<< Generator::Indent
		<< "<ProtoBody>"
		<< Generator::Break
		<< Generator::IncIndent;

	X3DExecutionContext::toXMLStream (ostream);

	ostream
		<< Generator::DecIndent
		<< Generator::Indent
		<< "</ProtoBody>"
		<< Generator::Break
		<< Generator::DecIndent;

	// </ProtoBody>

	ostream
		<< Generator::Indent
		<< "</ProtoDeclare>";
}

void
ProtoDeclaration::dispose ()
{
	X3DProtoObject::dispose ();
	X3DExecutionContext::dispose ();
	X3DNode::dispose ();
	
	removeChildren (getRootNodes ());
}

} // X3D
} // titania

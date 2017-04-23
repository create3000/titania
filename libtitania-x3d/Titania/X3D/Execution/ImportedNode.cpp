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

#include "ImportedNode.h"

#include "../Components/Networking/Inline.h"
#include "../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType ImportedNode::component      = ComponentType::TITANIA;
const std::string   ImportedNode::typeName       = "ImportedNode";
const std::string   ImportedNode::containerField = "importedNode";

ImportedNode::ImportedNode (X3DExecutionContext* const executionContext,
                            const X3DPtr <Inline> & inlineNode_,
                            const std::string & exportedName,
                            const std::string & importedName)
throw (Error <INVALID_NAME>,
       Error <NODE_NOT_AVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  inlineNode (inlineNode_),
	exportedNode (inlineNode_ -> getExportedNode (exportedName)),
	exportedName (exportedName),
	importedName (importedName)
{
	addType (X3DConstants::ImportedNode);

	addChildObjects (inlineNode, exportedNode);
}

X3DBaseNode*
ImportedNode::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating imported nodes is not supported.");
}

ImportedNode*
ImportedNode::copy (X3DExecutionContext* const executionContext, const CopyType type) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	try
	{
		const auto inlineNode = executionContext -> getNamedNode <Inline> (getInlineNode () -> getName ());

		return executionContext -> addImportedNode (inlineNode, exportedName, importedName) .getValue ();
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_NAME> ("Bad IMPORT specification in copy: " + std::string (error .what ()));
	}
}

void
ImportedNode::initialize ()
{
	X3DBaseNode::initialize ();

	inlineNode   .addInterest (&ImportedNode::set_node, this);
	exportedNode .addInterest (&ImportedNode::set_node, this);

	shutdown () .addInterest (&X3DWeakPtr <Inline>::dispose, inlineNode);
	shutdown () .addInterest (&X3DWeakPtr <X3DBaseNode>::dispose, exportedNode);

	set_node ();
}

X3DPtr <Inline>
ImportedNode::getInlineNode () const
throw (Error <DISPOSED>)
{
	if (inlineNode)
		return X3DPtr <Inline> (inlineNode);

	throw Error <DISPOSED> ("ImportedNode::getInlineNode: Inline node is already disposed.");
}

SFNode
ImportedNode::getExportedNode () const
throw (Error <DISPOSED>)
{
	if (exportedNode)
		return SFNode (exportedNode);

	throw Error <DISPOSED> ("ImportedNodee::getExportedNode: Exported node '" + exportedName + "' is already disposed.");
}

void
ImportedNode::set_node ()
{
	if (not inlineNode or not exportedNode)
		getExecutionContext () -> removeImportedNode (importedName);
}

void
ImportedNode::toStream (std::ostream & ostream) const
{
	try
	{
		if (Generator::ExistsNode (ostream, getInlineNode ()))
		{
			Generator::AddImportedNode (ostream, getExportedNode (), importedName);

			if (not getComments () .empty ())
			{
				ostream << Generator::TidyBreak;

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
				<< "IMPORT"
				<< Generator::Space
				<< Generator::Name (ostream, getInlineNode ())
				<< '.'
				<< exportedName;

			if (importedName not_eq exportedName)
			{
				ostream
					<< Generator::Space
					<< "AS"
					<< Generator::Space
					<< importedName;
			}
		}
	}
	catch (const X3DError &)
	{ }
}

void
ImportedNode::toXMLStream (std::ostream & ostream) const
//throw (Error <INVALID_NODE>,
//       Error <DISPOSED>)
{
	if (Generator::ExistsNode (ostream, getInlineNode ()))
	{
		Generator::AddImportedNode (ostream, getExportedNode (), importedName);

		ostream
			<< Generator::Indent
			<< "<IMPORT"
			<< Generator::Space
			<< "inlineDEF='"
			<< XMLEncode (Generator::Name (ostream, getInlineNode ()))
			<< "'"
			<< Generator::Space
			<< "exportedDEF='"
			<< XMLEncode (exportedName)
			<< "'";

		if (importedName not_eq exportedName)
		{
			ostream
				<< Generator::Space
				<< "AS='"
				<< XMLEncode (importedName)
				<< "'";
		}

		ostream
			<< "/>";
	}
	else
		throw Error <INVALID_NODE> ("ImportedNode::toXMLStream: Inline node does not exist.");
}

void
ImportedNode::toJSONStream (std::ostream & ostream) const
//throw (Error <INVALID_NODE>,
//       Error <DISPOSED>)
{
	if (Generator::ExistsNode (ostream, getInlineNode ()))
	{
		Generator::AddImportedNode (ostream, getExportedNode (), importedName);

		ostream
			<< '{'
			<< Generator::TidySpace
			<< '"'
			<< "IMPORT"
			<< '"'
			<< ':'
			<< Generator::TidyBreak
			<< Generator::IncIndent
			<< Generator::Indent
			<< '{'
			<< Generator::TidyBreak
			<< Generator::IncIndent;
	
		ostream
			<< Generator::Indent
			<< '"'
			<< "@inlineDEF"
			<< '"'
			<< ':'
			<< Generator::TidySpace
			<< SFString (Generator::Name (ostream, getInlineNode ()))
			<< ','
			<< Generator::TidyBreak;
	
		ostream
			<< Generator::Indent
			<< '"'
			<< "@importedDEF"
			<< '"'
			<< ':'
			<< Generator::TidySpace
			<< SFString (exportedName);
	
		if (importedName not_eq exportedName)
		{
			ostream
				<< ','
				<< Generator::TidyBreak
				<< Generator::Indent
				<< '"'
				<< "@AS"
				<< '"'
				<< ':'
				<< Generator::TidySpace
				<< SFString (importedName)
				<< Generator::TidyBreak;
		}
		else
			ostream << Generator::TidyBreak;

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< '}'
			<< Generator::TidyBreak
			<< Generator::DecIndent
			<< Generator::Indent
			<< '}';
	}
	else
		throw Error <INVALID_NODE> ("ImportedNode::toXMLStream: Inline node does not exist.");
}

void
ImportedNode::dispose ()
{
	X3DBaseNode::dispose ();
}

ImportedNode::~ImportedNode ()
{ }

} // X3D
} // titania

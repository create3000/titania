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

#include "../Browser/X3DBrowser.h"
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
	exportedName (exportedName),
	importedName (importedName),
	  routeIndex ()
{
	addType (X3DConstants::ImportedNode);

	addChildObjects (inlineNode);
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

	if (inlineNode)
		inlineNode -> checkLoadState () .addInterest (this, &ImportedNode::set_loadState);

	inlineNode .addInterest (&ImportedNode::set_inlineNode, this);

	set_loadState ();
	set_inlineNode ();
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
	try
	{
		return inlineNode -> getInternalScene () -> getExportedNode (exportedName);
	}
	catch (const X3DError &)
	{
		throw Error <DISPOSED> ("ImportedNode::getInlineNode: Inline node is already disposed.");
	}
}

void
ImportedNode::addRoute (const SFNode & sourceNode, const std::string & sourceField, const SFNode & destinationNode, const std::string & destinationField)
{
	// Add route.
	
	const auto id   = UnresolvedRouteId (sourceNode -> getId (), sourceField, destinationNode -> getId (), destinationField);
	const auto pair = routeIndex .emplace (id, UnresolvedRoute { sourceNode, sourceField, destinationNode, destinationField });

	// Try to resolve source or destination node routes.

	if (inlineNode)
	{
		if (inlineNode -> checkLoadState () == COMPLETE_STATE)
			resolveRoute (pair .first -> second);
	}
}

void
ImportedNode::resolveRoute (UnresolvedRoute & route)
{
	try
	{
		SFNode sourceNode (route .sourceNode);
		SFNode destinationNode (route .destinationNode);
		const X3DPtr <ImportedNode> importedSourceNode (route .sourceNode);
		const X3DPtr <ImportedNode> importedDestinationNode (route .destinationNode);

		if (importedSourceNode)
			sourceNode = importedSourceNode -> getExportedNode ();

		if (importedDestinationNode)
			destinationNode = importedDestinationNode -> getExportedNode ();

		route .route = getExecutionContext () -> addSimpleRoute (sourceNode, route .sourceField, destinationNode, route .destinationField);
	}
	catch (const X3DError & error)
	{
		getBrowser () -> getConsole () -> error (error .what (), "\n");
	}
}

void
ImportedNode::deleteRoutes ()
{
	for (const auto & pair : routeIndex)
	{
		const auto & route = pair .second;

		if (route .route)
			getExecutionContext () -> deleteRoute (RoutePtr (route .route));
	}
}

void
ImportedNode::set_loadState ()
{
	switch (inlineNode -> checkLoadState ())
	{
		case NOT_STARTED_STATE:
		case FAILED_STATE:
		{
			deleteRoutes ();
			break;
		}
		case IN_PROGRESS_STATE:
			break;
		case COMPLETE_STATE:
		{
			deleteRoutes ();

			for (auto & pair : routeIndex)
				resolveRoute (pair .second);

			break;
		}
	}
}

void
ImportedNode::set_inlineNode ()
{
	if (not inlineNode)
		getExecutionContext () -> removeImportedNode (importedName);
}

void
ImportedNode::toStream (std::ostream & ostream) const
{
	try
	{
		if (Generator::ExistsNode (ostream, getInlineNode ()))
		{
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

			try
			{
				Generator::AddRouteNode (ostream, this);
				Generator::AddImportedNode (ostream, getExportedNode (), getImportedName ());
			}
			catch (const X3DError & error)
			{
				// Output unresolved routes.
	
				for (const auto & pair : routeIndex)
				{
					const auto & route            = pair .second;
					const auto & sourceNode       = route .sourceNode;
					const auto & sourceField      = route .sourceField;
					const auto & destinationNode  = route .destinationNode;
					const auto & destinationField = route .destinationField;
	
					if (Generator::ExistsRouteNode (ostream, sourceNode) and Generator::ExistsRouteNode (ostream, destinationNode))
					{
						const X3DPtr <ImportedNode> importedSourceNode (sourceNode);
						const X3DPtr <ImportedNode> importedDestinationNode (destinationNode);
	
						const auto & sourceNodeName      = importedSourceNode ? importedSourceNode -> getImportedName () : Generator::Name (ostream, sourceNode);
						const auto & destinationNodeName = importedDestinationNode ? importedDestinationNode -> getImportedName () : Generator::Name (ostream, destinationNode);
	
						ostream
							<< Generator::Break
							<< Generator::Break
							<< Generator::Indent
							<< "ROUTE"
							<< Generator::Space
							<< sourceNodeName
							<< '.'
							<< sourceField
							<< Generator::Space
							<< "TO"
							<< Generator::Space
							<< destinationNodeName
							<< '.'
							<< destinationField;
					}
				}
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
		ostream
			<< Generator::Indent
			<< "<IMPORT"
			<< Generator::Space
			<< "inlineDEF='"
			<< XMLEncode (Generator::Name (ostream, getInlineNode ()))
			<< "'"
			<< Generator::Space
			<< "importedDEF='"
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

		try
		{
			Generator::AddRouteNode (ostream, this);
			Generator::AddImportedNode (ostream, getExportedNode (), getImportedName ());
		}
		catch (const X3DError & error)
		{
			// Output unresolved routes.

			for (const auto & pair : routeIndex)
			{
				const auto & route            = pair .second;
				const auto & sourceNode       = route .sourceNode;
				const auto & sourceField      = route .sourceField;
				const auto & destinationNode  = route .destinationNode;
				const auto & destinationField = route .destinationField;

				if (Generator::ExistsRouteNode (ostream, sourceNode) and Generator::ExistsRouteNode (ostream, destinationNode))
				{
					const X3DPtr <ImportedNode> importedSourceNode (sourceNode);
					const X3DPtr <ImportedNode> importedDestinationNode (destinationNode);

					const auto & sourceNodeName      = importedSourceNode ? importedSourceNode -> getImportedName () : Generator::Name (ostream, sourceNode);
					const auto & destinationNodeName = importedDestinationNode ? importedDestinationNode -> getImportedName () : Generator::Name (ostream, destinationNode);

					ostream
						<< Generator::Break
						<< Generator::Break
						<< Generator::Indent
						<< "<ROUTE"
						<< Generator::Space
						<< "fromNode='"
						<< XMLEncode (sourceNodeName)
						<< "'"
						<< Generator::Space
						<< "fromField='"
						<< XMLEncode (sourceField)
						<< "'"
						<< Generator::Space
						<< "toNode='"
						<< XMLEncode (destinationNodeName)
						<< "'"
						<< Generator::Space
						<< "toField='"
						<< XMLEncode (destinationField)
						<< "'"
						<< "/>";
				}
			}
		}
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

		try
		{
			Generator::AddRouteNode (ostream, this);
			Generator::AddImportedNode (ostream, getExportedNode (), getImportedName ());
		}
		catch (const X3DError & error)
		{
			// Output unresolved routes.

			for (const auto & pair : routeIndex)
			{
				const auto & route            = pair .second;
				const auto & sourceNode       = route .sourceNode;
				const auto & sourceField      = route .sourceField;
				const auto & destinationNode  = route .destinationNode;
				const auto & destinationField = route .destinationField;

				if (Generator::ExistsRouteNode (ostream, sourceNode) and Generator::ExistsRouteNode (ostream, destinationNode))
				{
					const X3DPtr <ImportedNode> importedSourceNode (sourceNode);
					const X3DPtr <ImportedNode> importedDestinationNode (destinationNode);

					const auto & sourceNodeName      = importedSourceNode ? importedSourceNode -> getImportedName () : Generator::Name (ostream, sourceNode);
					const auto & destinationNodeName = importedDestinationNode ? importedDestinationNode -> getImportedName () : Generator::Name (ostream, destinationNode);

					ostream
						<< Generator::TidyBreak
						<< Generator::Indent
						<< '{'
						<< Generator::TidySpace
						<< '"'
						<< "ROUTE"
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
						<< "@fromNode"
						<< '"'
						<< ':'
						<< Generator::TidySpace
						<< SFString (sourceNodeName)
						<< ','
						<< Generator::TidyBreak;

					ostream
						<< Generator::Indent
						<< '"'
						<< "@fromField"
						<< '"'
						<< ':'
						<< Generator::TidySpace
						<< SFString (sourceField)
						<< ','
						<< Generator::TidyBreak;

					ostream
						<< Generator::Indent
						<< '"'
						<< "@toNode"
						<< '"'
						<< ':'
						<< Generator::TidySpace
						<< SFString (destinationNodeName)
						<< ','
						<< Generator::TidyBreak;

					ostream
						<< Generator::Indent
						<< '"'
						<< "@toField"
						<< '"'
						<< ':'
						<< Generator::TidySpace
						<< SFString (destinationField)
						<< Generator::TidyBreak;

					ostream
						<< Generator::DecIndent
						<< Generator::Indent
						<< '}'
						<< Generator::TidyBreak
						<< Generator::DecIndent
						<< Generator::Indent
						<< '}';
				}
			}
		}
	}
	else
		throw Error <INVALID_NODE> ("ImportedNode::toXMLStream: Inline node does not exist.");
}

void
ImportedNode::dispose ()
{
	deleteRoutes ();

	X3DBaseNode::dispose ();
}

ImportedNode::~ImportedNode ()
{ }

} // X3D
} // titania

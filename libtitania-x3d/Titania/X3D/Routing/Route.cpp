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

#include "Route.h"

#include "../Bits/Error.h"
#include "../Execution/X3DExecutionContext.h"

#include <Titania/Backtrace.h>
#include <cassert>

namespace titania {
namespace X3D {

const Component   Route::component      = Component ("Titania", 1);
const std::string Route::typeName       = "Route";
const std::string Route::containerField = "route";

Route::Route (X3DExecutionContext* const executionContext,
              const SFNode & p_sourceNode,      X3DFieldDefinition* const p_sourceField,
              const SFNode & p_destinationNode, X3DFieldDefinition* const p_destinationField) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	        sourceNode (p_sourceNode),
	       sourceField (p_sourceField),
	   destinationNode (p_destinationNode),
	  destinationField (p_destinationField),
	         connected (false),
	disconnectedOutput ()
{
	addType (X3DConstants::Route);

	addChildObjects (sourceNode, destinationNode);
}

Route*
Route::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating routes is not supported.");
}

Route*
Route::copy (X3DExecutionContext* const executionContext, const CopyType type) const
{
	try
	{
		const SFNode sourceNode      = executionContext -> getLocalNode (getExecutionContext () -> getLocalName (getSourceNode ()));
		const SFNode destinationNode = executionContext -> getLocalNode (getExecutionContext () -> getLocalName (getDestinationNode ()));

		return executionContext -> addRoute (sourceNode, getSourceField (), destinationNode, getDestinationField ()) .getValue ();
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_NAME> ("Bad ROUTE specification in copy: " + std::string (error .what ()));
	}
}

void
Route::initialize ()
{
	X3DBaseNode::initialize ();

	sourceNode      .addInterest (&Route::set_node, this);
	destinationNode .addInterest (&Route::set_node, this);

	connect ();

	set_node ();
}

RouteId
Route::getKey () const
{
	return std::pair (sourceField, destinationField);
}

bool
Route::isConnected () const
{
	return sourceNode and destinationNode;
}

///  throws Error <DISPOSED>
SFNode
Route::getSourceNode () const
{
	if (isConnected ())
		return SFNode (sourceNode);

	throw Error <DISPOSED> ("Route is already disposed.");
}

///  throws Error <DISPOSED>
const std::string &
Route::getSourceField () const
{
	if (isConnected ())
		return sourceField -> getName ();

	throw Error <DISPOSED> ("Route is already disposed.");
}

///  throws Error <DISPOSED>
SFNode
Route::getDestinationNode () const
{
	if (isConnected ())
		return SFNode (destinationNode);

	throw Error <DISPOSED> ("Route is already disposed.");
}

///  throws Error <DISPOSED>
const std::string &
Route::getDestinationField () const
{
	if (isConnected ())
		return destinationField -> getName ();

	throw Error <DISPOSED> ("Route is already disposed.");
}

void
Route::connect ()
{
	if (connected)
		return;

	if (sourceNode and destinationNode)
	{
		connected = true;

		sourceField -> addInterest (destinationField);
		sourceField -> addOutputRoute (this);
		destinationField -> addInputRoute (this);
	}
}

void
Route::disconnect ()
{
	if (connected)
	{
		connected = false;

		disconnectedOutput .processInterests ();
		disconnectedOutput .dispose ();

		sourceNode      .dispose ();
		destinationNode .dispose ();

		sourceField -> removeInterest (destinationField);
		sourceField -> removeOutputRoute (this);
		destinationField -> removeInputRoute (this);
	}
}

void
Route::erase ()
{
	try
	{
		getExecutionContext () -> deleteRoute (this);
	}
	catch (const Error <DISPOSED> & error)
	{ }
}

void
Route::set_node ()
{
	try
	{
		if (not sourceNode or not destinationNode)
			getExecutionContext () -> deleteRoute (this);
	}
	catch (const Error <DISPOSED> & error)
	{ }
}

//throw Error <INVALID_NODE>,
//       Error <DISPOSED>
void
Route::toStream (std::ostream & ostream) const
{
	const std::string & sourceNodeName      = Generator::LocalName (ostream, getSourceNode ());
	const std::string & destinationNodeName = Generator::LocalName (ostream, getDestinationNode ());

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
		<< "ROUTE"
		<< Generator::Space
		<< sourceNodeName;

	ostream << '.';

	ostream << sourceField -> getName ();

	if (sourceField -> getAccessType () == inputOutput)
		ostream << "_changed";

	ostream
		<< Generator::Space
		<< "TO"
		<< Generator::Space
		<< destinationNodeName;

	ostream << '.';

	if (destinationField -> getAccessType () == inputOutput)
		ostream << "set_";

	ostream << destinationField -> getName ();
}

//throw Error <INVALID_NODE>,
//      Error <DISPOSED>
void
Route::toXMLStream (std::ostream & ostream) const
{
	const std::string & sourceNodeName      = Generator::LocalName (ostream, getSourceNode ());
	const std::string & destinationNodeName = Generator::LocalName (ostream, getDestinationNode ());

	ostream
		<< Generator::Indent
		<< "<ROUTE"
		<< Generator::Space
		<< "fromNode='"
		<< XMLEncode (sourceNodeName)
		<< "'"
		<< Generator::Space
		<< "fromField='"
		<< XMLEncode (sourceField -> getName ());

	if (sourceField -> getAccessType () == inputOutput)
		ostream << "_changed";

	ostream
		<< "'"
		<< Generator::Space
		<< "toNode='"
		<< XMLEncode (destinationNodeName)
		<< "'"
		<< Generator::Space
		<< "toField='";

	if (destinationField -> getAccessType () == inputOutput)
		ostream << "set_";

	ostream
		<< XMLEncode (destinationField -> getName ())
		<< "'"
		<< "/>";
}

///  throws Error <INVALID_NODE>,
///        Error <DISPOSED>
void
Route::toJSONStream (std::ostream & ostream) const
{
	const std::string & sourceNodeName      = Generator::LocalName (ostream, getSourceNode ());
	const std::string & destinationNodeName = Generator::LocalName (ostream, getDestinationNode ());

	ostream
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
		<< SFString (sourceField -> getName ())
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
		<< SFString (destinationField -> getName ())
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

void
Route::dispose ()
{
	disconnect ();

	X3DBaseNode::dispose ();
}

} // X3D
} // titania

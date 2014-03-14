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

#include "Route.h"

#include "../Bits/Error.h"
#include "../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string Route::componentName  = "Browser";
const std::string Route::typeName       = "Route";
const std::string Route::containerField = "route";

Route::Route (X3DExecutionContext* const executionContext,
              const SFNode & sourceNode,      X3DFieldDefinition* const sourceField,
              const SFNode & destinationNode, X3DFieldDefinition* const destinationField) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	        sourceNode (sourceNode),
	       sourceField (sourceField),
	   destinationNode (destinationNode),
	  destinationField (destinationField),
	disconnectedOutput ()
{
	sourceNode      -> shutdown () .addInterest (this, &Route::remove);
	destinationNode -> shutdown () .addInterest (this, &Route::remove);

	setup ();
	connect ();
}

Route*
Route::create (X3DExecutionContext* const) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating routes is not supported.");
}

Route*
Route::clone (X3DExecutionContext* const) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("Cloning routes is not supported.");
}

Route*
Route::copy (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	try
	{
		const SFNode sourceNode      = executionContext -> getNode (getExecutionContext () -> getLocalName (getSourceNode ()));
		const SFNode destinationNode = executionContext -> getNode (getExecutionContext () -> getLocalName (getDestinationNode ()));

		return executionContext -> addRoute (sourceNode, getSourceField (), destinationNode, getDestinationField ()) .getValue ();
	}
	catch (const X3DError & error)
	{
		throw Error <INVALID_NAME> ("Bad ROUTE specification in copy: " + std::string (error .what ()));
	}
}

bool
Route::isConnected () const
{
	return sourceNode and destinationNode and
	       not sourceNode      -> getParents () .empty () and
	       not destinationNode -> getParents () .empty ();
}

RouteId
Route::getId () const
{
	return std::make_pair (sourceField, destinationField);
}

SFNode
Route::getSourceNode () const
throw (Error <DISPOSED>)
{
	if (isConnected ())
		return sourceNode;

	throw Error <DISPOSED> ("Route is already disposed.");
}

const std::string &
Route::getSourceField () const
throw (Error <DISPOSED>)
{
	if (isConnected ())
		return sourceField -> getName ();

	throw Error <DISPOSED> ("Route is already disposed.");
}

SFNode
Route::getDestinationNode () const
throw (Error <DISPOSED>)
{
	if (isConnected ())
		return destinationNode;

	throw Error <DISPOSED> ("Route is already disposed.");
}

const std::string &
Route::getDestinationField () const
throw (Error <DISPOSED>)
{
	if (isConnected ())
		return destinationField -> getName ();

	throw Error <DISPOSED> ("Route is already disposed.");
}

void
Route::connect ()
{
	sourceField -> addInterest (destinationField);
	sourceField -> addOutputRoute (this);
	destinationField -> addInputRoute (this);
}

void
Route::disconnect ()
{
	if (sourceNode and destinationNode)
	{
		sourceNode      = nullptr;
		destinationNode = nullptr;

		sourceField -> removeInterest (destinationField);
		sourceField -> removeOutputRoute (this);
		destinationField -> removeInputRoute  (this);

		disconnected () .processInterests ();
	}
}

void
Route::remove ()
{
	getExecutionContext () -> deleteRoute (this);
}

void
Route::toStream (std::ostream & ostream) const
{
	try
	{
		const std::string sourceNodeName      = Generator::GetLocalName (getSourceNode ());
		const std::string destinationNodeName = Generator::GetLocalName (getDestinationNode ());

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
	catch (...)
	{ }
}

void
Route::dispose ()
{
	disconnect ();

	X3DBaseNode::dispose ();
}

} // X3D
} // titania

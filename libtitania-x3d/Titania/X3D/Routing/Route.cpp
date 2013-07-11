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

#include <iostream>

namespace titania {
namespace X3D {

Route::Route (X3DExecutionContext* const executionContext,
              const SFNode & _sourceNode,      X3DFieldDefinition* const sourceField,
              const SFNode & _destinationNode, X3DFieldDefinition* const destinationField) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	        X3DRoute (),
	      sourceNode (_sourceNode),                                         // SFNode   [ ] sourceNode         NULL
	 destinationNode (_destinationNode),                                    // SFNode   [ ] destinationNode    NULL
	     sourceField (sourceField),
	destinationField (destinationField),
	       connected (false)
{
	setComponent ("Browser");
	setTypeName ("Route");

	addChildren (sourceNode, destinationNode);

	setup ();
}

X3DBaseNode*
Route::create (X3DExecutionContext* const) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating routes is not supported.");
}

Route*
Route::clone (X3DExecutionContext* const executionContext) const
{
	//	std::clog << __func__ << ": " << getTypeName () << " " << getName () << std::endl;

	try
	{
		const SFNode & sourceNode      = executionContext -> getNode (getExecutionContext () -> getLocalName (getSourceNode ()));
		const SFNode & destinationNode = executionContext -> getNode (getExecutionContext () -> getLocalName (getDestinationNode ()));

		return *executionContext -> addRoute (sourceNode, getSourceField (), destinationNode, getDestinationField ());
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
	connect ();
}

bool
Route::isConnected ()
{
	return connected;
}

RouteId
Route::getId () const
{
	return std::make_pair (sourceField, destinationField);
}

const SFNode &
Route::getSourceNode () const
{
	return sourceNode;
}

const std::string &
Route::getSourceField () const
{
	return sourceField -> getName ();
}

const SFNode &
Route::getDestinationNode () const
{
	return destinationNode;
}

const std::string &
Route::getDestinationField () const
{
	return destinationField -> getName ();
}

void
Route::connect ()
{
	if (not connected)
	{
		sourceField -> addInterest (destinationField);
		sourceField -> addOutputRoute (this);
		destinationField -> addInputRoute  (this);

		connected = true;
	}
}

void
Route::disconnect ()
{
	if (connected)
	{
		sourceField -> removeInterest (destinationField);
		sourceField -> removeOutputRoute (this);
		destinationField -> removeInputRoute  (this);

		connected = false;
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
		std::string sourceNodeName      = Generator::GetLocalName (sourceNode);
		std::string destinationNodeName = Generator::GetLocalName (destinationNode);

		if (getComments () .size ())
		{
			ostream << Generator::TidyBreak;

			for (const auto & comment : getComments ())
			{
				ostream
					<< Generator::Indent
					<< Generator::Comment
					<< comment
					<< Generator::Break;
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

	sourceNode      .dispose ();
	destinationNode .dispose ();

	X3DBaseNode::dispose ();
}

} // X3D
} // titania

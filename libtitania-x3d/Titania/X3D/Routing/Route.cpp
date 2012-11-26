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
 ******************************************************************************/

#include "Route.h"

#include "../Bits/Error.h"
#include "../Execution/X3DExecutionContext.h"

#include <iostream>

namespace titania {
namespace X3D {

Route::Route (X3DExecutionContext* const executionContext,
              const SFNode <X3DBasicNode> & sourceNode,      X3DFieldDefinition* const sourceField,
              const SFNode <X3DBasicNode> & destinationNode, X3DFieldDefinition* const destinationField) :
	    X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	        X3DRoute (),                                                    
	      sourceNode (sourceNode),                                          // SFNode   [ ] sourceNode         NULL
	 destinationNode (destinationNode),                                     // SFNode   [ ] destinationNode    NULL
	     sourceField (sourceField),                                         
	destinationField (destinationField),                                    
	       connected (false)                                                
{
	setTypeName ("Route");

	setup ();
}

X3DBasicNode*
Route::create (X3DExecutionContext* const) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating routes is not supported.");
}

Route*
Route::copy (X3DExecutionContext* const executionContext) const
{
	//	std::clog << __func__ << ": " << getTypeName () << " " << getName () << std::endl;

	try
	{
		const SFNode <X3DBasicNode> & sourceNode = executionContext -> getNode (getSourceNode () .getName () .last ());

		const SFNode <X3DBasicNode> & destinationNode = executionContext -> getNode (getDestinationNode () .getName () .last ());

		return *executionContext -> addRoute (sourceNode, getSourceField (), destinationNode, getDestinationField ());
	}
	catch (const Error <INVALID_NAME> & error)
	{
		throw Error <INVALID_NAME> ("Bad ROUTE specification in copy: " + std::string (error .what ()));
	}
}

void
Route::initialize ()
{
	X3DBasicNode::initialize ();
	connect ();
}

bool
Route::isConnected ()
{
	return connected;
}

const SFNode <X3DBasicNode> &
Route::getSourceNode () const
{
	return sourceNode;
}

const basic::id &
Route::getSourceField () const
{
	return sourceField -> getName ();
}

const SFNode <X3DBasicNode> &
Route::getDestinationNode () const
{
	return destinationNode;
}

const basic::id &
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
		destinationField  -> removeInputRoute  (this);

		connected = false;
	}
}

void
Route::toStream (std::ostream & ostream) const
{
	ostream
		<< "ROUTE"
		<< Generator::Space;

	if (sourceNode .getName () .count ())
		ostream << sourceNode .getName () .last ();
	else
		ostream << Generator::GetName (sourceNode);

	ostream << '.';

	ostream << sourceField -> getName ();
	
	if (sourceField -> getAccessType () == inputOutput)
		ostream << "_changed";

	ostream
		<< Generator::Space
		<< "TO"
		<< Generator::Space;

	if (destinationNode .getName () .count ())
		ostream << destinationNode .getName () .last ();
	else
		ostream << Generator::GetName (destinationNode);

	ostream << '.';
	
	if (destinationField -> getAccessType () == inputOutput)
		ostream << "set_";

	ostream << destinationField -> getName ();
}

void
Route::dispose ()
{
	disconnect ();

	X3DBasicNode::dispose ();
}

} // X3D
} // titania

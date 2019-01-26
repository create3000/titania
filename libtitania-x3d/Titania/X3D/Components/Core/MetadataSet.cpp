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

#include "MetadataSet.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataBoolean.h"
#include "../Core/MetadataDouble.h"
#include "../Core/MetadataFloat.h"
#include "../Core/MetadataInteger.h"
#include "../Core/MetadataString.h"

namespace titania {
namespace X3D {

const Component   MetadataSet::component      = Component ("Core", 1);
const std::string MetadataSet::typeName       = "MetadataSet";
const std::string MetadataSet::containerField = "metadata";

MetadataSet::Fields::Fields () :
	value (new MFNode ())
{ }

MetadataSet::MetadataSet (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	          X3DNode (),
	X3DMetadataObject (),
	           fields (),
	    metadataIndex (),
	            nodes ()
{
	addType (X3DConstants::MetadataSet);

	addField (inputOutput, "metadata",  metadata ());
	addField (inputOutput, "name",      name ());
	addField (inputOutput, "reference", reference ());
	addField (inputOutput, "value",     value ());

	addChildObjects (nodes);
}

X3DBaseNode*
MetadataSet::create (X3DExecutionContext* const executionContext) const
{
	return new MetadataSet (executionContext);
}

void
MetadataSet::initialize ()
{
	X3DNode::initialize ();
	X3DMetadataObject::initialize ();

	value () .addInterest (&MetadataSet::set_value, this);

	set_value ();
}

///  throws Error <INVALID_NAME>, Error <DISPOSED>
X3DMetadataObject*
MetadataSet::getObject (const std::string & name) const
{
	const auto iter = metadataIndex .find (name);

	if (iter not_eq metadataIndex .end ())
		return iter -> second;

	throw Error <INVALID_NAME> ("MetadataSet::getObject: Invalid name '" + name + "'.");
}

///  throws Error <DISPOSED>
void
MetadataSet::removeValue (const std::string & name)
{
	const auto iter = std::remove_if (value () .begin (), value () .end (), [&] (const SFNode &node)
	                                  {
	                                     const auto metadataObject = x3d_cast <X3DMetadataObject*> (node);

	                                     if (not metadataObject)
														 return false;

	                                     if (not metadataObject -> getBelongsToProvider ())
														  return false;

	                                     if (metadataObject -> name () == name)
	                                     {
	                                        metadataIndex .erase (name);
	                                        return true;
	                                     }

	                                     return false;
												 });

	value () .erase (iter, value () .end ());
}

void
MetadataSet::setValue (const std::string & name, X3DMetadataObject* const metadataObject)
{
	metadataIndex .emplace (name, metadataObject);

	metadataObject -> name ()      = name;
	metadataObject -> reference () = getBrowser () -> getProviderUrl ();

	getExecutionContext () -> addNamedNode (getExecutionContext () -> getUniqueName (name), SFNode (metadataObject));
}

void
MetadataSet::addValue (const SFNode & node)
{
	const auto metadataObject = x3d_cast <X3DMetadataObject*> (node);

	if (not metadataObject)
		return;

	if (not metadataObject -> getBelongsToProvider ())
		return;

	metadataIndex .emplace (metadataObject -> name (), metadataObject);
}

///  throws Error <DISPOSED>
void
MetadataSet::removeValues ()
{
	metadataIndex .clear ();
}

void
MetadataSet::set_value ()
{
	// Refresh index.

	removeValues ();

	for (const auto & node : value ())
		addValue (node);

	// Handle meta clone count.

	for (const auto & node : nodes)
	{
		if (node)
			node -> removeMetaCloneCount (1);
	}

	nodes = value ();

	for (const auto & node : nodes)
	{
		if (node)
			node -> addMetaCloneCount (1);
	}
}

void
MetadataSet::dispose ()
{
	removeValues ();

	for (const auto & node : nodes)
	{
		if (node)
			node -> removeMetaCloneCount (1);
	}

	X3DMetadataObject::dispose ();
	X3DNode::dispose ();
}

} // X3D
} // titania

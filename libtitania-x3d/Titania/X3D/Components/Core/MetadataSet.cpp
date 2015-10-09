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

#include "MetadataSet.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataBoolean.h"
#include "../Core/MetadataDouble.h"
#include "../Core/MetadataFloat.h"
#include "../Core/MetadataInteger.h"
#include "../Core/MetadataString.h"

namespace titania {
namespace X3D {

const ComponentType MetadataSet::component      = ComponentType::CORE;
const std::string   MetadataSet::typeName       = "MetadataSet";
const std::string   MetadataSet::containerField = "metadata";

MetadataSet::Fields::Fields () :
	value (new MFNode ())
{ }

MetadataSet::MetadataSet (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	          X3DNode (),
	X3DMetadataObject (),
	           fields (),
	      metadataSet ()
{
	addType (X3DConstants::MetadataSet);

	addField (inputOutput, "metadata",  metadata ());
	addField (inputOutput, "name",      name ());
	addField (inputOutput, "reference", reference ());
	addField (inputOutput, "value",     value ());
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

	value () .addInterest (this, &MetadataSet::set_value);

	set_value ();
}

template <>
void
MetadataSet::setMetaData <MFBool> (const std::string & name, const MFBool & boolean)
throw (Error <DISPOSED>)
{
	try
	{
		getMetaData <MFBool> (name) = boolean;
		return;
	}
	catch (const Error <INVALID_FIELD> &)
	{
		removeMetaData (name);
	}
	catch (const Error <INVALID_NAME> &)
	{ }
	catch (...)
	{
		throw;
	}

	const auto node = new MetadataBoolean (getExecutionContext ());

	node -> setup ();
	node -> name ()  = name;
	node -> value () = boolean;

	value () .emplace_back (node);
	addMetaData (value () .back ());
}

template <>
void
MetadataSet::setMetaData <MFDouble> (const std::string & name, const MFDouble & double_)
throw (Error <DISPOSED>)
{
	try
	{
		getMetaData <MFDouble> (name) = double_;
		return;
	}
	catch (const Error <INVALID_FIELD> &)
	{
		removeMetaData (name);
	}
	catch (const Error <INVALID_NAME> &)
	{ }
	catch (...)
	{
		throw;
	}

	const auto node = new MetadataDouble (getExecutionContext ());

	node -> setup ();
	node -> name ()  = name;
	node -> value () = double_;

	value () .emplace_back (node);
	addMetaData (value () .back ());
}

template <>
void
MetadataSet::setMetaData <MFFloat> (const std::string & name, const MFFloat & float_)
throw (Error <DISPOSED>)
{
	try
	{
		getMetaData <MFFloat> (name) = float_;
		return;
	}
	catch (const Error <INVALID_FIELD> &)
	{
		removeMetaData (name);
	}
	catch (const Error <INVALID_NAME> &)
	{ }
	catch (...)
	{
		throw;
	}

	const auto node = new MetadataFloat (getExecutionContext ());

	node -> setup ();
	node -> name ()  = name;
	node -> value () = float_;

	value () .emplace_back (node);
	addMetaData (value () .back ());
}

template <>
void
MetadataSet::setMetaData <MFInt32> (const std::string & name, const MFInt32 & integer)
throw (Error <DISPOSED>)
{
	try
	{
		getMetaData <MFInt32> (name) = integer;
		return;
	}
	catch (const Error <INVALID_FIELD> &)
	{
		removeMetaData (name);
	}
	catch (const Error <INVALID_NAME> &)
	{ }
	catch (...)
	{
		throw;
	}

	const auto node = new MetadataInteger (getExecutionContext ());

	node -> setup ();
	node -> name ()  = name;
	node -> value () = integer;

	value () .emplace_back (node);
	addMetaData (value () .back ());
}

template <>
void
MetadataSet::setMetaData <MFString> (const std::string & name, const MFString & string)
throw (Error <DISPOSED>)
{
	try
	{
		getMetaData <MFString> (name) = string;
		return;
	}
	catch (const Error <INVALID_FIELD> &)
	{
		removeMetaData (name);
	}
	catch (const Error <INVALID_NAME> &)
	{ }
	catch (...)
	{
		throw;
	}

	const auto node = new MetadataString (getExecutionContext ());

	node -> setup ();
	node -> name ()  = name;
	node -> value () = string;

	value () .emplace_back (node);
	addMetaData (value () .back ());
}

template <>
void
MetadataSet::setMetaData <MFNode> (const std::string & name, const MFNode & nodes)
throw (Error <DISPOSED>)
{
	try
	{
		getMetaData <MFNode> (name) = nodes;
		return;
	}
	catch (const Error <INVALID_FIELD> &)
	{
		removeMetaData (name);
	}
	catch (const Error <INVALID_NAME> &)
	{ }
	catch (...)
	{
		throw;
	}

	const auto node = new MetadataString (getExecutionContext ());

	node -> setup ();
	node -> name ()  = name;
	node -> value () = nodes;

	value () .emplace_back (node);
	addMetaData (value () .back ());
}

X3DFieldDefinition*
MetadataSet::getMetaData (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <DISPOSED>)
{
	const auto iter = metadataSet .find (name);

	if (iter not_eq metadataSet .end ())
		return iter -> second -> getField ("value");

	throw Error <INVALID_NAME> ("MetadataSet::getMetaData: Invalid name.");
}

void
MetadataSet::removeMetaData (const std::string & name)
throw (Error <DISPOSED>)
{
	const auto iter = std::remove_if (value () .begin (), value () .end (), [&name] (const SFNode &node)
	                                  {
	                                     const auto metadata = x3d_cast <X3DMetadataObject*> (node);

	                                     if (not metadata)
														 return false;

	                                     return metadata -> name () == name;
												 });

	value () .erase (iter, value () .end ());
}

void
MetadataSet::addMetaData (const SFNode & node)
{
	const auto metadataObject = dynamic_cast <X3DMetadataObject*> (node .getValue ());

	if (metadataObject)
		metadataSet .emplace (metadataObject -> name (), metadataObject) .first -> second .addParent (this);
}

void
MetadataSet::removeMetaData ()
throw (Error <DISPOSED>)
{
	for (const auto & pair : metadataSet)
		metadataSet .second -> removeParent (this);

	fieldIndex .clear ();
}

void
MetadataSet::set_value ()
{
	removeMetaData ();

	for (const auto & node : value ())
		addMetaData (node);
}

void
MetadataSet::dispose ()
{
	removeMetaData ();

	X3DMetadataObject::dispose ();
	X3DNode::dispose ();
}

} // X3D
} // titania

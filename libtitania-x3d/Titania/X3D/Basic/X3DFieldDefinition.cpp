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

#include "X3DFieldDefinition.h"

namespace titania {
namespace X3D {

X3DFieldDefinition::X3DFieldDefinition () :
	X3DChildObject (),               
	     reference (nullptr),        
	    accessType (initializeOnly), 
	     aliasName (),               
	   inputRoutes (),               
	  outputRoutes (),               
	     interests (),               
	        events ()                
{ }

X3DFieldDefinition*
X3DFieldDefinition::clone (X3DExecutionContext* const) const
{
	return clone ();
}

void
X3DFieldDefinition::setReference (X3DFieldDefinition* const value)
{
	reference = value;

	// create IS relationship
	switch (getAccessType () & reference -> getAccessType ())
	{
		case initializeOnly:
			write (*reference);
			break;
		case inputOnly:
			reference -> addInterest (this);
			break;
		case outputOnly:
			addInterest (reference);
			//write (reference);
			break;
		case inputOutput:
			write (*reference);
			reference -> addInterest (this);
			addInterest (reference);
			break;
	}
}

bool
X3DFieldDefinition::hasRoots (ChildObjectSet & seen)
{
	if (getParents () .size ())
	{
		for (auto & parent : getParents ())
			if (parent -> hasRoots (seen))
				return true;

		return false;
	}

	// this is a root node
	return true;
}

X3DFieldDefinition*
X3DFieldDefinition::getReference () const
{
	return reference;
}

void
X3DFieldDefinition::setAliasName (const std::string & value)
{
	aliasName = value;
}

const std::string &
X3DFieldDefinition::getAliasName () const
{
	return aliasName;
}

void
X3DFieldDefinition::setAccessType (const AccessType value)
{
	accessType = value;
}

AccessType
X3DFieldDefinition::getAccessType () const
{
	return accessType;
}

bool
X3DFieldDefinition::isInitializeable () const
{
	return accessType & initializeOnly;
}

bool
X3DFieldDefinition::isInput () const
{
	return accessType & inputOnly;
}

bool
X3DFieldDefinition::isOutput () const
{
	return accessType & outputOnly;
}

void
X3DFieldDefinition::addInputRoute (X3DRoute* const route)
{
	inputRoutes .insert (route);
}

void
X3DFieldDefinition::removeInputRoute (X3DRoute* const route)
{
	inputRoutes .erase (route);
}

const RouteSet &
X3DFieldDefinition::getInputRoutes () const
{
	return inputRoutes;
}

void
X3DFieldDefinition::addOutputRoute (X3DRoute* const route)
{
	outputRoutes .insert (route);
}

void
X3DFieldDefinition::removeOutputRoute (X3DRoute* const route)
{
	outputRoutes .erase (route);
}

const RouteSet &
X3DFieldDefinition::getOutputRoutes () const
{
	return outputRoutes;
}

void
X3DFieldDefinition::addInterest (X3DFieldDefinition* const interest)
{
	interests .insert (interest);
}

void
X3DFieldDefinition::addInterest (X3DFieldDefinition & interest)
{
	interests .insert (&interest);
}

void
X3DFieldDefinition::removeInterest (X3DFieldDefinition* const interest)
{
	interests .erase (interest);
}

void
X3DFieldDefinition::removeInterest (X3DFieldDefinition & interest)
{
	interests .erase (&interest);
}

void
X3DFieldDefinition::processEvents (ChildObjectSet & sourceFields)
{
	//	if (inputRoutes  .size ())
	sourceFields .insert (this);
	
	events .emplace_front (this);
	registerInterest (this);

	for (const auto & fieldDefinition : interests)
		fieldDefinition -> processEvent (this, sourceFields);
}

void
X3DFieldDefinition::processEvent (X3DFieldDefinition* const field, ChildObjectSet & sourceFields)
{
	if (not sourceFields .insert (this) .second)
		return;

	events .emplace_back (field);
	registerInterest (this);

	for (const auto & fieldDefinition : interests)
		fieldDefinition -> processEvent (field, sourceFields);
}

void
X3DFieldDefinition::processInterests ()
{
	for (const auto & event : FieldDefinitionArray (std::move (events)))
	{
		write (*event);
		X3DChildObject::processInterests ();
	}
}

void
X3DFieldDefinition::dispose ()
{
	for (const auto & route : inputRoutes)
		route -> disconnect ();

	for (const auto & route : outputRoutes)
		route -> disconnect ();

	inputRoutes  .clear ();
	outputRoutes .clear ();

	interests .clear ();
	events    .clear ();

	X3DChildObject::dispose ();
}

X3DFieldDefinition::~X3DFieldDefinition ()
{ }

} // X3D
} // titania

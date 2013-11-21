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

#include "../Routing/Route.h"

namespace titania {
namespace X3D {

X3DFieldDefinition::X3DFieldDefinition () :
	 X3DChildObject (),
	     references (),
	     accessType (initializeOnly),
	      aliasName (),
	    inputRoutes (),
	   outputRoutes (),
	 inputInterests (),
	outputInterests ()
{ }

X3DFieldDefinition &
X3DFieldDefinition::operator = (const X3DFieldDefinition & value)
{
	write (value);
	addEvent ();
	return *this;
}

bool
X3DFieldDefinition::hasRoots (ChildObjectSet & seen)
{
	if (getParents () .empty ())
		return true;

	for (auto & parent : getParents ())
		if (parent -> hasRoots (seen))
			return true;

	return false;
}

void
X3DFieldDefinition::addReference (X3DFieldDefinition* const reference)
{
	if (references .emplace (reference) .second)
	{
		// Create IS relationship

		switch (getAccessType () & reference -> getAccessType ())
		{
			case initializeOnly:
				break;
			case inputOnly:
				reference -> addInterest (this);
				break;
			case outputOnly:
				addInterest (reference);
				break;
			case inputOutput:
				reference -> addInterest (this);
				addInterest (reference);
				break;
		}

		updateReference (reference);
	}
}

void
X3DFieldDefinition::removeReference (X3DFieldDefinition* const reference)
{
	if (references .erase (reference))
	{
		// Remove IS relationship

		switch (getAccessType () & reference -> getAccessType ())
		{
			case initializeOnly:
				break;
			case inputOnly:
				reference -> removeInterest (this);
				break;
			case outputOnly:
				removeInterest (reference);
				break;
			case inputOutput:
				reference -> removeInterest (this);
				removeInterest (reference);
				break;
		}
	}
}

void
X3DFieldDefinition::updateReferences ()
{
	for (auto & reference : references)
		updateReference (reference);
}

void
X3DFieldDefinition::updateReference (X3DFieldDefinition* const reference)
{
	switch (getAccessType () & reference -> getAccessType ())
	{
		case inputOnly:
		case outputOnly:
			break;
		case initializeOnly:
		case inputOutput:
			write (*reference);
			break;
	}
}

void
X3DFieldDefinition::addInterest (X3DFieldDefinition* const fieldDefinition)
{
	outputInterests .emplace (fieldDefinition);
	fieldDefinition -> addInputInterest (this);
}

void
X3DFieldDefinition::addInterest (X3DFieldDefinition & fieldDefinition)
{
	outputInterests .emplace (&fieldDefinition);
	fieldDefinition .addInputInterest (this);
}

void
X3DFieldDefinition::removeInterest (X3DFieldDefinition* const fieldDefinition)
{
	outputInterests .erase (fieldDefinition);
	fieldDefinition -> removeInputInterest (this);
}

void
X3DFieldDefinition::removeInterest (X3DFieldDefinition & fieldDefinition)
{
	outputInterests .erase (&fieldDefinition);
	fieldDefinition .removeInputInterest (this);
}

void
X3DFieldDefinition::addInputInterest (X3DFieldDefinition* const fieldDefinition)
{
	inputInterests .emplace (fieldDefinition);
}

void
X3DFieldDefinition::removeInputInterest (X3DFieldDefinition* const fieldDefinition)
{
	inputInterests .erase (fieldDefinition);
}

void
X3DFieldDefinition::processEvent (const EventPtr & event)
{
	if (not event -> sources .emplace (this) .second)
		return;

	isTainted (false);

	if (event -> object not_eq this)
		write (*event -> object);

	processInterests ();

	bool first = true;

	for (const auto & fieldDefinition : outputInterests)
	{
		if (first)
		{
			first = false;
			fieldDefinition -> addEvent (fieldDefinition, event);
		}
		else
			fieldDefinition -> addEvent (fieldDefinition, EventPtr (new Event (*event)));
	}
}

void
X3DFieldDefinition::dispose ()
{
	references .clear ();

	for (const auto & route : RouteSet (std::move (inputRoutes)))
		route -> remove ();

	for (const auto & route : RouteSet (std::move (outputRoutes)))
		route -> remove ();
		
	for (auto & fieldDefinition : FieldDefinitionSet (std::move (inputInterests)))
		fieldDefinition -> removeInterest (this);

	for (auto & fieldDefinition : outputInterests) // No copy is made
		fieldDefinition -> removeInputInterest (this);

	inputInterests  .clear ();
	outputInterests .clear ();

	X3DChildObject::dispose ();
}

X3DFieldDefinition::~X3DFieldDefinition ()
{ }

} // X3D
} // titania

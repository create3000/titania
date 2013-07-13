/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
	     reference (NULL),
	    accessType (initializeOnly),
	     aliasName (),
	   inputRoutes (),
	  outputRoutes (),
	     interests ()
{ }

X3DFieldDefinition &
X3DFieldDefinition::operator = (const X3DFieldDefinition & value)
{
	write (value);
	addEvent ();
	return *this;
}

void
X3DFieldDefinition::setReference (X3DFieldDefinition* const value)
{
	reference = value;

	// create IS relationship
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

	updateReference ();
}

void
X3DFieldDefinition::updateReference ()
{
	if (reference)
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
}

void
X3DFieldDefinition::removeReference ()
{
	if (reference)
	{
		// remove IS relationship
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

		reference = NULL;
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

void
X3DFieldDefinition::processEvent (Event & event)
{
	if (not event .sources .insert (this) .second)
		return;

	isTainted (false);

	if (event .object not_eq this)
		write (*event .object);

	processInterests ();

	for (const auto & fieldDefinition : interests)
		fieldDefinition -> addEvent (fieldDefinition, event);
}

void
X3DFieldDefinition::dispose ()
{
	// removeReference ();

	for (const auto & route : RouteSet (std::move (inputRoutes)))
		route -> remove ();

	for (const auto & route : RouteSet (std::move (outputRoutes)))
		route -> remove ();

	interests .clear ();

	X3DChildObject::dispose ();
}

X3DFieldDefinition::~X3DFieldDefinition ()
{ }

} // X3D
} // titania

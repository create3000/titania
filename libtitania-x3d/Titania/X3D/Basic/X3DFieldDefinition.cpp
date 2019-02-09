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

#include "X3DFieldDefinition.h"

#include "../Routing/Route.h"

//#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

X3DFieldDefinition::X3DFieldDefinition () :
	X3DChildObject (),
	            data ()
{
	X3DChildObject::setup ();
}

void
X3DFieldDefinition::realize () const
{
	if (data)
		return;

	data .reset (new Data ());
}

X3DFieldDefinition &
X3DFieldDefinition::operator = (const X3DFieldDefinition & value)
{
	set (value);
	addEvent ();
	return *this;
}

bool
X3DFieldDefinition::hasRootedObjects (ChildObjectSet & seen)
{
	if (getParents () .empty ())
		return true;

	for (auto & parent : getParents ())
	{
		if (X3DChildObject::hasRootedObjects (parent, seen))
			return true;
	}

	return false;
}

void
X3DFieldDefinition::setAccessType (const AccessType value)
{
	realize ();

	data -> accessType = value;
}

AccessType
X3DFieldDefinition::getAccessType () const
{
	if (data)
		return data -> accessType;

	return AccessType::initializeOnly;
}

void
X3DFieldDefinition::setUnit (const UnitCategory value)
{
	realize ();

	data -> unit = value;
}

UnitCategory
X3DFieldDefinition::getUnit () const
{
	if (data)
		return data -> unit;

	return UnitCategory::NONE;
}

/// Returns true if is set during parse otherwise false;
void
X3DFieldDefinition::isGeospatial (const bool value)
{
	realize ();

	data -> flags .set (GEO_BIT, value);
}

bool
X3DFieldDefinition::isGeospatial () const
{
	if (data)
		return data -> flags [GEO_BIT];
	
	return false;
}

/// Returns true if is set during parse otherwise false;
void
X3DFieldDefinition::isSet (const bool value)
{
	realize ();

	data -> flags .set (IS_SET_BIT, value);
}

bool
X3DFieldDefinition::isSet () const
{
	if (data)
		return data -> flags [IS_SET_BIT];
	
	return false;
}

void
X3DFieldDefinition::isHidden (const bool value)
{
	realize ();

	data -> flags .set (HIDDEN_BIT, value);
}

bool
X3DFieldDefinition::isHidden () const
{
	if (data)
		return data -> flags [HIDDEN_BIT];
	
	return false;
}

///  Returns true if this field (from proto) is a reference for @a accesType (from node within proto), otherwise false.
bool
X3DFieldDefinition::isReference (const AccessType accessType) const
{
	return accessType == getAccessType () or accessType == inputOutput;
}

void
X3DFieldDefinition::addReference (X3DFieldDefinition* const reference)
{
	realize ();

	if (data -> references .emplace (reference) .second)
	{
		// Create IS relationship

		switch (getAccessType () & reference -> getAccessType ())
		{
			case initializeOnly:
				reference -> addInterest (this);
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

		updateIsReference (reference);
	}
}

void
X3DFieldDefinition::removeReference (X3DFieldDefinition* const reference)
{
	if (data)
	{
		if (data -> references .erase (reference))
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
}

///  Set fields value from proto instance field's value.
void
X3DFieldDefinition::updateReferences ()
{
	if (data)
	{
		for (auto & reference : data -> references)
			updateIsReference (reference);
	}
}

void
X3DFieldDefinition::updateIsReference (X3DFieldDefinition* const reference)
{
	switch (getAccessType () & reference -> getAccessType ())
	{
		case inputOnly:
		case outputOnly:
			break;
		case initializeOnly:
		case inputOutput:
			set (*reference);
			break;
	}
}

void
X3DFieldDefinition::addInterest (X3DFieldDefinition* const fieldDefinition) const
{
	realize ();

	data -> outputInterests .emplace (fieldDefinition);
	fieldDefinition -> addInputInterest (this);
}

void
X3DFieldDefinition::removeInterest (X3DFieldDefinition* const fieldDefinition) const
{
	if (data)
	{
		data -> outputInterests .erase (fieldDefinition);
		fieldDefinition -> removeInputInterest (this);
	}
}

void
X3DFieldDefinition::addInputInterest (const X3DFieldDefinition* const fieldDefinition) const
{
	realize ();

	data -> inputInterests .emplace (fieldDefinition);
}

void
X3DFieldDefinition::removeInputInterest (const X3DFieldDefinition* const fieldDefinition) const
{
	if (data)
		data -> inputInterests .erase (fieldDefinition);
}

void
X3DFieldDefinition::processEvent (const EventPtr & event)
{
	if (not event -> sources .emplace (this) .second)
		return;

	setTainted (false);

	if (event -> object not_eq this)
		set (*event -> object);

	std::vector <X3DFieldDefinition*> outputInterests;

	if (data)
		outputInterests .assign (data -> outputInterests .cbegin (), data -> outputInterests .cend ());

	processInterests ();

	if (data)
	{
		//event -> object = this; // TODO: This must be done probably.

		bool first = true;

		for (const auto & fieldDefinition : outputInterests)
		{
			if (first)
			{
				first = false;

				fieldDefinition -> addEventObject (fieldDefinition, event);
			}
			else
				fieldDefinition -> addEventObject (fieldDefinition, std::make_shared <Event> (*event));
		}
	}
}

void
X3DFieldDefinition::dispose ()
{
	if (data)
	{
		std::unique_ptr <Data> temp = std::move (data);

		for (const auto & route : temp -> inputRoutes)
			route -> erase ();

		for (const auto & route : temp -> outputRoutes)
			route -> erase ();

		for (auto & fieldDefinition : temp -> inputInterests)
			fieldDefinition -> removeInterest (this);

		for (auto & fieldDefinition : temp -> outputInterests)
			fieldDefinition -> removeInputInterest (this);
	}

	X3DChildObject::dispose ();
}

X3DFieldDefinition::~X3DFieldDefinition ()
{ }

} // X3D
} // titania

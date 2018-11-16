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

#include "PositionDamper2D.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType PositionDamper2D::component      = ComponentType::FOLLOWERS;
const std::string   PositionDamper2D::typeName       = "PositionDamper2D";
const std::string   PositionDamper2D::containerField = "children";

PositionDamper2D::Fields::Fields () :
	         set_value (new SFVec2f ()),
	   set_destination (new SFVec2f ()),
	      initialValue (new SFVec2f ()),
	initialDestination (new SFVec2f ()),
	     value_changed (new SFVec2f ())
{ }

PositionDamper2D::PositionDamper2D (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DDamperNode (),
	       fields (),
	       buffer ()
{
	addType (X3DConstants::PositionDamper2D);

	addField (inputOutput,    "metadata",           metadata ());
	addField (inputOnly,      "set_value",          set_value ());
	addField (inputOnly,      "set_destination",    set_destination ());
	addField (initializeOnly, "initialValue",       initialValue ());
	addField (initializeOnly, "initialDestination", initialDestination ());
	addField (initializeOnly, "order",              order ());
	addField (inputOutput,    "tau",                tau ());
	addField (inputOutput,    "tolerance",          tolerance ());
	addField (outputOnly,     "isActive",           isActive ());
	addField (outputOnly,     "value_changed",      value_changed ());
}

X3DBaseNode*
PositionDamper2D::create (X3DExecutionContext* const executionContext) const
{
	return new PositionDamper2D (executionContext);
}

void
PositionDamper2D::initialize ()
{
	X3DDamperNode::initialize ();

	set_value ()       .addInterest (&PositionDamper2D::set_value_, this);
	set_destination () .addInterest (&PositionDamper2D::set_destination_, this);
	order ()           .addInterest (&PositionDamper2D::set_order, this);

	buffer .resize (getOrder () + 1, initialValue ());

	buffer [0] = initialDestination ();

	if (equals (initialDestination (), initialValue (), getTolerance ()))
		value_changed () = initialDestination ();

	else
		set_active (true);
}

bool
PositionDamper2D::equals (const Vector2f & lhs, const Vector2f & rhs, const float tolerance) const
{
	return abs (lhs - rhs) < tolerance;
}

void
PositionDamper2D::set_value_ ()
{
	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
		value = set_value ();

	value_changed () = set_value ();

	set_active (true);
}

void
PositionDamper2D::set_destination_ ()
{
	buffer [0] = set_destination ();

	set_active (true);
}

void
PositionDamper2D::set_order ()
{
	buffer .resize (getOrder () + 1, buffer .back ());
}

void
PositionDamper2D::prepareEvents ()
{
	size_t order = buffer .size () - 1;

	if (tau ())
	{
		const time_type delta = 1 / getBrowser () -> getCurrentFrameRate ();

		const float alpha = std::exp (-delta / tau ());
		
		for (size_t i = 0; i < order; ++ i)
		{
			buffer [i + 1] = lerp (buffer [i], buffer [i + 1], alpha);
		}

		value_changed () = buffer [order];

		if (not equals (buffer [order], buffer [0], getTolerance ()))
			return;
	}
	else
	{
		value_changed () = buffer [0];

		order = 0;
	}

	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
		value = buffer [order];

	set_active (false);
}

} // X3D
} // titania

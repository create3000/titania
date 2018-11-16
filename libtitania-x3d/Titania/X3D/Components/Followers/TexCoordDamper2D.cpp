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

#include "TexCoordDamper2D.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType TexCoordDamper2D::component      = ComponentType::FOLLOWERS;
const std::string   TexCoordDamper2D::typeName       = "TexCoordDamper2D";
const std::string   TexCoordDamper2D::containerField = "children";

TexCoordDamper2D::Fields::Fields () :
	         set_value (new MFVec2f ()),
	   set_destination (new MFVec2f ()),
	      initialValue (new MFVec2f ()),
	initialDestination (new MFVec2f ()),
	     value_changed (new MFVec2f ())
{ }

TexCoordDamper2D::TexCoordDamper2D (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DDamperNode (),
	       fields (),
	       buffer ()
{
	addType (X3DConstants::TexCoordDamper2D);

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
TexCoordDamper2D::create (X3DExecutionContext* const executionContext) const
{
	return new TexCoordDamper2D (executionContext);
}

void
TexCoordDamper2D::initialize ()
{
	X3DDamperNode::initialize ();

	set_value ()       .addInterest (&TexCoordDamper2D::set_value_, this);
	set_destination () .addInterest (&TexCoordDamper2D::set_destination_, this);
	order ()           .addInterest (&TexCoordDamper2D::set_order, this);

	buffer .resize (getOrder () + 1);

	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
	{
		value .assign (initialValue () .cbegin (), initialValue () .cend ());
		value .resize (initialDestination () .size ());
	}

	buffer [0] .assign (initialDestination () .cbegin (), initialDestination () .cend ());

	if (equals (initialDestination (), initialValue (), getTolerance ()))
		value_changed () = initialDestination ();

	else
		set_active (true);
}

void
TexCoordDamper2D::set_value_ ()
{
	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
		value .assign (set_value () .begin (), set_value () .end ());

	buffer [0] .resize (set_value () .size ());

	value_changed () = set_value ();

	set_active (true);
}

void
TexCoordDamper2D::set_destination_ ()
{
	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
		value .resize (set_destination () .size ());

	buffer [0] .assign (set_destination () .cbegin (), set_destination () .cend ());

	set_active (true);
}

void
TexCoordDamper2D::set_order ()
{
	buffer .resize (getOrder () + 1, buffer .back ());
}

void
TexCoordDamper2D::prepareEvents ()
{
	size_t order = buffer .size () - 1;

	if (tau ())
	{
		const time_type delta = 1 / getBrowser () -> getCurrentFrameRate ();

		const float alpha = std::exp (-delta / tau ());
		
		for (size_t i = 0; i < order; ++ i)
		{
			for (size_t j = 0, s = buffer [i] .size (); j < s; ++ j)
			{
				buffer [i + 1] [j] = lerp (buffer [i] [j], buffer [i + 1] [j], alpha);
			}
		}

		value_changed () .assign (buffer [order] .cbegin (), buffer [order] .cend ());

		if (not equals (buffer [order], buffer [0], getTolerance ()))
			return;
	}
	else
	{
		value_changed () .assign (buffer [0] .cbegin (), buffer [0] .cend ());

		order = 0;
	}

	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
		value = buffer [order];

	set_active (false);
}

} // X3D
} // titania

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

#include "OrientationDamper.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string OrientationDamper::componentName  = "Followers";
const std::string OrientationDamper::typeName       = "OrientationDamper";
const std::string OrientationDamper::containerField = "children";

OrientationDamper::Fields::Fields () :
	         set_value (new SFRotation ()),
	   set_destination (new SFRotation ()),
	      initialValue (new SFRotation ()),
	initialDestination (new SFRotation ()),
	     value_changed (new SFRotation ())
{ }

OrientationDamper::OrientationDamper (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DDamperNode (),
	       fields (),
	       buffer ()
{
	addType (X3DConstants::OrientationDamper);

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
OrientationDamper::create (X3DExecutionContext* const executionContext) const
{
	return new OrientationDamper (executionContext);
}

void
OrientationDamper::initialize ()
{
	X3DDamperNode::initialize ();

	set_value ()       .addInterest (&OrientationDamper::set_value_, this);
	set_destination () .addInterest (&OrientationDamper::set_destination_, this);
	order ()           .addInterest (&OrientationDamper::set_order, this);

	buffer .resize (getOrder () + 1, initialValue ());

	buffer [0] = initialDestination ();

	if (equals (initialDestination (), initialValue (), getTolerance ()))
		value_changed () = initialDestination ();

	else
		set_active (true);
}

bool
OrientationDamper::equals (const Rotation4d & lhs, const Rotation4d & rhs, const double tolerance) const
{
	const Rotation4d d = ~rhs * lhs;

	return abs (d .angle ()) < tolerance;
}

void
OrientationDamper::set_value_ ()
{
	for (auto & value : std::pair (buffer .begin () + 1, buffer .end ()))
		value = set_value ();

	value_changed () = set_value ();

	set_active (true);
}

void
OrientationDamper::set_destination_ ()
{
	buffer [0] = set_destination ();

	set_active (true);
}

void
OrientationDamper::set_order ()
{
	buffer .resize (getOrder () + 1, buffer .back ());
}

void
OrientationDamper::prepareEvents ()
{
	size_t order = buffer .size () - 1;

	if (tau ())
	{
		const time_type delta = 1 / getBrowser () -> getCurrentFrameRate ();
		const double    alpha = std::exp (-delta / tau ());
		
		for (size_t i = 0; i < order; ++ i)
		{
			try
			{
				buffer [i + 1] = slerp (buffer [i], buffer [i + 1], alpha);
			}
			catch (const std::domain_error &)
			{ }
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

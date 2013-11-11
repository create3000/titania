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
	        value ()
{
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

	set_value ()       .addInterest (this, &OrientationDamper::_set_value);
	set_destination () .addInterest (this, &OrientationDamper::_set_destination);
	order ()           .addInterest (this, &OrientationDamper::set_order);

	value .resize (getOrder () + 1, initialValue ());

	value [0] = initialDestination ();

	set_active (not equals (initialDestination (), initialValue (), getTolerance ()));
}

bool
OrientationDamper::equals (const Rotation4f & lhs, const Rotation4f & rhs, float tolerance) const
{
	Rotation4f d = ~rhs * lhs;

	return abs (d .angle ()) < tolerance;
}

void
OrientationDamper::_set_value ()
{
	for (auto & v : basic::adapter (value .begin () + 1, value .end ()))
		v = set_value ();

	value_changed () = set_value ();

	set_active (true);
}

void
OrientationDamper::_set_destination ()
{
	if (not equals (value [0], set_destination (), getTolerance ()))
	{
		value [0] = set_destination ();

		set_active (true);
	}
}

void
OrientationDamper::set_order ()
{
	value .resize (getOrder () + 1, value .back ());
}

void
OrientationDamper::prepareEvents ()
{
	size_t order = value .size () - 1;

	if (tau ())
	{
		time_type delta = 1 / getBrowser () -> getCurrentFrameRate ();

		float alpha = std::exp (-delta / tau ());
		
		for (size_t i = 0; i < order; ++ i)
		{
			value [i + 1] = slerp (value [i], value [i + 1], alpha);
		}

		value_changed () = value [order];

		if (not equals (value [order], value [0], getTolerance ()))
			return;
	}
	else
	{
		value_changed () = value [0];

		order = 0;
	}

	for (auto & v : basic::adapter (value .begin () + 1, value .end ()))
		v = value [order];

	set_active (false);
}

} // X3D
} // titania

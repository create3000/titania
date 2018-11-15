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

#include "X3DInterpolatorNode.h"

namespace titania {
namespace X3D {

X3DInterpolatorNode::Fields::Fields () :
	set_fraction (new SFFloat ()),
	         key (new MFFloat ())
{ }

X3DInterpolatorNode::X3DInterpolatorNode () :
	X3DChildNode (),
	      fields ()
{
	addType (X3DConstants::X3DInterpolatorNode);
}

void
X3DInterpolatorNode::setup ()
{
	// If an X3DInterpolatorNode value_changed outputOnly field is read before it receives any inputs,
	// keyValue[0] is returned if keyValue is not empty. If keyValue is empty (i.e., [ ]), the initial
	// value for the respective field type is returned (EXAMPLE  (0, 0, 0) for SFVec3f);

	set_key ();

	if (not key () .empty ())
		interpolate (0, 0, 0);

	X3DChildNode::setup ();
}

void
X3DInterpolatorNode::initialize ()
{
	X3DChildNode::initialize ();

	set_fraction () .addInterest (&X3DInterpolatorNode::set_fraction_, this);
	key ()          .addInterest (&X3DInterpolatorNode::set_key, this);
}

void
X3DInterpolatorNode::set_fraction_ ()
{
	if (key () .empty ())
		return;

	if (key () .size () == 1)
		return interpolate (0, 0, 0);

	if (set_fraction () <= key () .get1Value (0))
		return interpolate (0, 1, 0);

	if (set_fraction () >= key () [key () .size () - 1])
		return interpolate (key () .size () - 2, key () .size () - 1, 1);

	const auto iter = std::upper_bound (key () .cbegin (), key () .cend (), set_fraction ());

	if (iter not_eq key () .cend ())
	{
		const size_t index1 = iter - key () .cbegin ();
		const size_t index0 = index1 - 1;
		const float  weight = (set_fraction () - key () .get1Value (index0)) / (key () .get1Value (index1) - key () .get1Value (index0));

		interpolate (index0, index1, std::clamp (weight, 0.0f, 1.0f));
	}
}

void
X3DInterpolatorNode::set_key ()
{
	set_keyValue ();
}

} // X3D
} // titania

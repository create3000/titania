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

#include "CoordinateInterpolator.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string CoordinateInterpolator::componentName  = "Interpolation";
const std::string CoordinateInterpolator::typeName       = "CoordinateInterpolator";
const std::string CoordinateInterpolator::containerField = "children";

CoordinateInterpolator::Fields::Fields () :
	     keyValue (new MFVec3f ()),
	value_changed (new MFVec3f ())
{ }

CoordinateInterpolator::CoordinateInterpolator (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DInterpolatorNode (),
	             fields ()
{
	addType (X3DConstants::CoordinateInterpolator);

	addField (inputOutput, "metadata",      metadata ());
	addField (inputOnly,   "set_fraction",  set_fraction ());
	addField (inputOutput, "key",           key ());
	addField (inputOutput, "keyValue",      keyValue ());
	addField (outputOnly,  "value_changed", value_changed ());
}

X3DBaseNode*
CoordinateInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new CoordinateInterpolator (executionContext);
}

void
CoordinateInterpolator::initialize ()
{
	X3DInterpolatorNode::initialize ();
}

void
CoordinateInterpolator::set_keyValue ()
{ }

void
CoordinateInterpolator::interpolate (size_t index0, size_t index1, const float weight)
{
	const size_t size = key () .empty () ? 0 : keyValue () .size () / key () .size ();

	index0 *= size;
	index1  = index0 + (key () .size () > 1 ? size : 0);

	value_changed () .resize (size);

	for (size_t i = 0; i < size; ++ i)
	{
		value_changed () [i] = math::lerp <Vector3f> (keyValue () .get1Value (index0 + i),
		                                              keyValue () .get1Value (index1 + i),
		                                              weight);
	}
}

} // X3D
} // titania

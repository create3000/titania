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

#include "FogCoordinate.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   FogCoordinate::component      = Component ("EnvironmentalEffects", 4);
const std::string FogCoordinate::typeName       = "FogCoordinate";
const std::string FogCoordinate::containerField = "fogCoord";

FogCoordinate::Fields::Fields () :
	depth (new MFFloat ())
{ }

FogCoordinate::FogCoordinate (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometricPropertyNode (),
	                  fields ()
{
	addType (X3DConstants::FogCoordinate);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "depth",    depth ());

	depth () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
FogCoordinate::create (X3DExecutionContext* const executionContext) const
{
	return new FogCoordinate (executionContext);
}

void
FogCoordinate::set1Depth (const size_t index, const float value)
{
	if (not depth () .empty ())
	{
		if (index >= depth () .size ())
			depth () .resize (index, depth () .back ());
	}

	depth () .set1Value (index, value);
}

float
FogCoordinate::get1Depth (const size_t index) const
{
	if (index < depth () .size ())
		return depth () [index];

	else if (depth () .size ())
		return depth () .back ();

	else
		return 0;
}

void
FogCoordinate::addDepth (std::vector <float> & depths, const size_t index) const
{
	if (index < depth () .size ())
		depths .emplace_back (depth () [index]);

	else if (depth () .size ())
		depths .emplace_back (depth () .back ());

	else
		depths .emplace_back (0);
}

void
FogCoordinate::assignDepths (const std::vector <float> & depths)
{
	depth () .assign (depths .begin (), depths .end ());
}

void
FogCoordinate::eraseDepth (const size_t index)
{
	if (index < depth () .size ())
		depth () .erase (depth () .begin () + index);
}

void
FogCoordinate::resize (size_t size)
{
	if (depth () .empty ())
		depth () .resize (size);

	else
		depth () .resize (size, depth () .back ());
}

} // X3D
} // titania

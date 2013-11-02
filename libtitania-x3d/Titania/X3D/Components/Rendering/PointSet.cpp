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

#include "PointSet.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"

namespace titania {
namespace X3D {

const std::string PointSet::componentName  = "Rendering";
const std::string PointSet::typeName       = "PointSet";
const std::string PointSet::containerField = "geometry";

PointSet::Fields::Fields () :
	attrib (new MFNode ()),
	color (new SFNode ()),
	coord (new SFNode ()),
	fogCoord (new SFNode ())
{ }

PointSet::PointSet (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "attrib",   attrib ());
	addField (inputOutput, "color",    color ());
	addField (inputOutput, "coord",    coord ());
	addField (inputOutput, "fogCoord", fogCoord ());
}

X3DBaseNode*
PointSet::create (X3DExecutionContext* const executionContext) const
{
	return new PointSet (executionContext);
}

bool
PointSet::isTransparent () const
{
	auto _color = x3d_cast <X3DColorNode*> (color ());

	return _color and _color -> isTransparent ();
}

void
PointSet::build ()
{
	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	if (not _coord)
		return;

	auto _color = x3d_cast <X3DColorNode*> (color ());

	if (_color)
	{
		getColors () .reserve (_coord -> size ());
		
		for (size_t i = 0, size = _color -> size (); i < size; ++ i)
			_color -> emplace_back (getColors (), i);
		
		getColors () .resize  (_coord -> size (), Color4f (1, 1, 1, 1));
	}

	for (size_t i = 0, size = _coord -> size (); i < size; ++ i)
		_coord -> emplace_back (getVertices (), i);

	addElements (GL_POINTS, getVertices () .size ());
	setSolid (false);
}

void
PointSet::draw ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw ();
}

} // X3D
} // titania

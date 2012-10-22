/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "PointSet.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/Color.h"
#include "../Rendering/ColorRGBA.h"
#include "../Rendering/Coordinate.h"

namespace titania {
namespace X3D {

PointSet::PointSet (X3DExecutionContext* const executionContext) :
	   X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),                                 
	         attrib (),                                 // MFNode[in,out] attrib    [ ]         [X3DVertexAttributeNode]
	          color (),                                 // SFNode [in,out] color     NULL        [X3DColorNode]
	          coord (),                                 // SFNode [in,out] coord     NULL        [X3DCoordinateNode]
	       fogCoord ()                                  // SFNode [in,out] fogCoord  [ ]         [FogCoordinate]
{
	setComponent ("Rendering");
	setTypeName ("PointSet");

	appendField (inputOutput, "metadata", metadata);
	appendField (inputOutput, "attrib",   attrib);
	appendField (inputOutput, "color",    color);
	appendField (inputOutput, "coord",    coord);
	appendField (inputOutput, "fogCoord", fogCoord);
}

X3DBasicNode*
PointSet::create (X3DExecutionContext* const executionContext) const
{
	return new PointSet (executionContext);
}

void
PointSet::build ()
{
	X3DGeometryNode::build ();

	SFNode <Coordinate> _coord = coord;

	if (not _coord or not _coord -> point .size ())
		return;

	SFNode <Color>     _color     = color;
	SFNode <ColorRGBA> _colorRGBA = color;

	if (_color)
	{
		setGLNumColors (3);

		float r, g, b;

		size_t i;

		for (i = 0; i < _color -> color .size (); i ++)
		{
			_color -> color .at (i) .getValue (r, g, b);
			getGLColors () -> push_back (r);
			getGLColors () -> push_back (g);
			getGLColors () -> push_back (b);
		}

		for ( ; i < _coord -> point .size (); i ++)
		{
			getGLColors () -> push_back (1);
			getGLColors () -> push_back (1);
			getGLColors () -> push_back (1);
		}
	}
	else if (_colorRGBA)
	{
		setGLNumColors (4);

		float r, g, b, a;

		size_t i;

		for (i = 0; i < _color -> color .size (); i ++)
		{
			_colorRGBA -> color .at (i) .getValue (r, g, b, a);
			getGLColors () -> push_back (r);
			getGLColors () -> push_back (g);
			getGLColors () -> push_back (b);
			getGLColors () -> push_back (a);
		}

		for ( ; i < _coord -> point .size (); i ++)
		{
			getGLColors () -> push_back (1);
			getGLColors () -> push_back (1);
			getGLColors () -> push_back (1);
			getGLColors () -> push_back (1);
		}
	}

	float x, y, z;

	for (const auto & point : _coord -> point)
	{
		point .getValue (x, y, z);
		getGLPoints () -> push_back (x);
		getGLPoints () -> push_back (y);
		getGLPoints () -> push_back (z);
	}

	setGLMode (GL_POINTS);
	setGLIndices (_coord -> point .size ());
}

void
PointSet::draw ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw ();
}

} // X3D
} // titania

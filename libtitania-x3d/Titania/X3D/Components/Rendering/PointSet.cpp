/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
	   X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	         attrib (),                                                    // MFNode[in,out] attrib    [ ]         [X3DVertexAttributeNode]
	          color (),                                                    // SFNode [in,out] color     NULL        [X3DColorNode]
	          coord (),                                                    // SFNode [in,out] coord     NULL        [X3DCoordinateNode]
	       fogCoord ()                                                     // SFNode [in,out] fogCoord  [ ]         [FogCoordinate]
{
	setComponent ("Rendering");
	setTypeName ("PointSet");

	addField (inputOutput, "metadata", metadata);
	addField (inputOutput, "attrib",   attrib);
	addField (inputOutput, "color",    color);
	addField (inputOutput, "coord",    coord);
	addField (inputOutput, "fogCoord", fogCoord);
}

X3DBaseNode*
PointSet::create (X3DExecutionContext* const executionContext) const
{
	return new PointSet (executionContext);
}

void
PointSet::build ()
{
	SFNode <Coordinate> _coord = coord;

	if (not _coord)
		return;

	SFNode <Color>     _color     = color;
	SFNode <ColorRGBA> _colorRGBA = color;

	if (_color)
	{
		getColors () .reserve (_coord -> point .size ());
		getColors () .assign  (_color -> color .begin (), _color -> color .end ());		
		getColors () .resize  (_coord -> point .size (), Color3f (1, 1, 1));
	}
	else if (_colorRGBA)
	{
		getColorsRGBA () .reserve (_coord -> point .size ());
		getColorsRGBA () .assign  (_colorRGBA -> color .begin (), _colorRGBA -> color .end ());		
		getColorsRGBA () .resize  (_coord -> point .size (), Color4f (1, 1, 1, 1));
	}

	getVertices () .assign (_coord -> point .begin (), _coord -> point .end ());

	setVertexMode (GL_POINTS);
}

void
PointSet::display ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::display ();
}

} // X3D
} // titania

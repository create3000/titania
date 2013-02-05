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

#include "GeoElevationGrid.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

GeoElevationGrid::GeoElevationGrid (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	     set_height (),                                                    // MFDouble [in]     set_height
	          color (),                                                    // SFNode   [in,out] color            NULL               [X3DColorNode]
	         normal (),                                                    // SFNode   [in,out] normal           NULL               [X3DNormalNode]
	       texCoord (),                                                    // SFNode   [in,out] texCoord         NULL               [X3DTextureCoordinateNode]
	         yScale (1),                                                   // SFFloat  [in,out] yScale           1.0                [0,∞)
	 colorPerVertex (true),                                                // SFBool   [ ]      colorPerVertex   TRUE
	    creaseAngle (),                                                    // SFDouble [ ]      creaseAngle      0                  [0,∞)
	  geoGridOrigin (),                                                    // SFVec3d  [ ]      geoGridOrigin    0 0 0              (-∞,∞)
	      geoOrigin (),                                                    // SFNode   [ ]      geoOrigin        NULL               [GeoOrigin]
	      geoSystem ({ "GD", "WE" }),                                      // MFString [ ]      geoSystem        ["GD","WE"]        [see <a href="#Specifyingaspatialreference">25.2.3</a>]
	         height ({ 0, 0 }),                                            // MFDouble [ ]      height           [0 0]              (-∞,∞)
	normalPerVertex (true),                                                // SFBool   [ ]      normalPerVertex  TRUE
	          solid (true),                                                // SFBool   [ ]      solid            TRUE
	     xDimension (),                                                    // SFInt32  [ ]      xDimension       0                  (0,∞)
	       xSpacing (1),                                                   // SFDouble [ ]      xSpacing         1.0                [0,∞)
	     zDimension (),                                                    // SFInt32  [ ]      zDimension       0                  (0,∞)
	       zSpacing (1)                                                    // SFDouble [ ]      zSpacing         1.0                [0,∞)
{
	setComponent ("Geospatial");
	setTypeName ("GeoElevationGrid");

	addField (inputOutput,    "metadata",        metadata);
	addField (inputOnly,      "set_height",      set_height);
	addField (inputOutput,    "color",           color);
	addField (inputOutput,    "normal",          normal);
	addField (inputOutput,    "texCoord",        texCoord);
	addField (inputOutput,    "yScale",          yScale);
	addField (initializeOnly, "ccw",             ccw);
	addField (initializeOnly, "colorPerVertex",  colorPerVertex);
	addField (initializeOnly, "creaseAngle",     creaseAngle);
	addField (initializeOnly, "geoGridOrigin",   geoGridOrigin);
	addField (initializeOnly, "geoOrigin",       geoOrigin);
	addField (initializeOnly, "geoSystem",       geoSystem);
	addField (initializeOnly, "height",          height);
	addField (initializeOnly, "normalPerVertex", normalPerVertex);
	addField (initializeOnly, "solid",           solid);
	addField (initializeOnly, "xDimension",      xDimension);
	addField (initializeOnly, "xSpacing",        xSpacing);
	addField (initializeOnly, "zDimension",      zDimension);
	addField (initializeOnly, "zSpacing",        zSpacing);
}

X3DBaseNode*
GeoElevationGrid::create (X3DExecutionContext* const executionContext) const
{
	return new GeoElevationGrid (executionContext);
}

} // X3D
} // titania

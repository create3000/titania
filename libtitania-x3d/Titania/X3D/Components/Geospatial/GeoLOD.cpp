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
 ******************************************************************************/

#include "GeoLOD.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

GeoLOD::GeoLOD (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	    X3DChildNode (),                                                    
	X3DBoundedObject (),                                                    
	        children (),                                                    // MFNode   [out] children                          [ ]
	   level_changed (),                                                    // SFInt32  [out] level_changed
	          center (),                                                    // SFVec3d  [ ]   center         0 0 0              (-∞,∞)
	       child1Url (),                                                    // MFString [ ]   child1Url      [ ]                 [URI]
	       child2Url (),                                                    // MFString [ ]   child2Url      [ ]                 [URI]
	       child3Url (),                                                    // MFString [ ]   child3Url      [ ]                 [URI]
	       child4Url (),                                                    // MFString [ ]   child4Url      [ ]                 [URI]
	       geoOrigin (),                                                    // SFNode   [ ]   geoOrigin      NULL               [GeoOrigin]
	       geoSystem ({ "GD", "WE" }),                                      // MFString [ ]   geoSystem      ["GD","WE"]        [see <a href="#Specifyingaspatialreference">25.2.3</a>]
	           range (10),                                                  // SFFloat  [ ]   range          10                 [0,∞)
	         rootUrl (),                                                    // MFString [ ]   rootUrl        [ ]                 [URI]
	        rootNode ()                                                     // MFNode   [ ]   rootNode       [ ]                 [X3DChildNode]
{
	setComponent ("Geospatial");
	setTypeName ("GeoLOD");

	addField (inputOutput,    "metadata",      metadata);
	addField (initializeOnly, "bboxSize",      bboxSize);
	addField (initializeOnly, "bboxCenter",    bboxCenter);
	addField (outputOnly,     "children",      children);
	addField (outputOnly,     "level_changed", level_changed);
	addField (initializeOnly, "center",        center);
	addField (initializeOnly, "child1Url",     child1Url);
	addField (initializeOnly, "child2Url",     child2Url);
	addField (initializeOnly, "child3Url",     child3Url);
	addField (initializeOnly, "child4Url",     child4Url);
	addField (initializeOnly, "geoOrigin",     geoOrigin);
	addField (initializeOnly, "geoSystem",     geoSystem);
	addField (initializeOnly, "range",         range);
	addField (initializeOnly, "rootUrl",       rootUrl);
	addField (initializeOnly, "rootNode",      rootNode);
}

X3DBaseNode*
GeoLOD::create (X3DExecutionContext* const executionContext) const
{
	return new GeoLOD (executionContext);
}

void
GeoLOD::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();
}

Box3f
GeoLOD::getBBox ()
{
	return Box3f ();
}

void
GeoLOD::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania

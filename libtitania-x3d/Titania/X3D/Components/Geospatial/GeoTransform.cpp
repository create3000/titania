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

#include "GeoTransform.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Tools/Geospatial/GeoTransformTool.h"

namespace titania {
namespace X3D {

const ComponentType GeoTransform::component      = ComponentType::GEOSPATIAL;
const std::string   GeoTransform::typeName       = "GeoTransform";
const std::string   GeoTransform::containerField = "children";

GeoTransform::Fields::Fields () :
	     translation (new SFVec3f ()),
	        rotation (new SFRotation ()),
	           scale (new SFVec3f (1, 1, 1)),
	scaleOrientation (new SFRotation ()),
	       geoCenter (new SFVec3d ())
{ }

GeoTransform::GeoTransform (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTransformMatrix4DNode (),
	     X3DGeospatialObject (),
	                  fields ()
{
	addType (X3DConstants::GeoTransform);

	addField (inputOutput,    "metadata",         metadata ());
	addField (initializeOnly, "geoSystem",        geoSystem ());
	addField (inputOutput,    "translation",      translation ());
	addField (inputOutput,    "rotation",         rotation ());
	addField (inputOutput,    "scale",            scale ());
	addField (inputOutput,    "scaleOrientation", scaleOrientation ());
	addField (inputOutput,    "geoCenter",        geoCenter ());
	addField (initializeOnly, "geoOrigin",        geoOrigin ());
	addField (initializeOnly, "bboxSize",         bboxSize ());
	addField (initializeOnly, "bboxCenter",       bboxCenter ());
	addField (inputOnly,      "addChildren",      addChildren ());
	addField (inputOnly,      "removeChildren",   removeChildren ());
	addField (inputOutput,    "children",         children ());
}

X3DBaseNode*
GeoTransform::create (X3DExecutionContext* const executionContext) const
{
	return new GeoTransform (executionContext);
}

void
GeoTransform::initialize ()
{
	X3DTransformMatrix4DNode::initialize ();
	X3DGeospatialObject::initialize ();

	addInterest (this, &GeoTransform::eventsProcessed);

	eventsProcessed ();
}

void
GeoTransform::eventsProcessed ()
{
	Matrix4d transformation;

	transformation .set (translation () .getValue (),
	                     rotation () .getValue (),
	                     scale () .getValue (),
	                     scaleOrientation () .getValue ());

	setMatrix (transformation * getLocationMatrix (geoCenter ()));
}

void
GeoTransform::addTool ()
{
	X3DTransformMatrix4DNode::addTool (new GeoTransformTool (this));
}

void
GeoTransform::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DTransformMatrix4DNode::dispose ();
}

} // X3D
} // titania

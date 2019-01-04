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

#include "GeoOrigin.h"

#include "../../Browser/Geospatial/Geospatial.h"
#include "../../Execution/X3DScene.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

// GeoOrigin node usage is deprecated and its use is discouraged. The presence of a GeoOrigin
// node is tolerated but can be ignored in X3D scenes having version 3.0, 3.1 or 3.2.
// GeoOrigin node is not allowed in X3D scenes having version 3.3 or higher.

const std::string GeoOrigin::componentName  = "Geospatial";
const std::string GeoOrigin::typeName       = "GeoOrigin";
const std::string GeoOrigin::containerField = "geoOrigin";

GeoOrigin::Fields::Fields () :
	rotateYUp (new SFBool ()),
	geoSystem (new MFString ({ "GD", "WE" })),
	geoCoords (new SFVec3d ())
{ }

GeoOrigin::GeoOrigin (X3DExecutionContext* const executionContext) :
	   X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       X3DNode (),
	        fields (),
	referenceFrame (),
	       radians (false)
{
	addType (X3DConstants::GeoOrigin);

	addField (inputOutput,    "metadata",  metadata ());
	addField (initializeOnly, "geoSystem", geoSystem ());
	addField (inputOutput,    "geoCoords", geoCoords ());
	addField (initializeOnly, "rotateYUp", rotateYUp ());

	geoCoords () .setUnit (UnitCategory::LENGTH);
	geoCoords () .isGeospatial (true);
}

X3DBaseNode*
GeoOrigin::create (X3DExecutionContext* const executionContext) const
{
	return new GeoOrigin (executionContext);
}

void
GeoOrigin::initialize ()
{
	X3DNode::initialize ();

	geoSystem () .addInterest (&GeoOrigin::set_geoSystem, this);

	set_geoSystem ();
}

void
GeoOrigin::set_geoSystem ()
{
	referenceFrame = Geospatial::getReferenceFrame (geoSystem (), radians);
}

Vector3d
GeoOrigin::getOrigin () const
{
	return referenceFrame -> convert (geoCoords ());
}

} // X3D
} // titania

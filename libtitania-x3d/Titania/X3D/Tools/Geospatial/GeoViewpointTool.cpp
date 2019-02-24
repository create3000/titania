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

#include "GeoViewpointTool.h"

#include "../Grouping/X3DTransformNodeTool.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

GeoViewpointTool::GeoViewpointTool (X3DBaseNode* const node) :
	            X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	           GeoViewpoint (node -> getExecutionContext ()),
	            X3DBaseTool (node),
	   X3DViewpointNodeTool (),
	X3DGeospatialObjectTool ()
{
	addType (X3DConstants::GeoViewpointTool);
}

void
GeoViewpointTool::initialize ()
{
	X3DViewpointNodeTool::initialize ();
	X3DGeospatialObjectTool::initialize ();
}

void
GeoViewpointTool::realize ()
{
	X3DViewpointNodeTool::realize ();

	const auto & transformTool = getTransformTools () [0];

	getNode <GeoViewpoint> () -> position ()    .addInterest (&GeoViewpointTool::set_geo_position,    this);
	getNode <GeoViewpoint> () -> orientation () .addInterest (&GeoViewpointTool::set_geo_orientation, this);

	transformTool -> translation () .addInterest (&GeoViewpointTool::set_translation, this);
	transformTool -> rotation ()    .addInterest (&GeoViewpointTool::set_rotation,    this);

	set_geo_position ();
	set_geo_orientation ();
}

void
GeoViewpointTool::set_geo_position ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> translation () = getNode <GeoViewpoint> () -> getPosition ();

	transformTool -> translation () .removeInterest (&GeoViewpointTool::set_translation, this);
	transformTool -> translation () .addInterest (&GeoViewpointTool::connectTranslation, this);
}

void
GeoViewpointTool::set_geo_orientation ()
{
	const auto & transformTool = getTransformTools () [0];

	transformTool -> rotation () = getNode <GeoViewpoint> () -> getOrientation ();

	transformTool -> rotation () .removeInterest (&GeoViewpointTool::set_rotation, this);
	transformTool -> rotation () .addInterest (&GeoViewpointTool::connectRotation, this);
}

void
GeoViewpointTool::set_translation ()
{
	const auto & transformTool   = getTransformTools () [0];
	const auto   userOrientation = getNode <GeoViewpoint> () -> getUserOrientation ();

	getNode <GeoViewpoint> () -> setPosition (transformTool -> translation () .getValue ());
	getNode <GeoViewpoint> () -> setUserOrientation (userOrientation);

	getNode <GeoViewpoint> () -> position () .removeInterest (&GeoViewpointTool::set_geo_position, this);
	getNode <GeoViewpoint> () -> position () .addInterest (&GeoViewpointTool::connectGeoPosition,  this);

	getNode <GeoViewpoint> () -> orientation () .removeInterest (&GeoViewpointTool::set_geo_orientation, this);
	getNode <GeoViewpoint> () -> orientation () .addInterest (&GeoViewpointTool::connectGeoOrientation,  this);
}

void
GeoViewpointTool::set_rotation ()
{
	const auto & transformTool = getTransformTools () [0];

	getNode <GeoViewpoint> () -> setOrientation (transformTool -> rotation ());

	getNode <GeoViewpoint> () -> orientation () .removeInterest (&GeoViewpointTool::set_geo_orientation, this);
	getNode <GeoViewpoint> () -> orientation () .addInterest (&GeoViewpointTool::connectGeoOrientation,  this);
}

void
GeoViewpointTool::connectGeoPosition (const SFVec3d & field)
{
	field .removeInterest (&GeoViewpointTool::connectGeoPosition, this);
	field .addInterest (&GeoViewpointTool::set_geo_position,      this);
}

void
GeoViewpointTool::connectGeoOrientation (const SFRotation & field)
{
	field .removeInterest (&GeoViewpointTool::connectGeoOrientation, this);
	field .addInterest (&GeoViewpointTool::set_geo_orientation,      this);
}

void
GeoViewpointTool::connectTranslation (const SFVec3f & field)
{
	field .removeInterest (&GeoViewpointTool::connectTranslation, this);
	field .addInterest (&GeoViewpointTool::set_translation,       this);
}

void
GeoViewpointTool::connectRotation (const SFRotation & field)
{
	field .removeInterest (&GeoViewpointTool::connectRotation, this);
	field .addInterest (&GeoViewpointTool::set_rotation,       this);
}

void
GeoViewpointTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	X3DViewpointNodeTool::traverse (type, renderObject);
	X3DGeospatialObjectTool::traverse (type, renderObject);
}

void
GeoViewpointTool::dispose ()
{
	X3DGeospatialObjectTool::dispose ();
	X3DViewpointNodeTool::dispose ();
}

GeoViewpointTool::~GeoViewpointTool ()
{ }

} // X3D
} // titania

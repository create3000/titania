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

#include "Sound.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../../Types/Geometry.h"

#include "../Sound/X3DSoundSourceNode.h"


namespace titania {
namespace X3D {

const ComponentType Sound::component      = ComponentType::SOUND;
const std::string   Sound::typeName       = "Sound";
const std::string   Sound::containerField = "children";

Sound::Fields::Fields () :
	 intensity (new SFFloat (1)),
	spatialize (new SFBool (true)),
	  location (new SFVec3f ()),
	 direction (new SFVec3f (0, 0, 1)),
	   minBack (new SFFloat (1)),
	  minFront (new SFFloat (1)),
	   maxBack (new SFFloat (10)),
	  maxFront (new SFFloat (10)),
	  priority (new SFFloat ()),
	    source (new SFNode ())
{ }

Sound::Sound (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DSoundNode (),
	          fields (),
	      sourceNode (),
	       traversed (true),
	currentTraversed (true)
{
	addType (X3DConstants::Sound);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "intensity",  intensity ());
	addField (initializeOnly, "spatialize", spatialize ());
	addField (inputOutput,    "location",   location ());
	addField (inputOutput,    "direction",  direction ());
	addField (inputOutput,    "minBack",    minBack ());
	addField (inputOutput,    "minFront",   minFront ());
	addField (inputOutput,    "maxBack",    maxBack ());
	addField (inputOutput,    "maxFront",   maxFront ());
	addField (inputOutput,    "priority",   priority ());
	addField (inputOutput,    "source",     source ());

	location () .setUnit (UnitCategory::LENGTH);
	minBack ()  .setUnit (UnitCategory::LENGTH);
	minFront () .setUnit (UnitCategory::LENGTH);
	maxBack ()  .setUnit (UnitCategory::LENGTH);
	maxFront () .setUnit (UnitCategory::LENGTH);

	addChildObjects (sourceNode,
	                 traversed);
}

X3DBaseNode*
Sound::create (X3DExecutionContext* const executionContext) const
{
	return new Sound (executionContext);
}

void
Sound::initialize ()
{
	X3DSoundNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (&Sound::set_live, this);
	isLive () .addInterest (&Sound::set_live, this);
	traversed .addInterest (&Sound::set_live, this);

	source () .addInterest (&Sound::set_source, this);

	set_live ();
	set_source ();
}

void
Sound::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> sensorEvents ()    .removeInterest (&Sound::update,   this);
	getExecutionContext () -> isLive () .removeInterest (&Sound::set_live, this);

	X3DSoundNode::setExecutionContext (executionContext);

	getExecutionContext () -> isLive () .addInterest (&Sound::set_live, this);

	set_live ();
}

void
Sound::setTraversed (const bool value)
{
   if (value)
	{
		if (traversed == false)
			traversed = true;
	}
	else
	{
		if (currentTraversed != traversed)
			traversed = currentTraversed;
	}

   currentTraversed = value;
}

void
Sound::set_live ()
{
	if (isLive () && getExecutionContext () -> isLive () && traversed)
	{
		getBrowser () -> sensorEvents () .addInterest (&Sound::update, this);
	}
	else
	{
		getBrowser () -> sensorEvents () .removeInterest (&Sound::update, this);
	}
}

void
Sound::set_source ()
{
	if (sourceNode)
		sourceNode -> setVolume (0);

	sourceNode .set (x3d_cast <X3DSoundSourceNode*> (source ()));
}

void
Sound::update ()
{
	if (not getTraversed ())
	{
		if (sourceNode)
			sourceNode -> setVolume (0);
	}

	setTraversed (false);
}

void
Sound::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	try
	{
		if (type not_eq TraverseType::DISPLAY)
			return;

		if (not sourceNode)
			return;
	
		if (not sourceNode -> isActive () or sourceNode -> isPaused ())
			return;

		setTraversed (true);

		const auto & modelViewMatrix = renderObject -> getModelViewMatrix () .get ();

		double minDistance, maxDistance;
		Vector3d minIntersection, maxIntersection;

		getEllipsoidParameter (modelViewMatrix, maxBack (), maxFront (), maxDistance, maxIntersection);
		getEllipsoidParameter (modelViewMatrix, minBack (), minFront (), minDistance, minIntersection);

		if (maxDistance < 1) // Sphere radius is 1
		{
			if (minDistance < 1) // Sphere radius is 1
			{
				sourceNode -> setVolume (intensity ());
			}
			else
			{
				const auto d1 = abs (minIntersection); // Viewer is here at (0, 0, 0)
				const auto d2 = distance (maxIntersection, minIntersection);
				const auto d  = 1 - (d1 / d2);

				//__LOG__ << d << std::endl;

				sourceNode -> setVolume (intensity () * d);
			}
		}
		else
		{
			sourceNode -> setVolume (0);
		}
	}
	catch (const std::domain_error &)
	{
		if (sourceNode)
			sourceNode -> setVolume (0);
	}
}

/*
 * http://de.wikipedia.org/wiki/Ellipse
 *
 * The ellipsoid is transformed to a sphere for easier calculation and then the viewer position is
 * transformed into this coordinate system. The radius and distance can then be obtained.
 *
 * throws std::domain_error
 */

void
Sound::getEllipsoidParameter (Matrix4d sphereMatrix, const double back, const double front, double & distance, Vector3d & intersection)
{
	const auto a = (back + front) / 2;
	const auto e = a - back;
	const auto b = std::sqrt (a * a - e * e);

	auto       location = Vector3d (0, 0, e);
	const auto scale    = Vector3d (b, b, a);

	const auto rotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (this -> direction () .getValue ()));

	sphereMatrix .translate (this -> location () .getValue ());
	sphereMatrix .rotate (rotation);
	sphereMatrix .translate (location);
	sphereMatrix .scale (scale);

	const auto viewer = inverse (sphereMatrix) .origin ();
	location .negate ();
	location /= scale;

	const auto   line          = Line3d (viewer, normalize (location - viewer));
	const auto   sphere        = Sphere3d (1, Vector3f ());
	const auto   intersections = sphere .intersects (line);
	const auto & intersection1 = std::get <0> (intersections);
	const auto & intersection2 = std::get <1> (intersections);

	if (math::distance (viewer, intersection1) < math::distance (viewer, intersection2))
		intersection = intersection1 * sphereMatrix;
	else
		intersection = intersection2 * sphereMatrix;

	distance = abs (viewer); // Distance to sphere center
}

Sound::~Sound ()
{ }

} // X3D
} // titania

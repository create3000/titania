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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "Sound.h"

#include "../../Browser/Core/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/Geometry.h"

#include "../Sound/X3DSoundSourceNode.h"

#include "../../Tools/Sound/SoundTool.h"

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
	  sourceNode ()
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

	addChildren (sourceNode);
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

	source () .addInterest (this, &Sound::set_source);

	set_source ();
}

void
Sound::set_source ()
{
	if (sourceNode)
		sourceNode -> setVolume (0);

	sourceNode .set (x3d_cast <X3DSoundSourceNode*> (source ()));
}

void
Sound::traverse (const TraverseType type)
{
	if (type not_eq TraverseType::DISPLAY)
		return;

	if (not sourceNode)
		return;

	if (not sourceNode -> isActive () or sourceNode -> isPaused ())
		return;

	try
	{
		float minRadius, maxRadius, minDistance, maxDistance;

		getEllipsoidParameter (maxBack (), maxFront (), maxRadius, maxDistance);
		getEllipsoidParameter (minBack (), minFront (), minRadius, minDistance);

		if (maxDistance < maxRadius)
		{
			if (minDistance < minRadius)
				sourceNode -> setVolume (intensity ());

			else
			{
				const auto d1 = maxRadius - maxDistance;
				const auto d2 = maxRadius - minRadius;

				sourceNode -> setVolume (intensity () * (d1 / d2));
			}
		}
		else
			sourceNode -> setVolume (0);
	}
	catch (const std::domain_error &)
	{
		sourceNode -> setVolume (0);
	}
}

/*
 * http://de.wikipedia.org/wiki/Ellipse
 *
 * The ellipsoid is transformed to a sphere for easier calculation and then the viewer position is
 * transformed into this coordinate system. The radius and distance can then be obtained.
 */

void
Sound::getEllipsoidParameter (const float & back, const float & front, float & radius, float & distance)
throw (std::domain_error)
{
	const auto a = (back + front) / 2;
	const auto e = a - back;
	const auto b = std::sqrt (a * a - e * e);

	auto modelViewMatrix  = getModelViewMatrix () .get ();

	modelViewMatrix .translate (location () .getValue ());
	modelViewMatrix .rotate (Rotation4d (Vector3d (0, 0, 1), Vector3d (direction () .getValue ())));

	modelViewMatrix .translate (Vector3d (0, 0, e));
	modelViewMatrix .scale (Vector3d (1, 1, a / b));

	const auto viewer = inverse (modelViewMatrix) .origin ();

	radius   = b;
	distance = abs (viewer);
}

void
Sound::addTool ()
{
	X3DSoundNode::addTool (new SoundTool (this));
}

Sound::~Sound ()
{ }

} // X3D
} // titania

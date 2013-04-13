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

#include "Sound.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Bits/Cast.h"

#include "../../Rendering/Matrix.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace X3D {

Sound::Sound (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DSoundNode (),                                                    
	   intensity (1),                                                   // SFFloat [in,out] intensity   1            [0,1]
	  spatialize (true),                                                // SFBool  [ ]      spatialize  TRUE
	    location (),                                                    // SFVec3f [in,out] location    0 0 0        (-∞,∞)
	   direction (0, 0, 1),                                             // SFVec3f [in,out] direction   0 0 1        (-∞,∞)
	     minBack (1),                                                   // SFFloat [in,out] minBack     1            [0,∞)
	    minFront (1),                                                   // SFFloat [in,out] minFront    1            [0,∞)
	     maxBack (10),                                                  // SFFloat [in,out] maxBack     10           [0,∞)
	    maxFront (10),                                                  // SFFloat [in,out] maxFront    10           [0,∞)
	    priority (),                                                    // SFFloat [in,out] priority    0            [0,1]
	      source (),                                                    // SFNode  [in,out] source      NULL         [X3DSoundSourceNode]
	     _source (NULL)                                                 
{
	setComponent ("Sound");
	setTypeName ("Sound");

	addField (inputOutput,    "metadata",   metadata);
	addField (inputOutput,    "intensity",  intensity);
	addField (initializeOnly, "spatialize", spatialize);
	addField (inputOutput,    "location",   location);
	addField (inputOutput,    "direction",  direction);
	addField (inputOutput,    "minBack",    minBack);
	addField (inputOutput,    "minFront",   minFront);
	addField (inputOutput,    "maxBack",    maxBack);
	addField (inputOutput,    "maxFront",   maxFront);
	addField (inputOutput,    "priority",   priority);
	addField (inputOutput,    "source",     source);
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

	source .addInterest (this, &Sound::set_source);

	set_source ();
}

void
Sound::set_source ()
{
	_source = x3d_cast <X3DSoundSourceNode*> (source .getValue ());
}

void
Sound::traverse (TraverseType type)
{
	if (type == TraverseType::COLLECT)
	{
		if (_source)
		{			
			float minRadius, maxRadius, minDistance, maxDistance;
			
			getEllipsoidParameter (maxBack, maxFront, maxRadius, maxDistance);
			getEllipsoidParameter (minBack, minFront, minRadius, minDistance);
			
			if (maxDistance < maxRadius)
			{
				if (minDistance < minRadius)
					_source -> setVolume (intensity);
					
				else
				{
					float d1 = maxRadius - maxDistance;
					float d2 = maxRadius - minRadius;
					
					_source -> setVolume (intensity * (d1 / d2));
				}
			}
			else
				_source -> setVolume (0);
		}
	}
}

// http://de.wikipedia.org/wiki/Ellipse

void
Sound::getEllipsoidParameter (const float & back, const float & front, float & radius, float & distance)
{
	float a = (back + front) / 2;
	float e = a - back;
	float b = std::sqrt (a * a - e * e);

	Matrix4f transformationMatrix = ModelViewMatrix4f ();
	transformationMatrix .translate (location .getValue ());
	transformationMatrix .rotate (Rotation4f (Vector3f (0, 0, 1), direction .getValue ()));
	
	transformationMatrix .translate (Vector3f (0, 0, e));
	transformationMatrix .scale (Vector3f (1, 1, b / a));
	
	Vector3f viewer = inverse (transformationMatrix) .translation ();

	radius   = b;
	distance = abs (viewer);
}


} // X3D
} // titania

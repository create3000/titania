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

#include "ShapeContainer.h"

#include "../Rendering/ViewVolume.h"
#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

ShapeContainer::ShapeContainer (X3DShapeNode* shape,
                                X3DFogObject* fog,
                                const LightContainerArray & localLights) :
	               shape (shape),                
	                 fog (fog),                  
	         localLights (localLights),          
	     modelViewMatrix (ModelViewMatrix4f ()),
	              matrix (modelViewMatrix),
	            distance (getDistance (shape, matrix))                    
{ }

void
ShapeContainer::assign (X3DShapeNode* shape,
                        X3DFogObject* fog,
                        const LightContainerArray & localLights)
{
	this -> shape            = shape;
	this -> fog             = fog;
	this -> localLights     = localLights;
	this -> modelViewMatrix = ModelViewMatrix4f ();
	this -> matrix          = this -> modelViewMatrix;
	this -> distance        = getDistance (shape, this -> matrix);
}

void
ShapeContainer::multMatrix (const Matrix4f & matrix)
{
	this -> matrix   = modelViewMatrix * matrix;
	this -> distance = getDistance (shape, this -> matrix);
}

bool
ShapeContainer::intersect (const Sphere3f & sphere, std::deque <Vector3f> & collisionNormal) const
{
	return shape -> intersect (matrix, sphere, collisionNormal);
}

bool
ShapeContainer::draw ()
{
	bool drawn = false;

	if (distance < 0)
	{
		glLoadMatrixf (matrix .data ());
		
		if (ViewVolume () .intersect (shape -> getBBox ()))
		{
			for (const auto & light : localLights)
				light -> enable ();

			if (fog)
				fog -> enable ();

			shape -> draw ();

			for (const auto & light : basic::adapter (localLights .crbegin (), localLights .crend ()))
				light -> disable ();

			drawn = true;
		}
	}

	return drawn;
}

float
ShapeContainer::getDistance (X3DShapeNode* shape, const Matrix4f & matrix)
{
	Box3f bbox  = shape -> getBBox () * matrix;
	float depth = bbox .size () .z () * 0.5f;

	return bbox .center () .z () - depth;
}

} // X3D
} // titania

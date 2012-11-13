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
 ******************************************************************************/

#include "ProximitySensor.h"

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/Geometry.h"

#include <iostream>

namespace titania {
namespace X3D {

ProximitySensor::ProximitySensor (X3DExecutionContext* const executionContext) :
	              X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	X3DEnvironmentalSensorNode (),                                                    
	  centerOfRotation_changed (),                                                    // SFVec3f    [out] centerOfRotation_changed
	       orientation_changed (),                                                    // SFRotation [out] orientation_changed
	          position_changed (),                                                    // SFVec3f    [out] position_changed
	                    inside (false)                                                
{
	setComponent ("EnvironmentalSensor");
	setTypeName ("ProximitySensor");

	appendField (inputOutput, "metadata",                 metadata);
	appendField (inputOutput, "enabled",                  enabled);
	appendField (inputOutput, "size",                     size);
	appendField (inputOutput, "center",                   center);
	appendField (outputOnly,  "enterTime",                enterTime);
	appendField (outputOnly,  "exitTime",                 exitTime);
	appendField (outputOnly,  "isActive",                 isActive);
	appendField (outputOnly,  "position_changed",         position_changed);
	appendField (outputOnly,  "orientation_changed",      orientation_changed);
	appendField (outputOnly,  "centerOfRotation_changed", centerOfRotation_changed);
}

X3DBasicNode*
ProximitySensor::create (X3DExecutionContext* const executionContext) const
{
	return new ProximitySensor (executionContext);
}

void
ProximitySensor::initialize ()
{
	X3DEnvironmentalSensorNode::initialize ();

	enabled .addInterest (this, &ProximitySensor::set_enabled);

	set_enabled ();
}

void
ProximitySensor::set_enabled ()
{
	if (enabled)
		getBrowser () -> addSensor (this);
	else
		getBrowser () -> removeSensor (this);
}

void
ProximitySensor::update ()
{
	if (inside)
	{
		matrix = ~viewpoint -> getMatrix () * matrix;

		Vector3f   translation, scale;
		Rotation4f rotation;
		matrix .get (translation, rotation, scale);

		Vector3f   position    = inverse (matrix) .translation ();
		Rotation4f orientation = ~rotation;

		if (not isActive)
		{
			enterTime = getCurrentTime ();
			isActive  = true;

			position_changed    = position;
			orientation_changed = orientation;
		}
		else
		{
			if (position_changed not_eq position)
				position_changed = position;

			if (orientation_changed not_eq orientation)
				orientation_changed = orientation;
		}

		//		std::clog << "#######################" << std::endl;
		//		std::clog << position << std::endl;
		//		std::clog << orientation << std::endl;
	}
	else
	{
		if (isActive)
		{
			exitTime = getCurrentTime ();
			isActive = false;
		}
	}

	viewpoint = nullptr;
	inside    = false;
}

void
ProximitySensor::draw ()
{
	if (inside /* or getBrowser () -> getEditMode () */)
		return;

	Matrix4f centerMatrix = ModelViewMatrix4f ();
	centerMatrix .translate (center);

	if (isInside (centerMatrix))
	{
		viewpoint = getBrowser () -> getActiveViewpoint ();
		matrix    = viewpoint -> getCurrentMatrix () * ModelViewMatrix4f ();
		inside    = true;
	}

	//drawHandle();
}

bool
ProximitySensor::isInside (const Matrix4f & matrix) const
{
	auto size_2 = size * 0.5f;

	float x = size_2 .x ();
	float y = size_2 .y ();
	float z = size_2 .z ();

	Vector3f points [6] = {
		matrix * Vector3f (0,  0,  z),
		matrix * Vector3f (0,  0, -z),
		matrix * Vector3f (0,  y,  0),
		matrix * Vector3f (0, -y,  0),
		matrix * Vector3f (x,  0,  0),
		matrix * Vector3f (-x,  0,  0)
	};

	Vector3f normals [6] = {
		normalize (matrix .multDirMatrix (Vector3f (0,  0,  1))),
		normalize (matrix .multDirMatrix (Vector3f (0,  0, -1))),
		normalize (matrix .multDirMatrix (Vector3f (0,  1,  0))),
		normalize (matrix .multDirMatrix (Vector3f (0, -1,  0))),
		normalize (matrix .multDirMatrix (Vector3f (1,  0,  0))),
		normalize (matrix .multDirMatrix (Vector3f (-1,  0,  0)))
	};

	for (int i = 0; i < 6; ++ i)
	{
		if (Plane3f (points [i], normals [i]) .distanceFromOrigin () < 0)
			return false;
	}

	return true;
}

void
ProximitySensor::drawHandle ()
{
	float cx, cy, cz;

	center .getValue (cx, cy, cz);

	glPushMatrix ();
	glTranslatef (cx, cy, cz);

	glDepthMask (GL_FALSE);
	glDisable (GL_LIGHTING);
	glColor4f (0.5, 0, 1, 0.5);
	glEnable (GL_BLEND);
	glDisable (GL_TEXTURE_2D);

	auto size_2 = size * 0.5f;

	float x = size_2 .x ();
	float y = size_2 .y ();
	float z = size_2 .z ();

	glEnable (GL_CULL_FACE);

	glFrontFace (GL_CCW);

	glBegin (GL_QUADS); // Start Drawing Quads

	// Front Face
	glNormal3f (0, 0, 1);
	glVertex3f (-x, -y,  z);
	glVertex3f (x, -y,  z);
	glVertex3f (x,  y,  z);
	glVertex3f (-x,  y,  z);

	// Back Face
	glNormal3f (0, 0, -1);
	glVertex3f (-x, -y, -z);
	glVertex3f (-x,  y, -z);
	glVertex3f (x,  y, -z);
	glVertex3f (x, -y, -z);

	// Top Face
	glNormal3f (0, 1, 0);
	glVertex3f (-x,  y, -z);
	glVertex3f (-x,  y,  z);
	glVertex3f (x,  y,  z);
	glVertex3f (x,  y, -z);

	// Bottom Face
	glNormal3f (0, -1, 0);
	glVertex3f (-x, -y, -z);
	glVertex3f (x, -y, -z);
	glVertex3f (x, -y,  z);
	glVertex3f (-x, -y,  z);

	// Right face
	glNormal3f (1, 0, 0);
	glVertex3f (x, -y, -z);
	glVertex3f (x,  y, -z);
	glVertex3f (x,  y,  z);
	glVertex3f (x, -y,  z);

	// Left Face
	glNormal3f (-1, 0, 0);
	glVertex3f (-x, -y, -z);
	glVertex3f (-x, -y,  z);
	glVertex3f (-x,  y,  z);
	glVertex3f (-x,  y, -z);

	glEnd (); // Done Drawing Quads

	glPopMatrix ();
}

void
ProximitySensor::dispose ()
{
	if (enabled)
		getBrowser () -> removeSensor (this);

	X3DEnvironmentalSensorNode::dispose ();
}

} // X3D
} // titania

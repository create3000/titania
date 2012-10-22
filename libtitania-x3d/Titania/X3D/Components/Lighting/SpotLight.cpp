/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#include "SpotLight.h"

#include "../../Execution/X3DExecutionContext.h"

#include "../../Browser/Browser.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

SpotLight::SpotLight (X3DExecutionContext* const executionContext) :
	X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DLightNode (),                                 
	 attenuation (1, 0, 0),                          // SFVec3f [in,out] attenuation  1 0 0         [0,∞)
	   beamWidth (),                                 // SFFloat [in,out] beamWidth    π/2           (0,π/2]
	 cutOffAngle (0.785398),                         // SFFloat [in,out] cutOffAngle  π/4           (0,π/2]
	   direction (0, 0, -1),                         // SFVec3f [in,out] direction    0 0 -1        (-∞,∞)
	    location (),                                 // SFVec3f [in,out] location     0 0 0         (-∞,∞)
	      radius (100)                               // SFFloat [in,out] radius       100           [0,∞)
{
	setComponent ("Lighting");
	setTypeName ("SpotLight");

	appendField (inputOutput, "metadata",         metadata);
	appendField (inputOutput, "on",               on);
	appendField (inputOutput, "global",           global);
	appendField (inputOutput, "color",            color);
	appendField (inputOutput, "location",         location);
	appendField (inputOutput, "direction",        direction);
	appendField (inputOutput, "radius",           radius);
	appendField (inputOutput, "intensity",        intensity);
	appendField (inputOutput, "ambientIntensity", ambientIntensity);
	appendField (inputOutput, "beamWidth",        beamWidth);
	appendField (inputOutput, "cutOffAngle",      cutOffAngle);
	appendField (inputOutput, "attenuation",      attenuation);
}

X3DBasicNode*
SpotLight::create (X3DExecutionContext* const executionContext) const
{
	return new SpotLight (executionContext);
}

void
SpotLight::initialize ()
{
	X3DLightNode::initialize ();
	eventsProcessed ();
}

void
SpotLight::eventsProcessed ()
{
	X3DLightNode::eventsProcessed ();

	glAmbient [0] = color .getR () * ambientIntensity;
	glAmbient [1] = color .getG () * ambientIntensity;
	glAmbient [2] = color .getB () * ambientIntensity;
	glAmbient [3] = 1;

	glDiffuseSpecular [0] = color .getR () * intensity;
	glDiffuseSpecular [1] = color .getG () * intensity;
	glDiffuseSpecular [2] = color .getB () * intensity;
	glDiffuseSpecular [3] = 1;

	glSpotExponent = math::clamp <float> (beamWidth ? 0.5f / beamWidth : 0, 0, 128);
	glSpotCutOff   = math::clamp <float> (math::degree <float> (cutOffAngle), 0, 90);

	glPosition [0] = location .getX ();
	glPosition [1] = location .getY ();
	glPosition [2] = location .getZ ();
	glPosition [3] = 1; // point light

	glSpotDirection [0] = direction .getX ();
	glSpotDirection [1] = direction .getY ();
	glSpotDirection [2] = direction .getZ ();
}

void
SpotLight::enable ()
{
	X3DLightNode::enable ();

	glLightfv (light (), GL_AMBIENT,  glAmbient);
	glLightfv (light (), GL_DIFFUSE,  glDiffuseSpecular);
	glLightfv (light (), GL_SPECULAR, glDiffuseSpecular);

	glLightf (light (), GL_SPOT_EXPONENT, glSpotExponent);
	glLightf (light (), GL_SPOT_CUTOFF,   glSpotCutOff);

	glLightf (light (), GL_CONSTANT_ATTENUATION,  attenuation .getX ());
	glLightf (light (), GL_LINEAR_ATTENUATION,    attenuation .getY ());
	glLightf (light (), GL_QUADRATIC_ATTENUATION, attenuation .getZ ());

	glLightfv (light (), GL_POSITION,       glPosition);
	glLightfv (light (), GL_SPOT_DIRECTION, glSpotDirection);
}

} // X3D
} // titania

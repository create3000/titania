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

#include "SpotLight.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

SpotLight::Fields::Fields () :
	attenuation (new SFVec3f (1, 0, 0)),
	beamWidth (new SFFloat ()),
	cutOffAngle (new SFFloat (0.785398)),
	direction (new SFVec3f (0, 0, -1)),
	location (new SFVec3f ()),
	radius (new SFFloat (100))
{ }

SpotLight::SpotLight (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLightNode (),
	      fields ()
{
	setComponent ("Lighting");
	setTypeName ("SpotLight");

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "on",               on ());
	addField (inputOutput, "global",           global ());
	addField (inputOutput, "color",            color ());
	addField (inputOutput, "location",         location ());
	addField (inputOutput, "direction",        direction ());
	addField (inputOutput, "radius",           radius ());
	addField (inputOutput, "intensity",        intensity ());
	addField (inputOutput, "ambientIntensity", ambientIntensity ());
	addField (inputOutput, "beamWidth",        beamWidth ());
	addField (inputOutput, "cutOffAngle",      cutOffAngle ());
	addField (inputOutput, "attenuation",      attenuation ());
}

X3DBaseNode*
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

	float glAmbientIntensity = math::clamp <float> (ambientIntensity (), 0, 1);
	float glIntensity        = math::clamp <float> (intensity (), 0, 1);

	glAmbient [0] = color () .getR () * glAmbientIntensity;
	glAmbient [1] = color () .getG () * glAmbientIntensity;
	glAmbient [2] = color () .getB () * glAmbientIntensity;
	glAmbient [3] = 1;

	glDiffuseSpecular [0] = color () .getR () * glIntensity;
	glDiffuseSpecular [1] = color () .getG () * glIntensity;
	glDiffuseSpecular [2] = color () .getB () * glIntensity;
	glDiffuseSpecular [3] = 1;

	glSpotExponent = math::clamp <float> (beamWidth () ? 0.5f / beamWidth () : 0, 0, 128);
	glSpotCutOff   = math::clamp <float> (math::degree <float> (cutOffAngle ()), 0, 90);

	glPosition [0] = location () .getX ();
	glPosition [1] = location () .getY ();
	glPosition [2] = location () .getZ ();
	glPosition [3] = 1; // point light

	glSpotDirection [0] = direction () .getX ();
	glSpotDirection [1] = direction () .getY ();
	glSpotDirection [2] = direction () .getZ ();
}

void
SpotLight::draw (GLenum lightId)
{
	glLightfv (lightId, GL_AMBIENT,  glAmbient);
	glLightfv (lightId, GL_DIFFUSE,  glDiffuseSpecular);
	glLightfv (lightId, GL_SPECULAR, glDiffuseSpecular);

	glLightf (lightId, GL_SPOT_EXPONENT, glSpotExponent);
	glLightf (lightId, GL_SPOT_CUTOFF,   glSpotCutOff);

	glLightf (lightId, GL_CONSTANT_ATTENUATION,  attenuation () .getX ());
	glLightf (lightId, GL_LINEAR_ATTENUATION,    attenuation () .getY ());
	glLightf (lightId, GL_QUADRATIC_ATTENUATION, attenuation () .getZ ());

	glLightfv (lightId, GL_POSITION,       glPosition);
	glLightfv (lightId, GL_SPOT_DIRECTION, glSpotDirection);
}

} // X3D
} // titania

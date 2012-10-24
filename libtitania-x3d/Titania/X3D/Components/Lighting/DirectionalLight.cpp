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

#include "DirectionalLight.h"

#include "../../Execution/X3DExecutionContext.h"

#include "../../Browser/Browser.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

DirectionalLight::DirectionalLight (X3DExecutionContext* const executionContext) :
	X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DLightNode (),                                 
	   direction (0, 0, -1)                          // SFVec3f [in,out] direction  0 0 -1        (-∞,∞)
{
	setComponent ("Lighting");
	setTypeName ("DirectionalLight");

	appendField (inputOutput, "metadata",         metadata);
	appendField (inputOutput, "on",               on);
	appendField (inputOutput, "global",           global);
	appendField (inputOutput, "color",            color);
	appendField (inputOutput, "intensity",        intensity);
	appendField (inputOutput, "ambientIntensity", ambientIntensity);
	appendField (inputOutput, "direction",        direction);
}

X3DBasicNode*
DirectionalLight::create (X3DExecutionContext* const executionContext) const
{
	return new DirectionalLight (executionContext);
}

void
DirectionalLight::initialize ()
{
	X3DLightNode::initialize ();
	eventsProcessed ();
}

void
DirectionalLight::eventsProcessed ()
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

	glPosition [0] = -direction .getX ();
	glPosition [1] = -direction .getY ();
	glPosition [2] = -direction .getZ ();
	glPosition [3] = 0; // directional light
}

void
DirectionalLight::enable ()
{
	X3DLightNode::enable ();

	glLightfv (getLight (), GL_AMBIENT,  glAmbient);
	glLightfv (getLight (), GL_DIFFUSE,  glDiffuseSpecular);
	glLightfv (getLight (), GL_SPECULAR, glDiffuseSpecular);

	glLightf  (getLight (), GL_SPOT_EXPONENT, 0);
	glLightf  (getLight (), GL_SPOT_CUTOFF, 180);

	glLightfv (getLight (), GL_POSITION, glPosition);
}

} // X3D
} // titania

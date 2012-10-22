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

#include "BrowserOptions.h"

extern "C"
{
#include <GL/glx.h>
}

#include "../../Execution/X3DExecutionContext.h"
#include <Titania/Bits/String/Join.h>

#include <iostream>

namespace titania {
namespace X3D {

// Table 9.2 — Browser options
// Name                    Type/valid range                           Default                      Description
// Antialiased             Boolean                                     False                        Render using hardware antialiasing if available
// Dashboard               Boolean                                                                  Specified by bound NavigationInfo in contentDisplay browser navigation user interface
// EnableInlineViewpoints  Boolean                                     True                         Viewpoints from Inline nodes are included in list of viewpoints if made available by the Inline node.
// MotionBlur              Boolean                                     False                        Render animations with motion blur
// PrimitiveQuality        Low, Medium, High                           Medium                       Render quality (tesselation level) for Box, Cone, Cylinder, Sphere
// QualityWhenMoving       Low, Medium, High,                          Same (as while stationary)   SameRender quality while camera is moving
// Shading                 Pointset, Wireframe, Flat, Gouraud, Phong   Gouraud                      Specify shading mode for all objects
// SplashScreen            Boolean                                     Implementation-dependent     Display browser splash screen on startup
// TextureQuality          Low, Medium, High                           Medium                       Quality of texture map display

BrowserOptions::BrowserOptions (X3DExecutionContext* const executionContext) :
	          X3DBasicNode (executionContext -> getBrowser (),            executionContext),
	       X3DPropertyNode (),                                            
	           antialiased (),                                            
	             dashboard (),                                            
	enableInlineViewpoints (true),                                            // motionBlur (False),
	      primitiveQuality ("MEDIUM"),
	     qualityWhenMoving ("MEDIUM"),
	               shading ("GOURAUD"),
	          splashScreen (false),
	        textureQuality ("MEDIUM"),
	            quadSphere (new QuadSphereProperties (executionContext))
{
	setComponent ("Browser"),
	setTypeName ("BrowserOptions");

	appendField (inputOutput, "antialiased",            antialiased);
	appendField (inputOutput, "dashboard",              dashboard);
	appendField (inputOutput, "enableInlineViewpoints", enableInlineViewpoints);
	appendField (inputOutput, "primitiveQuality",       primitiveQuality);
	appendField (inputOutput, "qualityWhenMoving",      qualityWhenMoving);
	appendField (inputOutput, "shading",                shading);
	appendField (inputOutput, "splashScreen",           splashScreen);
	appendField (inputOutput, "textureQuality",         textureQuality);
	appendField (inputOutput, "quadSphere",             quadSphere);
}

BrowserOptions*
BrowserOptions::create (X3DExecutionContext* const executionContext)  const
{
	return new BrowserOptions (executionContext);
}

void
BrowserOptions::initialize ()
{
	X3DBasicNode::initialize ();

	quadSphere -> setup ();

	shading .addInterest (this, &BrowserOptions::set_shading);
}

void
BrowserOptions::set_shading ()
{
	std::clog << "Setting shading to " << shading << "." << std::endl;

	if (shading == "PHONG")
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	else if (shading == "GOURAUD")
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	else if (shading == "FLAT")
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	else if (shading == "WIREFRAME")
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

	else if (shading == "POINTSET")
		glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
}

} // X3D
} // titania

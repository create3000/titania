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
 ******************************************************************************/

#include "BrowserOptions.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Geometry3D/QuadSphereProperties.h"
#include <Titania/String/Join.h>

extern "C"
{
#include <GL/gl.h>
}

namespace titania {
namespace X3D {

// Table 9.2 — Browser options
// Name                    Type/valid range                            Default                      Description
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
	           X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	       X3DPropertyNode (),                                                    
	          splashScreen (false),                                               
	             dashboard (),                                                    
	enableInlineViewpoints (true),                                                // motionBlur (False),
	           antialiased (),                                                    
	        textureQuality ("MEDIUM"),                                            
	      primitiveQuality ("MEDIUM"),                                            
	     qualityWhenMoving ("MEDIUM"),                                            
	               shading ("GOURAUD"),                                           
	            motionBlur (new MotionBlur (executionContext)),                   
	     textureProperties (new TextureProperties (executionContext)),            
	         boxProperties (new BoxProperties (executionContext)),                
	      sphereProperties (new QuadSphereProperties (executionContext)),         
	             fontStyle (new FontStyle (executionContext))                     
{
	setComponent ("Browser"),
	setTypeName ("BrowserOptions");

	addField (inputOutput, "splashScreen",           splashScreen);
	addField (inputOutput, "dashboard",              dashboard);
	addField (inputOutput, "enableInlineViewpoints", enableInlineViewpoints);
	addField (inputOutput, "antialiased",            antialiased);
	addField (inputOutput, "textureQuality",         textureQuality);
	addField (inputOutput, "primitiveQuality",       primitiveQuality);
	addField (inputOutput, "qualityWhenMoving",      qualityWhenMoving);
	addField (inputOutput, "shading",                shading);

	addField (inputOutput, "motionBlur",             motionBlur);
	addField (inputOutput, "textureProperties",      textureProperties);
	addField (inputOutput, "sphereProperties",       sphereProperties);
	addField (inputOutput, "boxProperties",          boxProperties);
	addField (inputOutput, "fontStyle",              fontStyle);
}

BrowserOptions*
BrowserOptions::create (X3DExecutionContext* const executionContext)  const
{
	return new BrowserOptions (executionContext);
}

void
BrowserOptions::initialize ()
{
	X3DPropertyNode::initialize ();

	motionBlur        -> setup ();
	textureProperties -> setup ();
	boxProperties     -> setup ();
	sphereProperties  -> setup ();
	fontStyle         -> setup ();

	primitiveQuality .addInterest (this, &BrowserOptions::set_primitiveQuality);
	shading          .addInterest (this, &BrowserOptions::set_shading);

	set_textureQuality ();
	set_primitiveQuality ();
	set_shading ();
}

void
BrowserOptions::set_textureQuality ()
{
	textureProperties -> magnificationFilter = "NICEST";
	textureProperties -> minificationFilter  = "NICEST";
	textureProperties -> textureCompression  = "NICEST";
	textureProperties -> generateMipMaps     = true;

}

void
BrowserOptions::set_primitiveQuality ()
{
	std::clog << "Setting primitive quality to " << primitiveQuality << "." << std::endl;

	if (primitiveQuality == "LOW")
	{
		SFNode <QuadSphereProperties> quadSphereProperties = sphereProperties;

		if (quadSphereProperties)
		{
			quadSphereProperties -> uDimension = 20;
			quadSphereProperties -> vDimension = 10;
		}
	}
	else if (primitiveQuality == "MEDIUM")
	{
		SFNode <QuadSphereProperties> quadSphereProperties = sphereProperties;

		if (quadSphereProperties)
		{
			quadSphereProperties -> uDimension = 40;
			quadSphereProperties -> vDimension = 20;
		}
	}
	else if (primitiveQuality == "HIGH")
	{
		SFNode <QuadSphereProperties> quadSphereProperties = sphereProperties;

		if (quadSphereProperties)
		{
			quadSphereProperties -> uDimension = 80;
			quadSphereProperties -> vDimension = 40;
		}
	}
}

void
BrowserOptions::set_shading ()
{
	std::clog << "Setting shading to " << shading << "." << std::endl;

	if (shading == "PHONG")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_SMOOTH);
	}
	else if (shading == "GOURAUD")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_SMOOTH);
	}
	else if (shading == "FLAT")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_FLAT);
	}
	else if (shading == "WIREFRAME")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel (GL_SMOOTH);
	}
	else if (shading == "POINTSET")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
		glShadeModel (GL_SMOOTH);
	}
}

} // X3D
} // titania

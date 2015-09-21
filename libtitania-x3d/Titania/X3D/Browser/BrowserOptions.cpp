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

#include "BrowserOptions.h"

#include "../Bits/config.h"
#include "../Browser/Geometry2D/Arc2DOptions.h"
#include "../Browser/Geometry2D/ArcClose2DOptions.h"
#include "../Browser/Geometry2D/Circle2DOptions.h"
#include "../Browser/Geometry2D/Disk2DOptions.h"
#include "../Browser/Geometry2D/Rectangle2DOptions.h"
#include "../Browser/Geometry3D/BoxOptions.h"
#include "../Browser/Geometry3D/ConeOptions.h"
#include "../Browser/Geometry3D/CylinderOptions.h"
#include "../Browser/Geometry3D/QuadSphereOptions.h"
#include "../Browser/Rendering/MotionBlur.h"
#include "../Browser/RenderingProperties.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Texturing/TextureProperties.h"
#include "../Rendering/OpenGL.h"

#include <Titania/Physics/Constants.h>

namespace titania {
namespace X3D {

// Table 9.2 — Browser options
// Name                    Type/valid range                         Default                      Description
// SplashScreen            Boolean                                  Implementation-dependent     Display browser splash screen on startup.
// Dashboard               Boolean                                  False                        Specified by bound NavigationInfo in content. Display browser navigation user interface.
// Rubberband              Boolean                                  True                         Specified by bound NavigationInfo in content. Display rubber band navigation hint.
// EnableInlineViewpoints  Boolean                                  True                         Viewpoints from Inline nodes are included in list of viewpoints if made available by the Inline node.
// Antialiased             Boolean                                  False                        Render using hardware antialiasing if available.
// TextureQuality          Low, Medium, High                        Medium                       Quality of texture map display.
// PrimitiveQuality        Low, Medium, High                        Medium                       Render quality (tessellation level) for Box, Cone, Cylinder, Sphere.
// QualityWhenMoving       Low, Medium, High,                       Same (as while stationary)   Render quality while camera is moving.
// Shading                 Point, Wireframe, Flat, Gouraud, Phong   Gouraud                      Specify shading mode for all objects.
// MotionBlur              Boolean                                  False                        Render animations with motion blur.
//
// SplashScreenURL         String Array                             Implementation-dependent     The url of the file that should be used as splash screen.
// MotionBlurIntesity      Number                                   0.25                         Motion blur intesity in the range (0, 1).
// AnimateStairWalks       Boolean                                  False                        Animate stair walks. This can give unexpected results when the floor is animated.
// Gravity                 Number                                   g                            Gravitational acceleration. The standard value is the acceleration of the earth.

const ComponentType BrowserOptions::component      = ComponentType::TITANIA;
const std::string   BrowserOptions::typeName       = "BrowserOptions";
const std::string   BrowserOptions::containerField = "browserOptions";

BrowserOptions::Fields::Fields (X3DExecutionContext* const executionContext) :
	          splashScreen (new SFBool (false)),
	       splashScreenURL (new MFString ({ get_page ("about/splash.x3dv") .str () })),
	             dashboard (new SFBool (true)),
	            rubberBand (new SFBool (true)),
	enableInlineViewpoints (new SFBool (true)),
	           antialiased (new SFBool (true)),
	        textureQuality (new SFString ("MEDIUM")),
	      primitiveQuality (new SFString ("MEDIUM")),
	     qualityWhenMoving (new SFString ("MEDIUM")),
	               shading (new SFString ("GOURAUD")),
	     animateStairWalks (new SFBool ()),
	               gravity (new SFFloat (P_GN))
{ }

BrowserOptions::BrowserOptions (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     fields (executionContext)
{
	addType (X3DConstants::BrowserOptions);

	addField (inputOutput, "SplashScreen",           splashScreen ());
	addField (inputOutput, "Dashboard",              dashboard ());
	addField (inputOutput, "Rubberband",             rubberBand ());
	addField (inputOutput, "EnableInlineViewpoints", enableInlineViewpoints ());
	addField (inputOutput, "Antialiased",            antialiased ());
	addField (inputOutput, "TextureQuality",         textureQuality ());
	addField (inputOutput, "PrimitiveQuality",       primitiveQuality ());
	addField (inputOutput, "QualityWhenMoving",      qualityWhenMoving ());
	addField (inputOutput, "Shading",                shading ());
	addField (inputOutput, "MotionBlur",             motionBlur ());

	// Non standard options
	addField (inputOutput, "SplashScreenURL",        splashScreenURL ());
	addField (inputOutput, "MotionBlurIntensity",    motionBlurIntensity ());
	addField (inputOutput, "AnimateStairWalks",      animateStairWalks ());
	addField (inputOutput, "Gravity",                gravity ());

	addField (X3D_V3_3, "AntiAliased", "Antialiased");
}

BrowserOptions*
BrowserOptions::create (X3DExecutionContext* const executionContext)  const
{
	return new BrowserOptions (executionContext);
}

void
BrowserOptions::initialize ()
{
	X3DBaseNode::initialize ();

	antialiased ()      .addInterest (this, &BrowserOptions::set_antialiased);
	textureQuality ()   .addInterest (this, &BrowserOptions::set_textureQuality);
	primitiveQuality () .addInterest (this, &BrowserOptions::set_primitiveQuality);
	shading ()          .addInterest (this, &BrowserOptions::set_shading);

	set_antialiased ();
	set_textureQuality ();
	set_primitiveQuality ();
	set_shading ();
}

SFBool &
BrowserOptions::motionBlur ()
{
	return getBrowser () -> getMotionBlur () -> enabled ();
}

const SFBool &
BrowserOptions::motionBlur () const
{
	return getBrowser () -> getMotionBlur () -> enabled ();
}

SFFloat &
BrowserOptions::motionBlurIntensity ()
{
	return getBrowser () -> getMotionBlur () -> intensity ();
}

const SFFloat &
BrowserOptions::motionBlurIntensity () const
{
	return getBrowser () -> getMotionBlur () -> intensity ();
}

void
BrowserOptions::set_antialiased ()
{
	if (antialiased ())
	{
		glEnable (GL_MULTISAMPLE);

		GLint sampleBuffers, samples;

		glGetIntegerv (GL_SAMPLE_BUFFERS, &sampleBuffers);
		glGetIntegerv (GL_SAMPLES, &samples);

		getBrowser () -> getRenderingProperties () -> antialiased () = sampleBuffers and samples;
	}
	else
	{
		glDisable (GL_MULTISAMPLE);
		getBrowser () -> getRenderingProperties () -> antialiased () = false;
	}
}

void
BrowserOptions::set_textureQuality ()
{
	const auto & textureProperties = getBrowser () -> getTextureProperties ();

	if (textureQuality () == "HIGH")
	{
		textureProperties -> magnificationFilter () = "NICEST";
		textureProperties -> minificationFilter ()  = "NICEST";
		textureProperties -> textureCompression ()  = "NICEST";
		textureProperties -> generateMipMaps ()     = true;

		glHint (GL_GENERATE_MIPMAP_HINT,        GL_NICEST);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		return;
	}

	if (textureQuality () == "LOW")
	{
		textureProperties -> magnificationFilter () = "NICEST";
		textureProperties -> minificationFilter ()  = "AVG_PIXEL_NEAREST_MIPMAP";
		textureProperties -> textureCompression ()  = "FASTEST";
		textureProperties -> generateMipMaps ()     = true;

		glHint (GL_GENERATE_MIPMAP_HINT,        GL_FASTEST);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		return;
	}

	// MEDIUM

	textureProperties -> magnificationFilter () = "NICEST";
	textureProperties -> minificationFilter ()  = "AVG_PIXEL_AVG_MIPMAP";
	textureProperties -> textureCompression ()  = "NICEST";
	textureProperties -> generateMipMaps ()     = true;

	glHint (GL_GENERATE_MIPMAP_HINT,        GL_FASTEST);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

void
BrowserOptions::set_primitiveQuality ()
{
	if (primitiveQuality () == "HIGH")
	{
		getBrowser () -> getArc2DOptions ()      -> minAngle ()   = M_PI / 40;
		getBrowser () -> getArcClose2DOptions () -> minAngle ()   = M_PI / 40;
		getBrowser () -> getCircle2DOptions ()   -> segments ()   = 80;
		getBrowser () -> getDisk2DOptions ()     -> segments ()   = 80;
		getBrowser () -> getConeOptions ()       -> vDimension () = 32;
		getBrowser () -> getCylinderOptions ()   -> vDimension () = 32;

		const auto quadSphere = dynamic_cast <QuadSphereOptions*> (getBrowser () -> getSphereOptions () .getValue ());

		if (quadSphere)
		{
			quadSphere -> uDimension () = 63;
			quadSphere -> vDimension () = 31;
		}

		return;
	}

	if (primitiveQuality () == "LOW")
	{
		getBrowser () -> getArc2DOptions ()      -> minAngle ()   = M_PI / 10;
		getBrowser () -> getArcClose2DOptions () -> minAngle ()   = M_PI / 10;
		getBrowser () -> getCircle2DOptions ()   -> segments ()   = 20;
		getBrowser () -> getDisk2DOptions ()     -> segments ()   = 20;
		getBrowser () -> getConeOptions ()       -> vDimension () = 16;
		getBrowser () -> getCylinderOptions ()   -> vDimension () = 16;

		const auto quadSphere = dynamic_cast <QuadSphereOptions*> (getBrowser () -> getSphereOptions () .getValue ());

		if (quadSphere)
		{
			quadSphere -> uDimension () = 21;
			quadSphere -> vDimension () = 9;
		}

		return;
	}

	// MEDIUM

	getBrowser () -> getArc2DOptions ()      -> minAngle ()   = M_PI / 20;
	getBrowser () -> getArcClose2DOptions () -> minAngle ()   = M_PI / 20;
	getBrowser () -> getCircle2DOptions ()   -> segments ()   = 40;
	getBrowser () -> getDisk2DOptions ()     -> segments ()   = 40;
	getBrowser () -> getConeOptions ()       -> vDimension () = 20;
	getBrowser () -> getCylinderOptions ()   -> vDimension () = 20;

	const auto quadSphere = dynamic_cast <QuadSphereOptions*> (getBrowser () -> getSphereOptions () .getValue ());

	if (quadSphere)
	{
		quadSphere -> uDimension () = 31;
		quadSphere -> vDimension () = 15;
	}
}

void
BrowserOptions::set_shading ()
{
	getBrowser () -> getRenderingProperties () -> shading () = shading ();

	if (shading () == "PHONG")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_SMOOTH);

		getBrowser () -> getRenderingProperties () -> shading () = "GOURAUD";
	}
	else if (shading () == "FLAT")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_FLAT);
	}
	else if (shading () == "WIREFRAME")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel (GL_SMOOTH);
	}
	else if (shading () == "POINTSET")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
		glShadeModel (GL_SMOOTH);
	}
	else  // GOURAUD
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_SMOOTH);

		getBrowser () -> getRenderingProperties () -> shading () = "GOURAUD";
	}
}

}       // X3D
}       // titania

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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "../Browser/Networking/config.h"
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
#include "../Browser/Text/FontStyleOptions.h"
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
	          SplashScreen (new SFBool (false)),
	             Dashboard (new SFBool (true)),
	            RubberBand (new SFBool (true)),
	EnableInlineViewpoints (new SFBool (true)),
	           Antialiased (new SFBool (true)),
	        TextureQuality (new SFString ("MEDIUM")),
	      PrimitiveQuality (new SFString ("MEDIUM")),
	     QualityWhenMoving (new SFString ("MEDIUM")),
	               Shading (new SFString ("GOURAUD")),
	     AnimateStairWalks (new SFBool ()),
	               Gravity (new SFFloat (GN <float>))
{ }

BrowserOptions::BrowserOptions (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     fields (executionContext),
	    shading (ShadingType::GOURAUD)
{
	addType (X3DConstants::BrowserOptions);

	addField (inputOutput, "SplashScreen",           SplashScreen ());
	addField (inputOutput, "Dashboard",              Dashboard ());
	addField (inputOutput, "Rubberband",             RubberBand ());
	addField (inputOutput, "EnableInlineViewpoints", EnableInlineViewpoints ());
	addField (inputOutput, "Antialiased",            Antialiased ());
	addField (inputOutput, "TextureQuality",         TextureQuality ());
	addField (inputOutput, "PrimitiveQuality",       PrimitiveQuality ());
	addField (inputOutput, "QualityWhenMoving",      QualityWhenMoving ());
	addField (inputOutput, "Shading",                Shading ());
	addField (inputOutput, "MotionBlur",             MotionBlur ());

	// Non standard options
	addField (inputOutput, "MotionBlurIntensity",    MotionBlurIntensity ());
	addField (inputOutput, "AnimateStairWalks",      AnimateStairWalks ());
	addField (inputOutput, "Gravity",                Gravity ());

	addField (X3D_V3_3, "AntiAliased", "Antialiased");

	addChildren (shading);
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

	Antialiased ()      .addInterest (this, &BrowserOptions::set_Antialiased);
	TextureQuality ()   .addInterest (this, &BrowserOptions::set_TextureQuality);
	PrimitiveQuality () .addInterest (this, &BrowserOptions::set_PrimitiveQuality);
	Shading ()          .addInterest (this, &BrowserOptions::set_Shading);

	set_Antialiased ();
	set_TextureQuality ();
	set_PrimitiveQuality ();
	set_Shading ();
}

SFBool &
BrowserOptions::MotionBlur ()
{
	return getBrowser () -> getMotionBlur () -> enabled ();
}

const SFBool &
BrowserOptions::MotionBlur () const
{
	return getBrowser () -> getMotionBlur () -> enabled ();
}

SFFloat &
BrowserOptions::MotionBlurIntensity ()
{
	return getBrowser () -> getMotionBlur () -> intensity ();
}

const SFFloat &
BrowserOptions::MotionBlurIntensity () const
{
	return getBrowser () -> getMotionBlur () -> intensity ();
}

void
BrowserOptions::set_Antialiased ()
{
	if (Antialiased ())
	{
		glEnable (GL_MULTISAMPLE);

		GLint sampleBuffers, samples;

		glGetIntegerv (GL_SAMPLE_BUFFERS, &sampleBuffers);
		glGetIntegerv (GL_SAMPLES, &samples);

		getBrowser () -> getRenderingProperties () -> Antialiased () = sampleBuffers and samples;
	}
	else
	{
		glDisable (GL_MULTISAMPLE);
		getBrowser () -> getRenderingProperties () -> Antialiased () = false;
	}
}

void
BrowserOptions::set_TextureQuality ()
{
	const auto & textureProperties        = getBrowser () -> getDefaultTextureProperties ();
	const auto & cubeMapTextureProperties = getBrowser () -> getDefaultCubeMapTextureProperties ();

	if (TextureQuality () == "HIGH")
	{
		textureProperties -> minificationFilter  () = "NICEST";
		textureProperties -> magnificationFilter () = "NICEST";
		textureProperties -> textureCompression  () = "NICEST";
		textureProperties -> generateMipMaps     () = true;

		cubeMapTextureProperties -> minificationFilter  () = "AVG_PIXEL";
		cubeMapTextureProperties -> magnificationFilter () = "AVG_PIXEL";

		glHint (GL_GENERATE_MIPMAP_HINT,        GL_NICEST);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		return;
	}

	if (TextureQuality () == "LOW")
	{
		textureProperties -> minificationFilter  () = "AVG_PIXEL_NEAREST_MIPMAP";
		textureProperties -> magnificationFilter () = "NICEST";
		textureProperties -> textureCompression  () = "FASTEST";
		textureProperties -> generateMipMaps     () = true;

		cubeMapTextureProperties -> minificationFilter  () = "NEAREST_PIXEL";
		cubeMapTextureProperties -> magnificationFilter () = "NEAREST_PIXEL";

		glHint (GL_GENERATE_MIPMAP_HINT,        GL_FASTEST);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		return;
	}

	// MEDIUM

	textureProperties -> minificationFilter  () = "AVG_PIXEL_AVG_MIPMAP";
	textureProperties -> magnificationFilter () = "NICEST";
	textureProperties -> textureCompression  () = "NICEST";
	textureProperties -> generateMipMaps     () = true;

	cubeMapTextureProperties -> minificationFilter  () = "AVG_PIXEL";
	cubeMapTextureProperties -> magnificationFilter () = "AVG_PIXEL";

	glHint (GL_GENERATE_MIPMAP_HINT,        GL_FASTEST);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

void
BrowserOptions::set_PrimitiveQuality ()
{
	if (PrimitiveQuality () == "HIGH")
	{
		getBrowser () -> getArc2DOptions ()      -> minAngle ()   = PI <float> / 40;
		getBrowser () -> getArcClose2DOptions () -> minAngle ()   = PI <float> / 40;
		getBrowser () -> getCircle2DOptions ()   -> dimension ()  = 81;
		getBrowser () -> getDisk2DOptions ()     -> dimension ()  = 81;
		getBrowser () -> getConeOptions ()       -> xDimension () = 33;
		getBrowser () -> getCylinderOptions ()   -> xDimension () = 33;

		const auto quadSphere = dynamic_cast <QuadSphereOptions*> (getBrowser () -> getSphereOptions () .getValue ());

		if (quadSphere)
		{
			quadSphere -> xDimension () = 63;
			quadSphere -> yDimension () = 31;
		}

		getBrowser () -> getFontStyleOptions () -> bezierDimension () = 7;
		return;
	}

	if (PrimitiveQuality () == "LOW")
	{
		getBrowser () -> getArc2DOptions ()      -> minAngle ()   = PI <float> / 10;
		getBrowser () -> getArcClose2DOptions () -> minAngle ()   = PI <float> / 10;
		getBrowser () -> getCircle2DOptions ()   -> dimension ()  = 21;
		getBrowser () -> getDisk2DOptions ()     -> dimension ()  = 21;
		getBrowser () -> getConeOptions ()       -> xDimension () = 17;
		getBrowser () -> getCylinderOptions ()   -> xDimension () = 17;

		const auto quadSphere = dynamic_cast <QuadSphereOptions*> (getBrowser () -> getSphereOptions () .getValue ());

		if (quadSphere)
		{
			quadSphere -> xDimension () = 21;
			quadSphere -> yDimension () = 9;
		}

		getBrowser () -> getFontStyleOptions () -> bezierDimension () = 3;
		return;
	}

	// MEDIUM

	getBrowser () -> getArc2DOptions ()      -> minAngle ()   = PI <float> / 20;
	getBrowser () -> getArcClose2DOptions () -> minAngle ()   = PI <float> / 20;
	getBrowser () -> getCircle2DOptions ()   -> dimension ()  = 41;
	getBrowser () -> getDisk2DOptions ()     -> dimension ()  = 41;
	getBrowser () -> getConeOptions ()       -> xDimension () = 21;
	getBrowser () -> getCylinderOptions ()   -> xDimension () = 21;

	const auto quadSphere = dynamic_cast <QuadSphereOptions*> (getBrowser () -> getSphereOptions () .getValue ());

	if (quadSphere)
	{
		quadSphere -> xDimension () = 31;
		quadSphere -> yDimension () = 15;
	}

	getBrowser () -> getFontStyleOptions () -> bezierDimension () = 5;
}

void
BrowserOptions::set_Shading ()
{
	static const std::map <std::string, ShadingType> shadings = {
		std::make_pair ("POINT",     ShadingType::POINT),
		std::make_pair ("POINTSET",  ShadingType::POINT),
		std::make_pair ("WIREFRAME", ShadingType::WIREFRAME),
		std::make_pair ("FLAT",      ShadingType::FLAT),
		std::make_pair ("GOURAUD",   ShadingType::GOURAUD),
		std::make_pair ("PHONG",     ShadingType::PHONG),
	};

	getBrowser () -> getRenderingProperties () -> Shading () = Shading ();

	try
	{
		shading = shadings .at (Shading ());
	}
	catch (const std::out_of_range &)
	{
		shading = ShadingType::GOURAUD;
	}

	switch (shading)
	{
		case ShadingType::POINT:
		{
			glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
			glShadeModel (GL_SMOOTH);
			break;
		}
		case ShadingType::WIREFRAME:
		{
			glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
			glShadeModel (GL_SMOOTH);
			break;
		}
		case ShadingType::FLAT:
		{
			glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
			glShadeModel (GL_FLAT);
			break;
		}
		case ShadingType::GOURAUD:
		{
			glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
			glShadeModel (GL_SMOOTH);
	
			getBrowser () -> getRenderingProperties () -> Shading () = "GOURAUD";
			break;
		}
		case ShadingType::PHONG:
		{
			glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
			glShadeModel (GL_SMOOTH);

			#ifdef FIXED_PIPELINE
			if (getBrowser () -> getFixedPipelineRequired ())
				getBrowser () -> getRenderingProperties () -> Shading () = "GOURAUD";
			#endif
			break;
		}
	}
}

}       // X3D
}       // titania

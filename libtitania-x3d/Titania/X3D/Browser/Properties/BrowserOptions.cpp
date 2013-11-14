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

#include "../../Components/Layering/Viewport.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/FillProperties.h"
#include "../../Components/Shape/LineProperties.h"
#include "../../Components/Text/FontStyle.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Texturing/TextureProperties.h"
#include "../../Components/Texturing/TextureTransform.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Geometry2D/Arc2DOptions.h"
#include "../Geometry2D/ArcClose2DOptions.h"
#include "../Geometry2D/Circle2DOptions.h"
#include "../Geometry2D/Disk2DOptions.h"
#include "../Geometry2D/Rectangle2DOptions.h"
#include "../Geometry3D/BoxOptions.h"
#include "../Geometry3D/QuadSphereOptions.h"
#include "../Geometry3D/X3DSphereOptionNode.h"
#include "../Properties/MotionBlur.h"
#include "../X3DBrowser.h"

#include "../../Rendering/OpenGL.h"

#include <Titania/Physics/Constants.h>
#include <Titania/String/join.h>

namespace titania {
namespace X3D {

// Table 9.2 — Browser options
// Name                    Type/valid range                         Default                      Description
// SplashScreen            Boolean                                  Implementation-dependent     Display browser splash screen on startup
// Dashboard               Boolean                                  False                        Specified by bound NavigationInfo in content. Display browser navigation user interface.
// Rubberband              Boolean                                  True                         Specified by bound NavigationInfo in content. Display rubber band navigation hint.
// EnableInlineViewpoints  Boolean                                  True                         Viewpoints from Inline nodes are included in list of viewpoints if made available by the Inline node.
// Antialiased             Boolean                                  False                        Render using hardware antialiasing if available
// TextureQuality          Low, Medium, High                        Medium                       Quality of texture map display
// PrimitiveQuality        Low, Medium, High                        Medium                       Render quality (tessellation level) for Box, Cone, Cylinder, Sphere
// QualityWhenMoving       Low, Medium, High,                       Same (as while stationary)   Render quality while camera is moving
// Shading                 Point, Wireframe, Flat, Gouraud, Phong   Gouraud                      Specify shading mode for all objects
// MotionBlur              Boolean                                  False                        Render animations with motion blur
// MotionBlurIntesity      Number                                   0.25                         Motion blur intesity in the range (0, 1)
// AnimateStairWalks       Boolean                                  False                        Animate stair walks. This can give unexpected results when the floor is animated.
// Gravity                 Number                                   g                            Gravitational acceleration. The standard value is the acceleration of the earth.
// MinTextureSize          Number                                   8                            Default minumum texture size when scaling and filtering is applied.

const std::string BrowserOptions::componentName  = "Browser";
const std::string BrowserOptions::typeName       = "BrowserOptions";
const std::string BrowserOptions::containerField = "browserOptions";

BrowserOptions::Fields::Fields (X3DExecutionContext* const executionContext) :
	          splashScreen (new SFBool (false)),
	             dashboard (new SFBool (true)),
	            rubberBand (new SFBool (true)),
	enableInlineViewpoints (new SFBool (true)),
	           antialiased (new SFBool (true)),
	        textureQuality (new SFString ("MEDIUM")),
	      primitiveQuality (new SFString ("MEDIUM")),
	     qualityWhenMoving (new SFString ("MEDIUM")),
	               shading (new SFString ("GOURAUD")),
	     animateStairWalks (new SFBool ()),
	               gravity (new SFFloat (P_GN)),
	        minTextureSize (new SFInt32 (16)),
	     motionBlurOptions (new MotionBlur (executionContext)),
	            appearance (new Appearance (executionContext)),
	        lineProperties (new LineProperties (executionContext)),
	        fillProperties (new FillProperties (executionContext)),
	     textureProperties (new TextureProperties (executionContext)),
	      textureTransform (new TextureTransform (executionContext)),
	                 arc2D (new Arc2DOptions (executionContext)),
	            arcClose2D (new ArcClose2DOptions (executionContext)),
	              circle2D (new Circle2DOptions (executionContext)),
	                disc2D (new Disk2DOptions (executionContext)),
	           rectangle2D (new Rectangle2DOptions (executionContext)),
	                   box (new BoxOptions (executionContext)),
	                sphere (new QuadSphereOptions (executionContext)),
	              texCoord (new TextureCoordinate (executionContext)),
	             fontStyle (new FontStyle (executionContext)),
	              viewport (new Viewport (executionContext))
{ }

BrowserOptions::BrowserOptions (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     fields (executionContext)
{
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
	addField (inputOutput, "MotionBlurIntensity",    motionBlurIntensity ());
	addField (inputOutput, "AnimateStairWalks",      animateStairWalks ());
	addField (inputOutput, "Gravity",                gravity ());
	addField (inputOutput, "MinTextureSize",         minTextureSize ());

	addField ("AntiAliased", "Antialiased");

	addChildren (motionBlurOptions (),
	             appearance (),
	             lineProperties (),
	             fillProperties (),
	             textureProperties (),
	             textureTransform (),
	             arc2D (),
	             arcClose2D (),
	             circle2D (),
	             disc2D (),
	             rectangle2D (),
	             box (),
	             sphere (),
	             texCoord (),
	             fontStyle (),
	             viewport ());
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

	motionBlurOptions () -> setup ();
	appearance ()        -> setup ();
	lineProperties ()    -> setup ();
	fillProperties ()    -> setup ();
	textureProperties () -> setup ();
	textureTransform ()  -> setup ();
	arc2D ()             -> setup ();
	arcClose2D ()        -> setup ();
	circle2D ()          -> setup ();
	disc2D ()            -> setup ();
	rectangle2D ()       -> setup ();
	box ()               -> setup ();
	sphere ()            -> setup ();
	texCoord ()          -> setup ();
	fontStyle ()         -> setup ();
	viewport ()          -> setup ();

	lineProperties () -> applied () = false;
	fillProperties () -> hatched () = false;

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
	return motionBlurOptions () -> enabled ();
}

const SFBool &
BrowserOptions::motionBlur () const
{
	return motionBlurOptions () -> enabled ();
}

SFFloat &
BrowserOptions::motionBlurIntensity ()
{
	return motionBlurOptions () -> intensity ();
}

const SFFloat &
BrowserOptions::motionBlurIntensity () const
{
	return motionBlurOptions () -> intensity ();
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
	if (textureQuality () == "HIGH")
	{
		textureProperties () -> magnificationFilter () = "NICEST";
		textureProperties () -> minificationFilter ()  = "NICEST";
		textureProperties () -> textureCompression ()  = "NICEST";
		textureProperties () -> generateMipMaps ()     = true;

		glHint (GL_GENERATE_MIPMAP_HINT,        GL_NICEST);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		return;
	}

	if (textureQuality () == "LOW")
	{
		textureProperties () -> magnificationFilter () = "FASTEST";
		textureProperties () -> minificationFilter ()  = "FASTEST";
		textureProperties () -> textureCompression ()  = "FASTEST";
		textureProperties () -> generateMipMaps ()     = false;

		glHint (GL_GENERATE_MIPMAP_HINT,        GL_FASTEST);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		return;
	}

	// MEDIUM

	textureProperties () -> magnificationFilter () = "NICEST";
	textureProperties () -> minificationFilter ()  = "AVG_PIXEL_AVG_MIPMAP";
	textureProperties () -> textureCompression ()  = "NICEST";
	textureProperties () -> generateMipMaps ()     = true;

	glHint (GL_GENERATE_MIPMAP_HINT,        GL_FASTEST);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

void
BrowserOptions::set_primitiveQuality ()
{
	std::clog << "Setting primitive quality to " << primitiveQuality () << "." << std::endl;

	if (primitiveQuality () == "HIGH")
	{
		arc2D ()      -> minAngle () = M_PI / 40;
		arcClose2D () -> minAngle () = M_PI / 40;
		circle2D ()   -> segments () = 100;
		disc2D ()     -> segments () = 100;

		auto quadSphere = dynamic_cast <QuadSphereOptions*> (sphere () .getValue ());

		if (quadSphere)
		{
			quadSphere -> uDimension () = 80;
			quadSphere -> vDimension () = 40;
		}

		return;
	}

	if (primitiveQuality () == "LOW")
	{
		arc2D ()      -> minAngle () = M_PI / 10;
		arcClose2D () -> minAngle () = M_PI / 10;
		circle2D ()   -> segments () = 20;
		disc2D ()     -> segments () = 20;

		auto quadSphere = dynamic_cast <QuadSphereOptions*> (sphere () .getValue ());

		if (quadSphere)
		{
			quadSphere -> uDimension () = 20;
			quadSphere -> vDimension () = 10;
		}

		return;
	}

	// MEDIUM

	arc2D ()      -> minAngle () = M_PI / 20;
	arcClose2D () -> minAngle () = M_PI / 20;
	circle2D ()   -> segments () = 60;
	disc2D ()     -> segments () = 60;

	auto quadSphere = dynamic_cast <QuadSphereOptions*> (sphere () .getValue ());

	if (quadSphere)
	{
		quadSphere -> uDimension () = 40;
		quadSphere -> vDimension () = 20;
	}
}

void
BrowserOptions::set_shading ()
{
	std::clog << "Setting shading to " << shading () << "." << std::endl;

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

void
BrowserOptions::dispose ()
{
	motionBlurOptions () .dispose ();
	appearance ()        .dispose ();
	lineProperties ()    .dispose ();
	fillProperties ()    .dispose ();
	textureProperties () .dispose ();
	textureTransform ()  .dispose ();
	arc2D ()             .dispose ();
	arcClose2D ()        .dispose ();
	circle2D ()          .dispose ();
	disc2D ()            .dispose ();
	rectangle2D ()       .dispose ();
	box ()               .dispose ();
	sphere ()            .dispose ();
	texCoord ()          .dispose ();
	fontStyle ()         .dispose ();
	viewport ()          .dispose ();
}

} // X3D
} // titania

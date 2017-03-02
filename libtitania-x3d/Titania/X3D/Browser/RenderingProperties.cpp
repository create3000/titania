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

#include "RenderingProperties.h"

#include "../Browser/Networking/config.h"
#include "../Browser/X3DBrowser.h"
#include "../Execution/World.h"
#include "../Execution/X3DExecutionContext.h"
#include "../InputOutput/Loader.h"

#include "../Components/Layering/X3DLayerNode.h"

#include <Titania/String.h>

#include <iomanip>
#include <iostream>
#include <malloc.h>

namespace titania {
namespace X3D {

// Property Name     Value data type      Description
// Shading           String               The type of shading algorithm in use. Typical values are Flat, Gouraud, Phong, Wireframe.
// MaxTextureSize    String               The maximum texture size supported. The format shall be WIDTHxHEIGHT describing the number of pixels in each direction (for example 1024x1024).
// TextureUnits      Integer              The number of texture units supported for doing multitexture.
// MaxLights         Integer              The maximum number of lights supported.
// AntiAliased       Boolean              True or false if the rendering is currently anti-aliased or not
// ColorDepth        Integer              The number of bits of colour depth supported by the screen. Allows for optimized selection of textures, particularly for lower colour depth screen capabilities.
// TextureMemory     Float                The amount of memory in megabytes available for textures to be placed on the video card.

const ComponentType RenderingProperties::component      = ComponentType::TITANIA;
const std::string   RenderingProperties::typeName       = "RenderingProperties";
const std::string   RenderingProperties::containerField = "renderingProperties";

RenderingProperties::Fields::Fields () :
	        Enabled (),
	  CycleInterval (1),
	         Vendor (new SFString ()),
	       Renderer (new SFString ()),
	        Version (new SFString ()),
	        Shading (new SFString ("GOURAUD")),
	 MaxTextureSize (new SFInt32 ()),
	   TextureUnits (new SFInt32 ()),
	      MaxLights (new SFInt32 ()),
	    Antialiased (new SFBool ()),
	     ColorDepth (new SFInt32 ()),
	  TextureMemory (new SFDouble ())
{ }

RenderingProperties::RenderingProperties (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     fields (),
	    shading (ShadingType::GOURAUD),
	      world ()
{
	addType (X3DConstants::RenderingProperties);

	addField (outputOnly, "Vendor",         Vendor ());
	addField (outputOnly, "Renderer",       Renderer ());
	addField (outputOnly, "Version",        Version ());

	addField (outputOnly, "Shading",        Shading ());
	addField (outputOnly, "MaxTextureSize", MaxTextureSize ());
	addField (outputOnly, "TextureUnits",   TextureUnits ());
	addField (outputOnly, "MaxLights",      MaxLights ());
	addField (outputOnly, "Antialiased",    Antialiased ());
	addField (outputOnly, "ColorDepth",     ColorDepth ());
	addField (outputOnly, "TextureMemory",  TextureMemory ());

	addField (X3D_V3_3, "AntiAliased", "Antialiased");

	addChildObjects (Enabled (), CycleInterval (), shading, world);
}

RenderingProperties*
RenderingProperties::create (X3DExecutionContext* const executionContext)  const
{
	return new RenderingProperties (executionContext);
}

void
RenderingProperties::initialize ()
{
	X3DBaseNode::initialize ();

	if (glXGetCurrentContext ())
	{
		Vendor ()   = getBrowser () -> getVendor ();
		Renderer () = getBrowser () -> getRenderer ();
		Version ()  = getBrowser () -> getGLVersion ();

		GLint glRedBits, glGreen, glBlueBits, glAlphaBits;

		glGetIntegerv (GL_RED_BITS,   &glRedBits);
		glGetIntegerv (GL_GREEN_BITS, &glGreen);
		glGetIntegerv (GL_BLUE_BITS,  &glBlueBits);
		glGetIntegerv (GL_ALPHA_BITS, &glAlphaBits);

		TextureUnits ()   = getBrowser () -> getMaxTextureUnits ();
		MaxTextureSize () = getBrowser () -> getMaxTextureSize ();
		TextureMemory ()  = double (getBrowser () -> getTextureMemory ()) / (1 << 20);
		MaxLights ()      = getBrowser () -> getMaxLights ();
		ColorDepth ()     = glRedBits + glGreen + glBlueBits + glAlphaBits;

		Enabled () .addInterest (&RenderingProperties::set_Enabled, this);
		Shading () .addInterest (&RenderingProperties::set_Shading, this);

		getBrowser () -> initialized () .addInterest (&RenderingProperties::set_Enabled, this);
		getBrowser () -> getViewport () .addInterest (&RenderingProperties::build, this);
	}
}

void
RenderingProperties::set_Enabled ()
{
	getBrowser () -> initialized () .removeInterest (&RenderingProperties::set_Enabled, this);

	if (Enabled ())
	{
		if (not world)
		{
			// Visual display of RenderingProperties

			X3DScenePtr scene;

			try
			{
				scene = Loader (getBrowser () -> getPrivateScene ()) .createX3DFromURL ({ get_tool ("RenderingProperties.x3dv") .str () });
			}
			catch (const X3DError & error)
			{
				std::clog << error .what () << std::endl;

				scene = getBrowser () -> createScene ();
			}

			world = new World (scene);
			world -> setup ();
		}

		getBrowser () -> initialized ()   .addInterest (&RenderingProperties::reset, this);
		getBrowser () -> prepareEvents () .addInterest (&RenderingProperties::prepare, this);
		getBrowser () -> displayed ()     .addInterest (&RenderingProperties::display, this);

		reset ();
	}
	else
	{
		getBrowser () -> initialized ()   .removeInterest (&RenderingProperties::reset, this);
		getBrowser () -> prepareEvents () .removeInterest (&RenderingProperties::prepare, this);
		getBrowser () -> displayed ()     .removeInterest (&RenderingProperties::display, this);
	}
}

void
RenderingProperties::set_Shading ()
{
	static const std::map <std::string, ShadingType> shadings = {
		std::make_pair ("POINT",     ShadingType::POINT),
		std::make_pair ("POINTSET",  ShadingType::POINT),
		std::make_pair ("WIREFRAME", ShadingType::WIREFRAME),
		std::make_pair ("FLAT",      ShadingType::FLAT),
		std::make_pair ("GOURAUD",   ShadingType::GOURAUD),
		std::make_pair ("PHONG",     ShadingType::PHONG),
	};

	try
	{
		shading = shadings .at (Shading ());
	}
	catch (const std::out_of_range &)
	{
		shading = ShadingType::GOURAUD;
	}
}

void
RenderingProperties::reset ()
{
	clock       .reset ();
	renderClock .reset ();

	build ();
}

void
RenderingProperties::prepare ()
{
	renderClock .start ();
}

void
RenderingProperties::display ()
{
	clock       .stop ();
	renderClock .stop ();

	if (clock .interval () > CycleInterval ())
	{
		build ();

		clock       .reset ();
		renderClock .reset ();
	}

	// Display statistics

	PolygonModeLock polygonMode (GL_FILL);

	world -> traverse (TraverseType::DISPLAY, nullptr);
}

static
std::string
format_time (const time_type & time, const size_t fractions = 0)
{
	auto       t = std::floor (time);
	const auto f = std::floor ((t - time) * std::pow (10, fractions));

	const int32_t s = std::fmod (t, 60);

	t = std::floor (t / 60);

	const int32_t m = std::fmod (t, 60);
	t = std::floor (t / 60);

	const int32_t h = std::fmod (t, 24);
	t = std::floor (t / 24);

	const auto d = t;

	if (fractions)
		return basic::sprintf (("%02.0f:%02d:%02d:%02d.%0" + basic::to_string (fractions, std::locale::classic ()) + ".0f") .c_str (), d, h, m, s, f);

	return basic::sprintf ("%02.0f:%02d:%02d:%02d", d, h, m, s);
}

void
RenderingProperties::build ()
{
	try
	{
		GLint sampleBuffers, samples;
	
		glGetIntegerv (GL_SAMPLE_BUFFERS, &sampleBuffers);
		glGetIntegerv (GL_SAMPLES, &samples);
	
		size_t numOpaqueShapes      = 0;
		size_t numTransparentShapes = 0;
	
		for (const auto & layer : getBrowser () -> getWorld () -> getLayerSet () -> getLayers ())
		{
			numOpaqueShapes      += layer -> getNumOpaqueShapes ();
			numTransparentShapes += layer -> getNumTransparentShapes ();
		}
	
		const auto statistics = world -> getExecutionContext () -> getNamedNode ("RenderingProperties");
		auto &     string     = statistics -> getField <MFString> ("string");

		string .clear ();

		std::ostringstream stringstream;

		string .emplace_back (_ ("Current Graphics Renderer"));
		string .emplace_back (basic::sprintf (_ ("  Name: %s"), Renderer () .c_str ()));
		string .emplace_back ();
		string .emplace_back (_ ("Rendering properties"));
		string .emplace_back (basic::sprintf (_ ("Viewport:                  %d × %d pixel"), getBrowser () -> getViewport () [2] .getValue (), getBrowser () -> getViewport () [3] .getValue ()));
		string .emplace_back (basic::sprintf (_ ("Texture units:             %zd / %zd"), getBrowser () -> getTextureUnits () .size (), getBrowser () -> getCombinedTextureUnits () .size ()));
		string .emplace_back (basic::sprintf (_ ("Max texture size:          %zd × %zd pixel"), getBrowser () -> getMaxTextureSize (), getBrowser () -> getMaxTextureSize ()));
		string .emplace_back (basic::sprintf (_ ("Antialiased:               %s (%d/%d)"), Antialiased () .toString () .c_str (), sampleBuffers, samples));
		string .emplace_back (basic::sprintf (_ ("Max lights:                %d"), MaxLights () .getValue ()));
		string .emplace_back (basic::sprintf (_ ("Max clip planes:           %zd"), getBrowser () -> getMaxClipPlanes ()));
		string .emplace_back (basic::sprintf (_ ("Color depth:               %d bits"), ColorDepth () .getValue ()));
		string .emplace_back (basic::sprintf (_ ("Texture memory:            %s"), strfsize (getBrowser () -> getTextureMemory ()) .c_str ()));
		string .emplace_back (basic::sprintf (_ ("Available texture memory:  %s"), strfsize (getBrowser () -> getAvailableTextureMemory ()) .c_str ()));
		string .emplace_back (basic::sprintf (_ ("Memory usage:              %s"), strfsize (getBrowser () -> getMemoryUsage ()) .c_str ()));
		string .emplace_back ();
		string .emplace_back (basic::sprintf (_ ("Elapsed time:              %s"), format_time (chrono::now () - getBrowser () -> initialized ()) .c_str ()));
		string .emplace_back (basic::sprintf (_ ("Speed:                     %.2f m/s"), getBrowser () -> getCurrentSpeed ()));
		string .emplace_back (basic::sprintf (_ ("Frame rate:                %.1f fps"), getFPS ()));
		string .emplace_back (basic::sprintf (_ ("Display:                   %.2f %%"), 100 * renderClock .average () / clock .average ()));
		string .emplace_back (basic::sprintf (_ ("Shapes:                    %zd + %zd"), numOpaqueShapes, numTransparentShapes));
		string .emplace_back (basic::sprintf (_ ("Sensors:                   %zd"), getBrowser () -> sensors () .getRequesters () .size () + getBrowser () -> prepareEvents () .getRequesters () .size () - 2));
		// Remove BrowserTimings and Console from sensors (-2).
	}
	catch (const X3DError & error)
	{
		// Catch error from getNamedNode.
		//__LOG__ << error .what () << std::endl;
	}
}

void
RenderingProperties::toStream (std::ostream & stream) const
{
	if (glXGetCurrentContext ())
	{
		stream
			<< "\tCurrent Graphics Renderer" << std::endl
			<< "\t\tName: " << Vendor () .getValue () << ' ' << Renderer () .getValue () << std::endl
			<< "\tOpenGL extension version: " << Version () .getValue () << std::endl
			<< "\tShading language version: " << glGetString (GL_SHADING_LANGUAGE_VERSION) << ", 1.0 es Cobweb (default)" << std::endl

			<< "\tRendering Properties" << std::endl
			<< "\t\tTexture units: " << TextureUnits () << " / " << getBrowser () -> getMaxCombinedTextureUnits () - getBrowser () -> getMaxTextureUnits () << std::endl
			<< "\t\tMax texture size: " << MaxTextureSize () << " × " << MaxTextureSize () << " pixel" << std::endl
			<< "\t\tMax lights: " << MaxLights () << std::endl
			<< "\t\tMax clip planes: " << getBrowser () -> getMaxClipPlanes () << std::endl
			<< "\t\tAntialiased: " << Antialiased () .getValue () << std::endl
			<< "\t\tColor depth: " << ColorDepth () << " bits" << std::endl
			<< "\t\tTexture memory: " << (TextureMemory () > 0 ? strfsize (TextureMemory ()) : "n/a");
	}
}

void
RenderingProperties::dispose ()
{
	X3DBaseNode::dispose ();
}

} // X3D
} // titania

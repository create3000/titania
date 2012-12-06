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

#include "RenderingProperties.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Browser.h"
#include <Titania/String/Join.h>
#include <Titania/String/Split.h>
#include <Titania/String/strfsize.h>
#include <iomanip>
#include <iostream>

#include <sys/resource.h>

namespace titania {
namespace X3D {

// Property Name     Value data type      Description
// Shading           String               The type of shading algorithm in use. Typical values are Flat, Gouraud, Phong, Wireframe.
// MaxTextureSize    String               The maximum texture size supported. The format shall be WIDTHxHEIGHT describing the number of pixels in each direction (for example 1024x1024).
// TextureUnits      Integer              The number of texture units supported for doing multitexture.
// AntiAliased       Boolean              True or false if the rendering is currently anti-aliased or not
// ColorDepth        Integer              The number of bits of colour depth supported by the screen. Allows for optimized selection of textures, particularly for lower colour depth screen capabilities.
// TextureMemory     Float                The amount of memory in megabytes available for textures to be placed on the video card.

RenderingProperties::RenderingProperties (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),              
	  X3DChildNode (),                                                                 
	       enabled (),                                                                 
	 cycleInterval (1),                                                                // SFFloat  [in,out] cycleInterval  1
	        vendor (),                                                                 
	      renderer (),                                                                 
	       version (),                                                                 
	       shading (),                                                                 
	maxTextureSize (),                                                                 
	  textureUnits (),                                                                 
	     maxLights (),                                                                 
	   antiAliased (),                                                                 
	    colorDepth (),                                                                 
	 textureMemory (),                                                                 
	        string (),                                                                 // MFString [in,out] string
	    extensions (),                                                                 
	    fontFamily ("-schumacher-clean-medium-r-normal--12-120-75-75-c-60-iso8859-1"), // SFString [in,out] fontFamily
	    fontHeigth (0),                                                                
	      fontInfo (nullptr),                                                          
	        listId (0)                                                                 
{
	setComponent ("Browser"),
	setTypeName ("RenderingProperties");

	appendField (outputOnly, "enabled",        enabled);
	appendField (outputOnly, "cycleInterval",  cycleInterval);
	appendField (outputOnly, "vendor",         vendor);
	appendField (outputOnly, "renderer",       renderer);
	appendField (outputOnly, "version",        version);
	appendField (outputOnly, "shading",        shading);                           // doppelt
	appendField (outputOnly, "maxTextureSize", maxTextureSize);
	appendField (outputOnly, "textureUnits",   textureUnits);
	appendField (outputOnly, "maxLights",      maxLights);
	appendField (outputOnly, "antiAliased",    antiAliased);
	appendField (outputOnly, "colorDepth",     colorDepth);
	appendField (outputOnly, "textureMemory",  textureMemory);
	appendField (outputOnly, "string",         string);
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
		vendor   = (const char*) glGetString (GL_VENDOR);
		renderer = (const char*) glGetString (GL_RENDERER);
		version  = (const char*) glGetString (GL_VERSION);

		extensions = std::move (basic::ssplit ((char*) glGetString (GL_EXTENSIONS), " "));

		//extensions .push_back (std::string ("GLEW ") + (const char*) glewGetString (GLEW_VERSION));

		GLint glMaxTextureSize, glMaxLights, glTextureUnits;
		GLint glRedBits, glGreen, glBlueBits, glAlphaBits;
		GLint glPolygonSmooth;
		GLint glTextureMemory = -1;

		glGetIntegerv (GL_MAX_TEXTURE_SIZE,        &glMaxTextureSize);
		glGetIntegerv (GL_MAX_TEXTURE_IMAGE_UNITS, &glTextureUnits);
		glGetIntegerv (GL_MAX_LIGHTS,              &glMaxLights);

		glGetIntegerv (GL_RED_BITS,   &glRedBits);
		glGetIntegerv (GL_GREEN_BITS, &glGreen);
		glGetIntegerv (GL_BLUE_BITS,  &glBlueBits);
		glGetIntegerv (GL_ALPHA_BITS, &glAlphaBits);

		glGetIntegerv (GL_POLYGON_SMOOTH, &glPolygonSmooth);

		if (hasExtension ("GL_NVX_gpu_memory_info"))
			glGetIntegerv (GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &glTextureMemory);  // in KBytes

		textureUnits   = glTextureUnits;
		maxTextureSize = glMaxTextureSize;
		textureMemory  = glTextureMemory * 1024;
		maxLights      = glMaxLights;
		antiAliased    = glPolygonSmooth;
		colorDepth     = glRedBits + glGreen + glBlueBits + glAlphaBits;

		// Display

		listId = glGenLists (1);
		set_fontFamily ();

		enabled .addInterest (this, &RenderingProperties::set_enabled);
		set_enabled ();
	}
}

//
//http://developer.download.nvidia.com/opengl/specs/GL_NVX_gpu_memory_info.txt
//A call to glGetString(GL_EXTENSIONS) will return a space-separated string of extension names, which your application can parse at runtime.

bool
RenderingProperties::hasExtension (const std::string & name)
{
	return extensions .find (name) not_eq extensions .end ();
}

size_t
RenderingProperties::getAvailableTextureMemory ()
{
	if (hasExtension ("GL_NVX_gpu_memory_info"))
	{
		GLint kbytes = 0;

		glGetIntegerv (GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &kbytes); // in KBytes
		return 1024 * kbytes;
	}

	//http://www.opengl.org/registry/specs/ATI/meminfo.txt
	if (hasExtension ("GL_ATI_meminfo"))
	{
		// VBO_FREE_MEMORY_ATI                     0x87FB
		// TEXTURE_FREE_MEMORY_ATI                 0x87FC
		// RENDERBUFFER_FREE_MEMORY_ATI            0x87FD
		//
		// param[0] - total memory free in the pool
		// param[1] - largest available free block in the pool
		// param[2] - total auxiliary memory free
		// param[3] - largest auxiliary free block

		GLint kbytes [4] = { 0, 0, 0, 0 };

		// glGetIntegerv (TEXTURE_FREE_MEMORY_ATI, &kbytes); // in KBytes
		return 1024 * kbytes [0];
	}

	return 0;
}

void
RenderingProperties::set_enabled ()
{
	if (enabled)
	{
		getBrowser () -> initialized .addInterest (this, &RenderingProperties::reset);
		getBrowser () -> exposed     .addInterest (this, &RenderingProperties::prepare);
		getBrowser () -> displayed   .addInterest (this, &RenderingProperties::display);

		reset ();
	}
	else
	{
		getBrowser () -> initialized .removeInterest (this, &RenderingProperties::reset);
		getBrowser () -> exposed     .removeInterest (this, &RenderingProperties::prepare);
		getBrowser () -> displayed   .removeInterest (this, &RenderingProperties::display);
	}
}

void
RenderingProperties::set_fontFamily ()
{
	if (fontInfo)
		glDeleteLists (fontListBase, fontInfo -> max_char_or_byte2);

	Display* display = XOpenDisplay (nullptr);
	fontInfo = XLoadQueryFont (display, fontFamily .getValue () .c_str ());

	if (fontInfo)
	{
		fontListBase = glGenLists (fontInfo -> max_char_or_byte2);
		fontHeigth   = fontInfo -> ascent + fontInfo -> descent;
		glXUseXFont (fontInfo -> fid, 0, fontInfo -> max_char_or_byte2, fontListBase);
	}
	else
		std::clog << "Warning Statistics: Can't load font " << fontFamily << std::endl;
}

void
RenderingProperties::reset ()
{
	clock       .reset ();
	renderClock .reset ();

	build ();
	glCallList (listId);
}

void
RenderingProperties::build ()
{
	glNewList (listId, GL_COMPILE);

	GLfloat viewport [4];

	glGetFloatv (GL_VIEWPORT, viewport);

	GLfloat width  = viewport [2];
	GLfloat height = viewport [3];

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, width, 0, height, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	glDisable (GL_DEPTH_TEST);
	glColor3f (197.0 / 255.0, 96.0 / 255.0, 0);

	// Display stats.

	int i = 0;
	update_string ();

	for (const auto & line : string)
	{
		glPushMatrix ();
		glLoadIdentity ();
		glRasterPos2f (10, 10 + (string .size () - i - 1) * fontHeigth);
		glListBase (fontListBase);
		glCallLists (line .length (), GL_UNSIGNED_BYTE, line .getValue () .c_str ());
		glPopMatrix ();
		++ i;
	}

	glEndList ();
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

	if (clock .interval () > cycleInterval)
	{
		//			maxEventTime = std::max (maxEventTime,    getBrowser () -> getEventTime ());
		//			maxTraverseTime = std::max (maxTraverseTime, getBrowser () -> getRenderer () -> getTraverseTime ());
		//			maxDrawTime     = std::max (maxDrawTime,     getBrowser () -> getRenderer () -> getDrawTime ());

		build ();

		clock       .reset ();
		renderClock .reset ();
	}

	glCallList (listId);
}

void
RenderingProperties::update_string ()
{
	//	   time_type
	//       eventTime = getBrowser () -> getEventTime (),
	//	      traverseTime = getBrowser () -> getRenderer () -> getTraverseTime (),
	//	      renderInterval     = getBrowser () -> getRenderer () -> getDrawTime ();

	string .clear ();

	std::ostringstream stringstream;

	stringstream .str (""); stringstream << "Current Graphics Renderer";
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "  Name: " << renderer .getValue ();
	string .push_back (stringstream .str ());

	stringstream .str ("");
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Rendering properties";
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Texture units:             " << textureUnits;
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Max texture size:          " << maxTextureSize << " × " << maxTextureSize << " pixel";
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Antialiased:               " << antiAliased;
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Color depth:               " << colorDepth << " bits";
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Texture Memory:            " << (textureMemory > 0 ? strfsize (textureMemory) : "n/a");
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Available Texture Memory:  " << strfsize (getAvailableTextureMemory ());
	string .push_back (stringstream .str ());

	struct rusage usage;
	getrusage (RUSAGE_SELF, &usage);

	stringstream .str (""); stringstream << "Memory Usage:              " << strfsize (usage .ru_maxrss * 1024);
	string .push_back (stringstream .str ());

	stringstream .str ("");
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Frame Rate:                " << std::setprecision (1) << std::fixed << 1 / clock .average ();
	string .push_back (stringstream .str ());

	stringstream .str (""); stringstream << "Render Time:               " << std::setprecision (7) << std::fixed << renderClock .average ();
	string .push_back (stringstream .str ());

	//	//	stringstream .str (""); stringstream << "Event Time:    " << std::setprecision (7) << std::fixed << eventTime << " s (" << maxEventTime << ')';
	//	//	string .push_back (stringstream .str ());
	//
	//	//	stringstream .str (""); stringstream << "Traverse Time: " << std::setprecision (7) << std::fixed << traverseTime << " s (" << maxTraverseTime << ')';
	//	// string .push_back (stringstream .str ());
	//
	//	// stringstream .str (""); stringstream << "Draw Time:                 " << std::setprecision (7) << std::fixed << renderInterval << " s (" << maxDrawTime << ')';
	//	// string .push_back (stringstream .str ());
	//
	//	//	stringstream .str (""); stringstream << "Nodes:         " << getBrowser () -> getRenderer () -> getNumNodesDrawn () << " / " << getBrowser () -> getRenderer () -> getNumNodes ();
	//	// string .push_back (stringstream .str ());
	//
	//	//	stringstream .str (""); stringstream << "Transparent:   " << getBrowser () -> getRenderer () -> getNumTransparentNodesDrawn () << " / " << getBrowser () -> getRenderer () -> getNumTransparentNodes ();
	//	//	string .push_back (stringstream .str ());
	//
	//	//	stringstream .str (""); stringstream << "Textures:      " << getBrowser () -> getNumTextures ();
	//	//	string .push_back (stringstream .str ());
	//
	//	stringstream .str (""); stringstream << "Speed:                     " << std::setprecision (3) << std::fixed << getBrowser () -> getCurrentSpeed () << " m/s";
	//	string .push_back (stringstream .str ());
}

void
RenderingProperties::toStream (std::ostream & stream) const
{
	stream
		<< "\tCurrent Graphics Renderer" << std::endl
		<< "\t\tName: " << vendor .getValue () << ' ' << renderer .getValue () << std::endl
		<< "\tOpenGL extension version: " << version .getValue () << std::endl

		<< "\tRendering properties" << std::endl
		<< "\t\tTexture units: " << textureUnits << std::endl
		<< "\t\tMax texture size: " << maxTextureSize << " × " << maxTextureSize << " pixel" << std::endl
		<< "\t\tMax lights: " << maxLights << std::endl
		<< "\t\tAntialiased: " << antiAliased .getValue () << std::endl
		<< "\t\tColor depth: " << colorDepth << " bits" << std::endl
		<< "\t\tTexture memory: " << (textureMemory > 0 ? strfsize (textureMemory) : "n/a");
}

void
RenderingProperties::dispose ()
{
	X3DChildNode::dispose ();

	if (fontInfo)
		glDeleteLists (fontListBase, fontInfo -> max_char_or_byte2);

	if (listId)
		glDeleteLists (listId, 1);
}

} // X3D
} // titania

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

#include "X3DShadersContext.h"

#include "../../Components/Networking/LoadSensor.h"
#include "../../Components/Shaders/ComposedShader.h"
#include "../../Components/Shaders/ShaderPart.h"
#include "../../Rendering/OpenGL.h"
#include "../Core/BrowserOptions.h"
#include "../Networking/config.h"
#include "../X3DBrowser.h"

#include <regex>

namespace titania {
namespace X3D {

X3DShadersContext::X3DShadersContext () :
	              X3DBaseNode (),
	                  shaders (true),
	   shadingLanguageVersion (),
	  maxVertexUniformVectors (0),
	maxFragmentUniformVectors (0),
	      maxVertexAttributes (0),
	              pointShader (),
	          wireframeShader (),
	            gouraudShader (),
	              phongShader (),
	            defaultShader (),
	             shadowShader ()
{
	addChildObjects (shaders,
                    pointShader,
	                 wireframeShader,
	                 gouraudShader,
	                 phongShader,
	                 defaultShader,
	                 shadowShader);
}

void
X3DShadersContext::initialize ()
{
	if (getBrowser () -> getLoadUrlObjects ())
	{
		std::istringstream shadingLanguageVersionStream ((const char*) glGetString (GL_SHADING_LANGUAGE_VERSION));

		shadingLanguageVersionStream .imbue (std::locale::classic ());

		shadingLanguageVersionStream >> shadingLanguageVersion;

		glGetIntegerv (GL_MAX_VERTEX_UNIFORM_VECTORS,   &maxVertexUniformVectors);
		glGetIntegerv (GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniformVectors);
		glGetIntegerv (GL_MAX_VERTEX_ATTRIBS,           &maxVertexAttributes);

		// Shaders

		if (getBrowser () -> getSharedContext ())
		{
			pointShader     = getBrowser () -> getSharedContext () -> getPointShader ();
			wireframeShader = getBrowser () -> getSharedContext () -> getWireframeShader ();
			gouraudShader   = getBrowser () -> getSharedContext () -> getGouraudShader ();
			phongShader     = getBrowser () -> getSharedContext () -> getPhongShader ();
			shadowShader    = getBrowser () -> getSharedContext () -> getShadowShader ();

			getBrowser () -> getLoadSensor () -> watchList () .append (pointShader     -> parts ());
			getBrowser () -> getLoadSensor () -> watchList () .append (wireframeShader -> parts ());
			getBrowser () -> getLoadSensor () -> watchList () .append (gouraudShader   -> parts ());
			getBrowser () -> getLoadSensor () -> watchList () .append (phongShader     -> parts ());
			getBrowser () -> getLoadSensor () -> watchList () .append (shadowShader    -> parts ());
		}
		else
		{
			pointShader     = createShader ("TitaniaPointSet",  { get_shader ("Shaders/PointSet.vs")  .str () }, { get_shader ("Shaders/PointSet.fs")  .str () });
			wireframeShader = createShader ("TitaniaWireframe", { get_shader ("Shaders/Wireframe.vs") .str () }, { get_shader ("Shaders/Wireframe.fs") .str () });
			gouraudShader   = createShader ("TitaniaGouraud",   { get_shader ("Shaders/Gouraud.vs")   .str () }, { get_shader ("Shaders/Gouraud.fs")   .str () });
			phongShader     = createShader ("TitaniaPhong",     { get_shader ("Shaders/Phong.vs")     .str () }, { get_shader ("Shaders/Phong.fs")     .str () });
			shadowShader    = createShader ("TitaniaShadow",    { get_shader ("Shaders/Phong.vs")     .str () }, { get_shader ("Shaders/Phong.fs")     .str () }, true);
		}

		// Shading

		getBrowser () -> getLoadSensor () -> isLoaded ()       .addInterest (&X3DShadersContext::set_loaded,  this);
		getBrowser () -> getBrowserOptions () -> getShading () .addInterest (&X3DShadersContext::set_shading, this);

		set_shading ();
	}
}

X3DPtr <ComposedShader>
X3DShadersContext::createShader (const std::string & name, const MFString & vertexUrl, const MFString & fragmentUrl, const bool shadow)
{
	const auto vertexPart   = getExecutionContext () -> createNode <ShaderPart> ();
	const auto fragmentPart = getExecutionContext () -> createNode <ShaderPart> ();
	const auto shader       = getExecutionContext () -> createNode <ComposedShader> ();

	fragmentPart -> setName (name + "FragmentShaderPart");
	vertexPart   -> setName (name + "VertexShaderPart");
	shader       -> setName (name + "ComposedShader");

	vertexPart   -> setShadow (shadow);
	fragmentPart -> setShadow (shadow);

	fragmentPart -> type () = "FRAGMENT";
	vertexPart   -> url ()  = vertexUrl;
	fragmentPart -> url ()  = fragmentUrl;

	shader -> parts () .emplace_back (vertexPart);
	shader -> parts () .emplace_back (fragmentPart);

	getBrowser () -> getLoadSensor () -> watchList () .emplace_back (vertexPart);
	getBrowser () -> getLoadSensor () -> watchList () .emplace_back (fragmentPart);

	return shader;
}

void
X3DShadersContext::set_loaded ()
{
	getBrowser () -> getLoadSensor () -> isLoaded () .removeInterest (&X3DShadersContext::set_loaded,  this);

	if (not phongShader -> isValid ())
		phongShader = gouraudShader;

	if (not shadowShader -> isValid ())
		shadowShader = gouraudShader;

	set_shading ();
}

void
X3DShadersContext::set_shading ()
{
	const auto & shading = getBrowser () -> getBrowserOptions () -> getShading ();

	if (shading == ShadingType::PHONG)
	{
		defaultShader = getPhongShader ();
	}
	else // GOURAUD
	{
		defaultShader = getGouraudShader ();
	}

	try
	{
		ContextLock lock (getBrowser ());

		glEnable (GL_POINT_SPRITE);
		glEnable (GL_PROGRAM_POINT_SIZE);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }
}

X3DShadersContext::~X3DShadersContext ()
{ }

} // X3D
} // titania

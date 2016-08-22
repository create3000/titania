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

#include "X3DShadersContext.h"

#include "../../Components/Networking/LoadSensor.h"
#include "../../Components/Shaders/ComposedShader.h"
#include "../../Components/Shaders/ShaderPart.h"
#include "../../Rendering/OpenGL.h"
#include "../Networking/config.h"
#include "../RenderingProperties.h"
#include "../X3DBrowser.h"

namespace titania {
namespace X3D {

X3DShadersContext::X3DShadersContext () :
	           X3DBaseNode (),
	shadingLanguageVersion (),
#ifndef SHADER_PIPELINE
	        shaderPipeline (false),
#endif
	           pointShader (),
	       wireframeShader (),
	         gouraudShader (),
	           phongShader (),
	         defaultShader (),
	            shaderNode (nullptr)
{
	addChildren (pointShader,
	             wireframeShader,
	             gouraudShader,
	             phongShader,
	             defaultShader);
}

void
X3DShadersContext::initialize ()
{
	if (glXGetCurrentContext ())
	{
		// shadingLanguageVersionStream

		std::istringstream shadingLanguageVersionStream ((const char*) glGetString (GL_SHADING_LANGUAGE_VERSION));

		shadingLanguageVersionStream .imbue (std::locale::classic ());

		shadingLanguageVersionStream >> shadingLanguageVersion;

		// Shaders

		pointShader     = createShader ({ get_shader ("Shaders/Wireframe.vs") .str () }, { get_shader ("Shaders/Point.fs")     .str () });
		wireframeShader = createShader ({ get_shader ("Shaders/Wireframe.vs") .str () }, { get_shader ("Shaders/Wireframe.fs") .str () });
		gouraudShader   = createShader ({ get_shader ("Shaders/Gouraud.vs")   .str () }, { get_shader ("Shaders/Gouraud.fs")   .str () });
		phongShader     = createShader ({ get_shader ("Shaders/Phong.vs")     .str () }, { get_shader ("Shaders/Phong.fs")     .str () });
			
		pointShader     -> setGeometryType (0);
		wireframeShader -> setGeometryType (1);

		// Shading

		getBrowser () -> getRenderingProperties () -> getShading () .addInterest (this, &X3DShadersContext::set_shading);

		set_shading (getBrowser () -> getRenderingProperties () -> getShading ());
	}
}

#ifndef SHADER_PIPELINE
void
X3DShadersContext::setShaderPipeline (const bool value)
{
	shaderPipeline = value;

	set_shading (getBrowser () -> getRenderingProperties () -> getShading ());
}
#endif

X3DPtr <ComposedShader>
X3DShadersContext::createShader (const MFString & vertexUrl, const MFString & fragmentUrl)
{
	const auto vertexPart   = getExecutionContext () -> createNode <ShaderPart> ();
	const auto fragmentPart = getExecutionContext () -> createNode <ShaderPart> ();
	const auto shader       = getExecutionContext () -> createNode <ComposedShader> ();

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
X3DShadersContext::set_shading (const ShadingType & shading)
{
	if (shading == ShadingType::PHONG)
	{
		defaultShader = getPhongShader ();
	}
	else // GOURAUD
	{
		#ifndef SHADER_PIPELINE

		if (shaderPipeline)
			defaultShader = getGouraudShader ();
		else
			defaultShader = nullptr;

		#else

		defaultShader = getGouraudShader ();

		#endif
	}
}

X3DShadersContext::~X3DShadersContext ()
{ }

} // X3D
} // titania

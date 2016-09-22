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

#include "Shader.h"

#include "../../InputOutput/Loader.h"

#include <Titania/String/to_string.h>
#include <regex>

namespace titania {
namespace X3D {
namespace Shader {

bool
isOpenGLES (const std::string & source)
{
	// OpenGL ES1 shaders use '#version 100' directive.
	// OpenGL ES2 shaders use '#version 100' directive, there is no '#version 200'.
	// OpenGL ES3 shaders use '#version 300 es' directive.

	static const std::regex version_es (R"/(^(\s*|/\*.*?\*/|//.*?\n)*(#version\s+100|#version\s+300\s+es)\s*?\n)/");

	return std::regex_search (source, version_es);
}

static
std::string
getShaderSource (X3DBaseNode* const node, const std::string & string, const basic::uri & worldURL, const size_t level, std::set <basic::uri> & files)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	static const std::regex include (R"/(^#pragma\s+X3D\s+include\s+\"(.*?)\"\s*$)/");

	if (not files .insert (worldURL) .second)
		return "";

	if (level > 1024)
		throw Error <INVALID_URL> ("Header inclusion depth limit reached, might be caused by cyclic header inclusion.");

	std::istringstream input (string);
	std::ostringstream output;

	input  .imbue (std::locale::classic ());
	output .imbue (std::locale::classic ());

	size_t      lineNumber = 0;
	std::string line;

	if (level)
		output << "#line "<< lineNumber << " \"" << worldURL .filename () << "(" << node -> getName () << ")\""  << std::endl;

	while (std::getline (input, line))
	{
		std::smatch filename;

		if (std::regex_match (line, filename, include))
		{
			Loader loader (node -> getExecutionContext ());
			output << getShaderSource (node, loader .loadDocument (worldURL .transform (filename .str (1))), loader .getWorldURL (), level + 1, files) << std::endl;
			output << "#line "<< lineNumber + 1 << " \"" << worldURL << "\""  << std::endl;
		}
		else
		{
			output << line << std::endl;
		}

		++ lineNumber;
	}

	return output .str ();
}

GLenum
getShaderType (const std::string & type)
{
	// http://www.opengl.org/wiki/Shader
	// http://www.opengl.org/wiki/Rendering_Pipeline_Overview

	static const std::map <std::string, GLenum> shaderTypes {
		std::make_pair ("VERTEX",          GL_VERTEX_SHADER),
		std::make_pair ("TESS_CONTROL",    GL_TESS_CONTROL_SHADER),
		std::make_pair ("TESS_EVALUATION", GL_TESS_EVALUATION_SHADER),
		std::make_pair ("GEOMETRY",        GL_GEOMETRY_SHADER),
		std::make_pair ("FRAGMENT",        GL_FRAGMENT_SHADER)

		#ifdef GL_COMPUTE_SHADER
		// Requires GL 4.3 or ARB_compute_shader

		, std::make_pair ("COMPUTE", GL_COMPUTE_SHADER)

		#endif
	};

	try
	{
		return shaderTypes .at (type);
	}
	catch (const std::out_of_range &)
	{
		return GL_VERTEX_SHADER;
	}
}

GLint
getProgramStageBit (const std::string & type)
{
	// http://www.opengl.org/wiki/Rendering_Pipeline_Overview

	static const std::map <std::string, GLenum> programStageBits = {
		std::make_pair ("VERTEX",          GL_VERTEX_SHADER_BIT),
		std::make_pair ("TESS_CONTROL",    GL_TESS_CONTROL_SHADER_BIT),
		std::make_pair ("TESS_EVALUATION", GL_TESS_EVALUATION_SHADER_BIT),
		std::make_pair ("GEOMETRY",        GL_GEOMETRY_SHADER_BIT),
		std::make_pair ("FRAGMENT",        GL_FRAGMENT_SHADER_BIT)

		#ifdef GL_COMPUTE_SHADER_BIT
		// Requires GL 4.3 or ARB_compute_shader

		, std::make_pair ("COMPUTE", GL_COMPUTE_SHADER_BIT),

		#endif
	};

	try
	{
		return programStageBits .at (type);
	}
	catch (const std::out_of_range &)
	{
		return GL_VERTEX_SHADER_BIT;
	}
}

std::string
addConstants (X3DBrowser* const browser, const std::string & source)
{
	static const std::regex version (R"/(^([\s\S]*?[\n]{0,1})(\s*#version\s+\d+.*?\n))/");

	std::smatch match;

	if (not std::regex_search (source, match, version))
		return source;

	const auto begin    = match .str (0);
	const auto numLines = std::count (begin .begin (), begin .end (), '\n');

	std::string constants = "$1$2";

	constants += "#define x3d_GeometryPoints  0\n";
	constants += "#define x3d_GeometryLines   1\n";
	constants += "#define x3d_Geometry2D      2\n";
	constants += "#define x3d_Geometry3D      3\n";

	constants += "#define x3d_MaxClipPlanes  " + basic::to_string (browser -> getMaxClipPlanes (), std::locale::classic ()) + "\n";
	constants += "#define x3d_NoneClipPlane  vec4 (88.0, 51.0, 68.0, 33.0)\n"; // X3D!

	constants += "#define x3d_NoneFog          0\n";
	constants += "#define x3d_LinearFog        1\n";
	constants += "#define x3d_ExponentialFog   2\n";
	constants += "#define x3d_Exponential2Fog  3\n";

	constants += "#define x3d_MaxLights         " + basic::to_string (browser -> getMaxLights (), std::locale::classic ()) + "\n";
	constants += "#define x3d_NoneLight         0\n";
	constants += "#define x3d_DirectionalLight  1\n";
	constants += "#define x3d_PointLight        2\n";
	constants += "#define x3d_SpotLight         3\n";

	constants += "#define x3d_MaxTextures                " + basic::to_string (browser -> getMaxTextures (), std::locale::classic ()) + "\n";
	constants += "#define x3d_NoneTexture                0\n";
	constants += "#define x3d_TextureType2D              2\n";
	constants += "#define x3d_TextureType3D              3\n";
	constants += "#define x3d_TextureTypeCubeMapTexture  4\n";

	constants += "#define x3d_ShadowSamples  8\n";

	constants += "#line " + basic::to_string (numLines, std::locale::classic ()) + "\n";

	return std::regex_replace (source, version, constants);
}

std::string
getShaderSource (X3DBaseNode* const node, const std::string & string, const basic::uri & worldURL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	static const std::regex version (R"/(^(?:\s*|/\*.*?\*/|//.*?\n)*#version\s+\d+)/");

	std::set <basic::uri> files;

	const auto source = getShaderSource (node, string, worldURL, 0, files);

	if (std::regex_search (source, version))
		return addConstants (node -> getBrowser (), source);

	return addConstants (node -> getBrowser (), "#version 100\n#line 0\n" + source);
}

void
printShaderInfoLog (X3DBrowser* const browser, const std::string & typeName, const std::string & name, const std::string & type, const GLint shaderId)
{
	if (not shaderId)
		return;

	GLint infoLogLength;

	glGetShaderiv (shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1)
	{
		char infoLog [infoLogLength];

		glGetShaderInfoLog (shaderId, infoLogLength, 0, infoLog);

		browser -> print (std::string (80, '#'), '\n',
		                  typeName, name .empty () ? "" : " '" + name + "'" ," InfoLog (", type, "):\n",
		                  std::string (infoLog),
		                  std::string (80, '#'), '\n');
	}
}

void
printProgramInfoLog (X3DBrowser* const browser, const std::string & typeName, const std::string & name, const GLint programId)
{
	if (not programId)
		return;

	GLint infoLogLength;

	glGetProgramiv (programId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1)
	{
		char infoLog [infoLogLength];

		glGetProgramInfoLog (programId, infoLogLength, 0, infoLog);

		browser -> print (std::string (80, '#'), '\n',
		                  typeName, name .empty () ? "" : " '" + name + "'" , " Info Log:\n",
		                  std::string (infoLog),
		                  std::string (80, '#'), '\n');
	}
}

} // Shader
} // X3D
} // titania

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

#include "../../Browser/BrowserOptions.h"
#include "../../Browser/Networking/config.h"
#include "../../InputOutput/FileLoader.h"

#include <Titania/String/to_string.h>
#include <regex>

namespace titania {
namespace X3D {

bool
Shader::getOpenGLES (const std::string & source)
{
	// OpenGL ES1 shaders use '#version 100' directive.
	// OpenGL ES2 shaders use '#version 100' directive, there is no '#version 200'.
	// OpenGL ES3 shaders use '#version 300 es' directive.

	static const std::regex version_es (R"/(^(\s+|/\*.*?\*/|//.*?\n)*(#version\s+100|#version\s+300\s+es)\s*?\n)/");

	return std::regex_search (source, version_es);
}

GLenum
Shader::getShaderType (const std::string & type)
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
Shader::getProgramStageBit (const std::string & type)
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

ShaderSource
Shader::getSource (X3DBaseNode* const node, const std::string & string, const basic::uri & worldURL, const bool shadow)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	ShaderSource          source;
	std::set <basic::uri> files;

	source .string = getSource (node, string, worldURL, source .uris, 0, files);
	source .string = addDefinitions (node -> getBrowser (), source .string, shadow);

	return source;
}

std::string
Shader::getSource (X3DBaseNode* const node,
                   const std::string & source,
                   const basic::uri & worldURL,
                   std::vector <basic::uri> & uris,
                   const size_t level,
                   std::set <basic::uri> & files)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	static const std::regex include (R"/(^#pragma\s+X3D\s+include\s+\"(.*?)\"\s*$)/");

	if (not files .insert (worldURL) .second)
		return "";

	if (level > 1024)
		throw Error <INVALID_URL> ("Header inclusion depth limit reached, might be caused by cyclic header inclusion.");

	std::istringstream input (source);
	std::ostringstream output;

	input  .imbue (std::locale::classic ());
	output .imbue (std::locale::classic ());

	size_t      fileNumber = uris .size ();
	size_t      lineNumber = 1;
	std::string line;

	uris .emplace_back (worldURL);

	if (level)
		output << "#line "<< lineNumber << " " << fileNumber << std::endl;

	while (std::getline (input, line))
	{
		std::smatch filename;

		if (std::regex_match (line, filename, include))
		{
			FileLoader loader (node -> getExecutionContext ());

			output << getSource (node, loader .loadDocument (worldURL .transform (filename .str (1))), loader .getWorldURL (), uris, level + 1, files) << std::endl;

			output << "#line "<< lineNumber + 1 << " " << fileNumber << std::endl;
		}
		else
		{
			output << line << std::endl;
		}

		++ lineNumber;
	}

	return output .str ();
}

std::string
Shader::addDefinitions (X3DBrowser* const browser, std::string source, const bool shadow)
{
	#define COMMENTS     "\\s+|/\\*[\\s\\S]*?\\*/|//.*?\\n"
	#define LINE         "#line\\s+.*?\\n"
	#define IF           "#if\\s+.*?\\n"
	#define ELIF         "#elif\\s+.*?\\n"
	#define IFDEF        "#ifdef\\s+.*?\\n"
	#define IFNDEF       "#ifndef\\s+.*?\\n"
	#define ELSE         "#else.*?\\n"
	#define ENDIF        "#endif.*?\\n"
	#define DEFINE       "#define\\s+(?:[^\\n\\\\]|\\\\[^\\r\\n]|\\\\\\r?\\n)*\\n"
	#define UNDEF        "#undef\\s+.*?\\n"
	#define PRAGMA       "#pragma\\s+.*?\\n"
	#define PREPROCESSOR  LINE "|" IF "|" ELIF "|" IFDEF "|" IFNDEF "|" ELSE "|" ENDIF "|" DEFINE "|" UNDEF "|" PRAGMA
	#define VERSION      "#version\\s+.*?\\n"
	#define EXTENSION    "#extension\\s+.*?\\n"
	#define ANY          "[\\s\\S]*"

	static const std::regex head ("^((?:" COMMENTS "|" PREPROCESSOR "|" EXTENSION ")*)(" ANY ")$");

	std::smatch hmatch;

	if (not std::regex_search (source, hmatch, head))
		return source;

	const auto begin    = hmatch .str (1);
	const auto numLines = std::count (begin .begin (), begin .end (), '\n');

	std::ostringstream constants;
	std::ostringstream definitions;
	std::ifstream types (basic::uri (get_data ("shaders/Shaders/Types.h")) .path ());

	definitions .imbue (std::locale::classic ());

	constants << "#version 100\n#line 1\n";
	constants << "#define TITANIA\n";

	if (browser -> getBrowserOptions () -> LogarithmicDepthBuffer ())
		constants << "#define X3D_LOGARITHMIC_DEPTH_BUFFER\n";

	definitions << "#define x3d_None 0\n";

	definitions << "#define x3d_GeometryPoints  0\n";
	definitions << "#define x3d_GeometryLines   1\n";
	definitions << "#define x3d_Geometry2D      2\n";
	definitions << "#define x3d_Geometry3D      3\n";

	definitions << "#define x3d_MaxClipPlanes  " << browser -> getMaxClipPlanes () << "\n";

	definitions << "#define x3d_LinearFog        1\n";
	definitions << "#define x3d_ExponentialFog   2\n";
	definitions << "#define x3d_Exponential2Fog  3\n";

	definitions << "#define x3d_MaxLights         " << browser -> getMaxLights () << "\n";
	definitions << "#define x3d_DirectionalLight  1\n";
	definitions << "#define x3d_PointLight        2\n";
	definitions << "#define x3d_SpotLight         3\n";

	definitions << "#define x3d_MaxTextures                " << browser -> getMaxTextures () << "\n";
	definitions << "#define x3d_TextureType2D              2\n";
	definitions << "#define x3d_TextureType3D              3\n";
	definitions << "#define x3d_TextureTypeCubeMapTexture  4\n";

	if (shadow)
		definitions << "#define X3D_SHADOWS\n";

	definitions << "#define x3d_MaxShadows     4\n";
	definitions << "#define X3D_PCF_FILTERING\n";

	// Legacy

	definitions << "#define x3d_NoneClipPlane  vec4 (88.0, 51.0, 68.0, 33.0)\n"; // X3D!
	definitions << "#define x3d_NoneFog        0\n";
	definitions << "#define x3d_NoneLight      0\n";
	definitions << "#define x3d_NoneTexture    0\n";

	depreciatedWarning (browser, source, "x3d_NoneClipPlane", "x3d_NumClipPlanes");
	depreciatedWarning (browser, source, "x3d_NoneFog",       "x3d_None");
	depreciatedWarning (browser, source, "x3d_NoneLight",     "x3d_NumLights");
	depreciatedWarning (browser, source, "x3d_NoneTexture",   "x3d_NumTextures");

	// Types

	definitions << types .rdbuf ();
	definitions << "\n";

	// Combine

	definitions << "#line " << (numLines - 1)  << "\n";

	return constants .str () + hmatch .str (1) + definitions .str () + hmatch .str (2);
}

void
Shader::depreciatedWarning (X3DBrowser* const browser, const std::string & source, const std::string & depreciated, const std::string & current)
{
	if (source .find (depreciated) == std::string::npos)
		return;

	browser -> getConsole () -> warn ("Use of '", depreciated, "' is depreciated, use '", current, "' instead. See http://create3000.de/x_ite/custom-shaders/.\n");
}

void
Shader::printShaderInfoLog (X3DBrowser* const browser,
                            const std::string & typeName,
                            const std::string & name,
                            const std::string & type,
                            const GLint shaderId, 
                            const std::vector <basic::uri> & sources)
{
	if (not shaderId)
		return;

	GLint infoLogLength = 0;

	glGetShaderiv (shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1)
	{
		static const std::regex numbers (R"/((\d+):(\d+)\((\d+)\))/");

		std::string infoLog (infoLogLength, '\0');

		glGetShaderInfoLog (shaderId, infoLogLength, 0, &infoLog [0]);

		if (infoLogLength)
			infoLog .resize (infoLogLength - 1);

		basic::uri filename;
	
		try
		{
			std::smatch match;

			if (std::regex_search (infoLog, match, numbers))
				filename = sources .at (atoi (match .str (1) .c_str ()));
		}
		catch (const std::out_of_range &)
		{ }

		browser -> getConsole () -> error ("\n",
		                                   std::string (80, '#'), "\n",
		                                   typeName, (name .empty () ? "" : " '" + name + "'"), " InfoLog (", type, "):", "\n",
		                                   "in file '", filename, "'\n",
		                                   infoLog,
		                                   std::string (80, '#'), "\n",
		                                   "\n");
	}
}

void
Shader::printProgramInfoLog (X3DBrowser* const browser,
                             const std::string & typeName,
                             const std::string & name,
                             const GLint programId,
                             const std::vector <basic::uri> & sources)
{
	if (not programId)
		return;

	GLint infoLogLength = 0;

	glGetProgramiv (programId, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1)
	{
		static const std::regex numbers (R"/((\d+):(\d+)\((\d+)\))/");

		std::string infoLog (infoLogLength, '\0');

		glGetProgramInfoLog (programId, infoLogLength, 0, &infoLog [0]);

		if (infoLogLength)
			infoLog .resize (infoLogLength - 1);

		basic::uri filename;

		try
		{
			std::smatch match;

			if (std::regex_search (infoLog, match, numbers))
				filename = sources .at (atoi (match .str (1) .c_str ()));
		}
		catch (const std::out_of_range &)
		{ }

		browser -> getConsole () -> error ("\n",
		                                   std::string (80, '#'), "\n",
		                                   typeName, (name .empty () ? "" : " '" + name + "'"), " Info Log:\n",
		                                   "in file '", filename, "'\n",
		                                   infoLog,
		                                   std::string (80, '#'),"\n",
		                                   "\n");
	}
}

} // X3D
} // titania

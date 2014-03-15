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

#include "ProgramShader.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ShaderProgram.h"

namespace titania {
namespace X3D {

const std::string ProgramShader::componentName  = "Shaders";
const std::string ProgramShader::typeName       = "ProgramShader";
const std::string ProgramShader::containerField = "shaders";

// http://www.opengl.org/wiki/GLSL_Object#Program_separation
// http://www.opengl.org/wiki/GLAPI/glGenProgramPipelines

ProgramShader::Fields::Fields () :
	programs (new MFNode ())
{ }

ProgramShader::ProgramShader (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DShaderNode (),
	       fields (),
	   loadSensor (new LoadSensor (executionContext)),
	   pipelineId (0)
{
	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOnly,      "activate",   activate ());
	addField (outputOnly,     "isSelected", isSelected ());
	addField (outputOnly,     "isValid",    isValid ());
	addField (initializeOnly, "language",   language ());
	addField (inputOutput,    "programs",   programs ());
	
	addChildren (loadSensor);
}

X3DBaseNode*
ProgramShader::create (X3DExecutionContext* const executionContext) const
{
	return new ProgramShader (executionContext);
}

void
ProgramShader::initialize ()
{
	X3DShaderNode::initialize ();

	activate () .addInterest (this, &ProgramShader::set_activate);
	programs () .addInterest (loadSensor -> watchList ());

	loadSensor -> isInternal (true);
	loadSensor -> watchList () = programs ();
	loadSensor -> isActive () .addInterest (this, &ProgramShader::requestExplicitRelink);
	loadSensor -> setup ();
}

GLint
ProgramShader::getProgramStageBit (const String & type)
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

	const auto programStageBit = programStageBits .find (type);

	if (programStageBit not_eq programStageBits .end ())
		return programStageBit -> second;

	return GL_VERTEX_SHADER_BIT;
}

void
ProgramShader::requestExplicitRelink ()
{
	if (loadSensor -> isActive ())
		return;

	if (not getBrowser () -> getRenderingProperties () -> hasExtension ("GL_ARB_separate_shader_objects"))
	{
		isValid () = false;
		return;
	}

	if (pipelineId)
	{
		glDeleteProgramPipelines (1, &pipelineId);
		pipelineId = 0;
	}

	if (loadSensor -> progress () == 1.0f and (language () == "" or language () == "GLSL"))
	{
		glGenProgramPipelines (1, &pipelineId);

		glBindProgramPipeline (pipelineId);

		for (const auto & program : programs ())
		{
			const auto programNode = x3d_cast <ShaderProgram*> (program);

			if (programNode)
				glUseProgramStages (pipelineId, getProgramStageBit (programNode -> type ()), programNode -> getProgramId ());
		}

		isValid () = true;

		glBindProgramPipeline (0);
	}
	else
		isValid () = false;
	
	// Propagate event further.

	addEvent ();
}

void
ProgramShader::set_activate ()
{
	if (activate ())
		requestExplicitRelink ();
}

void
ProgramShader::enable ()
{
	glBindProgramPipeline (pipelineId);
}

void
ProgramShader::disable ()
{
	glBindProgramPipeline (0);
}

void
ProgramShader::draw ()
{
	X3DShaderNode::draw ();

	glBindProgramPipeline (pipelineId);
}

void
ProgramShader::dispose ()
{
	if (pipelineId)
		glDeleteProgramPipelines (1, &pipelineId);

	X3DShaderNode::dispose ();
}

} // X3D
} // titania

/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "../../Browser/Core/Cast.h"
#include "../../Browser/ContextLock.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ShaderProgram.h"

namespace titania {
namespace X3D {

const ComponentType ProgramShader::component      = ComponentType::SHADERS;
const std::string   ProgramShader::typeName       = "ProgramShader";
const std::string   ProgramShader::containerField = "shaders";

// http://www.opengl.org/wiki/GLSL_Object#Program_separation
// http://www.opengl.org/wiki/GLAPI/glGenProgramPipelines

ProgramShader::Fields::Fields () :
	programs (new MFNode ())
{ }

ProgramShader::ProgramShader (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DShaderNode (),
	       fields (),
	 programNodes (),
	   loadSensor (new LoadSensor (executionContext)),
	   pipelineId (0)
{
	addType (X3DConstants::ProgramShader);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOnly,      "activate",   activate ());
	addField (outputOnly,     "isSelected", isSelected ());
	addField (outputOnly,     "isValid",    isValid ());
	addField (initializeOnly, "language",   language ());
	addField (inputOutput,    "programs",   programs ());

	addChildren (programNodes, loadSensor);
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
	programs () .addInterest (this, &ProgramShader::set_programs);
	programs () .addInterest (loadSensor -> watchList ());

	loadSensor -> isPrivate (true);
	loadSensor -> watchList () = programs ();
	loadSensor -> isActive () .addInterest (this, &ProgramShader::requestExplicitRelink);
	loadSensor -> setup ();

	set_programs ();
}

void
ProgramShader::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	loadSensor -> setExecutionContext (executionContext);

	X3DShaderNode::setExecutionContext (executionContext);
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

	try
	{
		return programStageBits .at (type);
	}
	catch (const std::out_of_range &)
	{
		return GL_VERTEX_SHADER_BIT;
	}
}

void
ProgramShader::requestExplicitRelink ()
{
	if (loadSensor -> isActive ())
		return;

	if (not getBrowser () -> isExtensionAvailable ("GL_ARB_separate_shader_objects"))
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

		for (const auto & node : programs ())
		{
			const auto programNode = x3d_cast <ShaderProgram*> (node);

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
ProgramShader::set_programs ()
{
	programNodes .clear ();

	for (const auto & node : programs ())
	{
		const auto programNode = x3d_cast <ShaderProgram*> (node);

		if (programNode)
			programNodes .emplace_back (programNode);
	}
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
ProgramShader::setGeometryType (const size_t value)
{
	for (const auto & programNode : programNodes)
	{
		if (pipelineId and programNode -> getProgramId ())
			glActiveShaderProgram (pipelineId, programNode -> getProgramId ());

		programNode -> setGeometryType (value);
	}
}

size_t
ProgramShader::getGeometryType () const
{
	for (const auto & programNode : programNodes)
		return programNode -> getGeometryType ();

	return 3;
}

void
ProgramShader::setGlobalUniforms (ShapeContainer* const context)
{
	for (const auto & programNode : programNodes)
	{
		glActiveShaderProgram (pipelineId, programNode -> getProgramId ());
		programNode -> setGlobalUniforms (context);
	}
}

void
ProgramShader::setLocalUniforms (ShapeContainer* const context)
{
	for (const auto & programNode : programNodes)
	{
		glActiveShaderProgram (pipelineId, programNode -> getProgramId ());
		programNode -> setLocalUniforms (context);
	}
}

void
ProgramShader::enableColorAttrib (const GLuint buffer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableColorAttrib (buffer);
}

void
ProgramShader::disableColorAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableColorAttrib ();
}

void
ProgramShader::enableTexCoordAttrib (const std::vector <GLuint> & buffers)
{
	for (const auto & programNode : programNodes)
		programNode -> enableTexCoordAttrib (buffers);
}

void
ProgramShader::disableTexCoordAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableTexCoordAttrib ();
}

void
ProgramShader::enableNormalAttrib (const GLuint buffer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableNormalAttrib (buffer);
}

void
ProgramShader::disableNormalAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableNormalAttrib ();
}

void
ProgramShader::enableVertexAttrib (const GLuint buffer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableVertexAttrib (buffer);
}

void
ProgramShader::disableVertexAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableVertexAttrib ();
}

void
ProgramShader::enableFloatAttrib (const std::string & name, const GLuint buffer, const size_t components)
{
	for (const auto & programNode : programNodes)
		programNode -> enableFloatAttrib (name, buffer, components);
}

void
ProgramShader::disableFloatAttrib (const std::string & name)
{
	for (const auto & programNode : programNodes)
		programNode -> disableFloatAttrib (name);
}

void
ProgramShader::enableMatrix3Attrib (const std::string & name, const GLuint buffer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableMatrix3Attrib (name, buffer);
}

void
ProgramShader::disableMatrix3Attrib (const std::string & name)
{
	for (const auto & programNode : programNodes)
		programNode -> disableMatrix3Attrib (name);
}

void
ProgramShader::enableMatrix4Attrib (const std::string & name, const GLuint buffer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableMatrix4Attrib (name, buffer);
}

void
ProgramShader::disableMatrix4Attrib (const std::string & name)
{
	for (const auto & programNode : programNodes)
		programNode -> disableMatrix4Attrib (name);
}

void
ProgramShader::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		if (pipelineId)
			glDeleteProgramPipelines (1, &pipelineId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	X3DShaderNode::dispose ();
}

} // X3D
} // titania

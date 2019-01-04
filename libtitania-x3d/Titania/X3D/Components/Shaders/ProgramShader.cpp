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

#include "ProgramShader.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Shaders/Shader.h"
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

	addChildObjects (programNodes, loadSensor);
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

	activate () .addInterest (&ProgramShader::set_activate, this);
	programs () .addInterest (&ProgramShader::set_programs, this);
	programs () .addInterest (loadSensor -> watchList ());

	loadSensor -> setPrivate (true);
	loadSensor -> watchList () = programs ();
	loadSensor -> isActive () .addInterest (&ProgramShader::requestExplicitRelink, this);
	loadSensor -> setup ();

	set_programs ();
}

void
ProgramShader::setExecutionContext (X3DExecutionContext* const executionContext)
{
	loadSensor -> setExecutionContext (executionContext);

	X3DShaderNode::setExecutionContext (executionContext);
}

void
ProgramShader::requestExplicitRelink ()
{
	if (loadSensor -> isActive ())
		return;

	if (not getBrowser () -> getExtension ("GL_ARB_separate_shader_objects"))
	{
		isValid () = false;
		return;
	}

	if (pipelineId)
	{
		glDeleteProgramPipelines (1, &pipelineId);
		pipelineId = 0;
	}

	bool valid = true;

	if (loadSensor -> progress () == 1.0f and (language () == "" or language () == "GLSL"))
	{
		glGenProgramPipelines (1, &pipelineId);

		if (pipelineId)
		{
			glBindProgramPipeline (pipelineId);
	
			for (const auto & node : programs ())
			{
				const auto programNode = x3d_cast <ShaderProgram*> (node);
	
				if (programNode)
				{
					if (programNode -> isValid ())
						glUseProgramStages (pipelineId, Shader::getProgramStageBit (programNode -> type ()), programNode -> getProgramId ());
					else
					{
						valid = false;
						break;
					}
				}
			}
		}
		else
			valid = false;

		glBindProgramPipeline (0);
	}
	else
		valid = false;
	
	if (valid != isValid ())
		isValid () = valid;

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

	for (const auto & programNode : programNodes)
		programNode -> enable ();
}

void
ProgramShader::disable ()
{
	glBindProgramPipeline (0);

	for (const auto & programNode : programNodes)
		programNode -> disable ();
}

void
ProgramShader::draw (X3DRenderObject* const renderObject)
{
	glBindProgramPipeline (pipelineId);
}

void
ProgramShader::setGlobalUniforms (X3DRenderObject* const renderObject)
{
	glBindProgramPipeline (pipelineId);

	for (const auto & programNode : programNodes)
	{
		glActiveShaderProgram (pipelineId, programNode -> getProgramId ());
		programNode -> setGlobalUniforms (renderObject);
	}
}

void
ProgramShader::setLocalUniforms (ShapeContainer* const context)
{
	glBindProgramPipeline (pipelineId);

	for (const auto & programNode : programNodes)
	{
		glActiveShaderProgram (pipelineId, programNode -> getProgramId ());
		programNode -> setLocalUniforms (context);
	}
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
ProgramShader::enableIntegerAttrib (const std::string & name, const GLuint buffer, const size_t components)
{
	for (const auto & programNode : programNodes)
		programNode -> enableIntegerAttrib (name, buffer, components);
}

void
ProgramShader::disableIntegerAttrib (const std::string & name)
{
	for (const auto & programNode : programNodes)
		programNode -> disableIntegerAttrib (name);
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
ProgramShader::enableColorAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableColorAttrib (buffer, type, stride, pointer);
}

void
ProgramShader::disableColorAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableColorAttrib ();
}

void
ProgramShader::enableTexCoordAttrib (const std::vector <GLuint> & buffer,
                                     const GLenum type,
                                     const std::vector <GLsizei> & stride,
                                     const std::vector <GLvoid*> & pointer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableTexCoordAttrib (buffer, type, stride, pointer);
}

void
ProgramShader::disableTexCoordAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableTexCoordAttrib ();
}

void
ProgramShader::enableNormalAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableNormalAttrib (buffer, type, stride, pointer);
}

void
ProgramShader::disableNormalAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableNormalAttrib ();
}

void
ProgramShader::enableVertexAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer)
{
	for (const auto & programNode : programNodes)
		programNode -> enableVertexAttrib (buffer, type, stride, pointer);
}

void
ProgramShader::disableVertexAttrib ()
{
	for (const auto & programNode : programNodes)
		programNode -> disableVertexAttrib ();
}

void
ProgramShader::setParticle (const size_t id, const SoftParticle & particle, const Matrix4d & modelViewMatrix)
{
	for (const auto & programNode : programNodes)
		programNode -> setParticle (id, particle, modelViewMatrix);
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

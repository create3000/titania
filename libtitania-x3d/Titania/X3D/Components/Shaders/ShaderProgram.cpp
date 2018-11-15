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

#include "ShaderProgram.h"

#include "../../Browser/Shaders/Shader.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../InputOutput/FileLoader.h"

namespace titania {
namespace X3D {

const ComponentType ShaderProgram::component      = ComponentType::SHADERS;
const std::string   ShaderProgram::typeName       = "ShaderProgram";
const std::string   ShaderProgram::containerField = "programs";

// http://www.opengl.org/wiki/GLAPI/glCreateShaderProgramv

ShaderProgram::Fields::Fields () :
	type (new SFString ("VERTEX"))
{ }

ShaderProgram::ShaderProgram (X3DExecutionContext* const executionContext) :
	                X3DBaseNode (executionContext -> getBrowser (), executionContext),
	                    X3DNode (),
	               X3DUrlObject (),
	X3DProgrammableShaderObject (),
	                     fields (),
	                     buffer (),
	                  programId (0),
	                      valid (false)
{
	addType (X3DConstants::ShaderProgram);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "type",     type ());
	addField (inputOutput,    "url",      url ());

	addChildObjects (buffer);
}

X3DBaseNode*
ShaderProgram::create (X3DExecutionContext* const executionContext) const
{
	return new ShaderProgram (executionContext);
}

void
ShaderProgram::initialize ()
{
	X3DNode::initialize ();
	X3DUrlObject::initialize ();
	X3DProgrammableShaderObject::initialize ();

	type () .addInterest (&ShaderProgram::set_url, this);
	url ()  .addInterest (&ShaderProgram::set_url, this);

	buffer .addInterest (&ShaderProgram::set_buffer, this);

	set_url ();
}

void
ShaderProgram::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DUrlObject::setExecutionContext (executionContext);
	X3DNode::setExecutionContext (executionContext);
}

///  throws Error <INVALID_NAME>, Error <INVALID_FIELD>, Error <DISPOSED>
void
ShaderProgram::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
{
	X3DProgrammableShaderObject::addUserDefinedField (accessType, name, field);

	if (isInitialized ())
		url () .addEvent ();
}

///  throws Error <DISPOSED>
void
ShaderProgram::removeUserDefinedField (const std::string & name)
{
	X3DProgrammableShaderObject::removeUserDefinedField (name);

	if (isInitialized ())
		url () .addEvent ();
}

void
ShaderProgram::requestImmediateLoad ()
{
	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	if (not getBrowser () -> getExtension ("GL_ARB_separate_shader_objects"))
	{
		setLoadState (FAILED_STATE);
		return;
	}

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	valid = false;

	for (const auto & URL : basic::make_const_range (url ()))
	{
		try
		{
			// Create shader program

			FileLoader loader (getExecutionContext ());

			const auto document = loader .loadDocument (URL .raw ());
			const auto source   = Shader::getSource (this, document, loader .getWorldURL ());
			const auto string   = source .string .c_str ();

			setOpenGLES (Shader::getOpenGLES (source .string));

			if (programId)
				glDeleteProgram (programId);

			programId = glCreateProgram ();

			if (programId)
			{
				// Attach shader
	
				const auto shaderId = glCreateShader (Shader::getShaderType (type ()));
	
				if (shaderId)
				{
					glShaderSource  (shaderId, 1, &string, nullptr);
					glCompileShader (shaderId);
					glGetShaderiv   (shaderId, GL_COMPILE_STATUS, &valid);

					Shader::printShaderInfoLog (getBrowser (), getTypeName (), getName (), type (), shaderId, source .uris);

					if (valid)
					{
						glAttachShader (programId, shaderId);
			
						// x3d_FragColor
	
						if (Shader::getShaderType (type ()) == GL_FRAGMENT_SHADER)
							glBindFragDataLocation (programId, 0, "x3d_FragColor");
			
						// Link program
				
						glProgramParameteri (programId, GL_PROGRAM_SEPARABLE, true);
						glLinkProgram  (programId);
						glDetachShader (programId, shaderId);
		
						// Check for link status
				
						glGetProgramiv (programId, GL_LINK_STATUS, &valid);
					}
		
					// Print info log
		
					Shader::printProgramInfoLog (getBrowser (), getTypeName (), getName (), programId, source .uris);
		
					// Initialize uniform variables
					glUseProgram (programId);
					getDefaultUniforms ();
					addShaderFields ();
					glUseProgram (0);
	
					glDeleteShader (shaderId);

					if (valid)
						break;
				}
			}
		}
		catch (const X3DError & error)
		{
			#ifdef TITANIA_DEBUG
			std::clog << error .what () << std::endl;
			#endif
		}
	}

	setLoadState (valid ? COMPLETE_STATE : FAILED_STATE);
}

void
ShaderProgram::enable ()
{
	X3DProgrammableShaderObject::enable ();
}

void
ShaderProgram::disable ()
{
	X3DProgrammableShaderObject::disable ();
}

void
ShaderProgram::set_url ()
{
	buffer .addEvent ();
}

void
ShaderProgram::set_buffer ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

void
ShaderProgram::set_field (X3DFieldDefinition* const field)
{
	glUseProgram (programId);

	X3DProgrammableShaderObject::set_field (field);

	glUseProgram (0);
}

void
ShaderProgram::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		if (programId)
			glDeleteProgram (programId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	X3DProgrammableShaderObject::dispose ();
	X3DUrlObject::dispose ();
	X3DNode::dispose ();
}

} // X3D
} // titania

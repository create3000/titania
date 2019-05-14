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

#include "ComposedShader.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Shaders/Shader.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ShaderPart.h"

namespace titania {
namespace X3D {

const Component   ComposedShader::component      = Component ("Shaders", 1);
const std::string ComposedShader::typeName       = "ComposedShader";
const std::string ComposedShader::containerField = "shaders";

ComposedShader::Fields::Fields () :
	parts (new MFNode ())
{ }

ComposedShader::ComposedShader (X3DExecutionContext* const executionContext) :
	                X3DBaseNode (executionContext -> getBrowser (), executionContext),
	              X3DShaderNode (),
	X3DProgrammableShaderObject (),
	                     fields (),
	                 loadSensor (new LoadSensor (executionContext)),
	                  programId (0)
{
	addType (X3DConstants::ComposedShader);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOnly,      "activate",   activate ());
	addField (outputOnly,     "isSelected", isSelected ());
	addField (outputOnly,     "isValid",    isValid ());
	addField (initializeOnly, "language",   language ());
	addField (inputOutput,    "parts",      parts ());

	addChildObjects (loadSensor);
}

X3DBaseNode*
ComposedShader::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedShader (executionContext);
}

void
ComposedShader::initialize ()
{
	X3DShaderNode::initialize ();
	X3DProgrammableShaderObject::initialize ();

	activate () .addInterest (&ComposedShader::set_activate, this);
	parts ()    .addInterest (loadSensor -> watchList ());

	loadSensor -> setPrivate (true);
	loadSensor -> watchList () = parts ();
	loadSensor -> isLoaded () .addInterest (&ComposedShader::set_loaded, this);
	loadSensor -> setup ();
}

void
ComposedShader::setExecutionContext (X3DExecutionContext* const executionContext)
{
	loadSensor -> setExecutionContext (executionContext);

	X3DShaderNode::setExecutionContext (executionContext);
}

///  throws Error <INVALID_NAME>, Error <INVALID_FIELD>, Error <DISPOSED>
void
ComposedShader::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
{
	X3DProgrammableShaderObject::addUserDefinedField (accessType, name, field);
}

///  throws Error <DISPOSED>
void
ComposedShader::removeUserDefinedField (const std::string & name)
{
	X3DProgrammableShaderObject::removeUserDefinedField (name);
}

void
ComposedShader::set_activate ()
{
	if (activate ())
		set_loaded ();
}

void
ComposedShader::set_loaded ()
{
	if (loadSensor -> isLoaded ())
	{
		if (programId)
			glDeleteProgram (programId);

		programId = glCreateProgram ();

		// Attach shader

		GLint valid = true;

		if (programId)
		{
			bool openGLES = false;

			for (const auto & part : parts ())
			{
				const auto partNode = x3d_cast <ShaderPart*> (part);

				if (partNode)
				{
					if (partNode -> isValid ())
					{
						openGLES |= partNode -> getOpenGLES ();

						glAttachShader (programId, partNode -> getShaderId ());
					}
					else
					{
						valid = false;
						break;
					}
				}
			}

			setOpenGLES (openGLES);
		}
		else
		{
			valid = false;
		}

		if (valid)
		{
			// x3d_FragColor

			glBindFragDataLocation (programId, 0, "x3d_FragColor");

			// TransformFeedbackVaryings

			applyTransformFeedbackVaryings ();

			// Link program

			glLinkProgram (programId);

			// Check for link status

			glGetProgramiv (programId, GL_LINK_STATUS, &valid);
		}

		if (valid)
		{
			// Initialize uniform variables

			glUseProgram (programId);
			getDefaultUniforms ();
			addShaderFields ();
			glUseProgram (0);
		}

		// Print info log

		Shader::printProgramInfoLog (getBrowser (), getTypeName (), getName (), programId, { });

		isValid () = valid;

		// Detach shaders

		if (programId)
		{
			for (const auto & part : parts ())
			{
				const auto partNode = x3d_cast <ShaderPart*> (part);

				if (partNode and partNode -> isValid ())
					glDetachShader (programId, partNode -> getShaderId ());
			}
		}
	}
	else
	{
		isValid () = false;
	}
}

void
ComposedShader::set_field (X3DFieldDefinition* const field)
{
	glUseProgram (programId);

	X3DProgrammableShaderObject::set_field (field);

	glUseProgram (0);
}

void
ComposedShader::enable ()
{
	glUseProgram (programId);

	X3DProgrammableShaderObject::enable ();
}

void
ComposedShader::disable ()
{
	X3DProgrammableShaderObject::disable ();

	glUseProgram (0);
}

void
ComposedShader::setGlobalUniforms (X3DRenderObject* const renderObject)
{
	glUseProgram (programId);
	X3DProgrammableShaderObject::setGlobalUniforms (renderObject);
}

void
ComposedShader::setLocalUniforms (ShapeContainer* const context)
{
	X3DProgrammableShaderObject::setLocalUniforms (context);
}

void
ComposedShader::dispose ()
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
	X3DShaderNode::dispose ();
}

} // X3D
} // titania

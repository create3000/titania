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

#include "ComposedShader.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ShaderPart.h"

namespace titania {
namespace X3D {

const std::string ComposedShader::componentName  = "Shaders";
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
	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOnly,      "activate",   activate ());
	addField (outputOnly,     "isSelected", isSelected ());
	addField (outputOnly,     "isValid",    isValid ());
	addField (initializeOnly, "language",   language ());
	addField (inputOutput,    "parts",      parts ());
	
	addChildren (loadSensor);
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

	activate () .addInterest (this, &ComposedShader::set_activate);
	parts ()    .addInterest (loadSensor -> watchList ());

	loadSensor -> isInternal (true);
	loadSensor -> watchList () = parts ();
	loadSensor -> isActive () .addInterest (this, &ComposedShader::requestExplicitRelink);
	loadSensor -> setup ();
}

void
ComposedShader::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
{
	X3DShaderNode::addUserDefinedField (accessType, name, field);

	if (isInitialized ())
		loadSensor -> isActive () .addEvent ();
}

void
ComposedShader::removeUserDefinedField (X3DFieldDefinition* const field)
{
	X3DShaderNode::removeUserDefinedField (field);

	if (isInitialized ())
		loadSensor -> isActive () .addEvent ();
}

void
ComposedShader::requestExplicitRelink ()
{
	if (loadSensor -> isActive ())
		return;

	if (loadSensor -> progress () == 1.0f and (language () == "" or language () == "GLSL"))
	{
		if (programId)
			glDeleteProgram (programId);

		programId = glCreateProgram ();

		// Attach shader

		for (const auto & part : parts ())
		{
			const auto partNode = x3d_cast <ShaderPart*> (part);

			if (partNode)
				glAttachShader (programId, partNode -> getShaderId ());
		}

		// TransformFeedbackVaryings

		applyTransformFeedbackVaryings ();

		// Link program

		glLinkProgram (programId);

		// Check for link status

		GLint linkStatus;

		glGetProgramiv (programId, GL_LINK_STATUS, &linkStatus);

		isValid () = linkStatus;

		// Initialize uniform variables

		if (isValid ())
			setFields ();

		// Print info log

		printProgramInfoLog ();

		// Detach shader

		// Detaching and deleting is not necessary and could, if the program is setup manually, simply be left out.
		// http://www.opengl.org/wiki/GLSL_Object#Program_separation

		for (const auto & part : parts ())
		{
			const auto partNode = x3d_cast <ShaderPart*> (part);

			if (partNode)
				glDetachShader (programId, partNode -> getShaderId ());
		}
	}
	else
		isValid () = false;
	
	// Propagate event further.

	addEvent ();
}

void
ComposedShader::printProgramInfoLog () const
{
	if (not isValid () or 1)
	{
		GLint infoLogLength;

		glGetProgramiv (programId, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 1)
		{
			char infoLog [infoLogLength];

			glGetProgramInfoLog (programId, infoLogLength, 0, infoLog);

			getBrowser () -> print (std::string (80, '#'), '\n',
			                        "ComposedShader InfoLog:\n",
			                        std::string (infoLog),
			                        std::string (80, '#'), '\n');
		}
	}
}

void
ComposedShader::set_activate ()
{
	if (activate ())
		requestExplicitRelink ();
}

void
ComposedShader::enable ()
{
	glUseProgram (programId);
}

void
ComposedShader::disable ()
{
	glUseProgram (0);
}

void
ComposedShader::draw ()
{
	X3DShaderNode::draw ();

	glUseProgram (programId);
}

void
ComposedShader::dispose ()
{
	if (programId)
		glDeleteProgram (programId);

	X3DProgrammableShaderObject::dispose ();
	X3DShaderNode::dispose ();
}

} // X3D
} // titania

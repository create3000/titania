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
#include "../CubeMapTexturing/X3DEnvironmentTextureNode.h"
#include "../Shaders/ShaderPart.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

ComposedShader::Fields::Fields () :
	parts (new MFNode ())
{ }

ComposedShader::ComposedShader (X3DExecutionContext* const executionContext) :
	                X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	              X3DShaderNode (),                                                    
	X3DProgrammableShaderObject (),                                                    
	                     fields (),                                                    
	              shaderProgram (0),                                                   
	                shaderParts (),                                                    
	               textureUnits ()                                                     
{
	setComponent ("Shaders");
	setTypeName ("ComposedShader");

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOnly,      "activate",   activate ());
	addField (outputOnly,     "isSelected", isSelected ());
	addField (outputOnly,     "isValid",    isValid ());
	addField (initializeOnly, "language",   language ());
	addField (inputOutput,    "parts",      parts ());
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

	parts () .addInterest (this, &ComposedShader::set_parts);

	if (glXGetCurrentContext ())
	{
		shaderProgram = glCreateProgram ();

		set_parts ();

		for (const auto & field : getUserDefinedFields ())
		{
			field -> addInterest (this, &ComposedShader::set_field, field);
			set_field (field);
		}
	}
}

void
ComposedShader::set_parts ()
{
	remove_parts ();

	if (language () == "GLSL")
	{
		for (const auto & part : parts ())
		{
			auto _part = x3d_cast <ShaderPart*> (part .getValue ());

			glAttachShader (shaderProgram, _part -> getShaderId ());

			shaderParts .emplace_back (_part -> getShaderId ());
		}
	}

	glLinkProgram (shaderProgram);
}

void
ComposedShader::remove_parts ()
{
	for (const auto & shaderPart : shaderParts)
		glDetachShader (shaderProgram, shaderPart);

	shaderParts .clear ();
}

void
ComposedShader::set_field (X3DFieldDefinition* field)
{
	glUseProgram (shaderProgram);

	GLint location = glGetUniformLocation (shaderProgram, field -> getName () [0] .c_str ());

	if (location not_eq - 1)
	{
		switch (field -> getType ())
		{
			case X3DConstants::SFBool:
			{
				glUniform1i (location, static_cast <SFBool*> (field) -> getValue ());
				break;
			}
			case X3DConstants::SFColor:
			{
				float r, g, b;
				static_cast <SFColor*> (field) -> getValue (r, g, b);
				glUniform3f (location, r, g, b);
				break;
			}
			case X3DConstants::SFColorRGBA:
			{
				float r, g, b, a;
				static_cast <SFColorRGBA*> (field) -> getValue (r, g, b, a);
				glUniform4f (location, r, g, b, a);
				break;
			}
			case X3DConstants::SFDouble:
			{
				glUniform1f (location, static_cast <SFDouble*> (field) -> getValue ());
				break;
			}
			case X3DConstants::SFFloat:
			{
				glUniform1f (location, static_cast <SFFloat*> (field) -> getValue ());
				break;
			}
			case X3DConstants::SFInt32:
			{
				glUniform1i (location, static_cast <SFInt32*> (field) -> getValue ());
				break;
			}
			case X3DConstants::SFImage:
			{
				break;
			}
			case X3DConstants::SFMatrix3d:
			{
				glUniformMatrix3fv (location, 1, false, Matrix3f (static_cast <SFMatrix3d*> (field) -> getValue ()) .data ());
				break;
			}
			case X3DConstants::SFMatrix3f:
			{
				glUniformMatrix3fv (location, 1, false, static_cast <SFMatrix3f*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFMatrix4d:
			{
				glUniformMatrix4fv (location, 1, false, Matrix4f (static_cast <SFMatrix4d*> (field) -> getValue ()) .data ());
				break;
			}
			case X3DConstants::SFMatrix4f:
			{
				glUniformMatrix4fv (location, 1, false, static_cast <SFMatrix4f*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFNode:
			{
				auto node = static_cast <SFNode <X3DBaseNode>*> (field);

				auto texture = x3d_cast <X3DTextureNode*> (node -> getValue ());

				if (texture)
				{
					GLint textureUnit = 0;
					glGetUniformiv (shaderProgram, location, &textureUnit);

					if (not textureUnit)
					{
						if (getBrowser () -> getTextureUnits () .size ())
						{
							textureUnit = getBrowser () -> getTextureUnits () .top ();
							getBrowser () -> getTextureUnits () .pop ();
							textureUnits .emplace_back (textureUnit);
						}
					}

					glActiveTexture (GL_TEXTURE0 + textureUnit);

					if (x3d_cast <X3DTexture2DNode*> (texture))
						glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());

					else if (x3d_cast <X3DEnvironmentTextureNode*> (texture))
						glBindTexture (GL_TEXTURE_CUBE_MAP, texture -> getTextureId ());

					glUniform1i (location, textureUnit);
					glActiveTexture (GL_TEXTURE0);
				}

				break;
			}
			case X3DConstants::SFRotation:
			{
				float x, y, z, angle;
				static_cast <SFRotation*> (field) -> getValue (x, y, z, angle);
				glUniform4f (location, x, y, z, angle);
				break;
			}
			case X3DConstants::SFString:
			{
				break;
			}
			case X3DConstants::SFTime:
			{
				glUniform1f (location, static_cast <SFTime*> (field) -> getValue ());
				break;
			}
			case X3DConstants::SFVec2d:
			{
				double x, y;
				static_cast <SFVec2d*> (field) -> getValue (x, y);
				glUniform2f (location, x, y);
				break;
				break;
			}
			case X3DConstants::SFVec2f:
			{
				float x, y;
				static_cast <SFVec2f*> (field) -> getValue (x, y);
				glUniform2f (location, x, y);
				break;
			}
			case X3DConstants::SFVec3d:
			{
				double x, y, z;
				static_cast <SFVec3d*> (field) -> getValue (x, y, z);
				glUniform3f (location, x, y, z);
				break;
			}
			case X3DConstants::SFVec3f:
			{
				float x, y, z;
				static_cast <SFVec3f*> (field) -> getValue (x, y, z);
				glUniform3f (location, x, y, z);
				break;
			}
			case X3DConstants::SFVec4d:
			{
				double x, y, z, w;
				static_cast <SFVec4d*> (field) -> getValue (x, y, z, w);
				glUniform4f (location, x, y, z, w);
				break;
			}
			case X3DConstants::SFVec4f:
			{
				float x, y, z, w;
				static_cast <SFVec4f*> (field) -> getValue (x, y, z, w);
				glUniform4f (location, x, y, z, w);
				break;
			}
			case X3DConstants::MFBool:
			{
				auto array = static_cast <MFBool*> (field);

				std::vector <GLint> vector (array -> begin (), array -> end ());
				glUniform1iv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFColor:
			{
				auto array = static_cast <MFColor*> (field);

				std::vector <Color3f> vector (array -> begin (), array -> end ());
				glUniform3fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFColorRGBA:
			{
				auto array = static_cast <MFColorRGBA*> (field);

				std::vector <Color4f> vector (array -> begin (), array -> end ());
				glUniform4fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFDouble:
			{
				auto array = static_cast <MFDouble*> (field);

				std::vector <GLfloat> vector (array -> begin (), array -> end ());
				glUniform1fv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFFloat:
			{
				auto array = static_cast <MFFloat*> (field);

				std::vector <GLfloat> vector (array -> begin (), array -> end ());
				glUniform1fv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFImage:
			{
				break;
			}
			case X3DConstants::MFInt32:
			{
				auto array = static_cast <MFInt32*> (field);

				std::vector <GLint> vector (array -> begin (), array -> end ());
				glUniform1iv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFMatrix3d:
			{
				auto array = static_cast <MFMatrix3d*> (field);

				std::vector <Matrix3f> vector;
				vector .reserve (array -> size ());

				for (const auto & value :* array)
					vector .emplace_back (value .getValue ());

				glUniformMatrix3fv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFMatrix3f:
			{
				auto array = static_cast <MFMatrix3f*> (field);

				std::vector <Matrix3f> vector (array -> begin (), array -> end ());
				glUniformMatrix3fv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFMatrix4d:
			{
				auto array = static_cast <MFMatrix4d*> (field);

				std::vector <Matrix4f> vector;
				vector .reserve (array -> size ());

				for (const auto & value :* array)
					vector .emplace_back (value .getValue ());

				glUniformMatrix4fv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFMatrix4f:
			{
				auto array = static_cast <MFMatrix4f*> (field);

				std::vector <Matrix4f> vector (array -> begin (), array -> end ());
				glUniformMatrix4fv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFNode:
			{
				// Push back texture units
				{
					std::deque <size_t> textureUnits;

					for (size_t i = 0; ; ++ i)
					{
						GLint textureUnit = 0;

						GLint location = glGetUniformLocation (shaderProgram, (field -> getName () [0] + "[" + std::to_string (i) + "]") .c_str ());

						if (location not_eq - 1)
						{
							glGetUniformiv (shaderProgram, location, &textureUnit);

							if (textureUnit)
								textureUnits .emplace_back (textureUnit);
						}
						else
							break;
					}

					for (const auto & textureUnit : textureUnits)
						getBrowser () -> getTextureUnits () .push (textureUnit);
				}

				// Set uniform variable;

				auto array = static_cast <MFNode*> (field);

				std::vector <GLint> vector;
				vector .reserve (array -> size ());

				for (const auto & node :* array)
				{
					auto texture = x3d_cast <X3DTextureNode*> (node .getValue ());

					if (texture)
					{
						GLint textureUnit = 0;

						if (getBrowser () -> getTextureUnits () .size ())
						{
							textureUnit = getBrowser () -> getTextureUnits () .top ();
							getBrowser () -> getTextureUnits () .pop ();
							textureUnits .emplace_back (textureUnit);
						}

						glActiveTexture (GL_TEXTURE0 + textureUnit);

						if (x3d_cast <X3DTexture2DNode*> (texture))
							glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());

						else if (x3d_cast <X3DEnvironmentTextureNode*> (texture))
							glBindTexture (GL_TEXTURE_CUBE_MAP, texture -> getTextureId ());

						vector .emplace_back (textureUnit);
					}
				}

				glUniform1iv (location, vector .size (), vector .data ());
				glActiveTexture (GL_TEXTURE0);

				break;
			}
			case X3DConstants::MFRotation:
			{
				auto array = static_cast <MFRotation*> (field);

				std::vector <Vector4f> vector;
				vector .reserve (array -> size ());

				for (const auto & value :* array)
				{
					float x, y, z, angle;
					value .getValue (x, y, z, angle);
					vector .emplace_back (x, y, z, angle);
				}

				glUniform4fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFString:
			{
				break;
			}
			case X3DConstants::MFTime:
			{
				auto array = static_cast <MFTime*> (field);

				std::vector <GLfloat> vector (array -> begin (), array -> end ());
				glUniform1fv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFVec2d:
			{
				auto array = static_cast <MFVec2d*> (field);

				std::vector <Vector2f> vector;
				vector .reserve (array -> size ());

				for (const auto & value :* array)
					vector .emplace_back (value .getValue ());

				glUniform2fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec2f:
			{
				auto array = static_cast <MFVec2f*> (field);

				std::vector <Vector2f> vector (array -> begin (), array -> end ());
				glUniform2fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec3d:
			{
				auto array = static_cast <MFVec3d*> (field);

				std::vector <Vector3f> vector;
				vector .reserve (array -> size ());

				for (const auto & value :* array)
					vector .emplace_back (value .getValue ());

				glUniform3fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec3f:
			{
				auto array = static_cast <MFVec3f*> (field);

				std::vector <Vector3f> vector (array -> begin (), array -> end ());
				glUniform3fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec4d:
			{
				auto array = static_cast <MFVec4d*> (field);

				std::vector <Vector4f> vector;
				vector .reserve (array -> size ());

				for (const auto & value :* array)
					vector .emplace_back (value .getValue ());

				glUniform4fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec4f:
			{
				auto array = static_cast <MFVec4f*> (field);

				std::vector <Vector4f> vector (array -> begin (), array -> end ());
				glUniform4fv (location, vector .size (), vector [0] .data ());
				break;
			}
		}
	}
	else
		std::clog << "Warning: Uniform variable '" << field -> getName () << "' not found." << std::endl;

	glUseProgram (0);
}

void
ComposedShader::draw ()
{
	glEnable (GL_TEXTURE_2D);
	glEnable (GL_TEXTURE_CUBE_MAP);
	glUseProgram (shaderProgram);
}

void
ComposedShader::dispose ()
{
	remove_parts ();

	if (shaderProgram)
		glDeleteProgram (shaderProgram);

	for (const auto & textureUnit : textureUnits)
		getBrowser () -> getTextureUnits () .push (textureUnit);

	X3DProgrammableShaderObject::dispose ();
	X3DShaderNode::dispose ();
}

} // X3D
} // titania

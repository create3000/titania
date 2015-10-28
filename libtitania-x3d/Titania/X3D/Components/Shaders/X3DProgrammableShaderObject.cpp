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

#include "X3DProgrammableShaderObject.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../CubeMapTexturing/X3DEnvironmentTextureNode.h"
#include "../Texturing/X3DTexture2DNode.h"
#include "../Texturing3D/X3DTexture3DNode.h"

#include <Titania/String/to_string.h>

namespace titania {
namespace X3D {

X3DProgrammableShaderObject::X3DProgrammableShaderObject () :
	              X3DBaseNode (),
	transformFeedbackVaryings (),
	             textureUnits ()
{
	addType (X3DConstants::X3DProgrammableShaderObject);
}

void
X3DProgrammableShaderObject::applyTransformFeedbackVaryings () const
{
	if (not transformFeedbackVaryings .empty ())
	{
		const size_t size = transformFeedbackVaryings .size ();

		const GLchar* varyings [size];

		for (size_t i = 0; i < size; ++ i)
			varyings [i] = transformFeedbackVaryings [i] .c_str ();

		glTransformFeedbackVaryings (getProgramId (), size, varyings, GL_INTERLEAVED_ATTRIBS);
	}
}

void
X3DProgrammableShaderObject::setFields ()
{
	for (const auto & field : getUserDefinedFields ())
	{
		field -> addInterest (this, &X3DProgrammableShaderObject::set_field, field);

		set_field (field);
	}
}

void
X3DProgrammableShaderObject::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
throw (Error <INVALID_NAME>,
       Error <INVALID_FIELD>,
       Error <DISPOSED>)
{
	X3DBaseNode::addUserDefinedField (accessType, name, field);

	if (isInitialized ())
		field -> addInterest (this, &X3DProgrammableShaderObject::set_field, field);
}

void
X3DProgrammableShaderObject::removeUserDefinedField (const std::string & name)
throw (Error <DISPOSED>)
{
	try
	{
		getField (name) -> removeInterest (this, &X3DProgrammableShaderObject::set_field);
	}
	catch (const X3DError &)
	{ }

	X3DBaseNode::removeUserDefinedField (name);
}

void
X3DProgrammableShaderObject::set_field (X3DFieldDefinition* const field)
{
	glUseProgram (getProgramId ());

	const GLint location = glGetUniformLocation (getProgramId (), field -> getName () .c_str ());

	if (location not_eq -1)
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
				glUniform3fv (location, 1, static_cast <SFColor*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFColorRGBA:
			{
				glUniform4fv (location, 1, static_cast <SFColorRGBA*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFDouble:
			{
				glUniform1d (location, static_cast <SFDouble*> (field) -> getValue ());
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
				glUniformMatrix3dv (location, 1, false, static_cast <SFMatrix3d*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFMatrix3f:
			{
				glUniformMatrix3fv (location, 1, false, static_cast <SFMatrix3f*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFMatrix4d:
			{
				glUniformMatrix4dv (location, 1, false, static_cast <SFMatrix4d*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFMatrix4f:
			{
				glUniformMatrix4fv (location, 1, false, static_cast <SFMatrix4f*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFNode:
			{
				const auto node = static_cast <SFNode*> (field);

				GLint textureUnit = 0;
				glGetUniformiv (getProgramId (), location, &textureUnit);

				if (not textureUnit)
				{
					if (getBrowser () -> getCombinedTextureUnits () .empty ())
					{
						getBrowser () -> println ("Warning: Not enough combined texture units for uniform variable '", field -> getName (), "' available.");
						break;
					}
					else
					{
						textureUnit = getBrowser () -> getCombinedTextureUnits () .top ();
						getBrowser () -> getCombinedTextureUnits () .pop ();
						textureUnits .emplace_back (textureUnit);
					}
				}

				glActiveTexture (GL_TEXTURE0 + textureUnit);

				const auto texture = x3d_cast <X3DTextureNode*> (*node);

				if (x3d_cast <X3DTexture2DNode*> (texture))
					glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());

				else if (x3d_cast <X3DTexture3DNode*> (texture))
					glBindTexture (GL_TEXTURE_3D, texture -> getTextureId ());

				else if (x3d_cast <X3DEnvironmentTextureNode*> (texture))
					glBindTexture (GL_TEXTURE_CUBE_MAP, texture -> getTextureId ());

				else
					glBindTexture (GL_TEXTURE_2D, 0);

				glUniform1i (location, textureUnit);
				glActiveTexture (GL_TEXTURE0);

				break;
			}
			case X3DConstants::SFRotation:
			{
				glUniform4fv (location, 1, static_cast <SFRotation*> (field) -> getValue () .quat () .data ());
				break;
			}
			case X3DConstants::SFString:
			{
				break;
			}
			case X3DConstants::SFTime:
			{
				glUniform1d (location, static_cast <SFTime*> (field) -> getValue ());
				break;
			}
			case X3DConstants::SFVec2d:
			{
				glUniform2dv (location, 1, static_cast <SFVec2d*> (field) -> getValue () .data ());
				break;
				break;
			}
			case X3DConstants::SFVec2f:
			{
				glUniform2fv (location, 1, static_cast <SFVec2f*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFVec3d:
			{
				glUniform3dv (location, 1, static_cast <SFVec3d*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFVec3f:
			{
				glUniform3fv (location, 1, static_cast <SFVec3f*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFVec4d:
			{
				glUniform4dv (location, 1, static_cast <SFVec4d*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::SFVec4f:
			{
				glUniform4fv (location, 1, static_cast <SFVec4f*> (field) -> getValue () .data ());
				break;
			}
			case X3DConstants::MFBool:
			{
				const auto array = static_cast <MFBool*> (field);

				std::vector <GLint> vector (array -> begin (), array -> end ());
				glUniform1iv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFColor:
			{
				const auto array = static_cast <MFColor*> (field);

				std::vector <Color3f> vector (array -> begin (), array -> end ());
				glUniform3fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFColorRGBA:
			{
				const auto array = static_cast <MFColorRGBA*> (field);

				std::vector <Color4f> vector (array -> begin (), array -> end ());
				glUniform4fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFDouble:
			{
				const auto array = static_cast <MFDouble*> (field);

				std::vector <GLdouble> vector (array -> begin (), array -> end ());
				glUniform1dv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFFloat:
			{
				const auto array = static_cast <MFFloat*> (field);

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
				const auto array = static_cast <MFInt32*> (field);

				std::vector <GLint> vector (array -> begin (), array -> end ());
				glUniform1iv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFMatrix3d:
			{
				const auto array = static_cast <MFMatrix3d*> (field);

				std::vector <Matrix3d> vector (array -> begin (), array -> end ());
				glUniformMatrix3dv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFMatrix3f:
			{
				const auto array = static_cast <MFMatrix3f*> (field);

				std::vector <Matrix3f> vector (array -> begin (), array -> end ());
				glUniformMatrix3fv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFMatrix4d:
			{
				const auto array = static_cast <MFMatrix4d*> (field);

				std::vector <Matrix4d> vector (array -> begin (), array -> end ());
				glUniformMatrix4dv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFMatrix4f:
			{
				const auto array = static_cast <MFMatrix4f*> (field);

				std::vector <Matrix4f> vector (array -> begin (), array -> end ());
				glUniformMatrix4fv (location, vector .size (), false, vector [0] .data ());
				break;
			}
			case X3DConstants::MFNode:
			{
				// Push back texture units
				{
					std::vector <size_t> textureUnits;

					for (size_t i = 0;; ++ i)
					{
						GLint textureUnit = 0;

						const GLint location = glGetUniformLocation (getProgramId (), (field -> getName () [0] + "[" + basic::to_string (i) + "]") .c_str ());

						if (location not_eq -1)
						{
							glGetUniformiv (getProgramId (), location, &textureUnit);

							if (textureUnit)
								textureUnits .emplace_back (textureUnit);
						}
						else
							break;
					}

					for (const auto & textureUnit : textureUnits)
						getBrowser () -> getCombinedTextureUnits () .push (textureUnit);
				}

				// Set uniform variable;

				const auto array = static_cast <MFNode*> (field);

				std::vector <GLint> vector;
				vector .reserve (array -> size ());

				for (const auto & node : *array)
				{
					GLint textureUnit = 0;

					if (getBrowser () -> getCombinedTextureUnits () .empty ())
					{
						getBrowser () -> println ("Warning: Not enough combined texture units for uniform variable '", field -> getName (), "' available.");
						break;
					}
					else
					{
						textureUnit = getBrowser () -> getCombinedTextureUnits () .top ();
						getBrowser () -> getCombinedTextureUnits () .pop ();
						textureUnits .emplace_back (textureUnit);
					}

					glActiveTexture (GL_TEXTURE0 + textureUnit);

					const auto texture = x3d_cast <X3DTextureNode*> (node);

					if (x3d_cast <X3DTexture2DNode*> (texture))
						glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());

					else if (x3d_cast <X3DTexture3DNode*> (texture))
						glBindTexture (GL_TEXTURE_3D, texture -> getTextureId ());

					else if (x3d_cast <X3DEnvironmentTextureNode*> (texture))
						glBindTexture (GL_TEXTURE_CUBE_MAP, texture -> getTextureId ());

					else
						glBindTexture (GL_TEXTURE_2D, 0);

					vector .emplace_back (textureUnit);
				}

				glUniform1iv (location, vector .size (), vector .data ());
				glActiveTexture (GL_TEXTURE0);

				break;
			}
			case X3DConstants::MFRotation:
			{
				const auto array = static_cast <MFRotation*> (field);

				std::vector <Vector4f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
				{
					const auto & quat = value .getValue () .quat ();
					vector .emplace_back (quat .x (), quat .y (), quat .z (), quat .w ());
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
				const auto array = static_cast <MFTime*> (field);

				std::vector <GLdouble> vector (array -> begin (), array -> end ());
				glUniform1dv (location, vector .size (), vector .data ());
				break;
			}
			case X3DConstants::MFVec2d:
			{
				const auto array = static_cast <MFVec2d*> (field);

				std::vector <Vector2d> vector (array -> begin (), array -> end ());
				glUniform2dv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec2f:
			{
				const auto array = static_cast <MFVec2f*> (field);

				std::vector <Vector2f> vector (array -> begin (), array -> end ());
				glUniform2fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec3d:
			{
				const auto array = static_cast <MFVec3d*> (field);

				std::vector <Vector3d> vector (array -> begin (), array -> end ());
				glUniform3dv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec3f:
			{
				const auto array = static_cast <MFVec3f*> (field);

				std::vector <Vector3f> vector (array -> begin (), array -> end ());
				glUniform3fv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec4d:
			{
				const auto array = static_cast <MFVec4d*> (field);

				std::vector <Vector4d> vector (array -> begin (), array -> end ());
				glUniform4dv (location, vector .size (), vector [0] .data ());
				break;
			}
			case X3DConstants::MFVec4f:
			{
				const auto array = static_cast <MFVec4f*> (field);

				std::vector <Vector4f> vector (array -> begin (), array -> end ());
				glUniform4fv (location, vector .size (), vector [0] .data ());
				break;
			}
		}
	}
	else
	{
		#ifdef TITANIA_DEBUG
		std::clog << "Warning: Uniform variable '" << field -> getName () << "' not found." << std::endl;

		if (not getProgramId ())
			std::clog << "Warning: Couldn't allocate shader." << std::endl;
		#endif
	}

	glUseProgram (0);
}

void
X3DProgrammableShaderObject::setTextureBuffer (const std::string & name, GLuint textureId)
{
	glUseProgram (getProgramId ());

	const GLint location = glGetUniformLocation (getProgramId (), name .c_str ());

	if (location not_eq -1)
	{
		GLint textureUnit = 0;
		glGetUniformiv (getProgramId (), location, &textureUnit);

		if (not textureUnit)
		{
			if (getBrowser () -> getCombinedTextureUnits () .empty ())
			{
				getBrowser () -> println ("Warning: Not enough combined texture units for uniform variable '", name, "' available.");
				return;
			}
			else
			{
				textureUnit = getBrowser () -> getCombinedTextureUnits () .top ();
				getBrowser () -> getCombinedTextureUnits () .pop ();
				textureUnits .emplace_back (textureUnit);
			}
		}

		glActiveTexture (GL_TEXTURE0 + textureUnit);
		glBindTexture (GL_TEXTURE_BUFFER, textureId);

		glUniform1i (location, textureUnit);
		glActiveTexture (GL_TEXTURE0);

	}
	else
	{
		#ifdef TITANIA_DEBUG
		std::clog << "Warning: Uniform variable '" << name << "' not found." << std::endl;
		#endif
	}

	glUseProgram (0);
}

void
X3DProgrammableShaderObject::dispose ()
{
	for (const auto & textureUnit : textureUnits)
		getBrowser () -> getCombinedTextureUnits () .push (textureUnit);
}

} // X3D
} // titania

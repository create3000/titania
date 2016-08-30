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
#include "../../Browser/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderer.h"
#include "../CubeMapTexturing/X3DEnvironmentTextureNode.h"
#include "../Shape/LineProperties.h"
#include "../Shape/X3DAppearanceNode.h"
#include "../Shape/X3DMaterialNode.h"
#include "../Texturing/X3DTexture2DNode.h"
#include "../Texturing/X3DTextureTransformNode.h"
#include "../Texturing3D/X3DTexture3DNode.h"

#include <Titania/String/to_string.h>

#include "../../Debug.h"

namespace titania {
namespace X3D {

static constexpr size_t MAX_CLIP_PLANES = 6;
static constexpr size_t MAX_LIGHTS      = 8;
static constexpr size_t MAX_TEX_COORD   = 4;

X3DProgrammableShaderObject::X3DProgrammableShaderObject () :
	              X3DBaseNode (),
	         x3d_GeometryType (-1),
	            x3d_ClipPlane (),
	              x3d_FogType (-1),
	             x3d_FogColor (-1),
	   x3d_FogVisibilityRange (-1),
	 x3d_LinewidthScaleFactor (-1),
	             x3d_Lighting (-1),
	        x3d_ColorMaterial (-1),
	            x3d_LightType (),
	           x3d_LightColor (),
	x3d_LightAmbientIntensity (),
	       x3d_LightIntensity (),
	     x3d_LightAttenuation (),
	        x3d_LightLocation (),
	       x3d_LightDirection (),
	       x3d_LightBeamWidth (),
	     x3d_LightCutOffAngle (),
	          x3d_LightRadius (),
	    x3d_SeparateBackColor (-1),
	     x3d_AmbientIntensity (-1),
	         x3d_DiffuseColor (-1),
	        x3d_SpecularColor (-1),
	        x3d_EmissiveColor (-1),
	            x3d_Shininess (-1),
	         x3d_Transparency (-1),
	 x3d_BackAmbientIntensity (-1),
	     x3d_BackDiffuseColor (-1),
	    x3d_BackSpecularColor (-1),
	    x3d_BackEmissiveColor (-1),
	        x3d_BackShininess (-1),
	     x3d_BackTransparency (-1),
	          x3d_TextureType (-1),
	              x3d_Texture (-1),
	       x3d_CubeMapTexture (-1),
	        x3d_TextureMatrix (-1),
	         x3d_NormalMatrix (-1),
	     x3d_ProjectionMatrix (-1),
	      x3d_ModelViewMatrix (-1),
	                x3d_Color (-1),
	             x3d_TexCoord (-1),
	               x3d_Normal (-1),
	               x3d_Vertex (-1),
	   extensionGPUShaderFP64 (false),
	transformFeedbackVaryings (),
	             textureUnits (),
	          numGlobalLights (0)
{
	addType (X3DConstants::X3DProgrammableShaderObject);
}

/*
 *
 *  Construction
 *
 */

void
X3DProgrammableShaderObject::setOpenGLES (const bool openGLES)
{
	if (openGLES)
		extensionGPUShaderFP64 = false;
	else
		extensionGPUShaderFP64 = getBrowser () -> isExtensionAvailable ("GL_ARB_gpu_shader_fp64");
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
X3DProgrammableShaderObject::getDefaultUniforms ()
{
	const auto program = getProgramId ();

	glUseProgram (program);

	x3d_ClipPlane             .clear ();
	x3d_LightType             .clear ();
	x3d_LightColor            .clear ();
	x3d_LightAmbientIntensity .clear ();
	x3d_LightIntensity        .clear ();
	x3d_LightAttenuation      .clear ();
	x3d_LightLocation         .clear ();
	x3d_LightDirection        .clear ();
	x3d_LightBeamWidth        .clear ();
	x3d_LightCutOffAngle      .clear ();
	x3d_LightRadius           .clear ();

	// Get default uniforms.

	x3d_GeometryType = glGetUniformLocation (program, "x3d_GeometryType");

	for (size_t i = 0, size = getBrowser () -> getMaxClipPlanes (); i < size; ++ i)
		x3d_ClipPlane .emplace_back (glGetUniformLocation (program, ("x3d_ClipPlane[" + basic::to_string (i, std::locale::classic ()) + "]") .c_str ()));

	x3d_FogType            = glGetUniformLocation (program, "x3d_FogType");
	x3d_FogColor           = glGetUniformLocation (program, "x3d_FogColor");
	x3d_FogVisibilityRange = glGetUniformLocation (program, "x3d_FogVisibilityRange");

	x3d_LinewidthScaleFactor = glGetUniformLocation (program, "x3d_LinewidthScaleFactor");

	x3d_Lighting      = glGetUniformLocation (program, "x3d_Lighting");
	x3d_ColorMaterial = glGetUniformLocation (program, "x3d_ColorMaterial");

	for (size_t i = 0, size = getBrowser () -> getMaxLights (); i < size; ++ i)
	{
		const auto is = basic::to_string (i, std::locale::classic ());

		x3d_LightType             .emplace_back (glGetUniformLocation (program, ("x3d_LightType[" + is + "]") .c_str ()));
		x3d_LightColor            .emplace_back (glGetUniformLocation (program, ("x3d_LightColor[" + is + "]") .c_str ()));
		x3d_LightAmbientIntensity .emplace_back (glGetUniformLocation (program, ("x3d_LightAmbientIntensity[" + is + "]") .c_str ()));
		x3d_LightIntensity        .emplace_back (glGetUniformLocation (program, ("x3d_LightIntensity[" + is + "]") .c_str ()));
		x3d_LightAttenuation      .emplace_back (glGetUniformLocation (program, ("x3d_LightAttenuation[" + is + "]") .c_str ()));
		x3d_LightLocation         .emplace_back (glGetUniformLocation (program, ("x3d_LightLocation[" + is + "]") .c_str ()));
		x3d_LightDirection        .emplace_back (glGetUniformLocation (program, ("x3d_LightDirection[" + is + "]") .c_str ()));
		x3d_LightBeamWidth        .emplace_back (glGetUniformLocation (program, ("x3d_LightBeamWidth[" + is + "]") .c_str ()));
		x3d_LightCutOffAngle      .emplace_back (glGetUniformLocation (program, ("x3d_LightCutOffAngle[" + is + "]") .c_str ()));
		x3d_LightRadius           .emplace_back (glGetUniformLocation (program, ("x3d_LightRadius[" + is + "]") .c_str ()));
	}

	x3d_SeparateBackColor = glGetUniformLocation (program, "x3d_SeparateBackColor");

	x3d_AmbientIntensity = glGetUniformLocation (program, "x3d_AmbientIntensity");
	x3d_DiffuseColor     = glGetUniformLocation (program, "x3d_DiffuseColor");
	x3d_SpecularColor    = glGetUniformLocation (program, "x3d_SpecularColor");
	x3d_EmissiveColor    = glGetUniformLocation (program, "x3d_EmissiveColor");
	x3d_Shininess        = glGetUniformLocation (program, "x3d_Shininess");
	x3d_Transparency     = glGetUniformLocation (program, "x3d_Transparency");

	x3d_BackAmbientIntensity = glGetUniformLocation (program, "x3d_BackAmbientIntensity");
	x3d_BackDiffuseColor     = glGetUniformLocation (program, "x3d_BackDiffuseColor");
	x3d_BackSpecularColor    = glGetUniformLocation (program, "x3d_BackSpecularColor");
	x3d_BackEmissiveColor    = glGetUniformLocation (program, "x3d_BackEmissiveColor");
	x3d_BackShininess        = glGetUniformLocation (program, "x3d_BackShininess");
	x3d_BackTransparency     = glGetUniformLocation (program, "x3d_BackTransparency");

	x3d_TextureType    = glGetUniformLocation (program, "x3d_TextureType");
	x3d_Texture        = glGetUniformLocation (program, "x3d_Texture");
	x3d_CubeMapTexture = glGetUniformLocation (program, "x3d_CubeMapTexture");

	x3d_TextureMatrix    = glGetUniformLocation (program, "x3d_TextureMatrix");
	x3d_NormalMatrix     = glGetUniformLocation (program, "x3d_NormalMatrix");
	x3d_ProjectionMatrix = glGetUniformLocation (program, "x3d_ProjectionMatrix");
	x3d_ModelViewMatrix  = glGetUniformLocation (program, "x3d_ModelViewMatrix");

	x3d_Color    = glGetAttribLocation (program, "x3d_Color");
	x3d_TexCoord = glGetAttribLocation (program, "x3d_TexCoord");
	x3d_Normal   = glGetAttribLocation (program, "x3d_Normal");
	x3d_Vertex   = glGetAttribLocation (program, "x3d_Vertex");

	static const auto textureType    = std::vector <int32_t> ({ 0 });
	static const auto texture        = std::vector <int32_t> ({ 0 });
	static const auto cubeMapTexture = std::vector <int32_t> ({ 1 });

	glUniform1f  (x3d_LinewidthScaleFactor, 1);
	glUniform1iv (x3d_TextureType,          1, textureType    .data ());
	glUniform1iv (x3d_Texture,              1, texture        .data ()); // Set texture to active texture unit 0.
	glUniform1iv (x3d_CubeMapTexture,       1, cubeMapTexture .data ()); // Set cube map texture to active texture unit 1.

	glUseProgram (0);
}

/*
 *
 *  Fields
 *
 */

void
X3DProgrammableShaderObject::addShaderFields ()
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
	{
		field -> addInterest (this, &X3DProgrammableShaderObject::set_field, field);

		set_field (field);
	}
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

	if (location == -1)
	{
		#ifdef TITANIA_DEBUG
		//getBrowser () -> println ("Warning: Uniform variable '", field -> getName (), "' not found.");

		//if (not getProgramId ())
		//	getBrowser () -> println ("Warning: Couldn't allocate shader.");
		#endif

		glUseProgram (0);
		return;
	}

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
			if (extensionGPUShaderFP64)
				glUniform1d (location, static_cast <SFDouble*> (field) -> getValue ());
			else
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
			if (extensionGPUShaderFP64)
				glUniformMatrix3dv (location, 1, false, static_cast <SFMatrix3d*> (field) -> getValue () .data ());
			else
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
			if (extensionGPUShaderFP64)
				glUniformMatrix4dv (location, 1, false, static_cast <SFMatrix4d*> (field) -> getValue () .data ());
			else
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
			if (extensionGPUShaderFP64)
				glUniform4dv (location, 1, static_cast <SFRotation*> (field) -> getValue () .quat () .data ());
			else
				glUniform4fv (location, 1, Quaternion4f (static_cast <SFRotation*> (field) -> getValue () .quat ()) .data ());

			break;
		}
		case X3DConstants::SFString:
		{
			break;
		}
		case X3DConstants::SFTime:
		{
			if (extensionGPUShaderFP64)
				glUniform1d (location, static_cast <SFTime*> (field) -> getValue ());
			else
				glUniform1f (location, static_cast <SFTime*> (field) -> getValue ());

			break;
		}
		case X3DConstants::SFVec2d:
		{
			if (extensionGPUShaderFP64)
				glUniform2dv (location, 1, static_cast <SFVec2d*> (field) -> getValue () .data ());
			else
				glUniform2fv (location, 1, Vector2f (static_cast <SFVec2d*> (field) -> getValue ()) .data ());

			break;
		}
		case X3DConstants::SFVec2f:
		{
			glUniform2fv (location, 1, static_cast <SFVec2f*> (field) -> getValue () .data ());
			break;
		}
		case X3DConstants::SFVec3d:
		{
			if (extensionGPUShaderFP64)
				glUniform3dv (location, 1, static_cast <SFVec3d*> (field) -> getValue () .data ());
			else
				glUniform3fv (location, 1, Vector3f (static_cast <SFVec3d*> (field) -> getValue ()) .data ());

			break;
		}
		case X3DConstants::SFVec3f:
		{
			glUniform3fv (location, 1, static_cast <SFVec3f*> (field) -> getValue () .data ());
			break;
		}
		case X3DConstants::SFVec4d:
		{
			if (extensionGPUShaderFP64)
				glUniform4dv (location, 1, static_cast <SFVec4d*> (field) -> getValue () .data ());
			else
				glUniform4fv (location, 1, Vector4f (static_cast <SFVec4d*> (field) -> getValue ()) .data ());

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

			std::vector <int32_t> vector (array -> begin (), array -> end ());
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

			if (extensionGPUShaderFP64)
			{
				std::vector <double> vector (array -> begin (), array -> end ());
				glUniform1dv (location, vector .size (), vector .data ());
			}
			else
			{
				std::vector <float> vector (array -> begin (), array -> end ());
				glUniform1fv (location, vector .size (), vector .data ());
			}

			break;
		}
		case X3DConstants::MFFloat:
		{
			const auto array = static_cast <MFFloat*> (field);

			std::vector <float> vector (array -> begin (), array -> end ());
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

			std::vector <int32_t> vector (array -> begin (), array -> end ());
			glUniform1iv (location, vector .size (), vector .data ());
			break;
		}
		case X3DConstants::MFMatrix3d:
		{
			const auto array = static_cast <MFMatrix3d*> (field);

			if (extensionGPUShaderFP64)
			{
				std::vector <Matrix3d> vector (array -> begin (), array -> end ());
				glUniformMatrix3dv (location, vector .size (), false, vector [0] .data ());
			}
			else
			{
				std::vector <Matrix3f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value .getValue ());

				glUniformMatrix3fv (location, vector .size (), false, vector [0] .data ());
			}

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

			if (extensionGPUShaderFP64)
			{
				std::vector <Matrix4d> vector (array -> begin (), array -> end ());
				glUniformMatrix4dv (location, vector .size (), false, vector [0] .data ());
			}
			else
			{
				std::vector <Matrix4f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value .getValue ());

				glUniformMatrix4fv (location, vector .size (), false, vector [0] .data ());
			}

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

					const GLint location = glGetUniformLocation (getProgramId (), (field -> getName () [0] + "[" + basic::to_string (i, std::locale::classic ()) + "]") .c_str ());

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

			if (extensionGPUShaderFP64)
			{
				std::vector <Quaternion4d> vector;
				vector .reserve (array -> size ());
	
				for (const auto & value : *array)
					vector .emplace_back (value .getValue () .quat ());
	
				glUniform4dv (location, vector .size (), vector [0] .data ());
			}
			else
			{
				std::vector <Quaternion4f> vector;
				vector .reserve (array -> size ());
	
				for (const auto & value : *array)
					vector .emplace_back (value .getValue () .quat ());
	
				glUniform4fv (location, vector .size (), vector [0] .data ());
			}

			break;
		}
		case X3DConstants::MFString:
		{
			break;
		}
		case X3DConstants::MFTime:
		{
			const auto array = static_cast <MFTime*> (field);

			if (extensionGPUShaderFP64)
			{
				std::vector <double> vector (array -> begin (), array -> end ());
				glUniform1dv (location, vector .size (), vector .data ());
			}
			else
			{
				std::vector <float> vector (array -> begin (), array -> end ());
				glUniform1fv (location, vector .size (), vector .data ());
			}

			break;
		}
		case X3DConstants::MFVec2d:
		{
			const auto array = static_cast <MFVec2d*> (field);

			if (extensionGPUShaderFP64)
			{
				std::vector <Vector2d> vector (array -> begin (), array -> end ());
				glUniform2dv (location, vector .size (), vector [0] .data ());
			}
			else
			{
				std::vector <Vector2f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value .getValue ());

				glUniform2fv (location, vector .size (), vector [0] .data ());
			}

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

			if (extensionGPUShaderFP64)
			{
				std::vector <Vector3d> vector (array -> begin (), array -> end ());
				glUniform3dv (location, vector .size (), vector [0] .data ());
			}
			else
			{
				std::vector <Vector3f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value .getValue ());

				glUniform3fv (location, vector .size (), vector [0] .data ());
			}

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

			if (extensionGPUShaderFP64)
			{
				std::vector <Vector4d> vector (array -> begin (), array -> end ());
				glUniform4dv (location, vector .size (), vector [0] .data ());
			}
			else
			{
				std::vector <Vector4f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value .getValue ());

				glUniform4fv (location, vector .size (), vector [0] .data ());
			}

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
		//getBrowser () -> println ("Warning: Uniform variable '", name, "' not found.");
		#endif
	}

	glUseProgram (0);
}

/*
 *
 *  Pipeline
 *
 */

void
X3DProgrammableShaderObject::set_shading (const ShadingType & shading)
{
//	switch (geometryType)
//	{
//		case 0:
//		{
//			switch (shading)
//			{
//				case ShadingType::POINT:
//				{
//					primitiveMode = GL_POINTS;
//					wireframe     = true;
//					break;
//				}
//				case ShadingType::WIREFRAME:
//				{
//					primitiveMode = GL_POINTS;
//					wireframe     = true;
//					break;
//				}
//				default:
//				{
//					// case FLAT:
//					// case GOURAUD:
//					// case PHONG:
//
//					this .primitiveMode = GL_POINTS;
//					this .wireframe     = true;
//					break;
//				}
//			}
//
//			break;
//		}
//		case 1:
//		{
//			switch (shading)
//			{
//				case ShadingType::POINT:
//				{
//					primitiveMode = GL_POINTS;
//					wireframe     = true;
//					break;
//				}
//				case ShadingType::WIREFRAME:
//				{
//					this .primitiveMode = GL_LINES;
//					this .wireframe     = true;
//					break;
//				}
//				default:
//				{
//					// case FLAT:
//					// case GOURAUD:
//					// case PHONG:
//
//					primitiveMode = gl_LINES;
//					wireframe     = true;
//					break;
//				}
//			}
//
//			break;
//		}
//		case 2:
//		case 3:
//		{
//			switch (shading)
//			{
//				case ShadingType::POINT:
//				{
//					primitiveMode = GL_POINTS;
//					wireframe     = true;
//					break;
//				}
//				case ShadingType::WIREFRAME":
//				{
//					this .primitiveMode = GL_LINE_LOOP;
//					this .wireframe     = true;
//					break;
//				}
//				default:
//				{
//					// case FLAT:
//					// case GOURAUD:
//					// case PHONG:
//
//					primitiveMode = GL_TRIANGLES;
//					wireframe     = false;
//					break;
//				}
//			}	
//
//			break;
//		}
//	}
}

void
X3DProgrammableShaderObject::setGlobalUniforms (ShapeContainer* const context)
{
	const auto & browser      = getBrowser ();
	const auto & globalLights = context -> getRenderer () -> getGlobalLights ();

	if (extensionGPUShaderFP64)
		glUniformMatrix4dv (x3d_ProjectionMatrix, 1, false, browser -> getProjectionMatrix () .get () .data ());
	else
		glUniformMatrix4fv (x3d_ProjectionMatrix, 1, false, Matrix4f (browser -> getProjectionMatrix () .get ()) .data ());

	// Set global lights

	numGlobalLights = std::min (MAX_LIGHTS, globalLights .size ());

	for (size_t i = 0; i < numGlobalLights; ++ i)
		globalLights [i] -> setShaderUniforms (this, i);
}

void
X3DProgrammableShaderObject::setLocalUniforms (ShapeContainer* const context)
{
	static const auto textureType = std::vector <int32_t> ({ 0 });

	const auto & browser              = getBrowser ();
	const auto & clipPlanes           = context -> getClipPlanes ();
	const auto & linePropertiesNode   = browser -> getLineProperties ();
	const auto & materialNode         = browser -> getMaterial ();
	const auto & textureNode          = browser -> getTexture ();
	const auto & textureTransformNode = browser -> getTextureTransform ();
	auto         normalMatrix         = Matrix3d (context -> getModelViewMatrix ()); // Transposed when uniform is set.

	try
	{
		normalMatrix .inverse ();
	}
	catch (const std::domain_error &)
	{
		normalMatrix .set ();
	}

	// Geometry type

	glUniform1i (x3d_GeometryType, GLint (context -> getGeometryType ()));

	// Clip planes

	if (clipPlanes .empty ())
	{
		glUniform4f (x3d_ClipPlane [0], 0, 0, 0, 0);
	}
	else
	{
		const auto numClipPlanes = std::min (MAX_CLIP_PLANES, clipPlanes .size ());

		for (size_t i = 0; i < numClipPlanes; ++ i)
			clipPlanes [i] -> setShaderUniforms (this, i);

		if (numClipPlanes < MAX_CLIP_PLANES)
			glUniform4f (x3d_ClipPlane [numClipPlanes], 0, 0, 0, 0);
	}

	// Fog

	context -> getFog () -> setShaderUniforms (this);

	// Lights

	const auto & localLights = context -> getLocalLights ();
	const auto   numLights   = std::min (MAX_LIGHTS, numGlobalLights + localLights .size ());

	for (size_t i = numGlobalLights, l = 0; i < numLights; ++ i, ++ l)
		localLights [l] -> setShaderUniforms (this, i);

	if (numLights < MAX_LIGHTS)
		glUniform1i (x3d_LightType [numLights], 0);

	// Appearance

	glUniform1i (x3d_ColorMaterial, context -> getColorMaterial ());

	linePropertiesNode -> setShaderUniforms (this);

	if (materialNode)
		materialNode -> setShaderUniforms (this);
	else
		glUniform1i (x3d_Lighting, false);

	if (textureNode)
		textureNode -> setShaderUniforms (this);
	else
		glUniform1iv (x3d_TextureType, 1, textureType .data ());

	// Matrices

	textureTransformNode -> setShaderUniforms (this);

	if (extensionGPUShaderFP64)
	{
		glUniformMatrix3dv (x3d_NormalMatrix,    1, true,  normalMatrix .data ());
		glUniformMatrix4dv (x3d_ModelViewMatrix, 1, false, context -> getModelViewMatrix () .data ());
	}
	else
	{
		glUniformMatrix3fv (x3d_NormalMatrix,    1, true,  Matrix3f (normalMatrix) .data ());
		glUniformMatrix4fv (x3d_ModelViewMatrix, 1, false, Matrix4f (context -> getModelViewMatrix ()) .data ());
	}
}

void
X3DProgrammableShaderObject::enableColorAttrib (const GLuint buffer)
{
	if (x3d_Color == -1)
		return;

	glEnableVertexAttribArray (x3d_Color);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (x3d_Color, 4, GL_FLOAT, false, 0, nullptr);
}

void
X3DProgrammableShaderObject::disableColorAttrib ()
{
	if (x3d_Color == -1)
		return;

	glDisableVertexAttribArray (x3d_Color);
}

void
X3DProgrammableShaderObject::enableTexCoordAttrib (const std::vector <GLuint> & buffers)
{
	if (x3d_TexCoord == -1)
		return;

	for (size_t i = 0, size = std::min (MAX_TEX_COORD, buffers .size ()); i < size; ++ i)
	{
		glEnableVertexAttribArray (x3d_TexCoord + i);

		glBindBuffer (GL_ARRAY_BUFFER, buffers [i]);
		glVertexAttribPointer (x3d_TexCoord + i, 4, GL_FLOAT, false, 0, nullptr);

		break; // TODO: Currently only one tex coord node is supported.
	}
}

void
X3DProgrammableShaderObject::disableTexCoordAttrib ()
{
	if (x3d_TexCoord == -1)
		return;

	glDisableVertexAttribArray (x3d_TexCoord);
}

void
X3DProgrammableShaderObject::enableNormalAttrib (const GLuint buffer)
{
	if (x3d_Normal == -1)
		return;

	glEnableVertexAttribArray (x3d_Normal);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (x3d_Normal, 3, GL_FLOAT, false, 0, nullptr);
}

void
X3DProgrammableShaderObject::disableNormalAttrib ()
{
	if (x3d_Normal == -1)
		return;

	glDisableVertexAttribArray (x3d_Normal);
}

void
X3DProgrammableShaderObject::enableVertexAttrib (const GLuint buffer)
{
	if (x3d_Vertex == -1)
		return;

	glEnableVertexAttribArray (x3d_Vertex);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (x3d_Vertex, 3, GL_DOUBLE, false, 0, nullptr);
}

void
X3DProgrammableShaderObject::disableVertexAttrib ()
{
	if (x3d_Vertex == -1)
		return;

	glDisableVertexAttribArray (x3d_Vertex);
}

void
X3DProgrammableShaderObject::enableFloatAttrib (const std::string & name, const GLuint buffer, const size_t components)
{
	const auto location = glGetAttribLocation (getProgramId (), name .c_str ());

	if (location == -1)
		return;

	glEnableVertexAttribArray (location);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (location, components, GL_FLOAT, false, 0, (void*) 0);
}

void
X3DProgrammableShaderObject::disableFloatAttrib (const std::string & name)
{
	const auto location = glGetAttribLocation (getProgramId (), name .c_str ());

	if (location == -1)
		return;

	glDisableVertexAttribArray (location);
}

void
X3DProgrammableShaderObject::enableMatrix3Attrib (const std::string & name, const GLuint buffer)
{
	const auto location = glGetAttribLocation (getProgramId (), name .c_str ());

	if (location == -1)
		return;

	glEnableVertexAttribArray (location + 0);
	glEnableVertexAttribArray (location + 1);
	glEnableVertexAttribArray (location + 2);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);

	glVertexAttribPointer (location + 0, 3, GL_FLOAT, false, sizeof (Matrix3f), (void*) (sizeof (Vector3f) * 0));
	glVertexAttribPointer (location + 1, 3, GL_FLOAT, false, sizeof (Matrix3f), (void*) (sizeof (Vector3f) * 1));
	glVertexAttribPointer (location + 2, 3, GL_FLOAT, false, sizeof (Matrix3f), (void*) (sizeof (Vector3f) * 2));
}

void
X3DProgrammableShaderObject::disableMatrix3Attrib (const std::string & name)
{
	const auto location = glGetAttribLocation (getProgramId (), name .c_str ());

	if (location == -1)
		return;

	glDisableVertexAttribArray (location + 0);
	glDisableVertexAttribArray (location + 1);
	glDisableVertexAttribArray (location + 2);
}

void
X3DProgrammableShaderObject::enableMatrix4Attrib (const std::string & name, const GLuint buffer)
{
	const GLint location = glGetAttribLocation (getProgramId (), name .c_str ());

	if (location == -1)
		return;

	glEnableVertexAttribArray (location + 0);
	glEnableVertexAttribArray (location + 1);
	glEnableVertexAttribArray (location + 2);
	glEnableVertexAttribArray (location + 3);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);

	glVertexAttribPointer (location + 0, 4, GL_FLOAT, false, sizeof (Matrix4f), (void*) (sizeof (Vector4f) * 0));
	glVertexAttribPointer (location + 1, 4, GL_FLOAT, false, sizeof (Matrix4f), (void*) (sizeof (Vector4f) * 1));
	glVertexAttribPointer (location + 2, 4, GL_FLOAT, false, sizeof (Matrix4f), (void*) (sizeof (Vector4f) * 2));
	glVertexAttribPointer (location + 3, 4, GL_FLOAT, false, sizeof (Matrix4f), (void*) (sizeof (Vector4f) * 3));
}

void
X3DProgrammableShaderObject::disableMatrix4Attrib (const std::string & name)
{
	const auto location = glGetAttribLocation (getProgramId (), name .c_str ());

	if (location == -1)
		return;

	glDisableVertexAttribArray (location + 0);
	glDisableVertexAttribArray (location + 1);
	glDisableVertexAttribArray (location + 2);
	glDisableVertexAttribArray (location + 3);
}

/*
 *
 *  Destruction
 *
 */

void
X3DProgrammableShaderObject::dispose ()
{
	for (const auto & textureUnit : textureUnits)
		getBrowser () -> getCombinedTextureUnits () .push (textureUnit);
}

} // X3D
} // titania

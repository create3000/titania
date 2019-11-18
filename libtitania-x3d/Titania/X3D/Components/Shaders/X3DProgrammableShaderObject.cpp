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

#include "X3DProgrammableShaderObject.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Core/RenderingProperties.h"
#include "../../Browser/ParticleSystems/SoftParticle.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderObject.h"

#include "../EnvironmentalEffects/X3DFogObject.h"
#include "../Navigation/OrthoViewpoint.h"
#include "../Navigation/NavigationInfo.h"
#include "../Navigation/X3DViewpointNode.h"
#include "../Shape/LineProperties.h"
#include "../Shape/FillProperties.h"
#include "../Shape/X3DAppearanceNode.h"
#include "../Shape/X3DMaterialNode.h"
#include "../Shape/X3DShapeNode.h"
#include "../Texturing/MultiTexture.h"
#include "../Texturing/X3DTextureCoordinateNode.h"
#include "../Texturing/X3DTextureTransformNode.h"

#include <Titania/String/to_string.h>

namespace titania {
namespace X3D {

X3DProgrammableShaderObject::X3DProgrammableShaderObject () :
	                            X3DBaseNode (),
	            x3d_LogarithmicFarFactor1_2 (-1),
	                       x3d_GeometryType (-1),
	                      x3d_NumClipPlanes (-1),
	                          x3d_ClipPlane (),
	                            x3d_FogType (-1),
	                           x3d_FogColor (-1),
	                 x3d_FogVisibilityRange (-1),
	                          x3d_FogMatrix (-1),
	                           x3d_FogCoord (-1),
	x3d_PointPropertiesPointSizeScaleFactor (-1),
	   x3d_PointPropertiesPointSizeMinValue (-1),
	   x3d_PointPropertiesPointSizeMaxValue (-1),
	x3d_PointPropertiesPointSizeAttenuation (-1),
	           x3d_PointPropertiesColorMode (-1),
	              x3d_LinePropertiesApplied (-1),
	 x3d_LinePropertiesLinewidthScaleFactor (-1),
	             x3d_LinePropertiesLinetype (-1),
	               x3d_FillPropertiesFilled (-1),
	              x3d_FillPropertiesHatched (-1),
	           x3d_FillPropertiesHatchColor (-1),
	           x3d_FillPropertiesHatchStyle (-1),
	                           x3d_Lighting (-1),
	                      x3d_ColorMaterial (-1),
	                          x3d_NumLights (-1),
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
	                        x3d_LightMatrix (),
	                        x3d_ShadowColor (),
	                    x3d_ShadowIntensity (),
	                         x3d_ShadowBias (),
	                       x3d_ShadowMatrix (),
	                      x3d_ShadowMapSize (),
	                          x3d_ShadowMap (),
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
	                        x3d_NumTextures (-1),
	                        x3d_TextureType (),
	                          x3d_Texture2D (),
	                          x3d_Texture3D (),
	                     x3d_CubeMapTexture (),
	                  x3d_MultiTextureColor (-1),
	                   x3d_MultiTextureMode (),
	              x3d_MultiTextureAlphaMode (),
	                 x3d_MultiTextureSource (),
	               x3d_MultiTextureFunction (),
	     x3d_TextureCoordinateGeneratorMode (),
	x3d_TextureCoordinateGeneratorParameter (),
	              x3d_NumProjectiveTextures (-1),
	                  x3d_ProjectiveTexture (),
	            x3d_ProjectiveTextureMatrix (),
	          x3d_ProjectiveTextureLocation (),
	                           x3d_Viewport (-1),
	                   x3d_ProjectionMatrix (-1),
	                    x3d_ModelViewMatrix (-1),
	                       x3d_NormalMatrix (-1),
	                      x3d_TextureMatrix (),
	                  x3d_CameraSpaceMatrix (-1),
	                           x3d_FogDepth (-1),
	                              x3d_Color (-1),
	                           x3d_TexCoord (),
	                             x3d_Normal (-1),
	                             x3d_Vertex (-1),
	                         x3d_ParticleId (-1),
	                       x3d_ParticleLife (-1),
	                x3d_ParticleElapsedTime (-1),
	                 extensionGPUShaderFP64 (false),
	              transformFeedbackVaryings (),
	                        numGlobalLights (0),
	            numGlobalProjectiveTextures (0),
	                           fogContainer (nullptr),
	                        lightContainers (),
	            projectiveTextureContainers (),
	                               textures ()
{
	addType (X3DConstants::X3DProgrammableShaderObject);
}

/*
 *
 *  Construction
 *
 */

void
X3DProgrammableShaderObject::initialize ()
{
	x3d_ClipPlane                           .resize (getBrowser () -> getMaxClipPlanes (), -1);
	x3d_LightType                           .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightColor                          .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightAmbientIntensity               .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightIntensity                      .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightAttenuation                    .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightLocation                       .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightDirection                      .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightBeamWidth                      .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightCutOffAngle                    .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightRadius                         .resize (getBrowser () -> getMaxLights (), -1);
	x3d_LightMatrix                         .resize (getBrowser () -> getMaxLights (), -1);
	x3d_ShadowColor                         .resize (getBrowser () -> getMaxLights (), -1);
	x3d_ShadowIntensity                     .resize (getBrowser () -> getMaxLights (), -1);
	x3d_ShadowBias                          .resize (getBrowser () -> getMaxLights (), -1);
	x3d_ShadowMatrix                        .resize (getBrowser () -> getMaxLights (), -1);
	x3d_ShadowMapSize                       .resize (getBrowser () -> getMaxLights (), -1);
	x3d_ShadowMap                           .resize (getBrowser () -> getMaxLights (), -1);
	x3d_TextureType                         .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_Texture2D                           .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_Texture3D                           .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_CubeMapTexture                      .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_MultiTextureMode                    .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_MultiTextureAlphaMode               .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_MultiTextureSource                  .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_MultiTextureFunction                .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_TextureCoordinateGeneratorMode      .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_TextureCoordinateGeneratorParameter .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_ProjectiveTexture                   .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_ProjectiveTextureMatrix             .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_ProjectiveTextureLocation           .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_TextureMatrix                       .resize (getBrowser () -> getMaxTextures (), -1);
	x3d_TexCoord                            .resize (getBrowser () -> getMaxTextures (), -1);

	lightContainers                         .resize (getBrowser () -> getMaxLights ());
	projectiveTextureContainers             .resize (getBrowser () -> getMaxTextures ());
}

void
X3DProgrammableShaderObject::setOpenGLES (const bool openGLES)
{
	if (openGLES)
		extensionGPUShaderFP64 = false;
	else
		extensionGPUShaderFP64 = getBrowser () -> getExtension ("GL_ARB_gpu_shader_fp64");
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

	// Get default uniforms.

	x3d_LogarithmicFarFactor1_2 = glGetUniformLocation (program, "x3d_LogarithmicFarFactor1_2");

	x3d_GeometryType  = glGetUniformLocation (program, "x3d_GeometryType");
	x3d_NumClipPlanes = glGetUniformLocation (program, "x3d_NumClipPlanes");

	for (size_t i = 0, size = getBrowser () -> getMaxClipPlanes (); i < size; ++ i)
		x3d_ClipPlane [i] = glGetUniformLocation (program, ("x3d_ClipPlane[" + basic::to_string (i, std::locale::classic ()) + "]") .c_str ());

	x3d_FogType            = getUniformLocation (program, "x3d_Fog.type",            "x3d_FogType");
	x3d_FogColor           = getUniformLocation (program, "x3d_Fog.color",           "x3d_FogColor");
	x3d_FogVisibilityRange = getUniformLocation (program, "x3d_Fog.visibilityRange", "x3d_FogVisibilityRange");
	x3d_FogMatrix          = getUniformLocation (program, "x3d_Fog.matrix",          "x3d_FogMatrix");
	x3d_FogCoord           = getUniformLocation (program, "x3d_Fog.fogCoord",        "x3d_FogCoord");

	x3d_PointPropertiesPointSizeScaleFactor = glGetUniformLocation (program, "x3d_PointProperties.pointSizeScaleFactor");
	x3d_PointPropertiesPointSizeMinValue    = glGetUniformLocation (program, "x3d_PointProperties.pointSizeMinValue");
	x3d_PointPropertiesPointSizeMaxValue    = glGetUniformLocation (program, "x3d_PointProperties.pointSizeMaxValue");
	x3d_PointPropertiesPointSizeAttenuation = glGetUniformLocation (program, "x3d_PointProperties.pointSizeAttenuation");
	x3d_PointPropertiesColorMode            = glGetUniformLocation (program, "x3d_PointProperties.colorMode");

	x3d_LinePropertiesApplied              = glGetUniformLocation (program, "x3d_LineProperties.applied");
	x3d_LinePropertiesLinewidthScaleFactor = getUniformLocation (program, "x3d_LineProperties.linewidthScaleFactor", "x3d_LinewidthScaleFactor");
	x3d_LinePropertiesLinetype             = glGetUniformLocation (program, "x3d_LineProperties.linetype");

	x3d_FillPropertiesFilled     = glGetUniformLocation (program, "x3d_FillProperties.filled");
	x3d_FillPropertiesHatched    = glGetUniformLocation (program, "x3d_FillProperties.hatched");
	x3d_FillPropertiesHatchColor = glGetUniformLocation (program, "x3d_FillProperties.hatchColor");
	x3d_FillPropertiesHatchStyle = glGetUniformLocation (program, "x3d_FillProperties.hatchStyle");

	x3d_Lighting      = glGetUniformLocation (program, "x3d_Lighting");
	x3d_ColorMaterial = glGetUniformLocation (program, "x3d_ColorMaterial");
	x3d_NumLights     = glGetUniformLocation (program, "x3d_NumLights");

	for (size_t i = 0, size = getBrowser () -> getMaxLights (); i < size; ++ i)
	{
		const auto is = basic::to_string (i, std::locale::classic ());

		x3d_LightType             [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].type",             "x3d_LightType[" + is + "]");
		x3d_LightColor            [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].color",            "x3d_LightColor[" + is + "]");
		x3d_LightAmbientIntensity [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].ambientIntensity", "x3d_LightAmbientIntensity[" + is + "]");
		x3d_LightIntensity        [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].intensity",        "x3d_LightIntensity[" + is + "]");
		x3d_LightAttenuation      [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].attenuation",      "x3d_LightAttenuation[" + is + "]");
		x3d_LightLocation         [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].location",         "x3d_LightLocation[" + is + "]");
		x3d_LightDirection        [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].direction",        "x3d_LightDirection[" + is + "]");
		x3d_LightBeamWidth        [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].beamWidth",        "x3d_LightBeamWidth[" + is + "]");
		x3d_LightCutOffAngle      [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].cutOffAngle",      "x3d_LightCutOffAngle[" + is + "]");
		x3d_LightRadius           [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].radius",           "x3d_LightRadius[" + is + "]");
		x3d_LightMatrix           [i] = getUniformLocation (program, "x3d_LightSource[" + is + "].matrix",           "x3d_LightMatrix[" + is + "]");

		x3d_ShadowColor           [i] = glGetUniformLocation (program, ("x3d_LightSource[" + is + "].shadowColor") .c_str ());
		x3d_ShadowIntensity       [i] = glGetUniformLocation (program, ("x3d_LightSource[" + is + "].shadowIntensity") .c_str ());
		x3d_ShadowBias            [i] = glGetUniformLocation (program, ("x3d_LightSource[" + is + "].shadowBias") .c_str ());
		x3d_ShadowMatrix          [i] = glGetUniformLocation (program, ("x3d_LightSource[" + is + "].shadowMatrix") .c_str ());
		x3d_ShadowMapSize         [i] = glGetUniformLocation (program, ("x3d_LightSource[" + is + "].shadowMapSize") .c_str ());
		x3d_ShadowMap             [i] = glGetUniformLocation (program, ("x3d_ShadowMap[" + is + "]") .c_str ());
	}

	x3d_SeparateBackColor = glGetUniformLocation (program, "x3d_SeparateBackColor");

	x3d_AmbientIntensity = getUniformLocation (program, "x3d_FrontMaterial.ambientIntensity", "x3d_AmbientIntensity");
	x3d_DiffuseColor     = getUniformLocation (program, "x3d_FrontMaterial.diffuseColor",     "x3d_DiffuseColor");
	x3d_SpecularColor    = getUniformLocation (program, "x3d_FrontMaterial.specularColor",    "x3d_SpecularColor");
	x3d_EmissiveColor    = getUniformLocation (program, "x3d_FrontMaterial.emissiveColor",    "x3d_EmissiveColor");
	x3d_Shininess        = getUniformLocation (program, "x3d_FrontMaterial.shininess",        "x3d_Shininess");
	x3d_Transparency     = getUniformLocation (program, "x3d_FrontMaterial.transparency",     "x3d_Transparency");

	x3d_BackAmbientIntensity = getUniformLocation (program, "x3d_BackMaterial.ambientIntensity", "x3d_BackAmbientIntensity");
	x3d_BackDiffuseColor     = getUniformLocation (program, "x3d_BackMaterial.diffuseColor",     "x3d_BackDiffuseColor");
	x3d_BackSpecularColor    = getUniformLocation (program, "x3d_BackMaterial.specularColor",    "x3d_BackSpecularColor");
	x3d_BackEmissiveColor    = getUniformLocation (program, "x3d_BackMaterial.emissiveColor",    "x3d_BackEmissiveColor");
	x3d_BackShininess        = getUniformLocation (program, "x3d_BackMaterial.shininess",        "x3d_BackShininess");
	x3d_BackTransparency     = getUniformLocation (program, "x3d_BackMaterial.transparency",     "x3d_BackTransparency");

	x3d_NumTextures           = glGetUniformLocation (program, "x3d_NumTextures");
	x3d_NumProjectiveTextures = glGetUniformLocation (program, "x3d_NumProjectiveTextures");
	x3d_MultiTextureColor     = glGetUniformLocation (program, "x3d_MultiTextureColor");

	for (size_t i = 0, size = getBrowser () -> getMaxTextures (); i < size; ++ i)
	{
		const auto is = basic::to_string (i, std::locale::classic ());

		x3d_TextureType    [i] = glGetUniformLocation (program, ("x3d_TextureType[" + is + "]")    .c_str ());
		x3d_Texture2D      [i] = glGetUniformLocation (program, ("x3d_Texture2D[" + is + "]")      .c_str ());
		x3d_Texture3D      [i] = glGetUniformLocation (program, ("x3d_Texture3D[" + is + "]")      .c_str ());
		x3d_CubeMapTexture [i] = glGetUniformLocation (program, ("x3d_CubeMapTexture[" + is + "]") .c_str ());

		x3d_MultiTextureMode      [i] = glGetUniformLocation (program, ("x3d_MultiTexture[" + is + "].mode")      .c_str ());
		x3d_MultiTextureAlphaMode [i] = glGetUniformLocation (program, ("x3d_MultiTexture[" + is + "].alphaMode") .c_str ());
		x3d_MultiTextureSource    [i] = glGetUniformLocation (program, ("x3d_MultiTexture[" + is + "].source")    .c_str ());
		x3d_MultiTextureFunction  [i] = glGetUniformLocation (program, ("x3d_MultiTexture[" + is + "].function")  .c_str ());

		x3d_TextureCoordinateGeneratorMode      [i] = glGetUniformLocation (program, ("x3d_TextureCoordinateGenerator[" + is + "].mode")      .c_str ());
		x3d_TextureCoordinateGeneratorParameter [i] = glGetUniformLocation (program, ("x3d_TextureCoordinateGenerator[" + is + "].parameter") .c_str ());

		x3d_ProjectiveTexture         [i] = glGetUniformLocation (program, ("x3d_ProjectiveTexture[" + is + "]") .c_str ());
		x3d_ProjectiveTextureMatrix   [i] = glGetUniformLocation (program, ("x3d_ProjectiveTextureMatrix[" + is + "]") .c_str ());
		x3d_ProjectiveTextureLocation [i] = glGetUniformLocation (program, ("x3d_ProjectiveTextureLocation[" + is + "]") .c_str ());

		x3d_TextureMatrix [i] = glGetUniformLocation (program, ("x3d_TextureMatrix[" + is + "]") .c_str ());
		x3d_TexCoord      [i] = getAttribLocation (program, "x3d_TexCoord" + is, i ? "" : "x3d_TexCoord");
	}

	x3d_Viewport          = glGetUniformLocation (program, "x3d_Viewport");
	x3d_ProjectionMatrix  = glGetUniformLocation (program, "x3d_ProjectionMatrix");
	x3d_ModelViewMatrix   = glGetUniformLocation (program, "x3d_ModelViewMatrix");
	x3d_NormalMatrix      = glGetUniformLocation (program, "x3d_NormalMatrix");
	x3d_CameraSpaceMatrix = glGetUniformLocation (program, "x3d_CameraSpaceMatrix");

	x3d_FogDepth = glGetAttribLocation (program, "x3d_FogDepth");
	x3d_Color    = glGetAttribLocation (program, "x3d_Color");
	x3d_Normal   = glGetAttribLocation (program, "x3d_Normal");
	x3d_Vertex   = glGetAttribLocation (program, "x3d_Vertex");

	x3d_ParticleId          = glGetUniformLocation (program, "x3d_Particle.id");
	x3d_ParticleLife        = glGetUniformLocation (program, "x3d_Particle.life");
	x3d_ParticleElapsedTime = glGetUniformLocation (program, "x3d_Particle.elapsedTime");

	static const auto   linetype          = getBrowser () -> getLinetypeUnit ();
	static const auto   hatchStyle        = getBrowser () -> getHatchStyleUnit ();
	static const auto & texture2D         = getBrowser () -> getTexture2DUnits ();
	static const auto & texture3D         = getBrowser () -> getTexture3DUnits ();
	static const auto & cubeMapTexture    = getBrowser () -> getCubeMapTextureUnits ();
	static const auto & projectiveTexture = getBrowser () -> getProjectiveTextureUnits ();
	static const auto   shadowMap         = std::vector <int32_t> (getBrowser () -> getMaxLights (), 0);

	glUniform1i  (x3d_LinePropertiesLinetype,   linetype);
	glUniform1i  (x3d_FillPropertiesHatchStyle, hatchStyle);
	glUniform1i  (x3d_NumTextures,              0);
	glUniform1iv (x3d_Texture2D [0],            texture2D         .size (), texture2D         .data ());
	glUniform1iv (x3d_Texture3D [0],            texture3D         .size (), texture3D         .data ());
	glUniform1iv (x3d_CubeMapTexture [0],       cubeMapTexture    .size (), cubeMapTexture    .data ());
	glUniform1iv (x3d_ShadowMap [0],            shadowMap         .size (), shadowMap         .data ());

	if (getBrowser () -> getProjectiveTextureMapping ())
		glUniform1iv (x3d_ProjectiveTexture [0], projectiveTexture .size (), projectiveTexture .data ());
}

GLint
X3DProgrammableShaderObject::getUniformLocation (GLuint program, const std::string & name, const std::string & depreciated) const
{
	// Legacy function to get uniform location.

	auto location = glGetUniformLocation (program, name .c_str ());

	if (location not_eq -1)
		return location;

	// Look for depreciated location.

	if (depreciated .size ())
	{
		location = glGetUniformLocation (program, depreciated .c_str ());

		if (location not_eq -1)
			getBrowser () -> getConsole () -> warn ("Using uniform location name »", depreciated, "« is depreciated, use »", name, "«. See http://create3000.de/x_ite/custom-shaders/.\n");

		return location;
	}

	return -1;
}

GLint
X3DProgrammableShaderObject::getAttribLocation (GLuint program, const std::string & name, const std::string & depreciated) const
{
	// Legacy function to get uniform location.

	auto location = glGetAttribLocation (program, name .c_str ());

	if (location not_eq -1)
		return location;

	// Look for depreciated location.

	if (depreciated .size ())
	{
		location = glGetAttribLocation (program, depreciated .c_str ());

		if (location not_eq -1)
			getBrowser () -> getConsole () -> warn ("Using uniform attribute name »", depreciated, "« is depreciated, use »", name, "«. See http://create3000.de/x_ite/custom-shaders/.\n");

		return location;
	}

	return -1;
}

/*
 *
 *  Fields
 *
 */

void
X3DProgrammableShaderObject::addShaderFields ()
{
	textures .clear ();

	for (const auto & field : getUserDefinedFields ())
	{
		field -> addInterest (&X3DProgrammableShaderObject::set_field, this, field);

		set_field (field);
	}
}

///  throws Error <INVALID_NAME>, Error <INVALID_FIELD>, Error <DISPOSED>
void
X3DProgrammableShaderObject::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
{
	X3DBaseNode::addUserDefinedField (accessType, name, field);

	if (isInitialized ())
	{
		field -> addInterest (&X3DProgrammableShaderObject::set_field, this, field);

		set_field (field);
	}
}

///  throws Error <DISPOSED>
void
X3DProgrammableShaderObject::removeUserDefinedField (const std::string & name)
{
	try
	{
		const auto field = getField (name);

		switch (field -> getType ())
		{
			case X3DConstants::SFNode:
			{
				const GLint location = glGetUniformLocation (getProgramId (), field -> getName () .c_str ());

				textures .erase (location);
				break;
			}
			case X3DConstants::MFNode:
			{
				const auto & mfnode = *static_cast <MFNode*> (field);

				for (size_t i = 0, size = mfnode .size (); i < size; ++ i)
				{
					const auto location = glGetUniformLocation (getProgramId (), (field -> getName () + "[" + basic::to_string (i, std::locale::classic ()) + "]") .c_str ());

					textures .erase (location);
				}

				break;
			}
			default:
				break;
		}

		field -> removeInterest (&X3DProgrammableShaderObject::set_field, this);
	}
	catch (const X3DError &)
	{ }

	X3DBaseNode::removeUserDefinedField (name);
}

void
X3DProgrammableShaderObject::set_field (X3DFieldDefinition* const field)
{
	const GLint location = glGetUniformLocation (getProgramId (), field -> getName () .c_str ());

	if (location == -1)
	{
		#ifdef TITANIA_DEBUG
		//getBrowser () -> println ("Warning: Uniform variable '", field -> getName (), "' not found.");

		//if (not getProgramId ())
		//	getBrowser () -> println ("Warning: Couldn't allocate shader.");
		#endif
		return;
	}

	switch (field -> getType ())
	{
		case X3DConstants::SFBool:
		{
			glUniform1i (location, static_cast <const SFBool*> (field) -> getValue ());
			break;
		}
		case X3DConstants::SFColor:
		{
			glUniform3fv (location, 1, static_cast <const SFColor*> (field) -> getValue () .data ());
			break;
		}
		case X3DConstants::SFColorRGBA:
		{
			glUniform4fv (location, 1, static_cast <const SFColorRGBA*> (field) -> getValue () .data ());
			break;
		}
		case X3DConstants::SFDouble:
		{
			if (extensionGPUShaderFP64)
				glUniform1d (location, static_cast <const SFDouble*> (field) -> getValue ());
			else
				glUniform1f (location, static_cast <const SFDouble*> (field) -> getValue ());

			break;
		}
		case X3DConstants::SFFloat:
		{
			glUniform1f (location, static_cast <const SFFloat*> (field) -> getValue ());
			break;
		}
		case X3DConstants::SFInt32:
		{
			glUniform1i (location, static_cast <const SFInt32*> (field) -> getValue ());
			break;
		}
		case X3DConstants::SFImage:
		{
			const auto image = static_cast <const SFImage*> (field);

			std::vector <int32_t> vector;

			vector .reserve (3 + image -> getArray () .size ());

			vector .emplace_back (image -> getWidth ());
			vector .emplace_back (image -> getHeight ());
			vector .emplace_back (image -> getComponents ());

			for (const auto & value : image -> getArray ())
				vector .emplace_back (value);

			glUniform1iv (location, vector .size (), vector .data ());
			break;
		}
		case X3DConstants::SFMatrix3d:
		{
			if (extensionGPUShaderFP64)
				glUniformMatrix3dv (location, 1, false, static_cast <const SFMatrix3d*> (field) -> getValue () .front () .data ());
			else
				glUniformMatrix3fv (location, 1, false, Matrix3f (static_cast <const SFMatrix3d*> (field) -> getValue ()) .front () .data ());

			break;
		}
		case X3DConstants::SFMatrix3f:
		{
			glUniformMatrix3fv (location, 1, false, static_cast <const SFMatrix3f*> (field) -> getValue () .front () .data ());
			break;
		}
		case X3DConstants::SFMatrix4d:
		{
			if (extensionGPUShaderFP64)
				glUniformMatrix4dv (location, 1, false, static_cast <const SFMatrix4d*> (field) -> getValue () .front () .data ());
			else
				glUniformMatrix4fv (location, 1, false, Matrix4f (static_cast <const SFMatrix4d*> (field) -> getValue ()) .front () .data ());

			break;
		}
		case X3DConstants::SFMatrix4f:
		{
			glUniformMatrix4fv (location, 1, false, static_cast <const SFMatrix4f*> (field) -> getValue () .front () .data ());
			break;
		}
		case X3DConstants::SFNode:
		{
			const auto & sfnode  = *static_cast <SFNode*> (field);
			const auto   texture = x3d_cast <X3DTextureNode*> (sfnode);

			if (texture and not x3d_cast <MultiTexture*> (sfnode))
			{
				textures [location] = std::make_shared <TextureValue> (field -> getName (), texture -> getTarget (), texture -> getTextureId (), -1);
				break;
			}

			textures .erase (location);
			break;
		}
		case X3DConstants::SFRotation:
		{
			if (extensionGPUShaderFP64)
				glUniformMatrix3dv (location, 1, false, static_cast <const SFRotation*> (field) -> getValue () .matrix () .front () .data ());
			else
				glUniformMatrix3fv (location, 1, false, Matrix3f (static_cast <const SFRotation*> (field) -> getValue () .matrix ()) .front () .data ());

			break;
		}
		case X3DConstants::SFString:
		{
			break;
		}
		case X3DConstants::SFTime:
		{
			if (extensionGPUShaderFP64)
				glUniform1d (location, static_cast <const SFTime*> (field) -> getValue ());
			else
				glUniform1f (location, static_cast <const SFTime*> (field) -> getValue ());

			break;
		}
		case X3DConstants::SFVec2d:
		{
			if (extensionGPUShaderFP64)
				glUniform2dv (location, 1, static_cast <const SFVec2d*> (field) -> getValue () .data ());
			else
				glUniform2fv (location, 1, Vector2f (static_cast <const SFVec2d*> (field) -> getValue ()) .data ());

			break;
		}
		case X3DConstants::SFVec2f:
		{
			glUniform2fv (location, 1, static_cast <const SFVec2f*> (field) -> getValue () .data ());
			break;
		}
		case X3DConstants::SFVec3d:
		{
			if (extensionGPUShaderFP64)
				glUniform3dv (location, 1, static_cast <const SFVec3d*> (field) -> getValue () .data ());
			else
				glUniform3fv (location, 1, Vector3f (static_cast <const SFVec3d*> (field) -> getValue ()) .data ());

			break;
		}
		case X3DConstants::SFVec3f:
		{
			glUniform3fv (location, 1, static_cast <const SFVec3f*> (field) -> getValue () .data ());
			break;
		}
		case X3DConstants::SFVec4d:
		{
			if (extensionGPUShaderFP64)
				glUniform4dv (location, 1, static_cast <const SFVec4d*> (field) -> getValue () .data ());
			else
				glUniform4fv (location, 1, Vector4f (static_cast <const SFVec4d*> (field) -> getValue ()) .data ());

			break;
		}
		case X3DConstants::SFVec4f:
		{
			glUniform4fv (location, 1, static_cast <const SFVec4f*> (field) -> getValue () .data ());
			break;
		}
		case X3DConstants::MFBool:
		{
			const auto array = static_cast <const MFBool*> (field);

			if (array -> empty ())
				break;

			std::vector <int32_t> vector (array -> cbegin (), array -> cend ());
			glUniform1iv (location, vector .size (), vector .data ());
			break;
		}
		case X3DConstants::MFColor:
		{
			const auto array = static_cast <const MFColor*> (field);

			if (array -> empty ())
				break;

			glUniform3fv (location, array -> size (), array -> at (0) .data ());
			break;
		}
		case X3DConstants::MFColorRGBA:
		{
			const auto array = static_cast <const MFColorRGBA*> (field);

			if (array -> empty ())
				break;

			glUniform4fv (location, array -> size (), array -> at (0) .data ());
			break;
		}
		case X3DConstants::MFDouble:
		{
			const auto array = static_cast <const MFDouble*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				glUniform1dv (location, array -> getValue () .size (), array -> getValue () .data ());
			}
			else
			{
				std::vector <float> vector (array -> cbegin (), array -> cend ());
				glUniform1fv (location, vector .size (), vector .data ());
			}

			break;
		}
		case X3DConstants::MFFloat:
		{
			const auto array = static_cast <const MFFloat*> (field);

			if (array -> empty ())
				break;

			glUniform1fv (location, array -> getValue () .size (), array -> getValue () .data ());
			break;
		}
		case X3DConstants::MFImage:
		{
			const auto array = static_cast <const MFImage*> (field);

			if (array -> empty ())
				break;

			std::vector <int32_t> vector;

			for (const auto & image : *array)
			{
				vector .emplace_back (image .getWidth ());
				vector .emplace_back (image .getHeight ());
				vector .emplace_back (image .getComponents ());

				for (const auto & value : image .getArray ())
					vector .emplace_back (value);
			}

			glUniform1iv (location, vector .size (), vector .data ());
			break;
		}
		case X3DConstants::MFInt32:
		{
			const auto array = static_cast <const MFInt32*> (field);

			if (array -> empty ())
				break;

			glUniform1iv (location, array -> getValue () .size (), array -> getValue () .data ());
			break;
		}
		case X3DConstants::MFMatrix3d:
		{
			const auto array = static_cast <const MFMatrix3d*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				std::vector <Matrix3d> vector (array -> cbegin (), array -> cend ());
				glUniformMatrix3dv (location, vector .size (), false, vector [0] .front () .data ());
			}
			else
			{
				std::vector <Matrix3f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value);

				glUniformMatrix3fv (location, vector .size (), false, vector [0] .front () .data ());
			}

			break;
		}
		case X3DConstants::MFMatrix3f:
		{
			const auto array = static_cast <const MFMatrix3f*> (field);

			if (array -> empty ())
				break;

			std::vector <Matrix3f> vector (array -> cbegin (), array -> cend ());
			glUniformMatrix3fv (location, vector .size (), false, vector [0] .front () .data ());
			break;
		}
		case X3DConstants::MFMatrix4d:
		{
			const auto array = static_cast <const MFMatrix4d*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				glUniformMatrix4dv (location, array -> size (), false, array -> at (0) .front () .data ());
			}
			else
			{
				std::vector <Matrix4f> vector (array -> cbegin (), array -> cend ());
				glUniformMatrix4fv (location, vector .size (), false, vector [0] .front () .data ());
			}

			break;
		}
		case X3DConstants::MFMatrix4f:
		{
			const auto array = static_cast <const MFMatrix4f*> (field);

			if (array -> empty ())
				break;

			glUniformMatrix4fv (location, array -> size (), false, array -> at (0) .front () .data ());
			break;
		}
		case X3DConstants::MFNode:
		{
			const auto & mfnode = *static_cast <MFNode*> (field);

			for (size_t i = 0, size = mfnode .size (); i < size; ++ i)
			{
				const auto location = glGetUniformLocation (getProgramId (), (field -> getName () + "[" + basic::to_string (i, std::locale::classic ()) + "]") .c_str ());

				if (location == -1)
					break;

				const auto & sfnode  = mfnode [i];
				const auto   texture = x3d_cast <X3DTextureNode*> (sfnode);

				if (texture and not x3d_cast <MultiTexture*> (sfnode))
				{
					textures [location] = std::make_shared <TextureValue> (field -> getName (), texture -> getTarget (), texture -> getTextureId (), -1);
					continue;
				}

				textures .erase (location);
			}

			break;
		}
		case X3DConstants::MFRotation:
		{
			const auto array = static_cast <const MFRotation*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				std::vector <Matrix3d> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value .matrix ());

				glUniformMatrix3dv (location, vector .size (), false, vector [0] .front () .data ());
			}
			else
			{
				std::vector <Matrix3f> vector;
				vector .reserve (array -> size ());

				for (const auto & value : *array)
					vector .emplace_back (value .matrix ());

				glUniformMatrix3fv (location, vector .size (), false, vector [0] .front () .data ());
			}

			break;
		}
		case X3DConstants::MFString:
		{
			break;
		}
		case X3DConstants::MFTime:
		{
			const auto array = static_cast <const MFTime*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				glUniform1dv (location, array -> getValue () .size (), array -> getValue () .data ());
			}
			else
			{
				std::vector <float> vector (array -> cbegin (), array -> cend ());
				glUniform1fv (location, vector .size (), vector .data ());
			}

			break;
		}
		case X3DConstants::MFVec2d:
		{
			const auto array = static_cast <const MFVec2d*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				glUniform2dv (location, array -> size (), array -> at (0) .data ());
			}
			else
			{
				std::vector <Vector2f> vector (array -> cbegin (), array -> cend ());
				glUniform2fv (location, vector .size (), vector [0] .data ());
			}

			break;
		}
		case X3DConstants::MFVec2f:
		{
			const auto array = static_cast <const MFVec2f*> (field);

			if (array -> empty ())
				break;

			glUniform2fv (location, array -> size (), array -> at (0) .data ());
			break;
		}
		case X3DConstants::MFVec3d:
		{
			const auto array = static_cast <const MFVec3d*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				glUniform3dv (location, array -> size (), array -> at (0) .data ());
			}
			else
			{
				std::vector <Vector3f> vector (array -> cbegin (), array -> cend ());
				glUniform3fv (location, vector .size (), vector [0] .data ());
			}

			break;
		}
		case X3DConstants::MFVec3f:
		{
			const auto array = static_cast <const MFVec3f*> (field);

			if (array -> empty ())
				break;

			glUniform3fv (location, array -> size (), array -> at (0) .data ());
			break;
		}
		case X3DConstants::MFVec4d:
		{
			const auto array = static_cast <const MFVec4d*> (field);

			if (array -> empty ())
				break;

			if (extensionGPUShaderFP64)
			{
				glUniform4dv (location, array -> size (), array -> at (0) .data ());
			}
			else
			{
				std::vector <Vector4f> vector (array -> cbegin (), array -> cend ());
				glUniform4fv (location, vector .size (), vector [0] .data ());
			}

			break;
		}
		case X3DConstants::MFVec4f:
		{
			const auto array = static_cast <const MFVec4f*> (field);

			if (array -> empty ())
				break;

			glUniform4fv (location, array -> size (), array -> at (0) .data ());
			break;
		}
	}
}

void
X3DProgrammableShaderObject::setTextureBuffer (const std::string & name, const GLenum textureTarget, const GLuint textureId)
{
	const GLint location = glGetUniformLocation (getProgramId (), name .c_str ());

	if (location not_eq -1)
	{
		textures [location] = std::make_shared <TextureValue> (name, textureTarget, textureId, -1);
	}
	else
	{
		#ifdef TITANIA_DEBUG
		getBrowser () -> getConsole () -> error ("Error: Uniform variable '", name, "' not found.");
		#endif
	}
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

bool
X3DProgrammableShaderObject::hasFog (FogContainer* const fogContainer)
{
	if (this -> fogContainer == fogContainer)
		return true;

	this -> fogContainer = fogContainer;

	return false;
}

bool
X3DProgrammableShaderObject::hasLight (const size_t index, LightContainer* const lightContainer)
{
	if (index < lightContainers .size ())
	{
		if (lightContainers [index] == lightContainer)
			return true;

		lightContainers [index] = lightContainer;

		return false;
	}
	else
	{
		return true;
	}
}

bool
X3DProgrammableShaderObject::hasProjectiveTexture (const size_t index, ProjectiveTextureContainer* const projectiveTextureContainer)
{
	if (index < projectiveTextureContainers .size ())
	{
		if (projectiveTextureContainers [index] == projectiveTextureContainer)
			return true;

		projectiveTextureContainers [index] = projectiveTextureContainer;

		return false;
	}
	else
	{
		return true;
	}
}

void
X3DProgrammableShaderObject::setGlobalUniforms (X3DRenderObject* const renderObject)
{
	const auto & browser                  = renderObject -> getBrowser ();
	const auto & cameraSpaceMatrix        = renderObject -> getCameraSpaceMatrix () .get ();
	const auto & projectionMatrix         = renderObject -> getProjectionMatrix () .get ();
	const auto & globalLights             = renderObject -> getGlobalLights ();
	const auto & globalProjectiveTextures = renderObject -> getGlobalProjectiveTextures ();

	// Set viewport.

	glUniform4iv (x3d_Viewport, 1, renderObject -> getViewVolumes () .back () .getViewport () .data ());

	// Set projection matrix.

	if (extensionGPUShaderFP64)
	{
		glUniformMatrix4dv (x3d_ProjectionMatrix,  1, false, projectionMatrix  .front () .data ());
		glUniformMatrix4dv (x3d_CameraSpaceMatrix, 1, false, cameraSpaceMatrix .front () .data ());
	}
	else
	{
		glUniformMatrix4fv (x3d_ProjectionMatrix,  1, false, Matrix4f (projectionMatrix)  .front () .data ());
		glUniformMatrix4fv (x3d_CameraSpaceMatrix, 1, false, Matrix4f (cameraSpaceMatrix) .front () .data ());
	}

	// Fog

	fogContainer = nullptr;

	// Set global lights.

	numGlobalLights = std::min (browser -> getMaxLights (), globalLights .size ());

	for (size_t i = 0; i < numGlobalLights; ++ i)
		globalLights [i] -> setShaderUniforms (renderObject, this, i);

	std::fill (lightContainers .begin (), lightContainers .end (), nullptr);

	// Set global projective textures.

	numGlobalProjectiveTextures = std::min (browser -> getMaxTextures (), globalProjectiveTextures .size ());

	for (size_t i = 0; i < numGlobalProjectiveTextures; ++ i)
		globalProjectiveTextures [i] -> setShaderUniforms (renderObject, this, i);

	std::fill (projectiveTextureContainers .begin (), projectiveTextureContainers .end (), nullptr);

	// Logarithmic depth buffer support.

	const auto viewpoint      = renderObject -> getViewpoint ();
	const auto navigationInfo = renderObject -> getNavigationInfo ();

	if (dynamic_cast <OrthoViewpoint*> (viewpoint))
		glUniform1f (x3d_LogarithmicFarFactor1_2, -1);
	else
		glUniform1f (x3d_LogarithmicFarFactor1_2, 1 / std::log2 (navigationInfo -> getFarValue (viewpoint) + 1));
}

void
X3DProgrammableShaderObject::setLocalUniforms (ShapeContainer* const context)
{
	const auto   browser               = context -> getBrowser ();
	const auto   renderObject          = context -> getRenderer ();
	const auto & clipPlanes            = context -> getClipPlanes ();
	const auto   stylePropertiesNode   = context -> getStyleProperties ();
	const auto   materialNode          = context -> getMaterial ();
	const auto   textureNode           = context -> getTexture ();
	const auto   textureTransformNode  = context -> getTextureTransform ();
	const auto   textureCoordinateNode = context -> getTextureCoordinate ();
	const auto & modelViewMatrix       = context -> getModelViewMatrix ();
	const auto   normalMatrix          = getNormalMatrix (modelViewMatrix); // Transposed when uniform is set.

	// Geometry type

	glUniform1i (x3d_GeometryType, context -> getGeometryType ());

	// Clip planes

	setClipPlanes (browser, clipPlanes);

	// Lights

	const auto & localLights = context -> getLocalLights ();
	const auto   numLights   = std::min (browser -> getMaxLights (), numGlobalLights + localLights .size ());

	for (size_t i = numGlobalLights, l = 0; i < numLights; ++ i, ++ l)
		localLights [l] -> setShaderUniforms (renderObject, this, i);

	glUniform1i (x3d_NumLights, numLights);

	// Legacy

	if (numLights < browser -> getMaxLights ())
		glUniform1i (x3d_LightType [numLights], 0);

	// Projective Textures

	const auto & localProjectiveTextures = context -> getLocalProjectiveTextures ();
	const auto   numProjectiveTextures   = std::min (browser -> getMaxTextures (), numGlobalProjectiveTextures + localProjectiveTextures .size ());

	for (size_t i = numGlobalProjectiveTextures, l = 0; i < numProjectiveTextures; ++ i, ++ l)
		localProjectiveTextures [l] -> setShaderUniforms (renderObject, this, i);

	glUniform1i (x3d_NumProjectiveTextures, numProjectiveTextures);

	// Fog

	context -> getFog () -> setShaderUniforms (this);
	glUniform1i (x3d_FogCoord, context -> getFogCoord ());

	// Appearance

	glUniform1i (x3d_ColorMaterial, context -> getColorMaterial ());

	stylePropertiesNode -> setShaderUniforms (this);

	if (materialNode)
	{
		glUniform1i (x3d_Lighting, true);

		// Material

		materialNode -> setShaderUniforms (this);
	}
	else
		glUniform1i (x3d_Lighting, false);

	if (textureNode)
		textureNode -> setShaderUniforms (this);
	else
		glUniform1i (x3d_NumTextures, 0);

	// Matrices

	textureTransformNode  -> setShaderUniforms (this);
	textureCoordinateNode -> setShaderUniforms (this);

	if (extensionGPUShaderFP64)
	{
		glUniformMatrix3dv (x3d_NormalMatrix,    1, true,  normalMatrix    .front () .data ());
		glUniformMatrix4dv (x3d_ModelViewMatrix, 1, false, modelViewMatrix .front () .data ());
	}
	else
	{
		glUniformMatrix3fv (x3d_NormalMatrix,    1, true,  Matrix3f (normalMatrix)    .front () .data ());
		glUniformMatrix4fv (x3d_ModelViewMatrix, 1, false, Matrix4f (modelViewMatrix) .front () .data ());
	}
}

Matrix3d
X3DProgrammableShaderObject::getNormalMatrix (const Matrix4d & modelViewMatrix) const
{
	try
	{
		// Transposed when uniform is set.
		return inverse (modelViewMatrix .submatrix ());
	}
	catch (const std::domain_error & error)
	{
		return Matrix3f ();
	}
}

void
X3DProgrammableShaderObject::setClipPlanes (const X3DBrowser* const browser, const ClipPlaneContainerArray & clipPlanes)
{
	const auto numClipPlanes = std::min (browser -> getMaxClipPlanes (), clipPlanes .size ());

	for (size_t i = 0; i < numClipPlanes; ++ i)
		clipPlanes [i] -> setShaderUniforms (this, i);

	glUniform1i (x3d_NumClipPlanes, numClipPlanes);

	// Legacy

	if (numClipPlanes < browser -> getMaxClipPlanes ())
		glUniform4f (x3d_ClipPlane [numClipPlanes], 88, 51, 68, 33);
}

void
X3DProgrammableShaderObject::enable ()
{
	for (const auto & pair : textures)
	{
		const auto location      = pair .first;
		const auto object        = pair .second;
		const auto textureTarget = object -> textureTarget;
		const auto textureId     = object -> textureId;

		if (getBrowser () -> getCombinedTextureUnits () .empty ())
		{
			getBrowser () -> getConsole () -> warn ("Warning: Not enough combined texture units for uniform variable '", object -> name, "' available.");
			continue;
		}

		const auto textureUnit = object -> textureUnit = getBrowser () -> getCombinedTextureUnits () .top ();

		getBrowser () -> getCombinedTextureUnits () .pop ();

		glUniform1i (location, textureUnit);
		glActiveTexture (GL_TEXTURE0 + textureUnit);
		glBindTexture (textureTarget, textureId);
	}

	glActiveTexture (GL_TEXTURE0);
}

void
X3DProgrammableShaderObject::disable ()
{
	for (const auto & pair : textures)
	{
		const auto object      = pair .second;
		const auto textureUnit = object -> textureUnit;

		if (textureUnit not_eq -1)
			getBrowser () -> getCombinedTextureUnits () .push (textureUnit);

		object -> textureUnit = -1;
	}
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
X3DProgrammableShaderObject::enableIntegerAttrib (const std::string & name, const GLuint buffer, const size_t components)
{
	const auto location = glGetAttribLocation (getProgramId (), name .c_str ());

	if (location == -1)
		return;

	glEnableVertexAttribArray (location);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (location, components, GL_INT, false, 0, (void*) 0);
}

void
X3DProgrammableShaderObject::disableIntegerAttrib (const std::string & name)
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
	const auto location = glGetAttribLocation (getProgramId (), name .c_str ());

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

void
X3DProgrammableShaderObject::enableFogDepthAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer)
{
	if (x3d_FogDepth == -1)
		return;

	glEnableVertexAttribArray (x3d_FogDepth);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (x3d_FogDepth, 1, type, false, stride, pointer);
}

void
X3DProgrammableShaderObject::disableFogDepthAttrib ()
{
	if (x3d_FogDepth == -1)
		return;

	glDisableVertexAttribArray (x3d_FogDepth);
}

void
X3DProgrammableShaderObject::enableColorAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer)
{
	if (x3d_Color == -1)
		return;

	glEnableVertexAttribArray (x3d_Color);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (x3d_Color, 4, type, false, stride, pointer);
}

void
X3DProgrammableShaderObject::disableColorAttrib ()
{
	if (x3d_Color == -1)
		return;

	glDisableVertexAttribArray (x3d_Color);
}

void
X3DProgrammableShaderObject::enableTexCoordAttrib (const std::vector <GLuint> & buffer,
                                                   const GLenum type,
                                                   const std::vector <GLsizei> & stride,
                                                   const std::vector <GLvoid*> & pointer)
{
	const auto channels = std::min (buffer .size (), x3d_TexCoord .size ());

	for (size_t i = 0; i < channels; ++ i)
	{
		if (x3d_TexCoord [i] == -1)
			continue;

		glEnableVertexAttribArray (x3d_TexCoord [i]);

		glBindBuffer (GL_ARRAY_BUFFER, buffer [i]);
		glVertexAttribPointer (x3d_TexCoord [i], 4, type, false, stride .empty () ? 0 : stride [i], pointer .empty () ? nullptr : pointer [i]);
	}
}

void
X3DProgrammableShaderObject::disableTexCoordAttrib ()
{
	for (size_t i = 0, size = x3d_TexCoord .size (); i < size; ++ i)
	{
		if (x3d_TexCoord [i] == -1)
			return;

		glDisableVertexAttribArray (x3d_TexCoord [i]);
	}
}

void
X3DProgrammableShaderObject::enableNormalAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer)
{
	if (x3d_Normal == -1)
		return;

	glEnableVertexAttribArray (x3d_Normal);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (x3d_Normal, 3, type, false, stride, pointer);
}

void
X3DProgrammableShaderObject::disableNormalAttrib ()
{
	if (x3d_Normal == -1)
		return;

	glDisableVertexAttribArray (x3d_Normal);
}

void
X3DProgrammableShaderObject::enableVertexAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer)
{
	if (x3d_Vertex == -1)
		return;

	glEnableVertexAttribArray (x3d_Vertex);

	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer (x3d_Vertex, 3, type, false, stride, pointer);
}

void
X3DProgrammableShaderObject::disableVertexAttrib ()
{
	if (x3d_Vertex == -1)
		return;

	glDisableVertexAttribArray (x3d_Vertex);
}

void
X3DProgrammableShaderObject::setParticle (const size_t id, const SoftParticle & particle, const Matrix4d & modelViewMatrix)
{
	const auto normalMatrix = getNormalMatrix (modelViewMatrix);

	if (extensionGPUShaderFP64)
	{
		glUniformMatrix3dv (x3d_NormalMatrix,    1, true,  normalMatrix    .front () .data ());
		glUniformMatrix4dv (x3d_ModelViewMatrix, 1, false, modelViewMatrix .front () .data ());
	}
	else
	{
		glUniformMatrix3fv (x3d_NormalMatrix,    1, true,  Matrix3f (normalMatrix)    .front () .data ());
		glUniformMatrix4fv (x3d_ModelViewMatrix, 1, false, Matrix4f (modelViewMatrix) .front () .data ());
	}

	glUniform1i (x3d_ParticleId,          id);
	glUniform1i (x3d_ParticleLife,        particle .life);
	glUniform1f (x3d_ParticleElapsedTime, particle .elapsedTime / particle .lifetime);
}

/*
 *
 *  Destruction
 *
 */

void
X3DProgrammableShaderObject::dispose ()
{ }

} // X3D
} // titania

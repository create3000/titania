/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra?e 31a, Leipzig, Germany 2011.
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

#include "SpotLight.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/LightContainer.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/TextureBuffer.h"
#include "../../Rendering/X3DRenderObject.h"

#include "../Navigation/X3DViewpointNode.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

static constexpr int32_t SPOT_LIGHT = 3;

const Component   SpotLight::component      = Component ("Lighting", 2);
const std::string SpotLight::typeName       = "SpotLight";
const std::string SpotLight::containerField = "children";

SpotLight::Fields::Fields () :
	attenuation (new SFVec3f (1, 0, 0)),
	  direction (new SFVec3f (0, 0, -1)),
	   location (new SFVec3f ()),
	     radius (new SFFloat (100)),
	  beamWidth (new SFFloat (0.7854)),
	cutOffAngle (new SFFloat (1.5708))
{ }

SpotLight::SpotLight (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLightNode (),
	      fields ()
{
	addType (X3DConstants::SpotLight);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "global",           global ());
	addField (inputOutput, "on",               on ());
	addField (inputOutput, "color",            color ());
	addField (inputOutput, "intensity",        intensity ());
	addField (inputOutput, "ambientIntensity", ambientIntensity ());
	addField (inputOutput, "attenuation",      attenuation ());
	addField (inputOutput, "location",         location ());
	addField (inputOutput, "direction",        direction ());
	addField (inputOutput, "radius",           radius ());
	addField (inputOutput, "beamWidth",        beamWidth ());
	addField (inputOutput, "cutOffAngle",      cutOffAngle ());

	addField (inputOutput,    "shadowColor",     shadowColor ());
	addField (inputOutput,    "shadowIntensity", shadowIntensity ());
	addField (inputOutput,    "shadowBias",      shadowBias ());
	addField (initializeOnly, "shadowMapSize",   shadowMapSize ());

	location ()    .setUnit (UnitCategory::LENGTH);
	radius ()      .setUnit (UnitCategory::LENGTH);
	beamWidth ()   .setUnit (UnitCategory::ANGLE);
	cutOffAngle () .setUnit (UnitCategory::ANGLE);
}

X3DBaseNode*
SpotLight::create (X3DExecutionContext* const executionContext) const
{
	return new SpotLight (executionContext);
}

void
SpotLight::initialize ()
{
	X3DLightNode::initialize ();

	addInterest (&SpotLight::eventsProcessed, this);

	eventsProcessed ();
}

float
SpotLight::getRadius () const
{
	return std::max <float> (0, radius ());
}

float
SpotLight::getBeamWidth () const
{
	// If the beamWidth is greater than the cutOffAngle, beamWidth is defined to be equal to the cutOffAngle.

	const auto cutOffAngle = getCutOffAngle ();

	if (beamWidth () > cutOffAngle)
		return cutOffAngle;

	return std::clamp <float> (beamWidth (), 0, pi <float> / 2);
}

float
SpotLight::getCutOffAngle () const
{
	return std::clamp <float> (cutOffAngle (), 0, pi <float> / 2);
}

void
SpotLight::eventsProcessed ()
{
	const auto ambientIntensity = getAmbientIntensity ();
	const auto intensity        = getIntensity ();
	const auto beamWidth        = getBeamWidth ();

	glAmbient [0] = ambientIntensity * color () .getRed ();
	glAmbient [1] = ambientIntensity * color () .getGreen ();
	glAmbient [2] = ambientIntensity * color () .getBlue ();
	glAmbient [3] = 1;

	glDiffuseSpecular [0] = intensity * color () .getRed ();
	glDiffuseSpecular [1] = intensity * color () .getGreen ();
	glDiffuseSpecular [2] = intensity * color () .getBlue ();
	glDiffuseSpecular [3] = 1;

	glSpotExponent = std::clamp <float> (beamWidth ? 0.5f / beamWidth : 0.0f, 0, 128);
	glSpotCutOff   = std::clamp <float> (math::degrees (getCutOffAngle ()), 0, 90);

	glPosition [0] = location () .getX ();
	glPosition [1] = location () .getY ();
	glPosition [2] = location () .getZ ();
	glPosition [3] = 1; // point light

	glSpotDirection [0] = direction () .getX ();
	glSpotDirection [1] = direction () .getY ();
	glSpotDirection [2] = direction () .getZ ();
}

void
SpotLight::draw (GLenum lightId)
{
	glLightfv (lightId, GL_AMBIENT,  glAmbient);
	glLightfv (lightId, GL_DIFFUSE,  glDiffuseSpecular);
	glLightfv (lightId, GL_SPECULAR, glDiffuseSpecular);

	glLightf (lightId, GL_SPOT_EXPONENT, glSpotExponent);
	glLightf (lightId, GL_SPOT_CUTOFF,   glSpotCutOff);

	glLightf (lightId, GL_CONSTANT_ATTENUATION,  std::max (0.0f, attenuation () .getX ()));
	glLightf (lightId, GL_LINEAR_ATTENUATION,    std::max (0.0f, attenuation () .getY ()));
	glLightf (lightId, GL_QUADRATIC_ATTENUATION, std::max (0.0f, attenuation () .getZ ()));

	glLightfv (lightId, GL_POSITION,       glPosition);
	glLightfv (lightId, GL_SPOT_DIRECTION, glSpotDirection);
}

bool
SpotLight::renderShadowMap (X3DRenderObject* const renderObject, LightContainer* const lightContainer)
{
	try
	{
		using namespace std::placeholders;

		const auto modelMatrix         = lightContainer -> getModelViewMatrix () .get () * renderObject -> getCameraSpaceMatrix () .get ();
		auto       invLightSpaceMatrix = global () ? modelMatrix : Matrix4d ();

		invLightSpaceMatrix .translate (location () .getValue ());
		invLightSpaceMatrix .rotate (Rotation4d (Vector3d (0, 0, 1), negate (Vector3d (direction () .getValue ()))));
		invLightSpaceMatrix .inverse ();

		const auto & shadowTextureBuffer = lightContainer -> getShadowTextureBuffer ();
		const auto   groupNode           = lightContainer -> getGroup ();                                          // Group to be shadowd
		const auto   groupBBox           = groupNode -> getSubBBox ();                                             // Group bbox.
		const auto   lightBBox           = groupBBox * invLightSpaceMatrix;                                        // Group bbox from the perspective of the light.
		const auto   lightBBoxExtents    = lightBBox .extents ();                                                  // Group bbox from the perspective of the light.
		const auto   farValue            = std::min <double> (getRadius (), -lightBBoxExtents .first .z ());
		const auto   viewport            = Vector4i (0, 0, getShadowMapSize (), getShadowMapSize ());
		const auto   projectionMatrix    = camera <double>::perspective (getCutOffAngle () * 2, 0.125, farValue, viewport [2], viewport [3]);

		if (farValue < 0)
			return false;

		renderObject -> getBrowser () -> getDisplayTools () .push (false);

		shadowTextureBuffer -> bind ();

		renderObject -> getCameraSpaceMatrix        () .push (renderObject -> getViewpoint () -> getCameraSpaceMatrix ());
		renderObject -> getInverseCameraSpaceMatrix () .push (renderObject -> getViewpoint () -> getInverseCameraSpaceMatrix ());
		renderObject -> getViewVolumes              () .emplace_back (projectionMatrix, viewport, viewport);
		renderObject -> getProjectionMatrix         () .push (projectionMatrix);
		renderObject -> getModelViewMatrix          () .push (invLightSpaceMatrix);

		renderObject -> render (TraverseType::DEPTH, [&groupNode]
		(const TraverseType type, X3DRenderObject* const renderObject)
		{
			groupNode -> X3DGroupingNode::traverse (type, renderObject);
		});

		renderObject -> getModelViewMatrix          () .pop ();
		renderObject -> getProjectionMatrix         () .pop ();
		renderObject -> getViewVolumes              () .pop_back ();
		renderObject -> getInverseCameraSpaceMatrix () .pop ();
		renderObject -> getCameraSpaceMatrix        () .pop ();

		shadowTextureBuffer -> unbind ();
	
		renderObject -> getBrowser () -> getDisplayTools () .pop ();

		//#define DEBUG_SPOT_LIGHT_SHADOW_BUFFER
		#ifdef  DEBUG_SPOT_LIGHT_SHADOW_BUFFER
		#ifdef  TITANIA_DEBUG
		// Disable background in renderer when debug framebuffer.
		{
			const auto viewport = Vector4i (0, 0, 100, 100);

			FrameBuffer frameBuffer (renderObject -> getBrowser (), 100, 100, 4);
	
			frameBuffer .setup ();
			frameBuffer .bind ();
	
			renderObject -> getViewVolumes              () .emplace_back (projectionMatrix, viewport, viewport);
			renderObject -> getProjectionMatrix         () .push (projectionMatrix);
			renderObject -> getCameraSpaceMatrix        () .push (renderObject -> getViewpoint () -> getCameraSpaceMatrix ());
			renderObject -> getInverseCameraSpaceMatrix () .push (renderObject -> getViewpoint () -> getInverseCameraSpaceMatrix ());
			renderObject -> getModelViewMatrix          () .push (invLightSpaceMatrix);

			renderObject -> render (TraverseType::DEPTH, [groupNode]
			(const TraverseType type, X3DRenderObject* const renderObject)
			{
				groupNode -> X3DGroupingNode::traverse (type, renderObject);
			});

			renderObject -> getModelViewMatrix          () .pop ();
			renderObject -> getProjectionMatrix         () .pop ();
			renderObject -> getViewVolumes              () .pop_back ();
			renderObject -> getInverseCameraSpaceMatrix () .pop ();
			renderObject -> getCameraSpaceMatrix        () .pop ();
	
			frameBuffer .readDepth ();
			frameBuffer .unbind ();
	
			glDrawPixels (100, 100, GL_LUMINANCE, GL_FLOAT, frameBuffer .getDepth () .data ());
		}
		#endif
		#endif
	
		if (not global ())
			invLightSpaceMatrix .mult_left (inverse (modelMatrix));

		lightContainer -> setShadowMatrix (invLightSpaceMatrix * projectionMatrix * getBiasMatrix ());
		return true;
	}
	catch (const std::exception & error)
	{
		__LOG__ << std::endl;
		return false;
	}
}

void
SpotLight::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i, const Matrix4d & modelViewMatrix, const Matrix3d & lightMatrix)
{
	const auto worldLocation  = Vector3f (modelViewMatrix .mult_vec_matrix (location () .getValue ()));
	const auto worldDirection = Vector3f (normalize (modelViewMatrix .mult_dir_matrix (direction () .getValue ())));

	glUniform1i  (shaderObject -> getLightTypeUniformLocation             () [i], SPOT_LIGHT);
	glUniform3fv (shaderObject -> getLightColorUniformLocation            () [i], 1, getColor () .data ());
	glUniform1f  (shaderObject -> getLightIntensityUniformLocation        () [i], getIntensity ()); 
	glUniform1f  (shaderObject -> getLightAmbientIntensityUniformLocation () [i], getAmbientIntensity ());
	glUniform3fv (shaderObject -> getLightAttenuationUniformLocation      () [i], 1, attenuation () .getValue () .data ());
	glUniform3fv (shaderObject -> getLightLocationUniformLocation         () [i], 1, worldLocation .data ());
	glUniform3fv (shaderObject -> getLightDirectionUniformLocation        () [i], 1, worldDirection .data ());
	glUniform1f  (shaderObject -> getLightBeamWidthUniformLocation        () [i], getBeamWidth ());
	glUniform1f  (shaderObject -> getLightCutOffAngleUniformLocation      () [i], getCutOffAngle ());
	glUniform1f  (shaderObject -> getLightRadiusUniformLocation           () [i], getRadius ());

	if (shaderObject -> isExtensionGPUShaderFP64Available ())
		glUniformMatrix3dv (shaderObject -> getLightMatrixUniformLocation () [i], 1, false, lightMatrix .front () .data ());
	else
		glUniformMatrix3fv (shaderObject -> getLightMatrixUniformLocation () [i], 1, false, Matrix3f (lightMatrix) .front () .data ());
}

} // X3D
} // titania

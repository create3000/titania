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

#include "DirectionalLight.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/LightContainer.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/TextureBuffer.h"
#include "../../Tools/Lighting/DirectionalLightTool.h"

#include "../Layering/X3DLayerNode.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

static constexpr int32_t DIRECTIONAL_LIGHT = 1;

const ComponentType DirectionalLight::component      = ComponentType::LIGHTING;
const std::string   DirectionalLight::typeName       = "DirectionalLight";
const std::string   DirectionalLight::containerField = "children";

DirectionalLight::Fields::Fields () :
   direction (new SFVec3f (0, 0, -1))
{ }

DirectionalLight::DirectionalLight (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLightNode (),
	      fields ()
{
	addType (X3DConstants::DirectionalLight);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "global",           global ());
	addField (inputOutput, "on",               on ());
	addField (inputOutput, "color",            color ());
	addField (inputOutput, "intensity",        intensity ());
	addField (inputOutput, "ambientIntensity", ambientIntensity ());
	addField (inputOutput, "direction",        direction ());

	addField (inputOutput, "shadowColor",      shadowColor ());
	addField (inputOutput, "shadowIntensity",  shadowIntensity ());
	addField (inputOutput, "shadowDiffusion",  shadowDiffusion ());
	addField (inputOutput, "shadowMapSize",    shadowMapSize ());
}

X3DBaseNode*
DirectionalLight::create (X3DExecutionContext* const executionContext) const
{
	return new DirectionalLight (executionContext);
}

void
DirectionalLight::initialize ()
{
	X3DLightNode::initialize ();

	addInterest (this, &DirectionalLight::eventsProcessed);

	eventsProcessed ();
}

void
DirectionalLight::eventsProcessed ()
{
	const auto ambientIntensity = getAmbientIntensity ();
	const auto intensity        = getIntensity ();

	glAmbient [0] = ambientIntensity * color () .getRed ();
	glAmbient [1] = ambientIntensity * color () .getGreen ();
	glAmbient [2] = ambientIntensity * color () .getBlue ();
	glAmbient [3] = 1;

	glDiffuseSpecular [0] = intensity * color () .getRed ();
	glDiffuseSpecular [1] = intensity * color () .getGreen ();
	glDiffuseSpecular [2] = intensity * color () .getBlue ();
	glDiffuseSpecular [3] = 1;

	glPosition [0] = -direction () .getX ();
	glPosition [1] = -direction () .getY ();
	glPosition [2] = -direction () .getZ ();
	glPosition [3] = 0; // directional light
}

void
DirectionalLight::draw (GLenum lightId)
{
	glLightfv (lightId, GL_AMBIENT,  glAmbient);
	glLightfv (lightId, GL_DIFFUSE,  glDiffuseSpecular);
	glLightfv (lightId, GL_SPECULAR, glDiffuseSpecular);

	glLightf  (lightId, GL_SPOT_EXPONENT, 0);
	glLightf  (lightId, GL_SPOT_CUTOFF, 180);

	glLightfv (lightId, GL_POSITION, glPosition);
}

// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/

bool
DirectionalLight::renderShadowMap (LightContainer* const lightContainer)
{
	try
	{
		getBrowser () -> setRenderTools (false);
	
		const auto transformationMatrix = lightContainer -> getModelViewMatrix () * getCameraSpaceMatrix ();
		auto       invLightSpaceMatrix  = global () ? transformationMatrix : Matrix4d ();
	
		invLightSpaceMatrix .rotate (Rotation4d (Vector3d (0, 0, 1), negate (Vector3d (direction () .getValue ()))));
		invLightSpaceMatrix .inverse ();
	
		const auto & textureBuffer    = lightContainer -> getTextureBuffer ();
		const auto   group            = lightContainer -> getGroup ();                                          // Group to be shadowd
		const auto   groupBBox        = group -> X3DGroupingNode::getBBox ();                                   // Group bbox.
		const auto   lightBBox        = groupBBox * invLightSpaceMatrix;                                        // Group bbox from the perspective of the light.
		const auto   viewport         = Vector4i (0, 0, getShadowMapSize (), getShadowMapSize ());
		const auto   projectionMatrix = ortho (lightBBox);
	
		textureBuffer -> bind ();
	
		getViewVolumes      () .emplace_back (projectionMatrix, viewport, viewport);
		getProjectionMatrix () .push (projectionMatrix);
		getModelViewMatrix  () .push (invLightSpaceMatrix);
		getModelViewMatrix  () .mult_left (inverse (group -> getMatrix ()));
	
		getCurrentLayer () -> renderDepth (group);
	
		getModelViewMatrix  () .pop ();
		getProjectionMatrix () .pop ();
		getViewVolumes      () .pop_back ();
	
		textureBuffer -> unbind ();
	
		//#define DEBUG_DIRECTIONAL_LIGHT_SHADOW_BUFFER
		#ifdef  DEBUG_DIRECTIONAL_LIGHT_SHADOW_BUFFER
		#ifdef  TITANIA_DEBUG
		// Disable background in renderer when debug framebuffer.
		{
			const auto viewport = Vector4i (0, 0, 100, 100);
	
			FrameBuffer frameBuffer (getBrowser (), 100, 100, 4);
		
			frameBuffer .setup ();
			frameBuffer .bind ();
		
			getViewVolumes      () .emplace_back (projectionMatrix, viewport, viewport);
			getProjectionMatrix () .push (projectionMatrix);
			getModelViewMatrix  () .push (invLightSpaceMatrix);
			getModelViewMatrix  () .mult_left (inverse (group -> getMatrix ()));
		
			getCurrentLayer () -> renderDepth (group);
		
			getModelViewMatrix  () .pop ();
			getProjectionMatrix () .pop ();
			getViewVolumes      () .pop_back ();
		
			frameBuffer .readDepth ();
			frameBuffer .unbind ();
		
			glDrawPixels (100, 100, GL_LUMINANCE, GL_FLOAT, frameBuffer .getDepth () .data ());
		}
		#endif
		#endif
		
		if (not global ())
			invLightSpaceMatrix .mult_left (inverse (transformationMatrix));
	
		lightContainer -> setShadowMatrix (getCameraSpaceMatrix () * invLightSpaceMatrix * projectionMatrix * getBiasMatrix ());

		getBrowser () -> setRenderTools (true);
		return true;
	}
	catch (const std::domain_error & error)
	{
		return false;
	}
}

void
DirectionalLight::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i, const Matrix4d & modelViewMatrix)
{
	const auto worldDirection = Vector3f (normalize (modelViewMatrix .mult_dir_matrix (direction () .getValue ())));

	glUniform1i  (shaderObject -> getLightTypeUniformLocation             () [i], DIRECTIONAL_LIGHT);
	glUniform3fv (shaderObject -> getLightColorUniformLocation            () [i], 1, getColor () .data ());
	glUniform1f  (shaderObject -> getLightIntensityUniformLocation        () [i], getIntensity ());
	glUniform1f  (shaderObject -> getLightAmbientIntensityUniformLocation () [i], getAmbientIntensity ());
	glUniform3fv (shaderObject -> getLightDirectionUniformLocation        () [i], 1, worldDirection .data ());
}

void
DirectionalLight::addTool ()
{
	X3DLightNode::addTool (new DirectionalLightTool (this));
}

} // X3D
} // titania

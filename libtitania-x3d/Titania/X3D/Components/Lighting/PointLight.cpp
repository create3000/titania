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

#include "PointLight.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/TextureBuffer.h"
#include "../../Tools/Lighting/PointLightTool.h"

#include "../Layering/X3DLayerNode.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

static constexpr int32_t POINT_LIGHT = 2;

const ComponentType PointLight::component      = ComponentType::LIGHTING;
const std::string   PointLight::typeName       = "PointLight";
const std::string   PointLight::containerField = "children";

PointLight::Fields::Fields () :
	attenuation (new SFVec3f (1, 0, 0)),
	   location (new SFVec3f ()),
	     radius (new SFFloat (100))
{ }

PointLight::PointLight (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLightNode (),
	      fields ()
{
	addType (X3DConstants::PointLight);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "global",           global ());
	addField (inputOutput, "on",               on ());
	addField (inputOutput, "color",            color ());
	addField (inputOutput, "intensity",        intensity ());
	addField (inputOutput, "ambientIntensity", ambientIntensity ());
	addField (inputOutput, "attenuation",      attenuation ());
	addField (inputOutput, "location",         location ());
	addField (inputOutput, "radius",           radius ());

	addField (inputOutput, "shadowColor",      shadowColor ());
	addField (inputOutput, "shadowIntensity",  shadowIntensity ());
	addField (inputOutput, "shadowDiffusion",  shadowDiffusion ());
	addField (inputOutput, "shadowMapSize",    shadowMapSize ());

	location () .setUnit (UnitCategory::LENGTH);
	radius ()   .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
PointLight::create (X3DExecutionContext* const executionContext) const
{
	return new PointLight (executionContext);
}

void
PointLight::initialize ()
{
	X3DLightNode::initialize ();

	addInterest (this, &PointLight::eventsProcessed);

	eventsProcessed ();
}

float
PointLight::getRadius () const
{
	return std::max <float> (0, radius ());
}

size_t
PointLight::getShadowMapSize () const
{
	return shadowMapSize () - shadowMapSize () % 2;
}

// Determine far value for shadow map calculation.
double
PointLight::getFarValue (const Box3d & box) const
{
	double farValue = 0;

	// Determine far value.

	for (const auto point : box .points ())
	{
		const auto length = abs (point);

		if (length > farValue)
			farValue = length;
	}
	
	return std::min <double> (getRadius (), farValue);
}

void
PointLight::eventsProcessed ()
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

	glPosition [0] = location () .getX ();
	glPosition [1] = location () .getY ();
	glPosition [2] = location () .getZ ();
	glPosition [3] = 1;                       // point light
}

void
PointLight::draw (GLenum lightId)
{
	glLightfv (lightId, GL_AMBIENT,  glAmbient);
	glLightfv (lightId, GL_DIFFUSE,  glDiffuseSpecular);
	glLightfv (lightId, GL_SPECULAR, glDiffuseSpecular);

	glLightf  (lightId, GL_SPOT_EXPONENT, 0);
	glLightf  (lightId, GL_SPOT_CUTOFF, 180); // point light

	glLightf  (lightId, GL_CONSTANT_ATTENUATION,  std::max (0.0f, attenuation () .getX ()));
	glLightf  (lightId, GL_LINEAR_ATTENUATION,    std::max (0.0f, attenuation () .getY ()));
	glLightf  (lightId, GL_QUADRATIC_ATTENUATION, std::max (0.0f, attenuation () .getZ ()));

	glLightfv (lightId, GL_POSITION, glPosition);
}

bool
PointLight::renderShadowMap (LightContainer* const lightContainer)
{
	try
	{
		// Vertices of the tetrahedron.

		static const Matrix4d rotations [4] = {
			Matrix4d (Rotation4d (Vector3d (0, 0, 1), negate (Vector3d (0, 1, 0)))),
			Matrix4d (Rotation4d (Vector3d (0, 0, 1), negate (Vector3d (0, 1, 0) * Rotation4d (1, 0, 0, radians (120.0))))),
			Matrix4d (Rotation4d (Vector3d (0, 0, 1), negate (Vector3d (0, 1, 0) * Rotation4d (1, 0, 0, radians (120.0)) * Rotation4d (0, 1, 0, radians ( 120.0))))),
			Matrix4d (Rotation4d (Vector3d (0, 0, 1), negate (Vector3d (0, 1, 0) * Rotation4d (1, 0, 0, radians (120.0)) * Rotation4d (0, 1, 0, radians (-120.0))))),
		};

		getBrowser () -> setRenderTools (false);

		const auto transformationMatrix = lightContainer -> getModelViewMatrix () * getCameraSpaceMatrix ();
		auto       invLightSpaceMatrix  = global () ? transformationMatrix : Matrix4d ();
		
		invLightSpaceMatrix .translate (location () .getValue ());
		invLightSpaceMatrix .inverse ();

		const auto & textureBuffer    = lightContainer -> getTextureBuffer ();
		const auto   group            = lightContainer -> getGroup ();           // Group to be shadowd
		const auto   groupBBox        = group -> X3DGroupingNode::getBBox ();    // Group bbox.
		const auto   lightBBox        = groupBBox * invLightSpaceMatrix;         // Group bbox from the perspective of the light.
		const auto   shadowMapSize1_2 = getShadowMapSize () / 2;
		const auto   viewport         = Vector4i (0, 0, shadowMapSize1_2, shadowMapSize1_2);
		const auto   projectionMatrix = perspective <double> (radians (120.0), 0.125, getFarValue (lightBBox), viewport);
		const auto   directionMatrix  = getCameraSpaceMatrix () * invLightSpaceMatrix;

		// Render to frame buffer.

		#define DEBUG_POINT_LIGHT_SHADOW_BUFFER
		#ifdef  DEBUG_POINT_LIGHT_SHADOW_BUFFER
		#ifdef  TITANIA_DEBUG
		FrameBuffer frameBuffer (getBrowser (), 100, 100, 4);
	
		frameBuffer .setup ();
		#endif
		#endif

		for (size_t y = 0; y < 2; ++ y)
		{
			for (size_t x = 0; x < 2; ++ x)
			{
				const auto rotation             = rotations [y * 2 + x] * directionMatrix;
				const auto transformationMatrix = lightContainer -> getModelViewMatrix () * getCameraSpaceMatrix ();
				auto       invLightSpaceMatrix  = global () ? transformationMatrix : Matrix4d ();

				invLightSpaceMatrix .translate (location () .getValue ());
				invLightSpaceMatrix .mult_left (rotation);
				invLightSpaceMatrix .inverse ();

				const auto viewport = Vector4i (x * shadowMapSize1_2, y * shadowMapSize1_2, shadowMapSize1_2, shadowMapSize1_2);

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

				#ifdef DEBUG_POINT_LIGHT_SHADOW_BUFFER
				#ifdef TITANIA_DEBUG
				{
					const auto viewport = Vector4i (x * 50, y * 50, 50, 50);

					frameBuffer .bind ();

					getViewVolumes      () .emplace_back (projectionMatrix, viewport, viewport);
					getProjectionMatrix () .push (projectionMatrix);
					getModelViewMatrix  () .push (invLightSpaceMatrix);
					getModelViewMatrix  () .mult_left (inverse (group -> getMatrix ()));

					getCurrentLayer () -> renderDepth (group);

					getModelViewMatrix  () .pop ();
					getProjectionMatrix () .pop ();
					getViewVolumes      () .pop_back ();

					frameBuffer .unbind ();
				}
				#endif
				#endif
			}
		}

		#ifdef DEBUG_POINT_LIGHT_SHADOW_BUFFER
		#ifdef TITANIA_DEBUG
		frameBuffer .bind ();
		frameBuffer .readDepth ();
		frameBuffer .unbind ();

		glDrawPixels (100, 100, GL_LUMINANCE, GL_FLOAT, frameBuffer .getDepth () .data ());
		#endif
		#endif
	
		if (not global ())
			invLightSpaceMatrix .mult_left (inverse (transformationMatrix));

		lightContainer -> setShadowMatrix (getCameraSpaceMatrix () * invLightSpaceMatrix * projectionMatrix * getBiasMatrix ());

		getBrowser () -> setRenderTools (true);
		return true;
	}
	catch (const std::domain_error &)
	{
		return false;
	}
}

void
PointLight::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i, const Matrix4d & modelViewMatrix)
{
	const auto worldLocation = Vector3f (modelViewMatrix .mult_vec_matrix (location () .getValue ()));

	glUniform1i  (shaderObject -> getLightTypeUniformLocation             () [i], POINT_LIGHT);
	glUniform3fv (shaderObject -> getLightColorUniformLocation            () [i], 1, getColor () .data ());
	glUniform1f  (shaderObject -> getLightIntensityUniformLocation        () [i], getIntensity ());
	glUniform1f  (shaderObject -> getLightAmbientIntensityUniformLocation () [i], getAmbientIntensity ());
	glUniform3fv (shaderObject -> getLightAttenuationUniformLocation      () [i], 1, attenuation () .getValue () .data ());
	glUniform3fv (shaderObject -> getLightLocationUniformLocation         () [i], 1, worldLocation .data ());
	glUniform1f  (shaderObject -> getLightRadiusUniformLocation           () [i], getRadius ());

	// TODO: remove me
	glUniform1f (shaderObject -> getShadowIntensityUniformLocation () [i], 0);
}

void
PointLight::addTool ()
{
	X3DLightNode::addTool (new PointLightTool (this));
}

} // X3D
} // titania

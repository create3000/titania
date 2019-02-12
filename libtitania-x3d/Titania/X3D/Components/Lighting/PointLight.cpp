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

#include "PointLight.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/TextureBuffer.h"
#include "../../Rendering/X3DRenderObject.h"

#include "../Navigation/X3DViewpointNode.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

static constexpr int32_t POINT_LIGHT = 2;

const Component   PointLight::component      = Component ("Lighting", 2);
const std::string PointLight::typeName       = "PointLight";
const std::string PointLight::containerField = "children";

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

	addField (inputOutput,    "shadowColor",     shadowColor ());
	addField (inputOutput,    "shadowIntensity", shadowIntensity ());
	addField (inputOutput,    "shadowBias",      shadowBias ());
	addField (initializeOnly, "shadowMapSize",   shadowMapSize ());

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

	addInterest (&PointLight::eventsProcessed, this);

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
	const auto shadowMapSize = X3DLightNode::getShadowMapSize ();

	return shadowMapSize - shadowMapSize % 6;
}

// Determine far value for shadow map calculation.
double
PointLight::getFarValue (const Box3d & box, const Vector3d & location) const
{
	const auto   extents = box .extents ();
	const auto & min     = extents .first;
	const auto & max     = extents .second;

	const auto farValue = std::max (abs (min - location), abs (max - location));

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
PointLight::renderShadowMap (X3DRenderObject* const renderObject, LightContainer* const lightContainer)
{
	try
	{
		using namespace std::placeholders;
		using namespace math_literals;

		// Negated normals of the point light cube.

		static const Matrix4d orientationMatrices [6] = {
			Matrix4d (Rotation4d (Vector3d ( 1,  0,  0), Vector3d (0, 0, 1))), // left
			Matrix4d (Rotation4d (Vector3d (-1,  0,  0), Vector3d (0, 0, 1))), // right
			Matrix4d (Rotation4d (Vector3d ( 0,  0, -1), Vector3d (0, 0, 1))), // front
			Matrix4d (Rotation4d (Vector3d ( 0,  0,  1), Vector3d (0, 0, 1))), // back
			Matrix4d (Rotation4d (Vector3d ( 0,  1,  0), Vector3d (0, 0, 1))), // bottom
			Matrix4d (Rotation4d (Vector3d ( 0, -1,  0), Vector3d (0, 0, 1))), // top
		};

		static const Vector4d viewports [6] = {
			Vector4d (0,    0.5, 0.25, 0.5), // left
			Vector4d (0.5,  0.5, 0.25, 0.5), // right
			Vector4d (0.75, 0.5, 0.25, 0.5), // front
			Vector4d (0.25, 0.5, 0.25, 0.5), // back
			Vector4d (0.0,  0,   0.5,  0.5), // bottom
			Vector4d (0.5,  0,   0.5,  0.5), // top   
		};

		const auto modelMatrix         = lightContainer -> getModelViewMatrix () .get () * renderObject -> getCameraSpaceMatrix () .get ();
		auto       invLightSpaceMatrix = global () ? modelMatrix : Matrix4d ();

		invLightSpaceMatrix .translate (location () .getValue ());
		invLightSpaceMatrix .inverse ();

		const auto & shadowTextureBuffer = lightContainer -> getShadowTextureBuffer ();                            
		const auto   groupNode           = lightContainer -> getGroup (); // Group to be shadowed.
		const auto   shadowMapSize       = getShadowMapSize ();

		// Render to frame buffer.

		renderObject -> getBrowser () -> getDisplayTools () .push (false);

		shadowTextureBuffer -> setFiltering (GL_NEAREST);

		for (size_t i = 0; i < 6; ++ i)
		{
			const auto & v                = viewports [i];
			const auto   viewport         = Vector4i (v [0] * shadowMapSize, v [1] * shadowMapSize, v [2] * shadowMapSize, v [3] * shadowMapSize);
			const auto   projectionMatrix = camera <double>::perspective2 (90.0_deg, 0.125, 10000, viewport [2], viewport [3]);

			shadowTextureBuffer -> bind ();

			renderObject -> getViewVolumes              () .emplace_back (projectionMatrix, viewport, viewport);
			renderObject -> getCameraSpaceMatrix        () .push (renderObject -> getViewpoint () -> getCameraSpaceMatrix ());
			renderObject -> getInverseCameraSpaceMatrix () .push (renderObject -> getViewpoint () -> getInverseCameraSpaceMatrix ());
			renderObject -> getProjectionMatrix         () .push (projectionMatrix);

			renderObject -> getModelViewMatrix  () .push (orientationMatrices [i]);
			renderObject -> getModelViewMatrix  () .mult_left (invLightSpaceMatrix);

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

			shadowTextureBuffer -> unbind ();
		}

		renderObject -> getBrowser () -> getDisplayTools () .pop ();

		if (not global ())
			invLightSpaceMatrix .mult_left (inverse (modelMatrix));

		lightContainer -> setShadowMatrix (invLightSpaceMatrix);

		return true;
	}
	catch (const std::exception &)
	{
		__LOG__ << std::endl;
		return false;
	}
}

void
PointLight::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i, const Matrix4d & modelViewMatrix, const Matrix3d & lightMatrix)
{
	const auto worldLocation = Vector3f (modelViewMatrix .mult_vec_matrix (location () .getValue ()));

	glUniform1i  (shaderObject -> getLightTypeUniformLocation             () [i], POINT_LIGHT);
	glUniform3fv (shaderObject -> getLightColorUniformLocation            () [i], 1, getColor () .data ());
	glUniform1f  (shaderObject -> getLightIntensityUniformLocation        () [i], getIntensity ());
	glUniform1f  (shaderObject -> getLightAmbientIntensityUniformLocation () [i], getAmbientIntensity ());
	glUniform3fv (shaderObject -> getLightAttenuationUniformLocation      () [i], 1, attenuation () .getValue () .data ());
	glUniform3fv (shaderObject -> getLightLocationUniformLocation         () [i], 1, worldLocation .data ());
	glUniform1f  (shaderObject -> getLightRadiusUniformLocation           () [i], getRadius ());

	if (shaderObject -> isExtensionGPUShaderFP64Available ())
		glUniformMatrix3dv (shaderObject -> getLightMatrixUniformLocation () [i], 1, false, lightMatrix .front () .data ());
	else
		glUniformMatrix3fv (shaderObject -> getLightMatrixUniformLocation () [i], 1, false, Matrix3f (lightMatrix) .front () .data ());
}

} // X3D
} // titania

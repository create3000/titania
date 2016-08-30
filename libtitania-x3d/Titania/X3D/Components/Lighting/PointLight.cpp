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
#include "../../Tools/Lighting/PointLightTool.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

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

void
PointLight::eventsProcessed ()
{
	const float glAmbientIntensity = math::clamp <float> (ambientIntensity (), 0, 1);
	const float glIntensity        = math::clamp <float> (intensity (), 0, 1);

	glAmbient [0] = glAmbientIntensity * color () .getRed ();
	glAmbient [1] = glAmbientIntensity * color () .getGreen ();
	glAmbient [2] = glAmbientIntensity * color () .getBlue ();
	glAmbient [3] = 1;

	glDiffuseSpecular [0] = glIntensity * color () .getRed ();
	glDiffuseSpecular [1] = glIntensity * color () .getGreen ();
	glDiffuseSpecular [2] = glIntensity * color () .getBlue ();
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

void
PointLight::renderShadowMap (LightContainer* const lightContainer)
{

}

void
PointLight::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i, const Matrix4d & modelViewMatrix)
{
	const auto worldLocation = Vector3f (modelViewMatrix .mult_vec_matrix (location () .getValue ()));

	glUniform1i  (shaderObject -> getLightTypeUniformLocation             () [i], POINT_LIGHT);
	glUniform3fv (shaderObject -> getLightColorUniformLocation            () [i], 1, color () .getValue () .data ());
	glUniform1f  (shaderObject -> getLightIntensityUniformLocation        () [i], intensity ());        // clamp
	glUniform1f  (shaderObject -> getLightAmbientIntensityUniformLocation () [i], ambientIntensity ()); // clamp
	glUniform3fv (shaderObject -> getLightAttenuationUniformLocation      () [i], 1, attenuation () .getValue () .data ());
	glUniform3fv (shaderObject -> getLightLocationUniformLocation         () [i], 1, worldLocation .data ());
	glUniform1f  (shaderObject -> getLightRadiusUniformLocation           () [i], radius ());
}

void
PointLight::addTool ()
{
	X3DLightNode::addTool (new PointLightTool (this));
}

} // X3D
} // titania

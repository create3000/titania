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

#include "LightContainer.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Lighting/X3DLightNode.h"
#include "../Components/Shaders/X3DProgrammableShaderObject.h"
#include "../Rendering/X3DRenderObject.h"

#include "TextureBuffer.h"

namespace titania {
namespace X3D {

LightContainer::LightContainer (X3DBrowser* const browser, X3DLightNode* const node, X3DGroupingNode* const group, const Matrix4d & modelViewMatrix) :
	X3DCollectableObject (),
	             browser (browser),
	                node (node),
	               group (group),
	     modelViewMatrix ({ modelViewMatrix }),
	         lightMatrix (),
	        shadowMatrix (),
	 shadowTextureBuffer (),
	         textureUnit (0),
	             lightId (0)
{
	try
	{
		lightMatrix = inverse (modelViewMatrix .submatrix ());
	}
	catch (const std::domain_error & error)
	{ }

	try
	{
		if (node -> getShadowIntensity () > 0 and node -> getShadowMapSize () > 0 and not browser -> getFixedPipelineRequired ())
		{
			shadowTextureBuffer .reset (new TextureBuffer (browser,
			                                               node -> getShadowMapSize (),
			                                               node -> getShadowMapSize (),
			                                               false));
			shadowTextureBuffer -> setup ();
		}
	}
	catch (const std::runtime_error & error)
	{
		// Couldn't create texture buffer.
		__LOG__ << error .what () << std::endl;
	}
}

void
LightContainer::renderShadowMap (X3DRenderObject* const renderObject)
{
	if (shadowTextureBuffer)
	{
		if (not node -> renderShadowMap (renderObject, this))
			shadowTextureBuffer .reset ();
	}
}

void
LightContainer::enable ()
{
	if (browser -> getFixedPipelineRequired ())
	{
		auto & lights = browser -> getLights ();
	
		if (not lights .empty ())
		{
			lightId = lights .top ();
			lights .pop ();
	
			glEnable (lightId);
	
			glLoadMatrixd (modelViewMatrix .get () .front () .data ());
	
			node -> draw (lightId);
		}
	}

	if (shadowTextureBuffer and not browser -> getCombinedTextureUnits () .empty ())
	{
		textureUnit = browser -> getCombinedTextureUnits () .top ();
		browser -> getCombinedTextureUnits () .pop ();

		glActiveTexture (GL_TEXTURE0 + textureUnit);
		glBindTexture (GL_TEXTURE_2D, shadowTextureBuffer -> getDepthTextureId ());
		glActiveTexture (GL_TEXTURE0);
	}
}

void
LightContainer::disable ()
{
	if (browser -> getFixedPipelineRequired ())
	{
		if (lightId)
		{
			glDisable (lightId);
			browser -> getLights () .push (lightId);
		}
	}

	if (textureUnit)
		browser -> getCombinedTextureUnits () .push (textureUnit);
}

void
LightContainer::setShaderUniforms (X3DRenderObject* const renderObject, X3DProgrammableShaderObject* const shaderObject, const size_t i)
{
	node -> setShaderUniforms (shaderObject, i, modelViewMatrix .get (), lightMatrix);

	if (textureUnit)
	{
		const auto cameraSpaceMatrix = renderObject -> getCameraSpaceMatrix () .get ();

		glUniform3fv (shaderObject -> getShadowColorUniformLocation     () [i], 1, node -> getShadowColor () .data ());
		glUniform1f  (shaderObject -> getShadowIntensityUniformLocation () [i], node -> getShadowIntensity ());
		glUniform1f  (shaderObject -> getShadowBiasUniformLocation      () [i], node -> getShadowBias ());

		if (shaderObject -> isExtensionGPUShaderFP64Available ())
			glUniformMatrix4dv (shaderObject -> getShadowMatrixUniformLocation () [i], 1, false, (cameraSpaceMatrix * shadowMatrix) .front () .data ());
		else
			glUniformMatrix4fv (shaderObject -> getShadowMatrixUniformLocation () [i], 1, false, Matrix4f (cameraSpaceMatrix * shadowMatrix) .front () .data ());
	
		glUniform1i (shaderObject -> getShadowMapSizeUniformLocation () [i], node -> getShadowMapSize ());
		glUniform1i (shaderObject -> getShadowMapUniformLocation () [i], textureUnit);
	}
	else
		glUniform1f (shaderObject -> getShadowIntensityUniformLocation () [i], 0);
}

LightContainer::~LightContainer ()
{ }

} // X3D
} // titania

/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "LightContainer.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Lighting/X3DLightNode.h"
#include "../Components/Shaders/X3DProgrammableShaderObject.h"

#include "TextureBuffer.h"

namespace titania {
namespace X3D {

LightContainer::LightContainer (X3DLightNode* const node, X3DGroupingNode* const group) :
	X3DCollectableObject (),
	             browser (node -> getBrowser ()),
	                node (node),
	     modelViewMatrix (node -> getModelViewMatrix () .get ()),
	        shadowMatrix (),
	               group (group),
	       textureBuffer (),
	         textureUnit (0),
	             lightId (0)
{
	try
	{
		if (node -> shadowIntensity () > 0 and not browser -> getFixedPipelineRequired ())
		{
			textureBuffer .reset (new TextureBuffer (browser,
			                                         std::max <int32_t> (node -> shadowMapSize (), 1),
			                                         std::max <int32_t> (node -> shadowMapSize (), 1),
			                                         false));
			textureBuffer -> setup ();
		}
	}
	catch (const std::runtime_error &)
	{
		// Couldn't create texture buffer.
	}
}

void
LightContainer::renderShadowMap ()
{
	if (textureBuffer)
	{
		if (not node -> renderShadowMap (this))
			textureBuffer .reset ();
	}
}

void
LightContainer::enable ()
{
	#ifdef FIXED_PIPELINE
	if (browser -> getFixedPipelineRequired ())
	{
		auto & lights = browser -> getLights ();
	
		if (not lights .empty ())
		{
			lightId = lights .top ();
			lights .pop ();
	
			glEnable (lightId);
	
			glLoadMatrixd (modelViewMatrix .data ());
	
			node -> draw (lightId);
		}
	}
	#endif

	if (textureBuffer and not browser -> getCombinedTextureUnits () .empty ())
	{
		textureUnit = browser -> getCombinedTextureUnits () .top ();
		browser -> getCombinedTextureUnits () .pop ();

		glActiveTexture (GL_TEXTURE0 + textureUnit);
		glBindTexture (GL_TEXTURE_2D, textureBuffer -> getDepthTextureId ());
		glActiveTexture (GL_TEXTURE0);
	}
}

void
LightContainer::disable ()
{
	#ifdef FIXED_PIPELINE
	if (browser -> getFixedPipelineRequired ())
	{
		if (lightId)
		{
			glDisable (lightId);
			browser -> getLights () .push (lightId);
		}
	}
	#endif

	if (textureUnit)
		browser -> getCombinedTextureUnits () .push (textureUnit);
}

void
LightContainer::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i)
{
	node -> setShaderUniforms (shaderObject, i, modelViewMatrix);

	if (textureUnit)
	{
		glUniform1f  (shaderObject -> getShadowIntensityUniformLocation () [i], node -> getShadowIntensity ());
		glUniform1f  (shaderObject -> getShadowDiffusionUniformLocation () [i], node -> getShadowDiffusion ());
		glUniform3fv (shaderObject -> getShadowColorUniformLocation     () [i], 1, node -> getShadowColor () .data ());

		if (shaderObject -> isExtensionGPUShaderFP64Available ())
			glUniformMatrix4dv (shaderObject -> getShadowMatrixUniformLocation () [i], 1, false, shadowMatrix .data ());
		else
			glUniformMatrix4fv (shaderObject -> getShadowMatrixUniformLocation () [i], 1, false, Matrix4f (shadowMatrix) .data ());
	
		glUniform1i (shaderObject -> getShadowMapUniformLocation () [i], textureUnit);
	}
	else
		glUniform1f (shaderObject -> getShadowIntensityUniformLocation () [i], 0);
}

LightContainer::~LightContainer ()
{ }

} // X3D
} // titania

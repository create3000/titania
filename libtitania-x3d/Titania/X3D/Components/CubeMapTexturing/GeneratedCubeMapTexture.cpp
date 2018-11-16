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

#include "GeneratedCubeMapTexture.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/DependentRenderer.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Layering/X3DLayerNode.h"
#include "../Lighting/DirectionalLight.h"
#include "../Navigation/NavigationInfo.h"
#include "../Navigation/X3DViewpointNode.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

const ComponentType GeneratedCubeMapTexture::component      = ComponentType::CUBE_MAP_TEXTURING;
const std::string   GeneratedCubeMapTexture::typeName       = "GeneratedCubeMapTexture";
const std::string   GeneratedCubeMapTexture::containerField = "texture";

GeneratedCubeMapTexture::Fields::Fields () :
	           update (new SFString ("NONE")),
	             size (new SFInt32 (128)),
	textureProperties (new SFNode ())
{ }

GeneratedCubeMapTexture::GeneratedCubeMapTexture (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentTextureNode (),
	                   fields (),
	                loadState (NOT_STARTED_STATE),
	              transparent (false),
	               updateType (UpdateType::NONE),
	                 renderer (new DependentRenderer (executionContext)),
	              frameBuffer (),
	              modelMatrix ()
{
	addType (X3DConstants::GeneratedCubeMapTexture);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "update",            update ());
	addField (initializeOnly, "size",              size ());
	addField (initializeOnly, "textureProperties", textureProperties ());

	addChildObjects (loadState, renderer);
}

X3DBaseNode*
GeneratedCubeMapTexture::create (X3DExecutionContext* const executionContext) const
{
	return new GeneratedCubeMapTexture (executionContext);
}

void
GeneratedCubeMapTexture::initialize ()
{
	X3DEnvironmentTextureNode::initialize ();

	setLoadState (COMPLETE_STATE);

	renderer -> setup ();

	update () .addInterest (&GeneratedCubeMapTexture::set_update, this);
	size   () .addInterest (&GeneratedCubeMapTexture::set_size, this);

	set_update ();
	set_size ();
}

void
GeneratedCubeMapTexture::setExecutionContext (X3DExecutionContext* const executionContext)
{
	renderer -> setExecutionContext (executionContext);

	X3DEnvironmentTextureNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_size ();
}

void
GeneratedCubeMapTexture::set_update ()
{
	static const std::map <std::string, UpdateType> updateTypes = {
		std::pair ("NONE",            UpdateType::NONE),
		std::pair ("NEXT_FRAME_ONLY", UpdateType::NEXT_FRAME_ONLY),
		std::pair ("ALWAYS",          UpdateType::ALWAYS),
	};

	try
	{
		updateType = updateTypes .at (update ());
	}
	catch (const std::out_of_range &)
	{
		updateType = UpdateType::NONE;
	}
}

void
GeneratedCubeMapTexture::set_size ()
{
	try
	{
		const auto size = std::max <int32_t> (1, this -> size ());
	
		frameBuffer .reset (new FrameBuffer (getBrowser (), size, size, std::min <size_t> (getBrowser () -> getMaxSamples (), 8)));
	
		frameBuffer -> setup ();
	}
	catch (const std::runtime_error & error)
	{
		// Couldn't create texture buffer.
		__LOG__ << error .what () << std::endl;

		frameBuffer .reset ();
	}
}

void
GeneratedCubeMapTexture::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (type != TraverseType::DISPLAY)
		return;

	if (updateType == UpdateType::NONE)
		return;

	if (size () <= 0)
		return;

	if (not frameBuffer)
		return;

	if (renderObject -> getBrowser () not_eq getBrowser ())
		return;

	if (not renderObject -> isIndependent ())
		return;

	renderObject -> getGeneratedCubeMapTextures () .emplace (this);

	modelMatrix = renderObject -> getModelViewMatrix () .get () * renderObject -> getCameraSpaceMatrix () .get ();
}

void
GeneratedCubeMapTexture::renderTexture (X3DRenderObject* const renderObject, const TraverseFunction & traverse)
{
	using namespace std::placeholders;

	// Negated normals of the texture cube.

	static constexpr Vector3d directions [6] = {
		Vector3d ( 0,  0, -1), // front
		Vector3d ( 0,  0,  1), // back
		Vector3d ( 1,  0,  0), // left
		Vector3d (-1,  0,  0), // right
		Vector3d ( 0, -1,  0), // top
		Vector3d ( 0,  1,  0), // bottom
	};

	// Negated scales of the texture cube.

	static constexpr Vector3d scale [6] = {
		Vector3d (-1, -1,  1), // front
		Vector3d (-1, -1,  1), // back
		Vector3d (-1, -1,  1), // left
		Vector3d (-1, -1,  1), // right
		Vector3d ( 1,  1,  1), // top
		Vector3d ( 1,  1,  1), // bottom
	};

	renderer -> setRenderer (renderObject);

	const auto & browser          = renderObject -> getBrowser ();
	const auto & layer            = renderer -> getLayer ();
	const auto   viewport         = Vector4i (0, 0, size (), size ());
	const auto & background       = renderer -> getBackground ();
	const auto & navigationInfo   = renderer -> getNavigationInfo ();
	const auto & viewpoint        = renderer -> getViewpoint ();
	const auto   nearValue        = navigationInfo -> getNearValue ();
	const auto   farValue         = navigationInfo -> getFarValue (viewpoint);
	const auto   projectionMatrix = camera <double>::perspective (radians (90.0), nearValue, farValue, 1, 1);

	transparent = background -> isTransparent ();

	browser     -> getDisplayTools () .push (false);
	frameBuffer -> bind ();

	renderer -> getViewVolumes      () .emplace_back (projectionMatrix, viewport, viewport);
	renderer -> getProjectionMatrix () .push (projectionMatrix);

	for (size_t i = 0; i < 6; ++ i)
	{
		const auto rotation = Rotation4d (Vector3d (0, 0, 1), directions [i]);

		glClear (GL_COLOR_BUFFER_BIT); // Always clear, X3DBackground could be transparent!

		// Setup inverse texture space matrix.

		renderer -> getCameraSpaceMatrix () .push (modelMatrix);
		renderer -> getCameraSpaceMatrix () .rotate (rotation);
		renderer -> getCameraSpaceMatrix () .scale (scale [i]);

		try
		{
			renderer -> getInverseCameraSpaceMatrix () .push (inverse (renderer -> getCameraSpaceMatrix () .get ()));
		}
		catch (const std::domain_error &)
		{
			renderer -> getInverseCameraSpaceMatrix () .push (Matrix4d ());

			__LOG__ << std::endl;
		}

		renderer -> getModelViewMatrix () .push (renderer -> getInverseCameraSpaceMatrix () .get ());

		// Setup headlight if enabled.

		if (navigationInfo -> headlight ())
		{
			browser -> getHeadlight () -> getModelViewMatrix () .push (renderer -> getInverseCameraSpaceMatrix () .get ());
			browser -> getHeadlight () -> getModelViewMatrix () .mult_left (renderObject -> getCameraSpaceMatrix () .get ());
		}

		// Render layer's children.

		layer -> traverse (TraverseType::DISPLAY, renderer);

		// Pop matrices.

		if (navigationInfo -> headlight ())
			browser -> getHeadlight () -> getModelViewMatrix () .pop ();

		renderer -> getModelViewMatrix          () .pop ();
		renderer -> getCameraSpaceMatrix        () .pop ();
		renderer -> getInverseCameraSpaceMatrix () .pop ();

		// Transfer image.

		setImage (getTargets () [i], GL_RGBA, GL_RGBA, frameBuffer -> readPixels () .data ());
	}

	renderer -> getProjectionMatrix () .pop ();
	renderer -> getViewVolumes      () .pop_back ();

	frameBuffer -> unbind ();
	browser     -> getDisplayTools () .pop ();

	if (updateType == UpdateType::NEXT_FRAME_ONLY)
	   update () = "NONE";
}

void
GeneratedCubeMapTexture::dispose ()
{
	frameBuffer .reset ();

	X3DEnvironmentTextureNode::dispose ();
}

GeneratedCubeMapTexture::~GeneratedCubeMapTexture ()
{ }

} // X3D
} // titania

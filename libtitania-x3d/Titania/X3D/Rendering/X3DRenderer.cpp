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

#include "X3DRenderer.h"

#include "../Browser/BrowserOptions.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Navigation/NavigationInfo.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Shape/Appearance.h"
#include "../Rendering/FrameBuffer.h"

#include <Titania/Utility/Range.h>
#include <algorithm>

namespace titania {
namespace X3D {

static constexpr size_t DEPTH_BUFFER_WIDTH  = 16;
static constexpr size_t DEPTH_BUFFER_HEIGHT = 16;

X3DRenderer::X3DRenderer () :
	             X3DNode (),
	     viewVolumeStack (),
	       globalObjects (),
	        localObjects (),
	          collisions (),
	              shapes (),
	   transparentShapes (),
	     collisionShapes (),
	    activeCollisions (),
	         depthBuffer (),
	               speed (),
	            distance (),
	     numOpaqueShapes (0),
	numTransparentShapes (0),
	  numCollisionShapes (0)
{
	addType (X3DConstants::X3DRenderObject);
}

void
X3DRenderer::initialize ()
{
	depthBuffer .reset (new FrameBuffer (getBrowser (), DEPTH_BUFFER_WIDTH, DEPTH_BUFFER_HEIGHT, 0, false));
}

void
X3DRenderer::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (depthBuffer)
		depthBuffer -> setBrowser (executionContext -> getBrowser ());
}

void
X3DRenderer::addShape (X3DShapeNode* const shape)
{
	const Box3f bbox   = shape -> getBBox () * getModelViewMatrix () .get ();
	const float depth  = bbox .size () .z () / 2;
	const float min    = bbox .center () .z () - depth;
	const float center = bbox .center () .z ();

	if (min < 0)
	{
		const auto & viewVolume = viewVolumeStack .back ();
	
		if (viewVolume .intersects (bbox))
		{
			if (shape -> isTransparent ())
			{
				if (numTransparentShapes < transparentShapes .size ())
					transparentShapes [numTransparentShapes] -> assign (shape, getFog (), getLocalObjects (), viewVolume .getScissor (), getModelViewMatrix () .get (), center);

				else
					transparentShapes .emplace_back (new ShapeContainer (shape, getFog (), getLocalObjects (), viewVolume .getScissor (), getModelViewMatrix () .get (), center));

				++ numTransparentShapes;
			}
			else
			{
				if (numOpaqueShapes < shapes .size ())
					shapes [numOpaqueShapes] -> assign (shape, getFog (), getLocalObjects (), viewVolume .getScissor (), getModelViewMatrix () .get (), center);

				else
					shapes .emplace_back (new ShapeContainer (shape, getFog (), getLocalObjects (), viewVolume .getScissor (), getModelViewMatrix () .get (), center));

				++ numOpaqueShapes;
			}
		}
	}
}

void
X3DRenderer::addCollision (X3DShapeNode* const shape)
{
	const Box3f bbox   = shape -> getBBox () * getModelViewMatrix () .get ();
	const float depth  = bbox .size () .z () / 2;
	const float min    = bbox .center () .z () - depth;
	const float center = bbox .center () .z ();

	if (min < 0)
	{
		const auto & viewVolume = viewVolumeStack .back ();
	
		if (viewVolume .intersects (bbox))
		{
			if (numCollisionShapes < collisionShapes .size ())
				collisionShapes [numCollisionShapes] -> assign (shape, getCollisions (), getLocalObjects (), viewVolume .getScissor (), getModelViewMatrix () .get (), center);

			else
				collisionShapes .emplace_back (new CollisionShape (shape, getCollisions (), getLocalObjects (), viewVolume .getScissor (), getModelViewMatrix () .get (), center));

			++ numCollisionShapes;
		}
	}
}

void
X3DRenderer::render (const TraverseType type)
{
	numOpaqueShapes      = 0;
	numTransparentShapes = 0;
	numCollisionShapes   = 0;

	switch (type)
	{
		case TraverseType::NAVIGATION:
		{
			depthBuffer -> bind ();

			collect (type);
			navigation ();

			depthBuffer -> unbind ();
			break;
		}
		case TraverseType::COLLISION:
		{
			// Collect for collide and gravite

			depthBuffer -> bind ();

			collect (type);
			collide ();

			depthBuffer -> unbind ();
			break;
		}
		case TraverseType::DISPLAY:
		{
			collect (type);
			draw ();
			break;
		}
		default:
			break;
	}

	getGlobalObjects () .clear ();
}

void
X3DRenderer::draw ()
{
	// Enable global lights

	for (const auto & object : getGlobalObjects ())
		object -> enable ();

	if (1)
	{
		// Sorted blend

		// Render opaque objects first

		glEnable (GL_DEPTH_TEST);
		glDepthMask (GL_TRUE);
		glDisable (GL_BLEND);

		for (const auto & shape : basic::make_range (shapes .cbegin (), numOpaqueShapes))
			shape -> draw ();

		// Render transparent objects

		glDepthMask (GL_FALSE);
		glEnable (GL_BLEND);

		std::sort (transparentShapes .begin (), transparentShapes .begin () + numTransparentShapes, ShapeContainerComp { });

		for (const auto & shape : basic::make_range (transparentShapes .cbegin (), numTransparentShapes))
			shape -> draw ();

		glDepthMask (GL_TRUE);
		glDisable (GL_BLEND);
	}
	else
	{
		//	http://wiki.delphigl.com/index.php/Blenden

		// Double blend

		glEnable (GL_DEPTH_TEST);

		// Render transparent objects

		std::sort (transparentShapes .begin (), transparentShapes .begin () + numTransparentShapes, ShapeContainerComp { });

		glEnable (GL_BLEND);

		for (const auto & shape : basic::make_range (transparentShapes .cbegin (), numTransparentShapes))
		{
			glDepthFunc (GL_GREATER);
			glDepthMask (GL_FALSE);
			glBlendFunc (GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);

			shape -> draw ();

			glDepthFunc (GL_LEQUAL);
			glDepthMask (GL_TRUE);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			shape -> draw ();
		}

		// Render opaque objects

		glDepthFunc (GL_GREATER);
		glDepthMask (GL_FALSE);
		glBlendFunc (GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
		glBlendFuncSeparate (GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		for (const auto & shape : basic::make_range (shapes .cbegin (), numOpaqueShapes))
			shape -> draw ();

		glDisable (GL_BLEND);
		glDepthFunc (GL_LEQUAL);
		glDepthMask (GL_TRUE);

		for (const auto & shape : basic::make_range (shapes .cbegin (), numOpaqueShapes))
			shape -> draw ();
	}

	// Disable global lights

	for (const auto & object : basic::make_reverse_range (getGlobalObjects ()))
		object -> disable ();

	// Reset to default OpenGL appearance

	getBrowser () -> getAppearance () -> draw ();
}

void
X3DRenderer::navigation ()
{
	// Measure distance

	// Get NavigationInfo values

	const auto navigationInfo = getCurrentNavigationInfo ();
	const auto viewpoint      = getCurrentViewpoint ();

	const double zNear = navigationInfo -> getNearPlane ();
	const double zFar  = navigationInfo -> getFarPlane (viewpoint);

	// Render all objects

	for (const auto & shape : basic::make_range (collisionShapes .cbegin (), numCollisionShapes))
		shape -> draw ();

	distance = depthBuffer -> getDistance (zNear, zFar);
}

void
X3DRenderer::collide ()
{
	// Collision

	std::vector <Collision*> collisions;

	const Sphere3f collisionSphere (getCurrentNavigationInfo () -> getCollisionRadius () * 1.1f, Vector3f ());

	for (const auto & shape : basic::make_range (collisionShapes .cbegin (), numCollisionShapes))
	{
		if (shape -> intersects (collisionSphere))
		{
			for (auto & collision : shape -> getCollisions ())
				collisions .emplace_back (collision);
		}
	}

	// Set isActive to FALSE for appropriate nodes

	std::vector <Collision*> difference;

	if (collisions .empty ())
		difference = std::move (activeCollisions);

	else
	{
		std::sort (collisions .begin (), collisions .end ());

		std::set_difference (activeCollisions .begin (), activeCollisions .end (),
		                     collisions .begin (), collisions .end (),
		                     std::back_inserter (difference));
	}

	for (const auto & collision : difference)
	{
		collision -> set_active (false);
		collision -> removeParent (this);
	}

	// Set isActive to TRUE for appropriate nodes

	activeCollisions = std::move (collisions);

	for (const auto & collision : activeCollisions)
	{
		collision -> addParent (this);
		collision -> set_active (true);
	}
}

void
X3DRenderer::gravite ()
{
	// Terrain following and gravitation

	if (getBrowser () -> getViewer () not_eq ViewerType::WALK)
		return;

	// Get NavigationInfo values

	const auto navigationInfo = getCurrentNavigationInfo ();
	const auto viewpoint      = getCurrentViewpoint ();

	const float zNear      = navigationInfo -> getNearPlane ();
	const float zFar       = navigationInfo -> getFarPlane (viewpoint);
	const float height     = navigationInfo -> getAvatarHeight ();
	const float stepHeight = navigationInfo -> getStepHeight ();

	// Bind buffer

	depthBuffer -> bind ();

	// Render as opaque objects

	for (const auto & shape : basic::make_range (collisionShapes .cbegin (), numCollisionShapes))
		shape -> draw ();

	// Get distance and unbind buffer

	float distance = depthBuffer -> getDistance (zNear, zFar);

	depthBuffer -> unbind ();

	// Gravite or step up

	if (zFar - distance > 0) // Are there polygons under the viewer
	{
		distance -= height;

		const Rotation4f up (Vector3f (0, 1, 0), getCurrentViewpoint () -> getUpVector ());

		if (distance > 0)
		{
			// Gravite and fall down the floor

			const float currentFrameRate = speed ? getBrowser () -> getCurrentFrameRate () : 1000000.0;

			speed -= getBrowser () -> getBrowserOptions () -> gravity () / currentFrameRate;

			float translation = speed / currentFrameRate;

			if (translation < -distance)
			{
				// The ground is reached.
				translation = -distance;
				speed       = 0;
			}

			getCurrentViewpoint () -> positionOffset () += Vector3f (0, translation, 0) * up;
		}
		else
		{
			speed = 0;

			if (-distance > 0.01 and - distance < stepHeight)
			{
				// Get size of camera
				const float size = getCurrentNavigationInfo () -> getCollisionRadius () * 2;

				// Step up
				Vector3f translation = getCurrentLayer () -> getTranslation (Vector3f (), size, size, Vector3f (0, -distance, 0) * up);

				if (getBrowser () -> getBrowserOptions () -> animateStairWalks ())
				{
					float step = getBrowser () -> getCurrentSpeed () / getBrowser () -> getCurrentFrameRate ();
					step = abs (getInverseCameraSpaceMatrix () .mult_matrix_dir (Vector3f (0, step, 0) * up));

					Vector3f offset = Vector3f (0, step, 0) * up;

					if (math::abs (offset) > math::abs (translation) or getBrowser () -> getCurrentSpeed () == 0)
						offset = translation;

					getCurrentViewpoint () -> positionOffset () += offset;
				}
				else
					getCurrentViewpoint () -> positionOffset () += translation;
			}
		}
	}
	else
	{
		speed = 0;
	}
}

void
X3DRenderer::dispose ()
{
	depthBuffer .reset ();
	activeCollisions .clear ();
}

X3DRenderer::~X3DRenderer ()
{ }

} // X3D
} // titania

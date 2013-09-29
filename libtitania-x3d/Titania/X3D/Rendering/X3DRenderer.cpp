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

#include "../Browser/X3DBrowser.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Shape/Appearance.h"
#include "../Rendering/ViewVolume.h"

#include <Titania/Utility/Adapter.h>
#include <algorithm>

namespace titania {
namespace X3D {

static constexpr size_t DEPTH_BUFFER_WIDTH  = 16;
static constexpr size_t DEPTH_BUFFER_HEIGHT = 16;

X3DRenderer::X3DRenderer () :
	             X3DNode (),
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
{ }

void
X3DRenderer::initialize ()
{
	depthBuffer .reset (new DepthBuffer (DEPTH_BUFFER_WIDTH, DEPTH_BUFFER_HEIGHT));
}

void
X3DRenderer::addShape (X3DShapeNode* shape)
{
	Matrix4f matrix   = ModelViewMatrix4f ();
	float    distance = getDistance (shape, matrix);

	if (distance < 0)
	{
		if (ViewVolume (matrix) .intersect (shape -> getBBox ()))
		{
			X3DFogObject*               fog         = getCurrentLayer () -> getFog ();
			const LightContainerArray & localLights = getCurrentLayer () -> getLocalLights ();

			if (shape -> isTransparent ())
			{
				if (numTransparentShapes < transparentShapes .size ())
					transparentShapes [numTransparentShapes] -> assign (shape, fog, localLights, matrix, distance);
				else
					transparentShapes .emplace_back (new ShapeContainer (shape, fog, localLights, matrix, distance));

				++ numTransparentShapes;
			}
			else
			{
				if (numOpaqueShapes < shapes .size ())
					shapes [numOpaqueShapes] -> assign (shape, fog, localLights, matrix, distance);
				else
					shapes .emplace_back (new ShapeContainer (shape, fog, localLights, matrix, distance));

				++ numOpaqueShapes;
			}
		}
	}
}

void
X3DRenderer::addCollision (X3DShapeNode* shape)
{
	Matrix4f matrix   = ModelViewMatrix4f ();
	float    distance = getDistance (shape, matrix);

	if (distance < 0)
	{
		if (ViewVolume (matrix) .intersect (shape -> getBBox ()))
		{
			const CollisionArray & collisions = getCurrentLayer () -> getCollisions ();

			if (numCollisionShapes < collisionShapes .size ())
				collisionShapes [numCollisionShapes] -> assign (shape, collisions, matrix, distance);
			else
				collisionShapes .emplace_back (new CollisionShape (shape, collisions, matrix, distance));

			++ numCollisionShapes;
		}
	}
}

float
X3DRenderer::getDistance (X3DShapeNode* shape, const Matrix4f & matrix)
{
	Box3f bbox  = shape -> getBBox () * matrix;
	float depth = bbox .size () .z () * 0.5f;

	return bbox .center () .z () - depth;
}

void
X3DRenderer::render (const TraverseType type)
{
	numOpaqueShapes      = 0;
	numTransparentShapes = 0;
	numCollisionShapes   = 0;

	getBrowser () -> getRenderers () .emplace (this);

	collect (type);

	switch (type)
	{
		case TraverseType::NAVIGATION:
		{
			navigation ();
			break;
		}
		case TraverseType::COLLISION:
		{
			collide ();
			break;
		}
		case TraverseType::COLLECT:
		{
			draw ();
			break;
		}
		default:
			break;
	}

	getBrowser () -> getRenderers () .pop ();
}

void
X3DRenderer::draw ()
{
	glPushMatrix ();

	// Enable global lights

	const LightContainerArray & globalLights = getCurrentLayer () -> getGlobalLights ();

	for (const auto & light : globalLights)
		light -> enable ();

	if (1)
	{
		// Sorted blend

		// Render opaque objects first

		glEnable (GL_DEPTH_TEST);
		glDepthMask (GL_TRUE);
		glDisable (GL_BLEND);

		for (const auto & shape : basic::adapter (shapes .cbegin (), shapes .cbegin () + numOpaqueShapes))
			shape -> draw ();

		// Render transparent objects

		glDepthMask (GL_FALSE);
		glEnable (GL_BLEND);

		std::stable_sort (transparentShapes .begin (), transparentShapes .begin () + numTransparentShapes, ShapeContainerComp ());

		for (const auto & shape : basic::adapter (transparentShapes .cbegin (), transparentShapes .cbegin () + numTransparentShapes))
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

		std::stable_sort (transparentShapes .begin (), transparentShapes .begin () + numTransparentShapes, ShapeContainerComp ());

		glEnable (GL_BLEND);

		for (const auto & shape : basic::adapter (transparentShapes .cbegin (), transparentShapes .cbegin () + numTransparentShapes))
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

		for (const auto & shape : basic::adapter (shapes .cbegin (), shapes .cbegin () + numOpaqueShapes))
			shape -> draw ();

		glDisable (GL_BLEND);
		glDepthFunc (GL_LEQUAL);
		glDepthMask (GL_TRUE);

		for (const auto & shape : basic::adapter (shapes .cbegin (), shapes .cbegin () + numOpaqueShapes))
			shape -> draw ();
	}

	// Disable global lights

	for (const auto & light : basic::adapter (globalLights .crbegin (), globalLights .crend ()))
		light -> disable ();

	glPopMatrix ();
	
	// Reset to default OpenGL appearance

	getBrowser () -> getBrowserOptions () -> appearance () -> draw ();
}

void
X3DRenderer::navigation ()
{
	// Measure distance

	// Bind buffer

	depthBuffer -> bind ();

	// Get NavigationInfo values

	auto navigationInfo = getCurrentNavigationInfo ();

	float zNear = navigationInfo -> getNearPlane ();
	float zFar  = navigationInfo -> getFarPlane ();

	// Render

	glPushMatrix ();

	{
		// Render opaque objects first

		glEnable (GL_DEPTH_TEST);
		glDepthMask (GL_TRUE);
		glDisable (GL_BLEND);

		for (const auto & shape : basic::adapter (collisionShapes .cbegin (), collisionShapes .cbegin () + numCollisionShapes))
			shape -> draw ();
	}

	glPopMatrix ();

	distance = depthBuffer -> getDistance (zNear, zFar);

	depthBuffer -> unbind ();
}

//void
//X3DRenderer::collide ()
//{
//return;
//
//	// Collision
//
//	// Bind buffer
//
//	depthBuffer -> bind ();
//
//	// Get NavigationInfo values
//
//	auto navigationInfo = getCurrentNavigationInfo ();
//
//	float zNear           = navigationInfo -> getNearPlane ();
//	float zFar            = navigationInfo -> getFarPlane ();
//	float collisionRadius = navigationInfo -> getCollisionRadius ();
//
//	// Transform viewpoint
//
//	auto viewpoint = getCurrentViewpoint ();
//
//	Matrix4f cameraSpaceMatrix = viewpoint -> getParentMatrix ();
//
//	cameraSpaceMatrix .translate (viewpoint -> getUserPosition ());
//	cameraSpaceMatrix .rotate (Rotation4f (Vector3f (0, 0, 1), -getBrowser () -> velocity));
//	//multMatrix (inverse (cameraSpaceMatrix));
//
//	// Render
//
//	glPushMatrix ();
//
//	{
//		// Render opaque objects first
//
//		glEnable (GL_DEPTH_TEST);
//		glDepthMask (GL_TRUE);
//		glDisable (GL_BLEND);
//
//		for (const auto & shape : basic::adapter (shapes .cbegin (), shapes .cbegin () + numOpaqueShapes))
//			shape -> draw ();
//
//		// Render transparent objects
//
//		for (const auto & shape : basic::adapter (transparentShapes .cbegin (), transparentShapes .cbegin () + numTransparentShapes))
//			shape -> draw ();
//	}
//
//	glPopMatrix ();
//
//	// Gravite or step up
//
//	Vector3f translation = getBrowser () -> velocity / (float) getBrowser () -> getCurrentFrameRate ();
//	float    length      = math::abs (translation);
//
//	if (length)
//	{
//		float distance = depthBuffer -> getDistance (zNear, zFar);
//
//		//__LOG__ << distance << std::endl;
//
//		if (zFar - distance > 0) // Are there polygons under the viewer
//		{
//			distance -= collisionRadius;
//
//			if (distance > 0)
//			{
//				// Move
//
//				if (length > distance)
//				{
//					// Collision: The ground is reached.
//					collisionRadius = length;
//					getCurrentViewpoint () -> positionOffset += normalize (translation) * distance;
//				}
//
//				else
//					getCurrentViewpoint () -> positionOffset += translation;
//			}
//			else
//			{
//				// Collision
//			}
//		}
//		else
//			getCurrentViewpoint () -> positionOffset += translation;
//
//		getBrowser () -> velocity = Vector3f ();
//	}
//
//	// Unbind buffer
//
//	depthBuffer -> unbind ();
//
//	{
//		bool     intersected = false;
//		Sphere3f collisionSphere (collisionRadius, Vector3f ());
//
//		std::deque <Vector3f> collisionNormal;
//
//		for (const auto & shape : basic::adapter (shapes .cbegin (), shapes .cbegin () + numOpaqueShapes))
//		{
//			if ((intersected = shape -> intersect (collisionSphere, collisionNormal)))
//				break;
//		}
//
//		if (not intersected)
//		{
//			for (const auto & shape : basic::adapter (transparentShapes .cbegin (), transparentShapes .cbegin () + numTransparentShapes))
//			{
//				if ((intersected = shape -> intersect (collisionSphere, collisionNormal)))
//					break;
//			}
//		}
//
//		if (intersected)
//		{
//			if (getBrowser () -> getViewerType () == ViewerType::WALK or getBrowser () -> getViewerType () == ViewerType::FLY)
//			{
//				if (collisionNormal .size () == 1)
//				{
//					collisionNormal [0] = cameraSpaceMatrix .multDirMatrix (collisionNormal [0]);
//
//					auto translation = inverse (getCurrentViewpoint () -> getParentMatrix ()) .multDirMatrix (collisionNormal [0]);
//
//					getCurrentViewpoint () -> positionOffset += translation;
//				}
//			}
//
//			__LOG__ << SFTime (chrono::now ()) << " : " << intersected << " : " << collisionNormal .size () << std::endl;
//		}
//	}
//}

void
X3DRenderer::collide ()
{
	// Collision

	std::deque <Collision*> collisions;

	Sphere3f collisionSphere (getCurrentNavigationInfo () -> getCollisionRadius () * 1.1f, Vector3f ());

	for (const auto & shape : basic::adapter (collisionShapes .cbegin (), collisionShapes .cbegin () + numCollisionShapes))
	{
		if (shape -> intersect (collisionSphere))
		{
			for (auto & collision : shape -> getCollisions ())
				collisions .emplace_back (collision);
		}
	}

	// Set isActive to FALSE for appropriate nodes

	std::deque <Collision*> difference;

	if (collisions .empty ())
		difference .assign (activeCollisions .begin (), activeCollisions .end ());

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

	activeCollisions .assign (collisions .begin (), collisions .end ());

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

	// Bind buffer

	depthBuffer -> bind ();

	// Get NavigationInfo values

	auto navigationInfo = getCurrentNavigationInfo ();

	float zNear      = navigationInfo -> getNearPlane ();
	float zFar       = navigationInfo -> getFarPlane ();
	float height     = navigationInfo -> getAvatarHeight ();
	float stepHeight = navigationInfo -> getStepHeight ();

	// Render

	glPushMatrix ();

	{
		// Render opaque objects first

		glEnable (GL_DEPTH_TEST);
		glDepthMask (GL_TRUE);
		glDisable (GL_BLEND);

		for (const auto & shape : basic::adapter (collisionShapes .cbegin (), collisionShapes .cbegin () + numCollisionShapes))
			shape -> draw ();
	}

	glPopMatrix ();

	// Get distance and unbind buffer

	float distance = depthBuffer -> getDistance (zNear, zFar);

	depthBuffer -> unbind ();

	// Gravite or step up

	if (zFar - distance > 0) // Are there polygons under the viewer
	{
		distance -= height;

		if (distance > 0)
		{
			// Gravite

			float currentFrameRate = speed ? getBrowser () -> getCurrentFrameRate () : 1000000;

			speed -= getBrowser () -> getBrowserOptions () -> gravity () / currentFrameRate;

			float translation = speed / currentFrameRate;

			if (translation < -distance)
			{
				// The ground is reached.
				translation = -distance;
				speed       = 0;
			}

			getCurrentViewpoint () -> positionOffset () += Vector3f (0, translation, 0);
		}
		else
		{
			speed = 0;

			if (-distance > 0.01 and - distance < stepHeight)
			{
				// Get size of camera
				float size = getCurrentNavigationInfo () -> getCollisionRadius () * 2;

				// Step up
				Vector3f translation = getCurrentLayer () -> getTranslation (Vector3f (), size, size, Vector3f (0, -distance, 0));

				if (getBrowser () -> getBrowserOptions () -> animateStairWalks ())
				{
					float step = getBrowser () -> getCurrentSpeed () / getBrowser () -> getCurrentFrameRate ();
					step = abs (getInverseCameraSpaceMatrix () .multMatrixDir (Vector3f (0, step, 0)));

					Vector3f offset (0, step, 0);

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
{
	for (const auto & shape : shapes)
		delete shape;

	shapes .clear ();

	for (const auto & shape : transparentShapes)
		delete shape;

	transparentShapes .clear ();

	for (const auto & shape : collisionShapes)
		delete shape;

	collisionShapes .clear ();
}

} // X3D
} // titania

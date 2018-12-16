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

#include "X3DRenderObject.h"

#include "../Browser/Core/BrowserOptions.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/CubeMapTexturing/GeneratedCubeMapTexture.h"
#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Navigation/NavigationInfo.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Rendering/X3DGeometryNode.h"
#include "../Components/Shaders/ComposedShader.h"
#include "../Components/Shape/Appearance.h"
#include "../Components/Shape/X3DShapeNode.h"
#include "../Rendering/FrameBuffer.h"
#include "../Rendering/CollisionContainer.h"
#include "../Rendering/ShapeContainer.h"

#include <Titania/Math/Geometry/Camera.h>
#include <Titania/Utility/Range.h>

#include <algorithm>

namespace titania {
namespace X3D {

static constexpr size_t DEPTH_BUFFER_WIDTH    = 16;
static constexpr size_t DEPTH_BUFFER_HEIGHT   = 16;
static constexpr auto   DEPTH_BUFFER_VIEWPORT = Vector4i (0, 0, DEPTH_BUFFER_WIDTH, DEPTH_BUFFER_HEIGHT);

static constexpr auto zAxis = Vector3d (0, 0, 1);

X3DRenderObject::X3DRenderObject () :
	             X3DBaseNode (),
	                   blend (),
	             depthOffset ({ 0 }),
	       cameraSpaceMatrix (),
	    invCameraSpaceMatrix (),
	        projectionMatrix (),
	         modelViewMatrix (),
	         viewVolumeStack (),
	            globalLights (),
	            localObjects (),
	              clipPlanes (),
	               localFogs (1),
	             localLights (),
	                  lights (),
	              lightIndex (0),
	                  shadow ({ false }),
	                 layouts (),
	generatedCubeMapTextures (),
	                 shaders (),
	              collisions (),
	            opaqueShapes (),
	       transparentShapes (),
	         collisionShapes (),
	        activeCollisions (),
	             depthShapes (),
	             depthBuffer (new FrameBuffer (getBrowser (), DEPTH_BUFFER_WIDTH, DEPTH_BUFFER_HEIGHT, 0, true)),
	                   speed (),
	         numOpaqueShapes (0),
	    numTransparentShapes (0),
	      numCollisionShapes (0),
	          numDepthShapes (0)
{
	addType (X3DConstants::X3DRenderObject);
}

void
X3DRenderObject::initialize ()
{
	depthBuffer -> setup (); // Throws a runtime error.
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <INSUFFICIENT_CAPABILITIES>, Error <DISPOSED
void
X3DRenderObject::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DRenderObject::dispose ();

	depthBuffer .reset (new FrameBuffer (executionContext -> getBrowser (), DEPTH_BUFFER_WIDTH, DEPTH_BUFFER_HEIGHT, 0, true));

	if (isInitialized ())
		depthBuffer -> setup (); // Throws a runtime error.
}

void
X3DRenderObject::setGlobalFog (Fog* const fog)
{
	localFogs [0] = std::make_shared <FogContainer> (getFog (), getFog () -> getModelMatrix () * getInverseCameraSpaceMatrix () .get ());
}

///  Contrains @a translation to a possible value the avatar can move.  If the avatar reaches and intersects with an
///  and obstacle and @a stepBack is true a translation in the opposite directiion is returned.  Future implementation will
///  will then return a value where the avatar slides along the wall. 
Vector3d
X3DRenderObject::constrainTranslation (const Vector3d & translation, const bool stepBack) const
{
	const auto navigationInfo = getNavigationInfo ();
	auto       distance       = getDistance (translation);

	// Constrain translation when the viewer collides with an obstacle.

	distance -= navigationInfo -> getCollisionRadius ();

	if (distance > 0)
	{
		// Move.

		const auto length = abs (translation);
		
		if (length > distance)
		{
			// Collision, the avatar would intersect with the obstacle.

			return normalize (translation) * distance;
		}

		// Everything is fine.

		return translation;
	}

	// Collision, the avatar is already within an obstacle.

	if (stepBack)
		return constrainTranslation (normalize (translation) * distance, false);

	return Vector3d ();
}

///  Returns the distance to the closest object in @a direction.  The maximum determinable value is avatarHeight * 2.
double
X3DRenderObject::getDistance (const Vector3d & direction) const
{
	try
	{
		ContextLock lock (getBrowser ());

		// Determine width and height of camera.

		const auto viewpoint       = getViewpoint ();
		const auto navigationInfo  = getNavigationInfo ();
		const auto collisionRadius = navigationInfo -> getCollisionRadius ();
		const auto bottom          = navigationInfo -> getStepHeight () - navigationInfo -> getAvatarHeight ();
		const auto nearValue       = navigationInfo -> getNearValue ();
		const auto avatarHeight    = navigationInfo -> getAvatarHeight ();

		// Reshape camera.

		const auto projectionMatrix = camera <double>::ortho (-collisionRadius, collisionRadius, std::min (bottom, -collisionRadius), collisionRadius, nearValue, std::max (collisionRadius * 2, avatarHeight * 2));

		// Translate camera to user position and to look in the direction of the @a direction.

		const auto localOrientation = ~Rotation4d (viewpoint -> orientation () .getValue ()) * viewpoint -> getOrientation ();
		auto       rotation         = Rotation4d (zAxis, -direction) * localOrientation;
	
		// The viewer is alway a straight box depending on the upVector.
		rotation *= viewpoint -> straightenHorizon (rotation);

		auto cameraSpaceProjectionMatrix = viewpoint -> getModelMatrix ();
		cameraSpaceProjectionMatrix .translate (viewpoint -> getUserPosition ());
		cameraSpaceProjectionMatrix .rotate (rotation);
		cameraSpaceProjectionMatrix .inverse ();

		cameraSpaceProjectionMatrix .mult_right (projectionMatrix);
		cameraSpaceProjectionMatrix .mult_left  (viewpoint -> getCameraSpaceMatrix ()); // !!! Must be from viewpoint.

		// Render depth.

		const_cast <X3DRenderObject*> (this) -> getProjectionMatrix () .push (cameraSpaceProjectionMatrix);

		const auto depth = getDepth (projectionMatrix);

		const_cast <X3DRenderObject*> (this) -> getProjectionMatrix () .pop ();

		return -depth;
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }
	catch (const std::domain_error &)
	{ }

	return 0;
}

///  Returns the depth value to the closest object.  The maximum determinable value is avatarHeight * 2.
double
X3DRenderObject::getDepth (const Matrix4d & projectionMatrix) const
{
	static const ViewVolume viewVolume (Matrix4d (), DEPTH_BUFFER_VIEWPORT, DEPTH_BUFFER_VIEWPORT);

	// Render all objects

	depthBuffer -> bind ();

	const_cast <X3DRenderObject*> (this) -> getViewVolumes () .emplace_back (viewVolume);
	const_cast <X3DRenderObject*> (this) -> depth (collisionShapes, numCollisionShapes);
	const_cast <X3DRenderObject*> (this) -> getViewVolumes () .pop_back ();

	// Get distance from depth buffer

	const auto depth = depthBuffer -> getDepth (projectionMatrix, DEPTH_BUFFER_VIEWPORT);

	depthBuffer -> unbind ();

	return depth;
}

void
X3DRenderObject::render (const TraverseType type, const TraverseFunction & traverse)
{
	switch (type)
	{
		case TraverseType::COLLISION:
		{
			// Collect for collide and gravite
			numCollisionShapes = 0;

			traverse (type, this);
			collide ();
			gravite ();
			break;
		}
		case TraverseType::DEPTH:
		{
			numDepthShapes = 0;

			traverse (type, this);
			depth (depthShapes, numDepthShapes);
			break;
		}
		case TraverseType::DISPLAY:
		{
			lightIndex           = 0;
			numOpaqueShapes      = 0;
			numTransparentShapes = 0;

			setGlobalFog (getFog ());

			traverse (type, this);
			draw (traverse);
			break;
		}
		default:
			break;
	}
}

const std::shared_ptr <LightContainer> &
X3DRenderObject::getLightContainer () const
{
	return lights [const_cast <size_t &> (lightIndex) ++];
}

bool
X3DRenderObject::addCollisionShape (X3DShapeNode* const shapeNode)
{
	const auto   bbox       = shapeNode -> getBBox () * getModelViewMatrix () .get ();
	const auto & viewVolume = getViewVolumes () .back ();

	if (not viewVolume .intersects (bbox))
		return false;

	if (numCollisionShapes == collisionShapes .size ())
		collisionShapes .emplace_back (new CollisionContainer ());

	const auto & context = collisionShapes [numCollisionShapes];

	++ numCollisionShapes;

	context -> setScissor (getViewVolumes () .back () .getScissor ());
	context -> setModelViewMatrix (getModelViewMatrix () .get ());
	context -> setShape (shapeNode);
	context -> setCollisions (getCollisions ());
	context -> setLocalObjects (getLocalObjects ());
	context -> setClipPlanes (getClipPlanes ());

	return true;
}

bool
X3DRenderObject::addDepthShape (X3DShapeNode* const shapeNode)
{
	const auto   bbox       = shapeNode -> getBBox () * getModelViewMatrix () .get ();
	const auto & viewVolume = getViewVolumes () .back ();

	if (not viewVolume .intersects (bbox))
		return false;

	// It should be possible to sort out shapes that are far away.

	if (numDepthShapes == depthShapes .size ())
		depthShapes .emplace_back (new CollisionContainer ());

	const auto & context = depthShapes [numDepthShapes];

	++ numDepthShapes;

	context -> setScissor (getViewVolumes () .back () .getScissor ());
	context -> setModelViewMatrix (getModelViewMatrix () .get ());
	context -> setShape (shapeNode);
	context -> setLocalObjects (getLocalObjects ());
	context -> setClipPlanes (getClipPlanes ());

	return true;
}

bool
X3DRenderObject::addDisplayShape (X3DShapeNode* const shapeNode)
{
	const auto bbox   = shapeNode -> getBBox () * getModelViewMatrix () .get ();
	const auto depth  = bbox .size   () .z () / 2;
	const auto min    = bbox .center () .z () - depth;
	const auto center = bbox .center () .z () + getDepthOffset () .top ();

	if (min > 0)
	   return false;

	const auto & viewVolume = getViewVolumes () .back ();

	if (not viewVolume .intersects (bbox))
		return false;

   ShapeContainer* context = nullptr;

	if (getBlend () .empty () ? shapeNode -> isTransparent () : getBlend () .top ())
	{
		if (numTransparentShapes == transparentShapes .size ())
			transparentShapes .emplace_back (new ShapeContainer (this, true));

		context = transparentShapes [numTransparentShapes] .get ();

		++ numTransparentShapes;
	}
	else
	{
		if (numOpaqueShapes == opaqueShapes .size ())
			opaqueShapes .emplace_back (new ShapeContainer (this, false));

		context = opaqueShapes [numOpaqueShapes] .get ();

		++ numOpaqueShapes;
	}

	context -> setScissor (viewVolume .getScissor ());
	context -> setModelViewMatrix (getModelViewMatrix () .get ());
	context -> setShape (shapeNode);
	context -> setFog (getLocalFogs () .back ());
	context -> setLocalObjects (getLocalObjects ());
	context -> setClipPlanes (getClipPlanes ());
	context -> setLocalLights (getLocalLights ());
	context -> setShadow (getShadow ());
	context -> setDistance (center);

	return true;
}

void
X3DRenderObject::collide ()
{
	// Collision

	// TODO: transform collisionBox by getNavigationInfo () -> getModelMatrix ()

	const auto collisionRadius2 = 2.2 * getNavigationInfo () -> getCollisionRadius ();
	const auto collisionBox     = Box3d (Vector3d (collisionRadius2, collisionRadius2, collisionRadius2), Vector3d ());

	std::vector <Collision*> collisions;

	for (const auto & context : basic::make_range (collisionShapes .cbegin (), numCollisionShapes))
	{
	   try
	   {
			if (context -> intersects (collisionBox))
			{
				for (auto & collision : context -> getCollisions ())
					collisions .emplace_back (collision);
			}
		}
		catch (const std::domain_error &)
		{ }
	}

	// Set isActive to FALSE for appropriate nodes

	std::vector <Collision*> difference;

	if (collisions .empty ())
	{
		difference = std::move (activeCollisions);
	}
	else
	{
		std::sort (collisions .begin (), collisions .end ());

		std::set_difference (activeCollisions .cbegin (), activeCollisions .cend (),
		                     collisions .cbegin (), collisions .cend (),
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
X3DRenderObject::gravite ()
{
	try
	{
		// Terrain following and gravitation

		if (getBrowser () -> getActiveLayer () == getLayer ())
		{
			if (getBrowser () -> getCurrentViewer () not_eq X3DConstants::WalkViewer)
				return;
		}
		else if (getNavigationInfo () -> getViewer () not_eq X3DConstants::WalkViewer)
			return;

		// Get NavigationInfo values

		const auto navigationInfo  = getNavigationInfo ();
		const auto viewpoint       = getViewpoint ();
		const auto collisionRadius = navigationInfo -> getCollisionRadius ();
		const auto nearValue       = navigationInfo -> getNearValue ();
		const auto avatarHeight    = navigationInfo -> getAvatarHeight ();
		const auto stepHeight      = navigationInfo -> getStepHeight ();

		// Reshape viewpoint for gravite.

		const auto projectionMatrix = camera <double>::ortho (-collisionRadius, collisionRadius, -collisionRadius, collisionRadius, nearValue, std::max (collisionRadius * 2, avatarHeight * 2));
					
		// Transform viewpoint to look down the up vector

		const auto upVector = viewpoint -> getUpVector ();
		const auto down     = Rotation4d (zAxis, upVector);

		auto cameraSpaceProjectionMatrix = viewpoint -> getModelMatrix ();
		cameraSpaceProjectionMatrix .translate (viewpoint -> getUserPosition ());
		cameraSpaceProjectionMatrix .rotate (down);
		cameraSpaceProjectionMatrix .inverse ();

		cameraSpaceProjectionMatrix .mult_right (projectionMatrix);
		cameraSpaceProjectionMatrix .mult_left  (viewpoint -> getCameraSpaceMatrix ()); // !!! Must be from viewpoint.

		// Render depth.

		getProjectionMatrix () .push (cameraSpaceProjectionMatrix);

		auto distance = -getDepth (projectionMatrix);

		getProjectionMatrix () .pop ();

		// Gravite or step up

		const Rotation4d up (Vector3d (0, 1, 0), upVector);

		distance -= avatarHeight;

		if (distance > 0)
		{
			// Gravite and fall down to the floor.

			const auto currentFrameRate = speed ? getBrowser () -> getCurrentFrameRate () : 1000000.0;

			speed -= getBrowser () -> getBrowserOptions () -> Gravity () / currentFrameRate;

			auto translation = speed / currentFrameRate;

			if (translation < -distance)
			{
				// The ground is reached.
				translation = -distance;
				speed       = 0;
			}

			getViewpoint () -> positionOffset () += Vector3d (0, translation, 0) * up;
		}
		else
		{
			speed = 0;

			distance = -distance;

			if (distance > 0.01 and distance < stepHeight)
			{
				// Step up
				const auto translation = constrainTranslation (Vector3d (0, distance, 0) * up, false);

				if (getBrowser () -> getBrowserOptions () -> AnimateStairWalks ())
				{
					auto step = getBrowser () -> getCurrentSpeed () / getBrowser () -> getCurrentFrameRate ();

					step = abs (viewpoint -> getInverseCameraSpaceMatrix () .mult_matrix_dir (Vector3d (0, step, 0) * up));

					Vector3d offset = Vector3d (0, step, 0) * up;

					if (math::abs (offset) > math::abs (translation) or getBrowser () -> getCurrentSpeed () == 0)
						offset = translation;

					getViewpoint () -> positionOffset () += offset;
				}
				else
					getViewpoint () -> positionOffset () += translation;
			}
		}
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DRenderObject::depth (const CollisionContainerArray & shapes, const size_t numShapes)
{
	const auto & viewport = getViewVolumes () .back () .getViewport ();

	// Setup projection matrix.

	glMatrixMode (GL_PROJECTION);
	glLoadMatrixd (getProjectionMatrix () .get () .front () .data ());
	glMatrixMode (GL_MODELVIEW);

	// Render to depth buffer.

	glViewport (viewport [0], viewport [1], viewport [2], viewport [3]);
	glScissor  (viewport [0], viewport [1], viewport [2], viewport [3]);

	glClear (GL_DEPTH_BUFFER_BIT);

	glEnable (GL_DEPTH_TEST);
	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);
	glDisable (GL_CULL_FACE);

	for (const auto & context : basic::make_range (shapes .cbegin (), numShapes))
		context -> depth ();
}

void
X3DRenderObject::draw (const TraverseFunction & traverse)
{
	static constexpr auto comp = ShapeContainerComp { };

	// PREPARATIONS

	if (isIndependent ())
	{
		// Render shadow maps.
	
		for (const auto & object : getLights ())
			object -> renderShadowMap (this);
	
		// Render generated cube map textures.
	
		for (const auto & generatedCubeMapTexture : getGeneratedCubeMapTextures ())
			generatedCubeMapTexture -> renderTexture (this, traverse);
	}

	// BEGIN DRAW

	// Enable global lights.

	for (const auto & object : getGlobalLights ())
		object -> enable ();

	// Set global uniforms.

	if (not getBrowser () -> getFixedPipelineRequired ())
	{
		getBrowser () -> getPointShader ()     -> setGlobalUniforms (this);
		getBrowser () -> getWireframeShader () -> setGlobalUniforms (this);
		getBrowser () -> getDefaultShader ()   -> setGlobalUniforms (this);
		getBrowser () -> getShadowShader ()    -> setGlobalUniforms (this);
	}

	for (const auto & shaderNode : getShaders ())
		shaderNode -> setGlobalUniforms (this);

	glUseProgram (0);
	glBindProgramPipeline (0);

	// Configure viewport.

	const auto & viewport = getViewVolumes () .back () .getViewport ();

	glViewport (viewport [0], viewport [1], viewport [2], viewport [3]);
	glScissor  (viewport [0], viewport [1], viewport [2], viewport [3]);

	// Setup projection matrix
	// for fixed pipeline, background, particle systems.
	glMatrixMode (GL_PROJECTION);
	glLoadMatrixd (getProjectionMatrix () .get () .front () .data ());
	glMatrixMode (GL_MODELVIEW);

	// Draw background.

	glClear (GL_DEPTH_BUFFER_BIT);

	getBackground () -> draw (this, viewport);

	// Sorted blend.

	// Render opaque objects first.

	glDisable (GL_BLEND);
	glEnable (GL_DEPTH_TEST);
	glDepthMask (GL_TRUE);

	for (const auto & context : basic::make_range (opaqueShapes .cbegin (), numOpaqueShapes))
		context -> display ();

	// Render transparent objects.

	glEnable (GL_BLEND);
	glDepthMask (GL_FALSE);

	std::stable_sort (transparentShapes .begin (), transparentShapes .begin () + numTransparentShapes, comp);

	for (const auto & context : basic::make_range (transparentShapes .cbegin (), numTransparentShapes))
		context -> display ();

	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);

	// POST DRAW

	// Disable global lights
	
	for (const auto & object : basic::make_reverse_range (getGlobalLights ()))
		object -> disable ();

	if (getBrowser () -> getFixedPipelineRequired ())
	{
		// Reset to default OpenGL appearance

		ShapeContainer context (this, false);

		getBrowser () -> getDefaultAppearance () -> enable (&context);
	}

	// Clear node arrays.

	getShaders                  () .clear ();
	getGeneratedCubeMapTextures () .clear ();
	getGlobalLights             () .clear ();
	getLights                   () .clear ();
}

void
X3DRenderObject::dispose ()
{
	viewVolumeStack          .clear ();
	generatedCubeMapTextures .clear ();
	globalLights             .clear ();
	localObjects             .clear ();
	clipPlanes               .clear ();
	localFogs                .clear ();
	localLights              .clear ();
	lights                   .clear ();
	layouts                  .clear ();
	shaders                  .clear ();
	collisions               .clear ();

	lightIndex = 0;

	opaqueShapes      .clear ();
	transparentShapes .clear ();
	collisionShapes   .clear ();
	activeCollisions  .clear ();
	depthShapes       .clear ();

	depthBuffer .reset ();
	speed = 0;

	numOpaqueShapes      = 0;
	numTransparentShapes = 0;
	numCollisionShapes   = 0;
	numDepthShapes       = 0;
}

X3DRenderObject::~X3DRenderObject ()
{ }

} // X3D
} // titania

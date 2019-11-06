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

#include "SoftSystem.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Core/BrowserOptions.h"
#include "../../Browser/ParticleSystems/BVH.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/ParticleSystems/BoundedPhysicsModel.h"
#include "../../Components/ParticleSystems/X3DParticleEmitterNode.h"
#include "../../Components/ParticleSystems/X3DParticlePhysicsModelNode.h"
#include "../../Components/Rendering/X3DColorNode.h"
#include "../../Components/Rendering/X3DGeometryNode.h"
#include "../../Components/Shape/X3DAppearanceNode.h"
#include "../../Components/Texturing/X3DTextureCoordinateNode.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Browser/ParticleSystems/Random.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderObject.h"

#include <limits>

namespace titania {
namespace X3D {

SoftSystem::SoftSystem (ParticleSystem* const particleSystem) :
	          particleSystem (particleSystem),
	           primitiveMode (GL_POINTS),
	              idBufferId (0),
	        positionBufferId (0),
	     elapsedTimeBufferId (0),
	            lifeBufferId (0),
	           colorBufferId (0),
	       texCoordBufferIds (1, 0),
	          normalBufferId (0),
	          vertexBufferId (0),
	           positionArray (),
	        elapsedTimeArray (),
	               lifeArray (),
	              colorArray (),
	           texCoordArray (),
	             normalArray (),
	             vertexArray (),
	           texCoordCount (0),
	             vertexCount (0),
	      shaderGeometryType (0),
	               particles (),
	           colorMaterial (false),
	            texCoordAnim (false),
	              velocities (),
	                  speeds (),
	             turbulences (),
	               numForces (0),
	            geometryType (ParticleSystem::GeometryType::POINT),
	         createParticles (false),
	        particleLifetime (0),
	       lifetimeVariation (0),
	            maxParticles (0),
	            numParticles (0),
	            creationTime (0),
	               pauseTime (0),
	               deltaTime (0),
	               colorRamp (),
	            texCoordRamp (),
	           boundedVolume (),
	             emitterNode (),
          physicsModelNodes (),
	boundedPhysicsModelNodes (),
              colorRampNode (),
           texCoordRampNode (),
	              shaderNode ()
{
	particleSystem -> addChildObjects (shaderNode,
	                                   emitterNode,
                                      physicsModelNodes,
                                      boundedPhysicsModelNodes);
}

void
SoftSystem::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (particleSystem -> isInitialized ())
	{
		executionContext -> getBrowser () -> getBrowserOptions () -> Shading () .removeInterest (&SoftSystem::set_shader, this);

		executionContext -> getBrowser () -> sensorEvents () .removeInterest (&SoftSystem::animateParticles, this);
		executionContext -> isLive () .removeInterest (&SoftSystem::set_live, this);
	}

	if (particleSystem -> isInitialized ())
	{
		particleSystem -> getBrowser () -> getBrowserOptions () -> Shading () .addInterest (&SoftSystem::set_shader, this);

		particleSystem -> getExecutionContext () -> isLive () .addInterest (&SoftSystem::set_live, this);

		set_live ();
		set_emitter ();
		set_shader ();
	}
}

void
SoftSystem::initialize ()
{
	particleSystem -> isLive () .addInterest (&SoftSystem::set_live, this);
	particleSystem -> getExecutionContext () -> isLive () .addInterest (&SoftSystem::set_live, this);

	particleSystem -> getBrowser () -> getBrowserOptions () -> Shading () .addInterest (&SoftSystem::set_shader, this);
	//particleSystem -> getBrowser () -> getDefaultShader () .addInterest (&SoftSystem::set_shader, this);

	particleSystem -> enabled ()            .addInterest (&SoftSystem::set_enabled,           this);
	particleSystem -> createParticles ()    .addInterest (&SoftSystem::set_createParticles,   this);
	particleSystem -> geometryType ()       .addInterest (&SoftSystem::set_geometryType,      this);
	particleSystem -> maxParticles ()       .addInterest (&SoftSystem::set_enabled,           this);
	particleSystem -> particleLifetime ()   .addInterest (&SoftSystem::set_particleLifetime,  this);
	particleSystem -> lifetimeVariation ()  .addInterest (&SoftSystem::set_lifetimeVariation, this);
	particleSystem -> emitter ()            .addInterest (&SoftSystem::set_emitter,           this);
	particleSystem -> physics ()            .addInterest (&SoftSystem::set_physics,           this);
	particleSystem -> colorKey ()           .addInterest (&SoftSystem::set_color,             this);
	particleSystem -> colorRamp ()          .addInterest (&SoftSystem::set_colorRamp,         this);
	particleSystem -> texCoordKey ()        .addInterest (&SoftSystem::set_texCoord,          this);
	particleSystem -> texCoordRamp ()       .addInterest (&SoftSystem::set_texCoordRamp,      this);

	boundedPhysicsModelNodes .addInterest (&SoftSystem::set_boundedPhysics, this);

	glGenBuffers (1, &idBufferId);
	glGenBuffers (1, &positionBufferId);
	glGenBuffers (1, &elapsedTimeBufferId);
	glGenBuffers (1, &lifeBufferId);
	glGenBuffers (1, &colorBufferId);
	glGenBuffers (1, texCoordBufferIds .data ());
	glGenBuffers (1, &normalBufferId);
	glGenBuffers (1, &vertexBufferId);

	for (size_t i = 1, channels = particleSystem -> getBrowser () -> getMaxTextures (); i < channels; ++ i)
		texCoordBufferIds .emplace_back (texCoordBufferIds [0]);

	// Call order is higly important at startup.
	set_emitter ();
	set_enabled ();
	set_createParticles ();
	set_particleLifetime ();
	set_lifetimeVariation ();
	set_physics ();
	set_colorRamp ();
	set_texCoordRamp ();
}

Box3d
SoftSystem::getBBox () const
{
	if (particleSystem -> bboxSize () == Vector3f (-1, -1, -1))
	{
		if (emitterNode)
			return emitterNode -> getBBox ();

		return Box3d ();
	}

	return Box3d (particleSystem -> bboxSize () .getValue (), particleSystem -> bboxCenter () .getValue ());
}

bool
SoftSystem::isTransparent () const
{
	if (particleSystem -> getAppearance () -> isTransparent ())
		return true;

	if (colorRampNode and colorRampNode -> isTransparent ())
		return true;

	if (geometryType == ParticleSystem::GeometryType::POINT)
		return true;

	if (geometryType == ParticleSystem::GeometryType::GEOMETRY and particleSystem -> getGeometry ())
		return particleSystem -> getGeometry () -> isTransparent ();

	return false;
}

void
SoftSystem::set_live ()
{
	if (particleSystem -> getExecutionContext () -> isLive () and particleSystem -> isLive ())
	{
		if (particleSystem -> isActive () and particleSystem -> maxParticles ())
		{
			particleSystem -> getBrowser () -> sensorEvents () .addInterest (&SoftSystem::animateParticles, this);

			if (pauseTime)
			{
				creationTime += SFTime::now () - pauseTime;
				pauseTime     = 0;
			}
		}
	}
	else
	{
		if (particleSystem -> isActive () and particleSystem -> maxParticles ())
		{
			particleSystem -> getBrowser () -> sensorEvents () .removeInterest (&SoftSystem::animateParticles, this);

			if (pauseTime == 0)
				pauseTime = SFTime::now ();
		}
	}
}

void
SoftSystem::set_enabled ()
{
	if (particleSystem -> enabled () and particleSystem -> maxParticles ())
	{
		if (not particleSystem -> isActive ())
		{
			if (particleSystem -> getExecutionContext () -> isLive () and particleSystem -> isLive ())
			{
				particleSystem -> getBrowser () -> sensorEvents () .addInterest (&SoftSystem::animateParticles, this);

				pauseTime = 0;
			}
			else
				pauseTime = SFTime::now ();

			particleSystem -> isActive () = true;
		}
	}
	else
	{
		if (particleSystem -> isActive ())
		{
			if (particleSystem -> getExecutionContext () -> isLive () and particleSystem -> isLive ())
			{
				particleSystem -> getBrowser () -> sensorEvents () .removeInterest (&SoftSystem::animateParticles, this);
			}

			particleSystem -> isActive () = false;

			numParticles = 0;
		}
	}

	set_maxParticles ();
}

void
SoftSystem::set_createParticles ()
{
	createParticles = particleSystem -> createParticles ();
}

void
SoftSystem::set_geometryType ()
{
	// geometryType

	static const std::map <std::string, ParticleSystem::GeometryType> geometryTypes = {
		std::pair ("POINT",    ParticleSystem::GeometryType::POINT),
		std::pair ("LINE",     ParticleSystem::GeometryType::LINE),
		std::pair ("TRIANGLE", ParticleSystem::GeometryType::TRIANGLE),
		std::pair ("QUAD",     ParticleSystem::GeometryType::QUAD),
		std::pair ("SPRITE",   ParticleSystem::GeometryType::SPRITE),
		std::pair ("GEOMETRY", ParticleSystem::GeometryType::GEOMETRY),
	};

	try
	{
		geometryType = geometryTypes .at (particleSystem -> geometryType ());
	}
	catch (const std::out_of_range &)
	{
		geometryType = ParticleSystem::GeometryType::POINT;
	}

	// Create buffers

	std::vector <int32_t> idArray (maxParticles);

	switch (geometryType)
	{
		case ParticleSystem::GeometryType::POINT:
		{
			idArray          .resize (maxParticles);
			positionArray    .resize (maxParticles);
			elapsedTimeArray .resize (maxParticles);
			lifeArray        .resize (maxParticles);
			colorArray       .resize (maxParticles);
			texCoordArray    .resize (0);
			normalArray      .resize (0);
			vertexArray      .resize (maxParticles);

			std::iota (idArray     .begin (), idArray     .end (), 0);
			std::fill (lifeArray   .begin (), lifeArray   .end (), 1);
			std::fill (colorArray  .begin (), colorArray  .end (), Color4f (1, 1, 1, 1));
			std::fill (vertexArray .begin (), vertexArray .end (), Vector3d (0, 0, 0));

			texCoordCount = 0;
			vertexCount   = 1;
			break;
		}
		case ParticleSystem::GeometryType::LINE:
		{
			idArray          .resize (2 * maxParticles);
			positionArray    .resize (2 * maxParticles);
			elapsedTimeArray .resize (2 * maxParticles);
			lifeArray        .resize (2 * maxParticles);
			colorArray       .resize (2 * maxParticles);
			texCoordArray    .resize (0);
			normalArray      .resize (0);
			vertexArray      .resize (2 * maxParticles);

			for (size_t i = 0, size = idArray .size (); i < size; ++ i)
				idArray [i] = i / 2;

			std::fill (lifeArray   .begin (), lifeArray   .end (), 1);
			std::fill (colorArray  .begin (), colorArray  .end (), Color4f (1, 1, 1, 1));
			std::fill (vertexArray .begin (), vertexArray .end (), Vector3d (0, 0, 0));

			texCoordCount = 2;
			vertexCount   = 2;
			break;
		}
		case ParticleSystem::GeometryType::TRIANGLE:
		case ParticleSystem::GeometryType::QUAD:
		case ParticleSystem::GeometryType::SPRITE:
		{
			idArray          .resize (6 * maxParticles);
			positionArray    .resize (6 * maxParticles);
			elapsedTimeArray .resize (6 * maxParticles);
			lifeArray        .resize (6 * maxParticles);
			colorArray       .resize (6 * maxParticles);
			texCoordArray    .resize (6 * maxParticles);
			normalArray      .resize (6 * maxParticles);
			vertexArray      .resize (6 * maxParticles);

			for (size_t i = 0, size = idArray .size (); i < size; ++ i)
				idArray [i] = i / 6;

			std::fill (lifeArray   .begin (), lifeArray   .end (), 1);
			std::fill (colorArray  .begin (), colorArray  .end (), Color4f (1, 1, 1, 1));
			std::fill (normalArray .begin (), normalArray .end (), Vector3f (0, 0, 1));
			std::fill (vertexArray .begin (), vertexArray .end (), Vector3d (0, 0, 0));

			glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
			glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * normalArray .size (), normalArray .data (), GL_STATIC_DRAW);

			for (size_t i = 0, size = 6 * maxParticles; i < size; i += 6)
			{
				// p4 ------ p3
				// |       / |
				// |     /   |
				// |   /     |
				// | /       |
				// p1 ------ p2

				texCoordArray [i + 0] = texCoordArray [i + 3] = Vector4f (0, 0, 0, 1); // p1
				texCoordArray [i + 1] = Vector4f (1, 0, 0, 1);                         // p2
				texCoordArray [i + 2] = texCoordArray [i + 4] = Vector4f (1, 1, 0, 1); // p3
				texCoordArray [i + 5] = Vector4f (0, 1, 0, 1);                         // p4
			}

			glBindBuffer (GL_ARRAY_BUFFER, texCoordBufferIds [0]);
			glBufferData (GL_ARRAY_BUFFER, sizeof (Vector4f) * texCoordArray .size (), texCoordArray .data (), GL_STATIC_DRAW);
			glBindBuffer (GL_ARRAY_BUFFER, 0);

			texCoordCount = 4;
			vertexCount   = 6;
			break;
		}
		case ParticleSystem::GeometryType::GEOMETRY:
		{
			texCoordCount = 0;
			vertexCount   = 0;
			break;
		}
	}

	glBindBuffer (GL_ARRAY_BUFFER, idBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (int32_t) * idArray .size (), idArray .data (), GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	set_shader ();
}

void
SoftSystem::set_shader ()
{
	switch (geometryType)
	{
		case ParticleSystem::GeometryType::POINT:
		{
			primitiveMode      = GL_POINTS;
			shaderGeometryType = 0;
			shaderNode         = particleSystem -> getBrowser () -> getPointShader ();
			break;
		}
		case ParticleSystem::GeometryType::LINE:
		{
			primitiveMode      = GL_LINES;
			shaderGeometryType = 1;
			shaderNode         = particleSystem -> getBrowser () -> getWireframeShader ();
			break;
		}
		case ParticleSystem::GeometryType::TRIANGLE:
		case ParticleSystem::GeometryType::QUAD:
		case ParticleSystem::GeometryType::SPRITE:
		{
			primitiveMode      = GL_TRIANGLES;
			shaderGeometryType = 3;
			shaderNode         = particleSystem -> getBrowser () -> getDefaultShader ();
			break;
		}
		case ParticleSystem::GeometryType::GEOMETRY:
		{
			primitiveMode      = GL_TRIANGLES; // determine from geometry node.
			shaderGeometryType = 3;  // determine from geometry node.
			shaderNode         = particleSystem -> getBrowser () -> getDefaultShader ();
			break;
		}
	}
}

void
SoftSystem::set_maxParticles ()
{
	maxParticles = std::max <int32_t> (0, particleSystem -> maxParticles ());

	for (size_t i = numParticles, size = std::min (particles .size (), maxParticles); i < size; ++ i)
	{
		particles [i] .life     = 1;
		particles [i] .lifetime = -1;
	}

	particles .resize (maxParticles);

	numParticles = std::min (numParticles, maxParticles);

	if (not emitterNode -> isExplosive ())
		creationTime = SFTime::now ();

	set_geometryType ();
}

void
SoftSystem::set_particleLifetime ()
{
	particleLifetime = particleSystem -> particleLifetime ();
}

void
SoftSystem::set_lifetimeVariation ()
{
	lifetimeVariation = particleSystem -> lifetimeVariation ();
}

void
SoftSystem::set_emitter ()
{
	emitterNode = x3d_cast <X3DParticleEmitterNode*> (particleSystem -> emitter ());

	if (not emitterNode)
		emitterNode = particleSystem -> getBrowser () -> getDefaultEmitter ();

	createParticles = particleSystem -> createParticles ();
}

void
SoftSystem::set_physics ()
{
	// X3DParticlePhysicsModelNode

	physicsModelNodes .clear ();

	for (const auto & node : particleSystem -> physics ())
	{
		const auto physicsNode = x3d_cast <X3DParticlePhysicsModelNode*> (node);

		if (physicsNode)
			physicsModelNodes .emplace_back (physicsNode);
	}

	// BoundedPhysicsModel

	for (const auto & physicsNode : boundedPhysicsModelNodes)
		physicsNode -> removeInterest (boundedPhysicsModelNodes);

	boundedPhysicsModelNodes .clear ();

	for (const auto & node : particleSystem -> physics ())
	{
		const auto physicsNode = x3d_cast <BoundedPhysicsModel*> (node);

		if (physicsNode)
		{
			boundedPhysicsModelNodes .emplace_back (physicsNode);
			physicsNode -> addInterest (boundedPhysicsModelNodes);
		}
	}
}

void
SoftSystem::set_boundedPhysics ()
{
	std::vector <Vector3f> boundedNormals;
	std::vector <Vector3f> boundedVertices;

	for (const auto & boundedPhysicsModelNode : boundedPhysicsModelNodes)
	{
		boundedPhysicsModelNode -> addGeometry (boundedNormals, boundedVertices);
	}

	if (boundedVertices .empty ())
		boundedVolume .reset ();
	else
		boundedVolume = std::make_unique <BVH <float>> (std::move (boundedNormals), std::move (boundedVertices));
}

void
SoftSystem::set_colorRamp ()
{
	if (colorRampNode)
		colorRampNode -> removeInterest (&SoftSystem::set_color, this);

	colorRampNode = x3d_cast <X3DColorNode*> (particleSystem -> colorRamp ());

	if (colorRampNode)
		colorRampNode -> addInterest (&SoftSystem::set_color, this);

	set_color ();
}

void
SoftSystem::set_color ()
{
	if (colorRampNode)
	{
		colorRamp .resize (colorRampNode -> getSize ());

		for (size_t i = 0, size = colorRampNode -> getSize (); i < size; ++ i)
			colorRamp [i] = colorRampNode -> get1Color (i);
	}

	colorRamp .resize (particleSystem -> colorKey () .size (), Color4f (1, 1, 1, 1));

	colorMaterial = particleSystem -> colorKey () .size () and colorRampNode;
}

void
SoftSystem::set_texCoordRamp ()
{
	if (texCoordRampNode)
		texCoordRampNode -> removeInterest (&SoftSystem::set_texCoord, this);

	texCoordRampNode = x3d_cast <X3DTextureCoordinateNode*> (particleSystem -> texCoordRamp ());

	if (texCoordRampNode)
		texCoordRampNode -> addInterest (&SoftSystem::set_texCoord, this);

	set_texCoord ();
}

void
SoftSystem::set_texCoord ()
{
	if (texCoordRampNode)
	{
		texCoordRamp .resize (texCoordRampNode -> getSize ());

		for (size_t i = 0, size = texCoordRampNode -> getSize (); i < size; ++ i)
			texCoordRamp [i] = texCoordRampNode -> get1Point (i);
	}

	texCoordRamp .resize (particleSystem -> texCoordKey () .size () * texCoordCount);

	texCoordAnim = particleSystem -> texCoordKey () .size () and texCoordRampNode;
}

bool
SoftSystem::intersects (const Box3d & box,
                                        const ClipPlaneContainerArray & clipPlanes,
                                        const Matrix4d & modelViewMatrix)
{
	// TODO: implement me.
	return false;
}

void
SoftSystem::animateParticles ()
{
	// Determine delta time

	static constexpr int32_t DELAY = 15; // Delay in frames when dt full applys.

	const time_type dt = 1 / std::max (10.0, particleSystem -> getBrowser () -> getCurrentFrameRate ());

	// var deltaTime is only for the emitter, this.deltaTime is for the forces.
	time_type deltaTime = this -> deltaTime = ((DELAY - 1) * this -> deltaTime + dt) / DELAY; // Moving average about DELAY frames.

	// Determine numParticles

	if (emitterNode -> isExplosive ())
	{
		const time_type now              = SFTime::now ();
		const time_type particleLifetime = this -> particleLifetime + this -> particleLifetime * this -> lifetimeVariation;

		if (numParticles == 0 or now - creationTime > particleLifetime)
		{
			creationTime    = now;
			numParticles    = maxParticles;
			createParticles = particleSystem -> createParticles ();

			deltaTime = std::numeric_limits <time_type>::infinity ();
		}
		else
			createParticles = false;
	}
	else
	{
		if (numParticles < maxParticles)
		{
			const time_type now          = SFTime::now ();
			const int32_t   newParticles = std::max <int32_t> (0, std::floor ((now - creationTime) * maxParticles / particleLifetime));

			if (newParticles)
				creationTime = now;

			numParticles = std::floor (std::min (maxParticles, numParticles + newParticles));
		}
	}

	// Apply forces.

	if (emitterNode -> mass ())
	{
		velocities  .clear ();
		speeds      .clear ();
		turbulences .clear ();

		for (const auto & physicsModelNode : physicsModelNodes)
			physicsModelNode -> addForce (emitterNode, velocities, turbulences);

		for (MFVec3f::reference v : velocities)
			v = v .get () * float (deltaTime / emitterNode -> mass ());

		for (const auto & v : basic::make_const_range (velocities))
			speeds .emplace_back (abs (v));

		numForces = velocities .size ();
	}
	else
	{
		numForces = 0;
	}

	// Determine particle position, velocity and colors

	emitterNode -> animate (this, deltaTime);

	updateGeometry (Matrix4d (), false);

	particleSystem -> getBrowser () -> addEvent ();
}

void
SoftSystem::updateGeometry (const Matrix4d & modelViewMatrix, const bool state)
{
	switch (geometryType)
	{
		case ParticleSystem::GeometryType::POINT:
			if (not state)
				updatePoint ();
			break;
		case ParticleSystem::GeometryType::LINE:
			if (not state)
				updateLine ();
			break;
		case ParticleSystem::GeometryType::TRIANGLE:
		case ParticleSystem::GeometryType::QUAD:
		case ParticleSystem::GeometryType::SPRITE:
			updateQuad (modelViewMatrix, state);
			break;
		case ParticleSystem::GeometryType::GEOMETRY:
			break;
	}
}

void
SoftSystem::updatePoint ()
{
	// Colors

	if (colorMaterial)
	{
		for (size_t i = 0; i < numParticles; ++ i)
			colorArray [i] = particles [i] .color;

		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Color4f) * colorArray .size (), colorArray .data (), GL_STATIC_DRAW);
	}

	// Vertices

	size_t i = 0;

	for (const auto & particle : particles)
	{
		positionArray [i]    = particle .position;
		elapsedTimeArray [i] = particle .elapsedTime / particle .lifetime;
		lifeArray [i]        = particle .life;
		vertexArray [i]      = particle .position;

		++ i;
	}

	glBindBuffer (GL_ARRAY_BUFFER, positionBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * positionArray .size (), positionArray .data (), GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, elapsedTimeBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (float) * elapsedTimeArray .size (), elapsedTimeArray .data (), GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, lifeBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (int32_t) * lifeArray .size (), lifeArray .data (), GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3d) * vertexArray .size (), vertexArray .data (), GL_STATIC_DRAW);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
SoftSystem::updateLine ()
{
	const auto sy1_2 = particleSystem -> particleSize () .getY () / 2;

	// Colors

	if (colorMaterial)
	{
		size_t i = 0;

		for (const auto & particle : particles)
		{
			const auto & color = particle .color;

			colorArray [i ++] = color;
			colorArray [i ++] = color;
		}

		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Color4f) * colorArray .size (), colorArray .data (), GL_STATIC_DRAW);
	}

	// Vertices

	size_t i = 0;

	for (const auto & particle : particles)
	{
		const auto & position    = particle .position;
		const auto   elapsedTime = particle .elapsedTime / particle .lifetime;
		const auto   life        = particle .life;
		const auto   size1_2     = normalize (particle .velocity) * sy1_2;

		positionArray [i + 0]    = position;
		positionArray [i + 1]    = position;
		elapsedTimeArray [i + 0] = elapsedTime;
		elapsedTimeArray [i + 1] = elapsedTime;
		lifeArray [i + 0]        = life;
		lifeArray [i + 1]        = life;

		vertexArray [i + 0] = position - size1_2;
		vertexArray [i + 1] = position + size1_2;

		i += 2;
	}

	glBindBuffer (GL_ARRAY_BUFFER, positionBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * positionArray .size (), positionArray .data (), GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, elapsedTimeBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (float) * elapsedTimeArray .size (), elapsedTimeArray .data (), GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, lifeBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (int32_t) * lifeArray .size (), lifeArray .data (), GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3d) * vertexArray .size (), vertexArray .data (), GL_STATIC_DRAW);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
SoftSystem::updateQuad (const Matrix4d & modelViewMatrix, const bool state)
{
	try
	{
		const auto sx1_2 = particleSystem -> particleSize () .getX () / 2;
		const auto sy1_2 = particleSystem -> particleSize () .getY () / 2;

		// Sort particles

//		if (sortParticles) // always false
//		{
//			for (var i = 0; i < numParticles; ++ i)
//			{
//				var particle = particles [i];
//				particle .distance = modelViewMatrix .getDepth (particle .position);
//			}
//
//			// Expensisive function!!!
//			this .particleSorter .sort (0, numParticles);
//		}

		// Colors

		if (not state) // if called from animateParticles
		{
			if (colorMaterial)
			{
				size_t i = 0;

				for (const auto & particle : particles)
				{
					const auto & color = particle .color;

					// p4 ------ p3
					// |       / |
					// |     /   |
					// |   /     |
					// | /       |
					// p1 ------ p2

					// p1, p2, p3; p1, p3, p4
					colorArray [i ++] = color;
					colorArray [i ++] = color;
					colorArray [i ++] = color;

					colorArray [i ++] = color;
					colorArray [i ++] = color;
					colorArray [i ++] = color;
				}

				glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (Color4f) * colorArray .size (), colorArray .data (), GL_STATIC_DRAW);
			}

			if (texCoordAnim and not texCoordArray .empty ())
			{
				const auto & texCoordKeys = particleSystem -> texCoordKey ();
				const size_t size         = texCoordKeys .size ();
				size_t       index0       = 0;
				size_t       i            = 0;

				for (const auto & particle : particles)
				{
					// Determine index0.

					const float fraction = particle .elapsedTime / particle .lifetime;

					if (size == 1 || fraction <= texCoordKeys [0])
					{
						index0 = 0;
					}
					else if (fraction >= texCoordKeys [size - 1])
					{
						index0 = size - 2;
					}
					else
					{
						const auto iter = std::upper_bound (texCoordKeys .begin (), texCoordKeys .end (), fraction);

						if (iter not_eq texCoordKeys .end ())
							index0 = (iter - texCoordKeys .begin ()) - 1;
						else
							index0 = 0;
					}

					// Set texCoord.

					index0 *= texCoordCount;

					const auto & texCoord1 = texCoordRamp [index0 + 0];
					const auto & texCoord2 = texCoordRamp [index0 + 1];
					const auto & texCoord3 = texCoordRamp [index0 + 2];
					const auto & texCoord4 = texCoordRamp [index0 + 3];

					// p4 ------ p3
					// |       / |
					// |     /   |
					// |   /     |
					// | /       |
					// p1 ------ p2


					texCoordArray [i ++] = texCoord1; // p1
					texCoordArray [i ++] = texCoord2; // p2
					texCoordArray [i ++] = texCoord3; // p3

					texCoordArray [i ++] = texCoord1; // p1
					texCoordArray [i ++] = texCoord3; // p3
					texCoordArray [i ++] = texCoord4; // p4
				}

				glBindBuffer (GL_ARRAY_BUFFER, texCoordBufferIds [0]);
				glBufferData (GL_ARRAY_BUFFER, sizeof (Vector4f) * texCoordArray .size (), texCoordArray .data (), GL_STATIC_DRAW);
			}
		}

		// Vertices

		if (geometryType == ParticleSystem::GeometryType::SPRITE)
		{
			if (state) // if called from depth or draw
			{
				// Normals

				const auto rotation = particleSystem -> getScreenAlignedRotation (modelViewMatrix);
				const auto normal   = normalize (cross (rotation .x (), rotation .y ()));

				std::fill (normalArray .begin (), normalArray .end (), normal);

				glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * normalArray .size (), normalArray .data (), GL_STATIC_DRAW);

				// Vertices

				const Vector3f s1 = Vector3d (-sx1_2, -sy1_2, 0) * rotation;
				const Vector3f s2 = Vector3d ( sx1_2, -sy1_2, 0) * rotation;
				const Vector3f s3 = Vector3d ( sx1_2,  sy1_2, 0) * rotation;
				const Vector3f s4 = Vector3d (-sx1_2,  sy1_2, 0) * rotation;

				size_t i = 0;

				for (const auto & particle : particles)
				{
					const auto & position    = particle .position;
					const auto   elapsedTime = particle .elapsedTime / particle .lifetime;
					const auto   life        = particle .life;

					const auto p1 = position + s1;
					const auto p2 = position + s2;
					const auto p3 = position + s3;
					const auto p4 = position + s4;

					// p4 ------ p3
					// |       / |
					// |     /   |
					// |   /     |
					// | /       |
					// p1 ------ p2

					for (size_t n = 0; n < 6; ++ n)
					{
						positionArray [i + n]    = position;
						elapsedTimeArray [i + n] = elapsedTime;
						lifeArray [i + n]        = life;
					}

					vertexArray [i + 0] = p1; // p1
					vertexArray [i + 1] = p2; // p2
					vertexArray [i + 2] = p3; // p3

					vertexArray [i + 3] = p1; // p1
					vertexArray [i + 4] = p3; // p3
					vertexArray [i + 5] = p4; // p4

					i += 6;
				}

				glBindBuffer (GL_ARRAY_BUFFER, positionBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * positionArray .size (), positionArray .data (), GL_STATIC_DRAW);
				glBindBuffer (GL_ARRAY_BUFFER, elapsedTimeBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (float) * elapsedTimeArray .size (), elapsedTimeArray .data (), GL_STATIC_DRAW);
				glBindBuffer (GL_ARRAY_BUFFER, lifeBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (int32_t) * lifeArray .size (), lifeArray .data (), GL_STATIC_DRAW);
				glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3d) * vertexArray .size (), vertexArray .data (), GL_STATIC_DRAW);
			}
		}
		else
		{
			if (not state) // if called from animateParticles
			{
				// Vertices

				const auto s1 = Vector3f (-sx1_2, -sy1_2, 0);
				const auto s2 = Vector3f ( sx1_2, -sy1_2, 0);
				const auto s3 = Vector3f ( sx1_2,  sy1_2, 0);
				const auto s4 = Vector3f (-sx1_2,  sy1_2, 0);

				size_t i = 0;

				for (const auto & particle : particles)
				{
					const auto & position    = particle .position;
					const auto   elapsedTime = particle .elapsedTime / particle .lifetime;
					const auto   life        = particle .life;

					const auto p1 = position + s1;
					const auto p2 = position + s2;
					const auto p3 = position + s3;
					const auto p4 = position + s4;

					// p4 ------ p3
					// |       / |
					// |     /   |
					// |   /     |
					// | /       |
					// p1 ------ p2

					for (size_t n = 0; n < 6; ++ n)
					{
						positionArray [i + n]    = position;
						elapsedTimeArray [i + n] = elapsedTime;
						lifeArray [i + n]        = life;
					}

					vertexArray [i + 0] = p1; // p1
					vertexArray [i + 1] = p2; // p2
					vertexArray [i + 2] = p3; // p3

					vertexArray [i + 3] = p1; // p1
					vertexArray [i + 4] = p3; // p3
					vertexArray [i + 5] = p4; // p4

					i += 6;;
				}

				glBindBuffer (GL_ARRAY_BUFFER, positionBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * positionArray .size (), positionArray .data (), GL_STATIC_DRAW);
				glBindBuffer (GL_ARRAY_BUFFER, elapsedTimeBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (float) * elapsedTimeArray .size (), elapsedTimeArray .data (), GL_STATIC_DRAW);
				glBindBuffer (GL_ARRAY_BUFFER, lifeBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (int32_t) * lifeArray .size (), lifeArray .data (), GL_STATIC_DRAW);
				glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
				glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3d) * vertexArray .size (), vertexArray .data (), GL_STATIC_DRAW);
			}
		}

		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
SoftSystem::depth (const X3DShapeContainer* const context)
{

}

void
SoftSystem::display (ShapeContainer* const context)
{
	try
	{
		if (numParticles <= 0)
			return;

		// Traverse appearance before everything.

		particleSystem -> getAppearance () -> enable (context, shaderGeometryType);

		// Update geometry if SPRITE.

		updateGeometry (context -> getModelViewMatrix (), true);

		// Display geometry.

		if (geometryType == ParticleSystem::GeometryType::GEOMETRY)
		{
			const auto & geometryNode = particleSystem -> getGeometry ();

			if (geometryNode)
				geometryNode -> drawParticles (context, particles, numParticles);
		}
		else
		{
			const auto browser    = context -> getBrowser ();
			auto       shaderNode = context -> getShader ();

			if (not shaderNode -> isValid ())
				return;

			if (shaderNode == browser -> getDefaultShader ())
				shaderNode = this -> shaderNode;

			// Setup shader.

			context -> setGeometryType  (shaderGeometryType);
			context -> setColorMaterial (colorMaterial);
			context -> setTextureCoordinate (browser -> getDefaultTexCoord ());

			shaderNode -> enable ();
			shaderNode -> setLocalUniforms (context);

			// Setup vertex attributes.

			// ***************************************************************************************************************************************
			// For idBufferId, positionBufferId, elapsedTimeBufferId, lifeBufferId, colorBufferId, normalBufferId glVertexAttribDivisor could be used.
			// ***************************************************************************************************************************************

			shaderNode -> enableIntegerAttrib ("x3d_ParticleId",          idBufferId,          1);
			shaderNode -> enableFloatAttrib   ("x3d_ParticlePosition",    positionBufferId,    3);
			shaderNode -> enableFloatAttrib   ("x3d_ParticleElapsedTime", elapsedTimeBufferId, 1);
			shaderNode -> enableIntegerAttrib ("x3d_ParticleLife",        lifeBufferId,        1);

			if (colorMaterial)
				shaderNode -> enableColorAttrib (colorBufferId, GL_FLOAT, 0, nullptr);

			if (not texCoordArray .empty ())
				shaderNode -> enableTexCoordAttrib (texCoordBufferIds, GL_FLOAT, { }, { });

			if (not normalArray .empty ())
				shaderNode -> enableNormalAttrib (normalBufferId, GL_FLOAT, 0, nullptr);

			shaderNode -> enableVertexAttrib (vertexBufferId, GL_DOUBLE, 0, nullptr);

//			bool testWireframe = false;
//
//			switch (this .geometryType)
//			{
//				case POINT:
//				case LINE:
//					break;
//				case TRIANGLE:
//				case QUAD:
//				case SPRITE:
//					testWireframe = true;
//					break;
//				case GEOMETRY:
//					break;
//			}

//			if (shaderNode .wireframe && testWireframe)
//			{
//				// Wireframes are always solid so only one drawing call is needed.
//
//				for (var i = 0, length = this .numParticles * this .vertexCount; i < length; i += 3)
//					gl .drawArrays (shaderNode .primitiveMode, i, 3);
//			}
//			else
			{
				const auto positiveScale = context -> getModelViewMatrix () .submatrix () .determinant () > 0;

				glFrontFace (positiveScale ? GL_CCW : GL_CW);
				glEnable (GL_CULL_FACE);
				glCullFace (GL_BACK);

				glDrawArrays (primitiveMode, 0, numParticles * vertexCount);
			}

			shaderNode -> disableIntegerAttrib ("x3d_ParticleId");
			shaderNode -> disableFloatAttrib   ("x3d_ParticlePosition");
			shaderNode -> disableFloatAttrib   ("x3d_ParticleElapsedTime");
			shaderNode -> disableIntegerAttrib ("x3d_ParticleLife");

			shaderNode -> disableColorAttrib ();
			shaderNode -> disableTexCoordAttrib ();
			shaderNode -> disableNormalAttrib ();
			shaderNode -> disableVertexAttrib ();
			shaderNode -> disable ();
		}

		particleSystem -> getAppearance () -> disable (context);
	}
	catch (const std::exception & error)
	{
		// Catch error from setLocalUniforms.
		__LOG__ << error .what () << std::endl;
	}
}

void
SoftSystem::dispose ()
{
	if (idBufferId)
		glDeleteBuffers (1, &idBufferId);

	if (positionBufferId)
		glDeleteBuffers (1, &positionBufferId);

	if (elapsedTimeBufferId)
		glDeleteBuffers (1, &elapsedTimeBufferId);

	if (lifeBufferId)
		glDeleteBuffers (1, &lifeBufferId);

	if (colorBufferId)
		glDeleteBuffers (1, &colorBufferId);

	if (texCoordBufferIds [0])
		glDeleteBuffers (1, texCoordBufferIds .data ());

	if (normalBufferId)
		glDeleteBuffers (1, &normalBufferId);

	if (vertexBufferId)
		glDeleteBuffers (1, &vertexBufferId);
}

SoftSystem::~SoftSystem ()
{ }

} // X3D
} // titania

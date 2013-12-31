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

#include "ParticleSystem.h"

#include "../../Bits/config.h"
#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Shaders/ShaderPart.h"
#include "../Shape/X3DAppearanceNode.h"
#include "X3DParticlePhysicsModelNode.h"

#include <cstddef>
#include <random>

#include "../../Debug.h"

namespace titania {
namespace X3D {

const std::string ParticleSystem::componentName  = "ParticleSystems";
const std::string ParticleSystem::typeName       = "ParticleSystem";
const std::string ParticleSystem::containerField = "children";

// Special indices for glVertexAttribPointer

static constexpr int VERTEX_INDEX   = 0;
static constexpr int NORMAL_INDEX   = 1;
static constexpr int TEXCOORD_INDEX = 2;
static constexpr int COLOR_INDEX    = 3;

// gcc switch -malign-double

struct ParticleSystem::Particle
{
	///  @name Construction

	Particle () :
		     seed (random1 () * PARTICLE_RAND_MAX),
		 lifetime (0),
		 position (),
		 velocity (),
		startTime (0)
	{ }

	///  @name Static members

	static constexpr int32_t PARTICLE_RAND_MAX = 0x7fffffff;

	///  @name Members

	GLint seed;
	float lifetime;
	Vector3f position;
	Vector3f velocity;
	double startTime;

};

struct ParticleSystem::Point
{
	Point () :
		position (),
		   color ()
	{ }

	Vector3f position;
	Color4f color;

};

ParticleSystem::Fields::Fields () :
	          enabled (new SFBool (true)),
	     geometryType (new SFString ("QUAD")),
	  createParticles (new SFBool (true)),
	     maxParticles (new SFInt32 (200)),
	 particleLifetime (new SFFloat (5)),
	lifetimeVariation (new SFFloat (0.25)),
	     particleSize (new SFVec2f (0.02, 0.02)),
	         colorKey (new MFFloat ()),
	      texCoordKey (new MFFloat ()),
	         isActive (new SFBool ()),
	          physics (new MFNode ()),
	          emitter (new SFNode ()),
	        colorRamp (new SFNode ()),
	     texCoordRamp (new SFNode ())
{ }

ParticleSystem::ParticleSystem (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       X3DShapeNode (),
	             fields (),
	     geometryTypeId (GeometryType::QUAD),
	         readBuffer (0),
	        writeBuffer (1),
	transformFeedbackId ({ 0, 0 }),
	   particleBufferId ({ 0, 0 }),
	    pointFeedbackId (0),
	      pointBufferId (0),
	    transformShader (),
	        pointShader (),
	        emitterNode (nullptr),
	          particles (0),
	       creationTime (0)
{
	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "enabled",           enabled ());

	addField (initializeOnly, "geometryType",      geometryType ());
	addField (inputOutput,    "createParticles",   createParticles ());
	addField (inputOutput,    "maxParticles",      maxParticles ());

	addField (inputOutput,    "particleLifetime",  particleLifetime ());
	addField (inputOutput,    "lifetimeVariation", lifetimeVariation ());
	addField (inputOutput,    "particleSize",      particleSize ());

	addField (initializeOnly, "colorKey",          colorKey ());
	addField (initializeOnly, "texCoordKey",       texCoordKey ());
	addField (outputOnly,     "isActive",          isActive ());

	addField (initializeOnly, "bboxSize",          bboxSize ());
	addField (initializeOnly, "bboxCenter",        bboxCenter ());
	addField (initializeOnly, "physics",           physics ());
	addField (initializeOnly, "emitter",           emitter ());
	addField (initializeOnly, "colorRamp",         colorRamp ());
	addField (initializeOnly, "texCoordRamp",      texCoordRamp ());
	addField (inputOutput,    "appearance",        appearance ());
	addField (inputOutput,    "geometry",          geometry ());

	addChildren (transformShader, pointShader);
}

X3DBaseNode*
ParticleSystem::create (X3DExecutionContext* const executionContext) const
{
	return new ParticleSystem (executionContext);
}

void
ParticleSystem::initialize ()
{
	X3DShapeNode::initialize ();

	if (glXGetCurrentContext ())
	{
		// Generate transform buffers

		glGenTransformFeedbacks (2, transformFeedbackId .data ());
		glGenBuffers (2, particleBufferId .data ());

		for (size_t i = 0; i < 2; ++ i)
		{
			glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, transformFeedbackId [i]);
			glBindBufferBase (GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBufferId [i]);
		}

		// Generate point buffer

		glGenTransformFeedbacks (1, &pointFeedbackId);
		glGenBuffers (1, &pointBufferId);

		glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, pointFeedbackId);
		glBindBufferBase (GL_TRANSFORM_FEEDBACK_BUFFER, 0, pointBufferId);

		glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);

		// Setup

		enabled ()           .addInterest (this, &ParticleSystem::set_enabled);
		geometryType ()      .addInterest (this, &ParticleSystem::set_geometryType);
		maxParticles ()      .addInterest (this, &ParticleSystem::set_array_buffers);
		particleLifetime ()  .addInterest (this, &ParticleSystem::set_array_buffers);
		lifetimeVariation () .addInterest (this, &ParticleSystem::set_array_buffers);
		emitter ()           .addInterest (this, &ParticleSystem::set_emitter);
		emitter ()           .addInterest (this, &ParticleSystem::set_transform_shader);
		geometry ()          .addInterest (this, &ParticleSystem::set_geometry);

		set_enabled ();
		set_geometryType ();
		set_emitter ();
		set_transform_shader ();
		set_geometry ();
	}
}

void
ParticleSystem::set_enabled ()
{
	if (enabled ())
	{
		getBrowser () -> prepareEvents () .addInterest (this, &ParticleSystem::prepareEvents);
		getBrowser () -> sensors ()       .addInterest (this, &ParticleSystem::update);

		set_array_buffers ();

		isActive () = true;
	}
	else
	{
		getBrowser () -> prepareEvents () .removeInterest (this, &ParticleSystem::prepareEvents);
		getBrowser () -> sensors ()       .removeInterest (this, &ParticleSystem::update);

		particles = 0;

		isActive () = false;
	}
}

void
ParticleSystem::set_geometryType ()
{
	static const std::map <std::string, GeometryType> geometryTypes = {
		std::make_pair ("POINT",    GeometryType::POINT),
		std::make_pair ("LINE",     GeometryType::LINE),
		std::make_pair ("TRIANGLE", GeometryType::TRIANGLE),
		std::make_pair ("QUAD",     GeometryType::QUAD),
		std::make_pair ("GEOMETRY", GeometryType::GEOMETRY),
		std::make_pair ("SPRITE",   GeometryType::SPRITE)
	};

	auto iter = geometryTypes .find (geometryType ());

	if (iter not_eq geometryTypes .end ())
		geometryTypeId = iter -> second;

	else
		geometryTypeId = GeometryType::QUAD;

	// Shader

	X3DSFNode <ShaderPart> vertexShader = new ShaderPart (getExecutionContext ());
	vertexShader -> url () = { get_shader ("ParticleSystems/PointShader.vs") .str () };
	vertexShader -> setup ();

	pointShader = new ComposedShader (getExecutionContext ());
	pointShader -> addUserDefinedField (inputOutput, "time", new SFTime ());

	pointShader -> language () = "GLSL";
	pointShader -> parts () .emplace_back (vertexShader);
	pointShader -> setTransformFeedbackVaryings ({
		"To.position", "To.color"
	});
	pointShader -> setup ();
}

void
ParticleSystem::set_emitter ()
{
	// Emitter node

	emitterNode = x3d_cast <X3DParticleEmitterNode*> (emitter ());

	if (emitterNode)
		return;

	emitterNode = getBrowser () -> getBrowserOptions () -> emitter ();
}

void
ParticleSystem::set_transform_shader ()
{
	// Shader

	X3DSFNode <ShaderPart> vertexShader = new ShaderPart (getExecutionContext ());
	vertexShader -> url () = emitterNode -> getShaderUrl ();
	vertexShader -> setup ();

	transformShader = new ComposedShader (getExecutionContext ());
	transformShader -> addUserDefinedField (inputOutput, "time",              new SFTime ());
	transformShader -> addUserDefinedField (inputOutput, "deltaTime",         new SFFloat ());
	transformShader -> addUserDefinedField (inputOutput, "particleLifetime",  new SFFloat ());
	transformShader -> addUserDefinedField (inputOutput, "lifetimeVariation", new SFFloat ());
	transformShader -> addUserDefinedField (inputOutput, "position",          new SFVec3f ());
	transformShader -> addUserDefinedField (inputOutput, "direction",         new SFVec3f ());
	transformShader -> addUserDefinedField (inputOutput, "speed",             new SFFloat ());
	transformShader -> addUserDefinedField (inputOutput, "variation",         new SFFloat ());
	transformShader -> addUserDefinedField (inputOutput, "velocity",          new MFVec3f ());
	transformShader -> addUserDefinedField (inputOutput, "turbulence",        new MFFloat ());
	transformShader -> addUserDefinedField (inputOutput, "forces",            new SFInt32 ());

	transformShader -> language () = "GLSL";
	transformShader -> parts () .emplace_back (vertexShader);
	transformShader -> setTransformFeedbackVaryings ({
		"To.seed", "To.lifetime", "To.position", "To.velocity", /*"gl_SkipComponents1",*/ "To.startTime"
	});
	transformShader -> setup ();
}

void
ParticleSystem::set_geometry ()
{ }

void
ParticleSystem::set_array_buffers ()
{
	// Initialize array buffers

	size_t numParticles = std::max (0, maxParticles () .getValue ());

	// Particle buffers

	Particle particleArray [numParticles];

	for (size_t i = 0; i < 2; ++ i)
	{
		glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [i]);
		glBufferData (GL_ARRAY_BUFFER, sizeof (particleArray), particleArray, GL_DYNAMIC_DRAW);
	}

	// Point buffer

	Point pointArray [numParticles];

	glBindBuffer (GL_ARRAY_BUFFER, pointBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (pointArray), pointArray, GL_DYNAMIC_DRAW);

	glBindBuffer (GL_ARRAY_BUFFER, 0);

	// Reset state

	particles    = 0;
	creationTime = chrono::now ();
}

bool
ParticleSystem::isTransparent () const
{
	if (getAppearance () -> isTransparent ())
		return true;

	return false;
}

bool
ParticleSystem::isLineGeometry () const
{
	switch (geometryTypeId)
	{
		case GeometryType::POINT:
		case GeometryType::LINE:
			return true;
		case GeometryType::TRIANGLE:
		case GeometryType::QUAD:
		case GeometryType::SPRITE:
			return false;
		case GeometryType::GEOMETRY:
		{
			if (getGeometry ())
				return getGeometry () -> isLineGeometry ();

			return false;
		}
	}

	return false;
}

Box3f
ParticleSystem::getBBox ()
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		return Box3f ();
	}

	return Box3f (bboxSize (), bboxCenter ());
}

bool
ParticleSystem::intersect (const Sphere3f & sphere, const Matrix4f & matrix, const CollectableObjectArray & localObjects)
{
	return false;
}

void
ParticleSystem::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::PICKING:
		{
			break;
		}
		case TraverseType::NAVIGATION:
		case TraverseType::COLLISION:
		{
			break;
		}
		case TraverseType::COLLECT:
		{
			getBrowser () -> getRenderers () .top () -> addShape (this);
			break;
		}
		default:
			break;
	}
}

void
ParticleSystem::prepareEvents ()
{
	float deltaTime = 1 / getBrowser () -> getCurrentFrameRate ();

	transformShader -> setField <SFTime>  ("time",              getBrowser () -> getCurrentTime ());
	transformShader -> setField <SFFloat> ("deltaTime",         deltaTime);
	transformShader -> setField <SFFloat> ("particleLifetime",  particleLifetime (),  true);
	transformShader -> setField <SFFloat> ("lifetimeVariation", lifetimeVariation (), true);

	emitterNode -> setShaderFields (transformShader);

	MFVec3f velocity;
	MFFloat turbulence;

	for (const auto & node : physics ())
	{
		auto physics = x3d_cast <X3DParticlePhysicsModelNode*> (node);

		if (physics)
			physics -> getForce (emitterNode, velocity, turbulence);
	}

	if (emitterNode -> mass ())
	{
		for (auto & v : velocity)
			v = v * deltaTime / emitterNode -> mass () .getValue ();

		transformShader -> setField <MFVec3f> ("velocity",   velocity,                    true);
		transformShader -> setField <MFFloat> ("turbulence", turbulence,                  true);
		transformShader -> setField <SFInt32> ("forces",     int32_t (velocity .size ()), true);
	}
	else
		transformShader -> setField <SFInt32> ("forces", 0, true);

	//pointShader -> setField <SFTime> ("time", getBrowser () -> getCurrentTime ());
}

void
ParticleSystem::update ()
{
	// Create new particles if possible.

	int32_t createParticles = (getCurrentTime () - creationTime) * maxParticles () / particleLifetime ();

	if (createParticles)
		creationTime = getCurrentTime ();

	particles = std::min (std::max (0, maxParticles () .getValue ()), createParticles + particles);

	// Transform particles.

	transformShader -> enable ();

	glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);

	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	glEnableVertexAttribArray (2);
	glEnableVertexAttribArray (3);
	glEnableVertexAttribArray (4);

	glVertexAttribIPointer (0, 1, GL_INT,    sizeof (Particle), (const GLvoid*) offsetof (Particle, seed));
	glVertexAttribPointer  (1, 1, GL_FLOAT,  GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, lifetime));
	glVertexAttribPointer  (2, 3, GL_FLOAT,  GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, position));
	glVertexAttribPointer  (3, 3, GL_FLOAT,  GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, velocity));
	glVertexAttribLPointer (4, 1, GL_DOUBLE, sizeof (Particle), (const GLvoid*) offsetof (Particle, startTime));

	glEnable (GL_RASTERIZER_DISCARD);
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, transformFeedbackId [writeBuffer]);
	glBeginTransformFeedback (GL_POINTS);

	glDrawArrays (GL_POINTS, 0, particles);

	glEndTransformFeedback ();
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);
	glDisable (GL_RASTERIZER_DISCARD);

	glDisableVertexAttribArray (0);
	glDisableVertexAttribArray (1);
	glDisableVertexAttribArray (2);
	glDisableVertexAttribArray (3);
	glDisableVertexAttribArray (4);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	transformShader -> disable ();

	std::swap (readBuffer, writeBuffer);

	// Point shader
	
	pointShader -> enable ();

	glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);

	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	glEnableVertexAttribArray (2);

	glVertexAttribPointer  (0, 1, GL_FLOAT, GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, lifetime));
	glVertexAttribPointer  (1, 3, GL_FLOAT, GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, position));
	glVertexAttribLPointer (2, 1, GL_DOUBLE, sizeof (Particle), (const GLvoid*) offsetof (Particle, startTime));

	glEnable (GL_RASTERIZER_DISCARD);
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, pointFeedbackId);
	glBeginTransformFeedback (GL_POINTS);

	glDrawArrays (GL_POINTS, 0, particles);

	glEndTransformFeedback ();
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);
	glDisable (GL_RASTERIZER_DISCARD);

	glDisableVertexAttribArray (0);
	glDisableVertexAttribArray (1);
	glDisableVertexAttribArray (2);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	pointShader -> disable ();

GL_ERROR;
}

void
ParticleSystem::drawCollision ()
{ }

void
ParticleSystem::drawGeometry ()
{
	glBindBuffer (GL_ARRAY_BUFFER, pointBufferId);

	glEnableVertexAttribArray (VERTEX_INDEX);
	glVertexAttribPointer (VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof (Point), (const GLvoid*) offsetof (Point, position));

	glEnableVertexAttribArray (COLOR_INDEX);
	glVertexAttribPointer (COLOR_INDEX, 4, GL_FLOAT, GL_FALSE, sizeof (Point), (const GLvoid*) offsetof (Point, color));

	glDrawArrays (GL_POINTS, 0, particles);

	glDisableVertexAttribArray (VERTEX_INDEX);
	glDisableVertexAttribArray (COLOR_INDEX);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

float
ParticleSystem::random1 ()
{
	static std::uniform_real_distribution <float> distribution (-1, 1);
	static std::default_random_engine             engine;
	return distribution (engine);
}

void
ParticleSystem::dispose ()
{
	__LOG__ << std::endl;

	transformShader .dispose ();
	pointShader     .dispose ();

	if (transformFeedbackId [0])
		glDeleteTransformFeedbacks (2, transformFeedbackId .data ());

	if (particleBufferId [0])
		glDeleteBuffers (2, particleBufferId .data ());

	if (pointFeedbackId)
		glDeleteTransformFeedbacks (1, &pointFeedbackId);

	if (pointBufferId)
		glDeleteBuffers (1, &pointBufferId);

	X3DShapeNode::dispose ();
}

} // X3D
} // titania

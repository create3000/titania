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

#include "ParticleSystem.h"

#include "../../Browser/ParticleSystems/SoftSystem.h"

#include "../../Browser/Core/BrowserOptions.h"
#include "../../Bits/Cast.h"
#include "../../Browser/Networking/config.h"
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Browser/ParticleSystems/BVH.h"
#include "../../Browser/ParticleSystems/Random.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../ParticleSystems/BoundedPhysicsModel.h"
#include "../ParticleSystems/X3DParticleEmitterNode.h"
#include "../ParticleSystems/X3DParticlePhysicsModelNode.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Shaders/ShaderPart.h"
#include "../Shape/X3DAppearanceNode.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

#include <cstddef>

namespace titania {
namespace X3D {

// Constants

static constexpr size_t COLOR_RAMP_KEYS   = 0;
static constexpr size_t COLOR_RAMP_VALUES = 1;

static constexpr size_t TEXCOORD_RAMP_KEYS   = 0;
static constexpr size_t TEXCOORD_RAMP_VALUES = 1;

static constexpr Vector3f yAxis (0, 1, 0);
static constexpr Vector3f zAxis (0, 0, 1);

// Particle

struct ParticleSystem::Particle
{
	///  @name Construction

	Particle () :
		       seed (randomi ()),
		   lifetime (-1),
		   position (),
		   velocity (),
		      color (),
		elapsedTime (0),
		   distance (0)
	{ }
	
	///  @name Members

	int32_t seed;
	float lifetime;
	Vector3f position;
	Vector3f velocity;
	Color4f color;
	float elapsedTime;
	float distance;

};

// Vertex

struct ParticleSystem::Vertex
{
	///  @name Construction

	Vertex () :
		position (),
		   color (),
		texCoord ()
	{ }

	///  @name Members

	const Vector3f position;
	const Color4f color;
	const Vector4f texCoord;

};

// OddEvenMergeSort

// http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter46.html
// http://http.download.nvidia.com/developer/GPU_Gems_2/CD/Index.html

class ParticleSystem::OddEvenMergeSort
{
public:

	OddEvenMergeSort ();

	void
	setup (X3DProgrammableShaderObject* const);

	void
	sortStep (X3DProgrammableShaderObject* const, const int32_t);

	~OddEvenMergeSort ()
	{ }


private:

	void
	sortStep (X3DProgrammableShaderObject* const);

	void
	reset (X3DProgrammableShaderObject* const, const int32_t);

	int32_t stepsLeft;
	int32_t pass;
	int32_t stage;
	int32_t size;

};

ParticleSystem::OddEvenMergeSort::OddEvenMergeSort () :
	stepsLeft (0),
	     pass (0),
	    stage (0),
	     size (0)
{ }

void
ParticleSystem::OddEvenMergeSort::setup (X3DProgrammableShaderObject* const shader)
{
	shader -> addUserDefinedField (inputOutput, "sortParam", new SFVec4f ());
	shader -> addUserDefinedField (inputOutput, "sortSize",  new SFInt32 ());
}

void
ParticleSystem::OddEvenMergeSort::sortStep (X3DProgrammableShaderObject* const shader, const int32_t currentSize)
{
	try
	{
		if (stepsLeft <= 0)
			reset (shader, currentSize);

		if (currentSize < size)
			shader -> setField <SFInt32> ("sortSize", currentSize, true);

		sortStep (shader);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ParticleSystem::OddEvenMergeSort::sortStep (X3DProgrammableShaderObject* const shader)
{
	// Sort pass

	-- pass;

	if (pass < 0)
	{
		// Next stage
		pass = ++ stage;
	}

	// Sort step

	const int32_t pstage = 1 << stage;
	const int32_t ppass  = 1 << pass;

	shader -> setField <SFVec4f> ("sortParam", Vector4f (pstage + pstage,
	                                                     ppass % pstage,
	                                                     (pstage + pstage) - (ppass % pstage) - 1,
	                                                     ppass));

	//

	-- stepsLeft;
}

void
ParticleSystem::OddEvenMergeSort::reset (X3DProgrammableShaderObject* const shader, const int32_t currentSize)
{
	const int32_t fieldSize = next_power_of_two (int32_t (std::ceil (std::sqrt (currentSize))));

	if (fieldSize)
	{
		const int32_t logFieldSize = std::log2 (fieldSize);

		stepsLeft = logFieldSize * (2 * logFieldSize + 1);
	}

	stage = pass = -1;

	// Shader

	shader -> setField <SFInt32> ("sortSize", currentSize, true);

	size = currentSize;
}

// ParticleSystem

const ComponentType ParticleSystem::component      = ComponentType::PARTICLE_SYSTEMS;
const std::string   ParticleSystem::typeName       = "ParticleSystem";
const std::string   ParticleSystem::containerField = "children";

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
	           emitter (new SFNode ()),
	         colorRamp (new SFNode ()),
	      texCoordRamp (new SFNode ()),
	           physics (new MFNode ())
{ }

ParticleSystem::ParticleSystem (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	            X3DShapeNode (),
	                  fields (),
	          geometryTypeId (GeometryType::QUAD),
	      shaderGeometryType (X3D::GeometryType::GEOMETRY_3D),
	          glGeometryType (GL_POINTS),
	             numVertices (0),
	            numParticles (0),
	            creationTime (0),
	               pauseTime (0),
	              readBuffer (0),
	             writeBuffer (1),
	      particleFeedbackId ({ 0, 0 }),
	        particleBufferId ({ 0, 0 }),
	           particleMapId (0),
	        vertexFeedbackId (0),
	          vertexBufferId (0),
	        geometryBufferId (0),
	          colorRampMapId ({ 0, 0 }),
	       colorRampBufferId ({ 0, 0 }),
	       texCoordRampMapId ({ 0, 0 }),
	    texCoordRampBufferId ({ 0, 0 }),
	         transformShader (),
	          geometryShader (),
	             emitterNode (),
	           colorRampNode (),
	        texCoordRampNode (),
	            geometryNode (),
	              shaderNode (),
	               numColors (0),
	             numTexCoord (0),
	       physicsModelNodes (),
	boundedPhysicsModelNodes (),
	      boundedNormalMapId (0),
	   boundedNormalBufferId (0),
	     boundedSurfaceMapId (0),
	  boundedSurfaceBufferId (0),
	      boundedVolumeMapId (0),
	   boundedVolumeBufferId (0),
	           sortAlgorithm (new OddEvenMergeSort ()),
	              softSystem (new SoftSystem (this))
{
	addType (X3DConstants::ParticleSystem);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "enabled",           enabled ());

	addField (initializeOnly, "geometryType",      geometryType ());
	addField (inputOutput,    "maxParticles",      maxParticles ());
	addField (inputOutput,    "createParticles",   createParticles ());

	addField (inputOutput,    "particleLifetime",   particleLifetime ());
	addField (inputOutput,    "lifetimeVariation",  lifetimeVariation ());
	addField (inputOutput,    "particleSize",       particleSize ());

	addField (initializeOnly, "emitter",           emitter ());
	addField (initializeOnly, "physics",           physics ());
	addField (initializeOnly, "colorKey",          colorKey ());
	addField (initializeOnly, "colorRamp",         colorRamp ());
	addField (initializeOnly, "texCoordKey",       texCoordKey ());
	addField (initializeOnly, "texCoordRamp",      texCoordRamp ());
	addField (outputOnly,     "isActive",          isActive ());

	addField (initializeOnly, "bboxSize",          bboxSize ());
	addField (initializeOnly, "bboxCenter",        bboxCenter ());
	addField (inputOutput,    "appearance",        appearance ());
	addField (inputOutput,    "geometry",          geometry ());

	particleSize () .setUnit (UnitCategory::LENGTH);

	addChildObjects (transformShader,
	                 geometryShader,
	                 emitterNode,
	                 colorRampNode,
	                 texCoordRampNode,
	                 geometryNode,
	                 shaderNode,
	                 physicsModelNodes,
	                 boundedPhysicsModelNodes);
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

	if (isSoftSystem ())
	{
		softSystem -> initialize ();

		geometryType () .addInterest (&ParticleSystem::set_geometryType, this);

		set_geometryType ();
		return;
	}

	// Generate transform buffers

	glGenTransformFeedbacks (2, particleFeedbackId .data ());
	glGenBuffers (2, particleBufferId .data ());

	for (size_t i = 0; i < 2; ++ i)
	{
		glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, particleFeedbackId [i]);
		glBindBufferBase (GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBufferId [i]);
	}

	glGenTextures (1, &particleMapId);

	set_particle_map ();

	// Generate point buffer

	glGenTransformFeedbacks (1, &vertexFeedbackId);
	glGenBuffers (1, &vertexBufferId);

	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, vertexFeedbackId);
	glBindBufferBase (GL_TRANSFORM_FEEDBACK_BUFFER, 0, vertexBufferId);

	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);

	// Generate geometry buffer

	glGenBuffers (1, &geometryBufferId);

	// Color ramp

	glGenTextures (2, colorRampMapId .data ());
	glGenBuffers  (2, colorRampBufferId .data ());

	// Texture coordinate ramp

	glGenTextures (2, texCoordRampMapId .data ());
	glGenBuffers  (2, texCoordRampBufferId .data ());

	// Bounded physics model

	glGenTextures (1, &boundedNormalMapId);
	glGenBuffers  (1, &boundedNormalBufferId);

	glGenTextures (1, &boundedSurfaceMapId);
	glGenBuffers  (1, &boundedSurfaceBufferId);

	glGenTextures (1, &boundedVolumeMapId);
	glGenBuffers  (1, &boundedVolumeBufferId);

	// Setup
	
	getExecutionContext () -> isLive () .addInterest (&ParticleSystem::set_live, this);
	isLive () .addInterest (&ParticleSystem::set_live, this);

	getBrowser () -> getDefaultShader ()         .addInterest (&ParticleSystem::set_shader, this);
	getBrowser () -> getFixedPipelineRequired () .addInterest (&ParticleSystem::set_shader, this);

	enabled ()           .addInterest (&ParticleSystem::set_enabled, this);
	geometryType ()      .addInterest (&ParticleSystem::set_geometryType, this);
	createParticles ()   .addInterest (&ParticleSystem::set_createParticles, this);
	maxParticles ()      .addInterest (&ParticleSystem::set_enabled, this);
	lifetimeVariation () .addInterest (&ParticleSystem::set_particle_buffers, this);
	particleSize ()      .addInterest (&ParticleSystem::set_geometryType, this);
	colorKey ()          .addInterest (&ParticleSystem::set_colorKey, this);
	texCoordKey ()       .addInterest (&ParticleSystem::set_texCoordKey, this);
	emitter ()           .addInterest (&ParticleSystem::set_emitter, this);
	colorRamp ()         .addInterest (&ParticleSystem::set_colorRamp, this);
	texCoordRamp ()      .addInterest (&ParticleSystem::set_texCoordRamp, this);
	physics ()           .addInterest (&ParticleSystem::set_physics, this);
	geometry ()          .addInterest (&ParticleSystem::set_geometry, this);

	boundedPhysicsModelNodes .addInterest (&ParticleSystem::set_boundedPhysicsModel, this);

	set_emitter ();
	set_geometry_shader ();
	set_enabled ();
	set_geometryType ();
	set_physics ();
	set_colorRamp ();
	set_texCoordRamp ();
	set_geometry ();
}

bool
ParticleSystem::isSoftSystem () const
{
	// Also in X3DParticleEmitterNode
	return true;

	if (not getBrowser () -> getExtension ("GL_ARB_texture_buffer_object"))
		return true;

	if (not getBrowser () -> getExtension ("GL_ARB_transform_feedback3"))
		return true;

	return false;
}

void
ParticleSystem::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isSoftSystem ())
	{
		const auto e = getExecutionContext ();
		X3DShapeNode::setExecutionContext (executionContext);
		softSystem -> setExecutionContext (e);
		return;
	}

	if (isInitialized ())
	{
		getBrowser () -> getBrowserOptions () -> Shading () .removeInterest (&ParticleSystem::set_shader, this);
		getBrowser () -> getFixedPipelineRequired ()        .removeInterest (&ParticleSystem::set_shader, this);

		getBrowser () -> sensorEvents ()    .removeInterest (&ParticleSystem::animateParticles, this);
		getBrowser () -> sensorEvents ()    .removeInterest (&ParticleSystem::updateParticles, this);
		getExecutionContext () -> isLive () .removeInterest (&ParticleSystem::set_live, this);
	}

	X3DShapeNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getBrowser () -> getBrowserOptions () -> Shading () .addInterest (&ParticleSystem::set_shader, this);
		getBrowser () -> getFixedPipelineRequired ()        .addInterest (&ParticleSystem::set_shader, this);

		getExecutionContext () -> isLive () .addInterest (&ParticleSystem::set_live, this);

		set_live ();
		set_emitter ();
		set_shader ();
	}
}

Box3d
ParticleSystem::getBBox () const
{
	if (isSoftSystem ())
		return softSystem -> getBBox ();

	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		if (emitterNode)
			return emitterNode -> getBBox ();

		return Box3d ();
	}

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

bool
ParticleSystem::isTransparent () const
{
	if (isSoftSystem ())
		return softSystem -> isTransparent ();

	if (getAppearance () -> isTransparent ())
		return true;

	if (numColors and colorRampNode -> isTransparent ())
		return true;

	if (geometryTypeId == GeometryType::POINT)
		return true;

	if (geometryTypeId == GeometryType::GEOMETRY and getGeometry ())
		return getGeometry () -> isTransparent ();

	return false;
}

GeometryType
ParticleSystem::getGeometryType () const
{
	if (geometryTypeId == GeometryType::GEOMETRY)
	{
		if (getGeometry ())
			return getGeometry () -> getGeometryType ();

		return shaderGeometryType;
	}

	return shaderGeometryType;
}

void
ParticleSystem::set_live ()
{
	if (getExecutionContext () -> isLive () and isLive ())
	{
		if (isActive () and maxParticles ())
		{
			getBrowser () -> sensorEvents () .addInterest (&ParticleSystem::animateParticles, this);
			getBrowser () -> sensorEvents () .addInterest (&ParticleSystem::updateParticles, this);

			if (pauseTime)
			{
				creationTime += SFTime::now () - pauseTime;
				pauseTime = 0;
			}
		}
	}
	else
	{
		if (isActive () and maxParticles ())
		{
			getBrowser () -> sensorEvents () .removeInterest (&ParticleSystem::animateParticles, this);
			getBrowser () -> sensorEvents () .removeInterest (&ParticleSystem::updateParticles, this);

			if (pauseTime == 0)
				pauseTime = SFTime::now ();
		}
	}
}

void
ParticleSystem::set_enabled ()
{
	if (enabled () and maxParticles ())
	{
		if (not isActive ())
		{
			if (isLive () and getExecutionContext () -> isLive ())
			{
				getBrowser () -> sensorEvents () .addInterest (&ParticleSystem::animateParticles, this);
				getBrowser () -> sensorEvents () .addInterest (&ParticleSystem::updateParticles, this);
				pauseTime = 0;
			}
			else
				pauseTime = SFTime::now ();

			isActive () = true;
		}
	}
	else
	{
		if (isActive ())
		{
			if (isLive () and getExecutionContext () -> isLive ())
			{
				getBrowser () -> sensorEvents () .removeInterest (&ParticleSystem::animateParticles, this);
				getBrowser () -> sensorEvents () .removeInterest (&ParticleSystem::updateParticles, this);
			}

			isActive () = false;
		}
	}

	set_particle_buffers ();
}

void
ParticleSystem::set_geometryType ()
{
	static const std::map <std::string, GeometryType> geometryTypes = {
		std::pair ("POINT",    GeometryType::POINT),
		std::pair ("LINE",     GeometryType::LINE),
		std::pair ("TRIANGLE", GeometryType::TRIANGLE),
		std::pair ("QUAD",     GeometryType::QUAD),
		std::pair ("GEOMETRY", GeometryType::GEOMETRY),
		std::pair ("SPRITE",   GeometryType::SPRITE)
	};

	try
	{
		geometryTypeId = geometryTypes .at (geometryType ());
	}
	catch (const std::out_of_range &)
	{
		geometryTypeId = GeometryType::QUAD;
	}

	if (not isSoftSystem ())
		set_vertices ();
}

void
ParticleSystem::set_vertices ()
{
	try
	{
		//

		MFVec4f                texCoord;
		std::vector <Vector3f> vertices;

		Vector2f particleSize1_2 = particleSize () / 2.0f;

		switch (geometryTypeId)
		{
			case GeometryType::POINT:
			{
				glGeometryType = GL_POINTS;
				numVertices    = 0;
				break;
			}
			case GeometryType::LINE:
			{
				glGeometryType = GL_LINES;
				numVertices    = 2;

				texCoord .emplace_back (0, 0, 0, 1);
				texCoord .emplace_back (0, 1, 0, 1);

				vertices .reserve (numVertices);
				vertices .emplace_back (0, 0, -particleSize1_2 .y ());
				vertices .emplace_back (0, 0,  particleSize1_2 .y ());
				break;
			}
			case GeometryType::TRIANGLE:
			{
				glGeometryType = GL_TRIANGLES;
				numVertices    = 6;

				texCoord .emplace_back (0, 0, 0, 1);
				texCoord .emplace_back (1, 0, 0, 1);
				texCoord .emplace_back (1, 1, 0, 1);

				texCoord .emplace_back (0, 0, 0, 1);
				texCoord .emplace_back (1, 1, 0, 1);
				texCoord .emplace_back (0, 1, 0, 1);

				vertices .reserve (numVertices);
				vertices .emplace_back (-particleSize1_2 .x (), -particleSize1_2 .y (), 0);
				vertices .emplace_back (+particleSize1_2 .x (), -particleSize1_2 .y (), 0);
				vertices .emplace_back (+particleSize1_2 .x (),  particleSize1_2 .y (), 0);

				vertices .emplace_back (-particleSize1_2 .x (), -particleSize1_2 .y (), 0);
				vertices .emplace_back (+particleSize1_2 .x (),  particleSize1_2 .y (), 0);
				vertices .emplace_back (-particleSize1_2 .x (),  particleSize1_2 .y (), 0);
				break;
			}
			case GeometryType::QUAD:
			case GeometryType::SPRITE:
			{
				glGeometryType = GL_QUADS;
				numVertices    = 4;

				texCoord .emplace_back (0, 0, 0, 1);
				texCoord .emplace_back (1, 0, 0, 1);
				texCoord .emplace_back (1, 1, 0, 1);
				texCoord .emplace_back (0, 1, 0, 1);

				vertices .reserve (numVertices);
				vertices .emplace_back (-particleSize1_2 .x (), -particleSize1_2 .y (), 0);
				vertices .emplace_back (+particleSize1_2 .x (), -particleSize1_2 .y (), 0);
				vertices .emplace_back (+particleSize1_2 .x (),  particleSize1_2 .y (), 0);
				vertices .emplace_back (-particleSize1_2 .x (),  particleSize1_2 .y (), 0);
				break;
			}
			case GeometryType::GEOMETRY:
			{
				glGeometryType = GL_TRIANGLES;
				numVertices    = 0;

				vertices .resize (numVertices);
				break;
			}
		}

		if (numVertices)
		{
			glBindBuffer (GL_ARRAY_BUFFER, geometryBufferId);
			glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * numVertices, vertices .data (), GL_STATIC_COPY);

			glBindBuffer (GL_ARRAY_BUFFER, 0);

			// Vertex buffer

			set_vertex_buffer ();
		}

		set_texCoord ();
		set_shader ();

		geometryShader -> setField <SFInt32> ("numVertices",  int32_t (numVertices));
		geometryShader -> setField <SFInt32> ("geometryType", int32_t (geometryTypeId));
		geometryShader -> setField <MFVec4f> ("texCoord",     texCoord);
	
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ParticleSystem::set_shader ()
{
	switch (geometryTypeId)
	{
		case GeometryType::POINT:
		{
			shaderGeometryType = X3D::GeometryType::GEOMETRY_POINTS;
			shaderNode         = getBrowser () -> getPointShader ();
			break;
		}
		case GeometryType::LINE:
		{
			shaderGeometryType = X3D::GeometryType::GEOMETRY_LINES;
			shaderNode         = getBrowser () -> getWireframeShader ();
			break;
		}
		case GeometryType::TRIANGLE:
		case GeometryType::QUAD:
		case GeometryType::SPRITE:
		{
			shaderGeometryType = X3D::GeometryType::GEOMETRY_3D;
			shaderNode         = getBrowser () -> getDefaultShader ();
			break;
		}
		case GeometryType::GEOMETRY:
		{
			shaderGeometryType = X3D::GeometryType::GEOMETRY_3D;
			shaderNode         = getBrowser () -> getDefaultShader ();
			break;
		}
	}
}

void
ParticleSystem::set_createParticles ()
{
	try
	{
		transformShader -> setField <SFBool> ("createParticles", createParticles ());
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ParticleSystem::set_colorKey ()
{
	set_color ();
}

void
ParticleSystem::set_texCoordKey ()
{
	set_texCoord ();
}

void
ParticleSystem::set_emitter ()
{
	// Emitter node

	emitterNode .set (x3d_cast <X3DParticleEmitterNode*> (emitter ()));

	if (not emitterNode)
		emitterNode .set (getBrowser () -> getDefaultEmitter ());

	// Shader

	set_transform_shader ();
}

void
ParticleSystem::set_colorRamp ()
{
	if (colorRampNode)
		colorRampNode -> removeInterest (&ParticleSystem::set_color, this);

	colorRampNode .set (x3d_cast <X3DColorNode*> (colorRamp ()));

	if (colorRampNode)
		colorRampNode -> addInterest (&ParticleSystem::set_color, this);

	set_color ();
}

void
ParticleSystem::set_color ()
{
	try
	{
		if (colorRampNode and not colorKey () .empty () and not colorRampNode -> isEmpty ())
		{
			numColors = std::min (colorKey () .size (), colorRampNode -> getSize ());

			// Keys

			std::vector <float> colorKeysArray (colorKey () .cbegin (), colorKey () .cend ());
			colorKeysArray .resize (numColors);

			glBindBuffer (GL_TEXTURE_BUFFER, colorRampBufferId [COLOR_RAMP_KEYS]);
			glBufferData (GL_TEXTURE_BUFFER, numColors * sizeof (float), colorKeysArray .data (), GL_STATIC_COPY);

			// Values

			std::vector <Vector4f> colorValuesArray;

			colorRampNode -> getHSVA (colorValuesArray);
			colorValuesArray .resize (numColors);

			glBindBuffer (GL_TEXTURE_BUFFER, colorRampBufferId [COLOR_RAMP_VALUES]);
			glBufferData (GL_TEXTURE_BUFFER, numColors * sizeof (Vector4f), colorValuesArray .data (), GL_STATIC_COPY);

			// Shader

			transformShader -> setField <SFInt32> ("numColors", int32_t (numColors));
		}
		else
		{
			numColors = 0;

			// Clear buffers

			for (size_t i = 0; i < 2; ++ i)
			{
				glBindBuffer (GL_TEXTURE_BUFFER, colorRampBufferId [i]);
				glBufferData (GL_TEXTURE_BUFFER, 0, 0, GL_STATIC_COPY);
			}

			// Shader

			transformShader -> setField <SFInt32> ("numColors", 0);
		}

		// Update textures

		glBindTexture (GL_TEXTURE_BUFFER, colorRampMapId [COLOR_RAMP_KEYS]);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, colorRampBufferId [COLOR_RAMP_KEYS]);

		glBindTexture (GL_TEXTURE_BUFFER, colorRampMapId [COLOR_RAMP_VALUES]);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_RGBA32F, colorRampBufferId [COLOR_RAMP_VALUES]);

		glBindTexture (GL_TEXTURE_BUFFER, 0);
		glBindBuffer (GL_TEXTURE_BUFFER, 0);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ParticleSystem::set_texCoordRamp ()
{
	if (texCoordRampNode)
		texCoordRampNode -> removeInterest (&ParticleSystem::set_texCoord, this);

	texCoordRampNode .set (x3d_cast <X3DTextureCoordinateNode*> (texCoordRamp ()));

	if (texCoordRampNode)
		texCoordRampNode -> addInterest (&ParticleSystem::set_texCoord, this);

	set_texCoord ();
}

void
ParticleSystem::set_texCoord ()
{
	try
	{
		if (texCoordRampNode and not texCoordKey () .empty () and not texCoordRampNode -> isEmpty () and numVertices)
		{
			numTexCoord = std::min (texCoordKey () .size (), texCoordRampNode -> getSize ());

			// Keys

			std::vector <float> texCoordKeysArray (texCoordKey () .cbegin (), texCoordKey () .cend ());
			texCoordKeysArray .resize (numTexCoord);

			glBindBuffer (GL_TEXTURE_BUFFER, texCoordRampBufferId [TEXCOORD_RAMP_KEYS]);
			glBufferData (GL_TEXTURE_BUFFER, numTexCoord * sizeof (float), texCoordKeysArray .data (), GL_STATIC_COPY);

			// Values

			std::vector <Vector4f> texCoord;
			std::vector <Vector4f> texCoordValuesArray;

			texCoordRampNode -> getTexCoord (texCoord);

			if (geometryTypeId == GeometryType::TRIANGLE)
			{
				static constexpr size_t numVertices = 4;

				texCoord .resize (numTexCoord * numVertices);

				for (size_t n = 0, size = texCoordKey () .size () * numVertices; n < size; n += numVertices)
				{
					for (size_t i = 1; i < 3; ++ i)
					{
						texCoordValuesArray .emplace_back (texCoord [n]);
						texCoordValuesArray .emplace_back (texCoord [n + i]);
						texCoordValuesArray .emplace_back (texCoord [n + i + 1]);
					}
				}
			}
			else
			{
				texCoordValuesArray = std::move (texCoord);
				texCoordValuesArray .resize (numTexCoord * numVertices);
			}

			glBindBuffer (GL_TEXTURE_BUFFER, texCoordRampBufferId [TEXCOORD_RAMP_VALUES]);
			glBufferData (GL_TEXTURE_BUFFER, texCoordValuesArray .size () * sizeof (Vector4f), texCoordValuesArray .data (), GL_STATIC_COPY);

			// Shader

			geometryShader -> setField <SFInt32> ("numTexCoord", int32_t (numTexCoord));
		}
		else
		{
			numTexCoord = 0;

			// Clear buffers

			for (size_t i = 0; i < 2; ++ i)
			{
				glBindBuffer (GL_TEXTURE_BUFFER, texCoordRampBufferId [i]);
				glBufferData (GL_TEXTURE_BUFFER, 0, 0, GL_STATIC_COPY);
			}

			// Shader

			geometryShader -> setField <SFInt32> ("numTexCoord", 0);
		}

		// Update textures

		glBindTexture (GL_TEXTURE_BUFFER, texCoordRampMapId [TEXCOORD_RAMP_KEYS]);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, texCoordRampBufferId [TEXCOORD_RAMP_KEYS]);

		glBindTexture (GL_TEXTURE_BUFFER, texCoordRampMapId [TEXCOORD_RAMP_VALUES]);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_RGBA32F, texCoordRampBufferId [TEXCOORD_RAMP_VALUES]);

		glBindTexture (GL_TEXTURE_BUFFER, 0);
		glBindBuffer (GL_TEXTURE_BUFFER, 0);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ParticleSystem::set_geometry ()
{ }

void
ParticleSystem::set_physics ()
{
	// X3DParticlePhysicsModelNode

	physicsModelNodes .clear ();

	for (const auto & node : physics ())
	{
		const auto physicsNode = x3d_cast <X3DParticlePhysicsModelNode*> (node);

		if (physicsNode)
			physicsModelNodes .emplace_back (physicsNode);
	}

	// BoundedPhysicsModel

	for (const auto & physicsNode : boundedPhysicsModelNodes)
		physicsNode -> removeInterest (boundedPhysicsModelNodes);

	boundedPhysicsModelNodes .clear ();

	for (const auto & node : physics ())
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
ParticleSystem::set_boundedPhysicsModel ()
{
	try
	{
		// Triangulate

		std::vector <Vector3f> normals;
		std::vector <Vector3f> vertices;

		for (auto & physicsNode : boundedPhysicsModelNodes)
			physicsNode -> addGeometry (normals, vertices);

		// Update shader

		transformShader -> setField <SFBool> ("boundedVolume", not vertices .empty (), true);

		// Upload

		glBindBuffer (GL_TEXTURE_BUFFER, boundedNormalBufferId);
		glBufferData (GL_TEXTURE_BUFFER, normals .size () * sizeof (Vector3f), normals .empty () ? 0 : normals .data (), GL_STATIC_COPY);

		glBindBuffer (GL_TEXTURE_BUFFER, boundedSurfaceBufferId);
		glBufferData (GL_TEXTURE_BUFFER, vertices .size () * sizeof (Vector3f), vertices .empty () ? 0 : vertices .data (), GL_STATIC_COPY);

		// BVH

		const BVH <float> tree (std::move (normals), std::move (vertices));
		const auto treeArray = tree .toArray ();

		glBindBuffer (GL_TEXTURE_BUFFER, boundedVolumeBufferId);
		glBufferData (GL_TEXTURE_BUFFER, treeArray .size () * sizeof (BVH <float>::ArrayValue), treeArray .empty () ? 0 : treeArray .data (), GL_STATIC_COPY);

		// Update textures

		glBindTexture (GL_TEXTURE_BUFFER, boundedNormalMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, boundedNormalBufferId);

		glBindTexture (GL_TEXTURE_BUFFER, boundedSurfaceMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_RGB32F, boundedSurfaceBufferId);

		glBindTexture (GL_TEXTURE_BUFFER, boundedVolumeMapId);
		glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, boundedVolumeBufferId);

		glBindTexture (GL_TEXTURE_BUFFER, 0);
		glBindBuffer (GL_TEXTURE_BUFFER, 0);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ParticleSystem::set_particle_buffers ()
{
	// Initialize array buffers

	const size_t maxParticles = std::max (0, this -> maxParticles () .getValue ());

	// Particle buffers
	
	std::vector <Particle> particleArray;
	
	if (numParticles)
	{
		glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);
	
		const auto particles = static_cast <const Particle*> (glMapBufferRange (GL_ARRAY_BUFFER, 0, sizeof (Particle) * numParticles, GL_MAP_READ_BIT));
	
		if (particles)
			particleArray .assign (particles, particles + numParticles);

		glUnmapBuffer (GL_ARRAY_BUFFER);
	}

	particleArray .resize (maxParticles);

	for (size_t i = 0; i < 2; ++ i)
	{
		glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [i]);
		glBufferData (GL_ARRAY_BUFFER, particleArray .size () * sizeof (Particle), particleArray .data (), GL_STATIC_COPY);
	}

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	
	numParticles = std::min <size_t> (numParticles, maxParticles);

	// Vertex buffer

	set_vertex_buffer ();

	// Reset state

	if (not emitterNode -> isExplosive ())
		creationTime = SFTime::now ();
}

void
ParticleSystem::set_vertex_buffer ()
{
	// Initialize array buffers

	const size_t maxParticles = std::max (0, this -> maxParticles () .getValue ());

	// Vertex buffer

	std::vector <Vertex> vertexArray (maxParticles * numVertices);

	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData (GL_ARRAY_BUFFER, vertexArray .size () * sizeof (Vertex), vertexArray .data (), GL_STATIC_COPY);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
ParticleSystem::set_transform_shader ()
{
	X3DPtr <ShaderPart> vertexPart (new ShaderPart (getExecutionContext ()));
	vertexPart -> url () = emitterNode -> getShaderUrl ();
	vertexPart -> setup ();

	// Transform shader

	transformShader = new ComposedShader (getExecutionContext ());
	transformShader -> addUserDefinedField (inputOutput, "deltaTime",         new SFFloat (0.001));
	transformShader -> addUserDefinedField (inputOutput, "createParticles",   new SFBool (createParticles ()));
	transformShader -> addUserDefinedField (inputOutput, "particleLifetime",  new SFFloat (particleLifetime ()));
	transformShader -> addUserDefinedField (inputOutput, "lifetimeVariation", new SFFloat (lifetimeVariation ()));

	// Emitter

	emitterNode -> addShaderFields (transformShader);

	// Particle map

	transformShader -> addUserDefinedField (inputOutput, "modelViewMatrix",   new SFMatrix4f ());
	transformShader -> addUserDefinedField (inputOutput, "stride",            new SFInt32 (sizeof (Particle) / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "seedOffset",        new SFInt32 (offsetof (Particle, seed)        / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "lifetimeOffset",    new SFInt32 (offsetof (Particle, lifetime)    / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "positionOffset",    new SFInt32 (offsetof (Particle, position)    / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "velocityOffset",    new SFInt32 (offsetof (Particle, velocity)    / sizeof (float)));
	//transformShader -> addUserDefinedField (inputOutput, "colorOffset",     new SFInt32 (offsetof (Particle, color)       / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "elapsedTimeOffset", new SFInt32 (offsetof (Particle, elapsedTime) / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "distanceOffset",    new SFInt32 (offsetof (Particle, distance)    / sizeof (float)));

	// Color ramp

	transformShader -> addUserDefinedField (inputOutput, "numColors", new SFInt32 (numColors));

	// Forces

	transformShader -> addUserDefinedField (inputOutput, "velocity",      new MFVec3f ());
	transformShader -> addUserDefinedField (inputOutput, "turbulence",    new MFFloat ());
	transformShader -> addUserDefinedField (inputOutput, "numForces",     new SFInt32 ());
	transformShader -> addUserDefinedField (inputOutput, "boundedVolume", new SFBool ());

	// BHV

	transformShader -> addUserDefinedField (inputOutput, "bvhStride",      new SFInt32 (sizeof (BVH <float>::ArrayValue) / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "bvhTypeOffset",  new SFInt32 (offsetof (BVH <float>::ArrayValue, type)  / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "bvhMinOffset",   new SFInt32 (offsetof (BVH <float>::ArrayValue, min)   / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "bvhMaxOffset",   new SFInt32 (offsetof (BVH <float>::ArrayValue, max)   / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "bvhLeftOffset",  new SFInt32 (offsetof (BVH <float>::ArrayValue, left)  / sizeof (float)));
	transformShader -> addUserDefinedField (inputOutput, "bvhRightOffset", new SFInt32 (offsetof (BVH <float>::ArrayValue, right) / sizeof (float)));

	// Sort algorithm

	sortAlgorithm -> setup (transformShader);

	// Setup

	transformShader -> language () = "GLSL";
	transformShader -> parts () .emplace_back (vertexPart);
	transformShader -> setTransformFeedbackVaryings ({
	                                                    "To.seed",
	                                                    "To.lifetime",
	                                                    "To.position",
	                                                    "To.velocity",
	                                                    "To.color",
	                                                    "To.elapsedTime",
	                                                    "To.distance"
																	 });

	transformShader -> isValid () .addInterest (&ParticleSystem::set_transform_shader_texture_buffers, this);
	transformShader -> setup ();
}

void
ParticleSystem::set_transform_shader_texture_buffers ()
{
	// Color ramp

	transformShader -> enable ();
	transformShader -> setTextureBuffer ("particleMap",  GL_TEXTURE_BUFFER, particleMapId);
	transformShader -> setTextureBuffer ("colorKeyMap",  GL_TEXTURE_BUFFER, colorRampMapId [COLOR_RAMP_KEYS]);
	transformShader -> setTextureBuffer ("colorRampMap", GL_TEXTURE_BUFFER, colorRampMapId [COLOR_RAMP_VALUES]);

	transformShader -> setTextureBuffer ("boundedNormalMap",  GL_TEXTURE_BUFFER, boundedNormalMapId);
	transformShader -> setTextureBuffer ("boundedSurfaceMap", GL_TEXTURE_BUFFER, boundedSurfaceMapId);
	transformShader -> setTextureBuffer ("boundedVolumeMap",  GL_TEXTURE_BUFFER, boundedVolumeMapId);
	transformShader -> disable ();

	// Emitter

	emitterNode -> setTextureBuffer (transformShader);
}

void
ParticleSystem::swap_particle_buffers ()
{
	std::swap (readBuffer, writeBuffer);
	set_particle_map ();
}

void
ParticleSystem::set_particle_map ()
{
	glBindTexture (GL_TEXTURE_BUFFER, particleMapId);
	glTexBuffer (GL_TEXTURE_BUFFER, GL_R32F, particleBufferId [readBuffer]);
	glBindTexture (GL_TEXTURE_BUFFER, 0);
}

void
ParticleSystem::set_geometry_shader ()
{
	X3DPtr <ShaderPart> vertexPart (new ShaderPart (getExecutionContext ()));
	vertexPart -> url () = { get_shader ("ParticleSystems/Primitive.vs") .str () };
	vertexPart -> setup ();

	geometryShader = new ComposedShader (getExecutionContext ());
	geometryShader -> addUserDefinedField (inputOutput, "numVertices",  new SFInt32 (numVertices));
	geometryShader -> addUserDefinedField (inputOutput, "geometryType", new SFInt32 (int32_t (geometryTypeId)));
	geometryShader -> addUserDefinedField (inputOutput, "rotation",     new SFMatrix3f ());
	geometryShader -> addUserDefinedField (inputOutput, "texCoord",     new MFVec4f ());
	geometryShader -> addUserDefinedField (inputOutput, "numTexCoord",  new SFInt32 (numTexCoord));

	geometryShader -> language () = "GLSL";
	geometryShader -> parts () .emplace_back (vertexPart);
	geometryShader -> setTransformFeedbackVaryings ({
	                                                   "To.position", "To.color", "To.texCoord"
																	});

	geometryShader -> isValid () .addInterest (&ParticleSystem::set_geometry_shader_texture_buffers, this);
	geometryShader -> setup ();
}

void
ParticleSystem::set_geometry_shader_texture_buffers ()
{
	geometryShader -> enable ();
	geometryShader -> setTextureBuffer ("texCoordKeyMap",  GL_TEXTURE_BUFFER, texCoordRampMapId [TEXCOORD_RAMP_KEYS]);
	geometryShader -> setTextureBuffer ("texCoordRampMap", GL_TEXTURE_BUFFER, texCoordRampMapId [TEXCOORD_RAMP_VALUES]);
	geometryShader -> disable ();
}

bool
ParticleSystem::intersects (const Box3d & box,
                            const ClipPlaneContainerArray & clipPlanes,
                            const Matrix4d & modelViewMatrix)
{
	return false;
}

void
ParticleSystem::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (isHidden ())
		return;
		
	if (not isActive ())
		return;

	switch (type)
	{
		case TraverseType::POINTER:
		{
			//renderObject -> getBrowser () -> getHierarchy () .emplace_back (this);
			//renderObject -> getBrowser () -> getHierarchy () .emplace_back (getGeometry ());
			//
			//
			//renderObject -> getBrowser () -> getHierarchy () .pop_back ();
			//renderObject -> getBrowser () -> getHierarchy () .pop_back ();
			break;
		}
		case TraverseType::COLLISION:
		{
			renderObject -> addCollisionShape (this);
			break;
		}
		case TraverseType::DEPTH:
		{
			renderObject -> addDepthShape (this);
			break;
		}
		case TraverseType::DISPLAY:
		{
			if (renderObject -> addDisplayShape (this))
				getAppearance () -> traverse (type, renderObject);

			break;
		}
		default:
			break;
	}

	if (geometryTypeId == GeometryType::GEOMETRY)
	{
		if (getGeometry ())
			getGeometry () -> traverse (type, renderObject);
	}
}

void
ParticleSystem::animateParticles ()
{
	try
	{
		// Create new particles if possible.

		if (emitterNode -> isExplosive ())
		{
			const auto now = SFTime::now ();
			
			if (numParticles == 0 or now - creationTime > particleLifetime () + particleLifetime () * lifetimeVariation ())
			{
				creationTime = now;
				numParticles = maxParticles ();
				emitterNode -> resetShader ();
			}
		}
		else
		{
			if (numParticles < maxParticles ())
			{
				const auto now          = SFTime::now ();
				int32_t    newParticles = (now - creationTime) * maxParticles () / particleLifetime ();

				if (newParticles)
					creationTime = now;

				numParticles = std::min (std::max (0, maxParticles () .getValue ()), newParticles + numParticles);
			}
		}

		// Update shader

		constexpr float DELAY = 15; // Delay in frames when dt full applys.

		const float dt        = 1 / std::max (10.0, getBrowser () -> getCurrentFrameRate ());;
		auto &      deltaTime = transformShader -> getField <SFFloat> ("deltaTime");

		deltaTime = ((DELAY - 1) * deltaTime + dt) / DELAY; // Moving average about DELAY frames.
		transformShader -> setField <SFFloat> ("particleLifetime",  particleLifetime (),  true);
		transformShader -> setField <SFFloat> ("lifetimeVariation", lifetimeVariation (), true);

		emitterNode -> setShaderFields (transformShader);

		if (emitterNode -> mass ())
		{
			MFVec3f velocity;
			MFFloat turbulence;

			for (const auto & physicsModelNode : physicsModelNodes)
				physicsModelNode -> addForce (emitterNode, velocity, turbulence);

			for (MFVec3f::reference v : velocity)
				v = v .get () * float (deltaTime / emitterNode -> mass ());

			transformShader -> setField <MFVec3f> ("velocity",   velocity,                    true);
			transformShader -> setField <MFFloat> ("turbulence", turbulence,                  true);
			transformShader -> setField <SFInt32> ("numForces",  int32_t (velocity .size ()), true);
		}
		else
			transformShader -> setField <SFInt32> ("numForces", 0, true);

		// Update sort algorithm

		sortAlgorithm -> sortStep (transformShader, numParticles);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ParticleSystem::updateParticles ()
{
	// Transform particles.

	transformShader -> enable ();

	glEnable (GL_RASTERIZER_DISCARD);
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, particleFeedbackId [writeBuffer]);
	glBeginTransformFeedback (GL_POINTS);

	glDrawArrays (GL_POINTS, 0, numParticles);

	glEndTransformFeedback ();
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);
	glDisable (GL_RASTERIZER_DISCARD);

	transformShader -> disable ();

	// Swap particle buffers.

	swap_particle_buffers ();

	if (geometryTypeId != GeometryType::SPRITE)
		updateGeometry (Matrix4d ());
}

void
ParticleSystem::updateGeometry (const Matrix4d & modelViewMatrix)
{
	// Geometry shader

	switch (geometryTypeId)
	{
		case GeometryType::POINT:
			break;
		case GeometryType::SPRITE:
		{
			try
			{
				if (getExecutionContext () -> isLive () and isLive ())
				{
					geometryShader -> enable ();
	
					const auto rotationLocation = glGetUniformLocation (geometryShader -> getProgramId (), "rotation");
					const auto rotation         = getScreenAlignedRotation (modelViewMatrix);
	
					glUniformMatrix3fv (rotationLocation, 1, false, Matrix3f (rotation) .front () .data ());
				}
			}
			catch (const std::domain_error &)
			{ }

			[[fallthrough]];
		}
		case GeometryType::LINE:
		case GeometryType::TRIANGLE:
		case GeometryType::QUAD:
		{
			geometryShader -> enable ();

			glEnableVertexAttribArray (0);
			glEnableVertexAttribArray (1);
			glEnableVertexAttribArray (2);
			glEnableVertexAttribArray (3);
			glEnableVertexAttribArray (4);
			glEnableVertexAttribArray (5);

			glBindBuffer (GL_ARRAY_BUFFER, geometryBufferId);
			glVertexAttribPointer (0, 3, GL_FLOAT, false, 0, (void*) 0);

			glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);
			glVertexAttribPointer (1, 1, GL_FLOAT, false, sizeof (Particle), (void*) offsetof (Particle, lifetime));
			glVertexAttribPointer (2, 3, GL_FLOAT, false, sizeof (Particle), (void*) offsetof (Particle, position));
			glVertexAttribPointer (3, 3, GL_FLOAT, false, sizeof (Particle), (void*) offsetof (Particle, velocity));
			glVertexAttribPointer (4, 4, GL_FLOAT, false, sizeof (Particle), (void*) offsetof (Particle, color));
			glVertexAttribPointer (5, 1, GL_FLOAT, false, sizeof (Particle), (void*) offsetof (Particle, elapsedTime));

			glVertexAttribDivisor (1, 1);
			glVertexAttribDivisor (2, 1);
			glVertexAttribDivisor (3, 1);
			glVertexAttribDivisor (4, 1);
			glVertexAttribDivisor (5, 1);

			glEnable (GL_RASTERIZER_DISCARD);
			glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, vertexFeedbackId);
			glBeginTransformFeedback (GL_POINTS);

			glDrawArraysInstanced (GL_POINTS, 0, numVertices, numParticles);

			glEndTransformFeedback ();
			glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);
			glDisable (GL_RASTERIZER_DISCARD);

			glVertexAttribDivisor (1, 0);
			glVertexAttribDivisor (2, 0);
			glVertexAttribDivisor (3, 0);
			glVertexAttribDivisor (4, 0);
			glVertexAttribDivisor (5, 0);

			glDisableVertexAttribArray (0);
			glDisableVertexAttribArray (1);
			glDisableVertexAttribArray (2);
			glDisableVertexAttribArray (3);
			glDisableVertexAttribArray (4);
			glDisableVertexAttribArray (5);
			glBindBuffer (GL_ARRAY_BUFFER, 0);

			geometryShader -> disable ();
			break;
		}
		case GeometryType::GEOMETRY:
		{
			break;
		}
	}
}

void
ParticleSystem::depth (const X3DShapeContainer* const context)
{
	if (isSoftSystem ())
	{
		softSystem -> depth (context);
		return;
	}

	// Geometry shader.

	if (geometryTypeId == GeometryType::SPRITE)
		updateGeometry (context -> getModelViewMatrix ());

	// Draw.

	switch (geometryTypeId)
	{
		case GeometryType::POINT:
		{
			glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);

			glEnableClientState (GL_VERTEX_ARRAY);
			glVertexPointer (3, GL_FLOAT, sizeof (Particle), (void*) offsetof (Particle, position));

			glDrawArrays (GL_POINTS, 0, numParticles);

			glDisableClientState (GL_VERTEX_ARRAY);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
			break;
		}
		case GeometryType::LINE:
		case GeometryType::TRIANGLE:
		case GeometryType::QUAD:
		case GeometryType::SPRITE:
		{
			glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);

			glEnableClientState (GL_VERTEX_ARRAY);
			glVertexPointer (3, GL_FLOAT, sizeof (Vertex), (void*) offsetof (Vertex, position));

			glDrawArrays (glGeometryType, 0, numParticles * numVertices);

			glDisableClientState (GL_VERTEX_ARRAY);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
			break;
		}
		case GeometryType::GEOMETRY:
		{
			const auto geometryNode = x3d_cast <X3DGeometryNode*> (geometry ());

			if (geometryNode)
			{
				std::vector <Vector3f> positions;
				positions .reserve (numParticles);

				// Copy positions.

				glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);
				const auto particles = static_cast <const Particle*> (glMapBufferRange (GL_ARRAY_BUFFER, 0, sizeof (Particle) * numParticles, GL_MAP_READ_BIT));

				for (const auto & particle : basic::make_range (particles, numParticles))
					positions .emplace_back (particle .position);

				glUnmapBuffer (GL_ARRAY_BUFFER);
				glBindBuffer (GL_ARRAY_BUFFER, 0);

				// Draw geometries.

				for (const auto & position : positions)
				{
					glPushMatrix ();

					glTranslatef (position .x (),
					              position .y (),
					              position .z ());

					geometryNode -> depth (context);

					glPopMatrix ();
				}
			}

			break;
		}
	}
}

void
ParticleSystem::draw (ShapeContainer* const context)
{
	try
	{
		if (isSoftSystem ())
		{
			softSystem -> draw (context);
			return;
		}

		// Geometry shader.
	
		if (geometryTypeId == GeometryType::SPRITE)
			updateGeometry (context -> getModelViewMatrix ());

		// Draw.

		const auto browser    = context -> getBrowser ();
		auto       shaderNode = browser -> getShader ();

		if (shaderNode == browser -> getDefaultShader ())
			shaderNode = this -> shaderNode;

		context -> setGeometryType  (getGeometryType ());
		context -> setColorMaterial (numColors);

		glEnable (GL_CULL_FACE);
		glFrontFace (context -> getModelViewMatrix () .submatrix () .determinant () > 0 ? GL_CCW : GL_CW);
		glNormal3f (0, 0, 1);

		switch (geometryTypeId)
		{
			case GeometryType::POINT:
			{
				if (browser -> getFixedPipelineRequired ())
				{
					glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);
	
					if (numColors)
					{
						if (glIsEnabled (GL_LIGHTING))
							glEnable (GL_COLOR_MATERIAL);
	
						glEnableClientState (GL_COLOR_ARRAY);
						glColorPointer (4, GL_FLOAT, sizeof (Particle), (void*) offsetof (Particle, color));
					}
	
					glEnableClientState (GL_VERTEX_ARRAY);
					glVertexPointer (3, GL_FLOAT, sizeof (Particle), (void*) offsetof (Particle, position));
	
					glDrawArrays (GL_POINTS, 0, numParticles);
	
					glDisableClientState (GL_COLOR_ARRAY);
					glDisableClientState (GL_VERTEX_ARRAY);
					glBindBuffer (GL_ARRAY_BUFFER, 0);
					break;
				}

				// Enable shader.
			
				shaderNode -> enable ();
				shaderNode -> setLocalUniforms (context);

				// Enable vertex attribute nodes.
		
				if (numColors)
					shaderNode -> enableColorAttrib (particleBufferId [readBuffer], GL_FLOAT, sizeof (Particle), (void*) offsetof (Particle, color));
			
				shaderNode -> enableVertexAttrib (particleBufferId [readBuffer], GL_FLOAT, sizeof (Particle), (void*) offsetof (Particle, position));

				// Draw.

				glDrawArrays (GL_POINTS, 0, numParticles);

				// Disable shader.
				
				shaderNode -> disableColorAttrib ();
				shaderNode -> disableVertexAttrib ();
				shaderNode -> disable ();

				glBindBuffer (GL_ARRAY_BUFFER, 0);
				break;
			}
			case GeometryType::SPRITE:
			{
				try
				{
					const auto & rotation = geometryShader -> getField <SFMatrix3f> ("rotation") .getValue ();

					glNormal3fv (normalize (cross (rotation [0], rotation [1])) .data ());
				}
				catch (const std::exception &)
				{ }

				[[fallthrough]];
			}
			case GeometryType::LINE:
			case GeometryType::TRIANGLE:
			case GeometryType::QUAD:
			{
				if (browser -> getFixedPipelineRequired ())
				{
					glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
	
					if (numColors)
					{
						if (glIsEnabled (GL_LIGHTING))
							glEnable (GL_COLOR_MATERIAL);
	
						glEnableClientState (GL_COLOR_ARRAY);
						glColorPointer (4, GL_FLOAT, sizeof (Vertex), (void*) offsetof (Vertex, color));
					}
	
					if (browser -> getTexture ())
						enableTexCoord (browser);
	
					glEnableClientState (GL_VERTEX_ARRAY);
					glVertexPointer (3, GL_FLOAT, sizeof (Vertex), (void*) offsetof (Vertex, position));
	
					glDrawArrays (glGeometryType, 0, numParticles * numVertices);
	
					if (browser -> getTexture ())
						disableTexCoord (browser);
	
					glDisableClientState (GL_COLOR_ARRAY);
					glDisableClientState (GL_VERTEX_ARRAY);
					glBindBuffer (GL_ARRAY_BUFFER, 0);
					break;
				}

				// Enable shader.
			
				shaderNode -> enable ();
				shaderNode -> setLocalUniforms (context);
		
				// Enable vertex attribute nodes
		
				if (numColors)
					shaderNode -> enableColorAttrib (vertexBufferId, GL_FLOAT, sizeof (Vertex), (void*) offsetof (Vertex, color));
			
				shaderNode -> enableTexCoordAttrib ({ vertexBufferId }, GL_FLOAT, { sizeof (Vertex) }, { (void*) offsetof (Vertex, texCoord) });
				shaderNode -> enableVertexAttrib (vertexBufferId, GL_FLOAT, sizeof (Vertex), (void*) offsetof (Vertex, position));

				// Draw.
			
				glDrawArrays (glGeometryType, 0, numParticles * numVertices);

				// Disable shader.
			
				shaderNode -> disableColorAttrib ();
				shaderNode -> disableTexCoordAttrib ();
				shaderNode -> disableVertexAttrib ();
				shaderNode -> disable ();

				glBindBuffer (GL_ARRAY_BUFFER, 0);
				break;
			}
			case GeometryType::GEOMETRY:
			{
				const auto geometryNode = x3d_cast <X3DGeometryNode*> (geometry ());

				if (geometryNode)
				{
					std::vector <Vector3f> positions;
					positions .reserve (numParticles);

					// Copy positions.

					glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);
					const auto particles = static_cast <const Particle*> (glMapBufferRange (GL_ARRAY_BUFFER, 0, sizeof (Particle) * numParticles, GL_MAP_READ_BIT));

					for (const auto & particle : basic::make_range (particles, numParticles))
						positions .emplace_back (particle .position);

					glUnmapBuffer (GL_ARRAY_BUFFER);
					glBindBuffer (GL_ARRAY_BUFFER, 0);

					// Draw geometries.

					if (browser -> getFixedPipelineRequired ())
					{
						for (const auto & position : positions)
						{
							glPushMatrix ();
	
							glTranslatef (position .x (),
							              position .y (),
							              position .z ());
	
							geometryNode -> draw (context);
	
							glPopMatrix ();
						}

						break;
					}

					const auto modelViewMatrix = context -> getModelViewMatrix ();

					for (const auto & position : positions)
					{
						auto translatedModelViewMatrix = modelViewMatrix;

						translatedModelViewMatrix .translate (position);

						context -> setModelViewMatrix (translatedModelViewMatrix);

						geometryNode -> draw (context);
					}
				}

				break;
			}
		}

		// Set model view matrix for transparency sorting.

		if (getExecutionContext () -> isLive () and isLive ())
			transformShader -> setField <SFMatrix4f> ("modelViewMatrix", context -> getModelViewMatrix ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

///  throws std::domain_error
Matrix3d
ParticleSystem::getScreenAlignedRotation (const Matrix4d & modelViewMatrix) const
{
	const auto invModelViewMatrix = inverse (modelViewMatrix);
	const auto billboardToScreen  = normalize (invModelViewMatrix .mult_dir_matrix (zAxis));
	const auto viewerYAxis        = normalize (invModelViewMatrix .mult_dir_matrix (yAxis));

	auto x = cross (viewerYAxis, billboardToScreen);
	auto y = cross (billboardToScreen, x);
	auto z = billboardToScreen;

	// Compose rotation

	x .normalize ();
	y .normalize ();

	const auto rotation = Matrix3d (x [0], x [1], x [2],
	                                y [0], y [1], y [2],
	                                z [0], z [1], z [2]);

	return rotation;
}

void
ParticleSystem::enableTexCoord (X3DBrowser* const browser) const
{
	if (browser -> getTextureStages () .empty ())
	{
		glClientActiveTexture (GL_TEXTURE0);
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer (4, GL_FLOAT, sizeof (Vertex), (void*) offsetof (Vertex, texCoord));
	}
	else
	{
		for (const auto & unit : browser -> getTextureStages ())
		{
			if (unit >= 0)
			{
				glClientActiveTexture (GL_TEXTURE0 + unit);
				glEnableClientState (GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer (4, GL_FLOAT, sizeof (Vertex), (void*) offsetof (Vertex, texCoord));
			}
		}
	}
}

void
ParticleSystem::disableTexCoord (X3DBrowser* const browser) const
{
	if (browser -> getTextureStages () .empty ())
	{
		glClientActiveTexture (GL_TEXTURE0);
		glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		for (const auto & unit : browser -> getTextureStages ())
		{
			if (unit >= 0)
			{
				glClientActiveTexture (GL_TEXTURE0 + unit);
				glDisableClientState (GL_TEXTURE_COORD_ARRAY);
			}
		}
	}
}

void
ParticleSystem::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		softSystem -> dispose ();

		// Particle buffer

		if (particleFeedbackId [0])
			glDeleteTransformFeedbacks (2, particleFeedbackId .data ());

		if (particleMapId)
			glDeleteTextures (1, &particleMapId);

		if (particleBufferId [0])
			glDeleteBuffers (2, particleBufferId .data ());

		// Vertex buffer

		if (vertexFeedbackId)
			glDeleteTransformFeedbacks (1, &vertexFeedbackId);

		if (vertexBufferId)
			glDeleteBuffers (1, &vertexBufferId);

		// Primitive geometry buffer

		if (geometryBufferId)
			glDeleteBuffers (1, &geometryBufferId);

		// Color ramp texture buffer

		if (colorRampMapId [0])
			glDeleteTextures (2, colorRampMapId .data ());

		if (colorRampBufferId [0])
			glDeleteBuffers (2, colorRampBufferId .data ());

		// Color ramp texture buffer

		if (texCoordRampMapId [0])
			glDeleteTextures (2, texCoordRampMapId .data ());

		if (texCoordRampBufferId [0])
			glDeleteBuffers (2, texCoordRampBufferId .data ());

		// Bounded texture buffers

		if (boundedNormalMapId)
			glDeleteTextures (1, &boundedNormalMapId);

		if (boundedNormalBufferId)
			glDeleteBuffers (1, &boundedNormalBufferId);

		if (boundedSurfaceMapId)
			glDeleteTextures (1, &boundedSurfaceMapId);

		if (boundedSurfaceBufferId)
			glDeleteBuffers (1, &boundedSurfaceBufferId);

		if (boundedVolumeMapId)
			glDeleteTextures (1, &boundedVolumeMapId);

		if (boundedVolumeBufferId)
			glDeleteBuffers (1, &boundedVolumeBufferId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	// Dispose base

	X3DShapeNode::dispose ();
}

ParticleSystem::~ParticleSystem ()
{ }

} // X3D
} // titania

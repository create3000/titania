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
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/ShaderPart.h"

#include <cstddef>

#include "../../Debug.h"

namespace titania {
namespace X3D {

const std::string ParticleSystem::componentName  = "ParticleSystems";
const std::string ParticleSystem::typeName       = "ParticleSystem";
const std::string ParticleSystem::containerField = "children";

struct Particle
{
	float lifetime;
	Vector3f position;
	Vector3f velocity;

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
	          firstTime (true),
	         readBuffer (0),
	        writeBuffer (1),
	transformFeedbackId ({0, 0}),
	   particleBufferId ({0, 0}),
	             shader ()
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
		// Generate buffers

		Particle particles [maxParticles () .getValue ()];
	
		glGenBuffers (2, particleBufferId .data ());
		glGenTransformFeedbacks (2, transformFeedbackId .data ());

		for (size_t i = 0; i < 2; ++ i)
		{
			glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [i]);
			glBufferData (GL_ARRAY_BUFFER, sizeof (particles), particles, GL_DYNAMIC_DRAW);

			glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, transformFeedbackId [i]);
			glBindBufferBase (GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBufferId [i]);
		}

		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);

		// Shader

		X3DSFNode <ShaderPart> vertexShader = new ShaderPart (getExecutionContext ());
		vertexShader -> url () = { get_shader ("ParticleSystems/emitter.vs") .str () };
		vertexShader -> setup ();

		shader                = new ComposedShader (getExecutionContext ());
		shader -> language () = "GLSL";
		shader -> parts () .emplace_back (vertexShader);
		shader -> setTransformFeedbackVaryings ({ "lifetime0", "position0", "velocity0" });
		shader -> setup ();
	}
}

bool
ParticleSystem::isTransparent () const
{
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
ParticleSystem::update ()
{
	__LOG__ << std::endl;
	GL_ERROR;

	shader -> enable ();

	glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);

	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	glEnableVertexAttribArray (2);

	glVertexAttribPointer (0, 1, GL_FLOAT, GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, lifetime));
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, position));
	glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, velocity));

	glEnable (GL_RASTERIZER_DISCARD);
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, transformFeedbackId [writeBuffer]);
	glBeginTransformFeedback (GL_POINTS);

	if (firstTime)
	{
		firstTime = false;
		glDrawArrays (GL_POINTS, 0, 1);
	}
	else
		glDrawTransformFeedback (GL_POINTS, transformFeedbackId [readBuffer]);

	glEndTransformFeedback ();
	glBindTransformFeedback (GL_TRANSFORM_FEEDBACK, 0);
	glDisable (GL_RASTERIZER_DISCARD);

	glDisableVertexAttribArray (0);
	glDisableVertexAttribArray (1);
	glDisableVertexAttribArray (2);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	shader -> disable ();

	std::swap (readBuffer, writeBuffer);

	GL_ERROR;
}

void
ParticleSystem::draw ()
{
	update ();

	__LOG__ << std::endl;
	GL_ERROR;

	glDisable (GL_LIGHTING);
	glColor4f (1, 1, 1, 1);

	glBindBuffer (GL_ARRAY_BUFFER, particleBufferId [readBuffer]);

	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (Particle), (const GLvoid*) offsetof (Particle, position));

	glDrawTransformFeedback (GL_POINTS, transformFeedbackId [readBuffer]);

	glDisableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	GL_ERROR;
}

void
ParticleSystem::drawGeometry ()
{ }

void
ParticleSystem::dispose ()
{
	if (transformFeedbackId [0])
		glDeleteTransformFeedbacks (2, transformFeedbackId .data ());

	if (particleBufferId [0])
		glDeleteBuffers (2, particleBufferId .data ());

	shader .dispose ();

	X3DShapeNode::dispose ();
}

} // X3D
} // titania

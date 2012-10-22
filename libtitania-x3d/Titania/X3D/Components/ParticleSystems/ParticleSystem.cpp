/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "ParticleSystem.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

ParticleSystem::ParticleSystem (X3DExecutionContext* const executionContext) :
	     X3DBasicNode (executionContext -> getBrowser (), executionContext),
	     X3DShapeNode (),                                 
	  createParticles (true),                             // SFBool   [in,out] createParticles    TRUE
	          enabled (true),                             // SFBool   [in,out] enabled            TRUE
	lifetimeVariation (0.25),                             // SFFloat  [in,out] lifetimeVariation  0.25             [0,1]
	     maxParticles (200),                              // SFInt32  [in,out] maxParticles       200              [0,∞)
	 particleLifetime (5),                                // SFFloat  [in,out] particleLifetime   5                [0,∞)
	     particleSize (0.02, 0.02),                       // SFVec2f  [in,out] particleSize       0.02 0.02        [0,∞)
	         isActive (),                                 // SFBool   [out]    isActive
	        colorRamp (),                                 // SFNode   [ ]      colorRamp          NULL             [X3DColorNode]
	         colorKey (),                                 // MFFloat  [ ]      colorKey           NULL             [0,∞)
	          emitter (),                                 // SFNode   [ ]      emitter            NULL             [X3DParticleEmitterNode]
	     geometryType ("QUAD"),                           // SFString [ ]      geometryType       "QUAD"           ["LINE"|"POINT"|"QUAD"|"SPRITE"|"TRIANGLE"|"GEOMETRY"|...]
	          physics (),                                 // MFNode   [ ]      physics            [ ]               [X3DParticlePhysicsModelNode]
	     texCoordRamp (),                                 // SFNode   [ ]      texCoordRamp       NULL             [TextureCoordinate]
	      texCoordKey ()                                  // MFFloat  [ ]      texCoordKey        [ ]               [0,∞)
{
	setComponent ("ParticleSystems");
	setTypeName ("ParticleSystem");

	appendField (inputOutput,    "metadata",          metadata);
	appendField (initializeOnly, "bboxSize",          bboxSize);
	appendField (initializeOnly, "bboxCenter",        bboxCenter);
	appendField (inputOutput,    "appearance",        appearance);
	appendField (inputOutput,    "geometry",          geometry);
	appendField (inputOutput,    "createParticles",   createParticles);
	appendField (inputOutput,    "enabled",           enabled);
	appendField (inputOutput,    "lifetimeVariation", lifetimeVariation);
	appendField (inputOutput,    "maxParticles",      maxParticles);
	appendField (inputOutput,    "particleLifetime",  particleLifetime);
	appendField (inputOutput,    "particleSize",      particleSize);
	appendField (outputOnly,     "isActive",          isActive);
	appendField (initializeOnly, "colorRamp",         colorRamp);
	appendField (initializeOnly, "colorKey",          colorKey);
	appendField (initializeOnly, "emitter",           emitter);
	appendField (initializeOnly, "geometryType",      geometryType);
	appendField (initializeOnly, "physics",           physics);
	appendField (initializeOnly, "texCoordRamp",      texCoordRamp);
	appendField (initializeOnly, "texCoordKey",       texCoordKey);
}

X3DBasicNode*
ParticleSystem::create (X3DExecutionContext* const executionContext) const
{
	return new ParticleSystem (executionContext);
}

} // X3D
} // titania

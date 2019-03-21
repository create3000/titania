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

#ifndef __TITANIA_X3D_BROWSER_PARTICLE_SYSTEMS_SOFT_SYSTEM_H__
#define __TITANIA_X3D_BROWSER_PARTICLE_SYSTEMS_SOFT_SYSTEM_H__

#include "../../Browser/ParticleSystems/SoftParticle.h"
#include "../../Components/ParticleSystems/ParticleSystem.h"

namespace titania {
namespace X3D {

template <class Type>
class BVH;

class SoftSystem
{
public:

	///  @name Construction

	SoftSystem (ParticleSystem* const particleSystem);

	void
	setExecutionContext (X3DExecutionContext* const executionContext);

	void
	initialize ();

	///  @name Operations

	Box3d
	getBBox () const;

	bool
	isTransparent () const;

	void
	depth (const X3DShapeContainer* const context);

	void
	draw (ShapeContainer* const context);

	///  @name Destruction

	void
	dispose ();

	~SoftSystem ();


private:

	///  @name Friends

	friend class X3DParticleEmitterNode;

	///  @name Event handlers

	void
	set_live ();

	void
	set_shader ();

	void
	set_enabled ();

	void
	set_createParticles ();

	void
	set_geometryType ();

	void
	set_maxParticles ();

	void
	set_particleLifetime ();

	void
	set_lifetimeVariation ();

	void
	set_emitter ();

	void
	set_physics ();

	void
	set_boundedPhysics ();

	void
	set_colorRamp ();

	void
	set_color ();

	void
	set_texCoordRamp ();

	void
	set_texCoord ();

	///  @name Operations

	bool
	intersects (const Box3d & box,
	            const ClipPlaneContainerArray & clipPlanes,
	            const Matrix4d & modelViewMatrix);

	void
	animateParticles ();

	void
	updateGeometry (const Matrix4d & matrix, const bool state);

	void
	updatePoint ();

	void
	updateLine ();

	void
	updateQuad (const Matrix4d & modelViewMatrix, const bool state);

	///  @name Members

	ParticleSystem* const particleSystem;

	GLenum                     primitiveMode;
	GLuint                     idBufferId;
	GLuint                     positionBufferId;
	GLuint                     elapsedTimeBufferId;
	GLuint                     lifeBufferId;
	GLuint                     colorBufferId;
	std::vector <GLuint>       texCoordBufferIds;
	GLuint                     normalBufferId;
	GLuint                     vertexBufferId;
	std::vector <Vector3f>     positionArray;
	std::vector <float>        elapsedTimeArray;
	std::vector <int32_t>      lifeArray;
	std::vector <Color4f>      colorArray;
	std::vector <Vector4f>     texCoordArray;
	std::vector <Vector3f>     normalArray;
	std::vector <Vector3d>     vertexArray;
	size_t                     texCoordCount;
	size_t                     vertexCount;
	size_t                     shaderGeometryType;
	std::vector <SoftParticle> particles;
	bool                       colorMaterial;
	bool                       texCoordAnim;
	MFVec3f                    velocities;
	MFFloat                    speeds;
	MFFloat                    turbulences;
	size_t                     numForces;

	ParticleSystem::GeometryType              geometryType;
	bool                                      createParticles;
	time_type                                 particleLifetime;
	time_type                                 lifetimeVariation;
	size_t                                    maxParticles;
	size_t                                    numParticles;
	time_type                                 creationTime;
	time_type                                 pauseTime;
	time_type                                 deltaTime;
	std::vector <Color4f>                     colorRamp;
	std::vector <Vector4f>                    texCoordRamp;
	std::unique_ptr <BVH <float>>             boundedVolume;
	X3DPtr <X3DParticleEmitterNode>           emitterNode;
	X3DPtrArray <X3DParticlePhysicsModelNode> physicsModelNodes;
	X3DPtrArray <BoundedPhysicsModel>         boundedPhysicsModelNodes;
	X3DPtr <X3DColorNode>                     colorRampNode;
	X3DPtr <X3DTextureCoordinateNode>         texCoordRampNode;
	X3DPtr <ComposedShader>                   shaderNode;

};

} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_COMPONENTS_PARTICLE_SYSTEMS_PARTICLE_SYSTEM_H__
#define __TITANIA_X3D_COMPONENTS_PARTICLE_SYSTEMS_PARTICLE_SYSTEM_H__

#include "../Shape/X3DShapeNode.h"

#include <array>

namespace titania {
namespace X3D {

class SoftSystem;

class ParticleSystem :
	virtual public X3DShapeNode
{
public:

	///  @name Construction

	ParticleSystem (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Fields

	virtual
	SFBool &
	enabled ()
	{ return *fields .enabled; }

	virtual
	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	virtual
	SFString &
	geometryType ()
	{ return *fields .geometryType; }

	virtual
	const SFString &
	geometryType () const
	{ return *fields .geometryType; }

	virtual
	SFBool &
	createParticles ()
	{ return *fields .createParticles; }

	virtual
	const SFBool &
	createParticles () const
	{ return *fields .createParticles; }

	virtual
	SFInt32 &
	maxParticles ()
	{ return *fields .maxParticles; }

	virtual
	const SFInt32 &
	maxParticles () const
	{ return *fields .maxParticles; }

	virtual
	SFFloat &
	particleLifetime ()
	{ return *fields .particleLifetime; }

	virtual
	const SFFloat &
	particleLifetime () const
	{ return *fields .particleLifetime; }

	virtual
	SFFloat &
	lifetimeVariation ()
	{ return *fields .lifetimeVariation; }

	virtual
	const SFFloat &
	lifetimeVariation () const
	{ return *fields .lifetimeVariation; }

	virtual
	SFVec2f &
	particleSize ()
	{ return *fields .particleSize; }

	virtual
	const SFVec2f &
	particleSize () const
	{ return *fields .particleSize; }

	virtual
	MFFloat &
	colorKey ()
	{ return *fields .colorKey; }

	virtual
	const MFFloat &
	colorKey () const
	{ return *fields .colorKey; }

	virtual
	MFFloat &
	texCoordKey ()
	{ return *fields .texCoordKey; }

	virtual
	const MFFloat &
	texCoordKey () const
	{ return *fields .texCoordKey; }

	virtual
	SFBool &
	isActive ()
	{ return *fields .isActive; }

	virtual
	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	virtual
	SFNode &
	emitter ()
	{ return *fields .emitter; }

	virtual
	const SFNode &
	emitter () const
	{ return *fields .emitter; }

	virtual
	SFNode &
	colorRamp ()
	{ return *fields .colorRamp; }

	virtual
	const SFNode &
	colorRamp () const
	{ return *fields .colorRamp; }

	virtual
	SFNode &
	texCoordRamp ()
	{ return *fields .texCoordRamp; }

	virtual
	const SFNode &
	texCoordRamp () const
	{ return *fields .texCoordRamp; }

	virtual
	MFNode &
	physics ()
	{ return *fields .physics; }

	virtual
	const MFNode &
	physics () const
	{ return *fields .physics; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override;

	virtual
	bool
	isTransparent () const override;

	virtual
	X3D::GeometryType
	getGeometryType () const override;

	///  @name Operations

	virtual
	bool
	intersects (const Box3d & box,
	            const ClipPlaneContainerArray & clipPlanes,
	            const Matrix4d & modelViewMatrix) override;

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	virtual
	void
	depth (const X3DShapeContainer* const context) override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~ParticleSystem () override;


protected:

	///  @name Constuction

	virtual
	void
	initialize () override;


private:

	///  @name Friends

	friend class SoftSystem;

	///  @name Member types

	class Particle;
	class Vertex;
	class OddEvenMergeSort;

	enum class GeometryType :
		uint8_t
	{
		POINT,
		LINE,
		TRIANGLE,
		QUAD,
		SPRITE,
		GEOMETRY,
	};

	///  @name Member access

	bool
	isSoftSystem () const;

	///  @name Event handlers

	void
	set_live ();

	void
	set_enabled ();

	void
	set_geometryType ();

	void
	set_vertices ();

	void
	set_shader ();

	void
	set_createParticles ();

	void
	set_colorKey ();

	void
	set_texCoordKey ();

	void
	set_emitter ();

	void
	set_physics ();

	void
	set_boundedPhysicsModel ();

	void
	set_colorRamp ();

	void
	set_color ();

	void
	set_texCoordRamp ();

	void
	set_texCoord ();

	void
	set_geometry ();

	void
	set_particle_buffers ();

	void
	set_vertex_buffer ();

	void
	set_transform_shader ();

	void
	set_transform_shader_texture_buffers ();

	void
	swap_particle_buffers ();

	void
	set_particle_map ();

	void
	set_geometry_shader ();

	void
	set_geometry_shader_texture_buffers ();

	///  @name Operations

	void
	animateParticles ();

	void
	updateParticles ();

	void
	updateGeometry (const Matrix4d & modelViewMatrix);

	virtual
	void
	draw (ShapeContainer* const context) final override;

	///  throws std::domain_error
	Matrix3d
	getScreenAlignedRotation (const Matrix4d & modelViewMatrix) const;

	void
	enableTexCoord (X3DBrowser* const browser) const;

	void
	disableTexCoord (X3DBrowser* const browser) const;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const enabled;
		SFString* const geometryType;
		SFBool* const createParticles;
		SFInt32* const maxParticles;
		SFFloat* const particleLifetime;
		SFFloat* const lifetimeVariation;
		SFVec2f* const particleSize;
		MFFloat* const colorKey;
		MFFloat* const texCoordKey;
		SFBool* const isActive;
		SFNode* const emitter;
		SFNode* const colorRamp;
		SFNode* const texCoordRamp;
		MFNode* const physics;
	};

	Fields fields;

	GeometryType                              geometryTypeId;
	X3D::GeometryType                         shaderGeometryType;
	GLenum                                    glGeometryType;
	size_t                                    numVertices;
	int32_t                                   numParticles;
	time_type                                 creationTime;
	time_type                                 pauseTime;
	size_t                                    readBuffer;
	size_t                                    writeBuffer;
	std::array <GLuint, 2>                    particleFeedbackId;
	std::array <GLuint, 2>                    particleBufferId;
	GLuint                                    particleMapId;
	GLuint                                    vertexFeedbackId;
	GLuint                                    vertexBufferId;
	GLuint                                    geometryBufferId;
	std::array <GLuint, 2>                    colorRampMapId;
	std::array <GLuint, 2>                    colorRampBufferId;
	std::array <GLuint, 2>                    texCoordRampMapId;
	std::array <GLuint, 2>                    texCoordRampBufferId;
	X3DPtr <ComposedShader>                   transformShader;
	X3DPtr <ComposedShader>                   geometryShader;
	X3DPtr <X3DParticleEmitterNode>           emitterNode;
	X3DPtr <X3DColorNode>                     colorRampNode;
	X3DPtr <X3DTextureCoordinateNode>         texCoordRampNode;
	X3DPtr <X3DGeometryNode>                  geometryNode;
	X3DPtr <ComposedShader>                   shaderNode;
	size_t                                    numColors;
	size_t                                    numTexCoord;
	X3DPtrArray <X3DParticlePhysicsModelNode> physicsModelNodes;
	X3DPtrArray <BoundedPhysicsModel>         boundedPhysicsModelNodes;
	GLuint                                    boundedNormalMapId;
	GLuint                                    boundedNormalBufferId;
	GLuint                                    boundedSurfaceMapId;
	GLuint                                    boundedSurfaceBufferId;
	GLuint                                    boundedVolumeMapId;
	GLuint                                    boundedVolumeBufferId;

	std::unique_ptr <OddEvenMergeSort> sortAlgorithm;
	std::unique_ptr <SoftSystem>       softSystem;

};

} // X3D
} // titania

#endif

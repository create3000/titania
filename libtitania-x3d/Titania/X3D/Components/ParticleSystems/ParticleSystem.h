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

#ifndef __TITANIA_X3D_COMPONENTS_PARTICLE_SYSTEMS_PARTICLE_SYSTEM_H__
#define __TITANIA_X3D_COMPONENTS_PARTICLE_SYSTEMS_PARTICLE_SYSTEM_H__

#include "../ParticleSystems/X3DParticleEmitterNode.h"
#include "../Rendering/X3DColorNode.h"
#include "../Shape/X3DShapeNode.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

#include <array>

namespace titania {
namespace X3D {

class ParticleSystem :
	public X3DShapeNode
{
public:

	///  @name Construction

	ParticleSystem (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFBool &
	enabled ()
	{ return *fields .enabled; }

	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	SFString &
	geometryType ()
	{ return *fields .geometryType; }

	const SFString &
	geometryType () const
	{ return *fields .geometryType; }

	SFBool &
	createParticles ()
	{ return *fields .createParticles; }

	const SFBool &
	createParticles () const
	{ return *fields .createParticles; }

	SFInt32 &
	maxParticles ()
	{ return *fields .maxParticles; }

	const SFInt32 &
	maxParticles () const
	{ return *fields .maxParticles; }

	SFFloat &
	particleLifetime ()
	{ return *fields .particleLifetime; }

	const SFFloat &
	particleLifetime () const
	{ return *fields .particleLifetime; }

	SFFloat &
	lifetimeVariation ()
	{ return *fields .lifetimeVariation; }

	const SFFloat &
	lifetimeVariation () const
	{ return *fields .lifetimeVariation; }

	SFVec2f &
	particleSize ()
	{ return *fields .particleSize; }

	const SFVec2f &
	particleSize () const
	{ return *fields .particleSize; }

	MFFloat &
	colorKey ()
	{ return *fields .colorKey; }

	const MFFloat &
	colorKey () const
	{ return *fields .colorKey; }

	MFFloat &
	texCoordKey ()
	{ return *fields .texCoordKey; }

	const MFFloat &
	texCoordKey () const
	{ return *fields .texCoordKey; }

	SFBool &
	isActive ()
	{ return *fields .isActive; }

	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	SFNode &
	emitter ()
	{ return *fields .emitter; }

	const SFNode &
	emitter () const
	{ return *fields .emitter; }

	MFNode &
	physics ()
	{ return *fields .physics; }

	const MFNode &
	physics () const
	{ return *fields .physics; }

	SFNode &
	colorRamp ()
	{ return *fields .colorRamp; }

	const SFNode &
	colorRamp () const
	{ return *fields .colorRamp; }

	SFNode &
	texCoordRamp ()
	{ return *fields .texCoordRamp; }

	const SFNode &
	texCoordRamp () const
	{ return *fields .texCoordRamp; }

	///  @name Tests

	virtual
	bool
	isTransparent () const final override;

	///  @name Member access

	virtual
	Box3f
	getBBox () final override;

	///  @name Operations

	virtual
	bool
	intersect (const Sphere3f &, const Matrix4f &, const CollectableObjectArray &) final override;

	virtual
	void
	traverse (const TraverseType) final override;

	virtual
	void
	drawCollision () final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	~ParticleSystem ();


private:

	class OddEvenMergeSort;

	enum class GeometryType
	{
		POINT,
		LINE,
		TRIANGLE,
		QUAD,
		SPRITE,
		GEOMETRY,
	};

	class Particle;
	class Vertex;

	///  @name Constuction

	virtual
	void
	initialize () final override;

	///  @name Tests

	virtual
	bool
	isLineGeometry () const final override;

	///  @name Event handlers

	void
	set_enabled ();

	void
	set_geometryType ();

	void
	set_colorKey ();

	void
	set_texCoordKey ();

	void
	set_emitter ();

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
	swap_particle_buffers ();

	void
	set_particle_map ();

	void
	set_geometry_shader ();

	///  @name Operations

	void
	prepareEvents ();

	void
	update ();

	virtual
	void
	drawGeometry () final override;

	Matrix3f
	getScreenAlignedRotation () const
	throw (std::domain_error);

	void
	enableTexCoord () const;

	void
	disableTexCoord () const;

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

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
		MFNode* const physics;
		SFNode* const colorRamp;
		SFNode* const texCoordRamp;
	};

	Fields fields;

	GeometryType                         geometryTypeId;
	GLenum                               glGeometryType;
	size_t                               numVertices;
	int32_t                              numParticles;
	time_type                            creationTime;
	size_t                               readBuffer;
	size_t                               writeBuffer;
	std::array <GLuint, 2>               particleFeedbackId;
	std::array <GLuint, 2>               particleBufferId;
	GLuint                               particleMapId;
	GLuint                               vertexFeedbackId;
	GLuint                               vertexBufferId;
	GLuint                               geometryBufferId;
	std::array <GLuint, 2>               colorRampMapId;
	std::array <GLuint, 2>               colorRampBufferId;
	std::array <GLuint, 2>               texCoordRampMapId;
	std::array <GLuint, 2>               texCoordRampBufferId;
	X3DSFNode <ComposedShader>           transformShader;
	X3DSFNode <ComposedShader>           geometryShader;
	X3DParticleEmitterNode*              emitterNode;
	X3DSFNode <X3DColorNode>             colorRampNode;
	X3DSFNode <X3DTextureCoordinateNode> texCoordRampNode;
	bool                                 haveColor;

	std::unique_ptr <OddEvenMergeSort> sortAlgorithm;

};

} // X3D
} // titania

#endif

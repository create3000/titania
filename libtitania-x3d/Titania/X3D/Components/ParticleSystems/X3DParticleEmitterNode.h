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

#ifndef __TITANIA_X3D_COMPONENTS_PARTICLE_SYSTEMS_X3DPARTICLE_EMITTER_NODE_H__
#define __TITANIA_X3D_COMPONENTS_PARTICLE_SYSTEMS_X3DPARTICLE_EMITTER_NODE_H__

#include "../Core/X3DNode.h"
#include "../Shaders/ComposedShader.h"
#include "../../Types/Geometry.h"

#include "../../Browser/ParticleSystems/SoftParticle.h"

namespace titania {
namespace X3D {

class ParticleSystem;
class SoftSystem;

template <class Type>
class BVH;

class X3DParticleEmitterNode :
	virtual public X3DNode
{
public:

	///  @name Fields

	SFFloat &
	speed ()
	{ return *fields .speed; }

	const SFFloat &
	speed () const
	{ return *fields .speed; }

	SFFloat &
	variation ()
	{ return *fields .variation; }

	const SFFloat &
	variation () const
	{ return *fields .variation; }

	SFFloat &
	mass ()
	{ return *fields .mass; }

	const SFFloat &
	mass () const
	{ return *fields .mass; }

	SFFloat &
	surfaceArea ()
	{ return *fields .surfaceArea; }

	const SFFloat &
	surfaceArea () const
	{ return *fields .surfaceArea; }

//	virtual
//	SFFloat &
//	elasticity ()
//	{ return *fields .elasticity; }
//
//	virtual
//	const SFFloat &
//	elasticity () const
//	{ return *fields .elasticity; }

	///  @name Operations

	virtual
	Box3d
	getBBox () = 0;

	virtual
	bool
	isExplosive ()
	{ return false; }

	virtual
	MFString
	getShaderUrl () const = 0;

	virtual
	void
	addShaderFields (const X3DPtr <ComposedShader> & shader) const;	

	virtual
	void
	setTextureBuffer (const X3DPtr <ComposedShader> & shader) const
	{ }

	virtual
	void
	setShaderFields (const X3DPtr <ComposedShader> & shader) const;

	virtual
	void
	resetShader ()
	{ }


protected:

	///  @name Friends

	friend class SoftSystem;

	///  @name Construction

	X3DParticleEmitterNode ();

	///  @name Operations for SoftSystem

	bool
	isSoftSystem () const;

	virtual
	Vector3f
	getRandomPosition () const = 0;

	virtual
	Vector3f
	getRandomVelocity () const = 0;

	time_type
	getRandomLifetime (const time_type particleLifetime, const time_type lifetimeVariation) const;
	
	float
	getRandomSpeed () const;

	Vector3f
	getSphericalRandomVelocity () const;
	
	Vector3f
	getRandomNormal () const;
	
	Vector3f
	getRandomNormalWithAngle (const float angle) const;
	
	Vector3f
	getRandomNormalWithDirectionAndAngle (const Vector3f & direction, const float angle) const;
	
	Vector3f
	getRandomSurfaceNormal () const;

	void
	animate (SoftSystem* const softSystem, const time_type deltaTime) const;


private:

	///  @name Member access

	void
	bounce (const std::unique_ptr <BVH <float>> & boundedVolume,
	        const Vector3f & fromPosition,
	        Vector3f & toPosition,
	        Vector3f & velocity) const;

	void
	getColors (std::vector <SoftParticle> & particles,
	           const MFFloat & colorKeys,
	           const std::vector <Color4f> & colorRamp,
	           const size_t numParticles) const;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFFloat* const speed;
		SFFloat* const variation;
		SFFloat* const mass;
		SFFloat* const surfaceArea;
//		SFFloat* const elasticity;
	};

	Fields fields;

};

} // X3D
} // titania

#endif

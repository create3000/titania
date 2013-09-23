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

#include "../Shape/X3DShapeNode.h"

namespace titania {
namespace X3D {

class ParticleSystem :
	public X3DShapeNode
{
public:

	ParticleSystem (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final
	{ return containerField; }

	///  @name Fields

	SFBool &
	createParticles ()
	{ return *fields .createParticles; }

	const SFBool &
	createParticles () const
	{ return *fields .createParticles; }

	SFBool &
	enabled ()
	{ return *fields .enabled; }

	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	SFFloat &
	lifetimeVariation ()
	{ return *fields .lifetimeVariation; }

	const SFFloat &
	lifetimeVariation () const
	{ return *fields .lifetimeVariation; }

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

	SFVec2f &
	particleSize ()
	{ return *fields .particleSize; }

	const SFVec2f &
	particleSize () const
	{ return *fields .particleSize; }

	SFBool &
	isActive ()
	{ return *fields .isActive; }

	const SFBool &
	isActive () const
	{ return *fields .isActive; }

	SFNode &
	colorRamp ()
	{ return *fields .colorRamp; }

	const SFNode &
	colorRamp () const
	{ return *fields .colorRamp; }

	MFFloat &
	colorKey ()
	{ return *fields .colorKey; }

	const MFFloat &
	colorKey () const
	{ return *fields .colorKey; }

	SFNode &
	emitter ()
	{ return *fields .emitter; }

	const SFNode &
	emitter () const
	{ return *fields .emitter; }

	SFString &
	geometryType ()
	{ return *fields .geometryType; }

	const SFString &
	geometryType () const
	{ return *fields .geometryType; }

	MFNode &
	physics ()
	{ return *fields .physics; }

	const MFNode &
	physics () const
	{ return *fields .physics; }

	SFNode &
	texCoordRamp ()
	{ return *fields .texCoordRamp; }

	const SFNode &
	texCoordRamp () const
	{ return *fields .texCoordRamp; }

	MFFloat &
	texCoordKey ()
	{ return *fields .texCoordKey; }

	const MFFloat &
	texCoordKey () const
	{ return *fields .texCoordKey; }


private:


	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const createParticles;
		SFBool* const enabled;
		SFFloat* const lifetimeVariation;
		SFInt32* const maxParticles;
		SFFloat* const particleLifetime;
		SFVec2f* const particleSize;
		SFBool* const isActive;
		SFNode* const colorRamp;
		MFFloat* const colorKey;
		SFNode* const emitter;
		SFString* const geometryType;
		MFNode* const physics;
		SFNode* const texCoordRamp;
		MFFloat* const texCoordKey;
	};

	Fields fields;

};

} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_TOOLS_PARTICLE_SYSTEMS_PARTICLE_SYSTEM_TOOL_H__
#define __TITANIA_X3D_TOOLS_PARTICLE_SYSTEMS_PARTICLE_SYSTEM_TOOL_H__

#include "../Shape/X3DShapeNodeTool.h"
#include "../ToolColors.h"

#include "../../Components/ParticleSystems/ParticleSystem.h"
#include "../../Components/Rendering/X3DGeometryNode.h"
#include "../../Components/Shape/Appearance.h"

namespace titania {
namespace X3D {

class ParticleSystemTool :
	virtual public ParticleSystem,
	public X3DShapeNodeTool
{
public:

	///  @name Construction

	ParticleSystemTool (X3DBaseNode* const node) :
		     X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		  ParticleSystem (node -> getExecutionContext ()),
		     X3DBaseTool (node),
		X3DShapeNodeTool (ToolColors::ORANGE)
	{
		addType (X3DConstants::ParticleSystemTool);
	}

	///  @name Fields

	virtual
	SFBool &
	enabled () final override
	{ return getNode <ParticleSystem> () -> enabled (); }

	virtual
	const SFBool &
	enabled () const final override
	{ return getNode <ParticleSystem> () -> enabled (); }

	virtual
	SFString &
	geometryType () final override
	{ return getNode <ParticleSystem> () -> geometryType (); }

	virtual
	const SFString &
	geometryType () const final override
	{ return getNode <ParticleSystem> () -> geometryType (); }

	virtual
	SFBool &
	createParticles () final override
	{ return getNode <ParticleSystem> () -> createParticles (); }

	virtual
	const SFBool &
	createParticles () const final override
	{ return getNode <ParticleSystem> () -> createParticles (); }

	virtual
	SFInt32 &
	maxParticles () final override
	{ return getNode <ParticleSystem> () -> maxParticles (); }

	virtual
	const SFInt32 &
	maxParticles () const final override
	{ return getNode <ParticleSystem> () -> maxParticles (); }

	virtual
	SFFloat &
	particleLifetime () final override
	{ return getNode <ParticleSystem> () -> particleLifetime (); }

	virtual
	const SFFloat &
	particleLifetime () const final override
	{ return getNode <ParticleSystem> () -> particleLifetime (); }

	virtual
	SFFloat &
	lifetimeVariation ()
	{ return getNode <ParticleSystem> () -> lifetimeVariation (); }

	virtual
	const SFFloat &
	lifetimeVariation () const final override
	{ return getNode <ParticleSystem> () -> lifetimeVariation (); }

	virtual
	SFVec2f &
	particleSize () final override
	{ return getNode <ParticleSystem> () -> particleSize (); }

	virtual
	const SFVec2f &
	particleSize () const final override
	{ return getNode <ParticleSystem> () -> particleSize (); }

	virtual
	MFFloat &
	colorKey () final override
	{ return getNode <ParticleSystem> () -> colorKey (); }

	virtual
	const MFFloat &
	colorKey () const final override
	{ return getNode <ParticleSystem> () -> colorKey (); }

	virtual
	MFFloat &
	texCoordKey () final override
	{ return getNode <ParticleSystem> () -> texCoordKey (); }

	virtual
	const MFFloat &
	texCoordKey () const final override
	{ return getNode <ParticleSystem> () -> texCoordKey (); }

	virtual
	SFBool &
	isActive () final override
	{ return getNode <ParticleSystem> () -> isActive (); }

	virtual
	const SFBool &
	isActive () const final override
	{ return getNode <ParticleSystem> () -> isActive (); }

	virtual
	SFNode &
	emitter () final override
	{ return getNode <ParticleSystem> () -> emitter (); }

	virtual
	const SFNode &
	emitter () const final override
	{ return getNode <ParticleSystem> () -> emitter (); }

	virtual
	SFNode &
	colorRamp () final override
	{ return getNode <ParticleSystem> () -> colorRamp (); }

	virtual
	const SFNode &
	colorRamp () const final override
	{ return getNode <ParticleSystem> () -> colorRamp (); }

	virtual
	SFNode &
	texCoordRamp () final override
	{ return getNode <ParticleSystem> () -> texCoordRamp (); }

	virtual
	const SFNode &
	texCoordRamp () const final override
	{ return getNode <ParticleSystem> () -> texCoordRamp (); }

	virtual
	MFNode &
	physics () final override
	{ return getNode <ParticleSystem> () -> physics (); }

	virtual
	const MFNode &
	physics () const final override
	{ return getNode <ParticleSystem> () -> physics (); }

	///  @name Member access

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override
	{ X3DShapeNodeTool::setExecutionContext (executionContext); }

	virtual
	Box3d
	getBBox () const final override
	{ return X3DShapeNodeTool::getBBox (); }

	virtual
	size_t
	getGeometryType () const final override
	{ return getNode <ParticleSystem> () -> getGeometryType (); }

	///  @name Operations

	virtual
	bool
	intersects (const Box3d & box, const CollectableObjectArray & clipPlanes, const Matrix4d & modelViewMatrix) final override
	{ return X3DShapeNodeTool::intersects (box, clipPlanes, modelViewMatrix); }

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{ X3DShapeNodeTool::traverse (type, renderObject); }

	virtual
	void
	depth (const X3DShapeContainer* const context) final override
	{ X3DShapeNodeTool::depth (context); }

	virtual
	void
	display (ShapeContainer* const context) final override
	{ X3DShapeNodeTool::display (context); }

	///  @name Destruction

	virtual
	void
	dispose () final override
	{ X3DShapeNodeTool::dispose (); }


protected:

	///  @name Construction

	virtual
	void
	initialize () final override
	{ X3DShapeNodeTool::initialize (); }

};

} // X3D
} // titania

#endif

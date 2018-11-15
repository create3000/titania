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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_COLLISION_COLLECTION_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_COLLISION_COLLECTION_H__

#include "../Core/X3DChildNode.h"

namespace titania {
namespace X3D {

enum class AppliedParametersType
{
	BOUNCE,
	USER_FRICTION,
	FRICTION_COEFFICIENT_2,
	ERROR_REDUCTION,
	CONSTANT_FORCE,
	SPEED_1,
	SPEED_2,
	SLIP_1,
	SLIP_2
};

class CollisionCollection :
	virtual public X3DChildNode
{
public:

	///  @name Construction

	CollisionCollection (X3DExecutionContext* const executionContext);

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

	///  @name Fields

	SFBool &
	enabled ()
	{ return *fields .enabled; }

	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	MFString &
	appliedParameters ()
	{ return *fields .appliedParameters; }

	const MFString &
	appliedParameters () const
	{ return *fields .appliedParameters; }

	SFFloat &
	bounce ()
	{ return *fields .bounce; }

	const SFFloat &
	bounce () const
	{ return *fields .bounce; }

	SFFloat &
	minBounceSpeed ()
	{ return *fields .minBounceSpeed; }

	const SFFloat &
	minBounceSpeed () const
	{ return *fields .minBounceSpeed; }

	SFVec2f &
	frictionCoefficients ()
	{ return *fields .frictionCoefficients; }

	const SFVec2f &
	frictionCoefficients () const
	{ return *fields .frictionCoefficients; }

	SFVec2f &
	surfaceSpeed ()
	{ return *fields .surfaceSpeed; }

	const SFVec2f &
	surfaceSpeed () const
	{ return *fields .surfaceSpeed; }

	SFVec2f &
	slipFactors ()
	{ return *fields .slipFactors; }

	const SFVec2f &
	slipFactors () const
	{ return *fields .slipFactors; }

	SFFloat &
	softnessConstantForceMix ()
	{ return *fields .softnessConstantForceMix; }

	const SFFloat &
	softnessConstantForceMix () const
	{ return *fields .softnessConstantForceMix; }

	SFFloat &
	softnessErrorCorrection ()
	{ return *fields .softnessErrorCorrection; }

	const SFFloat &
	softnessErrorCorrection () const
	{ return *fields .softnessErrorCorrection; }

	MFNode &
	collidables ()
	{ return *fields .collidables; }

	const MFNode &
	collidables () const
	{ return *fields .collidables; }

	///  @name Member access

	const std::set <AppliedParametersType> &
	getAppliedParameters () const
	{ return appliedParametersTypes; }

	const X3DPtrArray <X3DNBodyCollidableNode> &
	getCollidables () const
	{ return collidableNodes; }


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_appliedParameters ();

	void
	set_collidables ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const enabled;
		MFString* const appliedParameters;
		SFFloat* const bounce;
		SFFloat* const minBounceSpeed;
		SFVec2f* const frictionCoefficients;
		SFVec2f* const surfaceSpeed;
		SFVec2f* const slipFactors;
		SFFloat* const softnessConstantForceMix;
		SFFloat* const softnessErrorCorrection;
		MFNode* const collidables;
	};

	Fields fields;

	X3DPtrArray <X3DNBodyCollidableNode> collidableNodes;
	std::set <AppliedParametersType>     appliedParametersTypes;

};

} // X3D
} // titania

#endif

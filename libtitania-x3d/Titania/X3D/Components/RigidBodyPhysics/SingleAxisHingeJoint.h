/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_SINGLE_AXIS_HINGE_JOINT_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_SINGLE_AXIS_HINGE_JOINT_H__

#include "../RigidBodyPhysics/X3DRigidJointNode.h"

namespace titania {
namespace X3D {

class SingleAxisHingeJoint :
	public X3DRigidJointNode
{
public:

	SingleAxisHingeJoint (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	SFVec3f &
	anchorPoint ()
	{ return *fields .anchorPoint; }

	const SFVec3f &
	anchorPoint () const
	{ return *fields .anchorPoint; }

	SFVec3f &
	axis ()
	{ return *fields .axis; }

	const SFVec3f &
	axis () const
	{ return *fields .axis; }

	SFFloat &
	maxAngle ()
	{ return *fields .maxAngle; }

	const SFFloat &
	maxAngle () const
	{ return *fields .maxAngle; }

	SFFloat &
	minAngle ()
	{ return *fields .minAngle; }

	const SFFloat &
	minAngle () const
	{ return *fields .minAngle; }

	SFFloat &
	stopBounce ()
	{ return *fields .stopBounce; }

	const SFFloat &
	stopBounce () const
	{ return *fields .stopBounce; }

	SFFloat &
	stopErrorCorrection ()
	{ return *fields .stopErrorCorrection; }

	const SFFloat &
	stopErrorCorrection () const
	{ return *fields .stopErrorCorrection; }

	SFFloat &
	angle ()
	{ return *fields .angle; }

	const SFFloat &
	angle () const
	{ return *fields .angle; }

	SFFloat &
	angleRate ()
	{ return *fields .angleRate; }

	const SFFloat &
	angleRate () const
	{ return *fields .angleRate; }

	SFVec3f &
	body1AnchorPoint ()
	{ return *fields .body1AnchorPoint; }

	const SFVec3f &
	body1AnchorPoint () const
	{ return *fields .body1AnchorPoint; }

	SFVec3f &
	body2AnchorPoint ()
	{ return *fields .body2AnchorPoint; }

	const SFVec3f &
	body2AnchorPoint () const
	{ return *fields .body2AnchorPoint; }


private:

	struct Fields
	{
		Fields ();

		SFVec3f* const anchorPoint;
		SFVec3f* const axis;
		SFFloat* const maxAngle;
		SFFloat* const minAngle;
		SFFloat* const stopBounce;
		SFFloat* const stopErrorCorrection;
		SFFloat* const angle;
		SFFloat* const angleRate;
		SFVec3f* const body1AnchorPoint;
		SFVec3f* const body2AnchorPoint;
	};

	Fields fields;

};

} // X3D
} // titania

#endif

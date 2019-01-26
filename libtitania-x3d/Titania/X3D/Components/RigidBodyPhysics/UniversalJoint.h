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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_UNIVERSAL_JOINT_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_UNIVERSAL_JOINT_H__

#include "../RigidBodyPhysics/X3DRigidJointNode.h"

namespace titania {
namespace X3D {

class UniversalJoint :
	public X3DRigidJointNode
{
public:

	///  @name Construction

	UniversalJoint (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
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

	SFVec3f &
	anchorPoint ()
	{ return *fields .anchorPoint; }

	const SFVec3f &
	anchorPoint () const
	{ return *fields .anchorPoint; }

	SFVec3f &
	axis1 ()
	{ return *fields .axis1; }

	const SFVec3f &
	axis1 () const
	{ return *fields .axis1; }

	SFVec3f &
	axis2 ()
	{ return *fields .axis2; }

	const SFVec3f &
	axis2 () const
	{ return *fields .axis2; }

	SFFloat &
	stop1Bounce ()
	{ return *fields .stop1Bounce; }

	const SFFloat &
	stop1Bounce () const
	{ return *fields .stop1Bounce; }

	SFFloat &
	stop2Bounce ()
	{ return *fields .stop2Bounce; }

	const SFFloat &
	stop2Bounce () const
	{ return *fields .stop2Bounce; }

	SFFloat &
	stop1ErrorCorrection ()
	{ return *fields .stop1ErrorCorrection; }

	const SFFloat &
	stop1ErrorCorrection () const
	{ return *fields .stop1ErrorCorrection; }

	SFFloat &
	stop2ErrorCorrection ()
	{ return *fields .stop2ErrorCorrection; }

	const SFFloat &
	stop2ErrorCorrection () const
	{ return *fields .stop2ErrorCorrection; }

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

	SFVec3f &
	body1Axis ()
	{ return *fields .body1Axis; }

	const SFVec3f &
	body1Axis () const
	{ return *fields .body1Axis; }

	SFVec3f &
	body2Axis ()
	{ return *fields .body2Axis; }

	const SFVec3f &
	body2Axis () const
	{ return *fields .body2Axis; }


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Joint handling

	virtual
	void
	addJoint () final override;

	virtual
	void
	removeJoint () final override;

	virtual
	void
	update1 () final override;

	virtual
	void
	update2 () final override;


private:

	///  @name Member types

	enum class OutputType
	{
		body1AnchorPoint,
		body2AnchorPoint,
		body1Axis,
		body2Axis,
	};

	///  @name Event handlers

	void
	set_forceOutput ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFVec3f* const anchorPoint;
		SFVec3f* const axis1;
		SFVec3f* const axis2;
		SFFloat* const stop1Bounce;
		SFFloat* const stop2Bounce;
		SFFloat* const stop1ErrorCorrection;
		SFFloat* const stop2ErrorCorrection;
		SFVec3f* const body1AnchorPoint;
		SFVec3f* const body2AnchorPoint;
		SFVec3f* const body1Axis;
		SFVec3f* const body2Axis;
	};

	Fields fields;

	///  @name Members

	std::array <bool, 4>                    outputs;
	std::shared_ptr <btUniversalConstraint> joint;

};

} // X3D
} // titania

#endif

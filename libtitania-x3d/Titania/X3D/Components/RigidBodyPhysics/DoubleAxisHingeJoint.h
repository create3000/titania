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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_DOUBLE_AXIS_HINGE_JOINT_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_DOUBLE_AXIS_HINGE_JOINT_H__

#include "../RigidBodyPhysics/X3DRigidJointNode.h"

namespace titania {
namespace X3D {

class DoubleAxisHingeJoint :
	public X3DRigidJointNode
{
public:

	///  @name Construction

	DoubleAxisHingeJoint (X3DExecutionContext* const executionContext);

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
	minAngle1 ()
	{ return *fields .minAngle1; }

	const SFFloat &
	minAngle1 () const
	{ return *fields .minAngle1; }

	SFFloat &
	maxAngle1 ()
	{ return *fields .maxAngle1; }

	const SFFloat &
	maxAngle1 () const
	{ return *fields .maxAngle1; }

	SFFloat &
	desiredAngularVelocity1 ()
	{ return *fields .desiredAngularVelocity1; }

	const SFFloat &
	desiredAngularVelocity1 () const
	{ return *fields .desiredAngularVelocity1; }

	SFFloat &
	desiredAngularVelocity2 ()
	{ return *fields .desiredAngularVelocity2; }

	const SFFloat &
	desiredAngularVelocity2 () const
	{ return *fields .desiredAngularVelocity2; }

	SFFloat &
	maxTorque1 ()
	{ return *fields .maxTorque1; }

	const SFFloat &
	maxTorque1 () const
	{ return *fields .maxTorque1; }

	SFFloat &
	maxTorque2 ()
	{ return *fields .maxTorque2; }

	const SFFloat &
	maxTorque2 () const
	{ return *fields .maxTorque2; }

	SFFloat &
	stopBounce1 ()
	{ return *fields .stopBounce1; }

	const SFFloat &
	stopBounce1 () const
	{ return *fields .stopBounce1; }

	SFFloat &
	stopConstantForceMix1 ()
	{ return *fields .stopConstantForceMix1; }

	const SFFloat &
	stopConstantForceMix1 () const
	{ return *fields .stopConstantForceMix1; }

	SFFloat &
	stopErrorCorrection1 ()
	{ return *fields .stopErrorCorrection1; }

	const SFFloat &
	stopErrorCorrection1 () const
	{ return *fields .stopErrorCorrection1; }

	SFFloat &
	suspensionForce ()
	{ return *fields .suspensionForce; }

	const SFFloat &
	suspensionForce () const
	{ return *fields .suspensionForce; }

	SFFloat &
	suspensionErrorCorrection ()
	{ return *fields .suspensionErrorCorrection; }

	const SFFloat &
	suspensionErrorCorrection () const
	{ return *fields .suspensionErrorCorrection; }

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
	body2Axis ()
	{ return *fields .body2Axis; }

	const SFVec3f &
	body2Axis () const
	{ return *fields .body2Axis; }

	SFFloat &
	hinge1Angle ()
	{ return *fields .hinge1Angle; }

	const SFFloat &
	hinge1Angle () const
	{ return *fields .hinge1Angle; }

	SFFloat &
	hinge2AngleRate ()
	{ return *fields .hinge2AngleRate; }

	const SFFloat &
	hinge2AngleRate () const
	{ return *fields .hinge2AngleRate; }

	SFFloat &
	hinge1AngleRate ()
	{ return *fields .hinge1AngleRate; }

	const SFFloat &
	hinge1AngleRate () const
	{ return *fields .hinge1AngleRate; }

	SFFloat &
	hinge2Angle ()
	{ return *fields .hinge2Angle; }

	const SFFloat &
	hinge2Angle () const
	{ return *fields .hinge2Angle; }

	SFVec3f &
	body1Axis ()
	{ return *fields .body1Axis; }

	const SFVec3f &
	body1Axis () const
	{ return *fields .body1Axis; }


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
		hinge1Angle,
		hinge2Angle,
		hinge1AngleRate,
		hinge2AngleRate
	};

	///  @name Event handlers

	void
	set_forceOutput ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFVec3f* const anchorPoint;
		SFVec3f* const axis1;
		SFVec3f* const axis2;
		SFFloat* const minAngle1;
		SFFloat* const maxAngle1;
		SFFloat* const desiredAngularVelocity1;
		SFFloat* const desiredAngularVelocity2;
		SFFloat* const maxTorque1;
		SFFloat* const maxTorque2;
		SFFloat* const stopBounce1;
		SFFloat* const stopConstantForceMix1;
		SFFloat* const stopErrorCorrection1;
		SFFloat* const suspensionForce;
		SFFloat* const suspensionErrorCorrection;
		SFVec3f* const body1AnchorPoint;
		SFVec3f* const body2AnchorPoint;
		SFVec3f* const body1Axis;
		SFVec3f* const body2Axis;
		SFFloat* const hinge1Angle;
		SFFloat* const hinge2Angle;
		SFFloat* const hinge1AngleRate;
		SFFloat* const hinge2AngleRate;
	};

	Fields fields;

	///  @name Members

	std::array <bool, 8>                outputs;
	std::shared_ptr <btHingeConstraint> joint;
	Vector3f                            localAnchorPoint1;
	Vector3f                            localAnchorPoint2;
	Vector3f                            localAxis1;
	Vector3f                            localAxis2;

};

} // X3D
} // titania

#endif

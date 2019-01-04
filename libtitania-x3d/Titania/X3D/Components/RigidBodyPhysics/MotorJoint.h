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

#ifndef __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_MOTOR_JOINT_H__
#define __TITANIA_X3D_COMPONENTS_RIGID_BODY_PHYSICS_MOTOR_JOINT_H__

#include "../RigidBodyPhysics/X3DRigidJointNode.h"

namespace titania {
namespace X3D {

class MotorJoint :
	public X3DRigidJointNode
{
public:

	///  @name Construction

	MotorJoint (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

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
	autoCalc ()
	{ return *fields .autoCalc; }

	const SFBool &
	autoCalc () const
	{ return *fields .autoCalc; }

	SFInt32 &
	enabledAxes ()
	{ return *fields .enabledAxes; }

	const SFInt32 &
	enabledAxes () const
	{ return *fields .enabledAxes; }

	SFVec3f &
	motor1Axis ()
	{ return *fields .motor1Axis; }

	const SFVec3f &
	motor1Axis () const
	{ return *fields .motor1Axis; }

	SFVec3f &
	motor2Axis ()
	{ return *fields .motor2Axis; }

	const SFVec3f &
	motor2Axis () const
	{ return *fields .motor2Axis; }

	SFVec3f &
	motor3Axis ()
	{ return *fields .motor3Axis; }

	const SFVec3f &
	motor3Axis () const
	{ return *fields .motor3Axis; }

	SFFloat &
	axis1Angle ()
	{ return *fields .axis1Angle; }

	const SFFloat &
	axis1Angle () const
	{ return *fields .axis1Angle; }

	SFFloat &
	axis2Angle ()
	{ return *fields .axis2Angle; }

	const SFFloat &
	axis2Angle () const
	{ return *fields .axis2Angle; }

	SFFloat &
	axis3Angle ()
	{ return *fields .axis3Angle; }

	const SFFloat &
	axis3Angle () const
	{ return *fields .axis3Angle; }

	SFFloat &
	axis1Torque ()
	{ return *fields .axis1Torque; }

	const SFFloat &
	axis1Torque () const
	{ return *fields .axis1Torque; }

	SFFloat &
	axis2Torque ()
	{ return *fields .axis2Torque; }

	const SFFloat &
	axis2Torque () const
	{ return *fields .axis2Torque; }

	SFFloat &
	axis3Torque ()
	{ return *fields .axis3Torque; }

	const SFFloat &
	axis3Torque () const
	{ return *fields .axis3Torque; }

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
	stop3Bounce ()
	{ return *fields .stop3Bounce; }

	const SFFloat &
	stop3Bounce () const
	{ return *fields .stop3Bounce; }

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

	SFFloat &
	stop3ErrorCorrection ()
	{ return *fields .stop3ErrorCorrection; }

	const SFFloat &
	stop3ErrorCorrection () const
	{ return *fields .stop3ErrorCorrection; }

	SFFloat &
	motor1Angle ()
	{ return *fields .motor1Angle; }

	const SFFloat &
	motor1Angle () const
	{ return *fields .motor1Angle; }

	SFFloat &
	motor2Angle ()
	{ return *fields .motor2Angle; }

	const SFFloat &
	motor2Angle () const
	{ return *fields .motor2Angle; }

	SFFloat &
	motor3Angle ()
	{ return *fields .motor3Angle; }

	const SFFloat &
	motor3Angle () const
	{ return *fields .motor3Angle; }

	SFFloat &
	motor1AngleRate ()
	{ return *fields .motor1AngleRate; }

	const SFFloat &
	motor1AngleRate () const
	{ return *fields .motor1AngleRate; }

	SFFloat &
	motor2AngleRate ()
	{ return *fields .motor2AngleRate; }

	const SFFloat &
	motor2AngleRate () const
	{ return *fields .motor2AngleRate; }

	SFFloat &
	motor3AngleRate ()
	{ return *fields .motor3AngleRate; }

	const SFFloat &
	motor3AngleRate () const
	{ return *fields .motor3AngleRate; }


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
		motor1Angle,
		motor2Angle,
		motor3Angle,
		motor1AngleRate,
		motor2AngleRate,
		motor3AngleRate
	};

	///  @name Event handlers

	void
	set_forceOutput ();

	void
	set_axes ();

	void
	set_axis1Angle ();
	
	void
	set_axis2Angle ();
	
	void
	set_axis3Angle ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFBool* const autoCalc;
		SFInt32* const enabledAxes;
		SFVec3f* const motor1Axis;
		SFVec3f* const motor2Axis;
		SFVec3f* const motor3Axis;
		SFFloat* const axis1Angle;
		SFFloat* const axis2Angle;
		SFFloat* const axis3Angle;
		SFFloat* const axis1Torque;
		SFFloat* const axis2Torque;
		SFFloat* const axis3Torque;
		SFFloat* const stop1Bounce;
		SFFloat* const stop2Bounce;
		SFFloat* const stop3Bounce;
		SFFloat* const stop1ErrorCorrection;
		SFFloat* const stop2ErrorCorrection;
		SFFloat* const stop3ErrorCorrection;
		SFFloat* const motor1Angle;
		SFFloat* const motor2Angle;
		SFFloat* const motor3Angle;
		SFFloat* const motor1AngleRate;
		SFFloat* const motor2AngleRate;
		SFFloat* const motor3AngleRate;
	};

	Fields fields;

	///  @name Members

	std::array <bool, 6>                      outputs;
	std::shared_ptr <btGeneric6DofConstraint> joint;

};

} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_COMPONENTS_POINTING_DEVICE_SENSOR_CYLINDER_SENSOR_H__
#define __TITANIA_X3D_COMPONENTS_POINTING_DEVICE_SENSOR_CYLINDER_SENSOR_H__

#include "../PointingDeviceSensor/X3DDragSensorNode.h"

namespace titania {
namespace X3D {

class CylinderSensor :
	public X3DDragSensorNode
{
public:

	CylinderSensor (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	SFRotation &
	axisRotation ()
	{ return *fields .axisRotation; }

	const SFRotation &
	axisRotation () const
	{ return *fields .axisRotation; }

	SFFloat &
	diskAngle ()
	{ return *fields .diskAngle; }

	const SFFloat &
	diskAngle () const
	{ return *fields .diskAngle; }

	SFFloat &
	minAngle ()
	{ return *fields .minAngle; }

	const SFFloat &
	minAngle () const
	{ return *fields .minAngle; }

	SFFloat &
	maxAngle ()
	{ return *fields .maxAngle; }

	const SFFloat &
	maxAngle () const
	{ return *fields .maxAngle; }

	SFFloat &
	offset ()
	{ return *fields .offset; }

	const SFFloat &
	offset () const
	{ return *fields .offset; }

	SFRotation &
	rotation_changed ()
	{ return *fields .rotation_changed; }

	const SFRotation &
	rotation_changed () const
	{ return *fields .rotation_changed; }

	virtual
	void
	set_active (const std::shared_ptr <Hit> &, bool) final;

	virtual
	void
	set_motion (const std::shared_ptr <Hit> &) final;


private:

	bool
	isBehind (const Line3f &, const Vector3f &) const;

	Vector3f
	getVector (const Line3f &, const Vector3f &) const;

	bool
	getTrackPoint (const Line3f &, Vector3f &, bool = false) const;

	float
	getAngle (const Rotation4f &) const;

	struct Fields
	{
		Fields ();

		SFRotation* const axisRotation;
		SFFloat* const diskAngle;
		SFFloat* const minAngle;
		SFFloat* const maxAngle;
		SFFloat* const offset;
		SFRotation* const rotation_changed;
	};

	Fields fields;

	Cylinder3f cylinder;
	bool       disk;
	Plane3f    yPlane;
	Plane3f    zPlane;
	bool       behind;
	Vector3f   fromVector;
	float      startOffset;
	float      angle;
	Matrix4f   inverseTransformationMatrix;

};

} // X3D
} // titania

#endif

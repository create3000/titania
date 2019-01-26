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

#ifndef __TITANIA_X3D_COMPONENTS_POINTING_DEVICE_SENSOR_CYLINDER_SENSOR_H__
#define __TITANIA_X3D_COMPONENTS_POINTING_DEVICE_SENSOR_CYLINDER_SENSOR_H__

#include "../PointingDeviceSensor/X3DDragSensorNode.h"

namespace titania {
namespace X3D {

class CylinderSensor :
	public X3DDragSensorNode
{
public:

	///  @name Construction

	CylinderSensor (X3DExecutionContext* const executionContext);

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


protected:

	///  @name Event handlers

	virtual
	void
	set_active (const bool active,
	            const HitPtr & hit,
	            const Matrix4d & modelViewMatrix,
	            const Matrix4d & projectionMatrix,
	            const Vector4i & viewport) final override;

	virtual
	void
	set_motion (const HitPtr & hit,
	            const Matrix4d & modelViewMatrix,
	            const Matrix4d & projectionMatrix,
	            const Vector4i & viewport) final override;


private:

	///  @name Operations

	bool
	isBehind (const Line3d &, const Vector3d &) const;

	Vector3d
	getTrackPoint (const Line3d &) const;

	double
	getAngle (const Rotation4d &) const;


	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

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

	Cylinder3d cylinder;
	bool       disk;
	Plane3d    yPlane;
	Plane3d    zPlane;
	Plane3d    sxPlane;
	Vector3d   szNormal;
	bool       behind;
	Vector3d   fromVector;
	Rotation4d startOffset;
	Matrix4d   inverseModelViewMatrix;

};

} // X3D
} // titania

#endif

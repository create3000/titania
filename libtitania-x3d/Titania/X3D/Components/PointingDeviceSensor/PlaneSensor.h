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

#ifndef __TITANIA_X3D_COMPONENTS_POINTING_DEVICE_SENSOR_PLANE_SENSOR_H__
#define __TITANIA_X3D_COMPONENTS_POINTING_DEVICE_SENSOR_PLANE_SENSOR_H__

#include "../PointingDeviceSensor/X3DDragSensorNode.h"

namespace titania {
namespace X3D {

class PlaneSensor :
	public X3DDragSensorNode
{
public:

	///  @name Construction

	PlaneSensor (X3DExecutionContext* const executionContext);

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

	SFVec3f &
	offset ()
	{ return *fields .offset; }

	const SFVec3f &
	offset () const
	{ return *fields .offset; }

	SFVec2f &
	minPosition ()
	{ return *fields .minPosition; }

	const SFVec2f &
	minPosition () const
	{ return *fields .minPosition; }

	SFVec2f &
	maxPosition ()
	{ return *fields .maxPosition; }

	const SFVec2f &
	maxPosition () const
	{ return *fields .maxPosition; }

	SFVec3f &
	translation_changed ()
	{ return *fields .translation_changed; }

	const SFVec3f &
	translation_changed () const
	{ return *fields .translation_changed; }


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

	///  throws std::domain_error
	std::pair <Vector3d, bool>
	getLineTrackPoint (const HitPtr &, const Line3d &);

	void
	trackStart (const Vector3d &);

	void
	track (const Vector3d &, const Vector3d &);

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFRotation* const axisRotation;
		SFVec3f* const offset;
		SFVec2f* const minPosition;
		SFVec2f* const maxPosition;
		SFVec3f* const translation_changed;
	};

	Fields fields;

	bool     planeSensor;
	Line3d   line;
	Plane3d  plane;
	Vector3d startPoint;
	Vector3d startOffset;
	Matrix4d modelViewMatrix;
	Matrix4d projectionMatrix;
	Vector4i viewport;
	Matrix4d inverseModelViewMatrix;

};

} // X3D
} // titania

#endif

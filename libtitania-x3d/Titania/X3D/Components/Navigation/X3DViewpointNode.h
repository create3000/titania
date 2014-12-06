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

#ifndef __TITANIA_X3D_COMPONENTS_NAVIGATION_X3DVIEWPOINT_NODE_H__
#define __TITANIA_X3D_COMPONENTS_NAVIGATION_X3DVIEWPOINT_NODE_H__

#include "../../Rendering/ViewVolume.h"
#include "../../Types/Geometry.h"
#include "../Core/X3DBindableNode.h"
#include "../Navigation/X3DViewpointObject.h"

namespace titania {
namespace X3D {

class EaseInEaseOut;
class PositionInterpolator;
class OrientationInterpolator;
class TimeSensor;

class X3DViewpointNode :
	public X3DBindableNode, public X3DViewpointObject
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Fields

	virtual
	SFRotation &
	orientation ()
	{ return *fields .orientation; }

	virtual
	const SFRotation &
	orientation () const
	{ return *fields .orientation; }

	virtual
	SFBool &
	jump ()
	{ return *fields .jump; }

	virtual
	const SFBool &
	jump () const
	{ return *fields .jump; }

	virtual
	SFVec3f &
	positionOffset ()
	{ return fields .positionOffset; }

	virtual
	const SFVec3f &
	positionOffset () const
	{ return fields .positionOffset; }

	virtual
	SFRotation &
	orientationOffset ()
	{ return fields .orientationOffset; }

	virtual
	const SFRotation &
	orientationOffset () const
	{ return fields .orientationOffset; }

	virtual
	SFVec3f &
	centerOfRotationOffset ()
	{ return fields .centerOfRotationOffset; }

	virtual
	const SFVec3f &
	centerOfRotationOffset () const
	{ return fields .centerOfRotationOffset; }

	virtual
	SFFloat &
	fieldOfViewScale ()
	{ return fields .fieldOfViewScale; }

	virtual
	const SFFloat &
	fieldOfViewScale () const
	{ return fields .fieldOfViewScale; }

	///  @name Member access

	virtual
	void
	setPosition (const Vector3f &) = 0;

	virtual
	Vector3f
	getPosition () const = 0;

	Vector3f
	getUserPosition () const;

	virtual
	void
	setOrientation (const Rotation4f & value)
	{ orientation () = value; }

	virtual
	Rotation4f
	getOrientation () const
	{ return orientation (); }

	Rotation4f
	getUserOrientation () const;

	virtual
	void
	setCenterOfRotation (const Vector3f &) = 0;

	virtual
	Vector3f
	getCenterOfRotation () const = 0;

	Vector3f
	getUserCenterOfRotation () const;

	virtual
	Vector3f
	getUpVector () const
	{ return Vector3f (0, 1, 0); }

	virtual
	float
	getMaxZFar () const
	{ return 1e5; }

	virtual
	float
	getSpeedFactor () const
	{ return 1; }

	virtual
	Matrix4d
	getProjectionMatrix (const double, const double, const Vector4i &) = 0;

	const Matrix4f &
	getCameraSpaceMatrix () const
	{ return cameraSpaceMatrix; }

	const Matrix4f &
	getInverseCameraSpaceMatrix () const
	{ return inverseCameraSpaceMatrix; }

	const Matrix4f &
	getParentMatrix () const
	{ return parentMatrix; }

	virtual
	Vector3d
	getScreenScale (const double, const Vector4i &) const = 0;

	///  @name Operations

	void
	isLockedToCamera (const bool);

	bool
	isLockedToCamera () const;

	void
	applyUserOffsets ();

	void
	resetUserOffsets ();

	void
	straighten (const bool = false);

	Rotation4f
	straightenHorizon (const Rotation4f &) const;

	Rotation4f
	straightenView (const Rotation4f &) const;

	void
	lookAt (Vector3f, const float = 1, const bool = false);

	void
	lookAt (Box3f, const float = 1, const bool = false);

	void
	transitionStart (X3DViewpointNode* const);

	void
	transitionStop ();

	///  Setup projection matrix for X3DBackgroundNode.
	virtual
	void
	background (const double, const double);

	///  Setup projection matrix.
	void
	reshape ();

	///  Setup custom projection matrix.
	void
	reshape (const double, const double);

	///  Setup modelview matrix.
	void
	transform ();

	virtual
	void
	traverse (const TraverseType) override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DViewpointNode ();


protected:

	///  @name Construction

	X3DViewpointNode ();

	virtual
	void
	initialize () override;

	///  @name Operations

	virtual
	void
	bindToLayer (X3DLayerNode* const) override;

	virtual
	void
	unbindFromLayer (X3DLayerNode* const) override;

	virtual
	void
	removeFromLayer (X3DLayerNode* const) override;


private:

	///  @name Protected fields

	SFVec3f &
	scaleOffset ()
	{ return fields .scaleOffset; }

	const SFVec3f &
	scaleOffset () const
	{ return fields .scaleOffset; }

	SFRotation &
	scaleOrientationOffset ()
	{ return fields .scaleOrientationOffset; }

	const SFRotation &
	scaleOrientationOffset () const
	{ return fields .scaleOrientationOffset; }

	///  @name Member access

	void
	setTransformationMatrix (Matrix4f);

	void
	setParentMatrix (const Matrix4f & value)
	{ parentMatrix = value; }

	///  @name Operations

	void
	getRelativeTransformation (X3DViewpointNode* const, Vector3f &, Rotation4f &, Vector3f &, Rotation4f &) const;

	void
	lookAt (const Vector3f &, const float, const float, const bool);

	virtual
	float
	getLookAtDistance (const Box3f &) const = 0;

	void
	set_isActive (const bool);

	void
	set_bind_ ();

	///  @name Members

	struct Fields
	{
		Fields ();

		SFRotation* const orientation;
		SFBool* const jump;
		SFVec3f positionOffset;
		SFRotation orientationOffset;
		SFVec3f scaleOffset;
		SFRotation scaleOrientationOffset;
		SFVec3f centerOfRotationOffset;
		SFFloat fieldOfViewScale;
	};

	Fields fields;

	Matrix4f parentMatrix;
	Matrix4f cameraSpaceMatrix;
	Matrix4f inverseCameraSpaceMatrix;

	X3DPtr <TimeSensor>              timeSensor;
	X3DPtr <EaseInEaseOut>           easeInEaseOut;
	X3DPtr <PositionInterpolator>    positionInterpolator;
	X3DPtr <OrientationInterpolator> orientationInterpolator;
	X3DPtr <PositionInterpolator>    scaleInterpolator;
	X3DPtr <OrientationInterpolator> scaleOrientationInterpolator;

};

} // X3D
} // titania

#endif

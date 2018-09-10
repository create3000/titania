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
	virtual public X3DBindableNode,
	virtual public X3DViewpointObject
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
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
	SFVec3d &
	positionOffset ()
	{ return fields .positionOffset; }

	virtual
	const SFVec3d &
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
	SFVec3d &
	centerOfRotationOffset ()
	{ return fields .centerOfRotationOffset; }

	virtual
	const SFVec3d &
	centerOfRotationOffset () const
	{ return fields .centerOfRotationOffset; }

	virtual
	SFDouble &
	fieldOfViewScale ()
	{ return fields .fieldOfViewScale; }

	virtual
	const SFDouble &
	fieldOfViewScale () const
	{ return fields .fieldOfViewScale; }

	///  @name Member access

	virtual
	void
	setPosition (const Vector3d & value) = 0;

	virtual
	Vector3d
	getPosition () const = 0;

	void
	setUserPosition (const Vector3d & value);

	Vector3d
	getUserPosition () const;

	virtual
	void
	setOrientation (const Rotation4d & value)
	{ orientation () = value; }

	virtual
	Rotation4d
	getOrientation () const
	{ return orientation () .getValue (); }

	void
	setUserOrientation (const Rotation4d & value);

	Rotation4d
	getUserOrientation () const;

	virtual
	void
	setCenterOfRotation (const Vector3d & value) = 0;

	virtual
	Vector3d
	getCenterOfRotation () const = 0;

	void
	setUserCenterOfRotation (const Vector3d & value);

	Vector3d
	getUserCenterOfRotation () const;

	virtual
	Vector3d
	getUpVector () const
	{ return Vector3d (0, 1, 0); }

	virtual
	double
	getMaxFarValue () const;

	virtual
	double
	getSpeedFactor () const
	{ return 1; }

	///  Setup projection matrix.
	Matrix4d
	getProjectionMatrix (X3DRenderObject* const renderObject) const;

	virtual
	Matrix4d
	getProjectionMatrix (const double nearValue, const double farValue, const Vector4i & viewport, const bool limit = false) const = 0;

	const Matrix4d &
	getCameraSpaceMatrix () const
	{ return cameraSpaceMatrix; }

	const Matrix4d &
	getInverseCameraSpaceMatrix () const
	{ return inverseCameraSpaceMatrix; }

	const Matrix4d &
	getModelMatrix () const
	{ return modelMatrix; }

	virtual
	Vector3d
	getScreenScale (const Vector3d & point, const Vector4i & viewport) const = 0;

	void
	setAnimate (const bool value)
	{ animate = value; }

	bool
	getAnimate () const
	{ return animate; }

	///  @name Operations

	virtual
	void
	isLockedToCamera (const bool);

	virtual
	const SFBool &
	isLockedToCamera () const
	{ return lockToCamera; }

	virtual
	void
	applyUserOffsets ();

	virtual
	void
	resetUserOffsets ();

	virtual
	void
	straighten (const bool horizon = false);

	virtual
	Rotation4d
	straightenHorizon (const Rotation4d & orientation) const;

	virtual
	Rotation4d
	straightenView (const Rotation4d & orientation) const;

	virtual
	void
	lookAt (Vector3d point, const double factor = 1, const bool straighten = false, const time_type cycleInterval = 0.2);

	virtual
	void
	lookAt (Box3d bbox, const double factor = 1, const bool straighten = false, const time_type cycleInterval = 0.2);

	virtual
	void
	transitionStart (X3DViewpointNode* const fromViewpoint);

	virtual
	void
	transitionStop ();

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DViewpointNode () override;


protected:

	///  @name Construction

	X3DViewpointNode ();

	virtual
	void
	initialize () override;

	///  @name Operations

	virtual
	void
	bindToLayer (X3DLayerNode* const layerNode) override;

	virtual
	void
	unbindFromLayer (X3DLayerNode* const layerNode) override;

	virtual
	void
	removeFromLayer (X3DLayerNode* const layerNode) override;


private:

	///  @name Protected fields

	SFVec3d &
	scaleOffset ()
	{ return fields .scaleOffset; }

	const SFVec3d &
	scaleOffset () const
	{ return fields .scaleOffset; }

	SFRotation &
	scaleOrientationOffset ()
	{ return fields .scaleOrientationOffset; }

	const SFRotation &
	scaleOrientationOffset () const
	{ return fields .scaleOrientationOffset; }

	void
	set_positionOffset ();

	void
	set_scaleOffset ();

	void
	set_fieldOfViewOffset ();

	///  @name Member access

	void
	setCameraSpaceMatrix (const Matrix4d & value);

	void
	setModelMatrix (const Matrix4d & value)
	{ modelMatrix = value; }

	///  @name Operations

	void
	getRelativeTransformation (X3DViewpointNode* const fromViewpoint,
	                           Vector3d & relativePosition,
	                           Rotation4d & relativeOrientation,
	                           Vector3d & relativeScale,
	                           Rotation4d & relativeScaleOrientation) const;

	void
	lookAt (const Vector3d & point, const std::pair <double, double> & distance, const double factor, const bool straighten, const time_type cycleInterval);

	virtual
	std::pair <double, double>
	getLookAtDistance (const Box3d & bbox) const = 0;

	void
	set_isActive (const bool active);

	void
	set_bind_ ();

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFRotation* const orientation;
		SFBool* const jump;
		SFVec3d positionOffset;
		SFRotation orientationOffset;
		SFVec3d scaleOffset;
		SFRotation scaleOrientationOffset;
		SFVec3d centerOfRotationOffset;
		SFDouble fieldOfViewScale;
	};

	Fields fields;

	///  @name Members

	Matrix4d modelMatrix;
	Matrix4d cameraSpaceMatrix;
	Matrix4d inverseCameraSpaceMatrix;

	X3DPtr <TimeSensor>              timeSensor;
	X3DPtr <EaseInEaseOut>           easeInEaseOut;
	X3DPtr <PositionInterpolator>    positionInterpolator;
	X3DPtr <OrientationInterpolator> orientationInterpolator;
	X3DPtr <PositionInterpolator>    scaleInterpolator;
	X3DPtr <OrientationInterpolator> scaleOrientationInterpolator;
	X3DPtr <ScalarInterpolator>      fieldOfViewInterpolator;
	
	SFBool lockToCamera;
	bool animate;

};

} // X3D
} // titania

#endif

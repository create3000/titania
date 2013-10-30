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

#include "../../Types/Geometry.h"
#include "../Core/X3DBindableNode.h"
#include "../Navigation/X3DViewpointObject.h"

#include "../Interpolation/EaseInEaseOut.h"
#include "../Interpolation/PositionInterpolator.h"
#include "../Interpolation/OrientationInterpolator.h"
#include "../Time/TimeSensor.h"

namespace titania {
namespace X3D {

class X3DViewpointNode :
	public X3DBindableNode, public X3DViewpointObject
{
public:

	///  @name Fields

	SFRotation &
	orientation ()
	{ return *fields .orientation; }

	const SFRotation &
	orientation () const
	{ return *fields .orientation; }

	SFVec3f &
	centerOfRotation ()
	{ return *fields .centerOfRotation; }

	const SFVec3f &
	centerOfRotation () const
	{ return *fields .centerOfRotation; }

	SFBool &
	jump ()
	{ return *fields .jump; }

	const SFBool &
	jump () const
	{ return *fields .jump; }

	SFVec3f &
	positionOffset ()
	{ return fields .positionOffset; }

	const SFVec3f &
	positionOffset () const
	{ return fields .positionOffset; }

	SFRotation &
	orientationOffset ()
	{ return fields .orientationOffset; }

	const SFRotation &
	orientationOffset () const
	{ return fields .orientationOffset; }

	SFVec3f &
	centerOfRotationOffset ()
	{ return fields .centerOfRotationOffset; }

	const SFVec3f &
	centerOfRotationOffset () const
	{ return fields .centerOfRotationOffset; }

	///  @name Member access

	virtual
	Vector3f
	getPosition () const = 0;

	Vector3f
	getUserPosition () const;

	Rotation4f
	getUserOrientation () const;

	Vector3f
	getUserCenterOfRotation () const;

	void
	setTransformationMatrix (const Matrix4f &);

	const Matrix4f &
	getTransformationMatrix () const
	{ return transformationMatrix; }

	const Matrix4f &
	getInverseTransformationMatrix () const
	{ return inverseTransformationMatrix; }

	const Matrix4f &
	getParentMatrix () const
	{ return parentMatrix; }

	virtual
	Vector3d
	getScreenScale (double, const Vector4i &) const = 0;

	///  @name Operations

	void
	resetUserOffsets ();

	void
	straighten ();

	virtual
	void
	lookAt (Box3f);

	void
	transitionStart (X3DViewpointNode*);

	void
	transitionStop ();

	void
	reshape ();

	virtual
	void
	reshape (const float, const float) = 0;

	void
	transform ();

	virtual
	void
	traverse (const TraverseType) override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DViewpointNode ();

	///  @name Member access

	void
	setParentMatrix (const Matrix4f & value)
	{ parentMatrix = value; }


private:

	///  @name Private fields

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

	///  @name Construction

	virtual
	void
	initialize () override;

	///  @name Operations

	void
	getRelativeTransformation (X3DViewpointNode*, Vector3f &, Rotation4f &, Vector3f &, Rotation4f &) const;

	virtual
	Vector3f
	getLookAtPositionOffset (Box3f) const = 0;

	virtual
	void
	bindToLayer (X3DLayerNode* const) override;

	virtual
	void
	unbindFromLayer (X3DLayerNode* const) override;

	void
	set_isActive (bool);

	void
	_set_bind ();

	void
	camera ();

	void
	collect ();

	///  @name Members

	struct Fields
	{
		Fields ();

		SFRotation* const orientation;
		SFVec3f* const centerOfRotation;
		SFBool* const jump;
		SFVec3f positionOffset;
		SFRotation orientationOffset;
		SFVec3f scaleOffset;
		SFRotation scaleOrientationOffset;
		SFVec3f centerOfRotationOffset;
	};

	Fields fields;

	Matrix4f parentMatrix;
	Matrix4f transformationMatrix;
	Matrix4f inverseTransformationMatrix;

	X3DSFNode <TimeSensor>              timeSensor;
	X3DSFNode <EaseInEaseOut>           easeInEaseOut;
	X3DSFNode <PositionInterpolator>    positionInterpolator;
	X3DSFNode <OrientationInterpolator> orientationInterpolator;
	X3DSFNode <PositionInterpolator>    scaleInterpolator;
	X3DSFNode <OrientationInterpolator> scaleOrientationInterpolator;

};

} // X3D
} // titania

#endif

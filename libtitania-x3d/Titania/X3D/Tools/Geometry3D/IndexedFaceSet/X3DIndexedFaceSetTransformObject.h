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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_TRANSFORM_OBJECT_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_TRANSFORM_OBJECT_H__

#include "X3DIndexedFaceSetSelectionObject.h"

namespace titania {
namespace X3D {

class TouchSensor;
class PlaneSensor;
class Transform;
class Switch;
class ScreenGroup;
class CoordinateDouble;

class X3DIndexedFaceSetTransformObject :
	virtual public X3DIndexedFaceSetSelectionObject
{
public:

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Hidden fields

	SFBool &
	transform ()
	{ return *fields .transform; }

	const SFBool &
	transform () const
	{ return *fields .transform; }

	SFBool &
	axisAlignedBoundingBox ()
	{ return *fields .axisAlignedBoundingBox; }

	const SFBool &
	axisAlignedBoundingBox () const
	{ return *fields .axisAlignedBoundingBox; }

	X3DPtr <Transform> &
	getSelectionTransform ()
	{ return selectionTransform; }

	const X3DPtr <Transform> &
	getSelectionTransform () const
	{ return selectionTransform; }

	const Matrix4d &
	getAxisRotation () const
	{ return axisRotation; }

	///  @name Destruction

	virtual
	void
	dispose ()
	{ }

	virtual
	~X3DIndexedFaceSetTransformObject () override;


protected:

	///  @name Construction

	X3DIndexedFaceSetTransformObject ();

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_loadState ();

	void
	set_transform ();

	void
	set_selection ();

	void
	set_touch_sensor_hitPoint ();

	void
	set_plane_sensor_active (const bool);

	void
	set_plane_sensor_translation ();

	void
	set_transform_active (const bool);

	void
	set_transform_modelViewMatrix ();

	Box3d
	getMinimumBBox () const;

	Box3d
	getMinimumBBox (const std::vector <Vector3d> &) const;

	Matrix3d
	getRotationMatrix (const Matrix4d & matrix) const;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFBool* const transform;
		SFBool* const axisAlignedBoundingBox;
	};

	///  @name Members

	Fields fields;

	X3DPtr <TouchSensor>      touchSensor;
	X3DPtr <PlaneSensor>      planeSensor;
	X3DPtr <PlaneSensor>      planeSensorNormal;
	X3DPtr <Switch>           transformToolSwitch;
	X3DPtr <Transform>        transformNode;
	X3DPtr <Transform>        transformTool;
	X3DPtr <Transform>        selectionTransform;
	X3DPtr <CoordinateDouble> selectionCoord;

	Matrix4d                  axisRotation;
	bool                      active;
	size_t                    events;
	UndoStepPtr               undoStep;

};

} // X3D
} // titania

#endif

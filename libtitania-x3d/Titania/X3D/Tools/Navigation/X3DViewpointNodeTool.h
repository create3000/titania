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

#ifndef __TITANIA_X3D_TOOLS_NAVIGATION_X3DVIEWPOINT_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_NAVIGATION_X3DVIEWPOINT_NODE_TOOL_H__

#include "../Core/X3DBindableNodeTool.h"

#include "../../Components/Grouping/X3DBoundedObject.h"
#include "../../Components/Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

class X3DViewpointNodeTool :
	virtual public X3DViewpointNode,
	public X3DBindableNodeTool,
	public X3DBoundedObject
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Fields

	virtual
	SFRotation &
	orientation () final override
	{ return getNode <X3DViewpointNode> () -> orientation (); }

	virtual
	const SFRotation &
	orientation () const final override
	{ return getNode <X3DViewpointNode> () -> orientation (); }

	virtual
	SFBool &
	jump () final override
	{ return getNode <X3DViewpointNode> () -> jump (); }

	virtual
	const SFBool &
	jump () const final override
	{ return getNode <X3DViewpointNode> () -> jump (); }

	virtual
	SFVec3d &
	positionOffset () final override
	{ return getNode <X3DViewpointNode> () -> positionOffset (); }

	virtual
	const SFVec3d &
	positionOffset () const final override
	{ return getNode <X3DViewpointNode> () -> positionOffset (); }

	virtual
	SFRotation &
	orientationOffset () final override
	{ return getNode <X3DViewpointNode> () -> orientationOffset (); }

	virtual
	const SFRotation &
	orientationOffset () const final override
	{ return getNode <X3DViewpointNode> () -> orientationOffset (); }

	virtual
	SFVec3d &
	centerOfRotationOffset () final override
	{ return getNode <X3DViewpointNode> () -> centerOfRotationOffset (); }

	virtual
	const SFVec3d &
	centerOfRotationOffset () const final override
	{ return getNode <X3DViewpointNode> () -> centerOfRotationOffset (); }

	virtual
	SFDouble &
	fieldOfViewScale () final override
	{ return getNode <X3DViewpointNode> () -> fieldOfViewScale (); }

	virtual
	const SFDouble &
	fieldOfViewScale () const final override
	{ return getNode <X3DViewpointNode> () -> fieldOfViewScale (); }

	///  @name Member access

	virtual
	Box3d
	getBBox () const final override;

	virtual
	Matrix4d
	getProjectionMatrix (X3DRenderObject* const renderObject) const final override
	{ return getNode <X3DViewpointNode> () -> getProjectionMatrix (renderObject); }

	virtual
	const Matrix4d &
	getCameraSpaceMatrix () const final override
	{ return getNode <X3DViewpointNode> () -> getCameraSpaceMatrix (); }

	virtual
	const Matrix4d &
	getInverseCameraSpaceMatrix () const final override
	{ return getNode <X3DViewpointNode> () -> getInverseCameraSpaceMatrix (); }

	virtual
	const Matrix4d &
	getModelMatrix () const final override
	{ return getNode <X3DViewpointNode> () -> getModelMatrix (); }

	virtual
	void
	setAnimate (const bool value) final override
	{ getNode <X3DViewpointNode> () -> setAnimate (value); }

	virtual
	bool
	getAnimate () const final override
	{ return getNode <X3DViewpointNode> () -> getAnimate (); }

	///  @name Operations
	virtual
	void
	isLockedToCamera (const bool value) final override
	{ getNode <X3DViewpointNode> () -> isLockedToCamera (value); }

	virtual
	const SFBool &
	isLockedToCamera () const final override
	{ return getNode <X3DViewpointNode> () -> isLockedToCamera (); }

	virtual
	void
	applyUserOffsets () final override
	{ getNode <X3DViewpointNode> () -> applyUserOffsets (); }

	virtual
	void
	resetUserOffsets () final override
	{ getNode <X3DViewpointNode> () -> resetUserOffsets (); }

	virtual
	void
	straighten (const bool horizon = false) final override
	{ getNode <X3DViewpointNode> () -> straighten (horizon); }

	virtual
	Rotation4d
	straightenHorizon (const Rotation4d & orientation) const final override
	{ return getNode <X3DViewpointNode> () -> straightenHorizon (orientation); }

	virtual
	Rotation4d
	straightenView (const Rotation4d & orientation) const final override
	{ return getNode <X3DViewpointNode> () -> straightenView (orientation); }

	virtual
	void
	lookAt (Vector3d point, const double factor = 1, const bool straighten = false, const time_type cycleInterval = 0.2) final override
	{ getNode <X3DViewpointNode> () -> lookAt (point, factor, straighten, cycleInterval); }

	virtual
	void
	lookAt (Box3d bbox, const double factor = 1, const bool straighten = false, const time_type cycleInterval = 0.2) final override
	{ getNode <X3DViewpointNode> () -> lookAt (bbox, factor, straighten, cycleInterval); }

	virtual
	void
	transitionStart (X3DViewpointNode* const fromViewpoint) final override
	{ getNode <X3DViewpointNode> () -> transitionStart (fromViewpoint); }

	virtual
	void
	transitionStop () final override
	{ getNode <X3DViewpointNode> () -> transitionStop (); }

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool really) final override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DViewpointNodeTool () override;


protected:

	///  @name Construction

	X3DViewpointNodeTool ();

	virtual
	void
	initialize () override;

	virtual
	void
	realize () override;


private:

	void
	set_positionOffset ();

	///  @name Members

	X3DPtr <Switch> switchNode;
	Vector3d        startPosition;
	Rotation4d      startOrientation;

};

} // X3D
} // titania

#endif

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
	SFRotation4d &
	orientationOffset () final override
	{ return getNode <X3DViewpointNode> () -> orientationOffset (); }

	virtual
	const SFRotation4d &
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
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override
	{ X3DBindableNodeTool::setExecutionContext (executionContext); }

	virtual
	Box3d
	getBBox () const final override;

	///  @name Operations

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
	addTool () override;

	virtual
	void
	removeTool (const bool) final override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	X3DViewpointNodeTool ();

	virtual
	void
	initialize () override;

	virtual
	void
	realize () final override;

	///  @name Operations

	virtual
	void
	reshape ()
	{ }

	///  @name Members

	Vector3d   startPosition;
	Rotation4d startOrientation;

};

} // X3D
} // titania

#endif

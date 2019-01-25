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

#ifndef __TITANIA_X3D_TOOLS_H_ANIM_HANIM_JOINT_TOOL_TOOL_H__
#define __TITANIA_X3D_TOOLS_H_ANIM_HANIM_JOINT_TOOL_TOOL_H__

#include "../Grouping/X3DTransformNodeTool.h"

#include "../../Components/H-Anim/HAnimJoint.h"

namespace titania {
namespace X3D {

class HAnimJointTool :
	virtual public HAnimJoint,
	public X3DTransformNodeTool
{
public:

	///  @name Construction

	HAnimJointTool (X3DBaseNode* const node) :
		         X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		          HAnimJoint (node -> getExecutionContext ()),
		         X3DBaseTool (node),
		X3DTransformNodeTool ()
	{
		//addType (X3DConstants::HAnimJointTool);
	}

	///  @name Fields

	virtual
	SFString &
	name () final override
	{ return getNode <HAnimJoint> () -> name (); }

	virtual
	const SFString &
	name () const final override
	{ return getNode <HAnimJoint> () -> name (); }

	virtual
	MFFloat &
	llimit () final override
	{ return getNode <HAnimJoint> () -> llimit (); }

	virtual
	const MFFloat &
	llimit () const final override
	{ return getNode <HAnimJoint> () -> llimit (); }

	virtual
	MFFloat &
	ulimit () final override
	{ return getNode <HAnimJoint> () -> ulimit (); }

	virtual
	const MFFloat &
	ulimit () const final override
	{ return getNode <HAnimJoint> () -> ulimit (); }

	virtual
	SFRotation &
	limitOrientation () final override
	{ return getNode <HAnimJoint> () -> limitOrientation (); }

	virtual
	const SFRotation &
	limitOrientation () const final override
	{ return getNode <HAnimJoint> () -> limitOrientation (); }

	virtual
	MFFloat &
	stiffness () final override
	{ return getNode <HAnimJoint> () -> stiffness (); }

	virtual
	const MFFloat &
	stiffness () const final override
	{ return getNode <HAnimJoint> () -> stiffness (); }

	virtual
	MFInt32 &
	skinCoordIndex () final override
	{ return getNode <HAnimJoint> () -> skinCoordIndex (); }

	virtual
	const MFInt32 &
	skinCoordIndex () const final override
	{ return getNode <HAnimJoint> () -> skinCoordIndex (); }

	virtual
	MFFloat &
	skinCoordWeight () final override
	{ return getNode <HAnimJoint> () -> skinCoordWeight (); }

	virtual
	const MFFloat &
	skinCoordWeight () const final override
	{ return getNode <HAnimJoint> () -> skinCoordWeight (); }

	virtual
	MFNode &
	displacers () final override
	{ return getNode <HAnimJoint> () -> displacers (); }

	virtual
	const MFNode &
	displacers () const final override
	{ return getNode <HAnimJoint> () -> displacers (); }

	///  @name Member access

	virtual
	const SFBool &
	isCameraObject () const final override
	{ return getNode <HAnimJoint> () -> isCameraObject (); }

	virtual
	const Matrix4d &
	getModelMatrix () const
	{ return getNode <HAnimJoint> () -> getModelMatrix (); }

	/// @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{ X3DTransformNodeTool::traverse (type, renderObject); }

	/// @name Destruction

	virtual
	void
	dispose () final override
	{ X3DTransformNodeTool::dispose (); }


private:

	virtual
	void
	initialize () final override
	{ X3DTransformNodeTool::initialize (); }

};

} // X3D
} // titania

#endif

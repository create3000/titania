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

#ifndef __TITANIA_X3D_TOOLS_H_ANIM_HANIM_HUMANOID_TOOL_H__
#define __TITANIA_X3D_TOOLS_H_ANIM_HANIM_HUMANOID_TOOL_H__

#include "../Core/X3DChildNodeTool.h"
#include "../Grouping/X3DBoundedObjectTool.h"
#include "../ToolColors.h"

#include "../../Components/H-Anim/HAnimHumanoid.h"

namespace titania {
namespace X3D {

class HAnimHumanoidTool :
	virtual public HAnimHumanoid,
	virtual public X3DChildNodeTool,
	public X3DBoundedObjectTool
{
public:

	///  @name Construction

	HAnimHumanoidTool (X3DBaseNode* const node) :
		         X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		       HAnimHumanoid (node -> getExecutionContext ()),
		         X3DBaseTool (node),
		    X3DChildNodeTool (),
		X3DBoundedObjectTool (ToolColors::GREEN)
	{
		//addType (X3DConstants::HAnimHumanoidTool);
	}

	///  @name Fields

	virtual
	SFString &
	name () final override
	{ return getNode <HAnimHumanoid> () -> name (); }

	virtual
	const SFString &
	name () const final override
	{ return getNode <HAnimHumanoid> () -> name (); }

	virtual
	SFString &
	version () final override
	{ return getNode <HAnimHumanoid> () -> version (); }

	virtual
	const SFString &
	version () const final override
	{ return getNode <HAnimHumanoid> () -> version (); }

	virtual
	MFString &
	info () final override
	{ return getNode <HAnimHumanoid> () -> info (); }

	virtual
	const MFString &
	info () const final override
	{ return getNode <HAnimHumanoid> () -> info (); }

	virtual
	SFVec3f &
	translation () final override
	{ return getNode <HAnimHumanoid> () -> translation (); }

	virtual
	const SFVec3f &
	translation () const final override
	{ return getNode <HAnimHumanoid> () -> translation (); }

	virtual
	SFRotation &
	rotation () final override
	{ return getNode <HAnimHumanoid> () -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final override
	{ return getNode <HAnimHumanoid> () -> rotation (); }

	virtual
	SFVec3f &
	scale () final override
	{ return getNode <HAnimHumanoid> () -> scale (); }

	virtual
	const SFVec3f &
	scale () const final override
	{ return getNode <HAnimHumanoid> () -> scale (); }

	virtual
	SFRotation &
	scaleOrientation () final override
	{ return getNode <HAnimHumanoid> () -> scaleOrientation (); }

	virtual
	const SFRotation &
	scaleOrientation () const final override
	{ return getNode <HAnimHumanoid> () -> scaleOrientation (); }

	virtual
	SFVec3f &
	center () final override
	{ return getNode <HAnimHumanoid> () -> center (); }

	virtual
	const SFVec3f &
	center () const final override
	{ return getNode <HAnimHumanoid> () -> center (); }

	virtual
	MFNode &
	viewpoints () final override
	{ return getNode <HAnimHumanoid> () -> viewpoints (); }

	virtual
	const MFNode &
	viewpoints () const final override
	{ return getNode <HAnimHumanoid> () -> viewpoints (); }

	virtual
	MFNode &
	sites () final override
	{ return getNode <HAnimHumanoid> () -> sites (); }

	virtual
	const MFNode &
	sites () const final override
	{ return getNode <HAnimHumanoid> () -> sites (); }

	virtual
	MFNode &
	joints () final override
	{ return getNode <HAnimHumanoid> () -> joints (); }

	virtual
	const MFNode &
	joints () const final override
	{ return getNode <HAnimHumanoid> () -> joints (); }

	virtual
	MFNode &
	segments () final override
	{ return getNode <HAnimHumanoid> () -> segments (); }

	virtual
	const MFNode &
	segments () const final override
	{ return getNode <HAnimHumanoid> () -> segments (); }

	virtual
	MFNode &
	skeleton () final override
	{ return getNode <HAnimHumanoid> () -> skeleton (); }

	virtual
	const MFNode &
	skeleton () const final override
	{ return getNode <HAnimHumanoid> () -> skeleton (); }

	virtual
	SFNode &
	skinNormal () final override
	{ return getNode <HAnimHumanoid> () -> skinNormal (); }

	virtual
	const SFNode &
	skinNormal () const final override
	{ return getNode <HAnimHumanoid> () -> skinNormal (); }

	virtual
	SFNode &
	skinCoord () final override
	{ return getNode <HAnimHumanoid> () -> skinCoord (); }

	virtual
	const SFNode &
	skinCoord () const final override
	{ return getNode <HAnimHumanoid> () -> skinCoord (); }

	virtual
	MFNode &
	skin () final override
	{ return getNode <HAnimHumanoid> () -> skin (); }

	virtual
	const MFNode &
	skin () const final override
	{ return getNode <HAnimHumanoid> () -> skin (); }

	/// @name Member access

	virtual
	Box3d
	getBBox () const final override
	{ return getNode <HAnimHumanoid> () -> getBBox (); }

	virtual
	const X3DPtr <Transform> &
	getTransform () const final override
	{ return getNode <HAnimHumanoid> () -> getTransform (); }

	/// @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{ return getNode <HAnimHumanoid> () -> traverse (type, renderObject); }

	virtual
	void
	removeTool (const bool really = false) final override
	{
		getTransform () -> removeTool (really);
		X3DChildNodeTool::removeTool (really);
	}

	virtual
	void
	beginUndo () final override
	{ getTransformTools () [0] -> beginUndo (); }
	
	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override
	{ getTransformTools () [0] -> endUndo (undoStep); }

	/// @name Destruction

	virtual
	void
	dispose () final override
	{
		X3DBoundedObjectTool::dispose ();
		X3DChildNodeTool::dispose ();
	}

protected:

	///  @name Construction

	virtual
	void
	initialize () final override
	{
		X3DChildNodeTool::initialize ();

		setTransformTool (0, getTransform ());
	}

};

} // X3D
} // titania

#endif

/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_TOOLS_RIGID_BODY_PHYSICS_X3DNBODY_COLLIDABLE_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_RIGID_BODY_PHYSICS_X3DNBODY_COLLIDABLE_NODE_TOOL_H__

#include "../Core/X3DChildNodeTool.h"
#include "../Grouping/X3DBoundedObjectTool.h"
#include "../Grouping/X3DTransformMatrix3DObjectTool.h"
#include "../ToolColors.h"

#include "../../Components/RigidBodyPhysics/X3DNBodyCollidableNode.h"

namespace titania {
namespace X3D {

class X3DNBodyCollidableNodeTool :
	virtual public X3DNBodyCollidableNode,
	virtual public X3DChildNodeTool,
	public X3DTransformMatrix3DObjectTool,
	public X3DBoundedObjectTool
{
public:

	///  @name Fields

	virtual
	SFBool &
	enabled () final override
	{ return getNode <X3DNBodyCollidableNode> () -> enabled (); }

	virtual
	const SFBool &
	enabled () const final override
	{ return getNode <X3DNBodyCollidableNode> () -> enabled (); }

	virtual
	SFVec3f &
	translation () final override
	{ return getNode <X3DNBodyCollidableNode> () -> translation (); }

	virtual
	const SFVec3f &
	translation () const final override
	{ return getNode <X3DNBodyCollidableNode> () -> translation (); }

	virtual
	SFRotation &
	rotation () final override
	{ return getNode <X3DNBodyCollidableNode> () -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final override
	{ return getNode <X3DNBodyCollidableNode> () -> rotation (); }

	///  @name Member access

	virtual
	void
	setBody (RigidBody* const value) final override
	{ getNode <X3DNBodyCollidableNode> () -> setBody (value); }

	virtual
	const X3DPtr <RigidBody> &
	getBody () const final override
	{ return getNode <X3DNBodyCollidableNode> () -> getBody (); }

	virtual
	const std::shared_ptr <btCompoundShape> &
	getCompoundShape () const final override
	{ return getNode <X3DNBodyCollidableNode> () -> getCompoundShape (); }

	/// @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override
	{
		X3DChildNodeTool::traverse (type, renderObject);
		X3DBoundedObjectTool::traverse (type, renderObject);
	}

	/// @name Destruction

	virtual
	void
	dispose () override
	{
		X3DBoundedObjectTool::dispose ();
		X3DTransformMatrix3DObjectTool::dispose ();
		X3DChildNodeTool::dispose ();
	}

	virtual
	~X3DNBodyCollidableNodeTool () override
	{ }

	/// @name Destruction

protected:

	///  @name Construction

	X3DNBodyCollidableNodeTool (const Color3f & color) :
		        X3DNBodyCollidableNode (),
		              X3DChildNodeTool (),
		X3DTransformMatrix3DObjectTool (),
		          X3DBoundedObjectTool (color)
	{
		//addType (X3DConstants::X3DNBodyCollidableNodeTool);
	}

	virtual
	void
	initialize () override
	{
		X3DChildNodeTool::initialize ();
		X3DTransformMatrix3DObjectTool::initialize ();
		X3DBoundedObjectTool::initialize ();
	}

	void
	realize ()
	{
		X3DBoundedObjectTool::realize ();

		getNode <X3DNBodyCollidableNode> () -> enabled () .addInterest (&X3DNBodyCollidableNodeTool::set_enabled, this);

		set_enabled (getNode <X3DNBodyCollidableNode> () -> enabled ());
	}
	
	///  @name Event handlers

	void
	set_enabled (const bool value)
	{
		setLinetype (value ? LineType::SOLID : LineType::DOTTED);
	}

};

} // X3D
} // titania

#endif

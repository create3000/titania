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

#ifndef __TITANIA_X3D_TOOLS_NAVIGATION_X3DVIEWPOINT_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_NAVIGATION_X3DVIEWPOINT_NODE_TOOL_H__

#include "../Core/X3DBindableNodeTool.h"

#include "../../Bits/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

template <class Type>
class X3DViewpointNodeTool :
	public X3DBindableNodeTool <Type>,
	public X3DBoundedObject
{
public:

	///  @name Fields

	virtual
	SFRotation &
	orientation () final override
	{ return getNode () -> orientation (); }

	virtual
	const SFRotation &
	orientation () const final override
	{ return getNode () -> orientation (); }

	virtual
	SFBool &
	jump () final override
	{ return getNode () -> jump (); }

	virtual
	const SFBool &
	jump () const final override
	{ return getNode () -> jump (); }

	virtual
	SFVec3f &
	positionOffset () final override
	{ return getNode () -> positionOffset (); }

	virtual
	const SFVec3f &
	positionOffset () const final override
	{ return getNode () -> positionOffset (); }

	virtual
	SFRotation &
	orientationOffset () final override
	{ return getNode () -> orientationOffset (); }

	virtual
	const SFRotation &
	orientationOffset () const final override
	{ return getNode () -> orientationOffset (); }

	virtual
	SFVec3f &
	centerOfRotationOffset () final override
	{ return getNode () -> centerOfRotationOffset (); }

	virtual
	const SFVec3f &
	centerOfRotationOffset () const final override
	{ return getNode () -> centerOfRotationOffset (); }

	virtual
	SFFloat &
	fieldOfViewScale () final override
	{ return getNode () -> fieldOfViewScale (); }

	virtual
	const SFFloat &
	fieldOfViewScale () const final override
	{ return getNode () -> fieldOfViewScale (); }

	///  @name Operations

	virtual
	Box3f
	getBBox () const final override
	{ return getInlineNode () -> getBBox (); }

	virtual
	void
	traverse (const TraverseType type) override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool) final override;

	///  @name Destruction

	virtual
	void
	dispose () override
	{
		X3DBoundedObject::dispose ();
		X3DBindableNodeTool <Type>::dispose ();

		X3DParentObject::removeChildren (bboxSize (), bboxCenter ());
	}

protected:

	using X3DBindableNodeTool <Type>::addType;
	using X3DBindableNodeTool <Type>::getNode;
	using X3DBindableNodeTool <Type>::getInlineNode;
	using X3DBindableNodeTool <Type>::getToolNode;
	using X3DBindableNodeTool <Type>::requestAsyncLoad;

	///  @name Construction

	X3DViewpointNodeTool () :
		X3DBindableNodeTool <Type> (),
		          X3DBoundedObject ()
	{
		addType (X3DConstants::X3DViewpointNodeTool);

		X3DParentObject::addChildren (bboxSize (), bboxCenter ());
	}

	virtual
	void
	initialize () override;

	virtual
	void
	realize () final override;

	virtual
	void
	reshape ()
	{ }

};

template <class Type>
void
X3DViewpointNodeTool <Type>::initialize ()
{
	X3DBindableNodeTool <Type>::initialize ();
	X3DBoundedObject::initialize ();

	requestAsyncLoad ({ get_tool ("ViewpointTool.x3dv") .str () });
}

template <class Type>
void
X3DViewpointNodeTool <Type>::realize ()
{
	try
	{
		getToolNode () -> setField <SFNode> ("viewpointNode", getNode ());
	}
	catch (const X3DError & error)
	{ }
}

template <class Type>
void
X3DViewpointNodeTool <Type>::addTool ()
{
	try
	{
		getToolNode () -> setField <SFBool> ("set_selected", getBrowser () -> getSelection () -> isSelected (this));
	}
	catch (const X3DError &)
	{ }
}

template <class Type>
void
X3DViewpointNodeTool <Type>::removeTool (const bool really)
{
	if (really)
		X3DBindableNodeTool <Type>::removeTool ();

	else
	{
		try
		{
			getToolNode () -> setField <SFBool> ("set_selected", false);
		}
		catch (const X3DError &)
		{ }
	}
}

template <class Type>
void
X3DViewpointNodeTool <Type>::traverse (const TraverseType type)
{
	getNode () -> traverse (type);

	if (type == TraverseType::DISPLAY) // Last chance to process events
		reshape ();

	// Tool

	X3DToolObject::traverse (type);
}

} // X3D
} // titania

#endif

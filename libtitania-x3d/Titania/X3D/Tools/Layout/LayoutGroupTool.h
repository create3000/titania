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

#ifndef __TITANIA_X3D_TOOLS_LAYOUT_LAYOUT_GROUP_H__
#define __TITANIA_X3D_TOOLS_LAYOUT_LAYOUT_GROUP_H__

#include "../Grouping/X3DGroupingNodeTool.h"
#include "../ToolColors.h"

#include "../../Components/Layout/LayoutGroup.h"

namespace titania {
namespace X3D {

class LayoutGroupTool :
	public X3DGroupingNodeTool <LayoutGroup>
{
public:

	///  @name Construction

	LayoutGroupTool (LayoutGroup* const node) :
		                      X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		        X3DBaseTool <LayoutGroup> (node),
		X3DGroupingNodeTool <LayoutGroup> (ToolColors::DARK_GREEN)
	{
		//addType (X3DConstants::LayoutGroupTool);
	}

	///  @name Fields

	virtual
	SFNode &
	viewport () final override
	{ return getNode () -> viewport (); }

	virtual
	const SFNode &
	viewport () const final override
	{ return getNode () -> viewport (); }

	virtual
	SFNode &
	layout () final override
	{ return getNode () -> layout (); }

	virtual
	const SFNode &
	layout () const final override
	{ return getNode () -> layout (); }
	
	///  @name Member access

	virtual
	Box3f
	getRectangleBBox () const final override
	{ return getNode () -> getRectangleBBox (); }

	virtual
	const Matrix4f &
	getMatrix () const final override
	{ return getNode () -> getMatrix (); }


private:

	///  @name Operations

	virtual
	void
	realize () final override;

	virtual
	void
	reshape () final override;

};

inline
void
LayoutGroupTool::realize ()
{
	try
	{
		const SFNode & tool = getToolNode ();

		tool -> setField <SFInt32> ("rectangle", 0);
	}
	catch (const X3DError & error)
	{ }

	X3DGroupingNodeTool <LayoutGroup>::realize ();
}

inline
void
LayoutGroupTool::reshape ()
{
	Box3f bbox = getNode () -> getRectangleBBox ();
	
	try
	{
		bbox *= ~getMatrix ();
	}
	catch (const std::domain_error &)
	{
		bbox = Box3f (Vector3f (), Vector3f ());
	}

	try
	{
		const SFNode & tool = getToolNode ();

		tool -> setField <SFInt32> ("rectangle", bbox .empty () ? -1 : 0, true);
		tool -> setField <SFVec3f> ("rectangleBBoxSize",   bbox .size (),   true);
		tool -> setField <SFVec3f> ("rectangleBBoxCenter", bbox .center (), true);
	}
	catch (const X3DError &)
	{ }

	X3DGroupingNodeTool <LayoutGroup>::reshape ();
}

} // X3D
} // titania

#endif

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

#include "SoundTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

SoundTool::SoundTool (X3DBaseNode* const node) :
	     X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	           Sound (node -> getExecutionContext ()),
	     X3DBaseTool (node),
	X3DSoundNodeTool (),
	X3DBoundedObject ()
{
	addType (X3DConstants::SoundTool);
}

void
SoundTool::initialize ()
{
	X3DSoundNodeTool::initialize ();
	X3DBoundedObject::initialize ();

	getBrowser () -> getSoundTools () .emplace_back (this);

	requestAsyncLoad ({ get_tool ("SoundTool.x3dv") .str () });
}

void
SoundTool::realize ()
{
	try
	{
		getToolNode () -> setField <SFNode>  ("sound", getNode <Sound> ());
	}
	catch (const X3DError & error)
	{ }
}

Box3d
SoundTool::getBBox () const
{
	if (getBrowser () -> getDisplayTools () .top ())
	{
		const auto a = (maxBack () + maxFront ()) / 2;
		const auto e = a - maxBack ();
		const auto b = std::sqrt (a * a - e * e);

		const auto center   = Vector3d (0, 0, e);
		const auto size     = Vector3d (b, b, a) * 2.0;
		const auto rotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (direction () .getValue ()));

		auto bbox = Box3d (size, center);

		bbox .rotate (rotation);
		bbox .translate (Vector3d (location () .getValue ()));

		return bbox;
	}

	return Box3d ();
}

void
SoundTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	getNode <Sound> () -> traverse (type, renderObject);

	X3DToolObject::traverse (type, renderObject);
}

void
SoundTool::addTool ()
{
	try
	{
		getToolNode () -> setField <SFBool> ("selected", getBrowser () -> getSelection () -> isSelected (SFNode (this)));
	}
	catch (const X3DError &)
	{ }
}

void
SoundTool::removeTool (const bool really)
{
	if (really)
		X3DSoundNodeTool::removeTool ();

	else
	{
		try
		{
			getToolNode () -> setField <SFBool> ("selected", false);
		}
		catch (const X3DError &)
		{ }
	}
}

void
SoundTool::dispose ()
{
	getBrowser () -> getSoundTools () .remove (nullptr);

	X3DBoundedObject::dispose ();
	X3DSoundNodeTool::dispose ();
}

} // X3D
} // titania

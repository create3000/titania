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

#include "X3DBoundedObjectTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../../Routing/Router.h"

namespace titania {
namespace X3D {

X3DBoundedObjectTool::X3DBoundedObjectTool (const Color3f & color) :
	X3DBoundedObject (),
	     X3DBaseTool (),
	        linetype (LineType::DOTTED),
	   displayCenter (false),
	           color (color)
{
	addType (X3DConstants::X3DBoundedObjectTool);
}

void
X3DBoundedObjectTool::initialize ()
{
	requestAsyncLoad ({ get_tool ("BoundingBox.x3dv") .str () });
}

void
X3DBoundedObjectTool::realize ()
{
	try
	{
		const SFNode & tool = getToolNode ();

		tool -> setField <SFBool>  ("displayCenter", displayCenter);
		tool -> setField <SFColor> ("color",         color);
		tool -> setField <SFInt32> ("linetype",      int32_t (linetype));
	}
	catch (const X3DError & error)
	{ }
}

void
X3DBoundedObjectTool::setLinetype (const LineType value)
{
	try
	{
		linetype = value;

		getToolNode () -> setField <SFInt32> ("linetype", int32_t (linetype));
	}
	catch (const X3DError & error)
	{ }
}

Box3d
X3DBoundedObjectTool::getGroupBBox () const
{
	try
	{
		Box3d bbox = getNode <X3DBoundedObject> () -> getBBox ();
	
		bbox *= inverse (getMatrix ());

		return bbox;
	}
	catch (const std::domain_error &)
	{
		return Box3d (Vector3d (), Vector3d ());
	}
}

void
X3DBoundedObjectTool::reshape ()
{
	try
	{
	   getBrowser () -> endUpdateForFrame ();

		const auto   bbox = getGroupBBox ();
		const auto & tool = getToolNode ();

		tool -> setField <SFVec3f> ("bboxSize",   bbox .size (),   true);
		tool -> setField <SFVec3f> ("bboxCenter", bbox .center (), true);

		getBrowser () -> getRouter () -> processEvents ();
	   getBrowser () -> beginUpdateForFrame ();
	}
	catch (const X3DError &)
	{
		getBrowser () -> beginUpdateForFrame ();
	}
}

void
X3DBoundedObjectTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	getNode <X3DBoundedObject> () -> traverse (type, renderObject);

	// Tool

	renderObject -> getModelViewMatrix () .push ();
	renderObject -> getModelViewMatrix () .mult_left (getMatrix ());

	if (type == TraverseType::DISPLAY) // Last chance to process events
		X3DBoundedObjectTool::reshape ();

	X3DToolObject::traverse (type, renderObject);

	renderObject -> getModelViewMatrix () .pop ();
}

} // X3D
} // titania

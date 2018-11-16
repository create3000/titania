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

#include "DepthBuffer.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/DepthTestContainer.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const ComponentType DepthBuffer::component      = ComponentType::TITANIA;
const std::string   DepthBuffer::typeName       = "DepthBuffer";
const std::string   DepthBuffer::containerField = "children";

DepthBuffer::Fields::Fields () :
	          enabled (new SFBool (true)),
	    depthFunction (new SFString ("LEQUAL")),
	      depthOffset (new SFDouble (0))
{ }

DepthBuffer::DepthBuffer (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DGroupingNode (),
	depthFunctionType (GL_LEQUAL)
{
	addType (X3DConstants::DepthBuffer);

	addField (inputOutput,    "metadata",       metadata ());

	addField (inputOutput,    "enabled",        enabled ());
	addField (inputOutput,    "depthFunction",  depthFunction ());
	addField (inputOutput,    "depthOffset",    depthOffset ());

	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
}

X3DBaseNode*
DepthBuffer::create (X3DExecutionContext* const executionContext) const
{
	return new DepthBuffer (executionContext);
}

void
DepthBuffer::initialize ()
{
	X3DGroupingNode::initialize ();

	depthFunction () .addInterest (&DepthBuffer::set_depthFunction, this);

	set_depthFunction ();
}

void
DepthBuffer::set_depthFunction ()
{
	static const std::map <std::string, GLenum> depthFunctionTypes = {
	   std::pair ("NEVER",    GL_NEVER),
	   std::pair ("LESS",     GL_LESS),
	   std::pair ("EQUAL",    GL_EQUAL),
	   std::pair ("LEQUAL",   GL_LEQUAL),
	   std::pair ("GREATER",  GL_GREATER),
	   std::pair ("NOTEQUAL", GL_NOTEQUAL),
	   std::pair ("GEQUAL",   GL_GEQUAL),
	   std::pair ("ALWAYS",   GL_ALWAYS),
	};

	try
	{
		depthFunctionType = depthFunctionTypes .at (depthFunction ());
	}
	catch (const X3DError &)
	{
		depthFunctionType = GL_LEQUAL;
	}
}

void
DepthBuffer::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	renderObject -> getBlend ()       .push (not enabled ());
	renderObject -> getDepthOffset () .push (depthOffset ());

	switch (type)
	{
		case TraverseType::DISPLAY:
		{
			renderObject -> getLocalObjects () .emplace_back (new DepthTestContainer (this));

			X3DGroupingNode::traverse (type, renderObject);

			renderObject -> getLocalObjects () .pop_back ();
			break;
		}
		default:
		{
			X3DGroupingNode::traverse (type, renderObject);
			break;
		}
	}

	renderObject -> getDepthOffset () .pop ();
	renderObject -> getBlend ()       .pop ();
}

} // X3D
} // titania

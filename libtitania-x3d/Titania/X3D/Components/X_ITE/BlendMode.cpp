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

#include "BlendMode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/BlendModeContainer.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const ComponentType BlendMode::component      = ComponentType::X_ITE;
const std::string   BlendMode::typeName       = "BlendMode";
const std::string   BlendMode::containerField = "children";

const std::map <std::string, GLenum> BlendMode::blendTypes = {
   std::make_pair ("ZERO",                     GL_ZERO ),
   std::make_pair ("ONE",                      GL_ONE ),
   std::make_pair ("SRC_COLOR",                GL_SRC_COLOR ),
   std::make_pair ("ONE_MINUS_SRC_COLOR",      GL_ONE_MINUS_SRC_COLOR ),
   std::make_pair ("DST_COLOR",                GL_DST_COLOR ),
   std::make_pair ("ONE_MINUS_DST_COLOR",      GL_ONE_MINUS_DST_COLOR ),
   std::make_pair ("SRC_ALPHA",                GL_SRC_ALPHA ),
   std::make_pair ("ONE_MINUS_SRC_ALPHA",      GL_ONE_MINUS_SRC_ALPHA ),
   std::make_pair ("DST_ALPHA",                GL_DST_ALPHA ),
   std::make_pair ("ONE_MINUS_DST_ALPHA",      GL_ONE_MINUS_DST_ALPHA ),
   std::make_pair ("CONSTANT_COLOR",           GL_CONSTANT_COLOR  ),
   std::make_pair ("ONE_MINUS_CONSTANT_COLOR", GL_ONE_MINUS_CONSTANT_COLOR ),
   std::make_pair ("CONSTANT_ALPHA",           GL_CONSTANT_ALPHA ),
   std::make_pair ("ONE_MINUS_CONSTANT_ALPHA", GL_ONE_MINUS_CONSTANT_ALPHA ),
   std::make_pair ("SRC_ALPHA_SATURATE",       GL_SRC_ALPHA_SATURATE ),
   std::make_pair ("SRC1_COLOR",               GL_SRC1_COLOR ),
   std::make_pair ("SRC1_ALPHA",               GL_SRC1_ALPHA ),
};

const std::map <std::string, GLenum> BlendMode::blendModes = {
   std::make_pair ("FUNC_ADD",              GL_FUNC_ADD ),
   std::make_pair ("FUNC_SUBTRACT",         GL_FUNC_SUBTRACT ),
   std::make_pair ("FUNC_REVERSE_SUBTRACT", GL_FUNC_REVERSE_SUBTRACT ),
};

BlendMode::Fields::Fields () :
	         enabled (new SFBool (true)),
	      blendColor (new SFColorRGBA ()),
	     sourceColor (new SFString ("SRC_ALPHA")),
	     sourceAlpha (new SFString ("ONE_MINUS_SRC_ALPHA")),
	destinationColor (new SFString ("ONE")),
	destinationAlpha (new SFString ("ONE_MINUS_SRC_ALPHA")),
	       modeColor (new SFString ("FUNC_ADD")),
	       modeAlpha (new SFString ("FUNC_ADD"))
{ }

BlendMode::BlendMode (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DGroupingNode (),
	     sourceColorType (GL_SRC_ALPHA),
	     sourceAlphaType (GL_ONE_MINUS_SRC_ALPHA),
	destinationColorType (GL_ONE),
	destinationAlphaType (GL_ONE_MINUS_SRC_ALPHA),
	       modeColorType (GL_FUNC_ADD),
	       modeAlphaType (GL_FUNC_ADD)
{
	addType (X3DConstants::BlendMode);

	addField (inputOutput,    "metadata",         metadata ());

	addField (inputOutput,    "enabled",          enabled ());
	addField (inputOutput,    "blendColor",       blendColor ());
	addField (inputOutput,    "sourceColor",      sourceColor ());
	addField (inputOutput,    "sourceAlpha",      sourceAlpha ());
	addField (inputOutput,    "destinationColor", destinationColor ());
	addField (inputOutput,    "destinationAlpha", destinationAlpha ());
	addField (inputOutput,    "modeColor",        modeColor ());
	addField (inputOutput,    "modeAlpha",        modeAlpha ());

	addField (initializeOnly, "bboxSize",         bboxSize ());
	addField (initializeOnly, "bboxCenter",       bboxCenter ());
	addField (inputOnly,      "addChildren",      addChildren ());
	addField (inputOnly,      "removeChildren",   removeChildren ());
	addField (inputOutput,    "children",         children ());
}

X3DBaseNode*
BlendMode::create (X3DExecutionContext* const executionContext) const
{
	return new BlendMode (executionContext);
}

void
BlendMode::initialize ()
{
	X3DGroupingNode::initialize ();

	sourceColor ()      .addInterest (&BlendMode::set_sourceColor,      this);
	sourceAlpha ()      .addInterest (&BlendMode::set_sourceAlpha,      this);
	destinationColor () .addInterest (&BlendMode::set_destinationColor, this);
	destinationAlpha () .addInterest (&BlendMode::set_destinationAlpha, this);
	modeColor ()        .addInterest (&BlendMode::set_modeColor,        this);
	modeAlpha ()        .addInterest (&BlendMode::set_modeAlpha,        this);

	set_sourceColor ();
	set_sourceAlpha ();
	set_destinationColor ();
	set_destinationAlpha ();
	set_modeColor ();
	set_modeAlpha ();
}

void
BlendMode::set_sourceColor ()
{
	try
	{
		sourceColorType = blendTypes .at (sourceColor ());
	}
	catch (const X3DError &)
	{
		sourceColorType = GL_SRC_ALPHA;
	}
}

void
BlendMode::set_sourceAlpha ()
{
	try
	{
		sourceAlphaType = blendTypes .at (sourceAlpha ());
	}
	catch (const X3DError &)
	{
		sourceAlphaType = GL_ONE_MINUS_SRC_ALPHA;
	}
}

void
BlendMode::set_destinationColor ()
{
	try
	{
		destinationColorType = blendTypes .at (destinationColor ());
	}
	catch (const X3DError &)
	{
		destinationColorType = GL_ONE;
	}
}

void
BlendMode::set_destinationAlpha ()
{
	try
	{
		destinationAlphaType = blendTypes .at (destinationAlpha ());
	}
	catch (const X3DError &)
	{
		destinationAlphaType = GL_ONE_MINUS_SRC_ALPHA;
	}
}

void
BlendMode::set_modeColor ()
{
	try
	{
		modeColorType = blendModes .at (modeColor ());
	}
	catch (const X3DError &)
	{
		modeColorType = GL_FUNC_ADD;
	}
}

void
BlendMode::set_modeAlpha ()
{
	try
	{
		modeAlphaType = blendModes .at (modeAlpha ());
	}
	catch (const X3DError &)
	{
		modeAlphaType = GL_FUNC_ADD;
	}
}

void
BlendMode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::DISPLAY:
		{
			renderObject -> getBlend () .push (enabled ());
			renderObject -> getLocalObjects () .emplace_back (new BlendModeContainer (this));

			X3DGroupingNode::traverse (type, renderObject);

			renderObject -> getLocalObjects () .pop_back ();
			renderObject -> getBlend () .pop ();
			break;
		}
		default:
		{
			X3DGroupingNode::traverse (type, renderObject);
			break;
		}
	}
}

} // X3D
} // titania

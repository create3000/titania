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

#include "Text.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Text/X3DFontStyleNode.h"


namespace titania {
namespace X3D {

const Component   Text::component      = Component ("Text", 3);
const std::string Text::typeName       = "Text";
const std::string Text::containerField = "geometry";

Text::Fields::Fields () :
	    string (new MFString ()),
	    length (new MFFloat ()),
	 maxExtent (new SFFloat ()),
	     solid (new SFBool ()),
	    origin (new SFVec3f ()),
	textBounds (new SFVec2f ()),
	lineBounds (new MFVec2f ()),
	 fontStyle (new SFNode ())
{ }

Text::Text (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	  fontStyleNode (),
	   textGeometry ()
{
	addType (X3DConstants::Text);

	addField (inputOutput,    "metadata",   metadata ());
	addField (inputOutput,    "string",     string ());
	addField (inputOutput,    "length",     length ());
	addField (inputOutput,    "maxExtent",  maxExtent ());
	addField (initializeOnly, "solid",      solid ());
	addField (outputOnly,     "origin",     origin ());
	addField (outputOnly,     "textBounds", textBounds ());
	addField (outputOnly,     "lineBounds", lineBounds ());
	addField (inputOutput,    "fontStyle",  fontStyle ());

	length ()     .setUnit (UnitCategory::LENGTH);
	maxExtent ()  .setUnit (UnitCategory::LENGTH);
	origin ()     .setUnit (UnitCategory::LENGTH);
	textBounds () .setUnit (UnitCategory::LENGTH);
	lineBounds () .setUnit (UnitCategory::LENGTH);

	addChildObjects (fontStyleNode, textGeometry);
}

X3DBaseNode*
Text::create (X3DExecutionContext* const executionContext) const
{
	return new Text (executionContext);
}

void
Text::initialize ()
{
	X3DGeometryNode::initialize ();

	fontStyle () .addInterest (&Text::set_fontStyle, this);

	set_fontStyle ();
}

void
Text::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (textGeometry)
		textGeometry -> setExecutionContext (executionContext);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		set_fontStyle ();

		addEvent ();
	}
}

Box3d
Text::createBBox () const
{
	return textGeometry -> X3DTextGeometry::getBBox ();
}

const Box3d &
Text::getBBox () const
{
	return textGeometry -> getBBox ();
}

bool
Text::isTransparent () const
{
	return textGeometry -> isTransparent ();
}

float
Text::getLength (const size_t index)
{
	if (index < length () .size ())
		return std::max <float> (0, length () [index]);

	return 0;
}

const Matrix4d &
Text::getMatrix () const
{
	return textGeometry -> getMatrix ();
}

void
Text::set_fontStyle ()
{
	if (fontStyleNode)
		fontStyleNode -> removeInterest (&Text::requestRebuild, this);

	fontStyleNode .set (x3d_cast <X3DFontStyleNode*> (fontStyle ()));

	if (not fontStyleNode)
		fontStyleNode .set (getBrowser () -> getDefaultFontStyle ());

	fontStyleNode -> addInterest (&Text::requestRebuild, this);
}

void
Text::build ()
{
	try
	{
		// Let the fontStyle build the text geometry.

		textGeometry .set (fontStyleNode -> getTextGeometry (this));
	}
	catch (const std::exception &)
	{
		// Bad alloc.
	}
}

void
Text::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	try
	{
		textGeometry -> traverse (type, renderObject);

		X3DGeometryNode::traverse (type, renderObject);
	}
	catch (const std::exception &)
	{
		// Bad alloc.
	}
}

void
Text::draw (ShapeContainer* const context)
{
	try
	{
		textGeometry -> draw (context);
	
		X3DGeometryNode::draw (context);
	}
	catch (const std::exception &)
	{
		// Bad alloc.
	}
}

SFNode
Text::toPrimitive () const
{
	return textGeometry -> toPrimitive ();
}

void
Text::dispose ()
{
	X3DGeometryNode::dispose ();
}

Text::~Text ()
{ }

} // X3D
} // titania

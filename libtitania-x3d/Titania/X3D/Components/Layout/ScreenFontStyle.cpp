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

#include <gdkmm.h>

#include "ScreenFontStyle.h"

#include "../../Browser/Layout/ScreenText.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   ScreenFontStyle::component      = Component ("Layout", 2);
const std::string ScreenFontStyle::typeName       = "ScreenFontStyle";
const std::string ScreenFontStyle::containerField = "fontStyle";

ScreenFontStyle::Fields::Fields () :
	pointSize (new SFFloat (12))
{ }

ScreenFontStyle::ScreenFontStyle (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DFontStyleNode (),
	          fields (),
	            font (),
	        fontFace ()
{
	addType (X3DConstants::ScreenFontStyle);

	addField (inputOutput,    "metadata",    metadata ());
	addField (initializeOnly, "language",    language ());
	addField (initializeOnly, "family",      family ());
	addField (initializeOnly, "style",       style ());
	addField (initializeOnly, "pointSize",   pointSize ());
	addField (initializeOnly, "spacing",     spacing ());
	addField (initializeOnly, "horizontal",  horizontal ());
	addField (initializeOnly, "leftToRight", leftToRight ());
	addField (initializeOnly, "topToBottom", topToBottom ());
	addField (initializeOnly, "justify",     justify ());
}

X3DBaseNode*
ScreenFontStyle::create (X3DExecutionContext* const executionContext) const
{
	return new ScreenFontStyle (executionContext);
}

void
ScreenFontStyle::initialize ()
{
	X3DFontStyleNode::initialize ();

	family () .addInterest (&ScreenFontStyle::set_font, this);
	style  () .addInterest (&ScreenFontStyle::set_font, this);

	set_font ();
}

X3DPtr <X3DTextGeometry>
ScreenFontStyle::getTextGeometry (Text* const text) const
{
	return X3DPtr <X3DTextGeometry> (new ScreenText (text, this));
}

double
ScreenFontStyle::getLineHeight () const
{
	return getSize () * spacing ();
}

double
ScreenFontStyle::getSize () const
{
	const double height   = Gdk::Screen::get_default () -> get_height ();             // Screen height in pixel
	const double height_m = Gdk::Screen::get_default () -> get_height_mm () / 1000.0; // Screen height in meter

	return point <double> * pointSize () * height / height_m;
}

void
ScreenFontStyle::set_font ()
{
	font     = createFont ();
	fontFace = font .getFace ();
}

void
ScreenFontStyle::dispose ()
{
	font     .dispose ();
	fontFace .dispose ();

	X3DFontStyleNode::dispose ();
}

ScreenFontStyle::~ScreenFontStyle ()
{ }

} // X3D
} // titania

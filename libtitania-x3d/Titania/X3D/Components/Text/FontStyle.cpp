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

#include "FontStyle.h"

#include "../../Browser/Text/FontStyleOptions.h"
#include "../../Browser/Text/PolygonText.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   FontStyle::component      = Component ("Text", 1);
const std::string FontStyle::typeName       = "FontStyle";
const std::string FontStyle::containerField = "fontStyle";

FontStyle::Fields::Fields () :
	size (new SFFloat (1))
{ }

FontStyle::FontStyle (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DFontStyleNode (),
	          fields (),
	            font (),
	        fontFace (),
	     polygonFont (),
	      lineHeight (1),
	           scale (1)
{
	addType (X3DConstants::FontStyle);

	addField (inputOutput,    "metadata",    metadata ());
	addField (initializeOnly, "language",    language ());
	addField (initializeOnly, "family",      family ());
	addField (initializeOnly, "style",       style ());
	addField (initializeOnly, "size",        size ());
	addField (initializeOnly, "spacing",     spacing ());
	addField (initializeOnly, "horizontal",  horizontal ());
	addField (initializeOnly, "leftToRight", leftToRight ());
	addField (initializeOnly, "topToBottom", topToBottom ());
	addField (initializeOnly, "justify",     justify ());

	size () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
FontStyle::create (X3DExecutionContext* const executionContext) const
{
	return new FontStyle (executionContext);
}

void
FontStyle::initialize ()
{
	X3DFontStyleNode::initialize ();

	getBrowser () -> getFontStyleOptions () -> addInterest (&FontStyle::set_font, this);

	family  () .addInterest (&FontStyle::set_font, this);
	style   () .addInterest (&FontStyle::set_font, this);
	size    () .addInterest (&FontStyle::set_font, this);
	spacing () .addInterest (&FontStyle::set_font, this);

	set_font ();
}

void
FontStyle::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
		getBrowser () -> getFontStyleOptions () -> removeInterest (&FontStyle::set_font, this);

	X3DFontStyleNode::setExecutionContext (executionContext);

	if (isInitialized ())
		getBrowser () -> getFontStyleOptions () -> addInterest (&FontStyle::set_font, this);
}

X3DPtr <X3DTextGeometry>
FontStyle::getTextGeometry (Text* const text) const
{
	return X3DPtr <X3DTextGeometry> (new PolygonText (text, this));
}

void
FontStyle::set_font ()
{
	// Set the font size to large text, otherwise it can crash in some cases and this prevents triangulation errors.
	constexpr double FONT_SIZE = 100;

	// Create a polygon font from a TrueType file.

	polygonFont = getPolygonFont (family ());

	if (not polygonFont -> getError ())
	{
		polygonFont -> setFaceSize (FONT_SIZE);

		// Calculate lineHeight.
		lineHeight = FONT_SIZE * spacing ();

		// Calculate scale.
		scale = size () / FONT_SIZE;
	}
	else
		polygonFont .reset ();

	addEvent ();
}

PolygonFontPtr
FontStyle::getPolygonFont (const MFString & family) const
{
	const size_t bezierDimension = getBrowser () -> getFontStyleOptions () -> bezierDimension ();
	bool         isExactMatch    = false;

	for (const auto & familyName : family)
	{
		const_cast <Font &> (font) = createFont (familyName, isExactMatch);

		if (isExactMatch)
		{
			PolygonFontPtr polygonFont (new FTGL::PolygonFont (font .getFilename (), bezierDimension));

			if (polygonFont -> getError ())
			  continue;

			const_cast <FontFace &> (fontFace) = font .getFace ();

			return polygonFont;
		}
	}

	const_cast <Font &> (font)         = createFont ("SERIF", isExactMatch);
	const_cast <FontFace &> (fontFace) = font .getFace ();

	return PolygonFontPtr (new FTGL::PolygonFont (font .getFilename (), bezierDimension));
}

void
FontStyle::dispose ()
{
	font        .dispose ();
	fontFace    .dispose ();
	polygonFont .reset ();

	X3DFontStyleNode::dispose ();
}

FontStyle::~FontStyle ()
{ }

} // X3D
} // titania

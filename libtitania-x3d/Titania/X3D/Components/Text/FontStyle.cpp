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

#include "FontStyle.h"

#include "../Text/Text.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

PolygonText::PolygonText (Text* const text, const FontStyle* const fontStyle) :
	X3DTextGeometry (),
	           text (text),
	      fontStyle (fontStyle)
{
	if (not fontStyle -> getPolygonFont ())
	{
		text -> origin ()     = Vector3d ();
		text -> textBounds () = Vector2d ();
		text -> lineBounds () .clear (),

		setBBox (Box3d ());

		return;
	}

	initialize (text, fontStyle);

	compile (text);
}

void
PolygonText::getLineBounds (const std::string & line, Vector2d & min, Vector2d & max) const
{
	FTBBox  ftbbox = fontStyle -> getPolygonFont () -> BBox (line .c_str ());
	FTPoint ftmin  = ftbbox .Lower ();
	FTPoint ftmax  = ftbbox .Upper ();

	min = Vector2d (ftmin .X (), ftmin .Y ());
	max = Vector2d (ftmax .X (), ftmax .Y ());
}

void
PolygonText::draw ()
{
	if (not fontStyle -> getPolygonFont ())
		return;

	glPushMatrix ();
	{
		glTranslatef (getMinorAlignment () .x (), getMinorAlignment () .y (), 0);

		double size = fontStyle -> getScale ();

		glScalef (size, size, size);

		// Render lines.
		size_t i = 0;

		for (const auto & line : text -> string ())
		{
			fontStyle -> getPolygonFont () -> Render (line .getValue () .c_str (),
			                                          -1,
			                                          FTPoint (getTranslation () [i] .x (), getTranslation () [i] .y (), 0),
			                                          FTPoint (getCharSpacing () [i], 0, 0),
			                                          FTGL::RENDER_ALL);

			++ i;
		}
	}
	glPopMatrix ();
}

const std::string FontStyle::componentName  = "Text";
const std::string FontStyle::typeName       = "FontStyle";
const std::string FontStyle::containerField = "fontStyle";

FontStyle::Fields::Fields () :
	size (new SFFloat (1))
{ }

FontStyle::FontStyle (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DFontStyleNode (),
	          fields (),
	     polygonFont (),
	      lineHeight (1),
	           scale (1)
{
	addField (inputOutput,    "metadata",    metadata ());
	addField (initializeOnly, "family",      family ());
	addField (initializeOnly, "style",       style ());
	addField (initializeOnly, "size",        size ());
	addField (initializeOnly, "spacing",     spacing ());
	addField (initializeOnly, "horizontal",  horizontal ());
	addField (initializeOnly, "justify",     justify ());
	addField (initializeOnly, "topToBottom", topToBottom ());
	addField (initializeOnly, "leftToRight", leftToRight ());
	addField (initializeOnly, "language",    language ());
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

	family  () .addInterest (this, &FontStyle::set_font);
	style   () .addInterest (this, &FontStyle::set_font);
	size    () .addInterest (this, &FontStyle::set_font);
	spacing () .addInterest (this, &FontStyle::set_font);

	set_font ();
}

std::shared_ptr <X3DTextGeometry>
FontStyle::getTextGeometry (Text* const text) const
{
	return std::shared_ptr <X3DTextGeometry> (new PolygonText (text, this));
}

void
FontStyle::set_font ()
{
	// Create a polygon font from a TrueType file.

	polygonFont = std::move (getPolygonFont (family ()));

	if (polygonFont -> Error () == 0)
	{
		polygonFont -> CharMap (ft_encoding_unicode);
		polygonFont -> UseDisplayList (true);

		// Set the font size to large text.
		polygonFont -> FaceSize (100);

		// Calculate lineHeight.
		lineHeight = polygonFont -> LineHeight () * spacing ();

		// Calculate scale.
		scale = size () / polygonFont -> LineHeight ();
	}
	else
		polygonFont .reset ();
}

PolygonFontPtr
FontStyle::getPolygonFont (const MFString & family) const
{
	bool isExactMatch = false;

	for (const auto & familyName : family)
	{
		Font font = getFont (familyName, isExactMatch);

		if (isExactMatch)
		{
			// Create a pixmap font from a TrueType file.
			PolygonFontPtr polygonFont (new FTPolygonFont (font .getFilename () .c_str ()));

			// Check for errors
			if (polygonFont -> Error () == 0)
				return polygonFont;
		}
	}

	return PolygonFontPtr (new FTPolygonFont (getFont ("SERIF", isExactMatch) .getFilename () .c_str ()));
}

void
FontStyle::dispose ()
{
	polygonFont .reset ();

	X3DFontStyleNode::dispose ();
}

} // X3D
} // titania

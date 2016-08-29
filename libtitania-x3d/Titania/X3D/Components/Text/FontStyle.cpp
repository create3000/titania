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

#include "../../Execution/X3DExecutionContext.h"
#include "../Text/Text.h"

namespace titania {
namespace X3D {

PolygonText::PolygonText (Text* const text, const FontStyle* const fontStyle) :
	X3DTextGeometry (fontStyle),
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
PolygonText::getLineExtents (const String & line, Vector2d & min, Vector2d & max) const
{
	const auto ftbbox = fontStyle -> getPolygonFont () -> getBBox (line .c_str ());
	const auto ftmin  = ftbbox .getLower ();
	const auto ftmax  = ftbbox .getUpper ();

	min = Vector2d (ftmin .x (), ftmin .y ());
	max = Vector2d (ftmax .x (), ftmax .y ());
}

void
PolygonText::draw ()
{
	if (not fontStyle -> getPolygonFont ())
		return;

	const double size = fontStyle -> getScale ();

	glTranslatef (getMinorAlignment () .x (), getMinorAlignment () .y (), 0);
	glScalef (size, size, size);

	// Triangulate lines.

	auto indices = std::vector <size_t> ();
	auto points  = std::vector <Vector3d> ();

	if (fontStyle -> horizontal ())
	{
		const bool    topToBottom = fontStyle -> topToBottom ();
		const bool    leftToRight = fontStyle -> leftToRight ();
		const int32_t first       = topToBottom ? 0 : text -> string () .size () - 1;
		const int32_t last        = topToBottom ? text -> string () .size () : -1;
		const int32_t step        = topToBottom ? 1 : -1;

		for (int32_t i = first; i not_eq last; i += step)
		{
			const auto & line = text -> string () [i] .getValue ();

			fontStyle -> getPolygonFont () -> triangulate (leftToRight
	                                                     ? line
	                                                     : String (line .rbegin (), line .rend ()),
			                                               FTGL::Vector3d (getTranslations () [i] .x (), getTranslations () [i] .y (), 0),
			                                               FTGL::Vector3d (getCharSpacing () [i], 0, 0),
			                                               indices,
			                                               points);
		}
	}
	else
	{
		const bool    leftToRight = fontStyle -> leftToRight ();
		const bool    topToBottom = fontStyle -> topToBottom ();
		const int32_t first       = leftToRight ? 0 : text -> string () .size () - 1;
		const int32_t last        = leftToRight ? text -> string () .size () : -1;
		const int32_t step        = leftToRight ? 1 : -1;

		for (int32_t i = first, g = 0; i not_eq last; i += step)
		{
			const auto & line = text -> string () [i] .getValue ();

			for (const auto & glyph : topToBottom ? line : String (line .rbegin (), line .rend ()))
			{
				fontStyle -> getPolygonFont () -> triangulate (String (1, glyph),
				                                               FTGL::Vector3d (getTranslations () [g] .x (), getTranslations () [g] .y (), 0),
				                                               FTGL::Vector3d (),
			                                                  indices,
			                                                  points);
				++ g;
			}
		}
	}

	// Render lines.

	glNormal3f (0, 0, 1);
	glBegin (GL_TRIANGLES);

	for (const auto index : indices)
		glVertex3dv (points [index] .data ());

	glEnd ();
}

const ComponentType FontStyle::component      = ComponentType::TEXT;
const std::string   FontStyle::typeName       = "FontStyle";
const std::string   FontStyle::containerField = "fontStyle";

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

	family  () .addInterest (this, &FontStyle::set_font);
	style   () .addInterest (this, &FontStyle::set_font);
	size    () .addInterest (this, &FontStyle::set_font);
	spacing () .addInterest (this, &FontStyle::set_font);

	set_font ();
}

std::unique_ptr <X3DTextGeometry>
FontStyle::getTextGeometry (Text* const text) const
{
	return std::unique_ptr <X3DTextGeometry> (new PolygonText (text, this));
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
}

PolygonFontPtr
FontStyle::getPolygonFont (const MFString & family) const
{
	bool isExactMatch = false;

	for (const auto & familyName : family)
	{
		const_cast <Font &> (font) = createFont (familyName, isExactMatch);

		if (isExactMatch)
		{
			PolygonFontPtr polygonFont (new FTGL::PolygonFont (font .getFilename () .c_str ()));

			if (not polygonFont -> getError ())
			{
				const_cast <FontFace &> (fontFace) = font .getFace ();

				return polygonFont;
			}
		}
	}

	const_cast <Font &> (font)         = createFont ("SERIF", isExactMatch);
	const_cast <FontFace &> (fontFace) = font .getFace ();

	return PolygonFontPtr (new FTGL::PolygonFont (font .getFilename () .c_str ()));
}

void
FontStyle::dispose ()
{
	font        .dispose ();
	fontFace    .dispose ();
	polygonFont .reset ();

	X3DFontStyleNode::dispose ();
}

} // X3D
} // titania

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
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Geometry3D/IndexedFaceSet.h"
#include "../Rendering/Coordinate.h"
#include "../Texturing/TextureCoordinate.h"
#include "../Text/Text.h"

namespace titania {
namespace X3D {

const Component   PolygonText::component      = Component ("Titania", 1);
const std::string PolygonText::typeName       = "PolygonText";
const std::string PolygonText::containerField = "textGeometry";

PolygonText::PolygonText (Text* const text, const FontStyle* const fontStyle) :
	    X3DBaseNode (text -> getBrowser (), text -> getExecutionContext ()),
	X3DTextGeometry (text, fontStyle),
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

	initialize ();
	build ();
}

X3DBaseNode*
PolygonText::create (X3DExecutionContext* const executionContext) const
{
	return new PolygonText (getText (), fontStyle);
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
PolygonText::triangulate (std::vector <size_t> & indices, std::vector <Vector3d> & points) const
{
	if (not fontStyle -> getPolygonFont ())
		return;

	if (fontStyle -> horizontal ())
	{
		const bool    topToBottom = fontStyle -> topToBottom ();
		const bool    leftToRight = fontStyle -> leftToRight ();
		const int32_t first       = topToBottom ? 0 : getText () -> string () .size () - 1;
		const int32_t last        = topToBottom ? getText () -> string () .size () : -1;
		const int32_t step        = topToBottom ? 1 : -1;

		for (int32_t i = first; i not_eq last; i += step)
		{
			const auto & line = getText () -> string () .get1Value (i);

			fontStyle -> getPolygonFont () -> triangulate (leftToRight
	                                                     ? line
	                                                     : String (line .rbegin (), line .rend ()),
			                                               Vector3d (getTranslations () [i] .x (), getTranslations () [i] .y (), 0),
			                                               Vector3d (getCharSpacing () [i], 0, 0),
			                                               indices,
			                                               points);
		}
	}
	else
	{
		const bool    leftToRight = fontStyle -> leftToRight ();
		const bool    topToBottom = fontStyle -> topToBottom ();
		const int32_t first       = leftToRight ? 0 : getText () -> string () .size () - 1;
		const int32_t last        = leftToRight ? getText () -> string () .size () : -1;
		const int32_t step        = leftToRight ? 1 : -1;

		for (int32_t i = first, g = 0; i not_eq last; i += step)
		{
			const auto & line = getText () -> string () .get1Value (i);

			for (const auto & glyph : topToBottom ? line : String (line .rbegin (), line .rend ()))
			{
				fontStyle -> getPolygonFont () -> triangulate (String (1, glyph),
				                                               Vector3d (getTranslations () [g] .x (), getTranslations () [g] .y (), 0),
				                                               Vector3d (),
			                                                  indices,
			                                                  points);
				++ g;
			}
		}
	}
}

void
PolygonText::build ()
{
	// Triangulate lines.

	auto indices = std::vector <size_t> ();
	auto points  = std::vector <Vector3d> ();

	triangulate (indices, points);

	// Create geometry

	const auto   size        = fontStyle -> getScale ();
	const auto   spacing     = fontStyle -> spacing () .getValue ();
	const auto & origin      = getText () -> origin () .getValue ();
	const auto   translation = Vector3d (getMinorAlignment () .x (), getMinorAlignment () .y (), 0);
	const auto   scale       = Vector3d (size, size, size);

	getText () -> getMultiTexCoords () .emplace_back ();

	auto & texCoords = getText () -> getTexCoords ();
	auto & normals   = getText () -> getNormals ();
	auto & vertices  = getText () -> getVertices ();

	for (const auto index : indices)
	{
		const auto & point = points [index] * scale + translation;

		texCoords .emplace_back ((point .x () - origin .x ()) / spacing, (point .y () - origin .y ()) / spacing, 0, 1);
		normals   .emplace_back (0, 0, 1);
		vertices  .emplace_back (point);
	}

	getText () -> addElements (GL_TRIANGLES, vertices .size ());
	getText () -> setSolid (getText () -> solid ());
}

SFNode
PolygonText::toPrimitive () const
{
	// Triangulate lines.

	auto indices = std::vector <size_t> ();
	auto points  = std::vector <Vector3d> ();

	triangulate (indices, points);

	// Create geometry

	const auto   size        = fontStyle -> getScale ();
	const auto   spacing     = fontStyle -> spacing () .getValue ();
	const auto & origin      = getText () -> origin () .getValue ();
	const auto   translation = Vector3d (getMinorAlignment () .x (), getMinorAlignment () .y (), 0);
	const auto   scale       = Vector3d (size, size, size);

	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> solid ()    = getText () -> solid ();
	geometry -> texCoord () = texCoord;
	geometry -> coord ()    = coord;

	for (const auto & p : points)
	{
		const auto point = p * scale + translation;

		texCoord -> point () .emplace_back ((point .x () - origin .x ()) / spacing, (point .y () - origin .y ()) / spacing);
		coord    -> point () .emplace_back (point);
	}

	for (size_t i = 0, size = indices .size (); i < size; i += 3)
	{
		geometry -> coordIndex () .emplace_back (indices [i + 0]);
		geometry -> coordIndex () .emplace_back (indices [i + 1]);
		geometry -> coordIndex () .emplace_back (indices [i + 2]);
		geometry -> coordIndex () .emplace_back (-1);
	}

	geometry -> texCoordIndex () = geometry -> coordIndex ();

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

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

	addField (inputOutput, "metadata",    metadata ());
	addField (inputOutput, "language",    language ());
	addField (inputOutput, "family",      family ());
	addField (inputOutput, "style",       style ());
	addField (inputOutput, "size",        size ());
	addField (inputOutput, "spacing",     spacing ());
	addField (inputOutput, "horizontal",  horizontal ());
	addField (inputOutput, "leftToRight", leftToRight ());
	addField (inputOutput, "topToBottom", topToBottom ());
	addField (inputOutput, "justify",     justify ());

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

} // X3D
} // titania

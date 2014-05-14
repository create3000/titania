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

#include "X3DFontStyleNode.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Text/Text.h"

#include <Titania/OS/file_exists.h>
#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

X3DTextGeometry::X3DTextGeometry () :
	          bbox (),
	  charSpacings (),
	       bearing (),
	minorAlignment (),
	  translations (),
	        listId (glGenLists (1))
{ }

void
X3DTextGeometry::initialize (Text* const text, const X3DFontStyleNode* const fontStyle)
{
	const size_t numLines = text -> string () .size ();

	text -> lineBounds () .resize (numLines);
	charSpacings .resize (numLines);
	translations .resize (numLines);

	if (numLines == 0)
	{
		text -> origin ()     = Vector3d ();
		text -> textBounds () = Vector2d ();

		setBBox (Box3d ());

		return;
	}

	if (fontStyle -> horizontal ())
		horizontal (text, fontStyle);
	else
		vertical (text, fontStyle);
}

void
X3DTextGeometry::horizontal (Text* const text, const X3DFontStyleNode* const fontStyle)
{
	Box2d bbox;

	const size_t numLines    = text -> string () .size ();
	const bool   topToBottom = fontStyle -> topToBottom ();
	const double lineHeight  = fontStyle -> getLineHeight ();
	const double scale       = fontStyle -> getScale ();

	// Calculate bboxes.

	const int first = topToBottom ? 0 : numLines - 1;
	const int last  = topToBottom ? numLines : -1;
	const int step  = topToBottom ? 1 : -1;

	for (int l = first; l not_eq last; l += step)
	{
		const auto & line = text -> string () [l];

		// Get line extents.

		Vector2d min, max;
		getLineExtents (fontStyle, line .getValue (), min, max);

		Vector2d size = max - min;

		// Calculate charSpacing and lineBounds.

		const size_t lineNumber = topToBottom ? l : numLines - l - 1;

		double   charSpacing = 0;
		Vector2d lineBound   = Vector2d (size .x (), lineNumber == 0 ? size .y () : lineHeight) * scale;
		double   length      = text -> getLength (l);

		if (text -> maxExtent ())
		{
			if (length)
				length = std::min <double> (text -> maxExtent (), length);

			else
				length = std::min <double> (text -> maxExtent (), size .x () * scale);
		}

		if (length)
		{
			charSpacing = (length - lineBound .x ()) / (line .length () - 1) / scale;
			lineBound .x (length);
			size .x (length / scale);
		}

		charSpacings [l]          = charSpacing;
		text -> lineBounds () [l] = lineBound;

		// Calculate line translation.

		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				translations [l] = Vector2d (0, -(lineNumber * lineHeight));
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				translations [l] = Vector2d (-min .x () - size .x () / 2, -(lineNumber * lineHeight));
				break;
			case X3DFontStyleNode::Alignment::END:
				translations [l] = Vector2d (-min .x () - size .x (), -(lineNumber * lineHeight));
				break;
		}

		// Calculate center.

		const Vector2d center = min + size / 2.0;

		// Add bbox.

		bbox += Box2d (size * scale, (center + translations [l]) * scale);
	}

	// Get text extents.

	Vector2d min, max;
	bbox .extents (min, max);

	const Vector2d size = max - min;

	// Calculate text position

	text -> textBounds () = size;
	bearing               = Vector2d (0, -max .y ());

	switch (fontStyle -> getMinorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
			minorAlignment = bearing;
			break;
		case X3DFontStyleNode::Alignment::FIRST:
			minorAlignment = Vector2d ();
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			minorAlignment = Vector2d (0, size .y () / 2 - max .y ());
			break;
		case X3DFontStyleNode::Alignment::END:
			minorAlignment = Vector2d (0, (numLines - 1) * lineHeight * scale);
			break;
	}

	// Translate bbox by minorAlignment.

	min += minorAlignment;
	max += minorAlignment;

	// The value of the origin field represents the upper left corner of the textBounds.

	text -> origin () = Vector3d (min .x (), max .y (), 0);

	setBBox (Box3d (Vector3d (min .x (), min .y (), 0),
	                Vector3d (max .x (), max .y (), 0),
	                extents_type ()));
}

void
X3DTextGeometry::vertical (Text* const text, const X3DFontStyleNode* const fontStyle)
{
	size_t numChars = 0;

	for (const auto & line : text -> string ())
		numChars += line .length ();

	charSpacings .resize (numChars);
	translations .resize (numChars);

	std::vector <double> yPad (numChars);

	//

	Box2d bbox;

	const size_t numLines    = text -> string () .size ();
	const bool   leftToRight = fontStyle -> leftToRight ();
	const bool   topToBottom = fontStyle -> topToBottom ();
	const double lineHeight  = fontStyle -> getLineHeight ();
	const double scale       = fontStyle -> getScale ();
	const double height      = lineHeight / fontStyle -> spacing ();

	// Calculate bboxes.

	const int first = leftToRight ? 0 : numLines - 1;
	const int last  = leftToRight ? numLines : -1;
	const int step  = leftToRight ? 1 : -1;
	size_t    t     = 0; // Translation index

	for (int l = first; l not_eq last; l += step)
	{
		const auto & line = text -> string () [l] .getValue ();
		
		const int    t0       = t;
		const size_t numChars = line .length ();

		// Calculate line bbox

		Box2d lineBBox;

		const int first = topToBottom ? 0 : numChars - 1;
		const int last  = topToBottom ? numChars : -1;
		const int step  = topToBottom ? 1 : -1;

		for (int g = first; g not_eq last; g += step)
		{
			const auto & glyph = line [g];

			// Get glyph extents.

			Vector2d min, max;
			getGlyphExtents (glyph, min, max);

			const Vector2d size = max - min;
			
			// Calculate glyph translation
			
			const int glyphNumber = topToBottom ? g : numChars - g - 1;

			translations [t] = Vector2d ((lineHeight - size .x ()) / 2, -(glyphNumber * height));
			
			// Calculate center.

			const Vector2d center = min + size / 2.0 + translations [t];

			// Add bbox.

			lineBBox += Box2d (size, center);

			// Incement translation index

			++ t;
		}
	
		// Get line extents.

		Vector2d min, max;
		lineBBox .extents (min, max);

		Vector2d size = max - min;

		// Calculate charSpacing and lineBounds.

		const size_t lineNumber = leftToRight ? l : numLines - l - 1;
		const double padding    = (lineHeight - size .x ()) / 2;

		double   charSpacing = 0;
		Vector2d lineBound   = Vector2d (l == 0 ? size .x () + padding : lineHeight, size .y ()) * scale;
		double   length      = text -> getLength (l);

		if (text -> maxExtent ())
		{
			if (length)
				length = std::min <double> (text -> maxExtent (), length);

			else
				length = std::min <double> (text -> maxExtent (), size .y () * scale);
		}

		if (length)
		{
			charSpacing = (length - lineBound .y ()) / (line .length () - 1) / scale;
			lineBound .y (length);
			size .y (length / scale);
			min .y (max .y () - size .y ());
		}

		text -> lineBounds () [l] = lineBound;

		// Calculate line translation.
		
		Vector2d translation;

		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				translation = Vector2d (lineNumber * lineHeight, -height);
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				translation = Vector2d (lineNumber * lineHeight, size .y () / 2 - max .y ());
				break;
			case X3DFontStyleNode::Alignment::END:
			{
				// This is needed to make maxExtend and charSpacing work.
				Vector2d glyphMin, v;
				getGlyphExtents (line [topToBottom ? numChars - 1 : 0], glyphMin, v);	
				translation = Vector2d (lineNumber * lineHeight, size .y () - max .y () + glyphMin .y ());
				break;
			}
		}

		// Calculate glyph translation		

		double space = 0;

		for (auto & glyph : basic::adapter (translations .begin () + t0, translations .begin () + t))
		{
			glyph += translation;

			glyph .y (glyph .y () - space);
			space += charSpacing;
		}

		// Calculate center.

		const Vector2d center = min + size / 2.0;

		// Add bbox.
			
		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				yPad [l] = max .y () + translation .y ();
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				break;
			case X3DFontStyleNode::Alignment::END:
				yPad [l] = min .y () + translation .y ();
				break;
		}

		bbox += Box2d (size * scale, (center + translation) * scale);
	}

	// Get text extents.

	Vector2d min, max;
	bbox .extents (min, max);

	const Vector2d size = max - min;
	
	// Extend lineBounds.
	
	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
		{
			size_t l = 0;
			
			for (auto & lineBound : text -> lineBounds ())
				lineBound .setY (lineBound .getY () + max .y () - yPad [l ++] * scale);

			break;
		}
		case X3DFontStyleNode::Alignment::MIDDLE:
			break;
		case X3DFontStyleNode::Alignment::END:
		{
			size_t l = 0;
			
			for (auto & lineBound : text -> lineBounds ())
				lineBound .setY (lineBound .getY () + yPad [l ++] * scale - min .y ());

			break;
		}
	}

	// Calculate text position

	text -> textBounds () = size;

	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			bearing = Vector2d (-min .x (), max .y ());
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			bearing = Vector2d (-min .x (), 0);
			break;
		case X3DFontStyleNode::Alignment::END:
			bearing = Vector2d (-min .x (), min .y ());
			break;
	}

	switch (fontStyle -> getMinorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			minorAlignment = Vector2d (-min .x (), 0);
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			minorAlignment = Vector2d (-min .x () - size .x () / 2, 0);
			break;
		case X3DFontStyleNode::Alignment::END:
			minorAlignment = Vector2d (-min .x () - size .x (), 0);
			break;
	}

	// Translate bbox by minorAlignment.

	min += minorAlignment;
	max += minorAlignment;
	
	// The value of the origin field represents the upper left corner of the textBounds.

	text -> origin () = Vector3d (min .x (), max .y (), 0);

	setBBox (Box3d (Vector3d (min .x (), min .y (), 0),
	                Vector3d (max .x (), max .y (), 0),
	                extents_type ()));
}

void
X3DTextGeometry::getLineExtents (const X3DFontStyleNode* const fontStyle, const String & line, Vector2d & min, Vector2d & max) const
{
	const bool normal = fontStyle -> horizontal () ? fontStyle -> leftToRight () : fontStyle -> topToBottom ();

	if (not line .empty ())
		getLineExtents (normal ? line : String (line .rbegin (), line .rend ()), min, max);

	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			min .x (0);
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
		case X3DFontStyleNode::Alignment::END:
			break;
	}
}

void
X3DTextGeometry::getGlyphExtents (const String::value_type & glyph, Vector2d & min, Vector2d & max) const
{
	getLineExtents (String (1, glyph), min, max);
}

void
X3DTextGeometry::compile (Text* const text)
{
	glNewList (listId, GL_COMPILE);

	if (text -> solid ())
		glEnable (GL_CULL_FACE);

	else
		glDisable (GL_CULL_FACE);

	draw ();

	glEndList ();
}

void
X3DTextGeometry::display ()
{
	glFrontFace (ModelViewMatrix4f () .determinant3 () > 0 ? GL_CCW : GL_CW);

	if (listId)
		glCallList (listId);
}

X3DTextGeometry::~X3DTextGeometry ()
{
	if (listId)
		glDeleteLists (listId, 1);
}

X3DFontStyleNode::Fields::Fields () :
	   language (new SFString ()),
	     family (new MFString ({ "SERIF" })),
	      style (new SFString ("PLAIN")),
	    spacing (new SFFloat (1)),
	 horizontal (new SFBool (true)),
	leftToRight (new SFBool (true)),
	topToBottom (new SFBool (true)),
	    justify (new MFString ({ "BEGIN" }))
{ }

X3DFontStyleNode::X3DFontStyleNode () :
	   X3DNode (),
	    fields (),
	    italic (false),
	      bold (false),
	alignments ({ Alignment::BEGIN, Alignment::FIRST })
{
	addNodeType (X3DConstants::X3DFontStyleNode);
}

void
X3DFontStyleNode::initialize ()
{
	X3DNode::initialize ();

	style ()       .addInterest (this, &X3DFontStyleNode::set_style);
	horizontal ()  .addInterest (this, &X3DFontStyleNode::set_justify);
	leftToRight () .addInterest (this, &X3DFontStyleNode::set_justify);
	topToBottom () .addInterest (this, &X3DFontStyleNode::set_justify);
	justify ()     .addInterest (this, &X3DFontStyleNode::set_justify);

	set_style ();
	set_justify ();
}

X3DFontStyleNode::Alignment
X3DFontStyleNode::getAlignment (const size_t index, const bool normal) const
{
	if (normal)
	{
		// Return for west-european normal alignment.

		static const std::map <std::string, Alignment> alignments = {
			std::make_pair ("BEGIN",  Alignment::BEGIN),
			std::make_pair ("FIRST",  Alignment::FIRST),
			std::make_pair ("MIDDLE", Alignment::MIDDLE),
			std::make_pair ("END",    Alignment::END)
		};

		const auto alignment = alignments .find (justify () [index]);

		if (alignment not_eq alignments .end ())
			return alignment -> second;
	}
	else
	{
		// Return appropriate alignment if topToBottom or leftToRight are FALSE.

		static const std::map <std::string, Alignment> ralignments = {
			std::make_pair ("BEGIN",  Alignment::END),
			std::make_pair ("FIRST",  Alignment::END),
			std::make_pair ("MIDDLE", Alignment::MIDDLE),
			std::make_pair ("END",    Alignment::BEGIN)
		};

		const auto ralignment = ralignments .find (justify () [index]);

		if (ralignment not_eq ralignments .end ())
			return ralignment -> second;
	}

	return index ? Alignment::FIRST : Alignment::BEGIN;
}

Font
X3DFontStyleNode::getFont () const
{
	bool isExactMatch = false;

	for (const auto & familyName : family ())
	{
		const Font font = getFont (familyName, isExactMatch);

		if (isExactMatch)
			return font;
	}

	return getFont ("SERIF", isExactMatch);
}

Font
X3DFontStyleNode::getFont (const String & familyName, bool & isExactMatch) const
{
	// Test if familyName is a valid path local path.
	// TODO: add support for network paths.

	const basic::uri uri = getExecutionContext () -> getWorldURL () .transform (familyName .raw ());

	if (uri .is_local ())
	{
		if (os::file_exists (uri .path ()))
		{
			isExactMatch = true;

			Font font;
			font .setFilename (uri .path ());
			font .substitute ();
			return font;
		}
	}

	Font font;
	font .setFamilyName (familyName == "TYPEWRITER" ? "monospace" : familyName);
	font .setWeight (isBold ()   ? Font::Weight::BOLD  : Font::Weight::NORMAL);
	font .setSlant  (isItalic () ? Font::Slant::ITALIC : Font::Slant::ROMAN);
	font .setScalable (true);
	font .substitute ();

	const Font match = font .match ();

	isExactMatch = (font == match);

	return match;
}

void
X3DFontStyleNode::set_style ()
{
	italic = (style () == "ITALIC" or style () == "BOLDITALIC");
	bold   = (style () == "BOLD"   or style () == "BOLDITALIC");
}

void
X3DFontStyleNode::set_justify ()
{
	const bool majorNormal = horizontal () ? leftToRight () : topToBottom ();

	alignments [0] = justify () .size () > 0
	                 ? getAlignment (0, majorNormal)
						  : majorNormal ? Alignment::BEGIN : Alignment::END;

	const bool minorNormal = horizontal () ? topToBottom () : leftToRight ();

	alignments [1] = justify () .size () > 1
	                 ? getAlignment (1, minorNormal)
						  : minorNormal ? Alignment::FIRST : Alignment::END;
}

} // X3D
} // titania

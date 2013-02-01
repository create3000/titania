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

#include "Text.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

Text::Text (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	         string (),                                                    // MFString [in,out] string      [ ]
	         length (),                                                    // MFFloat  [in,out] length      [ ]          [0,∞)
	      maxExtent (),                                                    // SFFloat  [in,out] maxExtent   0.0          [0,∞)
	         origin (),                                                    // SFVec3f  [out]    origin
	     textBounds (),                                                    // SFVec2f  [out]    textBounds
	     lineBounds (),                                                    // MFVec2f  [out]    lineBounds
	      fontStyle (),                                                    // SFNode   [in,out] fontStyle   NULL         [X3FontStyleNode]
	           font (),                                                    
	     lineHeight (),                                                    
	   charSpacings (),                                                    
	 minorAlignment (),                                                    
	    translation (),                                                    
	          scale ()                                                     
{
	setComponent ("Text");
	setTypeName ("Text");

	addField (inputOutput,    "metadata",   metadata);
	addField (inputOutput,    "string",     string);
	addField (inputOutput,    "length",     length);
	addField (inputOutput,    "maxExtent",  maxExtent);
	addField (initializeOnly, "solid",      solid);
	addField (outputOnly,     "origin",     origin);
	addField (outputOnly,     "textBounds", textBounds);
	addField (outputOnly,     "lineBounds", lineBounds);
	addField (inputOutput,    "fontStyle",  fontStyle);
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

	fontStyle .addInterest (this, &Text::set_fontStyle);

	set_fontStyle ();
}

float
Text::getLength (const size_t index)
{
	if (index < length .size ())
		return length [index];

	return 0;
}

const SFNode <X3DFontStyleNode> &
Text::getFontStyle () const
{
	if (this -> fontStyle)
		return this -> fontStyle;

	return getBrowser () -> getBrowserOptions () -> fontStyle;
}

void
Text::set_fontStyle ()
{
	const SFNode <X3DFontStyleNode> & fontStyle = getFontStyle ();

	// Create a pixmap font from a TrueType file.
	font .reset (new FTPolygonFont (fontStyle -> getFilename () .c_str ()));

	// If something went wrong, bail out.
	if (font -> Error ())
		return;

	font -> CharMap (ft_encoding_unicode);
	font -> UseDisplayList (true);

	// Set the font size to large text.
	font -> FaceSize (100);

	// Calculate lineHeight.
	lineHeight = font -> LineHeight () * fontStyle -> spacing;

	// Calculate scale.
	scale = fontStyle -> getSize () / font -> LineHeight ();
}

Box3f
Text::createBBox ()
{
	const SFNode <X3DFontStyleNode> & fontStyle = getFontStyle ();

	Box2f bbox;

	float y1 = 0;

	charSpacings .clear ();
	lineBounds   .clear ();
	translation  .clear ();

	// Calculate BBoxes.
	size_t i = 0;

	for (const auto & line : string)
	{
		Box2f            lineBBox = getLineBBox (line .getValue ());
		const Vector2f & min      = lineBBox .min ();
		const Vector2f & max      = lineBBox .max ();

		if (i == 1)
			y1 = max .y ();

		Vector2f size = lineBBox .size ();

		// Calculate charSpacing and lineBounds.

		float    charSpacing = 0;
		Vector2f lineBound   = Vector2f (size .x (), lineHeight) * scale;
		float    length      = getLength (i);

		if (length)
		{
			charSpacing = (length - lineBound .x ()) / (line .length () - 1) / scale;
			lineBound .x (length);
			size .x (length / scale);
		}

		charSpacings .emplace_back (charSpacing);
		lineBounds   .emplace_back (lineBound);

		// Calculate line translation.

		switch (fontStyle -> getMajorAlignment ())
		{
			case FontStyle::Alignment::BEGIN:
			case FontStyle::Alignment::FIRST:
				translation .emplace_back (0, -(i * lineHeight));
				break;
			case FontStyle::Alignment::MIDDLE:
				translation .emplace_back (-min .x () - size .x () / 2, -(i * lineHeight));
				break;
			case FontStyle::Alignment::END:
				translation .emplace_back (-min .x () - size .x (), -(i * lineHeight));
				break;
		}

		// Calculate center.

		Vector2f center = Vector2f (min .x (), min .y ()) + size / 2.0f;

		// Add bbox.

		bbox += Box2f (size, center + translation [i]) * scale;

		++ i;
	}

	textBounds .setValue (bbox .size ());

	if (string .size () > 1)
	{
		lineBounds .front () .setY (bbox .max () .y () + (lineHeight - y1) * scale);
		lineBounds .back  () .setY (textBounds .getY () - (lineBounds [0] .getY () + (string .size () - 2) * fontStyle -> spacing));
	}

	switch (fontStyle -> getMinorAlignment ())
	{
		case FontStyle::Alignment::BEGIN:
			minorAlignment = Vector2f (0, -bbox .max () .y ());
			break;
		case FontStyle::Alignment::FIRST:
			minorAlignment = Vector2f ();
			break;
		case FontStyle::Alignment::MIDDLE:
			minorAlignment = Vector2f (0, textBounds .getY () / 2 - bbox .max () .y ());
			break;
		case FontStyle::Alignment::END:
			minorAlignment = Vector2f  (0, textBounds .getY () - bbox .max () .y ());
			break;
	}

	bbox += minorAlignment;

	origin .setValue (Vector3f (bbox .min () .x (), bbox .max () .y (), 0));

	return Box3f (Vector3f (bbox .min () .x (), bbox .min () .y (), 0),
	              Vector3f (bbox .max () .x (), bbox .max () .y (), 0),
	              true);
}

Box2f
Text::getLineBBox (const std::string & line)
{
	const SFNode <X3DFontStyleNode> & fontStyle = getFontStyle ();

	FTBBox  ftbbox = font -> BBox (line .c_str ());
	FTPoint min    = ftbbox .Lower ();
	FTPoint max    = ftbbox .Upper ();

	switch (fontStyle -> getMajorAlignment ())
	{
		case FontStyle::Alignment::BEGIN:
		case FontStyle::Alignment::FIRST:
			return Box2f (Vector2f (0, min .Y ()), Vector2f (max .X (), max .Y ()), true);

		case FontStyle::Alignment::MIDDLE:
		case FontStyle::Alignment::END:
			return Box2f (Vector2f (min .X (), min .Y ()), Vector2f (max .X (), max .Y ()), true);
	}

	// Control never reaches this line.
	return Box2f ();
}

void
Text::display ()
{
	if (solid)
		glEnable (GL_CULL_FACE);

	else
		glDisable (GL_CULL_FACE);

	// Calculate translations and lineBounds if needed.
	getBBox ();

	glTranslatef (minorAlignment .x (), minorAlignment .y (), 0);
	glScalef (scale, scale, scale);

	// Render lines.
	size_t i = 0;

	for (const auto & line : string)
	{
		font -> Render (line .getValue () .c_str (),
		                -1,
		                FTPoint (translation [i] .x (), translation [i] .y (), 0),
		                FTPoint (charSpacings [i], 0, 0),
		                FTGL::RENDER_ALL);

		++ i;
	}
}

void
Text::dispose ()
{
	font .reset (NULL);

	X3DGeometryNode::dispose ();
}

} // X3D
} // titania

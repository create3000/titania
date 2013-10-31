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

#include <gdkmm.h>

#include "ScreenFontStyle.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Navigation/X3DViewpointNode.h"
#include "../Text/Text.h"

#include <Titania/Physics/Constants.h>

namespace titania {
namespace X3D {

static constexpr double M_POINT = M_INCH / 72;

ScreenText::ScreenText (Text* const text, const ScreenFontStyle* const fontStyle) :
	X3DTextGeometry (),
	           text (text),
	      fontStyle (fontStyle),
	        context (Cairo::Context::create (Cairo::ImageSurface::create (Cairo::FORMAT_RGB24, 0, 0))),
	      textureId (0),
	            min (),
	            max ()
{
	glGenTextures (1, &textureId);

	configure (context);
	initialize (text, fontStyle);
	setTextBounds ();

	build ();
	compile (text);
}

void
ScreenText::configure (const Cairo::RefPtr <Cairo::Context> & context)
{
	ScreenFontPtr font = fontStyle -> getScreenFont ();
	auto fontFace = Cairo::FtFontFace::create (font -> getPattern () .get ());

	context -> set_font_face (fontFace);
	context -> set_font_size (fontStyle -> getSize ());
	
	Cairo::FontOptions options;
	options .set_hint_style (Cairo::HINT_STYLE_MEDIUM);

	context -> set_font_options (options);
}

void
ScreenText::getLineBounds (const std::string & line, Vector2d & min, Vector2d & max) const
{
	Cairo::TextExtents textExtents;

	context -> get_text_extents (line, textExtents);

	min = Vector2d (textExtents .x_bearing, -textExtents .y_bearing - textExtents .height);
	max = min + Vector2d (textExtents .width, textExtents .height);
}

void
ScreenText::setTextBounds ()
{
	text -> textBounds () = math::ceil (text -> textBounds () .getValue ());

	Box3d bbox = getBBox ();

	Vector3d size    = bbox .size ();
	Vector3d size1_2 = size / 2.0;
	Vector3d center  = bbox .center ();

	min = center - size1_2;
	max = center + size1_2;

	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			max .x (min .x () + text -> textBounds () .getX ());
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			min .x (std::round (min .x ()));
			max .x (min .x () + text -> textBounds () .getX ());
			break;
		case X3DFontStyleNode::Alignment::END:
			min .x (max .x () - text -> textBounds () .getX ());
			break;
	}

	switch (fontStyle -> getMinorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			min .y (max .y () - text -> textBounds () .getY ());
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			max .y (std::round (max .y ()));
			min .y (max .y () - text -> textBounds () .getY ());
			break;
		case X3DFontStyleNode::Alignment::END:
			max .y (min .y () + text -> textBounds () .getY ());
			break;
	}

	text -> origin () = Vector3d (min .x (), max .y (), 0);

	setBBox (Box3d (min, max, true));
}

void
ScreenText::build ()
{
	// Create context

	auto surface = Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
	                                            text -> textBounds () .getX (),
	                                            text -> textBounds () .getY ());

	auto context = Cairo::Context::create (surface);

	configure (context);
	context -> set_source_rgb (1, 1, 1);

	// Render lines.

	Vector2d alignment = getBearing ();

	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			alignment += Vector2d (text -> textBounds () .getX () / 2, 0);
			break;
		case X3DFontStyleNode::Alignment::END:
			alignment += Vector2d (text -> textBounds () .getX (), 0);
			break;
	}

	auto                             font = context -> get_scaled_font ();
	std::vector <Cairo::Glyph>       glyphs;
	std::vector <Cairo::TextCluster> clusters;
	Cairo::TextClusterFlags          cluster_flags;

	size_t i = 0;

	for (const auto & line : text -> string ())
	{
		try
		{
			if (not line .empty ())
			{
				double x = alignment .x () + getTranslation () [i] .x ();
				double y = -(alignment .y () + getTranslation () [i] .y ());

				font -> text_to_glyphs (x, y, line, glyphs, clusters, cluster_flags);

				double space       = 0;
				double charSpacing = getCharSpacing () [i];

				for (auto & glyph : glyphs)
				{
					glyph .x += space;
					space    += charSpacing;
				}

				context -> show_text_glyphs (line, glyphs, clusters, cluster_flags);
			}
		}
		catch (...)
		{ }

		++ i;
	}

	//if (surface -> get_width () and surface -> get_height ())
	//	surface -> write_to_png ("/home/holger/test.png");

	// Set RGB to white

	unsigned char* first = surface -> get_data ();
	unsigned char* last  = first + 4 * surface -> get_width () * surface -> get_height ();

	while (first < last)
	{
		*first ++ = 255;
		*first ++ = 255;
		*first ++ = 255;
		++ first;
	}

	// Upload texture

	glBindTexture (GL_TEXTURE_2D, textureId);

	glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    false);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP);

	glTexImage2D (GL_TEXTURE_2D,
	              0,     // This texture is level 0 in mimpap generation.
	              GL_RGBA,
	              surface -> get_width (), surface -> get_height (),
	              false, // border
	              GL_BGRA, GL_UNSIGNED_BYTE,
	              surface -> get_data ());

	glBindTexture (GL_TEXTURE_2D, 0);
}

void
ScreenText::draw ()
{
	// See GL_ARB_texture_env_combine for blending textures

	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, textureId);
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode (GL_TEXTURE);
	glLoadIdentity ();
	glMatrixMode (GL_MODELVIEW);

	//

	glBegin (GL_QUADS);
	glNormal3d (0, 0, 1);
	glTexCoord2d (0, 1);
	glVertex3d (min .x (), min .y (), min .z ());
	glTexCoord2d (1, 1);
	glVertex3d (max .x (), min .y (), min .z ());
	glTexCoord2d (1, 0);
	glVertex3d (max .x (), max .y (), max .z ());
	glTexCoord2d (0, 0);
	glVertex3d (min .x (), max .y (), min .z ());
	glEnd ();

	//

	glDisable (GL_TEXTURE_2D);
}

// Same as in ScreenGroup
void
ScreenText::scale () const
{
	Matrix4d modelViewMatrix = ModelViewMatrix4d ();

	Vector3d   translation;
	Rotation4d rotation;

	modelViewMatrix .get (translation, rotation);

	double distance = math::abs (modelViewMatrix .translation ());

	Matrix4d matrix;
	matrix .set (translation, rotation, fontStyle -> getCurrentViewpoint () -> getScreenScale (distance, Viewport4i ()));

	glLoadMatrixd (matrix .data ());
}

void
ScreenText::display ()
{
	scale ();

	X3DTextGeometry::display ();
}

ScreenText::~ScreenText ()
{
	if (textureId)
		glDeleteTextures (1, &textureId);
}

const std::string ScreenFontStyle::componentName  = "Layout";
const std::string ScreenFontStyle::typeName       = "ScreenFontStyle";
const std::string ScreenFontStyle::containerField = "fontStyle";

ScreenFontStyle::Fields::Fields () :
	pointSize (new SFFloat (12))
{ }

ScreenFontStyle::ScreenFontStyle (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DFontStyleNode (),
	          fields (),
	      screenFont ()
{
	addField (inputOutput,    "metadata",    metadata ());
	addField (initializeOnly, "family",      family ());
	addField (initializeOnly, "style",       style ());
	addField (initializeOnly, "pointSize",   pointSize ());
	addField (initializeOnly, "spacing",     spacing ());
	addField (initializeOnly, "horizontal",  horizontal ());
	addField (initializeOnly, "justify",     justify ());
	addField (initializeOnly, "topToBottom", topToBottom ());
	addField (initializeOnly, "leftToRight", leftToRight ());
	addField (initializeOnly, "language",    language ());
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

	family () .addInterest (this, &ScreenFontStyle::set_font);
	style  () .addInterest (this, &ScreenFontStyle::set_font);

	set_font ();
}

std::shared_ptr <X3DTextGeometry>
ScreenFontStyle::getTextGeometry (Text* const text) const
{
	return std::shared_ptr <X3DTextGeometry> (new ScreenText (text, this));
}

double
ScreenFontStyle::getLineHeight () const
{
	return getSize () * spacing ();
}

double
ScreenFontStyle::getSize () const
{
	if (glXGetCurrentContext ())
	{
		double height   = Gdk::Screen::get_default () -> get_height ();
		double height_m = Gdk::Screen::get_default () -> get_height_mm () / 1000.0;

		return M_POINT * pointSize () * height / height_m;
	}

	return 0;
}

void
ScreenFontStyle::set_font ()
{
	screenFont .reset (new Font (getFont ()));
}

void
ScreenFontStyle::dispose ()
{
	screenFont .reset ();

	X3DFontStyleNode::dispose ();
}

} // X3D
} // titania

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

#include "ScreenText.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Layout/ScreenFontStyle.h"
#include "../../Components/Navigation/X3DViewpointNode.h"
#include "../../Components/Texturing/PixelTexture.h"
#include "../../Components/Text/Text.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderObject.h"

#include <Titania/Physics/Constants.h>

namespace titania {
namespace X3D {

const Component   ScreenText::component      = Component ("Titania", 2);
const std::string ScreenText::typeName       = "ScreenText";
const std::string ScreenText::containerField = "textGeometry";

ScreenText::ScreenText (Text* const text, const ScreenFontStyle* const fontStyle) :
	    X3DBaseNode (text -> getBrowser (), text -> getExecutionContext ()),
	X3DTextGeometry (text, fontStyle),
	      fontStyle (fontStyle),
	        context (Cairo::Context::create (Cairo::ImageSurface::create (Cairo::FORMAT_RGB24, 0, 0))),
	            min (),
	            max (),
	           bbox (),
	         matrix (),
	    textureNode (new PixelTexture (text -> getExecutionContext ()))
{
	addChildObjects (textureNode);

	textureNode -> setup ();

	const auto screenText = dynamic_cast <ScreenText*> (text -> getTextGeometry () .getValue ());

	if (screenText)
		matrix = screenText -> matrix;

	configure (context);
	initialize ();
	setTextBounds ();

	build ();
}

X3DBaseNode*
ScreenText::create (X3DExecutionContext* const executionContext) const
{
	return new ScreenText (getText (), fontStyle);
}

void
ScreenText::configure (const Cairo::RefPtr <Cairo::Context> & context)
{
	const auto & font     = fontStyle -> getFont ();
	const auto   fontFace = Cairo::FtFontFace::create (font .getPattern () .get ());

	context -> set_font_face (fontFace);
	context -> set_font_size (fontStyle -> getSize ());

	Cairo::FontOptions options;
	options .set_hint_style (Cairo::HINT_STYLE_NONE);

	context -> set_font_options (options);
}

void
ScreenText::getLineExtents (const String & line, Vector2d & min, Vector2d & max) const
{
	Cairo::TextExtents textExtents;

	context -> get_text_extents (line, textExtents);

	min = Vector2d (textExtents .x_bearing, -textExtents .y_bearing - textExtents .height);
	max = min + Vector2d (textExtents .width, textExtents .height);
}

void
ScreenText::setTextBounds ()
{
	getText () -> textBounds () = math::ceil (getText () -> textBounds () .getValue ());

	const auto extents = X3DTextGeometry::getBBox () .extents ();

	min = extents .first;
	max = extents .second;

	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			min .x (std::floor (min .x ()));
			max .x (min .x () + getText () -> textBounds () .getX ());
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			min .x (std::round (min .x ()));
			max .x (min .x () + getText () -> textBounds () .getX ());
			break;
		case X3DFontStyleNode::Alignment::END:
			max .x (std::ceil (max .x ()));
			min .x (max .x () - getText () -> textBounds () .getX ());
			break;
	}

	switch (fontStyle -> getMinorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			max .y (std::ceil (max .y ()));
			min .y (max .y () - getText () -> textBounds () .getY ());
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			max .y (std::round (max .y ()));
			min .y (max .y () - getText () -> textBounds () .getY ());
			break;
		case X3DFontStyleNode::Alignment::END:
			min .y (std::floor (min .y ()));
			max .y (min .y () + getText () -> textBounds () .getY ());
			break;
	}

	getText () -> origin () = Vector3f (min .x (), max .y (), 0);

	setBBox (Box3d (min, max, extents_type ()));
}

void
ScreenText::build ()
{
	const bool topToBottom = fontStyle -> topToBottom ();
	const bool leftToRight = fontStyle -> leftToRight ();

	// Create context

	const auto surface = Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
	                                                  getText () -> textBounds () .getX (),
	                                                  getText () -> textBounds () .getY ());

	const auto context = Cairo::Context::create (surface);

	configure (context);
	context -> set_source_rgb (1, 1, 1);

	// Render lines.

	const auto                       font = context -> get_scaled_font ();
	std::vector <Cairo::Glyph>       glyphs;
	std::vector <Cairo::TextCluster> clusters;
	Cairo::TextClusterFlags          cluster_flags;

	// Render lines.

	if (fontStyle -> horizontal ())
	{
		Vector2d alignment = getBearing ();

		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				alignment += Vector2d (getText () -> textBounds () .getX () / 2, 0);
				break;
			case X3DFontStyleNode::Alignment::END:
				alignment += Vector2d (getText () -> textBounds () .getX (), 0);
				break;
		}

		const int32_t first = topToBottom ? 0 : getText () -> string () .size () - 1;
		const int32_t last  = topToBottom ? getText () -> string () .size () : -1;
		const int32_t step  = topToBottom ? 1 : -1;

		for (int32_t i = first; i not_eq last; i += step)
		{
			try
			{
				const auto & line = getText () -> string () .get1Value (i);

				if (not line .empty ())
				{
					const double x = alignment .x () + getTranslations () [i] .x ();
					const double y = -(alignment .y () + getTranslations () [i] .y ());

					if (leftToRight)
					{
						font -> text_to_glyphs (x, y, line, glyphs, clusters, cluster_flags);

						double       space       = 0;
						const double charSpacing = getCharSpacing () [i];

						for (auto & glyph : glyphs)
						{
							glyph .x += space;
							space    += charSpacing;
						}

						context -> show_text_glyphs (line, glyphs, clusters, cluster_flags);
					}
					else
					{
						String reversed (line .rbegin (), line .rend ());

						font -> text_to_glyphs (x, y, reversed, glyphs, clusters, cluster_flags);

						double       space       = 0;
						const double charSpacing = getCharSpacing () [i];

						for (auto & glyph : glyphs)
						{
							glyph .x += space;
							space    += charSpacing;
						}

						context -> show_text_glyphs (reversed, glyphs, clusters, cluster_flags);
					}
				}
			}
			catch (...)
			{ }
		}
	}
	else
	{
		Vector2d alignment = getBearing ();

		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				alignment += Vector2d (0, getText () -> textBounds () .getY () / 2);
				break;
			case X3DFontStyleNode::Alignment::END:
				alignment += Vector2d (0, getText () -> textBounds () .getY ());
				break;
		}

		const bool    leftToRight = fontStyle -> leftToRight ();
		const bool    topToBottom = fontStyle -> topToBottom ();
		const int32_t first       = leftToRight ? 0 : getText () -> string () .size () - 1;
		const int32_t last        = leftToRight ? getText () -> string () .size () : -1;
		const int32_t step        = leftToRight ? 1 : -1;

		for (int32_t i = first, g = 0; i not_eq last; i += step)
		{
			const auto & line = getText () -> string () .get1Value (i);

			for (const auto glyph : topToBottom ? line : String (line .rbegin (), line .rend ()))
			{
				const double x = alignment .x () + getTranslations () [g] .x ();
				const double y = alignment .y () - getTranslations () [g] .y ();

				context -> move_to (x, y);
				context -> show_text (String (1, glyph));

				++ g;
			}
		}
	}

	// Debug
	//if (surface -> get_width () and surface -> get_height ())
	//	surface -> write_to_png ("/home/holger/test.png");

	// Set RGB to white, but leave alpha channel for better antialiasing results.

	{
		uint8_t* first = surface -> get_data ();
		uint8_t* last  = first + 4 * surface -> get_width () * surface -> get_height ();

		while (first not_eq last)
		{
			*first ++ = 255;
			*first ++ = 255;
			*first ++ = 255;
			++ first;
		}
	}

	// Upload texture

	glBindTexture (GL_TEXTURE_2D, textureNode -> getTextureId ());

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP);

	glTexImage2D (GL_TEXTURE_2D,
	              0,     // This texture is level 0 in mimpap generation.
	              GL_RGBA,
	              surface -> get_width (), surface -> get_height (),
	              0,     // border
	              GL_BGRA, GL_UNSIGNED_BYTE,
	              surface -> get_data ());

	glBindTexture (GL_TEXTURE_2D, 0);

	bbox = Box3d (Vector3d (std::numeric_limits <double>::infinity (),
	                        std::numeric_limits <double>::infinity (),
	                        std::numeric_limits <double>::infinity ()),
	              Vector3d ());

	// Create geometry

	getText () -> getMultiTexCoords () .emplace_back ();

	auto & texCoords = getText () -> getTexCoords ();
	auto & normals   = getText () -> getNormals ();
	auto & vertices  = getText () -> getVertices ();

	// Triangle 1

	// p1
	texCoords .emplace_back (0, 1, 0, 1);
	normals   .emplace_back (0, 0, 1);
	vertices  .emplace_back (min .x (), min .y (), min .z ());

	// p2
	texCoords .emplace_back (1, 1, 0, 1);
	normals   .emplace_back (0, 0, 1);
	vertices  .emplace_back (max .x (), min .y (), min .z ());

	// p3
	texCoords .emplace_back (1, 0, 0, 1);
	normals   .emplace_back (0, 0, 1);
	vertices  .emplace_back (max .x (), max .y (), max .z ());

	// Triangle 2

	// p1
	texCoords .emplace_back (0, 1, 0, 1);
	normals   .emplace_back (0, 0, 1);
	vertices  .emplace_back (min .x (), min .y (), min .z ());

	// p3
	texCoords .emplace_back (1, 0, 0, 1);
	normals   .emplace_back (0, 0, 1);
	vertices  .emplace_back (max .x (), max .y (), max .z ());

	// p4
	texCoords .emplace_back (0, 0, 0, 1);
	normals   .emplace_back (0, 0, 1);
	vertices  .emplace_back (min .x (), max .y (), min .z ());

	getText () -> addElements (GL_TRIANGLES, vertices .size ());
	getText () -> setSolid (getText () -> solid ());
}

void
ScreenText::transform (X3DRenderObject* const renderObject)
{
	try
	{
		// Determine model view matrix and bbox.

		const auto & modelViewMatrix  = renderObject -> getModelViewMatrix () .get ();
		const auto & viewport         = renderObject -> getViewVolumes () .back () .getViewport ();
		const auto & projectionMatrix = renderObject -> getProjectionMatrix () .get ();
		const auto   screenScale      = renderObject -> getViewpoint () -> getScreenScale (modelViewMatrix .origin (), viewport);

		auto screenMatrix = Matrix4d (normalize (modelViewMatrix .x ()) * screenScale .x (),
		                              normalize (modelViewMatrix .y ()) * screenScale .y (),
		                              normalize (modelViewMatrix .z ()) * screenScale .z (),
		                              modelViewMatrix .w ());

		// Snap to whole pixel

		auto screenPoint = ViewVolume::projectPoint (Vector3d (), screenMatrix, projectionMatrix, viewport);

		screenPoint .x (std::round (screenPoint .x ()));
		screenPoint .y (std::round (screenPoint .y ()));

		auto offset = ViewVolume::unProjectPoint (screenPoint .x (), screenPoint .y (), screenPoint .z (), screenMatrix, projectionMatrix, viewport);

		offset .z (0);
		screenMatrix .translate (offset);

		// Assign modelViewMatrix and relative matrix

		matrix = screenMatrix * inverse (modelViewMatrix);
		bbox   = X3DTextGeometry::getBBox () * matrix;
	}
	catch (const std::domain_error &)
	{ }
}

void
ScreenText::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		{
			break;
		}
		default:
		{
			transform (renderObject);
			break;
		}
	}
}

void
ScreenText::draw (ShapeContainer* const context)
{
	const auto modelViewMatrix = matrix * context -> getModelViewMatrix ();

	context -> setTexture (textureNode);
	context -> setTextureTransform (context -> getBrowser () -> getDefaultTextureTransform ());
	context -> setModelViewMatrix (modelViewMatrix);
}

SFNode
ScreenText::toPrimitive () const
{
	throw Error <NOT_SUPPORTED> ("ScreenText::toPrimitive");
}

ScreenText::~ScreenText ()
{ }

} // X3D
} // titania

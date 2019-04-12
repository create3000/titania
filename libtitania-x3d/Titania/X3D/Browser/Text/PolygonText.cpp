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

#include "PolygonText.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Text/FontStyle.h"
#include "../../Components/Text/Text.h"
#include "../../Execution/X3DExecutionContext.h"

#include <Titania/FTGL.h>

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

} // X3D
} // titania

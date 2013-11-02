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

#include "X3DComposedGeometryNode.h"

#include "../../Bits/Cast.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Rendering/X3DNormalNode.h"
#include "../Texturing/TextureCoordinate.h"
#include "../Texturing/TextureCoordinateGenerator.h"

namespace titania {
namespace X3D {

X3DComposedGeometryNode::Fields::Fields () :
	 colorPerVertex (new SFBool (true)),
	normalPerVertex (new SFBool (true)),
	          solid (new SFBool (true)),
	            ccw (new SFBool (true)),
	         attrib (new MFNode ()),
	       fogCoord (new SFNode ()),
	       texCoord (new SFNode ()),
	          color (new SFNode ()),
	         normal (new SFNode ()),
	          coord (new SFNode ())
{ }

X3DComposedGeometryNode::X3DComposedGeometryNode () :
	X3DGeometryNode (),
	         fields ()
{
	addNodeType (X3DConstants::X3DComposedGeometryNode);
}

bool
X3DComposedGeometryNode::isTransparent () const
{
	auto _color = x3d_cast <X3DColorNode*> (color ());

	return _color and _color -> isTransparent ();
}

void
X3DComposedGeometryNode::set_index (const MFInt32 & index)
{
	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	int32_t numPoints = -1;

	if (not index .empty ())
	{
		// Determine number of points and polygons.

		for (const auto & i : index)
		{
			numPoints = std::max <int32_t> (numPoints, i);
		}

		++ numPoints;

		// Resize coord .point if to small
		_coord -> resize (numPoints);
	}
}

void
X3DComposedGeometryNode::buildPolygons (size_t vertexCount, size_t size)
{
	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	if (not _coord or _coord -> empty ())
		return;

	size_t faces = size / vertexCount; // Integer division

	// Set size to a multiple of vertexCount.

	size = faces * vertexCount;

	// Color

	auto _color = x3d_cast <X3DColorNode*> (color ());

	if (_color)
	{
		_color -> resize (colorPerVertex () ? size : faces);
		getColors () .reserve (size);
	}

	// TextureCoordinate

	auto _textureCoordinate          = x3d_cast <TextureCoordinate*> (texCoord ());
	auto _textureCoordinateGenerator = x3d_cast <TextureCoordinateGenerator*> (texCoord ());

	if (_textureCoordinate)
		_textureCoordinate -> resize (size);

	if (_textureCoordinate or not _textureCoordinateGenerator)
		getTexCoord () .reserve (size);

	// Normal

	auto _normal = x3d_cast <X3DNormalNode*> (normal ());

	if (_normal)
		_normal -> resize (normalPerVertex () ? size : faces);

	getNormals () .reserve (size);

	// Vertices

	getVertices () .reserve (size);

	// Fill GeometryNode

	for (size_t index = 0, face = 0; index < size; ++ face)
	{
		for (size_t i = 0; i < vertexCount; ++ i, ++ index)
		{
			if (_color)
			{
				if (colorPerVertex ())
					_color -> emplace_back (getColors (), getIndex (index));

				else
					_color -> emplace_back (getColors (), face);
			}

			if (_textureCoordinate)
			{
				const auto & t = _textureCoordinate -> point () [getIndex (index)];
				getTexCoord () .emplace_back (t .getX (), t .getY (), 0);
			}

			if (_normal)
			{
				if (normalPerVertex ())
					_normal -> emplace_back (getNormals (), getIndex (index));

				else
					_normal -> emplace_back (getNormals (), face);
			}

			_coord -> emplace_back (getVertices (), getIndex (index));
		}
	}

	// Autogenerate normal and texCoord if not specified

	if (not _textureCoordinate and not _textureCoordinateGenerator)
		buildTexCoord ();

	if (not _normal)
		buildNormals (vertexCount, size);

	setTextureCoordinateGenerator (_textureCoordinateGenerator);
	addElements (getVertexMode (vertexCount), getVertices () .size ());
	setSolid (solid ());
	setCCW (ccw ());
}

void
X3DComposedGeometryNode::buildNormals (size_t vertexCount, size_t size)
{
	buildFaceNormals (vertexCount, size);

	if (normalPerVertex ())
	{
		NormalIndex normalIndex;

		for (size_t i = 0; i < size; ++ i)
			normalIndex [getIndex (i)] .emplace_back (i);

		refineNormals (normalIndex, getNormals (), M_PI, true);
	}
}

void
X3DComposedGeometryNode::buildFaceNormals (size_t vertexCount, size_t size)
{
	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	for (size_t index = 0; index < size; index += vertexCount)
	{
		Vector3f normal;

		for (size_t i = 1, size = vertexCount - 1; i < size; ++ i)
		{
			// Determine polygon normal.
			normal += _coord -> getNormal (getIndex (index),
			                               getIndex (index + i),
			                               getIndex (index + i + 1));
		}

		getNormals () .resize (getNormals () .size () + vertexCount, vertexCount == 3 ? normal : normalize (normal));
	}

	if (not ccw ())
	{
		for (auto & normal : getNormals ())
			normal .negate ();
	}
}

} // X3D
} // titania

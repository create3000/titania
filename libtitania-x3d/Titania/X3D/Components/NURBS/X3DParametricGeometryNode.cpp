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

#include "X3DParametricGeometryNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Geometry3D/IndexedFaceSet.h"
#include "../NURBS/CoordinateDouble.h"

namespace titania {
namespace X3D {

X3DParametricGeometryNode::X3DParametricGeometryNode () :
	X3DGeometryNode ()
{
	addType (X3DConstants::X3DParametricGeometryNode);
}

std::vector <float>
X3DParametricGeometryNode::getKnots (const std::vector <double> & knot, const bool closed, const size_t order, const size_t dimension) const
{
	std::vector <float> knots (knot .cbegin (), knot .cend ());

	// check the knot-vectors. If they are not according to standard
	// default uniform knot vectors will be generated.

	bool generateUniform = true;

	if (knots .size () == size_t (dimension + order))
	{
		generateUniform = false;

		size_t consecutiveKnots = 0;

		for (size_t i = 1; i < knots .size (); ++ i)
		{
			if (knots [i] == knots [i - 1])
				++ consecutiveKnots;
			else
				consecutiveKnots = 0;

			if (consecutiveKnots > order - 1)
				generateUniform = true;

			if (knots [i - 1] > knots [i])
				generateUniform = true;
		}
	}

	if (generateUniform)
	{
		knots .resize (dimension + order);

		for (size_t i = 0, size = knots .size (); i < size; ++ i)
			knots [i] = float (i) / (size - 1);
	}

	if (closed)
	{
		for (size_t i = 1, size = order - 1; i < size; ++ i)
			knots .emplace_back (knots .back () + (knots [i] - knots [i - 1]));
	}

	return knots;
}

NodeType
X3DParametricGeometryNode::getPrimitiveType () const
{
	return X3DConstants::IndexedFaceSet;
}

SFNode
X3DParametricGeometryNode::toPrimitive () const
{
	const auto geometryNode   = getExecutionContext () -> createNode <IndexedFaceSet> ();
	const auto coordinateNode = getExecutionContext () -> createNode <CoordinateDouble> ();
	auto       map            = std::map <Vector3d, size_t> ();

	geometryNode -> creaseAngle () = pi <float>;
	geometryNode -> solid ()       = getSolid ();
	geometryNode -> ccw ()         = getCCW ();
	geometryNode -> coord ()       = coordinateNode;

	for (const auto & vertex : getVertices ())
		map .emplace (vertex, map .size ());

	for (const auto & element : getElements ())
	{
		switch (element .vertexMode ())
		{
			case GL_TRIANGLES:
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 3)
				{
					const auto & vertex1 = getVertices () [i];
					const auto & vertex2 = getVertices () [i + 1];
					const auto & vertex3 = getVertices () [i + 2];

					geometryNode -> coordIndex () .emplace_back (map [vertex1]);
					geometryNode -> coordIndex () .emplace_back (map [vertex2]);
					geometryNode -> coordIndex () .emplace_back (map [vertex3]);
					geometryNode -> coordIndex () .emplace_back (-1);
				}

				break;
			}
			case GL_QUADS:
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 4)
				{
					const auto & vertex1 = getVertices () [i];
					const auto & vertex2 = getVertices () [i + 1];
					const auto & vertex3 = getVertices () [i + 2];
					const auto & vertex4 = getVertices () [i + 3];

					geometryNode -> coordIndex () .emplace_back (map [vertex1]);
					geometryNode -> coordIndex () .emplace_back (map [vertex2]);
					geometryNode -> coordIndex () .emplace_back (map [vertex3]);
					geometryNode -> coordIndex () .emplace_back (map [vertex4]);
					geometryNode -> coordIndex () .emplace_back (-1);
				}

				break;
			}
		}
	}

	for (const auto & [point, index] : map)
		coordinateNode -> set1Point (index, point);

	return geometryNode;
}

} // X3D
} // titania

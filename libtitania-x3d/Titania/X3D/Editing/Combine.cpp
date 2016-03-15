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

#include "Combine.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Grouping/X3DGroupingNode.h"
#include "../Components/Texturing/TextureCoordinate.h"
#include "../Components/Rendering/Coordinate.h"
#include "../Components/Rendering/Color.h"
#include "../Components/Rendering/X3DNormalNode.h"

#include <Titania/Utility/Map.h>

namespace titania {
namespace X3D {

Combine::Combine () :
	X3DEditor ()
{ }

void
Combine::combine (const X3DExecutionContextPtr & executionContext,
                  const X3DPtrArray <X3DShapeNode> & shapes,
                  const UndoStepPtr & undoStep) const
throw (Error <DISPOSED>,
       std::out_of_range)
{
	if (not executionContext -> hasComponent (ComponentType::GEOMETRY_3D))
		executionContext -> updateComponent (executionContext -> getBrowser () -> getComponent ("Geometry3D", 2));

	// Choose target

	const auto & masterShape = shapes .back ();

	X3DPtr <IndexedFaceSet>    masterGeometry (masterShape -> geometry ());
	X3DPtr <IndexedFaceSet>    targetGeometry (executionContext -> createNode <IndexedFaceSet> ());
	X3DPtr <X3DCoordinateNode> targetCoord (executionContext -> createNode <Coordinate> ());

	targetGeometry -> coord () = targetCoord;

	// Combine Coordinates

	const auto targetMatrix = ~getModelViewMatrix (executionContext, SFNode (masterShape));

	combineCoordinates (executionContext, shapes, targetGeometry, targetCoord, targetMatrix);

	replaceNode (masterShape -> getExecutionContext (), SFNode (masterShape), masterShape -> geometry (), SFNode (targetGeometry), undoStep);
}

void
Combine::combineCoordinates (const X3DExecutionContextPtr & executionContext,
                             const X3DPtrArray <X3DShapeNode> & shapes,
                             const X3DPtr <IndexedFaceSet> & targetGeometry,
                             const X3DPtr <X3DCoordinateNode> & targetCoord,
                             const Matrix4d & targetMatrix) const
{
	// Add colors, texCoords and normals if needed.

	bool addColors    = false;
	bool addTexCoords = false;
	bool addNormals   = false;

	for (const auto & shape : shapes)
	{
		const X3DPtr <IndexedFaceSet> geometry (shape -> geometry ());

		if (not geometry)
			continue;

		if (geometry -> colorIndex () .size () and geometry -> getColor ())
			addColors = true;
	
		if (geometry -> texCoordIndex () .size () and geometry -> getTexCoord ())
			addTexCoords = true;

		if (geometry -> normalIndex () .size () and geometry -> getNormal ())
			addNormals = true;
	}

	if (addColors)
		targetGeometry -> addColors ();

	if (addTexCoords)
		targetGeometry -> addTexCoords ();

	if (addNormals)
		targetGeometry -> addNormals ();

	for (const auto & shape : shapes)
	{
		const X3DPtr <IndexedFaceSet> geometry (shape -> geometry ());

		if (not geometry)
			continue;

		if (addColors and not (geometry -> colorIndex () .size () and geometry -> getColor ()))
			geometry -> addColors ();

		if (addTexCoords and not (geometry -> texCoordIndex () .size () and geometry -> getTexCoord ()))
			geometry -> addTexCoords ();

		if (addNormals and not (geometry -> normalIndex () .size () and geometry -> getNormal ()))
			geometry -> addNormals ();
	}

	// Combine

	const auto targetColor    = X3DPtr <X3DColorNode> (targetGeometry -> color ());
	const auto targetTexCoord = X3DPtr <TextureCoordinate> (targetGeometry -> texCoord ());
	const auto targetNormal   = X3DPtr <X3DNormalNode> (targetGeometry -> normal ());

	for (const auto & shape : shapes)
	{
		const X3DPtr <IndexedFaceSet>    geometry (shape -> geometry ());
		const X3DPtr <X3DCoordinateNode> coord (geometry -> coord ());

		if (not geometry)
			continue;
		
		if (not coord)
			continue;
	
		const auto color    = X3DPtr <X3DColorNode> (geometry -> color ());
		const auto texCoord = X3DPtr <TextureCoordinate> (geometry -> texCoord ());
		const auto normal   = X3DPtr <X3DNormalNode> (geometry -> normal ());

		std::map <int32_t, int32_t> colorIndex;
		std::map <int32_t, int32_t> texCoordIndex;
		std::map <int32_t, int32_t> normalIndex;
		std::map <int32_t, int32_t> coordIndex;

		for (const auto & index : geometry -> colorIndex ())
		{
			if (index >= 0)
				colorIndex .emplace (index, colorIndex .size ());
		}

		for (const auto & index : geometry -> texCoordIndex ())
		{
			if (index >= 0)
				texCoordIndex .emplace (index, texCoordIndex .size ());
		}

		for (const auto & index : geometry -> normalIndex ())
		{
			if (index >= 0)
				normalIndex .emplace (index, normalIndex .size ());
		}

		for (const auto & index : geometry -> coordIndex ())
		{
			if (index >= 0)
				coordIndex .emplace (index, coordIndex .size ());
		}

		const auto matrix = getModelViewMatrix (executionContext, SFNode (geometry)) * targetMatrix;

		size_t face = 0;

		for (size_t i = 0, size = geometry -> coordIndex () .size (); i < size; ++ i)
		{
			const int32_t index = geometry -> coordIndex () [i];

			if (index < 0)
			{
				++ face;

				if (targetColor)
					targetGeometry -> colorIndex () .emplace_back (-1);

				if (targetTexCoord)
					targetGeometry -> texCoordIndex () .emplace_back (-1);

				if (targetNormal)
					targetGeometry -> normalIndex () .emplace_back (-1);

				targetGeometry -> coordIndex () .emplace_back (-1);
				continue;
			}

			if (targetColor)
			{
				if (geometry -> colorPerVertex ())
					targetGeometry -> colorIndex () .emplace_back (colorIndex [geometry -> colorIndex () [i]] + targetColor -> getSize ());
				else
					targetGeometry -> colorIndex () .emplace_back (colorIndex [geometry -> colorIndex () [face]] + targetColor -> getSize ());
			}

			if (targetTexCoord)
				targetGeometry -> texCoordIndex () .emplace_back (texCoordIndex [geometry -> texCoordIndex () [i]] + targetTexCoord -> getSize ());

			if (targetNormal)
			{
				if (geometry -> normalPerVertex ())
					targetGeometry -> normalIndex () .emplace_back (normalIndex [geometry -> normalIndex () [i]] + targetNormal -> getSize ());
				else
					targetGeometry -> normalIndex () .emplace_back (normalIndex [geometry -> normalIndex () [face]] + targetNormal -> getSize ());
			}

			targetGeometry -> coordIndex () .emplace_back (coordIndex [index] + targetCoord -> getSize ());
		}

		if (targetColor)
		{
			for (const auto & index : basic::reverse (colorIndex))
				targetColor -> set1Color (targetColor -> getSize (), color -> get1Color (index .second));
		}

		if (targetTexCoord)
		{
			for (const auto & index : basic::reverse (texCoordIndex))
				targetTexCoord -> set1Point (targetTexCoord -> getSize (), texCoord -> get1Point (index .second));
		}

		if (targetNormal)
		{
			for (const auto & index : basic::reverse (normalIndex))
				targetNormal -> set1Vector (targetNormal -> getSize (), normal -> get1Vector (index .second));
		}

		for (const auto & index : basic::reverse (coordIndex))
			targetCoord -> set1Point (targetCoord -> getSize (), coord -> get1Point (index .second) * matrix);
	}

	// Add Color instead of ColorRGBA if not transparent.

	if (targetColor)
	{
		if (not targetColor -> getTransparent ())
		{
			const auto colorNode = executionContext -> createNode <Color> ();

			for (size_t i = 0, size = targetColor -> getSize (); i < size; ++ i)
				colorNode -> set1Color (i, targetColor -> get1Color (i));
		
		   targetGeometry -> color () = colorNode;
		}
	}
}

void
Combine::removeShapes (const X3DExecutionContextPtr & executionContext,
                       const MFNode & selection,
                       const X3DPtrArray <X3DGroupingNode> & groups,
                       const X3DPtrArray <X3DShapeNode> & shapes,
                       const X3DPtr <X3DShapeNode> & masterShape,
                       const UndoStepPtr & undoStep) const
{
	// Remove Shape nodes that are direct selected.

	MFNode nodes;

	for (const auto & node : selection)
	{
		X3DPtr <X3DShapeNode> shape (node);

		if (not shape or shape == masterShape)
			continue;

		nodes .emplace_back (node);
	}

	removeNodesFromScene (executionContext, nodes, true, undoStep);

	// Remove Shape nodes from selection.

	nodes .assign (shapes .begin (), shapes .end ());
	nodes .remove (SFNode (masterShape));

	removeNodesFromSceneGraph (selection, std::set <SFNode> (nodes .begin (), nodes .end ()), undoStep);

	for (const auto & node : nodes)
		removeNodesFromSceneIfNotExists (executionContext, { node }, undoStep);

	// Find empty groups in selection and remove from scene.

	nodes .clear ();

	for (const auto & group : groups)
	{
		if (group -> children () .empty ())
			nodes .emplace_back (group);
	}

	removeNodesFromScene (executionContext, nodes, true, undoStep);
}

Combine::~Combine ()
{ }

} // X3D
} // titania

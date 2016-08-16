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
#include "../Rendering/Tessellator.h"
#include "../Editing/Editor.h"

#include <Titania/Utility/Map.h>

namespace titania {
namespace X3D {

Combine::Combine ()
{ }

void
Combine::geometryUnion (const X3DExecutionContextPtr & executionContext,
                        const X3DPtrArray <X3DShapeNode> & shapes,
                        const UndoStepPtr & undoStep)
throw (Error <INVALID_NODE>,
       Error <DISPOSED>,
       std::domain_error)
{
	geometryBoolean (mesh_union, executionContext, shapes, undoStep);
}

void
Combine::geometryDifference (const X3DExecutionContextPtr & executionContext,
                             const X3DPtrArray <X3DShapeNode> & shapes,
                             const UndoStepPtr & undoStep)
throw (Error <INVALID_NODE>,
       Error <DISPOSED>,     
       std::domain_error)
{
	geometryBoolean (mesh_difference, executionContext, shapes, undoStep);
}

void
Combine::geometryIntersection (const X3DExecutionContextPtr & executionContext,
                               const X3DPtrArray <X3DShapeNode> & shapes,
                               const UndoStepPtr & undoStep)
throw (Error <INVALID_NODE>,
       Error <DISPOSED>,     
       std::domain_error)
{
	geometryBoolean (mesh_intersection, executionContext, shapes, undoStep);
}

void
Combine::geometrySymmetricDifference (const X3DExecutionContextPtr & executionContext,
                                      const X3DPtrArray <X3DShapeNode> & shapes,
                                      const UndoStepPtr & undoStep)
throw (Error <INVALID_NODE>,
       Error <DISPOSED>,     
       std::domain_error)
{
	geometryBoolean (mesh_symmetric_difference, executionContext, shapes, undoStep);
}

void
Combine::geometryBoolean (const BooleanOperation & booleanOperation,
                          const X3DExecutionContextPtr & executionContext,
                          const X3DPtrArray <X3DShapeNode> & shapes,
                          const UndoStepPtr & undoStep)
throw (Error <INVALID_NODE>,
       Error <DISPOSED>,
       std::domain_error)
{
	if (not executionContext -> hasComponent (ComponentType::GEOMETRY_3D))
		executionContext -> updateComponent (executionContext -> getBrowser () -> getComponent ("Geometry3D", 2));

	// Choose target

	const auto & masterShape    = shapes .back ();
	const auto   targetGeometry = executionContext -> createNode <IndexedFaceSet> ();
	const auto   targetCoord    = executionContext -> createNode <Coordinate> ();
	const auto   targetMatrix   = ~Editor () .getModelViewMatrix (executionContext -> getMasterScene (), SFNode (masterShape));

	targetGeometry -> coord () = targetCoord;

	// Filter geometries.

	X3DPtrArray <IndexedFaceSet> geometries;

	for (const auto & shape : shapes)
	{
		const X3DPtr <IndexedFaceSet> geometry (shape -> geometry ());

		if (not geometry)
			continue;

		geometries .emplace_back (geometry);
	}

	if (not geometries .empty ())
		targetGeometry -> creaseAngle () = geometries .front () -> creaseAngle ();

	// Combine Coordinates

	std::vector <mesh <double>> meshes;

	for (const auto & geometryNode : geometries)
	{
		const auto coordNode = geometryNode -> getCoord ();

		if (not coordNode)
			continue;

		const auto matrix = Editor () .getModelViewMatrix (geometryNode -> getMasterScene (), SFNode (geometryNode)) * targetMatrix;

		auto indices = std::vector <uint32_t> ();
		auto points  = std::vector <Vector3d> ();

		std::vector <size_t> vertices;
	
		for (const auto & index : geometryNode -> coordIndex ())
		{
			if (index < 0)
			{
				if (vertices .size () < 3)
				{
					vertices .clear ();
					continue;
				}

				opengl::tessellator <size_t> tessellator;

				tessellator .begin_polygon ();
				tessellator .begin_contour ();
			
				for (const auto & index : vertices)
				{
					const auto point = coordNode -> get1Point (index) * matrix;
		
					tessellator .add_vertex (point, index);
				}
			
				tessellator .end_contour ();
				tessellator .end_polygon ();
			
				const auto triangles = tessellator .triangles ();
		
				for (size_t v = 0, size = triangles .size (); v < size; )
				{
					indices .emplace_back (std::get <0> (triangles [v ++] .data ()));
					indices .emplace_back (std::get <0> (triangles [v ++] .data ()));
					indices .emplace_back (std::get <0> (triangles [v ++] .data ()));
				}

				vertices .clear ();
			}
			else
			{
				vertices .emplace_back (index);
			}
		}

		for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
			points .emplace_back (coordNode -> get1Point (i) * matrix);

		meshes .emplace_back (std::move (indices), std::move (points));

		if (not mesh_is_solid (meshes .back ()))
			throw Error <INVALID_NODE> ("geometry is not solid");
	}

	if (meshes .size () >= 2)
	{
		auto result = std::move (meshes .front ());

		for (const auto & mesh : std::make_pair (meshes .begin () + 1, meshes .end ()))
			result = booleanOperation (result, mesh);
	
		for (size_t i = 0, size = result .first .size (); i < size;)
		{
			targetGeometry -> coordIndex () .emplace_back (result .first [i++]);
			targetGeometry -> coordIndex () .emplace_back (result .first [i++]);
			targetGeometry -> coordIndex () .emplace_back (result .first [i++]);
			targetGeometry -> coordIndex () .emplace_back (-1);
		}

		targetCoord -> point () .assign (result .second .begin (), result .second .end ());

		// Replace node
	
		Editor () .replaceNode (masterShape -> getExecutionContext (), SFNode (masterShape), masterShape -> geometry (), SFNode (targetGeometry), undoStep);
	}
}

void
Combine::combineGeometry (const X3DExecutionContextPtr & executionContext,
                          const X3DPtrArray <X3DShapeNode> & shapes,
                          const UndoStepPtr & undoStep)
throw (Error <INVALID_NODE>,
       Error <DISPOSED>,
       std::domain_error)
{
	if (not executionContext -> hasComponent (ComponentType::GEOMETRY_3D))
		executionContext -> updateComponent (executionContext -> getBrowser () -> getComponent ("Geometry3D", 2));

	// Choose target

	const auto & masterShape    = shapes .back ();
	const auto   targetGeometry = executionContext -> createNode <IndexedFaceSet> ();
	const auto   targetCoord    = X3DPtr <X3DCoordinateNode> (executionContext -> createNode <Coordinate> ());
	const auto   targetMatrix   = ~Editor () .getModelViewMatrix (executionContext -> getMasterScene (), SFNode (masterShape));

	targetGeometry -> coord () = targetCoord;

	// Filter geometries.

	X3DPtrArray <IndexedFaceSet> geometries;

	for (const auto & shape : shapes)
	{
		const X3DPtr <IndexedFaceSet> geometry (shape -> geometry ());

		if (not geometry)
			continue;

		geometries .emplace_back (geometry);
	}

	if (not geometries .empty ())
		targetGeometry -> creaseAngle () = geometries .back () -> creaseAngle ();

	// Combine Coordinates

	for (const auto & geometry : geometries)
	{
		undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometry -> colorIndex    ()), geometry -> colorIndex    ());
		undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), geometry -> texCoordIndex ());
		undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometry -> normalIndex   ()), geometry -> normalIndex   ());
		undoStep -> addUndoFunction (&SFNode::setValue,  std::ref (geometry -> color         ()), geometry -> color         ());
		undoStep -> addUndoFunction (&SFNode::setValue,  std::ref (geometry -> texCoord      ()), geometry -> texCoord      ());
		undoStep -> addUndoFunction (&SFNode::setValue,  std::ref (geometry -> normal        ()), geometry -> normal        ());
	}

	combine (executionContext, geometries, targetGeometry, targetCoord, targetMatrix);

	for (const auto & geometry : geometries)
	{
		undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometry -> colorIndex    ()), geometry -> colorIndex    ());
		undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometry -> texCoordIndex ()), geometry -> texCoordIndex ());
		undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometry -> normalIndex   ()), geometry -> normalIndex   ());
		undoStep -> addRedoFunction (&SFNode::setValue,  std::ref (geometry -> color         ()), geometry -> color         ());
		undoStep -> addRedoFunction (&SFNode::setValue,  std::ref (geometry -> texCoord      ()), geometry -> texCoord      ());
		undoStep -> addRedoFunction (&SFNode::setValue,  std::ref (geometry -> normal        ()), geometry -> normal        ());
	}

	// Replace node

	Editor () .replaceNode (masterShape -> getExecutionContext (), SFNode (masterShape), masterShape -> geometry (), SFNode (targetGeometry), undoStep);
}

std::vector <int32_t>
Combine::combine (const X3DExecutionContextPtr & executionContext,
                  const X3DPtrArray <IndexedFaceSet> & geometries,
                  const X3DPtr <IndexedFaceSet> & targetGeometry,
                  const X3DPtr <X3DCoordinateNode> & targetCoord,
                  const Matrix4d & targetMatrix)
{
	std::vector <int32_t> points;

	// Add colors, texCoords and normals if needed.

	bool addColors    = targetGeometry -> getColor ();
	bool addTexCoords = targetGeometry -> getTexCoord ();
	bool addNormals   = targetGeometry -> getNormal ();

	for (const auto & geometry : geometries)
	{
		if (geometry -> getColor ())
			addColors = true;
	
		if (geometry -> getTexCoord ())
			addTexCoords = true;

		if (geometry -> getNormal ())
			addNormals = true;
	}

	const auto numVertices = targetGeometry -> coordIndex () .size ();
	const auto numFaces    = targetGeometry -> colorPerVertex () or targetGeometry -> normalPerVertex ()
	                         ? std::count_if (targetGeometry -> coordIndex () .begin (), targetGeometry -> coordIndex () .end (), [ ] (const int32_t i) { return i < 0; })
	                         : 0;

	if (addColors)
	{
		if (targetGeometry -> getColor ())
		{
			// If there are not enough indices in colorIndex, add them.

			if (targetGeometry -> colorPerVertex ())
			{
				for (size_t i = targetGeometry -> colorIndex () .size (); i < numVertices; ++ i)
					targetGeometry -> colorIndex () .emplace_back (targetGeometry -> coordIndex () [i]);
			}
			else
			{
				for (size_t i = targetGeometry -> colorIndex () .size (); i < numFaces; ++ i)
					targetGeometry -> colorIndex () .emplace_back (i);
			}
		}
		else
			targetGeometry -> addColors ();
	}

	if (addTexCoords)
	{
		if (targetGeometry -> getTexCoord ())
		{
			// If there are not enough indices in texCoordIndex, add them.

			for (size_t i = targetGeometry -> texCoordIndex () .size (); i < numVertices; ++ i)
				targetGeometry -> texCoordIndex () .emplace_back (targetGeometry -> coordIndex () [i]);
		}
		else
			targetGeometry -> addTexCoords ();
	}

	if (addNormals)
	{
		if (targetGeometry -> getNormal ())
		{
			// If there are not enough indices in normalIndex, add them.

			if (targetGeometry -> normalPerVertex ())
			{
			   for (size_t i = targetGeometry -> normalIndex () .size (); i < numVertices; ++ i)
					targetGeometry -> normalIndex () .emplace_back (targetGeometry -> coordIndex () [i]);
			}
			else
			{
				for (size_t i = targetGeometry -> normalIndex () .size (); i < numFaces; ++ i)
					targetGeometry -> normalIndex () .emplace_back (i);
			}
		}
		else
			targetGeometry -> addNormals ();
	}

	for (const auto & geometry : geometries)
	{
		const auto numVertices = geometry -> coordIndex () .size ();
		const auto numFaces    = geometry -> colorPerVertex () or geometry -> normalPerVertex ()
	                            ? std::count_if (geometry -> coordIndex () .begin (), geometry -> coordIndex () .end (), [ ] (const int32_t i) { return i < 0; })
	                            : 0;

		if (addColors)
		{
			if (geometry -> getColor ())
			{
				if (geometry -> colorPerVertex ())
				{
					for (size_t i = geometry -> colorIndex () .size (); i < numVertices; ++ i)
						geometry -> colorIndex () .emplace_back (geometry -> coordIndex () [i]);
				}
				else
				{
					for (size_t i = geometry -> colorIndex () .size (); i < numFaces; ++ i)
						geometry -> colorIndex () .emplace_back (i);
				}
			}
			else
				geometry -> addColors ();
		}

		if (addTexCoords)
		{
			if (geometry -> getTexCoord ())
			{
				for (size_t i = geometry -> texCoordIndex () .size (); i < numVertices; ++ i)
					geometry -> texCoordIndex () .emplace_back (geometry -> coordIndex () [i]);
			}
			else
				geometry -> addTexCoords ();
		}

		if (addNormals)
		{
			if (geometry -> getNormal ())
			{
				if (geometry -> normalPerVertex ())
				{
				   for (size_t i = geometry -> normalIndex () .size (); i < numVertices; ++ i)
						geometry -> normalIndex () .emplace_back (geometry -> coordIndex () [i]);
				}
				else
				{
					for (size_t i = geometry -> normalIndex () .size (); i < numFaces; ++ i)
						geometry -> normalIndex () .emplace_back (i);
				}
			}
			else
				geometry -> addNormals ();
		}
	}

	// Combine

	const auto targetColor    = X3DPtr <X3DColorNode> (targetGeometry -> color ());
	const auto targetTexCoord = X3DPtr <TextureCoordinate> (targetGeometry -> texCoord ());
	const auto targetNormal   = X3DPtr <X3DNormalNode> (targetGeometry -> normal ());

	for (const auto & geometry : geometries)
	{
		const X3DPtr <X3DCoordinateNode> coord (geometry -> coord ());

		if (not coord)
			continue;
	
		const auto color    = X3DPtr <X3DColorNode> (geometry -> color ());
		const auto texCoord = X3DPtr <TextureCoordinate> (geometry -> texCoord ());
		const auto normal   = X3DPtr <X3DNormalNode> (geometry -> normal ());

		std::map <int32_t, int32_t> colorArray;
		std::map <int32_t, int32_t> texCoordArray;
		std::map <int32_t, int32_t> normalArray;
		std::map <int32_t, int32_t> coordArray;

		size_t face = 0;

		for (size_t i = 0, size = geometry -> coordIndex () .size (); i < size; ++ i)
		{
		   const int32_t index = geometry -> coordIndex () [i];

		   if (index < 0)
		   {
		      ++ face;
		      continue;
		   }

			if (geometry -> colorPerVertex ())
				colorArray .emplace (geometry -> getVertexColorIndex (i), colorArray .size ());
			else
				colorArray .emplace (geometry -> getFaceColorIndex (face), colorArray .size ());

			texCoordArray .emplace (geometry -> getVertexTexCoordIndex (i), texCoordArray .size ());

			if (geometry -> normalPerVertex ())
				normalArray .emplace (geometry -> getVertexNormalIndex (i), normalArray .size ());
			else
				normalArray .emplace (geometry -> getFaceNormalIndex (face), normalArray .size ());

			coordArray .emplace (index, coordArray .size ());
		}

		const auto matrix = Editor () .getModelViewMatrix (geometry -> getMasterScene (), SFNode (geometry)) * targetMatrix;

		face              = 0;
		size_t first      = 0;
		size_t targetSize = targetGeometry -> coordIndex () .size ();

		for (size_t i = 0, size = geometry -> coordIndex () .size (); i < size; ++ i)
		{
			const int32_t index = geometry -> coordIndex () [i];

			if (index < 0)
			{
				if (geometry -> ccw () not_eq targetGeometry -> ccw ())
				{
					// Flip vertex ordering.

					const size_t s = i - first;

					for (size_t f = 0, s1_2 = s / 2; f < s1_2; ++ f)
					{
						const auto i0 = targetSize + first + f;
						const auto i1 = targetSize + first + (s - f - 1);

						if (targetColor and targetGeometry -> colorIndex () .size () >= i1)
							std::swap (targetGeometry -> colorIndex () [i0], targetGeometry -> colorIndex () [i1]);

						if (targetTexCoord and targetGeometry -> texCoordIndex () .size () >= i1)
							std::swap (targetGeometry -> texCoordIndex () [i0], targetGeometry -> texCoordIndex () [i1]);

						if (targetNormal and targetGeometry -> normalIndex () .size () >= i1)
							std::swap (targetGeometry -> normalIndex () [i0], targetGeometry -> normalIndex () [i1]);

						std::swap (targetGeometry -> coordIndex () [i0], targetGeometry -> coordIndex () [i1]);
					}
				}

				++ face;

				first = i + 1;

				if (targetColor)
					targetGeometry -> colorIndex () .emplace_back (-1);

				if (targetTexCoord)
					targetGeometry -> texCoordIndex () .emplace_back (-1);

				if (targetNormal)
					targetGeometry -> normalIndex () .emplace_back (-1);

				targetGeometry -> coordIndex () .emplace_back (-1);
				continue;
			}

			const auto point = coordArray [index] + targetCoord -> getSize ();

			if (targetColor)
			{
				if (geometry -> colorPerVertex ())
				{
					try
					{
						targetGeometry -> colorIndex () .emplace_back (colorArray .at (geometry -> getVertexColorIndex (i)) + targetColor -> getSize ());
					}
					catch (const std::out_of_range &)
					{
						targetGeometry -> colorIndex () .emplace_back (point);
					}
				}
				else
				{
					try
					{
						targetGeometry -> colorIndex () .emplace_back (colorArray .at (geometry -> getFaceColorIndex (face)) + targetColor -> getSize ());
					}
					catch (const std::out_of_range &)
					{
						targetGeometry -> colorIndex () .emplace_back (face);
					}
				}
			}

			if (targetTexCoord)
			{
				try
				{
					targetGeometry -> texCoordIndex () .emplace_back (texCoordArray .at (geometry -> getVertexTexCoordIndex (i)) + targetTexCoord -> getSize ());
				}
				catch (const std::out_of_range &)
				{
					targetGeometry -> texCoordIndex () .emplace_back (point);
				}
			}

			if (targetNormal)
			{
				if (geometry -> normalPerVertex ())
				{
					try
					{
						targetGeometry -> normalIndex () .emplace_back (normalArray .at (geometry -> getVertexNormalIndex (i)) + targetNormal -> getSize ());
					}
					catch (const std::out_of_range &)
					{
						targetGeometry -> normalIndex () .emplace_back (point);
					}
				}
				else
				{
					try
					{
						targetGeometry -> normalIndex () .emplace_back (normalArray .at (geometry -> getFaceNormalIndex (face)) + targetNormal -> getSize ());
					}
					catch (const std::out_of_range &)
					{
						targetGeometry -> normalIndex () .emplace_back (face);
					}
				}
			}

			targetGeometry -> coordIndex () .emplace_back (point);
			points .emplace_back (point);
		}

		if (targetColor)
		{
			for (const auto & index : basic::reverse (colorArray))
				targetColor -> set1Color (targetColor -> getSize (), color -> get1Color (index .second));
		}

		if (targetTexCoord)
		{
			for (const auto & index : basic::reverse (texCoordArray))
				targetTexCoord -> set1Point (targetTexCoord -> getSize (), texCoord -> get1Point (index .second));
		}

		if (targetNormal)
		{
			for (const auto & index : basic::reverse (normalArray))
				targetNormal -> set1Vector (targetNormal -> getSize (), normal -> get1Vector (index .second));
		}

		for (const auto & index : basic::reverse (coordArray))
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

	std::sort (points .begin (), points .end ());

	points .erase (std::unique (points .begin (), points .end ()), points .end ());

	return points;
}

void
Combine::removeShapes (const X3DExecutionContextPtr & executionContext,
                       const MFNode & selection,
                       const X3DPtrArray <X3DGroupingNode> & groups,
                       const X3DPtrArray <X3DShapeNode> & shapes,
                       const X3DPtr <X3DShapeNode> & masterShape,
                       const UndoStepPtr & undoStep)
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

	Editor () .removeNodesFromScene (executionContext, nodes, true, undoStep);

	// Remove Shape nodes from selection.

	nodes .assign (shapes .begin (), shapes .end ());
	nodes .remove (SFNode (masterShape));

	Editor () .removeNodesFromSceneGraph (selection, std::set <SFNode> (nodes .begin (), nodes .end ()), undoStep);

	for (const auto & node : nodes)
		Editor () .removeNodesFromSceneIfNotExists (executionContext, { node }, undoStep);

	// Find empty groups in selection and remove from scene.

	nodes .clear ();

	for (const auto & group : groups)
	{
		if (group -> children () .empty ())
			nodes .emplace_back (group);
	}

	Editor () .removeNodesFromScene (executionContext, nodes, true, undoStep);
}

Combine::~Combine ()
{ }

} // X3D
} // titania

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

#include "X3DIndexedFaceSetCutObject.h"

#include "../../Rendering/CoordinateTool.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/EnvironmentalEffects/FogCoordinate.h"
#include "../../../Components/Geometry2D/Arc2D.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Components/Rendering/X3DColorNode.h"
#include "../../../Components/Rendering/X3DNormalNode.h"
#include "../../../Components/Texturing/X3DTextureCoordinateNode.h"
#include "../../../Editing/Selection/FaceSelection.h"
#include "../../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

static constexpr double SELECTION_DISTANCE = 8; // in pixel

X3DIndexedFaceSetCutObject::Fields::Fields () :
	 cutSnapping (new SFBool (true))
{ }

X3DIndexedFaceSetCutObject::X3DIndexedFaceSetCutObject () :
	                  IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetSelectionObject (),
	                          fields ()
{
	addType (X3DConstants::X3DIndexedFaceSetCutObject);
}

/**
 *  Cuts a face into two connected faces. Run rebuildIndices, rebuildColor, rebuildTexCoord, rebuildNormal and
 *  rebuildCoord after this operation to remove degenerated faces.
 *
 *  @param  cutFace      Index of face to cut
 *  @param  cutEdge      Coordinates of edge to cut
 *  @param  startPoints  One or two points, depending on whether start from point or edge
 *  @param  endPoints    One or two points, depending on whether end at point or edge
 */

std::vector <int32_t>
X3DIndexedFaceSetCutObject::cut (const size_t cutFace,
                                 const std::pair <Vector3d, Vector3d> & cutEdge,
                                 const std::vector <int32_t> & startPoints,
                                 const std::vector <int32_t> & endPoints)
{
	const auto numFaces    = getFaceSelection () -> getNumFaces ();
	const auto faceNumber  = getFaceSelection () -> getFaceNumber (cutFace);
	const auto vertices    = getFaceSelection () -> getFaceVertices (cutFace);
	const auto startEdge   = getFaceSelection () -> getClosestEdge (cutEdge .first,  vertices);
	const auto endEdge     = getFaceSelection () -> getClosestEdge (cutEdge .second, vertices);
	auto       fillIndices = std::map <size_t, size_t> ();

	std::vector <int32_t> selection;

	int32_t startColor    = -1;
	int32_t endColor      = -1;
	int32_t startTexCoord = -1;
	int32_t endTexCoord   = -1;
	int32_t startNormal   = -1;
	int32_t endNormal     = -1;
	int32_t startPoint    = -1;
	int32_t endPoint      = -1;
	size_t  startVertex1  = -1;
	size_t  startVertex2  = -1;
	size_t  endVertex1    = -1;
	size_t  endVertex2    = -1;

	std::vector <int32_t> colorFace1;
	std::vector <int32_t> colorFace2;
	std::vector <int32_t> texFace1;
	std::vector <int32_t> texFace2;
	std::vector <int32_t> normalFace1;
	std::vector <int32_t> normalFace2;
	std::vector <int32_t> face1;
	std::vector <int32_t> face2;

	std::vector <size_t> adjacentFaces1;
	std::vector <size_t> adjacentFaces2;

	switch (startPoints .size ())
	{
		case 1:
		{
			startPoint   = startPoints [0];
			startVertex1 = getFaceSelection () -> getClosestVertex (getCoord () -> get1Point (startPoints [0]), vertices);
			endVertex2   = startVertex1;
			break;
		}
		case 2:
		{
			const auto  point1  = getCoord () -> get1Point (coordIndex () .get1Value (startEdge .index0));
			const auto  point2  = getCoord () -> get1Point (coordIndex () .get1Value (startEdge .index1));
			const auto  segment = LineSegment3d (point1, point2);
			const float t       = abs (cutEdge .first - point1) / abs (point2 - point1);

			if (not segment .is_between (cutEdge .first))
				return selection;

			if (colorIndex () .size () and getColor ())
			{
			   if (colorPerVertex ())
			   {
					const auto colorIndex0 = getVertexColorIndex (startEdge .index0);
					const auto colorIndex1 = getVertexColorIndex (startEdge .index1);

					if (colorIndex0 == colorIndex1)
					{
						startColor = colorIndex0;
					}
					else
					{
						const auto color0 = getColor () -> get1Color (colorIndex0);
						const auto color1 = getColor () -> get1Color (colorIndex1);
	
						startColor = getColor () -> getSize ();
						getColor () -> set1Color (startColor, lerp (color0, color1, t)); // Interpolation in RGB space
					}
				}
			}

			if (texCoordIndex () .size () and getTexCoord ())
			{
				const auto texCoord0 = getTexCoord () -> get1Point (getVertexTexCoordIndex (startEdge .index0));
				const auto texCoord1 = getTexCoord () -> get1Point (getVertexTexCoordIndex (startEdge .index1));

				startTexCoord = getTexCoord () -> getSize ();
				getTexCoord () -> set1Point (startTexCoord, lerp (texCoord0, texCoord1, t));
			}

			if (normalIndex () .size () and getNormal ())
			{
			   if (normalPerVertex ())
			   {
					const auto normalIndex0 = getVertexNormalIndex (startEdge .index0);
					const auto normalIndex1 = getVertexNormalIndex (startEdge .index1);

					if (normalIndex0 == normalIndex1)
					{
						startNormal = normalIndex0;
					}
					else
					{
						const auto normal0 = getNormal () -> get1Vector (normalIndex0);
						const auto normal1 = getNormal () -> get1Vector (normalIndex1);
	
						startNormal = getNormal () -> getSize ();
						getNormal () -> set1Vector (startNormal, lerp (normal0, normal1, t));
					}
				}
			}
			
			startPoint = getCoord () -> getSize ();

			if (getFogCoord ())
			{
				const auto depth1 = getFogCoord () -> get1Depth (coordIndex () .get1Value (startEdge .index0));
				const auto depth2 = getFogCoord () -> get1Depth (coordIndex () .get1Value (startEdge .index1));

				getFogCoord () -> set1Depth (startPoint, lerp (depth1, depth2, t));
			}

			getCoord () -> set1Point (startPoint, cutEdge .first);

			startVertex1   = startEdge .index1;
			endVertex2     = startEdge .index0;
			adjacentFaces1 = getFaceSelection () -> getAdjacentFaces (startEdge);
			break;
		}
		default:
			return selection;
	}

	switch (endPoints .size ())
	{
		case 1:
		{
			endPoint     = endPoints [0];
			endVertex1   = getFaceSelection () -> getClosestVertex (getCoord () -> get1Point (endPoints [0]), vertices);
			startVertex2 = endVertex1;
			break;
		}
		case 2:
		{
			const auto  point1  = getCoord () -> get1Point (coordIndex () .get1Value (endEdge .index0));
			const auto  point2  = getCoord () -> get1Point (coordIndex () .get1Value (endEdge .index1));
			const auto  segment = LineSegment3d (point1, point2);
			const float t       = abs (cutEdge .second - point1) / abs (point2 - point1);

			if (not segment .is_between (cutEdge .second))
				return selection;

			if (colorIndex () .size () and getColor ())
			{
			   if (colorPerVertex ())
			   {
					const auto colorIndex0 = getVertexColorIndex (endEdge .index0);
					const auto colorIndex1 = getVertexColorIndex (endEdge .index1);

					if (colorIndex0 == colorIndex1)
					{
						endColor = colorIndex0;
					}
					else
					{
						const auto color0 = getColor () -> get1Color (colorIndex0);
						const auto color1 = getColor () -> get1Color (colorIndex1);
	
						endColor = getColor () -> getSize ();
						getColor () -> set1Color (endColor, lerp (color0, color1, t)); // Interpolation in RGB space
					}
				}
			}
			
			if (texCoordIndex () .size () and getTexCoord ())
			{
				const auto texCoord0 = getTexCoord () -> get1Point (getVertexTexCoordIndex (endEdge .index0));
				const auto texCoord1 = getTexCoord () -> get1Point (getVertexTexCoordIndex (endEdge .index1));

				endTexCoord = getTexCoord () -> getSize ();
				getTexCoord () -> set1Point (endTexCoord, lerp (texCoord0, texCoord1, t));
			}

			if (normalIndex () .size () and getNormal ())
			{
			   if (normalPerVertex ())
			   {
					const auto normalIndex0 = getVertexNormalIndex (endEdge .index0);
					const auto normalIndex1 = getVertexNormalIndex (endEdge .index1);

					if (normalIndex0 == normalIndex1)
					{
						endNormal = normalIndex0;
					}
					else
					{
						const auto normal0 = getNormal () -> get1Vector (normalIndex0);
						const auto normal1 = getNormal () -> get1Vector (normalIndex1);
	
						endNormal = getNormal () -> getSize ();
						getNormal () -> set1Vector (endNormal, lerp (normal0, normal1, t));
					}
				}
			}
			
			endPoint = getCoord () -> getSize ();

			if (getFogCoord ())
			{
				const auto depth1 = getFogCoord () -> get1Depth (coordIndex () .get1Value (endEdge .index0));
				const auto depth2 = getFogCoord () -> get1Depth (coordIndex () .get1Value (endEdge .index1));

				getFogCoord () -> set1Depth (endPoint, lerp (depth1, depth2, t));
			}

			getCoord () -> set1Point (endPoint, cutEdge .second);

			endVertex1     = endEdge .index0;
			startVertex2   = endEdge .index1;
			adjacentFaces2 = getFaceSelection () -> getAdjacentFaces (endEdge);
			break;
		}
		default:
			return selection;
	}

	// Rewrite indices.

	startVertex1 = std::find (vertices .cbegin (), vertices .cend (), startVertex1) - vertices .cbegin ();
	startVertex2 = std::find (vertices .cbegin (), vertices .cend (), startVertex2) - vertices .cbegin ();
	endVertex1   = std::find (vertices .cbegin (), vertices .cend (), endVertex1)   - vertices .cbegin ();
	endVertex2   = std::find (vertices .cbegin (), vertices .cend (), endVertex2)   - vertices .cbegin ();

	if (endVertex1 < startVertex1)
		endVertex1 += vertices .size ();

	if (endVertex2 < startVertex2)
		endVertex2 += vertices .size ();	

	// Add start points.

	if (startPoints .size () == 2)
	{
		if (colorIndex () .size () and getColor ())
			if (colorPerVertex ())
				colorFace1 .emplace_back (startColor);

		if (texCoordIndex () .size () and getTexCoord ())
			texFace1 .emplace_back (startTexCoord);

		if (normalIndex () .size () and getNormal ())
			if (normalPerVertex ())
				normalFace1 .emplace_back (startNormal);

		face1 .emplace_back (startPoint);
	}

	if (endPoints .size () == 2)
	{
		if (colorIndex () .size () and getColor ())
			if (colorPerVertex ())
				colorFace2 .emplace_back (endColor);

		if (texCoordIndex () .size () and getTexCoord ())
			texFace2 .emplace_back (endTexCoord);

		if (normalIndex () .size () and getNormal ())
			if (normalPerVertex ())
				normalFace2 .emplace_back (endNormal);

		face2 .emplace_back (endPoint);
	}

	// Add face points.

	for (size_t i = startVertex1; i <= endVertex1; ++ i)
	{
		const auto vertex = vertices [i % vertices .size ()];

		if (colorIndex () .size () and getColor ())
			if (colorPerVertex ())
				colorFace1 .emplace_back (getVertexColorIndex (vertex));

		if (texCoordIndex () .size () and getTexCoord ())
			texFace1 .emplace_back (getVertexTexCoordIndex (vertex));

		if (normalIndex () .size () and getNormal ())
			if (normalPerVertex ())
				normalFace1 .emplace_back (getVertexNormalIndex (vertex));

		face1 .emplace_back (coordIndex () .get1Value (vertex));
	}

	for (size_t i = startVertex2; i <= endVertex2; ++ i)
	{
		const auto vertex = vertices [i % vertices .size ()];

		if (colorIndex () .size () and getColor ())
			if (colorPerVertex ())
				colorFace2 .emplace_back (getVertexColorIndex (vertex));

		if (texCoordIndex () .size () and getTexCoord ())
			texFace2 .emplace_back (getVertexTexCoordIndex (vertex));

		if (normalIndex () .size () and getNormal ())
			if (normalPerVertex ())
				normalFace2 .emplace_back (getVertexNormalIndex (vertex));

		face2 .emplace_back (coordIndex () .get1Value (vertex));
	}

	// Add end points.
	
	if (endPoints .size () == 2)
	{
		if (colorIndex () .size () and getColor ())
			if (colorPerVertex ())
				colorFace1 .emplace_back (endColor);

		if (texCoordIndex () .size () and getTexCoord ())
			texFace1 .emplace_back (endTexCoord);

		if (normalIndex () .size () and getNormal ())
			if (normalPerVertex ())
				normalFace1 .emplace_back (endNormal);

		face1 .emplace_back (endPoint);
	}
	
	if (startPoints .size () == 2)
	{
		if (colorIndex () .size () and getColor ())
			if (colorPerVertex ())
				colorFace2 .emplace_back (startColor);

		if (texCoordIndex () .size () and getTexCoord ())
			texFace2 .emplace_back (startTexCoord);

		if (normalIndex () .size () and getNormal ())
			if (normalPerVertex ())
				normalFace2 .emplace_back (startNormal);

		face2 .emplace_back (startPoint);
	}

	// Check if there are degenerated faces.

	if (face1 .size () < 3)
		return selection;

	if (face2 .size () < 3)
		return selection;

	// Add new color faces.

	if (colorIndex () .size () and getColor ())
	{
		if (colorPerVertex ())
		{
			for (const auto & point : colorFace1)
				colorIndex () .emplace_back (point);

			colorIndex () .emplace_back (-1);

			for (const auto & point : colorFace2)
				colorIndex () .emplace_back (point);

			colorIndex () .emplace_back (-1);
		}
		else
		{
			colorIndex () .set1Value (numFaces + 0, getFaceColorIndex (faceNumber));
			colorIndex () .set1Value (numFaces + 1, getFaceColorIndex (faceNumber));
		}
	}

	// Add new tex faces.

	if (texCoordIndex () .size () and getTexCoord ())
	{
		for (const auto & point : texFace1)
			texCoordIndex () .emplace_back (point);

		texCoordIndex () .emplace_back (-1);

		for (const auto & point : texFace2)
			texCoordIndex () .emplace_back (point);

		texCoordIndex () .emplace_back (-1);
	}

	// Add new normal faces.

	if (normalIndex () .size () and getNormal ())
	{
		if (normalPerVertex ())
		{
			for (const auto & vector : normalFace1)
				normalIndex () .emplace_back (vector);

			normalIndex () .emplace_back (-1);

			for (const auto & vector : normalFace2)
				normalIndex () .emplace_back (vector);

			normalIndex () .emplace_back (-1);
		}
		else
		{
			normalIndex () .set1Value (numFaces + 0, getFaceNormalIndex (faceNumber));
			normalIndex () .set1Value (numFaces + 1, getFaceNormalIndex (faceNumber));
		}
	}

	// Add two new faces.

	for (const auto & point : face1)
		coordIndex () .emplace_back (point);

	coordIndex () .emplace_back (-1);

	for (const auto & point : face2)
		coordIndex () .emplace_back (point);

	coordIndex () .emplace_back (-1);

	// Insert a point on the edge of the adjacent faces

	if (startPoints .size () == 2)
	{
		for (const auto & face : adjacentFaces1)
			addPoint (cutFace, face, startPoint, cutEdge .first, fillIndices);
	}

	if (endPoints .size () == 2)
	{
		for (const auto & face : adjacentFaces2)
			addPoint (cutFace, face, endPoint, cutEdge .second, fillIndices);
	}

	// Invalidate old face.

	const size_t begin = vertices .front ();
	const size_t end   = vertices .back () + 1;

	fillIndices .emplace (faceNumber, end - begin);

	if (not colorPerVertex ())
	{
		for (const auto & pair : basic::make_reverse_range (fillIndices))
			colorIndex () .insert (colorIndex () .begin () + pair .first, pair .second, -1);
	}

	if (not normalPerVertex ())
	{
		for (const auto & pair : basic::make_reverse_range (fillIndices))
			normalIndex () .insert (normalIndex () .begin () + pair .first, pair .second, -1);
	}

	std::fill (coordIndex () .begin () + begin, coordIndex () .begin () + end, -1);

	// Store points.

	return { startPoint, endPoint };
}

// Insert a point on the edge of the adjacent faces
void
X3DIndexedFaceSetCutObject::addPoint (const size_t cutFace, const size_t face, const int32_t index, const Vector3d & point, std::map <size_t, size_t> & fillIndices)
{
	if (face == cutFace)
		return;

	const auto vertices   = getFaceSelection () -> getFaceVertices (face);
	const auto edge       = getFaceSelection () -> getClosestEdge (point, vertices);
	const auto faceNumber = getFaceSelection () -> getFaceNumber (vertices .front ());
		
	// Move adjacent face and add point

	for (const auto & vertex : vertices)
	{
		if (colorIndex () .size () and getColor ())
		{
			if (colorPerVertex ())
				colorIndex () .emplace_back (getVertexColorIndex (vertex));
		}

		if (texCoordIndex () .size () and getTexCoord ())
			texCoordIndex () .emplace_back (getVertexTexCoordIndex (vertex));

		if (normalIndex () .size () and getNormal ())
		{
			if (normalPerVertex ())
				normalIndex () .emplace_back (getVertexNormalIndex (vertex));
		}

		coordIndex () .emplace_back (coordIndex () [vertex]);

		if (vertex == edge .index0)
		{
			const auto  point1  = getCoord () -> get1Point (coordIndex () .get1Value (edge .index0));
			const auto  point2  = getCoord () -> get1Point (coordIndex () .get1Value (edge .index1));
			const float t       = abs (point - point1) / abs (point2 - point1);

			if (colorIndex () .size () and getColor ())
			{
			   if (colorPerVertex ())
			   {
					const auto colorIndex0 = colorIndex () .get1Value (edge .index0);
					const auto colorIndex1 = colorIndex () .get1Value (edge .index1);

					if (colorIndex0 == colorIndex1)
					{
						colorIndex () .emplace_back (colorIndex0);
					}
					else
					{
						const auto color1 = getColor () -> get1Color (colorIndex0);
						const auto color2 = getColor () -> get1Color (colorIndex1);
	
						const auto color = getColor () -> getSize ();
						getColor () -> set1Color (color, lerp (color1, color2, t)); // Interpolation in RGB space
						colorIndex () .emplace_back (color);
					}
				}
			}

			if (texCoordIndex () .size () and getTexCoord ())
			{
				const auto texCoord1 = getTexCoord () -> get1Point (getVertexTexCoordIndex (edge .index0));
				const auto texCoord2 = getTexCoord () -> get1Point (getVertexTexCoordIndex (edge .index1));

				const auto texCoord = getTexCoord () -> getSize ();
				getTexCoord () -> set1Point (texCoord, lerp (texCoord1, texCoord2, t));
				texCoordIndex () .emplace_back (texCoord);
			}

			if (normalIndex () .size () and getNormal ())
			{
			   if (normalPerVertex ())
			   {
					const auto normalIndex0 = getVertexNormalIndex (edge .index0);
					const auto normalIndex1 = getVertexNormalIndex (edge .index1);

					if (normalIndex0 == normalIndex1)
					{
						normalIndex () .emplace_back (normalIndex0);
					}
					else
					{
						const auto normal1 = getNormal () -> get1Vector (normalIndex0);
						const auto normal2 = getNormal () -> get1Vector (normalIndex1);
	
						const auto normal = getNormal () -> getSize ();
						getNormal () -> set1Vector (normal, lerp (normal1, normal2, t));
						normalIndex () .emplace_back (normal);
					}
				}
			}

			coordIndex () .emplace_back (index);
		}
	}

	if (colorIndex () .size () and getColor ())
	{
		if (colorPerVertex ())
			colorIndex () .emplace_back (-1);
		else
			colorIndex () .emplace_back (getFaceColorIndex (faceNumber));
	}

	if (texCoordIndex () .size () and getTexCoord ())
		texCoordIndex () .emplace_back (-1);

	if (normalIndex () .size () and getNormal ())
	{
	   if (normalPerVertex ())
			normalIndex () .emplace_back (-1);
		else
			normalIndex () .emplace_back (getFaceNormalIndex (faceNumber));
	}

	coordIndex () .emplace_back (-1);

	// Invalidate old face.

	const size_t begin = vertices .front ();
	const size_t end   = vertices .back () + 1;

	fillIndices .emplace (faceNumber, end - begin);

	std::fill (coordIndex () .begin () + begin, coordIndex () .begin () + end, -1);
}

/**
 *  Cuts all faces into two connected faces. Run rebuildIndices, rebuildColor, rebuildTexCoord, rebuildNormal and
 *  rebuildCoord after this operation to remove degenerated faces.
 *
 *  @param  cutFaceArray   Index of faces to cut
 *  @param  cutEdgeArray   Coordinates of edges to cut
 *  @param  edgesArray     Array of one or two points, depending on whether end at point or edge. One array for start and one for end point or edge.
 */

std::vector <int32_t>
X3DIndexedFaceSetCutObject::cut (const std::vector <size_t> & cutFaceArray,
                                 const std::vector <std::vector <Vector3d>> & cutEdgeArray,
                                 const std::vector <std::vector <std::vector <int32_t>>> & edgesArray)
{
	auto selection   = std::vector <int32_t> ();
	auto colors      = std::map <std::pair <std::pair <int32_t, int32_t>, std::pair <int32_t, int32_t>>, int32_t> ();
	auto texCoords   = std::map <std::pair <std::pair <int32_t, int32_t>, std::pair <int32_t, int32_t>>, int32_t> ();
	auto normals     = std::map <std::pair <std::pair <int32_t, int32_t>, std::pair <int32_t, int32_t>>, int32_t> ();
	auto points      = std::map <std::pair <int32_t, int32_t>, int32_t> ();
	auto numFaces    = getFaceSelection () -> getNumFaces ();
	auto fillIndices = std::map <size_t, size_t> ();

	for (size_t f = 0, numCutFaces = cutFaceArray .size (); f < numCutFaces; ++ f)
	{
		if (edgesArray [f] .size () not_eq 2)
			continue;

		const auto & cutFace     = cutFaceArray [f];
		const auto & cutEdge     = cutEdgeArray [f];
		const auto & startPoints = edgesArray   [f] [0];
		const auto & endPoints   = edgesArray   [f] [1];

		const auto faceNumber = getFaceSelection () -> getFaceNumber (cutFace);
		const auto vertices   = getFaceSelection () -> getFaceVertices (cutFace);
		const auto startEdge  = getFaceSelection () -> getClosestEdge (cutEdge [0], vertices);
		const auto endEdge    = getFaceSelection () -> getClosestEdge (cutEdge [1], vertices);

		int32_t startColor    = -1;
		int32_t endColor      = -1;
		int32_t startTexCoord = -1;
		int32_t endTexCoord   = -1;
		int32_t startNormal   = -1;
		int32_t endNormal     = -1;
		int32_t startPoint    = -1;
		int32_t endPoint      = -1;
		size_t  startVertex1  = -1;
		size_t  startVertex2  = -1;
		size_t  endVertex1    = -1;
		size_t  endVertex2    = -1;
	
		std::vector <int32_t> colorFace1;
		std::vector <int32_t> colorFace2;
		std::vector <int32_t> texFace1;
		std::vector <int32_t> texFace2;
		std::vector <int32_t> normalFace1;
		std::vector <int32_t> normalFace2;
		std::vector <int32_t> face1;
		std::vector <int32_t> face2;
	
		switch (startPoints .size ())
		{
			case 1:
			{
				startPoint   = startPoints [0];
				startVertex1 = getFaceSelection () -> getClosestVertex (getCoord () -> get1Point (startPoints [0]), vertices);
				endVertex2   = startVertex1;
				break;
			}
			case 2:
			{
				const auto  point1  = getCoord () -> get1Point (coordIndex () .get1Value (startEdge .index0));
				const auto  point2  = getCoord () -> get1Point (coordIndex () .get1Value (startEdge .index1));
				const auto  segment = LineSegment3d (point1, point2);
				const float t       = abs (cutEdge [0] - point1) / abs (point2 - point1);
	
				if (not segment .is_between (cutEdge [0]))
					continue;
	
				const auto sortedPoints = std::minmax (startPoints [0], startPoints [1]);

				if (colorIndex () .size () and getColor ())
				{
				   if (colorPerVertex ())
				   {
						const int32_t point0 = getVertexColorIndex (startEdge .index0);
						const int32_t point1 = getVertexColorIndex (startEdge .index1);
						const auto    iter   = colors .find (std::pair (sortedPoints, std::minmax (point0, point1)));

						if (iter == colors .end ())
						{
							if (point0 == point1)
							{
								startColor = point0;
							}
							else
							{
								const auto color0 = getColor () -> get1Color (point0);
								const auto color1 = getColor () -> get1Color (point1);
			
								startColor = getColor () -> getSize ();
								getColor () -> set1Color (startColor, lerp (color0, color1, t)); // Interpolation in RGB space
		
								colors .emplace (std::pair (sortedPoints, std::minmax (point0, point1)), startColor);
							}
						}
						else
						{
							startColor = iter -> second;
						}
					}
				}
	
				if (texCoordIndex () .size () and getTexCoord ())
				{
					const int32_t point0 = getVertexTexCoordIndex (startEdge .index0);
					const int32_t point1 = getVertexTexCoordIndex (startEdge .index1);
					const auto    iter   = texCoords .find (std::pair (sortedPoints, std::minmax (point0, point1)));

					if (iter == texCoords .end ())
					{
						const auto texCoord0 = getTexCoord () -> get1Point (point0);
						const auto texCoord1 = getTexCoord () -> get1Point (point1);
		
						startTexCoord = getTexCoord () -> getSize ();
						getTexCoord () -> set1Point (startTexCoord, lerp (texCoord0, texCoord1, t));
	
						texCoords .emplace (std::pair (sortedPoints, std::minmax (point0, point1)), startTexCoord);
					}
					else
					{
						startTexCoord = iter -> second;
					}
				}
	
				if (normalIndex () .size () and getNormal ())
				{
				   if (normalPerVertex ())
				   {
						const int32_t point0 = getVertexNormalIndex (startEdge .index0);
						const int32_t point1 = getVertexNormalIndex (startEdge .index1);
						const auto    iter   = normals .find (std::pair (sortedPoints, std::minmax (point0, point1)));

						if (iter == normals .end ())
						{
							if (point0 == point1)
							{
								startNormal = point0;
							}
							else
							{
								const auto normal0 = getNormal () -> get1Vector (point0);
								const auto normal1 = getNormal () -> get1Vector (point1);
			
								startNormal = getNormal () -> getSize ();
								getNormal () -> set1Vector (startNormal, lerp (normal0, normal1, t));
	
								normals .emplace (std::pair (sortedPoints, std::minmax (point0, point1)), startNormal);
							}
						}
						else
						{
							startNormal = iter -> second;
						}
					}
				}
				
				const auto iter = points .find (sortedPoints);

				if (iter == points .end ())
				{
					startPoint = getCoord () -> getSize ();

					if (getFogCoord ())
					{
						const auto depth1 = getFogCoord () -> get1Depth (coordIndex () .get1Value (startEdge .index0));
						const auto depth2 = getFogCoord () -> get1Depth (coordIndex () .get1Value (startEdge .index1));
					
						getFogCoord () -> set1Depth (startPoint, lerp (depth1, depth2, t));
					}

					getCoord () -> set1Point (startPoint, cutEdge [0]);

					points .emplace (sortedPoints, startPoint);
				}
				else
				{
					startPoint = iter -> second;
				}
	
				startVertex1 = startEdge .index1;
				endVertex2   = startEdge .index0;
				break;
			}
			default:
				continue;
		}
	
		switch (endPoints .size ())
		{
			case 1:
			{
				endPoint     = endPoints [0];
				endVertex1   = getFaceSelection () -> getClosestVertex (getCoord () -> get1Point (endPoints [0]), vertices);
				startVertex2 = endVertex1;
				break;
			}
			case 2:
			{
				const auto  point1  = getCoord () -> get1Point (coordIndex () .get1Value (endEdge .index0));
				const auto  point2  = getCoord () -> get1Point (coordIndex () .get1Value (endEdge .index1));
				const auto  segment = LineSegment3d (point1, point2);
				const float t       = abs (cutEdge [1] - point1) / abs (point2 - point1);
	
				if (not segment .is_between (cutEdge [1]))
					continue;

				const auto sortedPoints = std::minmax (endPoints [0], endPoints [1]);

				if (colorIndex () .size () and getColor ())
				{
				   if (colorPerVertex ())
				   {
						const int32_t point0 = getVertexColorIndex (endEdge .index0);
						const int32_t point1 = getVertexColorIndex (endEdge .index1);
						const auto    iter   = colors .find (std::pair (sortedPoints, std::minmax (point0, point1)));

						if (iter == colors .end ())
						{
							if (point0 == point1)
							{
								endColor = point0;
							}
							else
							{
								const auto color0 = getColor () -> get1Color (point0);
								const auto color1 = getColor () -> get1Color (point1);
			
								endColor = getColor () -> getSize ();
								getColor () -> set1Color (endColor, lerp (color0, color1, t)); // Interpolation in RGB space
		
								colors .emplace (std::pair (sortedPoints, std::minmax (point0, point1)), endColor);
							}
						}
						else
						{
							endColor = iter -> second;
						}
					}
				}

				if (texCoordIndex () .size () and getTexCoord ())
				{
					const int32_t point0 = getVertexTexCoordIndex (endEdge .index0);
					const int32_t point1 = getVertexTexCoordIndex (endEdge .index1);
					const auto    iter   = texCoords .find (std::pair (sortedPoints, std::minmax (point0, point1)));

					if (iter == texCoords .end ())
					{
						const auto texCoord0 = getTexCoord () -> get1Point (point0);
						const auto texCoord1 = getTexCoord () -> get1Point (point1);
		
						endTexCoord = getTexCoord () -> getSize ();
						getTexCoord () -> set1Point (endTexCoord, lerp (texCoord0, texCoord1, t));
	
						texCoords .emplace (std::pair (sortedPoints, std::minmax (point0, point1)), endTexCoord);
					}
					else
					{
						endTexCoord = iter -> second;
					}
				}
	
				if (normalIndex () .size () and getNormal ())
				{
				   if (normalPerVertex ())
				   {
						const int32_t point0 = getVertexNormalIndex (endEdge .index0);
						const int32_t point1 = getVertexNormalIndex (endEdge .index1);
						const auto    iter   = normals .find (std::pair (sortedPoints, std::minmax (point0, point1)));

						if (iter == normals .end ())
						{
							if (point0 == point1)
							{
								endNormal = point0;
							}
							else
							{
								const auto normal0 = getNormal () -> get1Vector (point0);
								const auto normal1 = getNormal () -> get1Vector (point1);
			
								endNormal = getNormal () -> getSize ();
								getNormal () -> set1Vector (endNormal, lerp (normal0, normal1, t));
		
								normals .emplace (std::pair (sortedPoints, std::minmax (point0, point1)), endNormal);
							}
						}
						else
						{
							endNormal = iter -> second;
						}
					}
				}
			
				const auto iter = points .find (sortedPoints);

				if (iter == points .end ())
				{
					endPoint = getCoord () -> getSize ();

					if (getFogCoord ())
					{
						const auto depth1 = getFogCoord () -> get1Depth (coordIndex () .get1Value (endEdge .index0));
						const auto depth2 = getFogCoord () -> get1Depth (coordIndex () .get1Value (endEdge .index1));

						getFogCoord () -> set1Depth (endPoint, lerp (depth1, depth2, t));
					}

					getCoord () -> set1Point (endPoint, cutEdge [1]);

					points .emplace (sortedPoints, endPoint);
				}
				else
				{
					endPoint = iter -> second;
				}

				endVertex1   = endEdge .index0;
				startVertex2 = endEdge .index1;
				break;
			}
			default:
				continue;
		}
	
		// Rewrite indices.
	
		startVertex1 = std::find (vertices .cbegin (), vertices .cend (), startVertex1) - vertices .cbegin ();
		startVertex2 = std::find (vertices .cbegin (), vertices .cend (), startVertex2) - vertices .cbegin ();
		endVertex1   = std::find (vertices .cbegin (), vertices .cend (), endVertex1)   - vertices .cbegin ();
		endVertex2   = std::find (vertices .cbegin (), vertices .cend (), endVertex2)   - vertices .cbegin ();
	
		if (endVertex1 < startVertex1)
			endVertex1 += vertices .size ();
	
		if (endVertex2 < startVertex2)
			endVertex2 += vertices .size ();	
	
		// Add start points.
	
		if (startPoints .size () == 2)
		{
			if (colorIndex () .size () and getColor ())
				if (colorPerVertex ())
					colorFace1 .emplace_back (startColor);
	
			if (texCoordIndex () .size () and getTexCoord ())
				texFace1 .emplace_back (startTexCoord);
	
			if (normalIndex () .size () and getNormal ())
				if (normalPerVertex ())
					normalFace1 .emplace_back (startNormal);
	
			face1 .emplace_back (startPoint);
		}
	
		if (endPoints .size () == 2)
		{
			if (colorIndex () .size () and getColor ())
				if (colorPerVertex ())
					colorFace2 .emplace_back (endColor);
	
			if (texCoordIndex () .size () and getTexCoord ())
				texFace2 .emplace_back (endTexCoord);
	
			if (normalIndex () .size () and getNormal ())
				if (normalPerVertex ())
					normalFace2 .emplace_back (endNormal);
	
			face2 .emplace_back (endPoint);
		}
	
		// Add face points.
	
		for (size_t i = startVertex1; i <= endVertex1; ++ i)
		{
			const auto vertex = vertices [i % vertices .size ()];
	
			if (colorIndex () .size () and getColor ())
				if (colorPerVertex ())
					colorFace1 .emplace_back (getVertexColorIndex (vertex));
	
			if (texCoordIndex () .size () and getTexCoord ())
				texFace1 .emplace_back (getVertexTexCoordIndex (vertex));
	
			if (normalPerVertex ())
				if (normalIndex () .size () and getNormal ())
					normalFace1 .emplace_back (getVertexNormalIndex (vertex));
	
			face1 .emplace_back (coordIndex () .get1Value (vertex));
		}
	
		for (size_t i = startVertex2; i <= endVertex2; ++ i)
		{
			const auto vertex = vertices [i % vertices .size ()];
	
			if (colorIndex () .size () and getColor ())
				if (colorPerVertex ())
					colorFace2 .emplace_back (getVertexColorIndex (vertex));
	
			if (texCoordIndex () .size () and getTexCoord ())
				texFace2 .emplace_back (getVertexTexCoordIndex (vertex));
	
			if (normalIndex () .size () and getNormal ())
				if (normalPerVertex ())
					normalFace2 .emplace_back (getVertexNormalIndex (vertex));
	
			face2 .emplace_back (coordIndex () .get1Value (vertex));
		}
	
		// Add end points.
		
		if (endPoints .size () == 2)
		{
			if (colorIndex () .size () and getColor ())
				if (colorPerVertex ())
					colorFace1 .emplace_back (endColor);
	
			if (texCoordIndex () .size () and getTexCoord ())
				texFace1 .emplace_back (endTexCoord);
	
			if (normalIndex () .size () and getNormal ())
				if (normalPerVertex ())
					normalFace1 .emplace_back (endNormal);
	
			face1 .emplace_back (endPoint);
		}
		
		if (startPoints .size () == 2)
		{
			if (colorIndex () .size () and getColor ())
				if (colorPerVertex ())
					colorFace2 .emplace_back (startColor);
	
			if (texCoordIndex () .size () and getTexCoord ())
				texFace2 .emplace_back (startTexCoord);
	
			if (normalIndex () .size () and getNormal ())
				if (normalPerVertex ())
					normalFace2 .emplace_back (startNormal);
	
			face2 .emplace_back (startPoint);
		}
	
		// Check if there are degenerated faces.
	
		if (face1 .size () < 3)
			continue;
	
		if (face2 .size () < 3)
			continue;
	
		// Add new color faces.
	
		if (colorIndex () .size () and getColor ())
		{
			if (colorPerVertex ())
			{
				for (const auto & point : colorFace1)
					colorIndex () .emplace_back (point);
	
				colorIndex () .emplace_back (-1);
	
				for (const auto & point : colorFace2)
					colorIndex () .emplace_back (point);
	
				colorIndex () .emplace_back (-1);
			}
			else
			{
				colorIndex () .set1Value (numFaces + 0, getFaceColorIndex (faceNumber));
				colorIndex () .set1Value (numFaces + 1, getFaceColorIndex (faceNumber));
			}
		}
	
		// Add new tex faces.
	
		if (texCoordIndex () .size () and getTexCoord ())
		{
			for (const auto & point : texFace1)
				texCoordIndex () .emplace_back (point);
	
			texCoordIndex () .emplace_back (-1);
	
			for (const auto & point : texFace2)
				texCoordIndex () .emplace_back (point);
	
			texCoordIndex () .emplace_back (-1);
		}
	
		// Add new normal faces.
	
		if (normalIndex () .size () and getNormal ())
		{
			if (normalPerVertex ())
			{
				for (const auto & vector : normalFace1)
					normalIndex () .emplace_back (vector);
	
				normalIndex () .emplace_back (-1);
	
				for (const auto & vector : normalFace2)
					normalIndex () .emplace_back (vector);
	
				normalIndex () .emplace_back (-1);
			}
			else
			{
				normalIndex () .set1Value (numFaces + 0, getFaceNormalIndex (faceNumber));
				normalIndex () .set1Value (numFaces + 1, getFaceNormalIndex (faceNumber));
			}
		}
	
		// Add two new faces.
	
		for (const auto & point : face1)
			coordIndex () .emplace_back (point);
	
		coordIndex () .emplace_back (-1);
	
		for (const auto & point : face2)
			coordIndex () .emplace_back (point);
	
		coordIndex () .emplace_back (-1);

		numFaces += 2;

		// Invalidate old face.
	
		const size_t begin = vertices .front ();
		const size_t end   = vertices .back () + 1;

		fillIndices .emplace (faceNumber, end - begin);

		std::fill (coordIndex () .begin () + begin, coordIndex () .begin () + end, -1);

		// Store points.

		selection .emplace_back (startPoint);
		selection .emplace_back (endPoint);
	}
	
	// Invalidate old face.

	if (not colorPerVertex ())
	{
		for (const auto & pair : basic::make_reverse_range (fillIndices))
			colorIndex () .insert (colorIndex () .begin () + pair .first, pair .second, -1);
	}

	if (not normalPerVertex ())
	{
		for (const auto & pair : basic::make_reverse_range (fillIndices))
			normalIndex () .insert (normalIndex () .begin () + pair .first, pair .second, -1);
	}

	// 

	std::sort (selection .begin (), selection .end ());

	selection .erase (std::unique (selection .begin (), selection .end ()), selection .end ());

	return selection;
}

X3DIndexedFaceSetCutObject::~X3DIndexedFaceSetCutObject ()
{ }

} // X3D
} // titania

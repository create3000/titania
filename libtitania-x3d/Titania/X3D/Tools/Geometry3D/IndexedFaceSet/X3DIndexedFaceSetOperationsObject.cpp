/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DIndexedFaceSetOperationsObject.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Components/Shape/Shape.h"
#include "../../../Editing/Selection/FaceSelection.h"
#include "../../../Editing/Combine.h"
#include "../../../Editing/Editor.h"

#include <Titania/Utility/Map.h>

namespace titania {
namespace X3D {

X3DIndexedFaceSetOperationsObject::Fields::Fields () :
	     cutSelectedFaces (new SFTime ()),
	    copySelectedFaces (new SFTime ()),
	           pasteFaces (new SFString ()),
	          mergePoints (new SFTime ()),
	          splitPoints (new SFTime ()),
	          formNewFace (new SFTime ()),
	 extrudeSelectedEdges (new SFTime ()),
	 extrudeSelectedFaces (new SFTime ()),
	  chipOfSelectedFaces (new SFTime ()),
	   flipVertexOrdering (new SFTime ()),
	  deleteSelectedFaces (new SFTime ()),
	    clipboard_changed (new SFString ())
{ }

X3DIndexedFaceSetOperationsObject::X3DIndexedFaceSetOperationsObject () :
	                  IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetSelectionObject ()
{
	addType (X3DConstants::X3DIndexedFaceSetOperationsObject);
}

void
X3DIndexedFaceSetOperationsObject::initialize ()
{
	cutSelectedFaces ()     .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_cutSelectedFaces);
	copySelectedFaces ()    .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_copySelectedFaces);
	pasteFaces ()           .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_pasteFaces);
	mergePoints ()          .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_mergePoints);
	splitPoints ()          .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_splitPoints);
	formNewFace ()          .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_formNewFace);
	extrudeSelectedEdges () .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_extrudeSelectedEdges);
	extrudeSelectedFaces () .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_extrudeSelectedFaces);
	chipOfSelectedFaces ()  .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_chipOfSelectedFaces);
	flipVertexOrdering ()   .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_flipVertexOrdering);
	deleteSelectedFaces ()  .addInterest (this, &X3DIndexedFaceSetOperationsObject::set_deleteSelectedFaces);
}

void
X3DIndexedFaceSetOperationsObject::set_cutSelectedFaces ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Cut Selected Faces"));

	set_copySelectedFaces ();
	deleteSelectedFaces (undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_copySelectedFaces ()
{
	const auto transformationMatrix = Editor () .getModelViewMatrix (getMasterScene (), this);
	const auto geometry             = X3DPtr <IndexedFaceSet> (new IndexedFaceSet (getExecutionContext ()));

	geometry -> solid ()           = solid ();
	geometry -> ccw ()             = ccw ();
	geometry -> convex ()          = convex ();
	geometry -> creaseAngle ()     = creaseAngle ();
	geometry -> colorPerVertex ()  = colorPerVertex ();
	geometry -> normalPerVertex () = normalPerVertex ();

	// Generate nodes, we do not setup the nodes as it is not needed for stringification.

	X3DPtr <X3DColorNode> color;
	X3DPtr <X3DTextureCoordinateNode> texCoord;
	X3DPtr <X3DNormalNode> normal;
	X3DPtr <X3DCoordinateNode> coord;

	if (getColor ())
	{
		geometry -> color () = getColor () -> create (getExecutionContext ());
		color = geometry -> color ();
	}

	if (getTexCoord ())
	{
		geometry -> texCoord () = getTexCoord () -> create (getExecutionContext ());
		texCoord = geometry -> texCoord ();
	}

	if (getNormal ())
	{
		geometry -> normal () = getNormal () -> create (getExecutionContext ());
		normal = geometry -> normal ();
	}
		
	if (getCoord ())
	{
		geometry -> coord () = getCoord () -> create (getExecutionContext ());
		coord = geometry -> coord ();
	}

	// Generate mappings arrays.

	std::map <int32_t, int32_t> colorArray;
	std::map <int32_t, int32_t> texCoordArray;
	std::map <int32_t, int32_t> normalArray;
	std::map <int32_t, int32_t> coordArray;

	for (const auto & face : getSelectedFaces ())
	{
		const auto faceNumber = getFaceSelection () -> getFaceNumber (face);
		const auto vertices   = getFaceSelection () -> getFaceVertices (face);

		for (const auto & vertex : vertices)
		{
			if (color)
			{
				if (colorPerVertex ())
					colorArray .emplace (getVertexColorIndex (vertex), colorArray .size ());
				else
					colorArray .emplace (getFaceColorIndex (faceNumber), colorArray .size ());
			}

			if (texCoord)
				texCoordArray .emplace (getVertexTexCoordIndex (vertex), texCoordArray .size ());

			if (normal)
			{
				if (normalPerVertex ())
					normalArray .emplace (getVertexNormalIndex (vertex), normalArray .size ());
				else
					normalArray .emplace (getFaceNormalIndex (faceNumber), normalArray .size ());
			}

			coordArray .emplace (coordIndex () [vertex], coordArray .size ());
		}
	}

	// Generate indices

	for (const auto & face : getSelectedFaces ())
	{
		const auto vertices   = getFaceSelection () -> getFaceVertices (face);
		const auto faceNumber = getFaceSelection () -> getFaceNumber (face);

		for (const auto & vertex : vertices)
		{
		   const auto point = coordArray [coordIndex () [vertex]];

			if (color)
			{
				if (colorPerVertex ())
				{
					try
					{
						geometry -> colorIndex () .emplace_back (colorArray .at (getVertexColorIndex (vertex)));
					}
					catch (const std::out_of_range &)
					{
						geometry -> colorIndex () .emplace_back (point);
					}
				}
			}
	
			if (texCoord)
			{
				try
				{
					geometry -> texCoordIndex () .emplace_back (texCoordArray .at (getVertexTexCoordIndex (vertex)));
				}
				catch (const std::out_of_range &)
				{
					geometry -> texCoordIndex () .emplace_back (point);
				}
			}
	
			if (normal)
			{
				if (normalPerVertex ())
				{
					try
					{
						geometry -> normalIndex () .emplace_back (normalArray .at (getVertexNormalIndex (vertex)));
					}
					catch (const std::out_of_range &)
					{
						geometry -> normalIndex () .emplace_back (point);
					}
				}
			}
	
			geometry -> coordIndex () .emplace_back (point);
		}

		if (color)
		{
			if (colorPerVertex ())
			   geometry -> colorIndex () .emplace_back (-1);

			else
			{
				try
				{
					geometry -> colorIndex () .emplace_back (colorArray .at (getFaceColorIndex (faceNumber)));
				}
				catch (const std::out_of_range &)
				{
					geometry -> colorIndex () .emplace_back (faceNumber);
				}
			}
		}
	
		if (texCoord)
			geometry -> texCoordIndex () .emplace_back (-1);
	
		if (normal)
		{
			if (normalPerVertex ())
			   geometry -> normalIndex () .emplace_back (-1);

			else
			{
				try
				{
					geometry -> normalIndex () .emplace_back (normalArray .at (getFaceNormalIndex (faceNumber)));
				}
				catch (const std::out_of_range &)
				{
					geometry -> normalIndex () .emplace_back (faceNumber);
				}
			}
		}
	
		geometry -> coordIndex () .emplace_back (-1);
	}

	// Generate points

	if (color)
	{
		for (const auto & index : basic::reverse (colorArray))
			color -> set1Color (color -> getSize (), getColor () -> get1Color (index .second));
	}

	if (texCoord)
	{
		for (const auto & index : basic::reverse (texCoordArray))
			texCoord -> set1Point (texCoord -> getSize (), getTexCoord () -> get1Point (index .second));
	}

	if (normal)
	{
		for (const auto & index : basic::reverse (normalArray))
			normal -> set1Vector (normal -> getSize (), getNormal () -> get1Vector (index .second));
	}

	if (coord)
	{
		for (const auto & index : basic::reverse (coordArray))
			coord -> set1Point (coord -> getSize (), getCoord () -> get1Point (index .second) * transformationMatrix);
	}

	// Print out geometry

	const auto transform = getExecutionContext () -> createNode <Transform> ();
	const auto shape     = getExecutionContext () -> createNode <Shape> ();

	transform -> children () .emplace_back (shape);
	shape -> geometry () = geometry;

	clipboard_changed () = transform -> toString ();
}

void
X3DIndexedFaceSetOperationsObject::set_pasteFaces ()
{
	try
	{
		const auto undoStep     = std::make_shared <X3D::UndoStep> (_ ("Paste Faces"));
		const auto scene        = getBrowser () -> createX3DFromString (pasteFaces ());
		auto       geometries   = Editor () .getNodes <IndexedFaceSet> (scene -> getRootNodes (), { X3DConstants::IndexedFaceSet });
		const auto targetMatrix = ~Editor () .getModelViewMatrix (getMasterScene (), this);

		undoRestoreSelection (undoStep);
		undoSetColorIndex    (undoStep);
		undoSetTexCoordIndex (undoStep);
		undoSetNormalIndex   (undoStep);
		undoSetCoordIndex    (undoStep);
		undoSetColorColor    (undoStep);
		undoSetTexCoordPoint (undoStep);
		undoSetNormalVector  (undoStep);
		undoSetCoordPoint    (undoStep);

		const auto selection = Combine () .combine (getExecutionContext (), geometries, this, getCoord (), targetMatrix);

		replaceSelection () .assign (selection .begin (), selection .end ());

		redoSetCoordPoint    (undoStep);
		redoSetNormalVector  (undoStep);
		redoSetTexCoordPoint (undoStep);
		redoSetColorColor    (undoStep);
		redoSetCoordIndex    (undoStep);
		redoSetNormalIndex   (undoStep);
		redoSetTexCoordIndex (undoStep);
		redoSetColorIndex    (undoStep);
		redoRestoreSelection (selection, undoStep);

		undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DIndexedFaceSetOperationsObject::set_mergePoints ()
{
	int32_t masterPoint = getMasterPoint ();

	if (masterPoint < 0)
	   return;

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Merge Points"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetColorColor    (undoStep);
	undoSetTexCoordPoint (undoStep);
	undoSetNormalVector  (undoStep);
	undoSetCoordPoint    (undoStep);

	// Normaly not needed.
	//	if (getColor () and colorIndex () .empty ())
	//		colorIndex () = coordIndex ();
	//
	//	if (getTexCoord () and texCoordIndex () .empty ())
	//		texCoordIndex () = coordIndex ();
	//
	//	if (getNormal () and normalIndex () .empty ())
	//		normalIndex () = coordIndex ();

	std::vector <int32_t> pointsToRemove;

	for (const auto & selectedPoint : getSelectedPoints ())
	{
		const auto & index   = selectedPoint .first;
		const auto   indices = getFaceSelection () -> getSharedVertices (index);
		
		if (indices .empty ())
			continue;
		
		for (const auto & index : indices)
		{
			pointsToRemove .emplace_back (coordIndex () [index]);

			coordIndex () [index] = masterPoint;
		}
	}

	if (getCoord () -> getCloneCount () == 1)
	{
		// Erase points.

		const auto unique  = std::unique (pointsToRemove .begin (), pointsToRemove .end ());
		const auto removed = std::remove (pointsToRemove .begin (), unique, masterPoint);
	 
		pointsToRemove .erase (removed, pointsToRemove .end ());

		std::sort (pointsToRemove .begin (), pointsToRemove .end ());

		masterPoint -= std::lower_bound (pointsToRemove .begin (), pointsToRemove .end (), masterPoint) - pointsToRemove .begin ();

		erasePoints (pointsToRemove);
	}

	// Remove degenerated edges and faces.
	rebuildIndices  ();
	rebuildColor    ();
	rebuildTexCoord ();
	rebuildNormal   ();
	rebuildCoord    ();

	replaceSelection () = { masterPoint };

	redoSetCoordPoint    (undoStep);
	redoSetNormalVector  (undoStep);
	redoSetTexCoordPoint (undoStep);
	redoSetColorColor    (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);
	redoRestoreSelection ({ masterPoint }, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_splitPoints ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Split Points"));

	undoRestoreSelection (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetCoordPoint    (undoStep);

	std::set <int32_t> points;

	for (const auto & selectedPoint : getSelectedPoints ())
		points .emplace (selectedPoint .first);

	const auto selection = splitPoints (points);

	replaceSelection () .assign (selection .begin (), selection .end ());

	redoSetCoordPoint    (undoStep);
	redoSetCoordIndex    (undoStep);
	redoRestoreSelection (selection, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_formNewFace ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Form New Face From Selected Holes"));

	undoRestoreSelection (undoStep);
	if (colorIndex    () .size ()) undoSetColorIndex    (undoStep);
	if (texCoordIndex () .size ()) undoSetTexCoordIndex (undoStep);
	if (normalIndex   () .size ()) undoSetNormalIndex   (undoStep);
	undoSetCoordIndex (undoStep);

	const auto selection = formNewFace (getSelectedHoles ());

	replaceSelectedFaces () .assign (selection .begin (), selection .end ());

	redoSetCoordIndex (undoStep);
	if (normalIndex   () .size ()) redoSetNormalIndex   (undoStep);
	if (texCoordIndex () .size ()) redoSetTexCoordIndex (undoStep);
	if (colorIndex    () .size ()) redoSetColorIndex    (undoStep);
	redoRestoreSelectedFaces (selection, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_extrudeSelectedEdges ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Extrude Selected Edges"));
	undoRestoreSelection (undoStep);
	if (colorIndex    () .size ()) undoSetColorIndex    (undoStep);
	if (texCoordIndex () .size ()) undoSetTexCoordIndex (undoStep);
	if (normalIndex   () .size ()) undoSetNormalIndex   (undoStep);
	undoSetCoordIndex (undoStep);
	undoSetCoordPoint (undoStep);

	std::set <std::pair <size_t, size_t>> edges;

	for (const auto & edge : getSelectedEdges ())
	{
		if ((getSelectionType () == SelectionType::FACES and edge .second .size () not_eq 1 and getSelectedFaces () .size ()) or edge .second .empty ())
			continue;

	   for (const auto & pair : edge .second)
	   {
			if (pair .second - pair .first > 1)
				edges .emplace (pair .second, pair .first);
			else
				edges .emplace (pair);

			break;
		}
	}

	const auto selection = extrudeSelectedEdges (edges, { });

	replaceSelectedEdges () .assign (selection .begin (), selection .end ());

	redoSetCoordPoint (undoStep);
	redoSetCoordIndex (undoStep);
	if (normalIndex   () .size ()) redoSetNormalIndex   (undoStep);
	if (texCoordIndex () .size ()) redoSetTexCoordIndex (undoStep);
	if (colorIndex    () .size ()) redoSetColorIndex    (undoStep);
	redoRestoreSelectedEdges (selection, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_extrudeSelectedFaces ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Extrude Selected Faces"));

	undoRestoreSelection (undoStep);
	if (colorIndex    () .size ()) undoSetColorIndex    (undoStep);
	if (texCoordIndex () .size ()) undoSetTexCoordIndex (undoStep);
	if (normalIndex   () .size ()) undoSetNormalIndex   (undoStep);
	undoSetCoordIndex (undoStep);
	undoSetCoordPoint (undoStep);

	std::set <std::pair <size_t, size_t>> edges;
	std::set <int32_t>                    points;

	for (const auto & edge : getSelectedEdges ())
	{
		if ((getSelectionType () == SelectionType::FACES and edge .second .size () not_eq 1) or edge .second .empty ())
			continue;

		points .emplace (edge .first .first);
		points .emplace (edge .first .second);

	   for (const auto & pair : edge .second)
		{
			if (pair .second - pair .first > 1)
				edges .emplace (pair .second, pair .first);
			else
				edges .emplace (pair);
		}
	}

	//

	std::set <int32_t> facesPoints;
	std::set <int32_t> selection;

	const auto extrudesPoints = extrudeSelectedEdges (edges, getSelectedFaces ());

	for (const auto & face : getSelectedFaces ())
	{
		for (const auto & vertex : getFaceSelection () -> getFaceVertices (face))
		   facesPoints .emplace (coordIndex () [vertex]);
	}

	std::set_difference (facesPoints .begin (), facesPoints .end (), points .begin (), points .end (), std::inserter (selection, selection .begin ()));

	selection .insert (extrudesPoints .begin (), extrudesPoints .end ());

	replaceSelection () .assign (selection .begin (), selection .end ());

	//

	rebuildCoord ();

	redoSetCoordPoint (undoStep);
	redoSetCoordIndex (undoStep);
	if (normalIndex   () .size ()) redoSetNormalIndex   (undoStep);
	if (texCoordIndex () .size ()) redoSetTexCoordIndex (undoStep);
	if (colorIndex    () .size ()) redoSetColorIndex    (undoStep);
	redoRestoreSelectedEdges (std::vector <int32_t> (selection .begin (), selection .end ()), undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_chipOfSelectedFaces ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Chip Of Selected Faces"));

	undoRestoreSelection (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetCoordPoint    (undoStep);

	std::set <size_t> vertices;

	for (const auto & edge : getSelectedEdges ())
	{
	   for (const auto & pair : edge .second)
		{
			vertices .emplace (pair .first);
			vertices .emplace (pair .second);
		}
	}

	chipOf (vertices);

	std::vector <int32_t> selection;

	for (const auto & face : getSelectedFaces ())
	{
		for (const auto & vertex : getFaceSelection () -> getFaceVertices (face))
			selection .emplace_back (coordIndex () [vertex]);
	}

	replaceSelection () .assign (selection .begin (), selection .end ());

	redoSetCoordPoint    (undoStep);
	redoSetCoordIndex    (undoStep);
	redoRestoreSelection (selection, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_flipVertexOrdering ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Flip Vertex Ordering"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetNormalVector  (undoStep);

	flipVertexOrdering (getSelectedFaces ());

	// Set selection

	std::vector <int32_t> selection;

	for (const auto & point : getSelectedPoints ())
	   selection .emplace_back (point .first);

	replaceSelectedFaces () .assign (selection .begin (), selection .end ());

	// Redo

	redoSetNormalVector  (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);
	redoRestoreSelectedFaces (selection, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_deleteSelectedFaces ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Remove Selected Faces"));

	deleteSelectedFaces (undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::deleteSelectedFaces (const UndoStepPtr & undoStep)
{
	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetColorColor    (undoStep);
	undoSetTexCoordPoint (undoStep);
	undoSetNormalVector  (undoStep);
	undoSetCoordPoint    (undoStep);

	size_t i = 0;

	for (const auto & faceIndex : basic::make_reverse_range (getSelectedFaces ()))
	{
		const auto vertices   = getFaceSelection () -> getFaceVertices (faceIndex);
		const auto first      = faceIndex;
		const auto last       = faceIndex + vertices .size ();
		const auto faceNumber = getFaceSelection () -> getFaceNumber (faceIndex);

		if (colorPerVertex ())
		{
			if (last < colorIndex () .size ())
				colorIndex () .erase (colorIndex () .begin () + first, colorIndex () .begin () + last);
		}
		else
		{
			if (faceNumber < colorIndex () .size ())
				colorIndex () .erase (colorIndex () .begin () + faceNumber, colorIndex () .begin () + faceNumber + 1);
		}

		if (last < texCoordIndex () .size ())
			texCoordIndex () .erase (texCoordIndex () .begin () + first, texCoordIndex () .begin () + last);

		if (normalPerVertex ())
		{
			if (last < normalIndex () .size ())
				normalIndex () .erase (normalIndex () .begin () + first, normalIndex () .begin () + last);
		}
		else
		{
			if (faceNumber < normalIndex () .size ())
				normalIndex () .erase (normalIndex () .begin () + faceNumber, normalIndex () .begin () + faceNumber + 1);
		}

		coordIndex () .erase (coordIndex () .begin () + first, coordIndex () .begin () + last);

		++ i;
	}

	// Remove degenerated edges and faces.
	rebuildIndices  ();
	rebuildColor    ();
	rebuildTexCoord ();
	rebuildNormal   ();
	rebuildCoord    ();

	replaceSelection () = MFInt32 ();

	redoSetCoordPoint    (undoStep);
	redoSetNormalVector  (undoStep);
	redoSetTexCoordPoint (undoStep);
	redoSetColorColor    (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);
	redoRestoreSelection ({ }, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

std::vector <int32_t>
X3DIndexedFaceSetOperationsObject::splitPoints (const std::set <int32_t> & selectedPoints)
{
	std::vector <int32_t> points;

	for (const auto & index : selectedPoints)
	{
		const auto point   = getCoord () -> get1Point (index);
		const auto indices = getFaceSelection () -> getSharedVertices (index);
		
		if (indices .empty ())
			continue;
		
		points .emplace_back (coordIndex () [indices [0]]);

		for (const auto & index : std::make_pair (indices .begin () + 1, indices .end ()))
		{
			const auto size = getCoord () -> getSize ();
			
			points .emplace_back (size);
			
			coordIndex () [index] = size;

			getCoord () -> set1Point (size, point);
		}
	}

	return points;
}

std::vector <int32_t>
X3DIndexedFaceSetOperationsObject::formNewFace (const std::vector <std::vector <int32_t>> & holes)
{
	std::vector <int32_t> selection;
	std::vector <size_t>  vertices;

	for (const auto & hole : holes)
	{
		if (colorIndex () .size ())
		{
			if (not colorPerVertex () and getColor ())
				colorIndex () .emplace_back (getColor () -> getSize ());
		}

		if (normalIndex () .size () and getNormal ())
		{
			if (not normalPerVertex ())
				normalIndex () .emplace_back (getNormal () -> getSize ());
		}

	   for (const auto & point : hole)
	   {
			if (colorIndex () .size () and getColor ())
			{
				if (colorPerVertex ())
					colorIndex () .emplace_back (getColor () -> getSize ());
			}

		   if (texCoordIndex () .size () and getTexCoord ())
		   {
		      const auto vertices = getFaceSelection () -> getVertices (point);

				if (vertices .empty () or texCoordIndex () .size () < vertices .size ())
					texCoordIndex () .emplace_back (0);
				else
					texCoordIndex () .emplace_back (texCoordIndex () .get1Value (vertices [0]));
			}

			if (normalIndex () .size () and getNormal ())
			{
				if (normalPerVertex ())
					normalIndex () .emplace_back (getNormal () -> getSize ());
			}

			vertices      .emplace_back (coordIndex () .size ());
			coordIndex () .emplace_back (point);
			selection     .emplace_back (point);
		}

		if (colorIndex () .size () and getColor () and colorPerVertex ())
			colorIndex () .emplace_back (-1);
	   
	   if (texCoordIndex () .size () and getTexCoord ())
	      texCoordIndex () .emplace_back (-1);
		
		if (normalIndex () .size () and getNormal () and normalPerVertex ())
			normalIndex () .emplace_back (-1);

		coordIndex () .emplace_back (-1);

		if (normalIndex () .size () and getNormal ())
			getNormal () -> set1Vector (getNormal () -> getSize (), getPolygonNormal (vertices)); // getPolygonNormal fails with GeoCoordinate

		vertices .clear ();
	}

	if (colorIndex () .size () and getColor ())
		getColor () -> set1Color (getColor () -> getSize (), Color4f (1, 1, 1, 1));

	return selection;
}

std::vector <int32_t>
X3DIndexedFaceSetOperationsObject::extrudeSelectedEdges (const std::set <std::pair <size_t, size_t>> & edges, const std::set <size_t> & faces)
{
	std::map <int32_t, size_t>  pointIndex;
	std::map <int32_t, int32_t> points;

	for (const auto & edge : edges)
	{
		pointIndex .emplace (coordIndex () [edge .first],  edge .first);
		pointIndex .emplace (coordIndex () [edge .second], edge .second);
	}
		
	for (const auto & point : pointIndex)
	{
		const auto size = getCoord () -> getSize ();

		points .emplace (point .first, size);

		getCoord () -> set1Point (size, getCoord () -> get1Point (point .first));
	}

	size_t numFaces = getFaceSelection () -> getNumFaces ();

	for (const auto & edge : edges)
	{
		const auto size       = coordIndex () .size ();
		const auto faceNumber = getFaceSelection () -> getFaceNumber (edge .first);

		if (colorIndex () .size ())
		{
			if (colorPerVertex ())
			{
				colorIndex () .set1Value (size + 0, colorIndex () .get1Value (edge .first));
				colorIndex () .set1Value (size + 1, colorIndex () .get1Value (edge .second));
				colorIndex () .set1Value (size + 2, colorIndex () .get1Value (edge .second));
				colorIndex () .set1Value (size + 3, colorIndex () .get1Value (edge .first));
				colorIndex () .set1Value (size + 4, -1);
			}
			else
			{
				colorIndex () .set1Value (numFaces, colorIndex () .get1Value (faceNumber));
			}
		}

	   if (texCoordIndex () .size ())
	   {
			texCoordIndex () .set1Value (size + 0, texCoordIndex () .get1Value (edge .first));
			texCoordIndex () .set1Value (size + 1, texCoordIndex () .get1Value (edge .second));
			texCoordIndex () .set1Value (size + 2, texCoordIndex () .get1Value (edge .second));
			texCoordIndex () .set1Value (size + 3, texCoordIndex () .get1Value (edge .first));
			texCoordIndex () .set1Value (size + 4, -1);
		}

		if (normalIndex () .size ())
		{
			if (normalPerVertex ())
			{
				normalIndex () .set1Value (size + 0, normalIndex () .get1Value (edge .first));
				normalIndex () .set1Value (size + 1, normalIndex () .get1Value (edge .second));
				normalIndex () .set1Value (size + 2, normalIndex () .get1Value (edge .second));
				normalIndex () .set1Value (size + 3, normalIndex () .get1Value (edge .first));
				normalIndex () .set1Value (size + 4, -1);
			}
			else
			{
				normalIndex () .set1Value (numFaces, normalIndex () .get1Value (faceNumber));
			}
		}

		coordIndex () .emplace_back (coordIndex () [edge .first]);
		coordIndex () .emplace_back (coordIndex () [edge .second]);
		coordIndex () .emplace_back (points [coordIndex () [edge .second]]);
		coordIndex () .emplace_back (points [coordIndex () [edge .first]]);
		coordIndex () .emplace_back (-1);

		++ numFaces;
	}

	 if (not faces .empty ())
	 {
		std::map <size_t, int32_t> vertices;
		
		for (const auto & face : faces)
		{
			for (const auto & vertex : getFaceSelection () -> getFaceVertices (face))
			{
				const auto iter = points .find (coordIndex () [vertex]);

			   if (iter not_eq points .end ())
					vertices .emplace (vertex, iter -> second);
			}
		}

		for (const auto & vertex : vertices)
			coordIndex () [vertex .first] = vertex .second;
	}

	std::vector <int32_t> selection;

	for (const auto & point : points)
	   selection .emplace_back (point .second);

	return selection;
}

std::vector <int32_t>
X3DIndexedFaceSetOperationsObject::chipOf (const std::set <size_t> & selectedVertices)
{
	std::map <int32_t, std::vector <size_t>> pointIndex;

	for (const auto vertex : selectedVertices)
	   pointIndex [coordIndex () [vertex]] .emplace_back (vertex);

	std::vector <int32_t> points;

	for (const auto & pair : pointIndex)
	{
		const auto point = getCoord () -> get1Point (pair .first);
		const auto size  = getCoord () -> getSize ();
	
		points .emplace_back (size);
	
		for (const auto & vertex : pair .second)
			coordIndex () [vertex] = size;

		getCoord () -> set1Point (size, point);
	}

	rebuildCoord ();

	return points;
}

void
X3DIndexedFaceSetOperationsObject::flipVertexOrdering (const std::set <size_t> & faces)
{
	std::set <int32_t> indices;

	for (const auto & face : faces)
	{
		const auto faceNumber = getFaceSelection () -> getFaceNumber (face);
		const auto vertices   = getFaceSelection () -> getFaceVertices (face);
		const auto size       = vertices .size ();
		
		for (size_t i = 0, size1_2 = size / 2; i < size1_2; ++ i)
		{
			const auto i0 = vertices [i];
			const auto i1 = vertices [size - i - 1];
			
			if (colorPerVertex () and colorIndex () .size () >= i1)
				std::swap (colorIndex () [i0], colorIndex () [i1]);

			if (texCoordIndex () .size () >= i1)
				std::swap (texCoordIndex () [i0], texCoordIndex () [i1]);

			if (normalPerVertex () and normalIndex () .size () >= i1)
				std::swap (normalIndex () [i0], normalIndex () [i1]);

			std::swap (coordIndex () [i0], coordIndex () [i1]);
		}

		if (not normalIndex () .empty () and getNormal ())
		{
		   if (normalPerVertex ())
		   {
				for (size_t i = 0; i < size; ++ i)
					indices .emplace (normalIndex () .get1Value (vertices [i]));
			}
			else
			   indices .emplace (normalIndex () .get1Value (faceNumber));
		}
	}

	for (const auto index : indices)
		getNormal () -> set1Vector (index, negate (getNormal () -> get1Vector (index)));
}

///  A unique and sorted list of points given, the point will be removed from coords and the coord index will be updated.
void
X3DIndexedFaceSetOperationsObject::erasePoints (const std::vector <int32_t> & points)
{
	for (auto & point : basic::make_reverse_range (points))
	   getCoord () -> erasePoint (point);

	// Update coordIndex

	std::vector <size_t> counts;
	size_t               count = 0;

	for (size_t i = 0, size = points .size (); i < size; ++ i)
		counts .emplace_back (count ++);

	for (auto & index : coordIndex ())
	{
		const auto iter = std::upper_bound (points .begin (), points .end (), index);

		if (iter == points .end ())
			index -= count;
		else
			index -= counts [iter - points .begin ()];
	}
}

X3DIndexedFaceSetOperationsObject::~X3DIndexedFaceSetOperationsObject ()
{ }

} // X3D
} // titania

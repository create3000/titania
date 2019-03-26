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

#include "X3DIndexedFaceSetOperationsObject.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/EnvironmentalEffects/FogCoordinate.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Components/Rendering/X3DColorNode.h"
#include "../../../Components/Rendering/X3DNormalNode.h"
#include "../../../Components/Shape/Shape.h"
#include "../../../Components/Texturing/X3DTextureCoordinateNode.h"
#include "../../../Editing/Combine.h"
#include "../../../Editing/X3DEditor.h"
#include "../../../Editing/Selection/FaceSelection.h"
#include "../../../Editing/Undo/UndoStepContainer.h"

#include <Titania/Utility/Map.h>

namespace titania {
namespace X3D {

X3DIndexedFaceSetOperationsObject::Fields::Fields () :
	          cutGeometry (new SFTime ()),
	         copyGeometry (new SFTime ()),
	        pasteGeometry (new SFString ()),
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
	X3DIndexedFaceSetSelectionObject (),
	                          fields ()
{
	addType (X3DConstants::X3DIndexedFaceSetOperationsObject);
}

void
X3DIndexedFaceSetOperationsObject::initialize ()
{
	cutGeometry ()          .addInterest (&X3DIndexedFaceSetOperationsObject::set_cutGeometry,          this);
	copyGeometry ()         .addInterest (&X3DIndexedFaceSetOperationsObject::set_copyGeometry,         this);
	pasteGeometry ()        .addInterest (&X3DIndexedFaceSetOperationsObject::set_pasteGeometry,        this);
	mergePoints ()          .addInterest (&X3DIndexedFaceSetOperationsObject::set_mergePoints,          this);
	splitPoints ()          .addInterest (&X3DIndexedFaceSetOperationsObject::set_splitPoints,          this);
	formNewFace ()          .addInterest (&X3DIndexedFaceSetOperationsObject::set_formNewFace,          this);
	extrudeSelectedEdges () .addInterest (&X3DIndexedFaceSetOperationsObject::set_extrudeSelectedEdges, this);
	extrudeSelectedFaces () .addInterest (&X3DIndexedFaceSetOperationsObject::set_extrudeSelectedFaces, this);
	chipOfSelectedFaces ()  .addInterest (&X3DIndexedFaceSetOperationsObject::set_chipOfSelectedFaces,  this);
	flipVertexOrdering ()   .addInterest (&X3DIndexedFaceSetOperationsObject::set_flipVertexOrdering,   this);
	deleteSelectedFaces ()  .addInterest (&X3DIndexedFaceSetOperationsObject::set_deleteSelectedFaces,  this);
}

void
X3DIndexedFaceSetOperationsObject::set_cutGeometry ()
{
	if (getSelectedFaces () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Cut Selected Faces"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetFogCoord      (undoStep);
	undoSetColor         (undoStep);
	undoSetTexCoord      (undoStep);
	undoSetNormal        (undoStep);
	undoSetCoord         (undoStep);

	set_copyGeometry ();

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	deleteFaces (getSelectedFaces ());

	// Remove degenerated edges and faces.
	rebuildIndices  ();
	rebuildColor    ();
	rebuildTexCoord ();
	rebuildNormal   ();
	rebuildCoord    ();

	redoSetCoord         (undoStep);
	redoSetNormal        (undoStep);
	redoSetTexCoord      (undoStep);
	redoSetColor         (undoStep);
	redoSetFogCoord      (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);

	redoRestoreSelectedFaces ({ }, undoStep);
	replaceSelection () = MFInt32 ();

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_copyGeometry ()
{
	const auto modelMatrix = X3DEditor::getModelMatrix (X3DExecutionContextPtr (getMasterScene ()), SFNode (this));
	const auto geometry    = X3DPtr <IndexedFaceSet> (new IndexedFaceSet (getExecutionContext ()));

	// Save indices and add one if not exits.

	const auto colorIndex    = this -> colorIndex ();
	const auto texCoordIndex = this -> texCoordIndex ();
	const auto normalIndex   = this -> normalIndex ();

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	// Build geometry.

	geometry -> solid ()           = solid ();
	geometry -> ccw ()             = determinant (modelMatrix) >= 0 ? ccw () : not ccw ();
	geometry -> convex ()          = convex ();
	geometry -> creaseAngle ()     = creaseAngle ();
	geometry -> colorPerVertex ()  = colorPerVertex ();
	geometry -> normalPerVertex () = normalPerVertex ();

	// Generate nodes, we do not setup the nodes as it is not needed for stringification.

	X3DPtr <FogCoordinate> fogCoord;
	X3DPtr <X3DColorNode> color;
	X3DPtr <X3DTextureCoordinateNode> texCoord;
	X3DPtr <X3DNormalNode> normal;
	X3DPtr <X3DCoordinateNode> coord;
		
	if (getFogCoord ())
	{
		geometry -> fogCoord () = getFogCoord () -> create (getExecutionContext ());
		fogCoord = geometry -> fogCoord ();
	}

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
			}

			if (texCoord)
				texCoordArray .emplace (getVertexTexCoordIndex (vertex), texCoordArray .size ());

			if (normal)
			{
				if (normalPerVertex ())
					normalArray .emplace (getVertexNormalIndex (vertex), normalArray .size ());
			}

			coordArray .emplace (coordIndex () .get1Value (vertex), coordArray .size ());
		}

		if (color)
		{
			if (not colorPerVertex ())
				colorArray .emplace (getFaceColorIndex (faceNumber), colorArray .size ());
		}

		if (normal)
		{
			if (not normalPerVertex ())
				normalArray .emplace (getFaceNormalIndex (faceNumber), normalArray .size ());
		}
	}

	// Generate indices

	for (const auto & face : getSelectedFaces ())
	{
		const auto vertices   = getFaceSelection () -> getFaceVertices (face);
		const auto faceNumber = getFaceSelection () -> getFaceNumber (face);

		for (const auto & vertex : vertices)
		{
		   const auto point = coordArray [coordIndex () .get1Value (vertex)];

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
			{
			   geometry -> colorIndex () .emplace_back (-1);
			}
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
			{
			   geometry -> normalIndex () .emplace_back (-1);
			}
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

	if (fogCoord)
	{
		fogCoord -> resize (coord -> getSize ());

		for (const auto & index : basic::reverse (coordArray))
			fogCoord -> set1Depth (fogCoord -> getSize (), getFogCoord () -> get1Depth (index .second));
	}

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
			coord -> set1Point (coord -> getSize (), getCoord () -> get1Point (index .second) * modelMatrix);
	}

	// Reset indices.

	this -> colorIndex ()    = colorIndex;
	this -> texCoordIndex () = texCoordIndex;
	this -> normalIndex ()   = normalIndex;

	// Print out geometry

	const auto transform = getExecutionContext () -> createNode <Transform> ();
	const auto shape     = getExecutionContext () -> createNode <Shape> ();

	transform -> children () .emplace_back (shape);
	shape -> geometry () = geometry;

	clipboard_changed () = transform -> toString ();
}

void
X3DIndexedFaceSetOperationsObject::set_pasteGeometry ()
{
	try
	{
		const auto undoStep     = std::make_shared <UndoStep> (_ ("Paste Faces"));
		const auto scene        = getBrowser () -> createX3DFromString (pasteGeometry ());
		auto       geometries   = X3DEditor::getNodes <IndexedFaceSet> (scene -> getRootNodes (), { X3DConstants::IndexedFaceSet });
		const auto targetMatrix = inverse (X3DEditor::getModelMatrix (X3DExecutionContextPtr (getMasterScene ()), SFNode (this)));

		undoStep -> addUndoFunction (&SFBool::setValue, std::ref (colorPerVertex ()),  colorPerVertex ());
		undoStep -> addUndoFunction (&SFBool::setValue, std::ref (normalPerVertex ()), normalPerVertex ());

		undoRestoreSelection (undoStep);
		undoSetColorIndex    (undoStep);
		undoSetTexCoordIndex (undoStep);
		undoSetNormalIndex   (undoStep);
		undoSetCoordIndex    (undoStep);
		undoSetFogCoord      (undoStep);
		undoSetColor         (undoStep);
		undoSetTexCoord      (undoStep);
		undoSetNormal        (undoStep);
		undoSetCoord         (undoStep);

		addColorIndex ();
		addTexCoordIndex ();
		addNormalIndex ();

		const auto selection = Combine () .combine (X3DExecutionContextPtr (getExecutionContext ()), geometries, X3DPtr <IndexedFaceSet> (this), getCoord (), targetMatrix);

		redoSetCoord         (undoStep);
		redoSetNormal        (undoStep);
		redoSetTexCoord      (undoStep);
		redoSetColor         (undoStep);
		redoSetFogCoord      (undoStep);
		redoSetCoordIndex    (undoStep);
		redoSetNormalIndex   (undoStep);
		redoSetTexCoordIndex (undoStep);
		redoSetColorIndex    (undoStep);

		undoStep -> addRedoFunction (&SFBool::setValue, std::ref (colorPerVertex ()),  colorPerVertex ());
		undoStep -> addRedoFunction (&SFBool::setValue, std::ref (normalPerVertex ()), normalPerVertex ());

		redoRestoreSelectedFaces (selection, undoStep);
		replaceSelectedFaces () .assign (selection .cbegin (), selection .cend ());

		undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DIndexedFaceSetOperationsObject::set_mergePoints ()
{
	if (getSelectedPoints () .empty ())
		return;

	int32_t masterPoint = getMasterPoint ();

	if (masterPoint < 0)
	   return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Merge Points"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetFogCoord      (undoStep);
	undoSetColor         (undoStep);
	undoSetTexCoord      (undoStep);
	undoSetNormal        (undoStep);
	undoSetCoord         (undoStep);

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	std::vector <int32_t> pointsToRemove;

	for (const auto & selectedPoint : getSelectedPoints ())
	{
		const auto & index   = selectedPoint .first;
		const auto   indices = getFaceSelection () -> getSharedVertices (index);
		
		if (indices .empty ())
			continue;
		
		for (const auto & index : indices)
		{
			pointsToRemove .emplace_back (coordIndex () .get1Value (index));

			coordIndex () .set1Value (index, masterPoint);
		}
	}

	if (getCoord () -> getCloneCount () == 1)
	{
		// Erase points.

		std::sort (pointsToRemove .begin (), pointsToRemove .end ());

		const auto unique  = std::unique (pointsToRemove .begin (), pointsToRemove .end ());
		const auto removed = std::remove (pointsToRemove .begin (), unique, masterPoint);
	 
		pointsToRemove .erase (removed, pointsToRemove .end ());

		masterPoint -= std::lower_bound (pointsToRemove .cbegin (), pointsToRemove .cend (), masterPoint) - pointsToRemove .cbegin ();

		erasePoints (pointsToRemove);
	}

	std::vector <int32_t> selection = { masterPoint };

	// Remove degenerated edges and faces.
	rebuildIndices  ();
	rebuildColor    ();
	rebuildTexCoord ();
	rebuildNormal   ();
	rewriteArray (rebuildCoord (), selection);

	redoSetCoord         (undoStep);
	redoSetNormal        (undoStep);
	redoSetTexCoord      (undoStep);
	redoSetColor         (undoStep);
	redoSetFogCoord      (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);

	redoRestoreSelectedPoints ({ masterPoint }, undoStep);
	replaceSelectedPoints () .assign (selection .cbegin (), selection .cend ());

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_splitPoints ()
{
	if (getSelectedPoints () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Split Points"));

	undoRestoreSelection (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetFogCoord      (undoStep);
	undoSetCoord         (undoStep);

	std::vector <int32_t> points;

	for (const auto & selectedPoint : getSelectedPoints ())
		points .emplace_back (selectedPoint .first);

	std::sort (points .begin (), points .end ());

	points .erase (std::unique (points .begin (), points .end ()), points .end ());

	const auto selection = splitPoints (points);

	redoSetCoord         (undoStep);
	redoSetFogCoord      (undoStep);
	redoSetCoordIndex    (undoStep);

	redoRestoreSelectedPoints (selection, undoStep);
	replaceSelectedPoints () .assign (selection .cbegin (), selection .cend ());

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_formNewFace ()
{
	if (getSelectedHoles () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Form New Face From Selected Holes"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	const auto selection = formNewFace (getSelectedHoles ());

	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);
	redoRestoreSelectedFaces (selection, undoStep);

	replaceSelectedFaces () .assign (selection .cbegin (), selection .cend ());

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_extrudeSelectedEdges ()
{
	if (getSelectedEdges () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Extrude Selected Edges"));

	undoRestoreSelection (undoStep);

	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetFogCoord      (undoStep);
	undoSetCoord         (undoStep);

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	std::vector <std::pair <size_t, size_t>> edges;

	for (const auto & edge : getSelectedEdges ())
	{
		if ((getSelectionType () == SelectionType::FACES and edge .second .size () not_eq 1 and getSelectedFaces () .size ()) or edge .second .empty ())
			continue;

	   for (const auto & pair : edge .second)
	   {
			edges .emplace_back (pair);
			break;
		}
	}

	std::sort (edges .begin (), edges .end ());

	edges .erase (std::unique (edges .begin (), edges .end ()), edges .end ());

	const auto selection = extrudeSelectedEdges (edges, true, { }, false);

	redoSetCoord         (undoStep);
	redoSetFogCoord      (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);

	redoRestoreSelectedEdges (selection, undoStep);
	replaceSelectedEdges () .assign (selection .cbegin (), selection .cend ());

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_extrudeSelectedFaces ()
{
	if (getSelectedFaces () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Extrude Selected Faces"));

	undoRestoreSelection (undoStep);

	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetFogCoord      (undoStep);
	undoSetCoord         (undoStep);

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	bool flatFaces = false;

	std::vector <std::pair <size_t, size_t>> edges;
	std::vector <int32_t>                    points;

	for (const auto & edge : getSelectedEdges ())
	{
		if ((getSelectionType () == SelectionType::FACES and edge .second .size () not_eq 1) or edge .second .empty ())
			continue;

		points .emplace_back (edge .first .first);
		points .emplace_back (edge .first .second);

		flatFaces |= edge .second .size () == 1;

	   for (const auto & pair : edge .second)
		{
			edges .emplace_back (pair);
			break;
		}
	}

	std::sort (edges  .begin (), edges  .end ());
	std::sort (points .begin (), points .end ());

	edges  .erase (std::unique (edges  .begin (), edges  .end ()), edges  .end ());
	points .erase (std::unique (points .begin (), points .end ()), points .end ());

	//

	std::vector <int32_t> facesPoints;

	auto selection = extrudeSelectedEdges (edges, false, getSelectedFaces (), flatFaces);

	for (const auto & face : getSelectedFaces ())
	{
		for (const auto & vertex : getFaceSelection () -> getFaceVertices (face))
		   facesPoints .emplace_back (coordIndex () .get1Value (vertex));
	}

	std::sort (facesPoints .begin (), facesPoints .end ());

	facesPoints .erase (std::unique (facesPoints .begin (), facesPoints .end ()), facesPoints .end ());

	std::set_difference (facesPoints .cbegin (), facesPoints .cend (), points .cbegin (), points .cend (), std::inserter (selection, selection .begin ()));

	//

	rewriteArray (rebuildCoord (), selection);

	redoSetCoord         (undoStep);
	redoSetFogCoord      (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);

	redoRestoreSelectedFaces (std::vector <int32_t> (selection .cbegin (), selection .cend ()), undoStep);
	replaceSelectedFaces () .assign (selection .cbegin (), selection .cend ());

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_chipOfSelectedFaces ()
{
	if (getSelectedEdges () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Chip Of Selected Faces"));

	undoRestoreSelection (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetFogCoord      (undoStep);
	undoSetCoord         (undoStep);

	std::vector <size_t> vertices;

	for (const auto & edge : getSelectedEdges ())
	{
	   for (const auto & pair : edge .second)
		{
			vertices .emplace_back (pair .first);
			vertices .emplace_back (pair .second);
		}
	}

	std::sort (vertices .begin (), vertices .end ());

	vertices .erase (std::unique (vertices .begin (), vertices .end ()), vertices .end ());

	auto selection = chipOf (vertices);

	rewriteArray (rebuildCoord (), selection);

	redoSetCoord      (undoStep);
	redoSetFogCoord   (undoStep);
	redoSetCoordIndex (undoStep);

	redoRestoreSelectedFaces (selection, undoStep);
	replaceSelectedFaces () .assign (selection .cbegin (), selection .cend ());

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_flipVertexOrdering ()
{
	if (getSelectedFaces () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Flip Vertex Ordering"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetNormal        (undoStep);

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	flipVertexOrdering (getSelectedFaces ());

	// Set selection

	std::vector <int32_t> selection;

	for (const auto & point : getSelectedPoints ())
	   selection .emplace_back (point .first);

	// Redo

	redoSetNormal        (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);
	redoRestoreSelectedFaces (selection, undoStep);

	replaceSelectedFaces () .assign (selection .cbegin (), selection .cend ());

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::set_deleteSelectedFaces ()
{
	if (getSelectedFaces () .empty ())
		return;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Delete Selected Faces"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetFogCoord      (undoStep);
	undoSetColor         (undoStep);
	undoSetTexCoord      (undoStep);
	undoSetNormal        (undoStep);
	undoSetCoord         (undoStep);

	addColorIndex ();
	addTexCoordIndex ();
	addNormalIndex ();

	deleteFaces (getSelectedFaces ());

	// Set selection

	std::vector <int32_t> selection;

	for (const auto & point : getSelectedPoints ())
	   selection .emplace_back (point .first);

	// Remove degenerated edges and faces.
	rebuildIndices  ();
	rebuildColor    ();
	rebuildTexCoord ();
	rebuildNormal   ();
	rebuildCoord    ();

	redoSetCoord         (undoStep);
	redoSetNormal        (undoStep);
	redoSetTexCoord      (undoStep);
	redoSetColor         (undoStep);
	redoSetFogCoord      (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);
	redoRestoreSelectedFaces (selection, undoStep);

	replaceSelection () = MFInt32 ();

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetOperationsObject::deleteFaces (const std::set <size_t> & selectedFaces)
{
	for (const auto & faceIndex : basic::make_reverse_range (selectedFaces))
	{
		const auto faceNumber = getFaceSelection () -> getFaceNumber (faceIndex);
		const auto vertices   = getFaceSelection () -> getFaceVertices (faceIndex);
		const auto first      = faceIndex;
		const auto last       = faceIndex + vertices .size ();

		if (colorPerVertex ())
		{
			if (first < colorIndex () .size () and last < colorIndex () .size ())
				colorIndex () .erase (colorIndex () .begin () + first, colorIndex () .begin () + last);
		}
		else
		{
			if (faceNumber < colorIndex () .size ())
				colorIndex () .erase (colorIndex () .begin () + faceNumber);
		}

		if (first < texCoordIndex () .size () and last < texCoordIndex () .size ())
			texCoordIndex () .erase (texCoordIndex () .begin () + first, texCoordIndex () .begin () + last);

		if (normalPerVertex ())
		{
			if (first < normalIndex () .size () and last < normalIndex () .size ())
				normalIndex () .erase (normalIndex () .begin () + first, normalIndex () .begin () + last);
		}
		else
		{
			if (faceNumber < normalIndex () .size ())
				normalIndex () .erase (normalIndex () .begin () + faceNumber);
		}

		coordIndex () .erase (coordIndex () .begin () + first, coordIndex () .begin () + last);
	}
}

/// The array @a selectedPoints must be an array of unique points.
std::vector <int32_t>
X3DIndexedFaceSetOperationsObject::splitPoints (const std::vector <int32_t> & selectedPoints)
{
	std::vector <int32_t> points;

	const auto fogCoord = getFogCoord () and getFogCoord () -> getSize ();

	for (const auto & index : selectedPoints)
	{
		const auto fogDepth = fogCoord ? getFogCoord () -> get1Depth (index) : 0.0f;
		const auto point    = getCoord () -> get1Point (index);
		const auto indices  = getFaceSelection () -> getSharedVertices (index);
		
		if (indices .empty ())
			continue;
		
		points .emplace_back (coordIndex () .get1Value (indices [0]));

		for (const auto & index : std::pair (indices .cbegin () + 1, indices .cend ()))
		{
			const auto size = getCoord () -> getSize ();
			
			points .emplace_back (size);
			
			coordIndex () .set1Value (index, size);

			if (fogCoord)
				getFogCoord () -> set1Depth (size, fogDepth);

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
		if (colorIndex () .size () and getColor ())
		{
			if (not colorPerVertex ())
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

/// The array @a edges must be an  array of unique edges.
std::vector <int32_t>
X3DIndexedFaceSetOperationsObject::extrudeSelectedEdges (const std::vector <std::pair <size_t, size_t>> & edges,
                                                         const bool flipNewFaces,
                                                         const std::set <size_t> & faces,
                                                         const bool duplicateFaces)
{
	std::map <int32_t, size_t>  pointIndex;
	std::map <int32_t, int32_t> points;

	for (const auto & edge : edges)
	{
		pointIndex .emplace (coordIndex () .get1Value (edge .first),  edge .first);
		pointIndex .emplace (coordIndex () .get1Value (edge .second), edge .second);
	}

	const auto fogCoord = getFogCoord () and getFogCoord () -> getSize ();
		
	for (const auto & point : pointIndex)
	{
		const auto index = point .first;
		const auto size  = getCoord () -> getSize ();

		points .emplace (index, size);

		if (fogCoord)
			getFogCoord () -> set1Depth (size, getFogCoord () -> get1Depth (index));

		getCoord () -> set1Point (size, getCoord () -> get1Point (index));
	}

	size_t numFaces = getFaceSelection () -> getNumFaces ();

	for (const auto & edge : edges)
	{
		const auto size       = coordIndex () .size ();
		const auto faceNumber = getFaceSelection () -> getFaceNumber (edge .first);
		const auto first      = flipNewFaces ? edge .second : edge .first;
		const auto second     = flipNewFaces ? edge .first  : edge .second;

		if (colorIndex () .size ())
		{
			if (colorPerVertex ())
			{
				colorIndex () .set1Value (size + 0, colorIndex () .get1Value (first));
				colorIndex () .set1Value (size + 1, colorIndex () .get1Value (second));
				colorIndex () .set1Value (size + 2, colorIndex () .get1Value (second));
				colorIndex () .set1Value (size + 3, colorIndex () .get1Value (first));
				colorIndex () .set1Value (size + 4, -1);
			}
			else
			{
				colorIndex () .set1Value (numFaces, colorIndex () .get1Value (faceNumber));
			}
		}

	   if (texCoordIndex () .size ())
	   {
			texCoordIndex () .set1Value (size + 0, texCoordIndex () .get1Value (first));
			texCoordIndex () .set1Value (size + 1, texCoordIndex () .get1Value (second));
			texCoordIndex () .set1Value (size + 2, texCoordIndex () .get1Value (second));
			texCoordIndex () .set1Value (size + 3, texCoordIndex () .get1Value (first));
			texCoordIndex () .set1Value (size + 4, -1);
		}

		if (normalIndex () .size ())
		{
			if (normalPerVertex ())
			{
				normalIndex () .set1Value (size + 0, normalIndex () .get1Value (first));
				normalIndex () .set1Value (size + 1, normalIndex () .get1Value (second));
				normalIndex () .set1Value (size + 2, normalIndex () .get1Value (second));
				normalIndex () .set1Value (size + 3, normalIndex () .get1Value (first));
				normalIndex () .set1Value (size + 4, -1);
			}
			else
			{
				normalIndex () .set1Value (numFaces, normalIndex () .get1Value (faceNumber));
			}
		}

		coordIndex () .emplace_back (coordIndex () .get1Value (first));
		coordIndex () .emplace_back (coordIndex () .get1Value (second));
		coordIndex () .emplace_back (points [coordIndex () .get1Value (second)]);
		coordIndex () .emplace_back (points [coordIndex () .get1Value (first)]);
		coordIndex () .emplace_back (-1);

		++ numFaces;
	}

	 if (not faces .empty ())
	 {
		if (duplicateFaces)
		{
			for (const auto & face : faces)
			{
				const auto vertices = getFaceSelection () -> getFaceVertices (face);

				for (const auto & vertex : vertices)
				{
					const auto faceNumber = getFaceSelection () -> getFaceNumber (vertex);
					const auto iter       = points .find (coordIndex () .get1Value (vertex));

					if (colorIndex () .size ())
					{
						if (colorPerVertex ())
							colorIndex () .emplace_back (colorIndex () .get1Value (vertex));
						else
							colorIndex () .emplace_back (colorIndex () .get1Value (faceNumber));
					}

					if (texCoordIndex () .size ())
						texCoordIndex () .emplace_back (texCoordIndex () .get1Value (vertex));

					if (normalIndex () .size ())
					{
						if (normalPerVertex ())
							normalIndex () .emplace_back (normalIndex () .get1Value (vertex));
						else
							normalIndex () .emplace_back (normalIndex () .get1Value (faceNumber));
					}

				   if (iter not_eq points .end ())
						coordIndex () .emplace_back (iter -> second);
				}

				if (colorIndex () .size () and colorPerVertex ())
					colorIndex () .emplace_back (-1);

				if (texCoordIndex () .size ())
					texCoordIndex () .emplace_back (-1);

				if (normalIndex () .size () and normalPerVertex ())
					normalIndex () .emplace_back (-1);

				coordIndex () .emplace_back (-1);
			}
		}
		else
		{
			// Remap faces to extrude to new points.

			std::map <size_t, int32_t> vertices;
			
			for (const auto & face : faces)
			{
				for (const auto & vertex : getFaceSelection () -> getFaceVertices (face))
				{
					const auto iter = points .find (coordIndex () .get1Value (vertex));
	
				   if (iter not_eq points .end ())
						vertices .emplace (vertex, iter -> second);
				}
			}
	
			for (const auto & vertex : vertices)
				coordIndex () .set1Value (vertex .first, vertex .second);
		}
	}

	std::vector <int32_t> selection;

	for (const auto & point : points)
	   selection .emplace_back (point .second);

	return selection;
}

std::vector <int32_t>
X3DIndexedFaceSetOperationsObject::chipOf (const std::vector <size_t> & selectedVertices)
{
	std::map <int32_t, std::vector <size_t>> pointIndex;

	for (const auto vertex : selectedVertices)
	   pointIndex [coordIndex () .get1Value (vertex)] .emplace_back (vertex);

	const auto fogCoord = getFogCoord () and getFogCoord () -> getSize ();

	std::vector <int32_t> points;

	for (const auto & pair : pointIndex)
	{
		const auto index = pair .first;
		const auto point = getCoord () -> get1Point (index);
		const auto size  = getCoord () -> getSize ();
	
		points .emplace_back (size);
	
		for (const auto & vertex : pair .second)
			coordIndex () .set1Value (vertex, size);

		if (fogCoord)
			getFogCoord () -> set1Depth (size, getFogCoord () -> get1Depth (index));

		getCoord () -> set1Point (size, point);
	}

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
	const auto fogCoord = getFogCoord () and getFogCoord () -> getSize ();

	for (auto & point : basic::make_reverse_range (points))
	{
		if (fogCoord)
			getFogCoord () -> eraseDepth (point);

	   getCoord () -> erasePoint (point);
	}

	// Update coordIndex

	std::vector <size_t> counts;
	size_t               count = 0;

	for (size_t i = 0, size = points .size (); i < size; ++ i)
		counts .emplace_back (count ++);

	for (MFInt32::reference index : coordIndex ())
	{
		const auto iter = std::upper_bound (points .cbegin (), points .cend (), index);

		if (iter == points .end ())
			index = index .get () - count;
		else
			index = index .get () - counts [iter - points .cbegin ()];
	}
}

X3DIndexedFaceSetOperationsObject::~X3DIndexedFaceSetOperationsObject ()
{ }

} // X3D
} // titania

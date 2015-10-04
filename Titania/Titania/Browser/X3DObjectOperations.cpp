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

#include "X3DObjectOperations.h"

#include "../Base/X3DEditorObject.h"
#include "../Browser/X3DBrowserWindow.h"

#include <Titania/X3D/Components/Grouping/X3DGroupingNode.h>
#include <Titania/X3D/Components/Rendering/Coordinate.h>
#include <Titania/Utility/Map.h>

namespace titania {
namespace puck {

X3DObjectOperations::X3DObjectOperations () :
	X3DBrowserWindowInterface ()
{ }

void
X3DObjectOperations::on_combine_activated ()
{
	try
	{
		if (not getExecutionContext () -> hasComponent (X3D::ComponentType::GEOMETRY_3D))
			getExecutionContext () -> updateComponent (getBrowser () -> getComponent ("Geometry3D", 2));

		const auto undoStep  = std::make_shared <UndoStep> (_ ("Combine Objects"));
		const auto selection = getBrowserWindow () -> getSelection () -> getChildren ();
		const auto shapes    = X3DEditorObject::getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

		if (shapes .empty ())
			return;

		// Choose target

		const auto masterShape = shapes .back ();
	
		X3D::X3DPtr <X3D::IndexedFaceSet>    masterGeometry (masterShape -> geometry ());
		X3D::X3DPtr <X3D::X3DCoordinateNode> masterCoord (masterGeometry ? masterGeometry -> coord () : nullptr);

		X3D::X3DPtr <X3D::IndexedFaceSet>    targetGeometry (getExecutionContext () -> createNode <X3D::IndexedFaceSet> ());
		X3D::X3DPtr <X3D::X3DCoordinateNode> targetCoord;

		if (masterCoord)
		{
			targetCoord = masterCoord -> create (getExecutionContext ());
			getExecutionContext () -> addUninitializedNode (targetCoord);
		}
		else
		{
			if (not getExecutionContext () -> hasComponent (X3D::ComponentType::RENDERING))
				getExecutionContext () -> updateComponent (getBrowser () -> getComponent ("Rendering", 1));

			targetCoord = getExecutionContext () -> createNode <X3D::Coordinate> ();
		}
		
		targetGeometry -> coord () = targetCoord;

		// Combine Coordinates

		const auto targetMatrix = ~getBrowserWindow () -> findModelViewMatrix (masterShape);

		combineCoordinates (shapes, targetGeometry, targetCoord, targetMatrix);

		getBrowserWindow () -> replaceNode (X3D::SFNode (masterShape), masterShape -> geometry (), X3D::SFNode (targetGeometry), undoStep);
	
		//combineNormals (shapes, targetGeometry);

		// Remove shapes

		removeShapes (shapes, selection, masterShape, undoStep);

		// Select target

		const auto hierarchy = X3D::find (selection, masterShape, X3D::TRAVERSE_ROOT_NODES | X3D::TRAVERSE_PROTOTYPE_INSTANCES);

		getBrowserWindow () -> getSelection () -> setChildren ({ X3D::SFNode (hierarchy .front ()) }, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);

		getExecutionContext () -> realize ();
	}
	catch (const std::exception &)
	{ }
}

void
X3DObjectOperations::combineCoordinates (const X3D::X3DPtrArray <X3D::X3DShapeNode> & shapes,
                                         const X3D::X3DPtr <X3D::IndexedFaceSet> & targetGeometry,
                                         const X3D::X3DPtr <X3D::X3DCoordinateNode> & targetCoord,
                                         const X3D::Matrix4d & targetMatrix)
{
	for (const auto & shape : shapes)
	{
		const X3D::X3DPtr <X3D::IndexedFaceSet> geometry (shape -> geometry ());

		if (not geometry)
			continue;

		const X3D::X3DPtr <X3D::X3DCoordinateNode> coord (geometry -> coord ());

		if (not coord)
			continue;

		const auto matrix = getBrowserWindow () -> findModelViewMatrix (shape) * targetMatrix;

		std::map <int32_t, int32_t> coordIndex;

		for (const auto & index : geometry -> coordIndex ())
		{
			if (index >= 0)
				coordIndex .emplace (index, coordIndex .size ());
		}

		for (const auto & index : geometry -> coordIndex ())
		{
			if (index < 0)
				targetGeometry -> coordIndex () .emplace_back (-1);
			else
				targetGeometry -> coordIndex () .emplace_back (coordIndex [index] + targetCoord -> getSize ());
		}

		for (const auto & index : basic::reverse (coordIndex))
			targetCoord -> set1Point (targetCoord -> getSize (), coord -> get1Point (index .second) * matrix);
	}
}

void
X3DObjectOperations::removeShapes (const X3D::X3DPtrArray <X3D::X3DShapeNode> & shapes, const X3D::MFNode & selection, const X3D::X3DPtr <X3D::X3DShapeNode> & masterShape, const UndoStepPtr & undoStep)
{
	// Remove Shape nodes that are direct selected.

	X3D::MFNode nodes;

	for (const auto node : selection)
	{
		X3D::X3DPtr <X3D::X3DShapeNode> shape (node);

		if (not shape or shape == masterShape)
			continue;

		nodes .emplace_back (node);
	}

	getBrowserWindow () -> removeNodesFromScene (getExecutionContext (), nodes, undoStep);

	// Remove Shape nodes from selection.

	nodes .assign (shapes .begin (), shapes .end ());
	nodes .remove (X3D::SFNode (masterShape));

	getBrowserWindow () -> removeNodesFromSceneGraph (selection, std::set <X3D::SFNode> (nodes .begin (), nodes .end ()), undoStep);

	for (const auto & node : nodes)
		getBrowserWindow () -> removeNodesFromSceneIfNotExists (getExecutionContext (), { node }, undoStep);

	// Find empty groups in selection and remove from scene.

	const auto groups = X3DEditorObject::getNodes <X3D::X3DGroupingNode> (selection, { X3D::X3DConstants::X3DGroupingNode });

	nodes .clear ();

	for (const auto & group : groups)
	{
		if (group -> children () .empty ())
			nodes .emplace_back (group);
	}

	getBrowserWindow () -> removeNodesFromScene (getExecutionContext (), nodes, undoStep);
}

X3DObjectOperations::~X3DObjectOperations ()
{ }

} // puck
} // titania

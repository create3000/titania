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

#include "X3DFogCoordinateEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"

#include <Titania/X3D/Components/EnvironmentalEffects/FogCoordinate.h>
#include <Titania/X3D/Tools/Geometry3D/IndexedFaceSet/IndexedFaceSetTool.h>

namespace titania {
namespace puck {

X3DFogCoordinateEditor::X3DFogCoordinateEditor () :
	X3DNotebookPageInterface (),
	     indexedFaceSetNodes (),
	        fogCoordUndoStep (),
	        fogDepthUndoStep (),
	                changing (false)
{
	addChildObjects (indexedFaceSetNodes);
}

void
X3DFogCoordinateEditor::initialize ()
{
	getBrowserWindow () -> getSelection () -> getSelectGeometry () .addInterest (&X3DFogCoordinateEditor::set_select_geometries, this);

	set_select_geometries ();
}

void
X3DFogCoordinateEditor::set_select_geometries ()
{
	if (getBrowserWindow () -> getSelection () -> getSelectGeometry ())
	{
		getBrowserWindow () -> getSelection () -> getGeometries () .addInterest (&X3DFogCoordinateEditor::set_geometries, this);
	}
	else
	{
		getBrowserWindow () -> getSelection () -> getGeometries () .removeInterest (&X3DFogCoordinateEditor::set_geometries, this);
	}

	set_geometries ();
}

void
X3DFogCoordinateEditor::set_geometries ()
{
	// Disconnect.

	for (const auto & indexedFaceSetNode : indexedFaceSetNodes)
		indexedFaceSetNode -> getFogCoord () .removeInterest (&X3DFogCoordinateEditor::set_fogCoord, this);

	indexedFaceSetNodes .clear ();

	// Filter selection and connect.

	for (const auto & node : getBrowserWindow () -> getSelection () -> getGeometries ())
	{
		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::IndexedFaceSet:
				{
					const auto indexedFaceSetNode = dynamic_cast <X3D::IndexedFaceSet*> (node .getValue ());

					indexedFaceSetNodes .emplace_back (indexedFaceSetNode);

					// Fog coord.

					indexedFaceSetNode -> getField <X3D::SFInt32> ("selectedPoints_changed") .addInterest (&X3DFogCoordinateEditor::set_fogCoord, this);
					indexedFaceSetNode -> getField <X3D::SFTime>  ("touchTime")              .addInterest (&X3DFogCoordinateEditor::set_fogCoord, this);

					indexedFaceSetNode -> getFogCoord () .addInterest (&X3DFogCoordinateEditor::set_fogCoord, this);
					break;
				}
				default:
					continue;

				break;
			}
		}
	}

	getGeometryToolbarRevealer () .set_reveal_child (getBrowserWindow () -> getSelection () -> getSelectGeometry () and indexedFaceSetNodes .size ());

	set_fogCoord ();
}

void
X3DFogCoordinateEditor::set_fogCoord ()
{
	changing = true;

	const auto & currentPage = getBrowserWindow () -> getCurrentPage ();

	fogDepthUndoStep .reset ();

	switch (indexedFaceSetNodes .size ())
	{
		case 0:
		{
			currentPage -> getFogCoordCheckButton () .set_inconsistent (false);
			currentPage -> getFogCoordCheckButton () .set_active (false);
			currentPage -> getFogCoordCheckButton () .set_sensitive (false);

			currentPage -> getFogDepthBox () .set_sensitive (false);
			break;
		}
		default:
		{
			const auto currentTool = getCurrentTool ();

			size_t numFogCoord       = 0;
			bool   hasFogCoord       = currentTool -> getFogCoord ();
			size_t numSelectedPoints = currentTool -> getSelectedPoints () .size ();

			for (const auto & indexedFaceSetNode : indexedFaceSetNodes)
				numFogCoord += bool (indexedFaceSetNode -> getFogCoord ());

			currentPage -> getFogCoordCheckButton () .set_inconsistent (numFogCoord and numFogCoord not_eq indexedFaceSetNodes .size ());
			currentPage -> getFogCoordCheckButton () .set_active (numFogCoord and numFogCoord == indexedFaceSetNodes .size ());
			currentPage -> getFogCoordCheckButton () .set_sensitive (true);

			currentPage -> getFogDepthBox () .set_sensitive (hasFogCoord and numSelectedPoints);

			if (hasFogCoord and numSelectedPoints)
			{
				const auto index = currentTool -> getSelectedPoints () .begin () -> first;

				currentPage -> getFogDepthAdjustment () -> set_value (currentTool -> getFogCoord () -> get1Depth (index));
			}
			else
			{
				currentPage -> getFogDepthAdjustment () -> set_value (0);
			}

			break;
		}
	}

	changing = false;
}

void
X3DFogCoordinateEditor::on_fog_coord_toggled ()
{
	if (changing)
		return;

	const auto & currentPage = getBrowserWindow () -> getCurrentPage ();
	const auto   nodes       = X3D::MFNode (indexedFaceSetNodes);

	currentPage -> getFogCoordCheckButton () .set_inconsistent (false);

	addUndoFunction <X3D::SFNode> (nodes, "fogCoord", fogCoordUndoStep);

	if (currentPage -> getFogCoordCheckButton () .get_active ())
	{
		for (const auto & indexedFaceSetNode : indexedFaceSetNodes)
		{
			if (indexedFaceSetNode -> getFogCoord ())
				continue;

			indexedFaceSetNode -> addFogCoords ();
		}
	}
	else
	{
		for (const auto & indexedFaceSetNode : indexedFaceSetNodes)
			indexedFaceSetNode -> fogCoord () = nullptr;
	}

	addRedoFunction <X3D::SFNode> (nodes, "fogCoord", fogCoordUndoStep);
}

void
X3DFogCoordinateEditor::on_fog_depth_changed ()
{
	if (changing)
		return;

	const auto & currentPage = getBrowserWindow () -> getCurrentPage ();
	const auto   fogDepth    = currentPage -> getFogDepthAdjustment () -> get_value ();
	auto         fogCoords   = X3D::MFNode ();

	for (const auto & indexedFaceSetNode : indexedFaceSetNodes)
	{
		if (indexedFaceSetNode -> getFogCoord ())
			fogCoords .emplace_back (indexedFaceSetNode -> getFogCoord ());
	}

	addUndoFunction <X3D::MFFloat> (fogCoords, "depth", fogDepthUndoStep);

	for (const auto & indexedFaceSetNode : indexedFaceSetNodes)
	{
		if (not indexedFaceSetNode -> getFogCoord ())
			continue;

		const X3D::X3DPtr <X3D::IndexedFaceSetTool> tool (indexedFaceSetNode);

		if (not tool)
			continue;

		for (const auto & [index, point] : tool -> getSelectedPoints ())
			indexedFaceSetNode -> getFogCoord () -> set1Depth (index, fogDepth);
	}

	addRedoFunction <X3D::MFFloat> (fogCoords, "depth", fogDepthUndoStep);
}

X3D::X3DPtr <X3D::IndexedFaceSetTool>
X3DFogCoordinateEditor::getCurrentTool () const
{
	const auto result = std::max_element (indexedFaceSetNodes .begin (),
	                                      indexedFaceSetNodes .end (),
	                                      [ ] (const X3D::X3DPtr <X3D::IndexedFaceSet> & a, const X3D::X3DPtr <X3D::IndexedFaceSet> & b)
	                                      {
	                                         return a -> getField <X3D::SFTime> ("touchTime") <= b -> getField <X3D::SFTime> ("touchTime");
												     });

	return X3D::X3DPtr <X3D::IndexedFaceSetTool> (*result);
}

void
X3DFogCoordinateEditor::dispose ()
{ }

X3DFogCoordinateEditor::~X3DFogCoordinateEditor ()
{ }

} // puck
} // titania

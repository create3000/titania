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

#include "X3DGeometrySelectionEditor.h"

#include "../GeometryEditor/GeometryEditor.h"
#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Tools/Geometry3D/IndexedFaceSet/IndexedFaceSetTool.h>

namespace titania {
namespace puck {

static constexpr auto M2 = X3D::Matrix4d (2,0,0,0, 0,2,0,0, 0,0,2,0, 0,0,0,1);	

X3DGeometrySelectionEditor::X3DGeometrySelectionEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                        translation (this,
	                                     getGeometrySelectionTranslationXAdjustment (),
	                                     getGeometrySelectionTranslationYAdjustment (),
	                                     getGeometrySelectionTranslationZAdjustment (),
	                                     getGeometrySelectionTranslationBox (),
	                                     "translation"),
	                           rotation (this,
	                                     getGeometrySelectionRotationXAdjustment (),
	                                     getGeometrySelectionRotationYAdjustment (),
	                                     getGeometrySelectionRotationZAdjustment (),
	                                     getGeometrySelectionRotationAAdjustment (),
	                                     getGeometrySelectionRotationBox (),
	                                     "rotation"),
	                       rotationTool (new RotationTool (this, getGeometrySelectionRotationToolBox (), "rotation")),
	                              scale (this,
	                                     getGeometrySelectionScaleXAdjustment (),
	                                     getGeometrySelectionScaleYAdjustment (),
	                                     getGeometrySelectionScaleZAdjustment (),
	                                     getGeometrySelectionScaleBox (),
	                                     "scale"),
	                      transformNode (),
	                               tool (),
	                         lastMatrix (),
	                           undoStep ()
{
	addChildren (transformNode, tool);
}

void
X3DGeometrySelectionEditor::configure ()
{
	getGeometrySelectionUniformScaleButton () .set_active (getConfig () -> getBoolean ("geometrySelectionUniformScale"));

	getBrowserWindow () -> getGeometryEditor () -> getGeometryNodes () .addInterest (this, &X3DGeometrySelectionEditor::set_geometry_nodes);

	set_geometry_nodes (getBrowserWindow () -> getGeometryEditor () -> getGeometryNodes ());
}

void
X3DGeometrySelectionEditor::initialize ()
{
	translation .setUndo (false);
	rotation    .setUndo (false);
	scale       .setUndo (false);

	rotationTool -> setUndo (false);
}

void
X3DGeometrySelectionEditor::set_selection (const X3D::MFNode & selection)
{ }

void
X3DGeometrySelectionEditor::set_geometry_nodes (const X3D::MFNode & geometryNodes)
{
	for (const auto & node : geometryNodes)
	{
		const auto tool = X3D::X3DPtr <X3D::IndexedFaceSetTool> (node);

		if (tool)
			tool -> touchTime () .addInterest (this, &X3DGeometrySelectionEditor::set_touchTime);
	}

	set_touchTime ();
}

void
X3DGeometrySelectionEditor::set_touchTime ()
{
	if (tool)
	{
		tool -> selectedPoints_changed () .removeInterest (this, &X3DGeometrySelectionEditor::set_selectedPoints);
		tool -> selectedEdges_changed ()  .removeInterest (this, &X3DGeometrySelectionEditor::set_selectedEdges);
		tool -> selectedHoles_changed ()  .removeInterest (this, &X3DGeometrySelectionEditor::set_selectedHoles);
		tool -> selectedFaces_changed ()  .removeInterest (this, &X3DGeometrySelectionEditor::set_selectedFaces);

		tool -> getSelectionTransform () -> removeInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);
	}

	transformNode = getCurrentContext () -> createNode <X3D::Transform> ();
	tool          = getCurrentTool ();

	// Update composed widgets

	const X3D::MFNode transforms ({ transformNode });

	translation .setNodes (transforms);
	rotation    .setNodes (transforms);
	scale       .setNodes (transforms);
	rotationTool -> setNodes (transforms);

	//

	getGeometrySelectionExpander () .set_visible (tool);

	if (tool)
	{
		const auto & selectionTransform = tool -> getSelectionTransform ();

		tool -> selectedPoints_changed () .addInterest (this, &X3DGeometrySelectionEditor::set_selectedPoints);
		tool -> selectedEdges_changed ()  .addInterest (this, &X3DGeometrySelectionEditor::set_selectedEdges);
		tool -> selectedHoles_changed ()  .addInterest (this, &X3DGeometrySelectionEditor::set_selectedHoles);
		tool -> selectedFaces_changed ()  .addInterest (this, &X3DGeometrySelectionEditor::set_selectedFaces);

		transformNode      -> addInterest (this, &X3DGeometrySelectionEditor::set_matrix);
		selectionTransform -> addInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);

		set_selectedPoints ();
		set_selectedEdges ();
		set_selectedHoles ();
		set_selectedFaces ();
		set_tool_matrix ();
	}
}

void
X3DGeometrySelectionEditor::set_selectedPoints ()
{
	getGeometrySelectionBox () .set_sensitive (tool -> selectedPoints_changed ());
	getSelectedPointsLabel ()  .set_text (basic::to_string (tool -> selectedPoints_changed ()));
}

void
X3DGeometrySelectionEditor::set_selectedEdges ()
{
	getSelectedEdgesLabel () .set_text (basic::to_string (tool -> selectedEdges_changed ()));
}

void
X3DGeometrySelectionEditor::set_selectedHoles ()
{
	getSelectedHolesLabel () .set_text (basic::to_string (tool -> selectedHoles_changed ()));
}

void
X3DGeometrySelectionEditor::set_selectedFaces ()
{
	getSelectedFacesLabel () .set_text (basic::to_string (tool -> selectedFaces_changed ()));
}

void
X3DGeometrySelectionEditor::set_matrix ()
{
	try
	{
		tool -> getSelectionTransform () -> removeInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);
		tool -> getSelectionTransform () -> addInterest (this, &X3DGeometrySelectionEditor::connectToolMatrix);

		const auto & coordinateNode         = tool -> getCoord ();
		const auto   relativeTransformation = ~lastMatrix * transformNode -> getMatrix ();
		const auto   nodes                  = X3D::MFNode ({ coordinateNode });

		switch (coordinateNode -> getType () .back ())
		{
			case X3D::X3DConstants::Coordinate:
				addUndoFunction <X3D::MFVec3f> (nodes, "point", undoStep);
				break;
			case X3D::X3DConstants::CoordinateDouble:
			case X3D::X3DConstants::GeoCoordinate:
				addUndoFunction <X3D::MFVec3d> (nodes, "point", undoStep);
				break;
			default:
				break;
		}

		// Triggers later setMatrix.
		for (const auto & pair : tool -> getSelectedPoints ())
			coordinateNode -> set1Point (pair .first, pair .second * relativeTransformation);

		switch (coordinateNode -> getType () .back ())
		{
			case X3D::X3DConstants::Coordinate:
				addRedoFunction <X3D::MFVec3f> (nodes, "point", undoStep);
				break;
			case X3D::X3DConstants::CoordinateDouble:
			case X3D::X3DConstants::GeoCoordinate:
				addRedoFunction <X3D::MFVec3d> (nodes, "point", undoStep);
				break;
			default:
				break;
		}
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DGeometrySelectionEditor::set_tool_matrix ()
{
	undoStep .reset ();

	setMatrix ();
}

void
X3DGeometrySelectionEditor::setMatrix ()
{
	transformNode -> removeInterest (this, &X3DGeometrySelectionEditor::set_matrix);
	transformNode -> addInterest (this, &X3DGeometrySelectionEditor::connectMatrix);
	
	const auto   bbox                = tool -> getSelectionTransform () -> getBBox () .matrix ();
	const auto & axisRotation        = tool -> getAxisRotation ();
	const auto   localTransformation = M2 * bbox * axisRotation;

	transformNode -> setMatrix (localTransformation);

	lastMatrix = localTransformation;
}

void
X3DGeometrySelectionEditor::connectMatrix ()
{
	transformNode -> removeInterest (this, &X3DGeometrySelectionEditor::connectMatrix);
	transformNode -> addInterest (this, &X3DGeometrySelectionEditor::set_matrix);
}

void
X3DGeometrySelectionEditor::connectToolMatrix ()
{
	tool -> getSelectionTransform () -> removeInterest (this, &X3DGeometrySelectionEditor::connectToolMatrix);
	tool -> getSelectionTransform () -> addInterest (this, &X3DGeometrySelectionEditor::set_tool_matrix);

	setMatrix ();
}

bool
X3DGeometrySelectionEditor::on_geometry_selection_focus_in_event (GdkEventFocus* focus_event)
{
	undoStep .reset ();

	return false;
}

void
X3DGeometrySelectionEditor::on_geometry_selection_uniform_scale_toggled ()
{
	if (getGeometrySelectionUniformScaleButton () .get_active ())
	{
		getGeometrySelectionUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getGeometrySelectionUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

X3D::X3DPtr <X3D::IndexedFaceSetTool>
X3DGeometrySelectionEditor::getCurrentTool () const
{
	const auto geometryNodes = getNodes <X3D::X3DBaseNode> (getBrowserWindow () -> getGeometryEditor () -> getGeometryNodes (), { X3D::X3DConstants::IndexedFaceSetTool });

	const auto result = std::max_element (geometryNodes .begin (),
	                                      geometryNodes .end (),
	                                      [ ] (const X3D::SFNode & a, const X3D::SFNode & b)
	                                      {
	                                         return a -> getField <X3D::SFTime> ("touchTime") <= b -> getField <X3D::SFTime> ("touchTime");
												     });

	if (result == geometryNodes .end ())
		return nullptr;

	return X3D::X3DPtr <X3D::IndexedFaceSetTool> (*result);
}

void
X3DGeometrySelectionEditor::store ()
{
	getConfig () -> setItem ("geometrySelectionUniformScale", getGeometrySelectionUniformScaleButton () .get_active ());

	getBrowserWindow () -> getGeometryEditor () -> getGeometryNodes () .removeInterest (this, &X3DGeometrySelectionEditor::set_geometry_nodes);
}

X3DGeometrySelectionEditor::~X3DGeometrySelectionEditor ()
{ }

} // puck
} // titania

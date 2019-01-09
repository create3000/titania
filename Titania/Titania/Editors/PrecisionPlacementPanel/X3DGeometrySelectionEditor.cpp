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

#include "X3DGeometrySelectionEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Revealer/GeometryEditor/GeometryEditor.h"

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
	                       rotationTool (this, getGeometrySelectionRotationToolBox (), "rotation"),
	                           rotation (this,
	                                     getGeometrySelectionRotationXAdjustment (),
	                                     getGeometrySelectionRotationYAdjustment (),
	                                     getGeometrySelectionRotationZAdjustment (),
	                                     getGeometrySelectionRotationAAdjustment (),
	                                     getGeometrySelectionRotationBox (),
	                                     "rotation"),
	                      rotationEuler (this,
	                                     getGeometrySelectionRotationEulerXAdjustment (),
	                                     getGeometrySelectionRotationEulerYAdjustment (),
	                                     getGeometrySelectionRotationEulerZAdjustment (),
	                                     getGeometrySelectionRotationEulerBox (),
	                                     "rotation"),
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
	addChildObjects (transformNode, tool);
}

void
X3DGeometrySelectionEditor::initialize ()
{
	translation   .setUndo (false);
	rotationTool  .setUndo (false);
	rotation      .setUndo (false);
	rotationEuler .setUndo (false);
	scale         .setUndo (false);
}

void
X3DGeometrySelectionEditor::configure ()
{
	switch (getConfig () -> getItem <int32_t> ("geometrySelectionRotationType"))
	{
		case 1:
			getGeometrySelectionRotationEulerButton () .set_active (true);
			break;
		default:
			getGeometrySelectionRotationAxisAngleButton () .set_active (true);
			break;
	}

	getGeometrySelectionUniformScaleButton () .set_active (getConfig () -> getItem <bool> ("geometrySelectionUniformScale"));

	set_geometries (getBrowserWindow () -> getSelection () -> getGeometries ());
}

void
X3DGeometrySelectionEditor::set_geometries (const X3D::MFNode & geometryNodes)
{
	transformNode = getCurrentContext () -> createNode <X3D::Transform> ();

	// Update composed widgets

	const X3D::MFNode transforms ({ transformNode });

	transformNode -> scale () .setUnit (X3D::UnitCategory::LENGTH);

	translation   .setNodes (transforms);
	rotationTool  .setNodes (transforms);
	rotation      .setNodes (transforms);
	rotationEuler .setNodes (transforms);
	scale         .setNodes (transforms);

	// IndexedFaceSetTool detection

	for (const auto & node : geometryNodes)
	{
		const X3D::X3DPtr <X3D::IndexedFaceSetTool> tool (node);

		if (tool)
			tool -> touchTime () .addInterest (&X3DGeometrySelectionEditor::set_touchTime, this);
	}

	set_touchTime ();
}

void
X3DGeometrySelectionEditor::set_touchTime ()
{
	if (tool)
	{
		tool -> selectedPoints_changed () .removeInterest (&X3DGeometrySelectionEditor::set_selectedPoints, this);
		tool -> selectedEdges_changed ()  .removeInterest (&X3DGeometrySelectionEditor::set_selectedEdges, this);
		tool -> selectedHoles_changed ()  .removeInterest (&X3DGeometrySelectionEditor::set_selectedHoles, this);
		tool -> selectedFaces_changed ()  .removeInterest (&X3DGeometrySelectionEditor::set_selectedFaces, this);

		tool -> getSelectionTransform () -> removeInterest (&X3DGeometrySelectionEditor::set_tool_matrix, this);
	}

	tool = getCurrentTool ();

	//

	getGeometrySelectionExpander () .set_visible (tool);

	if (tool)
	{
		const auto & selectionTransform = tool -> getSelectionTransform ();

		tool -> selectedPoints_changed () .addInterest (&X3DGeometrySelectionEditor::set_selectedPoints, this);
		tool -> selectedEdges_changed ()  .addInterest (&X3DGeometrySelectionEditor::set_selectedEdges, this);
		tool -> selectedHoles_changed ()  .addInterest (&X3DGeometrySelectionEditor::set_selectedHoles, this);
		tool -> selectedFaces_changed ()  .addInterest (&X3DGeometrySelectionEditor::set_selectedFaces, this);

		transformNode      -> addInterest (&X3DGeometrySelectionEditor::set_matrix, this);
		selectionTransform -> addInterest (&X3DGeometrySelectionEditor::set_tool_matrix, this);

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
		tool -> getSelectionTransform () -> removeInterest (&X3DGeometrySelectionEditor::set_tool_matrix, this);
		tool -> getSelectionTransform () -> addInterest (&X3DGeometrySelectionEditor::connectToolMatrix, this);

		const auto & coordNode              = tool -> getCoord ();
		const auto   relativeTransformation = inverse (lastMatrix) * transformNode -> getMatrix ();
		const auto   nodes                  = X3D::MFNode ({ coordNode });

		switch (coordNode -> getType () .back ())
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
			coordNode -> set1Point (pair .first, pair .second * relativeTransformation);

		switch (coordNode -> getType () .back ())
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
	catch (const std::domain_error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
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
	transformNode -> removeInterest (&X3DGeometrySelectionEditor::set_matrix, this);
	transformNode -> addInterest (&X3DGeometrySelectionEditor::connectMatrix, this);
	
	const auto   bbox                = tool -> getSelectionTransform () -> getBBox () .matrix ();
	const auto & axisRotation        = tool -> getAxisRotation ();
	const auto   localTransformation = M2 * bbox * axisRotation;

	transformNode -> setMatrix (localTransformation);

	lastMatrix = localTransformation;
}

void
X3DGeometrySelectionEditor::connectMatrix ()
{
	transformNode -> removeInterest (&X3DGeometrySelectionEditor::connectMatrix, this);
	transformNode -> addInterest (&X3DGeometrySelectionEditor::set_matrix, this);
}

void
X3DGeometrySelectionEditor::connectToolMatrix ()
{
	tool -> getSelectionTransform () -> removeInterest (&X3DGeometrySelectionEditor::connectToolMatrix, this);
	tool -> getSelectionTransform () -> addInterest (&X3DGeometrySelectionEditor::set_tool_matrix, this);

	setMatrix ();
}

void
X3DGeometrySelectionEditor::on_geometry_selection_map ()
{
	getBrowserWindow () -> getSelection () -> getGeometries () .addInterest (&X3DGeometrySelectionEditor::set_geometries, this);
}

void
X3DGeometrySelectionEditor::on_geometry_selection_unmap ()
{
	getBrowserWindow () -> getSelection () -> getGeometries () .removeInterest (&X3DGeometrySelectionEditor::set_geometries, this);
}

bool
X3DGeometrySelectionEditor::on_geometry_selection_focus_in_event (GdkEventFocus* focus_event)
{
	undoStep .reset ();

	return false;
}

void
X3DGeometrySelectionEditor::on_geometry_selection_rotation_type_clicked ()
{
	getGeometrySelectionRotationPopover () .popup ();
}

void
X3DGeometrySelectionEditor::on_geometry_selection_rotation_axis_angle_toggled ()
{
	getGeometrySelectionRotationPopover () .popdown ();

	if (not getGeometrySelectionRotationAxisAngleButton () .get_active ())
		return;

	for (const auto widget : getGeometrySelectionRotationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeometrySelectionRotationBox () .set_visible (true);
	getGeometrySelectionRotationNotebook () .set_current_page (0);

	getConfig () -> setItem <int32_t> ("geometrySelectionRotationType", 0);
}

void
X3DGeometrySelectionEditor::on_geometry_selection_rotation_euler_toggled ()
{
	getGeometrySelectionRotationPopover () .popdown ();

	if (not getGeometrySelectionRotationEulerButton () .get_active ())
		return;

	for (const auto widget : getGeometrySelectionRotationNotebook () .get_children ())
		widget -> set_visible (false);

	getGeometrySelectionRotationEulerBox () .set_visible (true);
	getGeometrySelectionRotationNotebook () .set_current_page (1);

	getConfig () -> setItem <int32_t> ("geometrySelectionRotationType", 1);
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
	const auto geometryNodes = getNodes <X3D::X3DBaseNode> (getBrowserWindow () -> getSelection () -> getGeometries (), { X3D::X3DConstants::IndexedFaceSetTool });

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
}

X3DGeometrySelectionEditor::~X3DGeometrySelectionEditor ()
{ }

} // puck
} // titania

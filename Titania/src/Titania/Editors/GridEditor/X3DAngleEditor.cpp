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

#include "X3DAngleEditor.h"

#include "../../Configuration/config.h"
#include "X3DGridTool.h"

#include <Titania/X3D/Tools/Grids/X3DGridTool.h>

namespace titania {
namespace puck {

static constexpr int INDICES = 3;

X3DAngleEditor::X3DAngleEditor () :
	X3DGridEditorInterface (),
	               enabled (this, getAngleEnableSnappingCheckButton (), "enabled"),
	           translation (this,
	                        getAngleTranslationXAdjustment (),
	                        getAngleTranslationYAdjustment (),
	                        getAngleTranslationZAdjustment (),
	                        getAngleTranslationBox (),
	                        "translation"),
	                 scale (this,
	                        getAngleScaleXAdjustment (),
	                        getAngleScaleYAdjustment (),
	                        getAngleScaleZAdjustment (),
	                        getAngleScaleBox (),
	                        "scale"),
	            dimension0 (this, getAngleDimension0Adjustment (), getAngleDimensionSpinButton0 (), "dimension"),
	            dimension1 (this, getAngleDimension1Adjustment (), getAngleDimensionSpinButton1 (), "dimension"),
	            dimension2 (this, getAngleDimensionYAdjustment (), getAngleDimensionSpinButton2 (), "dimension"),
	       majorLineEvery0 (this, getAngleMajorLineEvery0Adjustment (), getAngleMajorLineEverySpinButton0 (), "majorLineEvery"),
	       majorLineEvery1 (this, getAngleMajorLineEvery1Adjustment (), getAngleMajorLineEverySpinButton1 (), "majorLineEvery"),
	       majorLineEvery2 (this, getAngleMajorLineEveryYAdjustment (), getAngleMajorLineEverySpinButton2 (), "majorLineEvery"),
	      majorLineOffset0 (this, getAngleMajorLineOffset0Adjustment (), getAngleMajorLineOffsetSpinButton0 (), "majorLineOffset"),
	      majorLineOffset1 (this, getAngleMajorLineOffset1Adjustment (), getAngleMajorLineOffsetSpinButton1 (), "majorLineOffset"),
	      majorLineOffset2 (this, getAngleMajorLineOffsetYAdjustment (), getAngleMajorLineOffsetSpinButton2 (), "majorLineOffset"),
	                 color (this,
	                        getAngleColorButton (),
	                        getAngleColorAdjustment (),
	                        getAngleColorBox (),
	                        "color"),
	             lineColor (this,
	                        getAngleLineColorButton (),
	                        getAngleLineColorAdjustment (),
	                        getAngleLineColorBox (),
	                        "lineColor"),
	        majorLineColor (this,
	                        getAngleMajorLineColorButton (),
	                        getAngleMajorLineColorAdjustment (),
	                        getAngleMajorLineColorBox (),
	                        "majorLineColor"),
	          snapDistance (this, getAngleSnapDistanceAdjustment (), getAngleSnapDistanceSpinButton (), "snapDistance"),
	          snapToCenter (this, getAngleSnapToCenterCheckButton (), "snapToCenter"),
	              undoStep (),
	              changing (false)
{
	//translation     .setUndo (false);
	//scale           .setUndo (false);
	//dimension       .setUndo (false);
	//majorLineEvery  .setUndo (false);
	//majorLineOffset .setUndo (false);
	//color           .setUndo (false);
	//lineColor       .setUndo (false);
	//majorLineColor  .setUndo (false);

	dimension0       .setIndex (0);
	dimension1       .setIndex (1);
	dimension2       .setIndex (2);
	majorLineEvery0  .setIndex (0);
	majorLineEvery1  .setIndex (1);
	majorLineEvery2  .setIndex (2);
	majorLineOffset0 .setIndex (0);
	majorLineOffset1 .setIndex (1);
	majorLineOffset2 .setIndex (2);
}

void
X3DAngleEditor::initialize ()
{
	const auto &      angleTool  = getBrowserWindow () -> getAngleGridTool () -> getTool ();
	const X3D::MFNode angleTools = { angleTool };

	enabled          .setNodes (angleTools);
	translation      .setNodes (angleTools);
	scale            .setNodes (angleTools);
	dimension0       .setNodes (angleTools);
	dimension1       .setNodes (angleTools);
	dimension2       .setNodes (angleTools);
	majorLineEvery0  .setNodes (angleTools);
	majorLineEvery1  .setNodes (angleTools);
	majorLineEvery2  .setNodes (angleTools);
	majorLineOffset0 .setNodes (angleTools);
	majorLineOffset1 .setNodes (angleTools);
	majorLineOffset2 .setNodes (angleTools);
	color            .setNodes (angleTools);
	lineColor        .setNodes (angleTools);
	majorLineColor   .setNodes (angleTools);
	snapDistance     .setNodes (angleTools);
	snapToCenter     .setNodes (angleTools);

	getBrowserWindow () -> getAngleGridTool () -> getEnabled () .addInterest (&X3DAngleEditor::set_angle_grid_visible, this);

	angleTool -> rotation ()        .addInterest (&X3DAngleEditor::set_rotation,       this);
	angleTool -> majorLineEvery ()  .addInterest (&X3DAngleEditor::set_majorLineEvery, this);
	angleTool -> majorLineOffset () .addInterest (&X3DAngleEditor::set_majorLineEvery, this);
	getCurrentScene ()              .addInterest (&X3DAngleEditor::set_majorLineEvery, this);

	set_angle_grid_visible ();
	on_angle_toggled ();
	set_rotation ();
	set_majorLineEvery ();
}

void
X3DAngleEditor::configure ()
{
	getAngleUniformScaleButton () .set_active (getConfig () -> getItem <bool> ("angleUniformScale"));
}

void
X3DAngleEditor::set_angle_grid_visible ()
{
	changing = true;

	const auto visible = getBrowserWindow () -> getAngleGridTool () -> getEnabled ();

	getAngleTransformBox ()            .set_sensitive (visible);
	getAngleMajorLinesBox ()           .set_sensitive (visible);
	getAngleColorsBox ()               .set_sensitive (visible);
	getAngleAdditonalScrolledWindow () .set_visible   (visible);

	getAngleCheckButton () .set_active (visible);

	changing = false;
}

void
X3DAngleEditor::on_angle_toggled ()
{
	if (changing)
		return;

	getBrowserWindow () -> getAngleGridTool () -> setEnabled (getAngleCheckButton () .get_active ());
}

void
X3DAngleEditor::on_angle_plane_changed ()
{
	if (changing)
		return;

	const auto &      grid  = getBrowserWindow () -> getAngleGridTool () -> getTool ();
	const X3D::MFNode nodes = { grid };

	addUndoFunction <X3D::SFRotation> (nodes, "rotation", undoStep);

	grid -> rotation () .removeInterest (&X3DAngleEditor::set_rotation, this);
	grid -> rotation () .addInterest (&X3DAngleEditor::connectRotation, this);

	getBrowserWindow () -> getAngleGridTool () -> setPlane (getAnglePlaneComboBoxText () .get_active_row_number ());

	addRedoFunction <X3D::SFRotation> (nodes, "rotation", undoStep);
}

void
X3DAngleEditor::set_rotation ()
{
	changing = true;

	getAnglePlaneComboBoxText () .set_active (getBrowserWindow () -> getAngleGridTool () -> getPlane ());

	changing = false;
}

void
X3DAngleEditor::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (&X3DAngleEditor::connectRotation, this);
	field .addInterest (&X3DAngleEditor::set_rotation, this);
}

void
X3DAngleEditor::on_angle_uniform_scale_clicked ()
{
	if (getAngleUniformScaleButton () .get_active ())
	{
		getAngleUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getAngleUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

void
X3DAngleEditor::on_angle_major_line_grid_value_changed ()
{
	const auto & grid  = getBrowserWindow () -> getAngleGridTool () -> getTool ();
	const size_t index = getAngleMajorGridAdjustment () -> get_value () - 1;

	if (grid -> majorLineEvery () .size () < INDICES * index + INDICES)
		grid -> majorLineEvery () .resize (INDICES * index + INDICES);

	if (grid -> majorLineOffset () .size () < INDICES * index + INDICES)
		grid -> majorLineOffset () .resize (INDICES * index + INDICES);

	majorLineEvery0  .setIndex (INDICES * index + 0);
	majorLineEvery1  .setIndex (INDICES * index + 1);
	majorLineEvery2  .setIndex (INDICES * index + 2);
	majorLineOffset0 .setIndex (INDICES * index + 0);
	majorLineOffset1 .setIndex (INDICES * index + 1);
	majorLineOffset2 .setIndex (INDICES * index + 2);
}

void
X3DAngleEditor::on_angle_major_line_grid_upper_changed ()
{
	getAngleMajorGridSpinButton ()   .set_sensitive (getAngleMajorGridAdjustment () -> get_upper () > 1);
	getAngleRemoveMajorGridButton () .set_sensitive (getAngleMajorGridAdjustment () -> get_upper () > 0);
}

void
X3DAngleEditor::on_angle_add_major_line_grid ()
{
	const auto   undoStep = std::make_shared <X3D::UndoStep> ("Add Major Line Grid");
	const auto & grid     = getBrowserWindow () -> getAngleGridTool () -> getTool ();
	const int    size     = getAngleMajorGridAdjustment () -> get_upper () + 1;

	undoStep -> addObjects (grid);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	getAngleMajorGridAdjustment () -> set_lower (1);
	getAngleMajorGridAdjustment () -> set_upper (size);
	getAngleMajorGridAdjustment () -> set_value (size);

	on_angle_major_line_grid_upper_changed ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	addUndoStep (undoStep);
}

void
X3DAngleEditor::on_angle_remove_major_line_grid ()
{
	const auto   undoStep = std::make_shared <X3D::UndoStep> ("Remove Major Line Grid");
	const auto & grid     = getBrowserWindow () -> getAngleGridTool () -> getTool ();
	const int    size     = getAngleMajorGridAdjustment () -> get_upper () - 1;
	const int    index    = (getAngleMajorGridAdjustment () -> get_value () - 1) * INDICES;
	const auto   iterL    = grid -> majorLineEvery ()  .begin () + index;
	const auto   iterO    = grid -> majorLineOffset () .begin () + index;

	undoStep -> addObjects (grid);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	grid -> majorLineEvery ()  .erase (iterL, iterL + INDICES);
	grid -> majorLineOffset () .erase (iterO, iterO + INDICES);

	getAngleMajorGridAdjustment () -> set_lower (bool (size));
	getAngleMajorGridAdjustment () -> set_upper (size);

	if (getAngleMajorGridAdjustment () -> get_value () > size)
		getAngleMajorGridAdjustment () -> set_value (size);

	on_angle_major_line_grid_upper_changed ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	addUndoStep (undoStep);
}

void
X3DAngleEditor::set_majorLineEvery ()
{
	const auto & grid = getBrowserWindow () -> getAngleGridTool () -> getTool ();
	const auto   size = grid -> majorLineEvery () .size () / INDICES;

	getAngleMajorGridAdjustment () -> set_lower (bool (size));
	getAngleMajorGridAdjustment () -> set_upper (size);
	getAngleMajorGridAdjustment () -> set_value (std::min <int32_t> (getAngleMajorGridAdjustment () -> get_value (), size > 0 ? size : 0));

	on_angle_major_line_grid_upper_changed ();
}

void
X3DAngleEditor::store ()
{
	getConfig () -> setItem ("angleUniformScale", getAngleUniformScaleButton () .get_active ());
}

X3DAngleEditor::~X3DAngleEditor ()
{ }

} // puck
} // titania

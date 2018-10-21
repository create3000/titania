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
 * Free Softwar	getAngleTransformBox ()  .set_sensitive (getAngleCheckButton () .get_active ());
	getAngleMajorLinesBox () .set_sensitive (getAngleCheckButton () .get_active ());
	getAngleColorsBox ()     .set_sensitive (getAngleCheckButton () .get_active ());
e Foundation.
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

#include "X3DAxonometricGridEditor.h"

#include "../../Configuration/config.h"
#include "X3DGridTool.h"

#include <Titania/X3D/Tools/Grids/AxonometricGridTool.h>

namespace titania {
namespace puck {

static constexpr int INDICES = 4;

X3DAxonometricGridEditor::X3DAxonometricGridEditor () :
	X3DGridEditorInterface (),
	               enabled (this, getAxonometricGridEnableSnappingCheckButton (), "enabled"),
	           translation (this,
	                        getAxonometricGridTranslationXAdjustment (),
	                        getAxonometricGridTranslationYAdjustment (),
	                        getAxonometricGridTranslationZAdjustment (),
	                        getAxonometricGridTranslationBox (),
	                        "translation"),
	                 scale (this,
	                        getAxonometricGridScaleXAdjustment (),
	                        getAxonometricGridScaleYAdjustment (),
	                        getAxonometricGridScaleZAdjustment (),
	                        getAxonometricGridScaleBox (),
	                        "scale"),
	            dimension0 (this, getAxonometricGridUVTDimensionAdjustment (), getAxonometricGridDimensionSpinButton0 (), "dimension"),
	            dimension1 (this, getAxonometricGridYDimensionAdjustment (), getAxonometricGridDimensionSpinButton1 (), "dimension"),
	                 angle (this,
	                        getAxonometricGridAngleAlphaAdjustment (),
	                        getAxonometricGridAngleBetaAdjustment (),
	                        getAxonometricGridAngleBox (),
	                        "angle"),
	                 gamma (this, getAxonometricGridAngleGammaAdjustment (), getAxonometricGridAngleGammaSpinButton (), "gamma"),
	       majorLineEvery0 (this, getAxonometricGridMajorLineEveryUAdjustment (), getAxonometricGridMajorLineEverySpinButton0 (), "majorLineEvery"),
	       majorLineEvery1 (this, getAxonometricGridMajorLineEveryVAdjustment (), getAxonometricGridMajorLineEverySpinButton1 (), "majorLineEvery"),
	       majorLineEvery2 (this, getAxonometricGridMajorLineEveryTAdjustment (), getAxonometricGridMajorLineEverySpinButton2 (), "majorLineEvery"),
	       majorLineEvery3 (this, getAxonometricGridMajorLineEveryYAdjustment (), getAxonometricGridMajorLineEverySpinButton3 (), "majorLineEvery"),
	      majorLineOffset0 (this, getAxonometricGridMajorLineOffsetUAdjustment (), getAxonometricGridMajorLineOffsetSpinButton0 (), "majorLineOffset"),
	      majorLineOffset1 (this, getAxonometricGridMajorLineOffsetVAdjustment (), getAxonometricGridMajorLineOffsetSpinButton1 (), "majorLineOffset"),
	      majorLineOffset2 (this, getAxonometricGridMajorLineOffsetTAdjustment (), getAxonometricGridMajorLineOffsetSpinButton2 (), "majorLineOffset"),
	      majorLineOffset3 (this, getAxonometricGridMajorLineOffsetYAdjustment (), getAxonometricGridMajorLineOffsetSpinButton3 (), "majorLineOffset"),
	                 color (this,
	                        getAxonometricGridColorButton (),
	                        getAxonometricGridColorAdjustment (),
	                        getAxonometricGridColorBox (),
	                        "color"),
	             lineColor (this,
	                        getAxonometricGridLineColorButton (),
	                        getAxonometricGridLineColorAdjustment (),
	                        getAxonometricGridLineColorBox (),
	                        "lineColor"),
	        majorLineColor (this,
	                        getAxonometricGridMajorLineColorButton (),
	                        getAxonometricGridMajorLineColorAdjustment (),
	                        getAxonometricGridMajorLineColorBox (),
	                        "majorLineColor"),
	          snapDistance (this, getAxonometricGridSnapDistanceAdjustment (), getAxonometricGridSnapDistanceSpinButton (), "snapDistance"),
	          snapToCenter (this, getAxonometricGridSnapToCenterCheckButton (), "snapToCenter"),
	                fields (getMasterBrowser () -> createNode <X3D::FieldSet> ()),
	              undoStep (),
	              changing (false)
{
	addChildObjects (fields);

	fields -> addUserDefinedField (X3D::inputOutput, "gamma", new X3D::SFDouble ());
	fields -> getField <X3D::SFDouble> ("gamma") .setUnit (X3D::UnitCategory::ANGLE);

	dimension0       .setIndex (0);
	dimension1       .setIndex (1);
	majorLineEvery0  .setIndex (0);
	majorLineEvery1  .setIndex (1);
	majorLineEvery2  .setIndex (2);
	majorLineEvery3  .setIndex (3);
	majorLineOffset0 .setIndex (0);
	majorLineOffset1 .setIndex (1);
	majorLineOffset2 .setIndex (2);
	majorLineOffset3 .setIndex (3);
}

void
X3DAxonometricGridEditor::initialize ()
{
	const auto &      gridTool  = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
	const X3D::MFNode gridTools = { gridTool };

	enabled          .setNodes (gridTools);
	translation      .setNodes (gridTools);
	scale            .setNodes (gridTools);
	dimension0       .setNodes (gridTools);
	dimension1       .setNodes (gridTools);
	angle            .setNodes (gridTools);
	gamma            .setNodes ({ fields });
	majorLineEvery0  .setNodes (gridTools);
	majorLineEvery1  .setNodes (gridTools);
	majorLineEvery2  .setNodes (gridTools);
	majorLineEvery3  .setNodes (gridTools);
	majorLineOffset0 .setNodes (gridTools);
	majorLineOffset1 .setNodes (gridTools);
	majorLineOffset2 .setNodes (gridTools);
	majorLineOffset3 .setNodes (gridTools);
	color            .setNodes (gridTools);
	lineColor        .setNodes (gridTools);
	majorLineColor   .setNodes (gridTools);
	snapDistance     .setNodes (gridTools);
	snapToCenter     .setNodes (gridTools);

	getBrowserWindow () -> getAxonometricGridTool () -> getEnabled () .addInterest (&X3DAxonometricGridEditor::set_axonometric_grid_visible, this);

	gridTool -> rotation ()        .addInterest (&X3DAxonometricGridEditor::set_rotation, this);
	gridTool -> majorLineEvery ()  .addInterest (&X3DAxonometricGridEditor::set_majorLineEvery, this);
	gridTool -> majorLineOffset () .addInterest (&X3DAxonometricGridEditor::set_majorLineEvery, this);
	gridTool -> getField <X3D::SFVec2d> ("angle") .addInterest (&X3DAxonometricGridEditor::set_angle, this);
	getCurrentScene () .addInterest (&X3DAxonometricGridEditor::set_angle, this);
	getCurrentScene () .addInterest (&X3DAxonometricGridEditor::set_majorLineEvery, this);

	set_axonometric_grid_visible ();
	on_axonometric_grid_toggled ();
	set_angle ();
	set_rotation ();
	set_majorLineEvery ();
}

void
X3DAxonometricGridEditor::configure ()
{
	getAxonometricGridUniformScaleButton () .set_active (getConfig () -> getItem <bool> ("axonometricGridUniformScale"));
}

void
X3DAxonometricGridEditor::set_angle ()
{
	const auto & gridTool = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
	const auto & angle    = gridTool -> getField <X3D::SFVec2d> ("angle");
	const auto   alpha    = getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, angle [0]);
	const auto   beta     = getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, angle [1]);
	const auto   pi       = getCurrentScene () -> toUnit (X3D::UnitCategory::ANGLE, math::pi <double>);

	getAxonometricGridAngleAlphaAdjustment () -> set_upper (pi - beta);
	getAxonometricGridAngleBetaAdjustment ()  -> set_upper (pi - alpha);

	fields -> setField <X3D::SFDouble> ("gamma", math::pi <double> - angle [0] - angle [1]);
}

void
X3DAxonometricGridEditor::set_axonometric_grid_visible ()
{
	changing = true;

	const auto visible = getBrowserWindow () -> getAxonometricGridTool () -> getEnabled ();

	getAxonometricGridTransformBox ()            .set_sensitive (visible);
	getAxonometricGridMajorLinesBox ()           .set_sensitive (visible);
	getAxonometricGridColorsBox ()               .set_sensitive (visible);
	getAxonometricGridAdditonalScrolledWindow () .set_visible   (visible);

	getAxonometricGridCheckButton () .set_active (visible);

	changing = false;
}

void
X3DAxonometricGridEditor::on_axonometric_grid_toggled ()
{
	if (changing)
		return;

	getBrowserWindow () -> getAxonometricGridTool () -> setEnabled (getAxonometricGridCheckButton () .get_active ());
}

void
X3DAxonometricGridEditor::on_axonometric_grid_plane_changed ()
{
	if (changing)
		return;

	const auto &      grid  = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
	const X3D::MFNode nodes = { grid };

	addUndoFunction <X3D::SFRotation> (nodes, "rotation", undoStep);

	grid -> rotation () .removeInterest (&X3DAxonometricGridEditor::set_rotation, this);
	grid -> rotation () .addInterest (&X3DAxonometricGridEditor::connectRotation, this);

	getBrowserWindow () -> getAxonometricGridTool () -> setPlane (getAxonometricGridPlaneComboBoxText () .get_active_row_number ());

	addRedoFunction <X3D::SFRotation> (nodes, "rotation", undoStep);
}

void
X3DAxonometricGridEditor::set_rotation ()
{
	changing = true;

	getAxonometricGridPlaneComboBoxText () .set_active (getBrowserWindow () -> getAxonometricGridTool () -> getPlane ());

	changing = false;
}

void
X3DAxonometricGridEditor::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (&X3DAxonometricGridEditor::connectRotation, this);
	field .addInterest (&X3DAxonometricGridEditor::set_rotation, this);
}

void
X3DAxonometricGridEditor::on_axonometric_grid_uniform_scale_clicked ()
{
	if (getAxonometricGridUniformScaleButton () .get_active ())
	{
		getAxonometricGridUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getAxonometricGridUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

void
X3DAxonometricGridEditor::on_axonometric_major_line_grid_value_changed ()
{
	const auto & grid  = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
	const size_t index = getAxonometricGridMajorGridAdjustment () -> get_value () - 1;

	if (grid -> majorLineEvery () .size () < INDICES * index + INDICES)
		grid -> majorLineEvery () .resize (INDICES * index + INDICES);

	if (grid -> majorLineOffset () .size () < INDICES * index + INDICES)
		grid -> majorLineOffset () .resize (INDICES * index + INDICES);

	majorLineEvery0  .setIndex (INDICES * index + 0);
	majorLineEvery1  .setIndex (INDICES * index + 1);
	majorLineEvery2  .setIndex (INDICES * index + 2);
	majorLineEvery3  .setIndex (INDICES * index + 3);
	majorLineOffset0 .setIndex (INDICES * index + 0);
	majorLineOffset1 .setIndex (INDICES * index + 1);
	majorLineOffset2 .setIndex (INDICES * index + 2);
	majorLineOffset3 .setIndex (INDICES * index + 3);
}

void
X3DAxonometricGridEditor::on_axonometric_major_line_grid_upper_changed ()
{
	getAxonometricGridMajorGridSpinButton ()   .set_sensitive (getAxonometricGridMajorGridAdjustment () -> get_upper () > 1);
	getAxonometricGridRemoveMajorGridButton () .set_sensitive (getAxonometricGridMajorGridAdjustment () -> get_upper () > 0);
}

void
X3DAxonometricGridEditor::on_axonometric_add_major_line_grid ()
{
	const auto   undoStep = std::make_shared <X3D::UndoStep> ("Add Major Line Grid");
	const auto & grid     = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
	const int    size     = getAxonometricGridMajorGridAdjustment () -> get_upper () + 1;

	undoStep -> addObjects (grid);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	getAxonometricGridMajorGridAdjustment () -> set_lower (1);
	getAxonometricGridMajorGridAdjustment () -> set_upper (size);
	getAxonometricGridMajorGridAdjustment () -> set_value (size);
	
	on_axonometric_major_line_grid_upper_changed ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	addUndoStep (undoStep);
}

void
X3DAxonometricGridEditor::on_axonometric_remove_major_line_grid ()
{
	const auto   undoStep = std::make_shared <X3D::UndoStep> ("Remove Major Line Grid");
	const auto & grid     = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
	const int    size     = getAxonometricGridMajorGridAdjustment () -> get_upper () - 1;
	const int    index    = (getAxonometricGridMajorGridAdjustment () -> get_value () - 1) * INDICES;
	const auto   iterL    = grid -> majorLineEvery ()  .begin () + index;
	const auto   iterO    = grid -> majorLineOffset () .begin () + index;

	undoStep -> addObjects (grid);
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	grid -> majorLineEvery ()  .erase (iterL, iterL + INDICES);
	grid -> majorLineOffset () .erase (iterO, iterO + INDICES);

	getAxonometricGridMajorGridAdjustment () -> set_lower (bool (size));
	getAxonometricGridMajorGridAdjustment () -> set_upper (size);
	
	if (getAxonometricGridMajorGridAdjustment () -> get_value () > size)
		getAxonometricGridMajorGridAdjustment () -> set_value (size);
		
	on_axonometric_major_line_grid_upper_changed ();

	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineEvery ()),  grid -> majorLineEvery ());
	undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (grid -> majorLineOffset ()), grid -> majorLineOffset ());

	addUndoStep (undoStep);
}

void
X3DAxonometricGridEditor::set_majorLineEvery ()
{
	const auto & grid = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
	const auto   size = grid -> majorLineEvery () .size () / INDICES;

	getAxonometricGridMajorGridAdjustment () -> set_lower (bool (size));
	getAxonometricGridMajorGridAdjustment () -> set_upper (size);
	getAxonometricGridMajorGridAdjustment () -> set_value (std::min <int32_t> (getAxonometricGridMajorGridAdjustment () -> get_value (), size > 0 ? size : 0));

	on_axonometric_major_line_grid_upper_changed ();
}

void
X3DAxonometricGridEditor::store ()
{
	getConfig () -> setItem ("axonometricGridUniformScale", getAxonometricGridUniformScaleButton () .get_active ());
}

X3DAxonometricGridEditor::~X3DAxonometricGridEditor ()
{ }

} // puck
} // titania

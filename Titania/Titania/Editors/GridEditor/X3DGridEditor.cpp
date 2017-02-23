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

#include "X3DGridEditor.h"

#include "../../Configuration/config.h"
#include "X3DGridTool.h"

#include <Titania/X3D/Tools/Grids/X3DGridTool.h>

namespace titania {
namespace puck {

using math::pi;

static const auto X_PLANE_ROTATION = X3D::Rotation4d (0, 0, -1, pi <double> / 2) * X3D::Rotation4d (1, 0, 0, pi <double> / 2);
static const auto Y_PLANE_ROTATION = X3D::Rotation4d ();
static const auto Z_PLANE_ROTATION = X3D::Rotation4d (1, 0, 0, pi <double> / 2);

static constexpr int INDICES = 3;

X3DGridEditor::X3DGridEditor () :
	X3DGridEditorInterface (),
	               enabled (this, getGridEnabledCheckButton (), "enabled"),
	           translation (this,
	                        getGridTranslationXAdjustment (),
	                        getGridTranslationYAdjustment (),
	                        getGridTranslationZAdjustment (),
	                        getGridTranslationBox (),
	                        "translation"),
	                 scale (this,
	                        getGridScaleXAdjustment (),
	                        getGridScaleYAdjustment (),
	                        getGridScaleZAdjustment (),
	                        getGridScaleBox (),
	                        "scale"),
	            dimension0 (this, getGridXDimensionAdjustment (), getGridDimensionSpinButton0 (), "dimension"),
	            dimension1 (this, getGridYDimensionAdjustment (), getGridDimensionSpinButton1 (), "dimension"),
	            dimension2 (this, getGridZDimensionAdjustment (), getGridDimensionSpinButton2 (), "dimension"),
	       majorLineEvery0 (this, getGridMajorLineEveryXAdjustment (), getGridMajorLineEverySpinButton0 (), "majorLineEvery"),
	       majorLineEvery1 (this, getGridMajorLineEveryYAdjustment (), getGridMajorLineEverySpinButton1 (), "majorLineEvery"),
	       majorLineEvery2 (this, getGridMajorLineEveryZAdjustment (), getGridMajorLineEverySpinButton2 (), "majorLineEvery"),
	      majorLineOffset0 (this, getGridMajorLineOffsetXAdjustment (), getGridMajorLineOffsetSpinButton0 (), "majorLineOffset"),
	      majorLineOffset1 (this, getGridMajorLineOffsetYAdjustment (), getGridMajorLineOffsetSpinButton1 (), "majorLineOffset"),
	      majorLineOffset2 (this, getGridMajorLineOffsetZAdjustment (), getGridMajorLineOffsetSpinButton2 (), "majorLineOffset"),
	                 color (this,
	                        getGridColorButton (),
	                        getGridColorAdjustment (),
	                        getGridColorBox (),
	                        "color"),
	             lineColor (this,
	                        getGridLineColorButton (),
	                        getGridLineColorAdjustment (),
	                        getGridLineColorBox (),
	                        "lineColor"),
	        majorLineColor (this,
	                        getGridMajorLineColorButton (),
	                        getGridMajorLineColorAdjustment (),
	                        getGridMajorLineColorBox (),
	                        "majorLineColor"),
	          snapDistance (this, getGridSnapDistanceAdjustment (), getGridSnapDistanceSpinButton (), "snapDistance"),
	          snapToCenter (this, getGridSnapToCenterCheckButton (), "snapToCenter"),
	              undoStep (),
	              changing (false)
{
	getGridCheckButton () .set_related_action (getBrowserWindow () -> getGridLayoutToolAction ());

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
X3DGridEditor::initialize ()
{
	const auto &      gridTool  = getBrowserWindow () -> getGridTool () -> getTool ();
	const X3D::MFNode gridTools = { gridTool };

	enabled          .setNodes (gridTools);
	translation      .setNodes (gridTools);
	scale            .setNodes (gridTools);
	dimension0       .setNodes (gridTools);
	dimension1       .setNodes (gridTools);
	dimension2       .setNodes (gridTools);
	majorLineEvery0  .setNodes (gridTools);
	majorLineEvery1  .setNodes (gridTools);
	majorLineEvery2  .setNodes (gridTools);
	majorLineOffset0 .setNodes (gridTools);
	majorLineOffset1 .setNodes (gridTools);
	majorLineOffset2 .setNodes (gridTools);
	color            .setNodes (gridTools);
	lineColor        .setNodes (gridTools);
	majorLineColor   .setNodes (gridTools);
	snapDistance     .setNodes (gridTools);
	snapToCenter     .setNodes (gridTools);

	gridTool -> rotation () .addInterest (&X3DGridEditor::set_rotation, this);
	getCurrentScene ()      .addInterest (&X3DGridEditor::set_majorLineEvery, this);

	on_grid_toggled ();
	set_rotation ();
	set_majorLineEvery ();
}

void
X3DGridEditor::configure ()
{
	getGridUniformScaleButton () .set_active (getConfig () -> getBoolean ("gridUniformScale"));
}

void
X3DGridEditor::on_grid_toggled ()
{
	getGridTransformBox ()  .set_sensitive (getGridCheckButton () .get_active ());
	getGridMajorLinesBox () .set_sensitive (getGridCheckButton () .get_active ());
	getGridColorsBox ()     .set_sensitive (getGridCheckButton () .get_active ());
	getGridSnappingBox ()   .set_visible   (getGridCheckButton () .get_active ());
}

void
X3DGridEditor::on_grid_plane_changed ()
{
	if (changing)
		return;

	const auto &      grid  = getBrowserWindow () -> getGridTool () -> getTool ();
	const X3D::MFNode nodes = { grid };

	addUndoFunction <X3D::SFRotation> (nodes, "rotation", undoStep);

	grid -> rotation () .removeInterest (&X3DGridEditor::set_rotation, this);
	grid -> rotation () .addInterest (&X3DGridEditor::connectRotation, this);

	switch (getGridPlaneComboBoxText () .get_active_row_number ())
	{
		case 0:
			grid -> rotation () = X_PLANE_ROTATION;
			break;
		case 1:
			grid -> rotation () = Y_PLANE_ROTATION;
			break;
		case 2:
			grid -> rotation () = Z_PLANE_ROTATION;
			break;
		default:
			break;
	}

	addRedoFunction <X3D::SFRotation> (nodes, "rotation", undoStep);
}

void
X3DGridEditor::set_rotation ()
{
	changing = true;

	constexpr float EPS  = math::radians (0.1);
	const auto &    grid = getBrowserWindow () -> getGridTool () -> getTool ();

	if (std::abs ((grid -> rotation () * ~X_PLANE_ROTATION) .angle ()) < EPS)
		getGridPlaneComboBoxText () .set_active (0);

	else if (std::abs ((grid -> rotation () * ~Y_PLANE_ROTATION) .angle ()) < EPS)
		getGridPlaneComboBoxText () .set_active (1);

	else if (std::abs ((grid -> rotation () * ~Z_PLANE_ROTATION) .angle ()) < EPS)
		getGridPlaneComboBoxText () .set_active (2);

	else
		getGridPlaneComboBoxText () .set_active (-1);

	changing = false;
}

void
X3DGridEditor::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (&X3DGridEditor::connectRotation, this);
	field .addInterest (&X3DGridEditor::set_rotation, this);
}

void
X3DGridEditor::on_grid_uniform_scale_clicked ()
{
	if (getGridUniformScaleButton () .get_active ())
	{
		getGridUniformScaleImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (true);
	}
	else
	{
		getGridUniformScaleImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		scale .setUniform (false);
	}
}

void
X3DGridEditor::on_major_line_grid_value_changed ()
{
	const auto & grid  = getBrowserWindow () -> getGridTool () -> getTool ();
	const size_t index = getGridMajorGridAdjustment () -> get_value () - 1;

	if (grid -> majorLineEvery () .size () < INDICES * index + 3)
		grid -> majorLineEvery () .resize (INDICES * index + 3);

	if (grid -> majorLineOffset () .size () < INDICES * index + 3)
		grid -> majorLineOffset () .resize (INDICES * index + 3);

	majorLineEvery0  .setIndex (INDICES * index + 0);
	majorLineEvery1  .setIndex (INDICES * index + 1);
	majorLineEvery2  .setIndex (INDICES * index + 2);
	majorLineOffset0 .setIndex (INDICES * index + 0);
	majorLineOffset1 .setIndex (INDICES * index + 1);
	majorLineOffset2 .setIndex (INDICES * index + 2);
}

void
X3DGridEditor::on_major_line_grid_upper_changed ()
{
	getGridMajorGridSpinButton ()   .set_sensitive (getGridMajorGridAdjustment () -> get_upper () > 1);
	getGridRemoveMajorGridButton () .set_sensitive (getGridMajorGridAdjustment () -> get_upper () > 0);
}

void
X3DGridEditor::on_add_major_line_grid ()
{
	const int size = getGridMajorGridAdjustment () -> get_upper () + 1;

	getGridMajorGridAdjustment () -> set_lower (1);
	getGridMajorGridAdjustment () -> set_upper (size);
	getGridMajorGridAdjustment () -> set_value (size);
	
	on_major_line_grid_upper_changed ();
}

void
X3DGridEditor::on_remove_major_line_grid ()
{
	const auto & grid  = getBrowserWindow () -> getGridTool () -> getTool ();
	const int    size  = getGridMajorGridAdjustment () -> get_upper () - 1;
	const int    index = (getGridMajorGridAdjustment () -> get_value () - 1) * INDICES;
	const auto   iterL = grid -> majorLineEvery ()  .begin () + index;
	const auto   iterO = grid -> majorLineOffset () .begin () + index;

	grid -> majorLineEvery ()  .erase (iterL, iterL + INDICES);
	grid -> majorLineOffset () .erase (iterO, iterO + INDICES);

	getGridMajorGridAdjustment () -> set_lower (bool (size));
	getGridMajorGridAdjustment () -> set_upper (size);
	
	if (getGridMajorGridAdjustment () -> get_value () > size)
		getGridMajorGridAdjustment () -> set_value (size);
		
	on_major_line_grid_upper_changed ();
}

void
X3DGridEditor::set_majorLineEvery ()
{
	const auto & grid = getBrowserWindow () -> getGridTool () -> getTool ();

	getGridMajorGridAdjustment () -> set_lower (bool (grid -> majorLineEvery () .size ()));
	getGridMajorGridAdjustment () -> set_upper (grid -> majorLineEvery () .size () / INDICES);
	getGridMajorGridAdjustment () -> set_value (grid -> majorLineEvery () .size () > 0);

	on_major_line_grid_upper_changed ();
}

void
X3DGridEditor::store ()
{
	getConfig () -> setItem ("gridUniformScale", getGridUniformScaleButton () .get_active ());
}

X3DGridEditor::~X3DGridEditor ()
{ }

} // puck
} // titania

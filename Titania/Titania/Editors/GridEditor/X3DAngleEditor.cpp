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

#include "X3DAngleEditor.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Tools/Grids/AngleTool.h>

namespace titania {
namespace puck {

static const auto X_PLANE_ROTATION = X3D::Rotation4f (0, 0, -1, M_PI / 2) * X3D::Rotation4f (1, 0, 0, M_PI / 2);
static const auto Y_PLANE_ROTATION = X3D::Rotation4f ();
static const auto Z_PLANE_ROTATION = X3D::Rotation4f (1, 0, 0, M_PI / 2);

static constexpr int INDICES = 3;

X3DAngleEditor::X3DAngleEditor () :
	X3DGridEditorInterface (),
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
	             dimension (this,
	                        getAngleDimension0Adjustment (),
	                        getAngleDimension1Adjustment (),
	                        getAngleDimensionYAdjustment (),
	                        getAngleDimensionBox (),
	                        "dimension"),
	        majorLineEvery (this,
	                        getAngleMajorLineEvery0Adjustment (),
	                        getAngleMajorLineEvery1Adjustment (),
	                        getAngleMajorLineEveryYAdjustment (),
	                        getAngleMajorLineEveryBox (),
	                        "majorLineEvery"),
	       majorLineOffset (this,
	                        getAngleMajorLineOffset0Adjustment (),
	                        getAngleMajorLineOffset1Adjustment (),
	                        getAngleMajorLineOffsetYAdjustment (),
	                        getAngleMajorLineOffsetBox (),
	                        "majorLineOffset"),
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
	              changing (false)
{
	getAngleCheckButton () .set_related_action (getBrowserWindow () -> getAngleToolAction ());

	translation     .setUndo (false);
	scale           .setUndo (false);
	dimension       .setUndo (false);
	majorLineEvery  .setUndo (false);
	majorLineOffset .setUndo (false);
	color           .setUndo (false);
	lineColor       .setUndo (false);
	majorLineColor  .setUndo (false);
}

void
X3DAngleEditor::initialize ()
{
	getAngleUniformScaleButton () .set_active (getConfig () .getBoolean ("angleUniformScale"));

	const auto & angleTool  = getBrowserWindow () -> getAngleTool ();
	X3D::MFNode  angleTools = { angleTool };

	translation     .setNodes (angleTools);
	scale           .setNodes (angleTools);
	dimension       .setNodes (angleTools);
	majorLineEvery  .setNodes (angleTools);
	majorLineOffset .setNodes (angleTools);
	color           .setNodes (angleTools);
	lineColor       .setNodes (angleTools);
	majorLineColor  .setNodes (angleTools);

	angleTool -> rotation () .addInterest (this, &X3DAngleEditor::set_rotation);
	getScene ()              .addInterest (this, &X3DAngleEditor::set_majorLineEvery);

	on_grid_toggled ();
	set_rotation ();
	set_majorLineEvery ();
}

void
X3DAngleEditor::on_angle_toggled ()
{
	getAngleBox () .set_sensitive (getAngleCheckButton () .get_active ());
}

void
X3DAngleEditor::on_angle_plane_changed ()
{
	if (changing)
		return;

	const auto & grid = getBrowserWindow () -> getAngleTool ();

	grid -> rotation () .removeInterest (this, &X3DAngleEditor::set_rotation);
	grid -> rotation () .addInterest (this, &X3DAngleEditor::connectRotation);

	switch (getAnglePlaneComboBoxText () .get_active_row_number ())
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
}

void
X3DAngleEditor::set_rotation ()
{
	changing = true;

	constexpr float EPS  = math::radians (0.1);
	const auto &    grid = getBrowserWindow () -> getAngleTool ();

	if (std::abs ((grid -> rotation () * ~X_PLANE_ROTATION) .angle ()) < EPS)
		getAnglePlaneComboBoxText () .set_active (0);

	else if (std::abs ((grid -> rotation () * ~Y_PLANE_ROTATION) .angle ()) < EPS)
		getAnglePlaneComboBoxText () .set_active (1);

	else if (std::abs ((grid -> rotation () * ~Z_PLANE_ROTATION) .angle ()) < EPS)
		getAnglePlaneComboBoxText () .set_active (2);

	else
		getAnglePlaneComboBoxText () .set_active (-1);

	changing = false;
}

void
X3DAngleEditor::connectRotation (const X3D::SFRotation & field)
{
	field .removeInterest (this, &X3DAngleEditor::connectRotation);
	field .addInterest (this, &X3DAngleEditor::set_rotation);
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
	const int index = getAngleMajorGridAdjustment () -> get_value () - 1;

	majorLineEvery  .setIndex (INDICES * index);
	majorLineOffset .setIndex (INDICES * index);
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
	const int size = getAngleMajorGridAdjustment () -> get_upper () + 1;

	getAngleMajorGridAdjustment () -> set_lower (1);
	getAngleMajorGridAdjustment () -> set_upper (size);
	getAngleMajorGridAdjustment () -> set_value (size);

	on_angle_major_line_grid_upper_changed ();
}

void
X3DAngleEditor::on_angle_remove_major_line_grid ()
{
	const auto & grid  = getBrowserWindow () -> getAngleTool ();
	const int    size  = getAngleMajorGridAdjustment () -> get_upper () - 1;
	const int    index = (getAngleMajorGridAdjustment () -> get_value () - 1) * INDICES;
	const auto   iterL = grid -> majorLineEvery ()  .begin () + index;
	const auto   iterO = grid -> majorLineOffset () .begin () + index;

	grid -> majorLineEvery ()  .erase (iterL, iterL + INDICES);
	grid -> majorLineOffset () .erase (iterO, iterO + INDICES);

	getAngleMajorGridAdjustment () -> set_lower (bool (size));
	getAngleMajorGridAdjustment () -> set_upper (size);

	if (getAngleMajorGridAdjustment () -> get_value () > size)
		getAngleMajorGridAdjustment () -> set_value (size);

	on_angle_major_line_grid_upper_changed ();
}

void
X3DAngleEditor::set_majorLineEvery ()
{
	const auto & grid = getBrowserWindow () -> getAngleTool ();

	getAngleMajorGridAdjustment () -> set_lower (bool (grid -> majorLineEvery () .size ()));
	getAngleMajorGridAdjustment () -> set_upper (grid -> majorLineEvery () .size () / INDICES);
	getAngleMajorGridAdjustment () -> set_value (grid -> majorLineEvery () .size () > 0);

	on_angle_major_line_grid_upper_changed ();
}

X3DAngleEditor::~X3DAngleEditor ()
{
	getConfig () .setItem ("angleUniformScale", getAngleUniformScaleButton () .get_active ());
}

} // puck
} // titania

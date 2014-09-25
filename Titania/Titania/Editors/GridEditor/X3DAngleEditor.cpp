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

X3DAngleEditor::X3DAngleEditor () :
	X3DGridEditorInterface (),
	           translation (getBrowserWindow (),
	                        getAngleTranslationXAdjustment (),
	                        getAngleTranslationYAdjustment (),
	                        getAngleTranslationZAdjustment (),
	                        getAngleTranslationBox (),
	                        "translation"),
	             dimension (getBrowserWindow (),
	                        getAngleDimensionAdjustment (),
	                        getAngleDimensionBox (),
	                        "dimension"),
	               spacing (getBrowserWindow (),
	                        getAngleSpacingAdjustment (),
	                        getAngleSpacingBox (),
	                        "spacing"),
	                 angle (getBrowserWindow (),
	                        getAngleAngleAdjustment (),
	                        getAngleAngleBox (),
	                        "angle"),
	        majorLineEvery (getBrowserWindow (),
	                        getAngleMajorLineEvery0Adjustment (),
	                        getAngleMajorLineEvery1Adjustment (),
	                        getAngleMajorLineEveryBox (),
	                        "majorLineEvery"),
	                 color (getBrowserWindow (),
	                        getAngleColorButton (),
	                        getAngleColorAdjustment (),
	                        getAngleColorBox (),
	                        "color"),
	             lineColor (getBrowserWindow (),
	                        getAngleLineColorButton (),
	                        getAngleLineColorAdjustment (),
	                        getAngleLineColorBox (),
	                        "lineColor"),
	        majorLineColor (getBrowserWindow (),
	                        getAngleMajorLineColorButton (),
	                        getAngleMajorLineColorAdjustment (),
	                        getAngleMajorLineColorBox (),
	                        "majorLineColor"),
	              changing (false)
{
	getAngleCheckButton () .set_related_action (getBrowserWindow () -> getAngleToolAction ());

	translation    .setUndo (false);
	dimension      .setUndo (false);
	spacing        .setUndo (false);
	angle          .setUndo (false);
	majorLineEvery .setUndo (false);
	color          .setUndo (false);
	lineColor      .setUndo (false);
	majorLineColor .setUndo (false);
}

void
X3DAngleEditor::initialize ()
{
	const auto & angleTool  = getBrowserWindow () -> getAngleTool ();
	X3D::MFNode  angleTools = { angleTool };

	translation    .setNodes (angleTools);
	dimension      .setNodes (angleTools);
	spacing        .setNodes (angleTools);
	angle          .setNodes (angleTools);
	majorLineEvery .setNodes (angleTools);
	color          .setNodes (angleTools);
	lineColor      .setNodes (angleTools);
	majorLineColor .setNodes (angleTools);

	angleTool -> rotation () .addInterest (this, &X3DAngleEditor::set_rotation);

	on_grid_toggled ();
	set_rotation ();
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

X3DAngleEditor::~X3DAngleEditor ()
{ }

} // puck
} // titania

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

#include "X3DBillboardEditor.h"

#include <Titania/X3D/Components/Navigation/Billboard.h>

namespace titania {
namespace puck {

X3DBillboardEditor::X3DBillboardEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                     axisOfRotation (this,
	                                     getBillboardAxisOfRotationXAdjustment (),
	                                     getBillboardAxisOfRotationYAdjustment (),
	                                     getBillboardAxisOfRotationZAdjustment (),
	                                     getBillboardAxisOfRotationBox (),
	                                     "axisOfRotation"),
	                 axisOfRotationTool (this, getBillboardAxisOfRotationToolBox (), "axisOfRotation"),
	                          billboard (),
	                           undoStep (),
	                           changing (false)
{
	addChildObjects (billboard);

	axisOfRotation .setNormalize (true);
}

void
X3DBillboardEditor::initialize ()
{ }

void
X3DBillboardEditor::set_selection (const X3D::MFNode & selection)
{
	// Dissconnect.

	if (billboard)
		billboard -> axisOfRotation () .removeInterest (&X3DBillboardEditor::set_axisOfRotation, this);

	// Get Billboard

	billboard = selection .empty () ? nullptr : selection .back ();
	const auto billboards = billboard ? X3D::MFNode ({ billboard }) : X3D::MFNode ();

	getBillboardExpander () .set_visible (billboard);

	axisOfRotation     .setNodes (billboards);
	axisOfRotationTool .setNodes (billboards);

	// Connect.
	
	if (billboard)
	{
		billboard -> axisOfRotation () .addInterest (&X3DBillboardEditor::set_axisOfRotation, this);

		set_axisOfRotation ();
	}
}

void
X3DBillboardEditor::on_axisOfRotation_changed ()
{
	if (changing)
		return;

	addUndoFunction (billboard, billboard -> axisOfRotation (), undoStep);

	billboard -> axisOfRotation () .removeInterest (&X3DBillboardEditor::set_axisOfRotation, this);
	billboard -> axisOfRotation () .addInterest (&X3DBillboardEditor::connectAxisOfRotation, this);

	switch (getBillboardAxisOfRotationComboBoxText () .get_active_row_number ())
	{
		case 0:
		{
			billboard -> axisOfRotation () = X3D::Vector3f (0, 0, 0);
			break;
		}
		case 1:
		{
			billboard -> axisOfRotation () = X3D::Vector3f (1, 0, 0);
			break;
		}
		case 2:
		{
			billboard -> axisOfRotation () = X3D::Vector3f (0, 1, 0);
			break;
		}
		case 3:
		{
			billboard -> axisOfRotation () = X3D::Vector3f (0, 0, 1);
			break;
		}
		default:
			break;
	}

	addRedoFunction (billboard, billboard -> axisOfRotation (), undoStep);
}

void
X3DBillboardEditor::set_axisOfRotation ()
{
	undoStep .reset ();

	changing = true;
	
	const auto axisOfRotation = X3D::normalize <float> (billboard -> axisOfRotation ());

	if (axisOfRotation == X3D::Vector3f (0, 0, 0))
		getBillboardAxisOfRotationComboBoxText () .set_active (0);

	else if (axisOfRotation == X3D::Vector3f (1, 0, 0))
		getBillboardAxisOfRotationComboBoxText () .set_active (1);

	else if (axisOfRotation == X3D::Vector3f (0, 1, 0))
		getBillboardAxisOfRotationComboBoxText () .set_active (2);

	else if (axisOfRotation == X3D::Vector3f (0, 0, 1))
		getBillboardAxisOfRotationComboBoxText () .set_active (3);

	else
		getBillboardAxisOfRotationComboBoxText () .set_active (-1);

	changing = false;
}

void
X3DBillboardEditor::connectAxisOfRotation (const X3D::SFVec3f & field)
{
	field .removeInterest (&X3DBillboardEditor::connectAxisOfRotation, this);
	field .addInterest (&X3DBillboardEditor::set_axisOfRotation, this);
}

X3DBillboardEditor::~X3DBillboardEditor ()
{ }

} // puck
} // titania

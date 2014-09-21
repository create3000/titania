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

#include "X3DGridEditor.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Tools/Grids/GridTool.h>

namespace titania {
namespace puck {

X3DGridEditor::X3DGridEditor () :
	X3DGridEditorInterface (),
	                 color (getBrowserWindow (),
	                        getGridColorButton (),
	                        getGridColorAdjustment (),
	                        getGridColorBox (),
	                        "color")
{ }

void
X3DGridEditor::initialize ()
{ }

void
X3DGridEditor::setup (const X3D::X3DPtr <X3D::GridTool> & grid)
{
	Configuration config (gconf_dir (), "GridEditor");

	setPlane (grid, config .hasItem ("Grid.plane") ? config .getInteger ("Grid.plane") : 1);
}

void
X3DGridEditor::on_grid_plane_changed ()
{
	getConfig () .setItem ("Grid.plane", getGridPlaneComboBoxText () .get_active_row_number ());

	setPlane (getBrowserWindow () -> getGridTool (),
	          getGridPlaneComboBoxText () .get_active_row_number ());
}

void
X3DGridEditor::setPlane (const X3D::X3DPtr <X3D::GridTool> & grid, const int32_t plane)
{
	switch (plane)
	{
		case 0:
			grid -> rotation () = X3D::Rotation4f (0, 0, -1, M_PI / 2);
			break;
		case 1:
			grid -> rotation () = X3D::Rotation4f ();
			break;
		case 2:
			grid -> rotation () = X3D::Rotation4f (1, 0, 0, M_PI / 2);
			break;
		default:
			break;
	}
}

X3DGridEditor::~X3DGridEditor ()
{ }

} // puck
} // titania

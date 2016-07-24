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

#include "X3DBoxEditor.h"

#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

X3DBoxEditor::X3DBoxEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                                size (this,
	                                      getBoxSizeXAdjustment (),
	                                      getBoxSizeYAdjustment (),
	                                      getBoxSizeZAdjustment (),
	                                      getBoxSizeBox (),
	                                      "size")
{ }

void
X3DBoxEditor::addShapes ()
{
	for (const auto & shapeNode : getShapes ())
		shapeNode -> geometry () .addInterest (this, &X3DBoxEditor::set_geometry);

	set_geometry ();
}

void
X3DBoxEditor::removeShapes ()
{
	for (const auto & shapeNode : getShapes ())
		shapeNode -> geometry () .removeInterest (this, &X3DBoxEditor::set_geometry);
}

void
X3DBoxEditor::set_geometry ()
{
	const auto node  = getOneSelection <X3D::Box> (getShapes (), "geometry");
	const auto nodes = node ? X3D::MFNode ({ node }) : X3D::MFNode ();

	getBoxExpander () .set_visible (node);

	size .setNodes (nodes);
}

void
X3DBoxEditor::on_box_uniform_size_clicked ()
{
	if (getBoxUniformSizeButton () .get_active ())
	{
		getBoxUniformSizeImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		size .setUniform (true);
	}
	else
	{
		getBoxUniformSizeImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		size .setUniform (false);
	}
}

X3DBoxEditor::~X3DBoxEditor ()
{
	getConfig () -> setItem ("boxUniformSize", getBoxUniformSizeButton () .get_active ());
}

} // puck
} // titania

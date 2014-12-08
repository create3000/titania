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

#include "X3DLayoutEditor.h"

#include "../../ComposedWidgets/RotationTool.h"

namespace titania {
namespace puck {

X3DLayoutEditor::X3DLayoutEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                              nodes (),
	                       layoutBuffer (),
	                             layout (),
	                           undoStep (),
	                           changing (false),
	                             alignX (this, getLayoutAlignXComboBoxText (), "align", 0, "CENTER"),
	                             alignY (this, getLayoutAlignYComboBoxText (), "align", 1, "CENTER"),
	                       offsetUnitsX (this, getLayoutOffsetUnitsXComboBoxText (), "offsetUnits", 0, "WORLD"),
	                       offsetUnitsY (this, getLayoutOffsetUnitsYComboBoxText (), "offsetUnits", 1, "WORLD"),
	                             offset (this,
	                                     getLayoutOffsetXAdjustment (),
	                                     getLayoutOffsetYAdjustment (),
	                                     getLayoutOffsetBox (),
	                                     "offset"),
	                         sizeUnitsX (this, getLayoutSizeUnitsXComboBoxText (), "sizeUnits", 0, "WORLD"),
	                         sizeUnitsY (this, getLayoutSizeUnitsYComboBoxText (), "sizeUnits", 1, "WORLD"),
	                               size (this,
	                                     getLayoutSizeXAdjustment (),
	                                     getLayoutSizeYAdjustment (),
	                                     getLayoutSizeBox (),
	                                     "size"),
	                         scaleModeX (this, getLayoutScaleModeXComboBoxText (), "scaleMode", 0, "NONE"),
	                         scaleModeY (this, getLayoutScaleModeYComboBoxText (), "scaleMode", 1, "NONE")
{
	addChildren (layoutBuffer);
	layoutBuffer .addInterest (this, &X3DLayoutEditor::set_node);
}

void
X3DLayoutEditor::initialize ()
{
	getLayoutUniformSizeButton () .set_active (getConfig () .getBoolean ("layoutUniformSize"));

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DLayoutEditor::set_layout);

	set_layout ();
}

void
X3DLayoutEditor::on_layout_toggled ()
{
	if (changing)
		return;

	getLayoutCheckButton () .set_inconsistent (false);
	getLayoutBox ()         .set_sensitive (getLayoutCheckButton () .get_active ());

	// Set field.

	addUndoFunction <X3D::SFNode> (nodes, "layout", undoStep);

	for (const auto & node : nodes)
	{
		try
		{
			auto & field = node -> getField <X3D::SFNode> ("layout");

			field .removeInterest (this, &X3DLayoutEditor::set_layout);
			field .addInterest (this, &X3DLayoutEditor::connectLayout);

			if (getLayoutCheckButton () .get_active ())
				getBrowserWindow () -> replaceNode (node, field, X3D::SFNode (layout), undoStep);
			else
				getBrowserWindow () -> replaceNode (node, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (nodes, "layout", undoStep);
}

void
X3DLayoutEditor::set_layout ()
{
	layoutBuffer .addEvent ();
}

void
X3DLayoutEditor::set_node ()
{
	undoStep .reset ();

	// Remove selection

	for (const auto & node : nodes)
	{
		try
		{
			node -> getField <X3D::SFNode> ("layout") .removeInterest (this, &X3DLayoutEditor::set_layout);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	nodes .clear ();

	// Find Layout in selection

	const auto & selection = getBrowserWindow () -> getSelection () -> getChildren ();

	layout = selection .empty () ? nullptr : selection .back ();

	try
	{
		if (not selection .empty ())
		{
			auto & field = selection .back () -> getField <X3D::SFNode> ("layout");
			
			field .addInterest (this, &X3DLayoutEditor::set_layout);

			nodes  = { selection .back () };
			layout = field;

			changing = true;

			getCreateLayoutBox () .set_visible (true);
			getLayoutCheckButton () .set_active (layout);
			getLayoutBox () .set_sensitive (layout);
			
			changing = false;

			if (not layout)
			{
				layout = new X3D::Layout (getExecutionContext ());
				layout -> setup ();
			}
		}
	}
	catch (const X3D::X3DError &)
	{ }

	getLayoutExpander () .set_visible (layout);
	getCreateLayoutBox () .set_visible (not nodes .empty ());

	const X3D::MFNode layouts (layout ? X3D::MFNode ({ layout }) : X3D::MFNode ());

	alignX       .setNodes (layouts);
	alignY       .setNodes (layouts);
	offsetUnitsX .setNodes (layouts);
	offsetUnitsY .setNodes (layouts);
	offset       .setNodes (layouts);
	sizeUnitsX   .setNodes (layouts);
	sizeUnitsY   .setNodes (layouts);
	size         .setNodes (layouts);
	scaleModeX   .setNodes (layouts);
	scaleModeY   .setNodes (layouts);
}

void
X3DLayoutEditor::connectLayout (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DLayoutEditor::connectLayout);
	field .addInterest (this, &X3DLayoutEditor::set_layout);
}

void
X3DLayoutEditor::on_layout_uniform_size_clicked ()
{
	if (getLayoutUniformSizeButton () .get_active ())
	{
		getLayoutUniformSizeImage () .set (Gtk::StockID ("Connected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		size .setUniform (true);
	}
	else
	{
		getLayoutUniformSizeImage () .set (Gtk::StockID ("Disconnected"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
		size .setUniform (false);
	}
}

X3DLayoutEditor::~X3DLayoutEditor ()
{
	getConfig () .setItem ("layoutUniformSize", getLayoutUniformSizeButton () .get_active ());
}

} // puck
} // titania

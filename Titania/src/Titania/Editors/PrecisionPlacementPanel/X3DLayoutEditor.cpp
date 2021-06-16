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

#include "X3DLayoutEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../ComposedWidgets/RotationTool.h"

#include <Titania/X3D/Components/Layout/Layout.h>

namespace titania {
namespace puck {

X3DLayoutEditor::X3DLayoutEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                             alignX (this, getLayoutAlignXComboBoxText (), "align", 0, "CENTER"),
	                             alignY (this, getLayoutAlignYComboBoxText (), "align", 1, "CENTER"),
	                       offsetUnitsX (this, getLayoutOffsetUnitsXComboBoxText (), "offsetUnits", 0, "WORLD"),
	                       offsetUnitsY (this, getLayoutOffsetUnitsYComboBoxText (), "offsetUnits", 1, "WORLD"),
	                            offsetX (this, getLayoutOffsetXAdjustment (), getLayoutOffsetXSpinButton (), "offset"),
	                            offsetY (this, getLayoutOffsetYAdjustment (), getLayoutOffsetYSpinButton (), "offset"),
	                         sizeUnitsX (this, getLayoutSizeUnitsXComboBoxText (), "sizeUnits", 0, "WORLD"),
	                         sizeUnitsY (this, getLayoutSizeUnitsYComboBoxText (), "sizeUnits", 1, "WORLD"),
	                              sizeX (this, getLayoutSizeXAdjustment (), getLayoutSizeXSpinButton (), "size"),
	                              sizeY (this, getLayoutSizeYAdjustment (), getLayoutSizeYSpinButton (), "size"),
	                         scaleModeX (this, getLayoutScaleModeXComboBoxText (), "scaleMode", 0, "NONE"),
	                         scaleModeY (this, getLayoutScaleModeYComboBoxText (), "scaleMode", 1, "NONE"),
	                            parents (),
	                       layoutBuffer (),
	                         layoutNode (),
	                           undoStep (),
	                           changing (false)
{
	addChildObjects (parents, layoutBuffer, layoutNode);

	layoutBuffer .addInterest (&X3DLayoutEditor::set_node, this);

	offsetX .setIndex (0);
	offsetY .setIndex (1);

	sizeX .setIndex (0);
	sizeY .setIndex (1);
}

void
X3DLayoutEditor::initialize ()
{ }

void
X3DLayoutEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::SFNode> ("layout") .removeInterest (&X3DLayoutEditor::set_layout, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	parents = getNodes <X3D::X3DBaseNode> (selection, { X3D::X3DConstants::LayoutLayer, X3D::X3DConstants::LayoutGroup }, false);

	for (const auto & parent : parents)
	{
		try
		{
			parent -> getField <X3D::SFNode> ("layout") .addInterest (&X3DLayoutEditor::set_layout, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_layout ();
}

void
X3DLayoutEditor::on_layout_unlink_clicked ()
{
	X3D::UndoStepPtr undoStep;

	unlinkClone (parents, "layout", undoStep);
}

void
X3DLayoutEditor::on_layout_toggled ()
{
	if (changing)
		return;

	getLayoutCheckButton () .set_inconsistent (false);
	getLayoutBox ()         .set_sensitive (getLayoutCheckButton () .get_active ());

	// Set field.

	const auto executionContext = X3D::MakePtr (getSelectionContext (parents, true));

	addUndoFunction <X3D::SFNode> (parents, "layout", undoStep);

	for (const auto & parent : parents)
	{
		try
		{
			auto & field = parent -> getField <X3D::SFNode> ("layout");

			field .removeInterest (&X3DLayoutEditor::set_layout, this);
			field .addInterest (&X3DLayoutEditor::connectLayout, this);

			if (getLayoutCheckButton () .get_active ())
				X3D::X3DEditor::replaceNode (executionContext, parent, field, layoutNode, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, parent, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (parents, "layout", undoStep);

	getLayoutUnlinkButton () .set_sensitive (getLayoutCheckButton () .get_active () and layoutNode -> getCloneCount () > 1);

	set_widgets ();
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

	// Find Layout in selection

	const auto    executionContext = X3D::MakePtr (getSelectionContext (parents, true));
	auto          tuple            = getSelection <X3D::Layout> (parents, "layout");
	const int32_t active           = std::get <1> (tuple);
	const bool    hasParent        = std::get <2> (tuple);
	const bool    hasField         = (active not_eq -2);

	layoutNode = std::move (std::get <0> (tuple));

	changing = true;

	getLayoutExpander ()     .set_visible (hasParent or layoutNode);
	getCreateLayoutBox ()    .set_sensitive (hasParent);
	getLayoutCheckButton ()  .set_sensitive (hasField);
	getLayoutCheckButton ()  .set_active (active > 0);
	getLayoutCheckButton ()  .set_inconsistent (active < 0);
	getLayoutUnlinkButton () .set_sensitive (active > 0 and layoutNode -> getCloneCount () > 1);
	getLayoutBox ()          .set_sensitive (active > 0);

	changing = false;

	// Widgets

	if (not layoutNode)
		layoutNode = executionContext -> createNode <X3D::Layout> ();

	set_widgets ();
}

void
X3DLayoutEditor::set_widgets ()
{
	const X3D::MFNode layoutNodes ({ layoutNode });

	alignX       .setNodes (layoutNodes);
	alignY       .setNodes (layoutNodes);
	offsetUnitsX .setNodes (layoutNodes);
	offsetUnitsY .setNodes (layoutNodes);
	offsetX      .setNodes (layoutNodes);
	offsetY      .setNodes (layoutNodes);
	sizeUnitsX   .setNodes (layoutNodes);
	sizeUnitsY   .setNodes (layoutNodes);
	sizeX        .setNodes (layoutNodes);
	sizeY        .setNodes (layoutNodes);
	scaleModeX   .setNodes (layoutNodes);
	scaleModeY   .setNodes (layoutNodes);
}

void
X3DLayoutEditor::connectLayout (const X3D::SFNode & field)
{
	field .removeInterest (&X3DLayoutEditor::connectLayout, this);
	field .addInterest (&X3DLayoutEditor::set_layout, this);
}

X3DLayoutEditor::~X3DLayoutEditor ()
{ }

} // puck
} // titania

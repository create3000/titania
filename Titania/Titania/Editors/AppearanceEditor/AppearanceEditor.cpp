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

#include "AppearanceEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Browser/BrowserSelection.h"

#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

AppearanceEditor::AppearanceEditor (X3DBrowserWindow* const browserWindow) :
	            X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DAppearanceEditorInterface (get_ui ("Editors/AppearanceEditor.glade")),
	           X3DMaterialEditor (),
	     X3DFillPropertiesEditor (),
	     X3DLinePropertiesEditor (),
	          X3DBlendModeEditor (),
	      X3DUsedMaterialsEditor (),
	    X3DMaterialPaletteEditor (),
	                  shapeNodes (),
	              appearanceNode (),
	            appearanceBuffer (),
	                    undoStep (),
	                    changing (false)
{
	addChildObjects (shapeNodes,
	                 appearanceNode,
	                 appearanceBuffer);

	appearanceBuffer .addInterest (&AppearanceEditor::set_node, this);
	setup ();
}

void
AppearanceEditor::initialize ()
{
	X3DAppearanceEditorInterface::initialize ();
	X3DMaterialEditor::initialize ();
	X3DFillPropertiesEditor::initialize ();
	X3DLinePropertiesEditor::initialize ();
	X3DBlendModeEditor::initialize ();
	X3DUsedMaterialsEditor::initialize ();
	X3DMaterialPaletteEditor::initialize ();
}

void
AppearanceEditor::configure ()
{
	X3DAppearanceEditorInterface::configure ();
	X3DMaterialPaletteEditor::configure ();

	if (getConfig () -> hasItem ("paned"))
		getPaned () .set_position (getConfig () -> getItem <int32_t> ("paned"));

	getAppearanceChildNotebook () .set_current_page (getConfig () -> getItem <int32_t> ("currentPage"));
}

void
AppearanceEditor::set_selection (const X3D::MFNode & selection)
{
	X3DAppearanceEditorInterface::set_selection (selection);

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> appearance () .removeInterest (&AppearanceEditor::set_appearance, this);

	shapeNodes = getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> appearance () .addInterest (&AppearanceEditor::set_appearance, this);

	set_appearance ();
}

void
AppearanceEditor::on_appearance_unlink_clicked ()
{
	unlinkClone (shapeNodes, "appearance", undoStep);
}

void
AppearanceEditor::on_appearance_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFNode> (shapeNodes, "appearance", undoStep);

	getAppearanceCheckButton () .set_inconsistent (false);

	const auto executionContext = X3D::MakePtr (getSelectionContext (shapeNodes));

	for (const auto & shapeNode : shapeNodes)
	{
		try
		{
			auto & field = shapeNode -> getField <X3D::SFNode> ("appearance");

			field .removeInterest (&AppearanceEditor::set_appearance, this);
			field .addInterest (&AppearanceEditor::connectAppearance, this);

			if (getAppearanceCheckButton () .get_active ())
				X3D::X3DEditor::replaceNode (executionContext, shapeNode, field, appearanceNode, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, shapeNode, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (shapeNodes, "appearance", undoStep);

	getAppearanceUnlinkButton () .set_sensitive (getAppearanceCheckButton () .get_active () and appearanceNode -> getCloneCount () > 1);

	set_selection (getBrowserWindow () -> getSelection () -> getNodes ());
}

void
AppearanceEditor::set_appearance ()
{
	appearanceBuffer .addEvent ();
}

void
AppearanceEditor::set_node ()
{
	undoStep .reset ();

	X3DMaterialEditor::set_appearance ();
	X3DFillPropertiesEditor::set_appearance ();
	X3DLinePropertiesEditor::set_appearance ();
	X3DBlendModeEditor::set_appearance ();

	const auto    executionContext = X3D::MakePtr (getSelectionContext (shapeNodes, true));
	auto          tuple            = getSelection <X3D::X3DAppearanceNode> (shapeNodes, "appearance");
	const int32_t active           = std::get <1> (tuple);
	const bool    hasParent        = std::get <2> (tuple);
	const bool    hasField         = (active not_eq -2);

	appearanceNode = std::move (std::get <0> (tuple));

	if (not appearanceNode)
		appearanceNode = executionContext -> createNode <X3D::Appearance> ();

	changing = true;

	getSelectAppearanceBox ()    .set_sensitive (hasParent and hasField);
	getAppearanceCheckButton ()  .set_active (active > 0);
	getAppearanceCheckButton ()  .set_inconsistent (active < 0);
	getAppearanceUnlinkButton () .set_sensitive (active > 0 and appearanceNode -> getCloneCount () > 1);

	changing = false;
}

void
AppearanceEditor::connectAppearance (const X3D::SFNode & field)
{
	field .removeInterest (&AppearanceEditor::connectAppearance, this);
	field .addInterest (&AppearanceEditor::set_appearance, this);
}

void
AppearanceEditor::store ()
{
	getConfig () -> setItem ("paned",       getPaned () .get_position ());
	getConfig () -> setItem ("currentPage", getAppearanceChildNotebook () .get_current_page ());

	X3DMaterialPaletteEditor::store ();
	X3DAppearanceEditorInterface::store ();
}

AppearanceEditor::~AppearanceEditor ()
{
	dispose ();
}

} // puck
} // titania

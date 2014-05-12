/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "OutlineEditor.h"

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

OutlineEditor::OutlineEditor (BrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DOutlineEditorInterface (get_ui ("OutlineEditor.xml"), gconf_dir ()),
	                 treeview (browserWindow, X3D::X3DExecutionContextPtr (getBrowser () -> getExecutionContext ()))
{ }

void
OutlineEditor::on_map ()
{
	getBrowserWindow () -> getSideBarLabel () .set_text (_ ("Outline Editor"));
}

void
OutlineEditor::initialize ()
{
	X3DOutlineEditorInterface::initialize ();

	getAllowEditingOfExternProtosMenuItem () .set_active (getConfig () .getBoolean ("allowEditingOfExternProtos"));
	getAllowEditingOfInlineNodesMenuItem ()  .set_active (getConfig () .getBoolean ("allowEditingOfInlineNodes"));
	getShowExternProtosMenuItem ()           .set_active (getConfig () .getBoolean ("showExternProtos"));
	getShowPrototypesMenuItem ()             .set_active (getConfig () .getBoolean ("showPrototypes"));
	getShowImportedNodesMenuItem ()          .set_active (getConfig () .getBoolean ("showImportedNodes"));
	getShowExportedNodesMenuItem ()          .set_active (getConfig () .getBoolean ("showExportedNodes"));

	getViewport () .add (treeview);
	treeview .show ();

	// Register browser interest
	getBrowser () -> initialized () .addInterest (this, &OutlineEditor::set_initialized);

	set_initialized ();
}

void
OutlineEditor::set_initialized ()
{
	treeview .set_execution_context (X3D::X3DExecutionContextPtr (getBrowser () -> getExecutionContext ()));
}

bool
OutlineEditor::on_button_press_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 3:
		{
			getPopupMenu () .popup (event -> button, event -> time);
			return true;
		}
		default:
			break;
	}

	return false;
}

// Popup menu

void
OutlineEditor::on_allow_editing_of_extern_protos_toggled ()
{
	getConfig () .setItem ("allowEditingOfExternProtos", getAllowEditingOfExternProtosMenuItem () .get_active ());
	treeview .set_allow_editing_of_extern_protos (getAllowEditingOfExternProtosMenuItem () .get_active ());
	set_initialized ();
}

void
OutlineEditor::on_allow_editing_of_inline_nodes_toggled ()
{
	getConfig () .setItem ("allowEditingOfInlineNodes", getAllowEditingOfInlineNodesMenuItem () .get_active ());
	treeview .set_allow_editing_of_inline_nodes (getAllowEditingOfInlineNodesMenuItem () .get_active ());
	set_initialized ();
}

void
OutlineEditor::on_show_extern_protos_toggled ()
{
	getConfig () .setItem ("showExternProtos", getShowExternProtosMenuItem () .get_active ());
	treeview .set_show_extern_protos (getShowExternProtosMenuItem () .get_active ());
	set_initialized ();
}

void
OutlineEditor::on_show_prototypes_toggled ()
{
	getConfig () .setItem ("showPrototypes", getShowPrototypesMenuItem () .get_active ());
	treeview .set_show_prototypes (getShowPrototypesMenuItem () .get_active ());
	set_initialized ();
}

void
OutlineEditor::on_show_imported_nodes_toggled ()
{
	getConfig () .setItem ("showImportedNodes", getShowImportedNodesMenuItem () .get_active ());
	treeview .set_show_imported_nodes (getShowImportedNodesMenuItem () .get_active ());
	set_initialized ();
}

void
OutlineEditor::on_show_exported_nodes_toggled ()
{
	getConfig () .setItem ("showExportedNodes", getShowExportedNodesMenuItem () .get_active ());
	treeview .set_show_exported_nodes (getShowExportedNodesMenuItem () .get_active ());
	set_initialized ();
}

} // puck
} // titania

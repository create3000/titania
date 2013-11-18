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

#include "NodePropertiesEditor.h"

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

NodePropertiesEditor::NodePropertiesEditor (BrowserWindow* const browserWindow, const X3D::SFNode & node) :
	                X3D::X3DBaseNode (browserWindow -> getBrowser (), browserWindow -> getExecutionContext ()),
	                X3DBaseInterface (browserWindow),
	X3DNodePropertiesEditorInterface (get_ui ("NodePropertiesEditor.ui"), gconf_dir ()),
	                            node (node)
{
	getWindow () .set_transient_for (getBrowserWindow () -> getWindow ());

	getHeaderLabel () .set_text (node -> getTypeName () + " »" + node -> getName () + "«");

	getTypeNameEntry () .set_text (node -> getTypeName ());
	getNameEntry ()     .set_text (node -> getName ());
}

void
NodePropertiesEditor::validateInsertId (Gtk::Entry & entry, const Glib::ustring & insert, int position)
{
	std::string text = entry .get_text () .insert (position, insert);

	if (not X3D::RegEx::Id .FullMatch (text))
		entry .signal_insert_text () .emission_stop ();
}

void
NodePropertiesEditor::validateDeleteId (Gtk::Entry & entry, int start_pos, int end_pos)
{
	std::string text = entry .get_text () .erase (start_pos, end_pos - start_pos);

	if (text .length () and not X3D::RegEx::Id .FullMatch (text))
		entry .signal_delete_text () .emission_stop ();
}

void
NodePropertiesEditor::on_type_name_insert_text (const Glib::ustring & text, int* position)
{
	validateInsertId (getTypeNameEntry (), text, *position);
}

void
NodePropertiesEditor::on_type_name_delete_text (int start_pos, int end_pos)
{
	validateDeleteId (getTypeNameEntry (), start_pos, end_pos);
}

void
NodePropertiesEditor::on_name_insert_text (const Glib::ustring & text, int* position)
{
	validateInsertId (getNameEntry (), text, *position);
}

void
NodePropertiesEditor::on_name_delete_text (int start_pos, int end_pos)
{
	validateDeleteId (getNameEntry (), start_pos, end_pos);
}

void
NodePropertiesEditor::on_ok ()
{
	std::string name = getNameEntry () .get_text ();

	if (name not_eq node -> getName ())
	{
		auto undoStep = std::make_shared <UndoStep> (_ ("Edit Node Properties"));

		undoStep -> addUndoFunction (&NodePropertiesEditor::updateNamedNode,
		                             node -> getName (),
		                             node,
		                             getBrowserWindow ());

		undoStep -> addRedoFunction (&NodePropertiesEditor::updateNamedNode,
		                             name,
		                             node,
		                             getBrowserWindow ());

		updateNamedNode (name, node, getBrowserWindow ());

		getBrowserWindow () -> setEditedWithUndo (true, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
	}

	close ();
}

void
NodePropertiesEditor::on_cancel ()
{
	close ();
}

void
NodePropertiesEditor::updateNamedNode (const std::string & name, const X3D::SFNode & node, BrowserWindow* const browserWindow)
{
	node -> getExecutionContext () -> removeNamedNode (node -> getName ());

	if (not name .empty ())
		node -> getExecutionContext () -> updateNamedNode (node -> getExecutionContext () -> getUniqueName (name), node);	

	browserWindow -> getOutlineTreeView () .queue_draw ();
}

NodePropertiesEditor::~NodePropertiesEditor ()
{
	__LOG__ << std::endl;
}

} // puck

} // titania

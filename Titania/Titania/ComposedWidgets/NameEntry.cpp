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

#include "NameEntry.h"

#include <Titania/X3D/Prototype/ExternProtoDeclaration.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>

namespace titania {
namespace puck {

NameEntry::NameEntry (X3DBaseInterface* const editor,
                      Gtk::Entry & entry,
                      Gtk::Button & button) :
	 X3DBaseInterface (editor -> getBrowserWindow (), editor -> getCurrentBrowser ()),
	X3DComposedWidget (editor),
	            entry (entry),
	           button (button),
	             node (),
	       nameOutput ()
{
	addChildObjects (node, nameOutput);

	entry  .signal_insert_text ()     .connect (sigc::mem_fun (this, &NameEntry::on_insert_text), false);
	entry  .signal_delete_text ()     .connect (sigc::mem_fun (this, &NameEntry::on_delete_text), false);
	entry  .signal_key_press_event () .connect (sigc::mem_fun (this, &NameEntry::on_key_press_event), false);
	button .signal_clicked ()         .connect (sigc::mem_fun (this, &NameEntry::on_clicked));

	entry  .set_sensitive (false);
	button .set_sensitive (false);

	setup ();
}

void
NameEntry::setNode (const X3D::SFNode & value)
{
	if (node)
		node -> name_changed () .removeInterest (&NameEntry::set_name, this);

	node = value;

	if (node)
	{
		node -> name_changed () .addInterest (&NameEntry::set_name, this);
		set_name ();
	}
	else
	{
		entry .set_text ("");
	}

	entry  .set_sensitive (node);
	button .set_sensitive (node);
}

void
NameEntry::on_insert_text (const Glib::ustring & text, int* position)
{
	validateIdOnInsert (entry, text, *position);
}

void
NameEntry::on_delete_text (int start_pos, int end_pos)
{
	validateIdOnDelete (entry, start_pos, end_pos);
}

bool
NameEntry::on_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			on_clicked ();
			return true;
		}
		case GDK_KEY_Escape:
		{
			entry .set_text (node -> getName ());
			button .grab_focus ();
			nameOutput = X3D::SFTime::now ();
			return true;
		}
		default:
			break;
	}

	return false;
}

void
NameEntry::on_clicked ()
{
	try
	{
		if (not node)
			return;
	
		const std::string name = entry .get_text ();
	
		if (name == node -> getName ())
		{
			nameOutput = X3D::SFTime::now ();
			return;
		}
	
		if (node -> isType ({ X3D::X3DConstants::ExternProtoDeclaration }))
		{
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Rename Extern Prototype To »%s«", name .c_str ())));
		
				X3D::X3DEditor::updateExternProtoDeclaration (getCurrentContext (), getCurrentContext () -> getUniqueExternProtoName (name), X3D::ExternProtoDeclarationPtr (node), undoStep);
		
				getBrowserWindow () -> addUndoStep (undoStep);
		}
		else if (node -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
		{
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Rename Prototype To »%s«", name .c_str ())));
	
			X3D::X3DEditor::updateProtoDeclaration (getCurrentContext (), getCurrentContext () -> getUniqueProtoName (name), X3D::ProtoDeclarationPtr (node), undoStep);
	
			getBrowserWindow () -> addUndoStep (undoStep);
		}
		else
		{
			const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Rename %s To »%s«"), node -> getTypeName () .c_str (), name .c_str ()));
	
			if (name .empty ())
				X3D::X3DEditor::removeNamedNode (X3D::X3DExecutionContextPtr (node -> getExecutionContext ()), node, undoStep);
			else
				X3D::X3DEditor::updateNamedNode (X3D::X3DExecutionContextPtr (node -> getExecutionContext ()), name, node, undoStep);
	
			getBrowserWindow () -> addUndoStep (undoStep);
		}

		nameOutput = X3D::SFTime::now ();
	}
	catch (X3D::X3DError & error)
	{
		entry .error_bell ();
	}
}

void
NameEntry::set_name ()
{
	entry .set_text (node -> getName ());
}

} // puck
} // titania

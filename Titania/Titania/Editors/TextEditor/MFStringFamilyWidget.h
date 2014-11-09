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

#ifndef __TITANIA_EDITORS_TEXT_EDITOR_MFSTRING_FAMILY_WIDGET_H__
#define __TITANIA_EDITORS_TEXT_EDITOR_MFSTRING_FAMILY_WIDGET_H__

#include "../../Base/X3DUserInterface.h"
#include "../../ComposedWidgets/MFStringWidget.h"

namespace titania {
namespace puck {

class MFStringFamilyWidget :
	public X3DMFStringWidget
{
public:

	///  @name Construction

	MFStringFamilyWidget (X3DUserInterface* const,
	                      Gtk::TreeView &,
	                      const Glib::RefPtr <Gtk::CellRendererText> &,
	                      Gtk::Button &,
	                      Gtk::Button &,
	                      const Glib::RefPtr <Gtk::TreeViewColumn> &,
	                      const std::string &);


private:

	///  @name Event handlers

	virtual
	void
	on_add_clicked () final override;

	bool
	on_button_release_event (GdkEventButton*);

	bool
	openDialog (const std::string &, std::string &);

	virtual
	void
	set_buffer () final override;

	///  @name Members

	X3DUserInterface* const                  userInterface;
	const Glib::RefPtr <Gtk::TreeViewColumn> familyChooserColumn;
	std::unique_ptr <Gtk::FontChooserDialog> fontChooserDialog;

};

inline
MFStringFamilyWidget::MFStringFamilyWidget (X3DUserInterface* const userInterface,
                                            Gtk::TreeView & treeView,
                                            const Glib::RefPtr <Gtk::CellRendererText> & cellRenderer,
                                            Gtk::Button & addButton,
                                            Gtk::Button & removeButton,
                                            const Glib::RefPtr <Gtk::TreeViewColumn> & familyChooserColumn,
                                            const std::string & name) :
	   X3DBaseInterface (userInterface -> getBrowserWindow (), userInterface -> getBrowser ()),
	  X3DMFStringWidget (userInterface, treeView, cellRenderer, addButton, removeButton, name, "SERIF"),
	      userInterface (userInterface),
	familyChooserColumn (familyChooserColumn),
	  fontChooserDialog ()
{
	treeView .signal_button_release_event () .connect (sigc::mem_fun (*this, &MFStringFamilyWidget::on_button_release_event));
}

inline
void
MFStringFamilyWidget::on_add_clicked ()
{
	std::string family;

	if (openDialog ("SERIF", family))
		append (family);
}

inline
bool
MFStringFamilyWidget::on_button_release_event (GdkEventButton* event)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	if (not path .size ())
		return false;

	if (column not_eq familyChooserColumn .operator -> ())
		return false;

	std::string family;

	if (openDialog (get1Value (path .front ()) .raw (), family))
		set1Value (path .front (), family);

	return true;
}

inline
bool
MFStringFamilyWidget::openDialog (const std::string & defaultValue, std::string & family)
{
	// Choose new URL

	fontChooserDialog .reset (new FontChooserDialog ());

	fontChooserDialog -> set_transient_for (userInterface -> getWindow ());
	fontChooserDialog -> set_modal (true);

	// Create font description.

	Pango::FontDescription fontDescription;

	fontDescription .set_family (defaultValue);

	// Run Dialog

	fontChooserDialog -> set_font_desc (fontDescription);

	const auto response_id = fontChooserDialog -> run ();

	if (response_id == Gtk::RESPONSE_OK)
	{
		const auto fontDescription = fontChooserDialog -> get_font_desc ();
		family = fontDescription .get_family ();
	}

	fontChooserDialog -> hide ();
	fontChooserDialog .reset ();
	return response_id == Gtk::RESPONSE_OK;
}

inline
void
MFStringFamilyWidget::set_buffer ()
{
	if (fontChooserDialog)
		fontChooserDialog -> hide ();

	X3DMFStringWidget::set_buffer ();
}

} // puck
} // titania

#endif

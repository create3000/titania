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

#ifndef __TMP_GLAD2CPP_EXTERNAL_TOOLS_EDITOR_H__
#define __TMP_GLAD2CPP_EXTERNAL_TOOLS_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

#include <gtksourceviewmm.h>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for ExternalToolsEditor.
 */
class X3DExternalToolsEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DExternalToolsEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DExternalToolsEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DExternalToolsEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::TreeStore> &
	getTreeStore () const
	{ return m_TreeStore; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getTreeSelection () const
	{ return m_TreeSelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getNameColumn () const
	{ return m_NameColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getNameRenderer () const
	{ return m_NameRenderer; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::TreeView &
	getTreeView () const
	{ return *m_TreeView; }

	Gtk::Button &
	getAddToolButton () const
	{ return *m_AddToolButton; }

	Gtk::Button &
	getRemoveToolButton () const
	{ return *m_RemoveToolButton; }

	Gtk::Box &
	getToolBox () const
	{ return *m_ToolBox; }

	Gsv::View &
	getSourceView () const
	{ return *m_SourceView; }

	Gtk::Expander &
	getOptionsExpander () const
	{ return *m_OptionsExpander; }

	Gtk::Entry &
	getShortcutKeyEntry () const
	{ return *m_ShortcutKeyEntry; }

	Gtk::ComboBoxText &
	getSaveTypeButton () const
	{ return *m_SaveTypeButton; }

	Gtk::ComboBoxText &
	getOutputTypeButton () const
	{ return *m_OutputTypeButton; }

	Gtk::ComboBoxText &
	getApplicabilityTypeButton () const
	{ return *m_ApplicabilityTypeButton; }

	Gtk::ComboBoxText &
	getInputEncodingButton () const
	{ return *m_InputEncodingButton; }

	Gtk::ComboBoxText &
	getInputTypeButton () const
	{ return *m_InputTypeButton; }

	///  @name Signal handlers

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_row_collapsed (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path) = 0;

	virtual
	void
	on_row_expanded (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path) = 0;

	virtual
	void
	on_tree_selection_changed () = 0;

	virtual
	void
	on_add_tool_clicked () = 0;

	virtual
	void
	on_remove_tool_clicked () = 0;

	virtual
	bool
	on_shortcut_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	bool
	on_shortcut_key_release_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_save_type_changed () = 0;

	virtual
	void
	on_output_type_changed () = 0;

	virtual
	void
	on_applicability_type_changed () = 0;

	virtual
	void
	on_input_format_changed () = 0;

	virtual
	void
	on_input_type_changed () = 0;

	///  @name Destruction

	virtual
	~X3DExternalToolsEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::TreeStore> m_TreeStore;
	Glib::RefPtr <Gtk::TreeSelection> m_TreeSelection;
	Glib::RefPtr <Gtk::TreeViewColumn> m_NameColumn;
	Glib::RefPtr <Gtk::CellRendererText> m_NameRenderer;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::TreeView* m_TreeView;
	Gtk::Button* m_AddToolButton;
	Gtk::Button* m_RemoveToolButton;
	Gtk::Box* m_ToolBox;
	Gsv::View* m_SourceView;
	Gtk::Expander* m_OptionsExpander;
	Gtk::Entry* m_ShortcutKeyEntry;
	Gtk::ComboBoxText* m_SaveTypeButton;
	Gtk::ComboBoxText* m_OutputTypeButton;
	Gtk::ComboBoxText* m_ApplicabilityTypeButton;
	Gtk::ComboBoxText* m_InputEncodingButton;
	Gtk::ComboBoxText* m_InputTypeButton;

};

} // puck
} // titania

#endif

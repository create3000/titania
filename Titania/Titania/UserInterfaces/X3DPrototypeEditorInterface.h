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
#ifndef __TMP_GLAD2CPP_PROTOTYPE_EDITOR_H__
#define __TMP_GLAD2CPP_PROTOTYPE_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DPrototypeEditorInterface :
	public X3DEditorInterface
{
public:

	X3DPrototypeEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DPrototypeEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::CellRendererText> &
	getURLCellRendererText () const
	{ return m_URLCellRendererText; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getURLChooserColumn () const
	{ return m_URLChooserColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getURLCellrendererPixbuf () const
	{ return m_URLCellrendererPixbuf; }

	Gtk::Menu &
	getPrototypeMenu () const
	{ return *m_PrototypeMenu; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Image &
	getEditPrototypeImage () const
	{ return *m_EditPrototypeImage; }

	Gtk::Label &
	getEditLabel () const
	{ return *m_EditLabel; }

	Gtk::Label &
	getHeaderLabel () const
	{ return *m_HeaderLabel; }

	Gtk::Button &
	getNewPrototypeButton () const
	{ return *m_NewPrototypeButton; }

	Gtk::Button &
	getCreateInstanceButton () const
	{ return *m_CreateInstanceButton; }

	Gtk::MenuButton &
	getMenuButton () const
	{ return *m_MenuButton; }

	Gtk::Image &
	getPrototypeImage () const
	{ return *m_PrototypeImage; }

	Gtk::Label &
	getPrototypeLabel () const
	{ return *m_PrototypeLabel; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Notebook &
	getPrototypeNotebook () const
	{ return *m_PrototypeNotebook; }

	Gtk::Expander &
	getInterfaceExpander () const
	{ return *m_InterfaceExpander; }

	Gtk::Box &
	getInterfaceBox () const
	{ return *m_InterfaceBox; }

	Gtk::Box &
	getURLBox () const
	{ return *m_URLBox; }

	Gtk::TreeView &
	getURLTreeView () const
	{ return *m_URLTreeView; }

	Gtk::Button &
	getURLAddButton () const
	{ return *m_URLAddButton; }

	Gtk::Button &
	getURLRemoveButton () const
	{ return *m_URLRemoveButton; }

	Gtk::Box &
	getInstancesBox () const
	{ return *m_InstancesBox; }

	virtual
	void
	on_create_instance_clicked () = 0;

	virtual
	void
	on_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	bool
	on_name_key_press_event (GdkEventKey* event) = 0;

	virtual
	void
	on_rename_clicked () = 0;

	virtual
	~X3DPrototypeEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::CellRendererText>   m_URLCellRendererText;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_URLChooserColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_URLCellrendererPixbuf;
	Gtk::Menu*                             m_PrototypeMenu;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::Image*                            m_EditPrototypeImage;
	Gtk::Label*                            m_EditLabel;
	Gtk::Label*                            m_HeaderLabel;
	Gtk::Button*                           m_NewPrototypeButton;
	Gtk::Button*                           m_CreateInstanceButton;
	Gtk::MenuButton*                       m_MenuButton;
	Gtk::Image*                            m_PrototypeImage;
	Gtk::Label*                            m_PrototypeLabel;
	Gtk::Box*                              m_NameBox;
	Gtk::Entry*                            m_NameEntry;
	Gtk::Button*                           m_RenameButton;
	Gtk::Notebook*                         m_PrototypeNotebook;
	Gtk::Expander*                         m_InterfaceExpander;
	Gtk::Box*                              m_InterfaceBox;
	Gtk::Box*                              m_URLBox;
	Gtk::TreeView*                         m_URLTreeView;
	Gtk::Button*                           m_URLAddButton;
	Gtk::Button*                           m_URLRemoveButton;
	Gtk::Box*                              m_InstancesBox;

};

} // puck
} // titania

#endif

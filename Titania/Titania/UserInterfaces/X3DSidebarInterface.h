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
#ifndef __TMP_GLAD2CPP_SIDEBAR_H__
#define __TMP_GLAD2CPP_SIDEBAR_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DSidebarInterface :
	public X3DUserInterface
{
public:

	X3DSidebarInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DSidebarInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		        filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	virtual
	const std::string &
	getWidgetName () const
	{ return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Label &
	getLabel () const
	{ return *m_Label; }

	Gtk::Notebook &
	getNotebook () const
	{ return *m_Notebook; }

	Gtk::Box &
	getViewpointListBox () const
	{ return *m_ViewpointListBox; }

	Gtk::Box &
	getHistoryViewBox () const
	{ return *m_HistoryViewBox; }

	Gtk::Box &
	getLibraryViewBox () const
	{ return *m_LibraryViewBox; }

	Gtk::Box &
	getOutlineEditorBox () const
	{ return *m_OutlineEditorBox; }

	Gtk::Box &
	getNodeEditorBox () const
	{ return *m_NodeEditorBox; }

	virtual
	void
	on_switch_page (Gtk::Widget* page, guint page_num) = 0;

	virtual
	~X3DSidebarInterface ();


private:

	virtual
	void
	construct () final override
	{ X3DUserInterface::construct (); }

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                   filename;
	Glib::RefPtr <Gtk::Builder>   m_builder;
	std::deque <sigc::connection> m_connections;
	Gtk::Window*                  m_Window;
	Gtk::Box*                     m_Widget;
	Gtk::Label*                   m_Label;
	Gtk::Notebook*                m_Notebook;
	Gtk::Box*                     m_ViewpointListBox;
	Gtk::Box*                     m_HistoryViewBox;
	Gtk::Box*                     m_LibraryViewBox;
	Gtk::Box*                     m_OutlineEditorBox;
	Gtk::Box*                     m_NodeEditorBox;

};

} // puck
} // titania

#endif

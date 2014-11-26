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
#ifndef __TMP_GLAD2CPP_ANIMATION_EDITOR_H__
#define __TMP_GLAD2CPP_ANIMATION_EDITOR_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DAnimationEditorInterface :
	public X3DUserInterface
{
public:

	X3DAnimationEditorInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DAnimationEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		        filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const Glib::ustring & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	void
	updateWidgets (const std::vector <Glib::ustring> & names) const
	{ getBuilder () -> add_from_file (filename, names); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getFrameAdjustment () const
	{ return m_FrameAdjustment; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getTextBuffer () const
	{ return m_TextBuffer; }

	const Glib::RefPtr <Gtk::TreeStore> &
	getTreeStore () const
	{ return m_TreeStore; }

	const Glib::RefPtr <Gtk::TreeModelFilter> &
	getTreeModelFilter () const
	{ return m_TreeModelFilter; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::ToolButton &
	getNewButton () const
	{ return *m_NewButton; }

	Gtk::ToolButton &
	getOpenButton () const
	{ return *m_OpenButton; }

	Gtk::ToolButton &
	getAddObjectButton () const
	{ return *m_AddObjectButton; }

	Gtk::ToolButton &
	getCutButton () const
	{ return *m_CutButton; }

	Gtk::ToolButton &
	getCopyButton () const
	{ return *m_CopyButton; }

	Gtk::ToolButton &
	getPasteButton () const
	{ return *m_PasteButton; }

	Gtk::ToolButton &
	getPlayButton () const
	{ return *m_PlayButton; }

	Gtk::SpinButton &
	getFrameSpinButton () const
	{ return *m_FrameSpinButton; }

	Gtk::TreeView &
	getTreeView () const
	{ return *m_TreeView; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::DrawingArea &
	getDrawingArea () const
	{ return *m_DrawingArea; }

	Gtk::Dialog &
	getNewDialog () const
	{ return *m_NewDialog; }

	Gtk::Button &
	getNewCancelButton () const
	{ return *m_NewCancelButton; }

	Gtk::Button &
	getNewOkButton () const
	{ return *m_NewOkButton; }

	Gtk::Entry &
	getNewNameEntry () const
	{ return *m_NewNameEntry; }

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_new () = 0;

	virtual
	void
	on_open () = 0;

	virtual
	void
	on_add_object () = 0;

	virtual
	void
	on_new_name_changed () = 0;

	virtual
	void
	on_new_name_delete_text (int start_pos, int end_pos) = 0;

	virtual
	void
	on_new_name_insert_text (const Glib::ustring & text, int* position) = 0;

	virtual
	~X3DAnimationEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                         filename;
	Glib::RefPtr <Gtk::Builder>         m_builder;
	Glib::RefPtr <Gtk::Adjustment>      m_FrameAdjustment;
	Glib::RefPtr <Gtk::TextBuffer>      m_TextBuffer;
	Glib::RefPtr <Gtk::TreeStore>       m_TreeStore;
	Glib::RefPtr <Gtk::TreeModelFilter> m_TreeModelFilter;
	Gtk::Window*                        m_Window;
	Gtk::Box*                           m_Widget;
	Gtk::ToolButton*                    m_NewButton;
	Gtk::ToolButton*                    m_OpenButton;
	Gtk::ToolButton*                    m_AddObjectButton;
	Gtk::ToolButton*                    m_CutButton;
	Gtk::ToolButton*                    m_CopyButton;
	Gtk::ToolButton*                    m_PasteButton;
	Gtk::ToolButton*                    m_PlayButton;
	Gtk::SpinButton*                    m_FrameSpinButton;
	Gtk::TreeView*                      m_TreeView;
	Gtk::Box*                           m_NameBox;
	Gtk::Entry*                         m_NameEntry;
	Gtk::Button*                        m_RenameButton;
	Gtk::DrawingArea*                   m_DrawingArea;
	Gtk::Dialog*                        m_NewDialog;
	Gtk::Button*                        m_NewCancelButton;
	Gtk::Button*                        m_NewOkButton;
	Gtk::Entry*                         m_NewNameEntry;

};

} // puck
} // titania

#endif

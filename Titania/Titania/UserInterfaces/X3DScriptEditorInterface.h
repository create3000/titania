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
#ifndef __TMP_GLAD2CPP_SCRIPT_EDITOR_H__
#define __TMP_GLAD2CPP_SCRIPT_EDITOR_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DScriptEditorInterface :
	public X3DUserInterface
{
public:

	X3DScriptEditorInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DScriptEditorInterface (const std::string & filename, const Arguments & ... arguments) :
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

	const Glib::RefPtr <Gtk::AccelGroup> &
	getAccelGroup () const
	{ return m_AccelGroup; }

	Gtk::Image &
	getFragmentShaderImage () const
	{ return *m_FragmentShaderImage; }

	Gtk::Image &
	getVertexShaderImage () const
	{ return *m_VertexShaderImage; }

	Gtk::Menu &
	getShaderTypeMenu () const
	{ return *m_ShaderTypeMenu; }

	Gtk::ImageMenuItem &
	getVertexMenuItem () const
	{ return *m_VertexMenuItem; }

	Gtk::ImageMenuItem &
	getFragmentMenuItem () const
	{ return *m_FragmentMenuItem; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Paned &
	getPaned () const
	{ return *m_Paned; }

	Gtk::Paned &
	getSidePaned () const
	{ return *m_SidePaned; }

	Gtk::Box &
	getNodeIndexBox () const
	{ return *m_NodeIndexBox; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Box &
	getConsoleBox () const
	{ return *m_ConsoleBox; }

	Gtk::Box &
	getScriptEditor () const
	{ return *m_ScriptEditor; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::ToolButton &
	getSaveButton () const
	{ return *m_SaveButton; }

	Gtk::ToolButton &
	getUndoButton () const
	{ return *m_UndoButton; }

	Gtk::ToolButton &
	getRedoButton () const
	{ return *m_RedoButton; }

	Gtk::ToolButton &
	getLoadStateButton () const
	{ return *m_LoadStateButton; }

	Gtk::ToggleToolButton &
	getDirectOutputToggleButton () const
	{ return *m_DirectOutputToggleButton; }

	Gtk::ToggleToolButton &
	getMustEvaluateToggleButton () const
	{ return *m_MustEvaluateToggleButton; }

	Gtk::MenuToolButton &
	getShaderTypeMenuButton () const
	{ return *m_ShaderTypeMenuButton; }

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_apply_clicked () = 0;

	virtual
	void
	on_undo_clicked () = 0;

	virtual
	void
	on_redo_clicked () = 0;

	virtual
	void
	on_loadState_clicked () = 0;

	virtual
	~X3DScriptEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::AccelGroup> m_AccelGroup;
	Gtk::Image*                    m_FragmentShaderImage;
	Gtk::Image*                    m_VertexShaderImage;
	Gtk::Menu*                     m_ShaderTypeMenu;
	Gtk::ImageMenuItem*            m_VertexMenuItem;
	Gtk::ImageMenuItem*            m_FragmentMenuItem;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Paned*                    m_Paned;
	Gtk::Paned*                    m_SidePaned;
	Gtk::Box*                      m_NodeIndexBox;
	Gtk::Box*                      m_NameBox;
	Gtk::Entry*                    m_NameEntry;
	Gtk::Button*                   m_RenameButton;
	Gtk::Box*                      m_ConsoleBox;
	Gtk::Box*                      m_ScriptEditor;
	Gtk::ScrolledWindow*           m_ScrolledWindow;
	Gtk::ToolButton*               m_SaveButton;
	Gtk::ToolButton*               m_UndoButton;
	Gtk::ToolButton*               m_RedoButton;
	Gtk::ToolButton*               m_LoadStateButton;
	Gtk::ToggleToolButton*         m_DirectOutputToggleButton;
	Gtk::ToggleToolButton*         m_MustEvaluateToggleButton;
	Gtk::MenuToolButton*           m_ShaderTypeMenuButton;

};

} // puck
} // titania

#endif

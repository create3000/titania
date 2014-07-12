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
#ifndef __TMP_GLAD2CPP_TEXT_EDITOR_H__
#define __TMP_GLAD2CPP_TEXT_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DTextEditorInterface :
	public X3DEditorInterface
{
public:

	template <class ... Arguments>
	X3DTextEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const std::string & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		widget -> set_name (name);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getMaxExtentAdjustment () const
	{ return m_MaxExtentAdjustment; }

	const Glib::RefPtr <Gtk::TextBuffer> &
	getStringTextBuffer () const
	{ return m_StringTextBuffer; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::CheckButton &
	getTextCheckButton () const
	{ return *m_TextCheckButton; }

	Gtk::Grid &
	getTextBox () const
	{ return *m_TextBox; }

	Gtk::SpinButton &
	getMaxExtentSpinButton () const
	{ return *m_MaxExtentSpinButton; }

	Gtk::TextView &
	getStringTextView () const
	{ return *m_StringTextView; }

	Gtk::Box &
	getLenghtBox () const
	{ return *m_LenghtBox; }

	virtual
	void
	on_string_changed () = 0;

	virtual
	void
	on_text_toggled () = 0;

	virtual
	void
	on_maxExtent_changed () = 0;

	virtual
	~X3DTextEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_MaxExtentAdjustment;
	Glib::RefPtr <Gtk::TextBuffer> m_StringTextBuffer;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::CheckButton*              m_TextCheckButton;
	Gtk::Grid*                     m_TextBox;
	Gtk::SpinButton*               m_MaxExtentSpinButton;
	Gtk::TextView*                 m_StringTextView;
	Gtk::Box*                      m_LenghtBox;

};

} // puck
} // titania

#endif

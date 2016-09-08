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
#ifndef __TMP_GLAD2CPP_FOG_EDITOR_H__
#define __TMP_GLAD2CPP_FOG_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for FogEditor.
 */
class X3DFogEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DFogEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DFogEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	///  @name Member access

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

	const Glib::RefPtr <Gtk::Adjustment> &
	getColorAdjustment () const
	{ return m_ColorAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getFogTypeListStore () const
	{ return m_FogTypeListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getVisibilityRangeAdjustment () const
	{ return m_VisibilityRangeAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getFogListBox () const
	{ return *m_FogListBox; }

	Gtk::Box &
	getFogActionBox () const
	{ return *m_FogActionBox; }

	Gtk::Button &
	getNewFogButton () const
	{ return *m_NewFogButton; }

	Gtk::Button &
	getRemoveFogButton () const
	{ return *m_RemoveFogButton; }

	Gtk::Expander &
	getFogExpander () const
	{ return *m_FogExpander; }

	Gtk::Grid &
	getFogBox () const
	{ return *m_FogBox; }

	Gtk::SpinButton &
	getVisibilityRangeSpinButton () const
	{ return *m_VisibilityRangeSpinButton; }

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
	getColorBox () const
	{ return *m_ColorBox; }

	Gtk::Button &
	getColorButton () const
	{ return *m_ColorButton; }

	Gtk::ComboBoxText &
	getFogTypeComboBoxText () const
	{ return *m_FogTypeComboBoxText; }

	///  @name Signal handlers

	virtual
	void
	on_new_fog_clicked () = 0;

	virtual
	void
	on_remove_fog_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DFogEditorInterface ();


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	///  @name Static members

	static const std::string m_widgetName;

	///  @name Members

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_ColorAdjustment;
	Glib::RefPtr <Gtk::ListStore>  m_FogTypeListStore;
	Glib::RefPtr <Gtk::Adjustment> m_VisibilityRangeAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Box*                      m_FogListBox;
	Gtk::Box*                      m_FogActionBox;
	Gtk::Button*                   m_NewFogButton;
	Gtk::Button*                   m_RemoveFogButton;
	Gtk::Expander*                 m_FogExpander;
	Gtk::Grid*                     m_FogBox;
	Gtk::SpinButton*               m_VisibilityRangeSpinButton;
	Gtk::Box*                      m_NameBox;
	Gtk::Entry*                    m_NameEntry;
	Gtk::Button*                   m_RenameButton;
	Gtk::Box*                      m_ColorBox;
	Gtk::Button*                   m_ColorButton;
	Gtk::ComboBoxText*             m_FogTypeComboBoxText;

};

} // puck
} // titania

#endif

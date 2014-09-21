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
#ifndef __TMP_GLAD2CPP_GRID_EDITOR_H__
#define __TMP_GLAD2CPP_GRID_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DGridEditorInterface :
	public X3DEditorInterface
{
public:

	X3DGridEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DGridEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
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
	getGridColorAdjustment () const
	{ return m_GridColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTranslationXAdjustment () const
	{ return m_GridTranslationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTranslationYAdjustment () const
	{ return m_GridTranslationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTranslationZAdjustment () const
	{ return m_GridTranslationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridTransparencyAdjustment () const
	{ return m_GridTransparencyAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridXDimensionAdjustment () const
	{ return m_GridXDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridXSpacingAdjustment () const
	{ return m_GridXSpacingAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridZDimensionAdjustment () const
	{ return m_GridZDimensionAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGridZSpacingAdjustment () const
	{ return m_GridZSpacingAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Expander &
	getGridExpander () const
	{ return *m_GridExpander; }

	Gtk::Grid &
	getInlineBox () const
	{ return *m_InlineBox; }

	Gtk::ComboBoxText &
	getGridPlaneComboBoxText () const
	{ return *m_GridPlaneComboBoxText; }

	Gtk::Box &
	getGridColorBox () const
	{ return *m_GridColorBox; }

	Gtk::Button &
	getGridColorButton () const
	{ return *m_GridColorButton; }

	Gtk::Box &
	getGridTransparencyBox () const
	{ return *m_GridTransparencyBox; }

	Gtk::Box &
	getGridTranslationBox () const
	{ return *m_GridTranslationBox; }

	Gtk::SpinButton &
	getGridXDimensionSpinButton () const
	{ return *m_GridXDimensionSpinButton; }

	Gtk::SpinButton &
	getGridZDimensionSpinButton () const
	{ return *m_GridZDimensionSpinButton; }

	Gtk::SpinButton &
	getGridXSpacingSpinButton () const
	{ return *m_GridXSpacingSpinButton; }

	Gtk::SpinButton &
	getGridZSpacingSpinButton () const
	{ return *m_GridZSpacingSpinButton; }

	virtual
	void
	on_grid_plane_changed () = 0;

	virtual
	~X3DGridEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_GridColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTranslationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTranslationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTranslationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridTransparencyAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridXDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridXSpacingAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridZDimensionAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GridZSpacingAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Expander*                 m_GridExpander;
	Gtk::Grid*                     m_InlineBox;
	Gtk::ComboBoxText*             m_GridPlaneComboBoxText;
	Gtk::Box*                      m_GridColorBox;
	Gtk::Button*                   m_GridColorButton;
	Gtk::Box*                      m_GridTransparencyBox;
	Gtk::Box*                      m_GridTranslationBox;
	Gtk::SpinButton*               m_GridXDimensionSpinButton;
	Gtk::SpinButton*               m_GridZDimensionSpinButton;
	Gtk::SpinButton*               m_GridXSpacingSpinButton;
	Gtk::SpinButton*               m_GridZSpacingSpinButton;

};

} // puck
} // titania

#endif

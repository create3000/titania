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
#ifndef __TMP_GLAD2CPP_BACKGROUND_EDITOR_H__
#define __TMP_GLAD2CPP_BACKGROUND_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class X3DBackgroundEditorInterface :
	public X3DEditorInterface
{
public:

	X3DBackgroundEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DBackgroundEditorInterface (const std::string & filename, const Arguments & ... arguments) :
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

	const Glib::RefPtr <Gtk::Adjustment> &
	getGroundAngleAdjustment () const
	{ return m_GroundAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getGroundColorAdjustment () const
	{ return m_GroundColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSkyAngleAdjustment () const
	{ return m_SkyAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSkyColorAdjustment () const
	{ return m_SkyColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransparencyAdjustment () const
	{ return m_TransparencyAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getColorBox () const
	{ return *m_ColorBox; }

	Gtk::Expander &
	getColorsExpander () const
	{ return *m_ColorsExpander; }

	Gtk::Box &
	getSkyGradientBox () const
	{ return *m_SkyGradientBox; }

	Gtk::Grid &
	getSkyColorGrid () const
	{ return *m_SkyColorGrid; }

	Gtk::ScrolledWindow &
	getSkyColorsScrolledWindow () const
	{ return *m_SkyColorsScrolledWindow; }

	Gtk::Button &
	getAddSkyColorButton () const
	{ return *m_AddSkyColorButton; }

	Gtk::Button &
	getRemoveSkyColorButton () const
	{ return *m_RemoveSkyColorButton; }

	Gtk::Box &
	getSkyColorBox () const
	{ return *m_SkyColorBox; }

	Gtk::Button &
	getSkyColorButton () const
	{ return *m_SkyColorButton; }

	Gtk::Box &
	getGroundGradientBox () const
	{ return *m_GroundGradientBox; }

	Gtk::Grid &
	getGroundColorGrid () const
	{ return *m_GroundColorGrid; }

	Gtk::ScrolledWindow &
	getGroundColorsScrolledWindow () const
	{ return *m_GroundColorsScrolledWindow; }

	Gtk::Button &
	getAddGroundColorButton () const
	{ return *m_AddGroundColorButton; }

	Gtk::Button &
	getRemoveGroundColorButton () const
	{ return *m_RemoveGroundColorButton; }

	Gtk::Box &
	getGroundColorBox () const
	{ return *m_GroundColorBox; }

	Gtk::Button &
	getGroundColorButton () const
	{ return *m_GroundColorButton; }

	Gtk::Scale &
	getTransparencyScale () const
	{ return *m_TransparencyScale; }

	virtual
	~X3DBackgroundEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_GroundAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_GroundColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SkyAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SkyColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransparencyAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Box*                      m_ColorBox;
	Gtk::Expander*                 m_ColorsExpander;
	Gtk::Box*                      m_SkyGradientBox;
	Gtk::Grid*                     m_SkyColorGrid;
	Gtk::ScrolledWindow*           m_SkyColorsScrolledWindow;
	Gtk::Button*                   m_AddSkyColorButton;
	Gtk::Button*                   m_RemoveSkyColorButton;
	Gtk::Box*                      m_SkyColorBox;
	Gtk::Button*                   m_SkyColorButton;
	Gtk::Box*                      m_GroundGradientBox;
	Gtk::Grid*                     m_GroundColorGrid;
	Gtk::ScrolledWindow*           m_GroundColorsScrolledWindow;
	Gtk::Button*                   m_AddGroundColorButton;
	Gtk::Button*                   m_RemoveGroundColorButton;
	Gtk::Box*                      m_GroundColorBox;
	Gtk::Button*                   m_GroundColorButton;
	Gtk::Scale*                    m_TransparencyScale;

};

} // puck
} // titania

#endif

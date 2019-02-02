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

#ifndef __TMP_GLAD2CPP_NOTEBOOK_PAGE_H__
#define __TMP_GLAD2CPP_NOTEBOOK_PAGE_H__

#include "../Base/X3DUserInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for NotebookPage.
 */
class X3DNotebookPageInterface :
	public X3DUserInterface
{
public:

	///  @name Construction

	X3DNotebookPageInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DNotebookPageInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DNotebookPageInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFogDepthAdjustment () const
	{ return m_FogDepthAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Viewport &
	getBox1 () const
	{ return *m_Box1; }

	Gtk::Viewport &
	getBox2 () const
	{ return *m_Box2; }

	Gtk::Viewport &
	getBox3 () const
	{ return *m_Box3; }

	Gtk::Viewport &
	getBox4 () const
	{ return *m_Box4; }

	Gtk::Toolbar &
	getGeometryToolbar () const
	{ return *m_GeometryToolbar; }

	Gtk::CheckButton &
	getFogCoordCheckButton () const
	{ return *m_FogCoordCheckButton; }

	Gtk::Box &
	getFogDepthBox () const
	{ return *m_FogDepthBox; }

	Gtk::SpinButton &
	getFogDepthSpinButton () const
	{ return *m_FogDepthSpinButton; }

	Gtk::Box &
	getTabWidget () const
	{ return *m_TabWidget; }

	Gtk::Image &
	getTabImage () const
	{ return *m_TabImage; }

	Gtk::Label &
	getTabLabel () const
	{ return *m_TabLabel; }

	Gtk::ToggleButton &
	getMuteButton () const
	{ return *m_MuteButton; }

	Gtk::Image &
	getMuteImage () const
	{ return *m_MuteImage; }

	Gtk::Button &
	getTabCloseButton () const
	{ return *m_TabCloseButton; }

	///  @name Signal handlers

	virtual
	bool
	on_key_release_event (GdkEventKey* key_event) = 0;

	virtual
	bool
	on_tab_image_query_tooltip (int x, int y, bool keyboard_mode, const Glib::RefPtr <Gtk::Tooltip> & tooltip) = 0;

	virtual
	void
	on_mute_toggled () = 0;

	///  @name Destruction

	virtual
	~X3DNotebookPageInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_FogDepthAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::Viewport* m_Box1;
	Gtk::Viewport* m_Box2;
	Gtk::Viewport* m_Box3;
	Gtk::Viewport* m_Box4;
	Gtk::Toolbar* m_GeometryToolbar;
	Gtk::CheckButton* m_FogCoordCheckButton;
	Gtk::Box* m_FogDepthBox;
	Gtk::SpinButton* m_FogDepthSpinButton;
	Gtk::Box* m_TabWidget;
	Gtk::Image* m_TabImage;
	Gtk::Label* m_TabLabel;
	Gtk::ToggleButton* m_MuteButton;
	Gtk::Image* m_MuteImage;
	Gtk::Button* m_TabCloseButton;

};

} // puck
} // titania

#endif

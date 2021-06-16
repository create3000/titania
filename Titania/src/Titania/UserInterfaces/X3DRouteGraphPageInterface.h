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

#ifndef __TMP_GLAD2CPP_ROUTE_GRAPH_PAGE_H__
#define __TMP_GLAD2CPP_ROUTE_GRAPH_PAGE_H__

#include "../Base/X3DUserInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for RouteGraphPage.
 */
class X3DRouteGraphPageInterface :
	public X3DUserInterface
{
public:

	///  @name Construction

	X3DRouteGraphPageInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DRouteGraphPageInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DRouteGraphPageInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DUserInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getHAdjustment () const
	{ return m_HAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getVAdjustment () const
	{ return m_VAdjustment; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::Overlay &
	getOverlay () const
	{ return *m_Overlay; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::Viewport &
	getViewport () const
	{ return *m_Viewport; }

	Gtk::Fixed &
	getFixed () const
	{ return *m_Fixed; }

	Gtk::Label &
	getPageNameLabel () const
	{ return *m_PageNameLabel; }

	Gtk::Popover &
	getRenamePagePopover () const
	{ return *m_RenamePagePopover; }

	Gtk::Box &
	getPageNameBox () const
	{ return *m_PageNameBox; }

	Gtk::Entry &
	getPageNameEntry () const
	{ return *m_PageNameEntry; }

	Gtk::Button &
	getRenamePageButton () const
	{ return *m_RenamePageButton; }

	///  @name Signal handlers

	virtual
	bool
	on_button_press_event (GdkEventButton* button_event) = 0;

	virtual
	bool
	on_button_release_event (GdkEventButton* release_event) = 0;

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	bool
	on_motion_notify_event (GdkEventMotion* motion_event) = 0;

	virtual
	bool
	on_draw (const ::Cairo::RefPtr < ::Cairo::Context> & cr) = 0;

	virtual
	void
	on_page_name_changed () = 0;

	virtual
	bool
	on_page_name_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_page_name_rename_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DRouteGraphPageInterface () override;


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
	Glib::RefPtr <Gtk::Adjustment> m_HAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_VAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::Overlay* m_Overlay;
	Gtk::ScrolledWindow* m_ScrolledWindow;
	Gtk::Viewport* m_Viewport;
	Gtk::Fixed* m_Fixed;
	Gtk::Label* m_PageNameLabel;
	Gtk::Popover* m_RenamePagePopover;
	Gtk::Box* m_PageNameBox;
	Gtk::Entry* m_PageNameEntry;
	Gtk::Button* m_RenamePageButton;

};

} // puck
} // titania

#endif

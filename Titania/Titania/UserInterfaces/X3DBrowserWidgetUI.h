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
 ******************************************************************************/

#ifndef __TMP_GLAD2CPP_BROWSER_WIDGET_H__
#define __TMP_GLAD2CPP_BROWSER_WIDGET_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DBrowserWidgetUI :
	public X3DUserInterface
{
public:

	template <class ... Arguments>
	X3DBrowserWidgetUI (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		connections ()
	{ create (filename); }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	const Glib::RefPtr <Gtk::IconFactory> &
	getIconFactory () const { return m_iconFactory; }

	Gtk::MessageDialog &
	getMessageDialog () const { return *m_messageDialog; }

	Gtk::Window &
	getWindow () const { return *m_window; }

	Gtk::VBox &
	getWidget () const { return *m_widget; }

	Gtk::Toolbar &
	getNavigationBar () const { return *m_navigationBar; }

	Gtk::ToolButton &
	getForwardButton () const { return *m_forwardButton; }

	Gtk::ToolButton &
	getBackButton () const { return *m_backButton; }

	Gtk::ToolButton &
	getReloadButton () const { return *m_reloadButton; }

	Gtk::ToolButton &
	getHomeButton () const { return *m_homeButton; }

	Gtk::Entry &
	getLocationEntry () const { return *m_locationEntry; }

	Gtk::Toolbar &
	getToolBar () const { return *m_toolBar; }

	Gtk::Paned &
	getVPaned () const { return *m_vPaned; }

	Gtk::Paned &
	getHPaned () const { return *m_hPaned; }

	Gtk::HBox &
	getSurfaceBox () const { return *m_surfaceBox; }

	Gtk::RadioToolButton &
	getArrowButton () const { return *m_arrowButton; }

	Gtk::RadioToolButton &
	getHandButton () const { return *m_handButton; }

	Gtk::ToolButton &
	getLookAtAllButton () const { return *m_lookAtAllButton; }

	Gtk::ToggleToolButton &
	getLookAtButton () const { return *m_lookAtButton; }

	Gtk::Box &
	getFooter () const { return *m_footer; }

	Gtk::Button &
	getFooterCloseButton () const { return *m_footerCloseButton; }

	Gtk::Notebook &
	getFooterNotebook () const { return *m_footerNotebook; }

	Gtk::Box &
	getConsoleBox () const { return *m_consoleBox; }

	Gtk::Box &
	getSideBar () const { return *m_sideBar; }

	Gtk::Button &
	getSideBarCloseButton () const { return *m_sideBarCloseButton; }

	Gtk::Notebook &
	getSideBarNotebook () const { return *m_sideBarNotebook; }

	Gtk::Box &
	getHistoryEditorBox () const { return *m_historyEditorBox; }

	Gtk::Box &
	getViewpointEditorBox () const { return *m_viewpointEditorBox; }

	Gtk::Box &
	getOutlineEditorBox () const { return *m_outlineEditorBox; }

	Gtk::Statusbar &
	getStatusBar () const { return *m_statusBar; }

	virtual
	void
	messageDialogResponse (int response_id) = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_reload () = 0;

	virtual
	void
	on_home () = 0;

	virtual
	void
	on_locationEntry_activate () = 0;

	virtual
	void
	on_locationEntryIcon_activate (EntryIconPosition icon_position, const GdkEventButton* event) = 0;

	virtual
	void
	on_arrow_button_toggled () = 0;

	virtual
	void
	on_hand_button_toggled () = 0;

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	void
	on_look_at_toggled () = 0;

	virtual
	void
	dispose ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::deque <sigc::connection>   connections;
	Glib::RefPtr <Gtk::Builder>     m_builder;
	Glib::RefPtr <Gtk::IconFactory> m_iconFactory;
	Gtk::MessageDialog*             m_messageDialog;
	Gtk::Window*                    m_window;
	Gtk::VBox*                      m_widget;
	Gtk::Toolbar*                   m_navigationBar;
	Gtk::ToolButton*                m_forwardButton;
	Gtk::ToolButton*                m_backButton;
	Gtk::ToolButton*                m_reloadButton;
	Gtk::ToolButton*                m_homeButton;
	Gtk::Entry*                     m_locationEntry;
	Gtk::Toolbar*                   m_toolBar;
	Gtk::Paned*                     m_vPaned;
	Gtk::Paned*                     m_hPaned;
	Gtk::HBox*                      m_surfaceBox;
	Gtk::RadioToolButton*           m_arrowButton;
	Gtk::RadioToolButton*           m_handButton;
	Gtk::ToolButton*                m_lookAtAllButton;
	Gtk::ToggleToolButton*          m_lookAtButton;
	Gtk::Box*                       m_footer;
	Gtk::Button*                    m_footerCloseButton;
	Gtk::Notebook*                  m_footerNotebook;
	Gtk::Box*                       m_consoleBox;
	Gtk::Box*                       m_sideBar;
	Gtk::Button*                    m_sideBarCloseButton;
	Gtk::Notebook*                  m_sideBarNotebook;
	Gtk::Box*                       m_historyEditorBox;
	Gtk::Box*                       m_viewpointEditorBox;
	Gtk::Box*                       m_outlineEditorBox;
	Gtk::Statusbar*                 m_statusBar;

};

} // puck
} // titania

#endif

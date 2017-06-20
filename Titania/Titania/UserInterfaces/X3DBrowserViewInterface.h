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
#ifndef __TMP_GLAD2CPP_BROWSER_VIEW_H__
#define __TMP_GLAD2CPP_BROWSER_VIEW_H__

#include "../Base/X3DViewInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for BrowserView.
 */
class X3DBrowserViewInterface :
	public X3DViewInterface
{
public:

	///  @name Construction

	X3DBrowserViewInterface () :
		X3DViewInterface ()
	{ }

	template <class ... Arguments>
	X3DBrowserViewInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DViewInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DBrowserViewInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DViewInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::MenuBar &
	getMenuBar () const
	{ return *m_MenuBar; }

	Gtk::ImageMenuItem &
	getUndoViewMenuItem () const
	{ return *m_UndoViewMenuItem; }

	Gtk::ImageMenuItem &
	getRedoViewMenuItem () const
	{ return *m_RedoViewMenuItem; }

	Gtk::ImageMenuItem &
	getLookAtSelectionMenuItem () const
	{ return *m_LookAtSelectionMenuItem; }

	Gtk::ImageMenuItem &
	getLookAtAllMenuItem () const
	{ return *m_LookAtAllMenuItem; }

	Gtk::ImageMenuItem &
	getResetUserOffsetsMenuItem () const
	{ return *m_ResetUserOffsetsMenuItem; }

	Gtk::MenuItem &
	getCamerasMenuItem () const
	{ return *m_CamerasMenuItem; }

	Gtk::MenuItem &
	getMainViewMenuItem () const
	{ return *m_MainViewMenuItem; }

	Gtk::MenuItem &
	getTopViewMenuItem () const
	{ return *m_TopViewMenuItem; }

	Gtk::MenuItem &
	getRightViewMenuItem () const
	{ return *m_RightViewMenuItem; }

	Gtk::MenuItem &
	getFrontViewMenuItem () const
	{ return *m_FrontViewMenuItem; }

	Gtk::MenuItem &
	getBottomViewMenuItem () const
	{ return *m_BottomViewMenuItem; }

	Gtk::MenuItem &
	getLeftViewMenuItem () const
	{ return *m_LeftViewMenuItem; }

	Gtk::MenuItem &
	getBackViewMenuItem () const
	{ return *m_BackViewMenuItem; }

	Gtk::MenuItem &
	getDisplayMenuItem () const
	{ return *m_DisplayMenuItem; }

	Gtk::MenuItem &
	getDescriptionMenuItem () const
	{ return *m_DescriptionMenuItem; }

	Gtk::Box &
	getBrowserBox () const
	{ return *m_BrowserBox; }

	///  @name Signal handlers

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	undo_view_activate () = 0;

	virtual
	void
	redo_view_activate () = 0;

	virtual
	void
	on_look_at_selection_activate () = 0;

	virtual
	void
	on_look_at_all_activate () = 0;

	virtual
	void
	on_reset_user_offsets_activate () = 0;

	virtual
	void
	on_main_view_activate () = 0;

	virtual
	void
	on_top_view_activate () = 0;

	virtual
	void
	on_right_view_activate () = 0;

	virtual
	void
	on_front_view_activate () = 0;

	virtual
	void
	on_bottom_view_activate () = 0;

	virtual
	void
	on_left_view_activate () = 0;

	virtual
	void
	on_back_view_activate () = 0;

	///  @name Destruction

	virtual
	~X3DBrowserViewInterface () override;


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
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::MenuBar* m_MenuBar;
	Gtk::ImageMenuItem* m_UndoViewMenuItem;
	Gtk::ImageMenuItem* m_RedoViewMenuItem;
	Gtk::ImageMenuItem* m_LookAtSelectionMenuItem;
	Gtk::ImageMenuItem* m_LookAtAllMenuItem;
	Gtk::ImageMenuItem* m_ResetUserOffsetsMenuItem;
	Gtk::MenuItem* m_CamerasMenuItem;
	Gtk::MenuItem* m_MainViewMenuItem;
	Gtk::MenuItem* m_TopViewMenuItem;
	Gtk::MenuItem* m_RightViewMenuItem;
	Gtk::MenuItem* m_FrontViewMenuItem;
	Gtk::MenuItem* m_BottomViewMenuItem;
	Gtk::MenuItem* m_LeftViewMenuItem;
	Gtk::MenuItem* m_BackViewMenuItem;
	Gtk::MenuItem* m_DisplayMenuItem;
	Gtk::MenuItem* m_DescriptionMenuItem;
	Gtk::Box* m_BrowserBox;

};

} // puck
} // titania

#endif

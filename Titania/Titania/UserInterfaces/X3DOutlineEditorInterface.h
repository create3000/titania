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
#ifndef __TMP_GLAD2CPP_OUTLINE_EDITOR_H__
#define __TMP_GLAD2CPP_OUTLINE_EDITOR_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DOutlineEditorInterface :
	public X3DUserInterface
{
public:

	template <class ... Arguments>
	X3DOutlineEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		     connections ()
	{ create (filename); }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	Gtk::Window &
	getWindow () const { return *m_window; }

	Gtk::Box &
	getWidget () const { return *m_widget; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const { return *m_scrolledWindow; }

	Gtk::Viewport &
	getViewport () const { return *m_viewport; }

	virtual
	void
	on_map () = 0;


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::deque <sigc::connection> connections;
	Glib::RefPtr <Gtk::Builder>   m_builder;
	Gtk::Window*                  m_window;
	Gtk::Box*                     m_widget;
	Gtk::ScrolledWindow*          m_scrolledWindow;
	Gtk::Viewport*                m_viewport;

};

} // puck
} // titania

#endif

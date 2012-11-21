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

#include "Surface.h"

#include "../Bits/Error.h"
#include "../Types/Time.h"

#include <gtkmm/main.h>

#include <csignal>
#include <cstdlib>

namespace titania {
namespace X3D {

void
signal_handler (int sig)
{
	// print out all the frames to stderr
	std::clog << "Error: signal " << sig << ":" << std::endl;
	backtrace_fn (100);
	exit (1);
}

Surface::Surface (const SFNode <X3DBrowser> & browser) :
	opengl::Surface (),                          
	       motionBlur (new MotionBlur (*browser)), 
	          browser (browser),                   
	  pointingDevice  (*this),                     
	          viewer  (*this)                      
{
	// install our handler
	std::signal (SIGSEGV, signal_handler);

	add_events (Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK);

	getBrowser () -> changed .addInterest (static_cast <Gtk::Widget*> (this), &Surface::queue_draw);
}

void
Surface::setup ()
{
	get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));

	getBrowser () -> setup ();
	motionBlur -> setup ();
	viewer     .initialize ();
}

void
Surface::reshape ()
{
	motionBlur -> clear ();
}

void
Surface::update (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	try
	{
		getBrowser () -> prepare ();
		getBrowser () -> display ();
		swapBuffers ();
		getBrowser () -> finish ();
	}
	catch (const std::exception & exception)
	{
		std::clog
			<< getBrowser () -> getCurrentTime () << " Execution Error:" << std::endl
			<< "  " << exception .what () << std::endl;
	}

	cairo -> set_source_rgb (0.1, 0.1, 0.1);

	cairo -> select_font_face ("monospace",
	                           Cairo::FONT_SLANT_NORMAL,
	                           Cairo::FONT_WEIGHT_BOLD);

	cairo -> move_to (20, 30);
	cairo -> show_text ("Titania");
}

void
Surface::dispose ()
{
	if (getContext () and makeCurrent ())
		browser -> dispose ();
}

Surface::~Surface ()
{ }

} // X3D
} // titania

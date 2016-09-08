///* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
// *******************************************************************************
// *
// * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// *
// * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
// *
// * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
// *
// * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
// *
// * The copyright notice above does not evidence any actual of intended
// * publication of such source code, and is an unpublished work by create3000.
// * This material contains CONFIDENTIAL INFORMATION that is the property of
// * create3000.
// *
// * No permission is granted to copy, distribute, or create derivative works from
// * the contents of this software, in whole or in part, without the prior written
// * permission of create3000.
// *
// * NON-MILITARY USE ONLY
// *
// * All create3000 software are effectively free software with a non-military use
// * restriction. It is free. Well commented source is provided. You may reuse the
// * source in any way you please with the exception anything that uses it must be
// * marked to indicate is contains 'non-military use only' components.
// *
// * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// *
// * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
// *
// * This file is part of the Titania Project.
// *
// * Titania is free software: you can redistribute it and/or modify it under the
// * terms of the GNU General Public License version 3 only, as published by the
// * Free Software Foundation.
// *
// * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
// * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
// * details (a copy is included in the LICENSE file that accompanied this code).
// *
// * You should have received a copy of the GNU General Public License version 3
// * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
// * copy of the GPLv3 License.
// *
// * For Silvio, Joy and Adi.
// *
// ******************************************************************************/
//
//extern "C"
//{
//#include <GL/glew.h>
//}
//
//#include <SFML/Graphics.hpp>
//
//#include "SurfaceN.h"
//
//#include <gdk/gdkx.h>
//
//#include <Titania/LOG.h>
//
//namespace titania {
//namespace opengl {
//
//SurfaceN::SurfaceN () :
//	Gtk::Widget (),
//	renderWindow (new sf::RenderWindow ())
//{
//	set_has_window (false); // Makes this behave like an interal object rather then a parent window.
//}
//
//void
//SurfaceN::on_realize ()
//{
//	Gtk::Widget::on_realize ();
//
//	if (not window)
//	{
//		//Create the GdkWindow:
//		GdkWindowAttr attributes;
//		memset (&attributes, 0, sizeof (attributes));
//
//		Gtk::Allocation allocation = get_allocation ();
//
//		//Set initial position and size of the Gdk::Window:
//		attributes .x      = allocation .get_x ();
//		attributes .y      = allocation .get_y ();
//		attributes .width  = allocation .get_width ();
//		attributes .height = allocation .get_height ();
//
//		attributes .event_mask  = get_events () | Gdk::EXPOSURE_MASK | Gdk::STRUCTURE_MASK;
//		attributes .window_type = GDK_WINDOW_CHILD;
//		attributes .wclass      = GDK_INPUT_OUTPUT;
//
//		window = Gdk::Window::create (get_window (), &attributes, GDK_WA_X | GDK_WA_Y);
//		set_has_window (true);
//		set_window (window);
//
//		// transparent background
//		unset_background_color ();
//
//		set_double_buffered (false);
//
//		//make the widget receive expose events
//		window -> set_user_data (gobj ());
//
//		renderWindow .reset (new sf::RenderWindow (gdk_x11_window_get_xid (window -> gobj ()), sf::WindowSettings (32, 8, 8)));
//
//		if (makeCurrent ())
//		{
//			glewInit ();
//			construct ();
//		}
//	}
//}
//
//void
//SurfaceN::on_size_allocate (Gtk::Allocation & allocation)
//{
//	//Do something with the space that we have actually been given:
//	//(We will not be given heights or widths less than we have requested, though
//	//we might get more)
//
//	set_allocation (allocation);
//
//	if (window)
//	{
//		window -> move_resize (allocation.get_x (),
//		                               allocation .get_y (),
//		                               allocation .get_width (),
//		                               allocation .get_height ());
//
//		renderWindow -> SetSize (allocation.get_width (), allocation .get_height ());
//	}
//
//	if (makeCurrent ())
//	{
//		glViewport (0, 0, get_width (), get_height ());
//
//		reshape ();
//	}
//}
//
//void
//SurfaceN::on_unrealize ()
//{
//	window .clear ();
//
//	//Call base class:
//	Gtk::Widget::on_unrealize ();
//}
//
////bool
////SurfaceN::glew ()
////{
////	// Initilaize GLEW.
////
////	GLenum glewErrorNum = glewInit ();
////
////	if (glewErrorNum not_eq GLEW_OK)
////		throw std::runtime_error (std::string ("Error in glew init: ") + (char*) glewGetErrorString (glewErrorNum));
////
////	if (not GLEW_ARB_vertex_buffer_object)
////		throw std::runtime_error ("The glew vertex buffer objects are not supported.");
////
////	return true;
////}
////
////bool
////SurfaceN::set_map_event (GdkEventAny* event)
////{
////	map_connection .disconnect ();
////
////	context .reset (new WindowContext (gdk_x11_display_get_xdisplay (get_display () -> gobj ()),
////	                                   gdk_x11_window_get_xid (get_window () -> gobj ())));
////
////	if (makeCurrent ())
////	{
////		signal_configure_event () .connect (sigc::mem_fun (*this, &SurfaceN::set_configure_event));
////
////		construct_connection = signal_draw () .connect (sigc::mem_fun (*this, &SurfaceN::set_construct));
////
////		glewInit ();
////	}
////
////	return false; // Propagate the event further.
////}
//
//bool
//SurfaceN::on_configure_event (GdkEventConfigure* event)
//{
//	if (makeCurrent ())
//	{
//		glViewport (0, 0, get_width (), get_height ());
//
//		reshape ();
//	}
//
//	return false; // Propagate the event further.
//}
//
//bool
//SurfaceN::on_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
//{
//	if (makeCurrent ())
//		update (cairo);
//
//	return false; // Propagate the event further.
//}
//
//bool
//SurfaceN::makeCurrent ()
//{
//	return renderWindow -> SetActive (true);
//}
//
//void
//SurfaceN::swapBuffers ()
//{
//	glFinish ();
//
//	if (window)
//		renderWindow -> Display ();
//}
//
//void
//SurfaceN::dispose ()
//{
//	renderWindow .reset ();
//}
//
//SurfaceN::~SurfaceN ()
//{ }
//
//} // opengl
//} // titania

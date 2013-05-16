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

#ifndef __TITANIA_OPEN_GL_DRAWING_AREA_H__
#define __TITANIA_OPEN_GL_DRAWING_AREA_H__

#include <gtkmm/drawingarea.h>

namespace titania {
namespace opengl {

class DrawingArea :
	public sigc::trackable
{
public:

	DrawingArea ();

	GtkDrawingArea*
	gobj ()
	{ return widget; }

	const GtkDrawingArea*
	gobj () const
	{ return widget; }

	///  @name Properties

	void
	pack_start_to (GtkBox* box,
	               gboolean expand,
	               gboolean fill,
	               guint padding)
	{ gtk_box_pack_start (box, GTK_WIDGET (gobj ()), expand, fill, padding); }

	double
	get_width () const
	{
		GtkAllocation* alloc = g_new (GtkAllocation, 1);

		gtk_widget_get_allocation (GTK_WIDGET (gobj ()), alloc);
		auto width = alloc -> width;
		g_free (alloc);
		return width;
	}

	double
	get_height () const
	{
		GtkAllocation* alloc = g_new (GtkAllocation, 1);

		gtk_widget_get_allocation (GTK_WIDGET (gobj ()), alloc);
		auto height = alloc -> height;
		g_free (alloc);
		return height;
	}

	void
	set_double_buffered (gboolean double_buffered)
	{ gtk_widget_set_double_buffered (GTK_WIDGET (gobj ()), double_buffered); }

	void
	set_app_paintable (gboolean app_paintable)
	{ gtk_widget_set_app_paintable (GTK_WIDGET (gobj ()), app_paintable); }

	void
	grab_focus ()
	{ gtk_widget_grab_focus (GTK_WIDGET (gobj ())); }

	void
	set_can_focus (gboolean can_focus)
	{ gtk_widget_set_can_focus (GTK_WIDGET (gobj ()), can_focus); }

	void
	show ()
	{ gtk_widget_show (GTK_WIDGET (gobj ())); }

	void
	add_events (gint events)
	{ gtk_widget_add_events (GTK_WIDGET (gobj ()), events); }

	GdkDisplay*
	get_display () const
	{ return gtk_widget_get_display (GTK_WIDGET (gobj ())); }

	GdkWindow*
	get_window () const
	{ return gtk_widget_get_window (GTK_WIDGET (gobj ())); }

	void
	queue_draw ()
	{ gtk_widget_queue_draw (GTK_WIDGET (gobj ())); }

	///  @name Signals

	sigc::signal <bool, GdkEventButton*>
	signal_button_press_event () const
	{ return m_signal_button_press_event; }

	sigc::signal <bool, GdkEventButton*>
	signal_button_release_event () const
	{ return m_signal_button_release_event; }

	sigc::signal <bool, GdkEventScroll*>
	signal_scroll_event () const
	{ return m_signal_scroll_event; }

	sigc::signal <bool, GdkEventMotion*>
	signal_motion_notify_event () const
	{ return m_signal_motion_notify_event; }

	sigc::signal <bool, GdkEventKey*>
	signal_key_press_event () const
	{ return m_signal_key_press_event; }

	sigc::signal <bool, GdkEventKey*>
	signal_key_release_event () const
	{ return m_signal_key_release_event; }

	sigc::signal <bool, GdkEventAny*>
	signal_map_event () const
	{ return m_signal_map_event; }

	sigc::signal <bool, GdkEventConfigure*>
	signal_configure_event () const
	{ return m_signal_configure_event; }

	sigc::signal <bool>
	signal_draw () const
	{ return m_signal_draw; }

	virtual
	~DrawingArea ();


private:

	static
	gboolean
	button_press_event_cb (GtkWidget* widget, GdkEventButton* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_button_press_event .emit (event); }

	static
	gboolean
	button_release_event_cb (GtkWidget* widget, GdkEventButton* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_button_release_event .emit (event); }

	static
	gboolean
	scroll_event_cb (GtkWidget* widget, GdkEventScroll* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_scroll_event .emit (event); }

	static
	gboolean
	motion_notify_event_cb (GtkWidget* widget, GdkEventMotion* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_motion_notify_event .emit (event); }

	static
	gboolean
	key_press_event_cb (GtkWidget* widget, GdkEventKey* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_key_press_event .emit (event); }

	static
	gboolean
	key_release_event_cb (GtkWidget* widget, GdkEventKey* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_key_release_event .emit (event); }

	static
	gboolean
	map_event_cb (GtkWidget* widget, GdkEventAny* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_map_event .emit (event); }

	static
	gboolean
	configure_event_cb (GtkWidget* widget, GdkEventConfigure* event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_configure_event .emit (event); }

	static
	gboolean
	draw_cb (GtkWidget* widget, cairo_t* cr, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_draw .emit (); }

	static
	gboolean
	expose_event_cb (GtkWidget *widget, GdkEventExpose *event, gpointer data)
	{ return static_cast <DrawingArea*> (data) -> m_signal_draw .emit (); }

	sigc::signal <bool, GdkEventButton*> m_signal_button_press_event;
	sigc::signal <bool, GdkEventButton*> m_signal_button_release_event;
	sigc::signal <bool, GdkEventScroll*> m_signal_scroll_event;
	sigc::signal <bool, GdkEventMotion*> m_signal_motion_notify_event;

	sigc::signal <bool, GdkEventKey*> m_signal_key_press_event;
	sigc::signal <bool, GdkEventKey*> m_signal_key_release_event;

	sigc::signal <bool, GdkEventAny*>                           m_signal_map_event;
	sigc::signal <bool, GdkEventConfigure*>                     m_signal_configure_event;
	sigc::signal <bool>                                         m_signal_draw;

	GtkDrawingArea* widget;

};

} // opengl
} // titania

#endif

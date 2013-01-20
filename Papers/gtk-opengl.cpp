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

#include <GL/gl.h>
#include <GL/glx.h>
#include <cstdlib>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include <iostream>

GLXContext context;

int
area_start (GtkWidget* widget, void* data)
{
	GdkWindow* window;
	Display*   display;
	int        id;

	window  = gtk_widget_get_window (widget);
	display = GDK_WINDOW_XDISPLAY (window);
	id      = GDK_WINDOW_XID (window);

	if (glXMakeCurrent (display, id, context) == TRUE)
	{
		glEnable (GL_DEPTH_TEST);
		glDepthFunc (GL_LEQUAL);
		glEnable (GL_CULL_FACE);
		glCullFace (GL_BACK);
		glDisable (GL_DITHER);
		glShadeModel (GL_SMOOTH);
	}

	return TRUE;
}

int
area_draw (GtkWidget* widget, GdkEventExpose* event, void* data)
{
	std::cout << "area_draw" << std::endl;

	GdkWindow* window;
	Display*   display;
	int        id;

	//if (event -> count > 0)
	//	return TRUE;

	window  = gtk_widget_get_window (widget);
	display = GDK_WINDOW_XDISPLAY (window);
	id      = GDK_WINDOW_XID (window);

	if (glXMakeCurrent (display, id, context) == TRUE)
	{
		glClear (GL_DEPTH_BUFFER_BIT);

		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho (0, 100, 100, 0, -1, 1);

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();

		glClearColor (0, 0, 0, 1);
		glClear (GL_COLOR_BUFFER_BIT);
		glColor3f (1, 1, 1);
		glBegin (GL_TRIANGLES);
		glVertex2f (10, 10);
		glVertex2f (10, 90);
		glVertex2f (90, 90);
		glEnd ();

		glXSwapBuffers (display, id);
	}

	return TRUE;
}

int
area_configure (GtkWidget* widget, GdkEventConfigure* event, void* data)
{
	GtkAllocation allocation;
	GdkWindow*    window;
	Display*      display;
	int           id;

	window  = gtk_widget_get_window (widget);
	display = GDK_WINDOW_XDISPLAY (window);
	id      = GDK_WINDOW_XID (window);

	if (glXMakeCurrent (display, id, context) == TRUE)
	{
		gtk_widget_get_allocation (widget, &allocation);
		glViewport (0, 0, allocation .width, allocation .height);
	}

	return TRUE;
}

int
main (int argc, char** argv)
{
	GtkWidget* window;
	GtkWidget* area;
	GdkVisual* visual;

	GdkScreen*   screen;
	XVisualInfo* xvisual;

	Display* display;

	int xscreen;
	int attributes [ ] = { GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
		                   GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, True, GLX_DEPTH_SIZE, 12, None };

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "GLX");

	area = gtk_drawing_area_new ();
	gtk_widget_set_double_buffered (area, FALSE);

	display = gdk_x11_get_default_xdisplay ();
	xscreen = DefaultScreen (display);
	screen  = gdk_screen_get_default ();
	xvisual = glXChooseVisual (display, xscreen, attributes);
	visual  = gdk_x11_screen_lookup_visual (screen, xvisual -> visualid);

	context = glXCreateContext (display, xvisual, NULL, TRUE);

	XFree (xvisual);

	gtk_container_add (GTK_CONTAINER (window), area);
	gtk_widget_set_size_request (area, 100, 100);

	g_signal_connect (area, "configure_event", G_CALLBACK (area_configure), window);
	g_signal_connect (area, "realize", G_CALLBACK (area_start), window);
	g_signal_connect (area, "draw", G_CALLBACK (area_draw), window);

	gtk_widget_show (area);

	gtk_widget_show (window);

	gtk_main ();

	glXDestroyContext (display, context);

	g_object_unref (G_OBJECT (visual));

	return 0;
}

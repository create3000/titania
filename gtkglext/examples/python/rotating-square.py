#!/usr/bin/python
# -*- coding: utf-8 -*-

# rotating-square.py
# A simple animation to demonstrate GtkGLExt.
#
# Copyright © 2012  B. Clausius <barcc@gmx.de>
# This program is in the public domain and you are using it at your own risk.
#
# This program is a port of rotating-square.c by
# Alif Wahid <awah005@users.sourceforge.net> and
# Naofumi Yasufuku  <naofumi@users.sourceforge.net>

from __future__ import print_function

import sys

from gi.repository import Gtk, Gdk, GLib
from gi.repository import GtkGLExt, GdkGLExt
from OpenGL import GL


DEFAULT_WIDTH = 200
DEFAULT_HEIGHT = 200
DEFAULT_TITLE = "Rotating Square"


animate = True
spin = 0.


# The following section contains all the callback function definitions.

def realize(widget, data):
    '''The "realize" signal handler. All the OpenGL initialization
    should be performed here, such as default background colour,
    certain states etc.'''

    # OpenGL BEGIN
    if not GtkGLExt.widget_begin_gl(widget):
        return

    GL.glClearColor(0., 0., 0., 0.)
    GL.glShadeModel(GL.GL_FLAT)

    GtkGLExt.widget_end_gl(widget, False)
    # OpenGL END

def configure_event(widget, event, data):
    '''The "configure_event" signal handler. Any processing required when
    the OpenGL-capable drawing area is re-configured should be done here.
    Almost always it will be used to resize the OpenGL viewport when
    the window is resized.'''

    allocation = widget.get_allocation()
    w = allocation.width
    h = allocation.height

    # OpenGL BEGIN
    if not GtkGLExt.widget_begin_gl(widget):
        return False

    GL.glViewport(0, 0, w, h)
    GL.glMatrixMode(GL.GL_PROJECTION)
    GL.glLoadIdentity()
    GL.glOrtho(-50., 50., -50., 50., -1., 1.)
    GL.glMatrixMode(GL.GL_MODELVIEW)
    GL.glLoadIdentity()

    GtkGLExt.widget_end_gl(widget, False)
    # OpenGL END

    return True

def draw (widget, cr, data):
    '''The "draw" signal handler. All the OpenGL re-drawing should
    be done here. This is repeatedly called as the painting routine
    every time the 'draw' event is signalled.'''

    # OpenGL BEGIN
    if not GtkGLExt.widget_begin_gl(widget):
        return False

    GL.glClear(GL.GL_COLOR_BUFFER_BIT)

    GL.glPushMatrix()
    GL.glRotatef(spin, 0., 0., 1.)
    GL.glColor3f(1., 1., 1.)
    GL.glRectf(-25., -25., 25., 25.)
    GL.glPopMatrix()

    GtkGLExt.widget_end_gl(widget, True)
    # OpenGL END

    return True

def idle(widget):
    '''The idle function. Often in animations,
    idle functions are suitable for continous
    frame updates.'''

    global spin

    window = widget.get_window()
    allocation = widget.get_allocation()

    spin += 2.
    if spin > 360.:
        spin -= 360.

    # Invalidate the whole window.
    window.invalidate_rect(allocation, False)

    # Update synchronously.
    window.process_updates(False)

    return True

def motion_notify_event(widget, event, data):
    '''The "motion_notify_event" signal handler. Any processing required when
    the OpenGL-capable drawing area is under drag motion should be done here.'''

    # Fill in the details here.
    return False

def button_press_event(widget, event, data):
    '''The "button_press_event" signal handler. Any processing required when
    mouse buttons (only left and middle buttons) are pressed on the OpenGL-
    capable drawing area should be done here.
    
    Starts/Stops animation according mouse button clicks.
    '''
    if event.button == 1:
        toggle_animation(widget)
        return True
    return False

def button_press_event_popup_menu(widget, event):
    '''For popup menu.'''
    
    if event.button == 3:
        # Popup menu.
        widget.popup(None, None, None, None, event.button, event.time)
        return True
    return False

def key_press_event(widget, event):
    '''The "key_press_event" signal handler. Any processing required when key
    presses occur should be done here.'''
    
    global spin

    window = widget.get_window()
    allocation = widget.get_allocation()

    if event.keyval == Gdk.KEY_Left:
        # Rotate left.
        if not animate:
            spin += 2.
            if spin > 360.:
                spin -= 360.
            window.invalidate_rect(allocation, False)
    elif event.keyval == Gdk.KEY_Right:
        # Rotate right.
        if not animate:
            spin -= 2.
            if spin < 360.:
                spin += 360.
            window.invalidate_rect(allocation, False)
    elif event.keyval == Gdk.KEY_a:
        # Toggle animation.
        toggle_animation(widget)
    elif event.keyval == Gdk.KEY_Escape:
        # Quit.
        Gtk.main_quit()
    else:
        return False

    return True

def unrealize(widget, data):
    '''The "unrealize" signal handler. Any processing required when
    the OpenGL-capable window is unrealized should be done here.'''
    
    # Fill in the details here
    pass


#
# The following section contains the idle function management routines.
#

# Helper functions to add or remove the idle function.

idle_id = 0

def idle_add(widget):
    global idle_id
    if idle_id == 0:
        #idle_id = GLib.idle_add(idle, widget, priority=Gdk.PRIORITY_REDRAW+100)
        idle_id = GLib.timeout_add(8, idle, widget, priority=Gdk.PRIORITY_REDRAW+100)

def idle_remove(widget):
    global idle_id
    if idle_id != 0:
        GLib.source_remove(idle_id)
        idle_id = 0

def map_event(widget, event, data):
    '''The "map_event" signal handler. Any processing required when the
    OpenGL-capable drawing area is mapped should be done here.'''
    
    if animate:
        idle_add(widget)
    return True

def unmap_event(widget, event, data):
    '''The "unmap_event" signal handler. Any processing required when the
    OpenGL-capable drawing area is unmapped should be done here.'''
    
    idle_remove(widget)
    return True

def visibility_notify_event(widget, event, data):
    '''The "visibility_notify_event" signal handler. Any processing required
    when the OpenGL-capable drawing area is visually obscured should be
    done here.'''
    
    if animate:
        if event.state == Gdk.VisibilityState.FULLY_OBSCURED:
            idle_remove(widget)
        else:
            idle_add(widget)
    return True


#
#  The following section contains some miscellaneous utility functions.
#

def toggle_animation(widget):
    '''Toggle animation.'''
    
    global animate
    animate = not animate

    if animate:
        idle_add(widget)
    else:
        idle_remove(widget)
        allocation = widget.get_allocation()
        widget.get_window().invalidate_rect(allocation, False)


#
# The following section contains the GUI building function definitions.
#

def create_popup_menu(drawing_area):
    '''Creates the popup menu to be displayed.'''
    
    menu = Gtk.Menu()

    # Toggle animation
    menu_item = Gtk.MenuItem.new_with_label("Toggle Animation")
    menu.append(menu_item)
    menu_item.connect_object("activate", toggle_animation, drawing_area)
    menu_item.show()

    # Quit
    menu_item = Gtk.MenuItem.new_with_label("Quit")
    menu.append(menu_item)
    menu_item.connect("activate", Gtk.main_quit)
    menu_item.show()

    return menu

def create_window(glconfig):
    '''Creates the simple application window with one
    drawing area that has an OpenGL-capable visual.'''

    ## Top-level window.

    window = Gtk.Window(Gtk.WindowType.TOPLEVEL)
    window.set_title(DEFAULT_TITLE)

    # Get automatically redrawn if any of their children changed allocation.
    window.set_reallocate_redraws(True)

    # Connect signal handlers to the window
    window.connect("delete_event", Gtk.main_quit)

    ## VBox.

    vbox = Gtk.VBox(False, 0)
    window.add(vbox)
    vbox.show()

    ## Drawing area to draw OpenGL scene.

    drawing_area = Gtk.DrawingArea()
    drawing_area.set_size_request(DEFAULT_WIDTH, DEFAULT_HEIGHT)

    # Set OpenGL-capability to the widget
    GtkGLExt.widget_set_gl_capability(drawing_area,
                  glconfig,
                  None,
                  True,
                  GdkGLExt.RenderType.RGBA_TYPE)
    drawing_area.add_events(
               Gdk.EventMask.BUTTON1_MOTION_MASK    |
               Gdk.EventMask.BUTTON2_MOTION_MASK    |
               Gdk.EventMask.BUTTON_PRESS_MASK      |
               Gdk.EventMask.VISIBILITY_NOTIFY_MASK)

    # Connect signal handlers to the drawing area
    drawing_area.connect_after("realize", realize, None)
    drawing_area.connect("configure_event", configure_event, None)
    drawing_area.connect("draw", draw, None)

    drawing_area.connect("motion_notify_event", motion_notify_event, None)
    drawing_area.connect("button_press_event", button_press_event, None)
    drawing_area.connect("unrealize", unrealize, None)

    # key_press_event handler for top-level window
    window.connect_object("key_press_event", key_press_event, drawing_area)

    # For idle function.
    drawing_area.connect("map_event", map_event, None)
    drawing_area.connect("unmap_event", unmap_event, None)
    drawing_area.connect("visibility_notify_event", visibility_notify_event, None)

    vbox.pack_start(drawing_area, True, True, 0)
    drawing_area.show()

    # Popup menu.

    menu = create_popup_menu(drawing_area)
    drawing_area.connect_object("button_press_event", button_press_event_popup_menu, menu)

    ## Simple quit button.

    button = Gtk.Button.new_with_label("Quit")
    button.connect("clicked", Gtk.main_quit)
    vbox.pack_start(button, False, False, 0)
    button.show()

    return window


#
# The following section contains utility function definitions.
#

def configure_gl():
    '''Configure the OpenGL framebuffer.'''

    # Try double-buffered visual
    try:
        glconfig = GdkGLExt.Config.new_by_mode(GdkGLExt.ConfigMode.RGB |
                                               GdkGLExt.ConfigMode.DEPTH |
                                               GdkGLExt.ConfigMode.DOUBLE)
    except TypeError:
        print("*** Cannot find the double-buffered visual.")
        print("*** Trying single-buffered visual.")

        # Try single-buffered visual
        try:
            glconfig = GdkGLExt.Config.new_by_mode(GdkGLExt.ConfigMode.RGB |
                                                   GdkGLExt.ConfigMode.DEPTH)
        except TypeError:
            print("*** No appropriate OpenGL-capable visual found.")
            sys.exit(1)

    return glconfig


#
# The main function is rather trivial.
#

def main():
    # Configure OpenGL framebuffer.
    glconfig = configure_gl()

    # Create and show the application window.
    window = create_window(glconfig)
    window.show()

    Gtk.main()
    return 0


if __name__ == '__main__':
    sys.exit(main())


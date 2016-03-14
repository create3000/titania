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

#include "X3DUserInterface.h"

#include "../Configuration/config.h"
#include "DialogFactory.h"

#include <Titania/String.h>
#include <Titania/String.h>
#include <cassert>

namespace titania {
namespace puck {

const std::unique_ptr <DialogFactory> X3DUserInterface::dialogFactory (new DialogFactory ());

// Only add presentable dialogs here.
const std::set <std::string> X3DUserInterface::restorableDialogs = {
	"ScenePropertiesEditor",
	"MotionBlurEditor",
	"GridEditor",
	"NodeEditor",
	"NodePropertiesEditor",
	"AppearanceEditor",
	"TextureEditor",
	"TextEditor",
	"GeometryPropertiesEditor",
	"TextureMappingEditor",
	"ColorEditor",
	"LayerEditor",
	"BackgroundEditor",
	"NavigationInfoEditor",
	"ViewpointEditor",
	"LightEditor",
	"InlineEditor",
	"PrecisionPlacementPanel",
	"PrototypeEditor",
	"NodeIndex",

};

X3DUserInterface::UserInterfaceArray X3DUserInterface::userInterfaces;

X3DUserInterface::X3DUserInterface () :
	X3DUserInterface ("")
{
	assert (false);
}

X3DUserInterface::X3DUserInterface (const std::string & widgetName) :
	      X3DBaseInterface (),
	                config (new Configuration (widgetName)),
	constructed_connection (),
	         userInterface (),
	               dialogs (new DialogIndex ())
{
	X3DBaseInterface::setName (widgetName);

	userInterfaces .emplace_back (this);
	userInterface = -- userInterfaces .end ();
}

void
X3DUserInterface::construct ()
{
	constructed_connection = getWidget () .signal_map () .connect (sigc::mem_fun (*this, &X3DUserInterface::on_constructed));

	getWindow () .signal_window_state_event () .connect (sigc::mem_fun (*this, &X3DUserInterface::on_window_state_event));
	getWindow () .signal_delete_event ()       .connect (sigc::mem_fun (*this, &X3DUserInterface::on_delete_event), false);

	restoreInterface ();
}

void
X3DUserInterface::setName (const std::string & value)
{
	X3DBaseInterface::setName (value);

	config .reset (new Configuration (value));
	configure ();
}

void
X3DUserInterface::on_constructed ()
{
	constructed_connection .disconnect ();

	getWindow () .set_deletable (true); /// ??? Does it work with the Gnome shell ???
	getWidget () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::TOP_LEFT_ARROW));

	getWidget () .signal_map ()   .connect (sigc::mem_fun (*this, &X3DUserInterface::on_map));
	getWidget () .signal_unmap () .connect (sigc::mem_fun (*this, &X3DUserInterface::on_unmap));

	on_map ();

	#ifdef TITANIA_DEBUG
	std::clog << "Initializing widget: " << getWidgetName () << std::endl;
	#endif

   connectFocusEvent (getWidget ());

	initialize ();

	if (isFullscreen ())
	   getWindow () .fullscreen ();

	 set_fullscreen (isFullscreen ());
}

void
X3DUserInterface::connectFocusEvent (Gtk::Widget & parent)
{
	for (auto & widget : getWidgets <Gtk::Widget> (parent))
	{
		const auto instance = widget -> gobj ();

		if (G_TYPE_CHECK_INSTANCE_TYPE (instance, GTK_TYPE_ENTRY) ||
		    G_TYPE_CHECK_INSTANCE_TYPE (instance, GTK_TYPE_TEXT_VIEW))
		{
			widget -> signal_focus_in_event ()  .connect (sigc::mem_fun (*this, &X3DUserInterface::on_focus_in_event));
			widget -> signal_focus_out_event () .connect (sigc::mem_fun (*this, &X3DUserInterface::on_focus_out_event));
		}
	}
}

void
X3DUserInterface::removeFocus (Gtk::Widget & parent)
{
	for (auto & widget : getWidgets <Gtk::Widget> (parent))
	{
		const auto instance = widget -> gobj ();

		if (G_TYPE_CHECK_INSTANCE_TYPE (instance, GTK_TYPE_ENTRY) ||
		    G_TYPE_CHECK_INSTANCE_TYPE (instance, GTK_TYPE_TEXT_VIEW))
		{
			if (widget -> has_focus ())
			{
			   getBrowserWindow () -> hasAccelerators (true);
			   break;
			}
		}
	}
}

void
X3DUserInterface::configure ()
{
	// Restore dialogs

	for (const auto & dialogName : basic::split (getConfig () -> getString ("dialogs"), ";"))
	{
		if (restorableDialogs .count (dialogName))
			addDialog (dialogName);
	}
}

void
X3DUserInterface::on_map ()
{
	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DEditorInterface::set_selection);

	restoreInterface ();
	configure ();
	set_selection (getBrowserWindow () -> getSelection () -> getChildren ());
}

void
X3DUserInterface::on_unmap ()
{
	store ();
	set_selection ({ });
}

bool
X3DUserInterface::on_window_state_event (GdkEventWindowState* event)
{
	const bool maximized  = event -> new_window_state & GDK_WINDOW_STATE_MAXIMIZED;
	const bool fullscreen = event -> new_window_state & GDK_WINDOW_STATE_FULLSCREEN;

	getConfig () -> setItem ("maximized",  maximized);

	if (fullscreen not_eq getConfig () -> getBoolean ("fullscreen"))
	{
		getConfig () -> setItem ("fullscreen", fullscreen);
		set_fullscreen (fullscreen);
	}

	return false;
}

bool
X3DUserInterface::on_focus_in_event (GdkEventFocus* event)
{
	getBrowserWindow () -> hasAccelerators (false);
	return false;
}

bool
X3DUserInterface::on_focus_out_event (GdkEventFocus* event)
{
	getBrowserWindow () -> hasAccelerators (true);
	return false;
}

bool
X3DUserInterface::on_delete_event (GdkEventAny*)
{
	removeFocus (getWidget ());

	return quit ();
}

bool
X3DUserInterface::hasDialog (const std::string & name) const
{
	return dialogs -> count (name);
}

std::shared_ptr <X3DUserInterface>
X3DUserInterface::addDialog (const std::string & name, const bool present)
{
	try
	{
		const auto dialog = dialogs -> at (name);

		if (present)
			dialog -> getWindow () .present ();

		return dialog;
	}
	catch (const std::out_of_range &)
	{
		const auto dialog = createDialog (name);

		dialogs -> emplace (name, dialog);
		dialog -> getWindow () .signal_hide () .connect (sigc::bind (sigc::mem_fun (*this, &X3DUserInterface::removeDialog), name), false);

		if (present)
			dialog -> getWindow () .present ();

		return dialog;
	}
}

std::shared_ptr <X3DUserInterface>
X3DUserInterface::createDialog (const std::string & name) const
{
	return dialogFactory -> createDialog (name, getBrowserWindow ());
}

void
X3DUserInterface::removeDialog (const std::string & name)
{
std::clog << getWidgetName () << " : " << name << std::endl;

	Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (*this, &X3DUserInterface::removeDialogImpl), name), Glib::PRIORITY_HIGH);
}

void
X3DUserInterface::removeDialogImpl (const std::string & name)
{
std::clog << getWidgetName () << " : " << name << std::endl;

	dialogs -> erase (name);
}

void
X3DUserInterface::reparent (Gtk::Box & box, Gtk::Window & window)
{
	getWindow () .set_transient_for (window);

	const auto container = getWidget () .get_parent ();

	if (container)
	   container -> remove (getWidget ());

	box .pack_start (getWidget (), true, true);
}

void
X3DUserInterface::reparent (Gtk::Overlay & overlay, Gtk::Window & window)
{
	getWindow () .set_transient_for (window);

	const auto container = getWidget () .get_parent ();

	if (container)
	   container -> remove (getWidget ());

	overlay .add_overlay (getWidget ());
}

void
X3DUserInterface::restoreInterface ()
{
	// Restore window size and position

	if (getConfig () -> hasItem ("x") and getConfig () -> hasItem ("y"))
	{
		getWindow () .move (getConfig () -> getInteger ("x"),
		                    getConfig () -> getInteger ("y"));
	}

	if (getConfig () -> getInteger ("width") > 0 and getConfig () -> getInteger ("height") > 0)
	{
		getWindow () .resize (getConfig () -> getInteger ("width"),
		                      getConfig () -> getInteger ("height"));
	}

	if (isMaximized ())
		getWindow () .maximize ();
}

void
X3DUserInterface::saveInterfaces ()
{
	for (const auto & userInterface : userInterfaces)
		userInterface -> saveInterface ();
}

void
X3DUserInterface::saveInterface ()
{
	if (not getWindow () .get_mapped ())
		return;

	if (not isMaximized () and not isFullscreen ())
	{
		int x, y, width, height;

		getWindow () .get_position (x, y);
		getConfig () -> setItem ("x", x);
		getConfig () -> setItem ("y", y);

		getWindow () .get_size (width, height);
		getConfig () -> setItem ("width",  width);
		getConfig () -> setItem ("height", height);
	}
}

bool
X3DUserInterface::quit ()
{
	// Save sessions

	if (this == userInterfaces .front ())
		saveInterfaces ();
	else
		saveInterface ();

	getWindow () .hide (); // Hide window as last command.

	// Prevent destroying Window.
	return true;
}

void
X3DUserInterface::store ()
{
	// Save dialogs

	std::string dialogNames;

	for (const auto & pair : *dialogs)
	{
		dialogNames += pair .first;
		dialogNames += ";";
	}

	if (not dialogNames .empty ())
		dialogNames .resize (dialogNames .size () - 1);

	getConfig () -> setItem ("dialogs", dialogNames);

	// Close dialogs

	for (const auto & pair : *dialogs)
		pair .second -> quit ();

	dialogs -> clear ();
}

void
X3DUserInterface::dispose ()
{
	X3DBaseInterface::dispose ();
}

X3DUserInterface::~X3DUserInterface ()
{
	userInterfaces .erase (userInterface);
}

} // puck
} // titania

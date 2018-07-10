/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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

#include "X3DUserInterface.h"

#include "../Browser/BrowserSelection.h"
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
	"AppearanceEditor",
	"BackgroundEditor",
	"BackgroundImageEditor",
	"ColorEditor",
	"ExternalToolsEditor",
	"GeometryPropertiesEditor",
	"GridEditor",
	"InlineEditor",
	"LayerEditor",
	"LightEditor",
	"MotionBlurEditor",
	"NavigationInfoEditor",
	"NodeEditor",
	"NodeIndex",
	"NodePropertiesEditor",
	"PrecisionPlacementPanel",
	"PrototypeEditor",
	"ScenePropertiesEditor",
	"TextEditor",
	"TextureEditor",
	"TextureMappingEditor",
	"UndoHistoryEditor",
	"ViewpointEditor",
};

X3DUserInterface::UserInterfaceArray X3DUserInterface::userInterfaces;

X3DUserInterface::X3DUserInterface () :
	     X3DBaseInterface (),
	               config (),
	windowStateConnection (),
	        mapConnection (),
	        userInterface (),
	              dialogs (new DialogIndex ()),
	          initialized (false)
{
	userInterfaces .emplace_back (this);
	userInterface = -- userInterfaces .end ();
}

void
X3DUserInterface::setup ()
{
	X3DBaseInterface::setup ();

	assert (not mapConnection .connected ());
	assert (not getWidget () .get_name () .empty ());

	setTypeName (getWidget () .get_name ());

	if (getName () .empty ())
		X3DBaseInterface::setName (getWidget () .get_name ());

	config .reset (new Configuration (getName ()));

	mapConnection = getWidget () .signal_map () .connect (sigc::mem_fun (this, &X3DUserInterface::on_initialize));

	getWindow () .signal_window_state_event () .connect (sigc::mem_fun (this, &X3DUserInterface::on_window_state_event));
	getWindow () .signal_delete_event ()       .connect (sigc::mem_fun (this, &X3DUserInterface::on_delete_event), false);

	restoreInterface ();

	if (isMaximized ())
		getWindow () .maximize ();

	if (isFullscreen ())
		getWindow () .fullscreen ();
}

void
X3DUserInterface::on_initialize ()
{
	//__LOG__ << "Initializing widget: " << getName () << std::endl;

	if (initialized)
		return;

	initialized = true;

	mapConnection .disconnect ();

	getWindow () .set_deletable (true); /// ??? Does it work with the Gnome shell ???
	getWidget () .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "default"));

	mapConnection = getWidget () .signal_map () .connect (sigc::mem_fun (this, &X3DUserInterface::on_map));
	getWidget () .signal_unmap () .connect (sigc::mem_fun (this, &X3DUserInterface::on_unmap));

	connectFocusEvent (getWidget ());

	configure ();
	on_maximized (isMaximized ());
	on_fullscreen (isFullscreen ());
	initialize ();

	on_map ();

	Glib::signal_idle () .connect_once (sigc::mem_fun (this, &X3DUserInterface::restoreDialogs), Glib::PRIORITY_HIGH);

	//__LOG__ << "Initialized widget: " << getName () << std::endl;
}

void
X3DUserInterface::connectFocusEvent (Gtk::Widget & parent)
{
	for (auto & widget : getWidgets <Gtk::Widget> (parent))
	{
		const auto instance = widget -> gobj ();

		if (G_TYPE_CHECK_INSTANCE_TYPE (instance, GTK_TYPE_ENTRY) or
		    G_TYPE_CHECK_INSTANCE_TYPE (instance, GTK_TYPE_TEXT_VIEW))
		{
			widget -> signal_focus_in_event ()  .connect (sigc::mem_fun (this, &X3DUserInterface::on_focus_in_event));
			widget -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DUserInterface::on_focus_out_event));
			continue;
		}
	}
}

void
X3DUserInterface::on_map ()
{
	restoreInterface ();

	getBrowserWindow () -> getSelection () -> getNodes () .addInterest (&X3DEditorInterface::set_selection_and_check, this);

	set_selection_and_check (getBrowserWindow () -> getSelection () -> getNodes ());
}

void
X3DUserInterface::on_unmap ()
{
	if (not getBrowserWindow () -> getSelection () .get ())
		return;

	getBrowserWindow () -> getSelection () -> getNodes () .removeInterest (&X3DEditorInterface::set_selection_and_check, this);

	set_selection_and_check ({ });
}

void
X3DUserInterface::set_selection_and_check (const X3D::MFNode & selection)
{
	if (getSelectionContext (selection))
		set_selection (selection);
	else
		set_selection ({ });
}

bool
X3DUserInterface::on_window_state_event (GdkEventWindowState* event)
{
	const bool maximized  = event -> new_window_state & GDK_WINDOW_STATE_MAXIMIZED;
	const bool fullscreen = event -> new_window_state & GDK_WINDOW_STATE_FULLSCREEN;

	if (maximized not_eq getConfig () -> getItem <bool> ("maximized"))
	{
		getConfig () -> setItem <bool> ("maximized", maximized);
		on_maximized (maximized);
	}

	if (fullscreen not_eq getConfig () -> getItem <bool> ("fullscreen"))
	{
		getConfig () -> setItem <bool> ("fullscreen", fullscreen);
		on_fullscreen (fullscreen);
	}

	return false;
}

void
X3DUserInterface::on_maximized (const bool value)
{
	restoreInterface ();
}

void
X3DUserInterface::on_fullscreen (const bool value)
{
	restoreInterface ();
}

bool
X3DUserInterface::on_focus_in_event (GdkEventFocus* event)
{
	getBrowserWindow () -> setAccelerators (false);
	return false;
}

bool
X3DUserInterface::on_focus_out_event (GdkEventFocus* event)
{
	getBrowserWindow () -> setAccelerators (true);
	return false;
}

bool
X3DUserInterface::on_delete_event (GdkEventAny* event)
{
	return quit ();
}

void
X3DUserInterface::setName (const std::string & value)
{
	X3DBaseInterface::setName (value);

	if (initialized)
		store ();

	config .reset (new Configuration (value));

	if (initialized)
		configure ();
}

void
X3DUserInterface::setTransparent (const bool value)
{
	if (value)
	{
		auto visual = getWindow () .get_screen () -> get_rgba_visual ();

		if (not visual)
			visual = getWindow () .get_screen () -> get_system_visual ();

		gtk_widget_set_visual (GTK_WIDGET (getWindow () .gobj ()), visual -> gobj ());
	}
	else
	{
		const auto visual = getWindow () .get_screen () -> get_system_visual ();

		if (visual)
			gtk_widget_set_visual (GTK_WIDGET (getWindow () .gobj ()), visual -> gobj ());
	}
}

void
X3DUserInterface::setTitleBar (Gtk::Window & window, Gtk::Widget & titlebar)
{
	const auto container = titlebar .get_parent ();

	if (container)
	   container -> remove (titlebar);

	window .set_titlebar (titlebar);
}

void
X3DUserInterface::unparent (Gtk::Widget & widget) const
{
	const auto container = widget .get_parent ();

	if (container)
	   container -> remove (widget);
}

void
X3DUserInterface::reparent (Gtk::Box & box, Gtk::Window & window)
{
	getWindow () .set_transient_for (window);

	unparent (getWidget ());

	for (const auto & widget : box .get_children ())
		box .remove (*widget);

	box .pack_start (getWidget (), true, true);
}

void
X3DUserInterface::reparent (Gtk::Overlay & overlay, Gtk::Window & window)
{
	getWindow () .set_transient_for (window);

	unparent (getWidget ());

	overlay .add_overlay (getWidget ());
}

void
X3DUserInterface::present ()
{
	getWindow () .present ();
}

//void
//X3DUserInterface::hide ()
//{
//	getWindow () .hide ();
//}

void
X3DUserInterface::restoreDialogs ()
{
	// Restore dialogs

	auto dialogNames = std::vector <std::string> ();

	basic::split (std::back_inserter (dialogNames), getConfig () -> getItem <std::string> ("dialogs"), ";");

	for (const auto & dialogName : dialogNames)
	{
		if (restorableDialogs .count (dialogName))
			addDialog (dialogName, true);
	}
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
		dialog -> getWindow () .signal_unmap () .connect (sigc::bind (sigc::mem_fun (this, &X3DUserInterface::removeDialog), name), false);

		if (present)
			dialog -> present ();

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
	Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &X3DUserInterface::removeDialogImpl), name), Glib::PRIORITY_HIGH);
}

void
X3DUserInterface::removeDialogImpl (const std::string & name)
{
	dialogs -> erase (name);
}

void
X3DUserInterface::restoreInterface ()
{
	// Restore window size and position

	std::string state;

	if (isMaximized ())
		state = "Maximized";

	if (isFullscreen ())
		state = "Fullscreen";

	if (getConfig () -> hasItem ("x" + state) and getConfig () -> hasItem ("y" + state))
	{
		getWindow () .move (getConfig () -> getItem <int32_t> ("x" + state),
		                    getConfig () -> getItem <int32_t> ("y" + state));
	}

	if (getConfig () -> getItem <int32_t> ("width" + state) > 0 and getConfig () -> getItem <int32_t> ("height" + state) > 0)
	{
		getWindow () .resize (getConfig () -> getItem <int32_t> ("width"  + state),
		                      getConfig () -> getItem <int32_t> ("height" + state));
	}
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

	std::string state;

	if (isMaximized ())
		state = "Maximized";

	if (isFullscreen ())
		state = "Fullscreen";

	int32_t x, y, width, height;

	getWindow () .get_position (x, y);
	getWindow () .get_size (width, height);

	getConfig () -> setItem <int32_t> ("x" + state, x);
	getConfig () -> setItem <int32_t> ("y" + state, y);

	getConfig () -> setItem <int32_t> ("width"  + state, width);
	getConfig () -> setItem <int32_t> ("height" + state, height);

	getWindow () .hide (); // Hide window as last command.
}

bool
X3DUserInterface::quit ()
{
	mapConnection .disconnect ();

	// Save sessions

	if (this == userInterfaces .front ())
		saveInterfaces ();
	else
		saveInterface ();

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

	if (dialogNames .empty ())
		getConfig () -> removeItem ("dialogs");
	else
		getConfig () -> setItem ("dialogs", dialogNames);
}

void
X3DUserInterface::dispose ()
{
	// Save configuration.

	store ();

	// Close dialogs

	for (const auto & pair : *dialogs)
		pair .second -> quit ();

	dialogs -> clear ();

	// Unparent widget, this is need if it was previously reparented. 

	const auto parent = getWidget () .get_parent ();

	if (parent)
		parent -> remove (getWidget ());

	// Call base.

	X3DBaseInterface::dispose ();
}

X3DUserInterface::~X3DUserInterface ()
{
	userInterfaces .erase (userInterface);
}

} // puck
} // titania

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

#include "PrototypeInstanceDialog.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Prototype/X3DProtoDeclarationNode.h>
#include <Titania/OS.h>

namespace titania {
namespace puck {

PrototypeInstanceDialog::PrototypeInstanceDialog (X3DBrowserWindow* const browserWindow) :
	                   X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DPrototypeInstanceDialogInterface (get_ui ("Editors/PrototypeInstanceDialog.xml"), gconf_dir ())
{
	getWindow () .set_modal (false);

	// Find all available proto objects

	const auto protoNodes = getCurrentContext () -> findProtoDeclarations ();

	// Remove all menu items

	for (const auto & widget : getPrototypeMenu () .get_children ())
		getPrototypeMenu () .remove (*widget);

	for (const auto & pair : protoNodes)
	{
		const auto image    = Gtk::manage (new Gtk::Image (Gtk::StockID (pair .second -> isExternproto () ? "ExternProto" : "Prototype"), Gtk::ICON_SIZE_MENU));
		const auto menuItem = Gtk::manage (new Gtk::ImageMenuItem (*image, pair .first));

		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (getPrototypeLabel (), &Gtk::Label::set_text), pair .first));
		menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (getOkButton (), &Gtk::Button::set_sensitive), true));
		menuItem -> show ();

		getPrototypeMenu () .append (*menuItem);
	}

	getPrototypeLabel () .set_text ("");
	getOkButton () .set_sensitive (false);

	setup ();
}

void
PrototypeInstanceDialog::run ()
{
	const auto responseId = getWindow () .run ();

	if (responseId == Gtk::RESPONSE_OK)
	{
		const auto undoStep  = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Create %s"), getPrototypeLabel () .get_text () .c_str ()));
		const auto instance  = getBrowserWindow () -> addPrototypeInstance (getCurrentContext (), getPrototypeLabel () .get_text (), undoStep);

		getBrowserWindow () -> getSelection () -> setChildren ({ instance }, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
	}

	getWindow () .hide ();
}

PrototypeInstanceDialog::~PrototypeInstanceDialog ()
{
	dispose ();
}

} // puck
} // titania

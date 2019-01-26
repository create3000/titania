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

#include "Clipboard.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../X3DBrowser.h"

namespace titania {
namespace X3D {

Clipboard::Fields::Fields () :
	    set_string (new SFString ()),
	        target (new SFString ("STRING")),
	string_changed (new SFString ())
{ }

const Component   Clipboard::component      = Component ("Titania", 1);
const std::string Clipboard::typeName       = "Clipboard";
const std::string Clipboard::containerField = "clipboard";

Clipboard::Clipboard (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     fields ()
{
	addType (X3DConstants::Clipboard);

	addField (inputOnly,   "set_string",     set_string ());
	addField (inputOutput, "target",         target ());
	addField (outputOnly,  "string_changed", string_changed ());
}

Clipboard*
Clipboard::create (X3DExecutionContext* const executionContext) const
{
	return new Clipboard (executionContext);
}

void
Clipboard::initialize ()
{
	X3DBaseNode::initialize ();

	Gtk::Clipboard::get () -> signal_owner_change () .connect (sigc::mem_fun (this, &Clipboard::on_owner_change));

	set_string () .addInterest (&Clipboard::set_string_, this);

	update ();
}

void
Clipboard::update ()
{
	Gtk::Clipboard::get () -> request_targets (sigc::mem_fun (this, &Clipboard::on_received_targets));
}

void
Clipboard::set_string_ ()
{
	Gtk::Clipboard::get () -> set ({ Gtk::TargetEntry (target ()) },
	                                 sigc::mem_fun (this, &Clipboard::on_get), sigc::mem_fun (this, &Clipboard::on_clear));

	string_changed () = set_string ();
}

void
Clipboard::on_get (Gtk::SelectionData & selection_data, guint info)
{
	if (selection_data .get_target () == target ())
		selection_data .set (target (), string_changed ());
}

void
Clipboard::on_clear ()
{
	//This isn't necessary.
}

void
Clipboard::on_received_targets (const std::vector <Glib::ustring> & receivedTargets)
{
	const auto iter = std::find_if (receivedTargets .begin (), receivedTargets .end (),
	                                [&] (const Glib::ustring & receivedTarget)
	                                {
//__LOG__ << receivedTarget << std::endl;
	                                   return receivedTarget == target ();
											  });

	if (iter == receivedTargets .end ())
		return;

	Gtk::Clipboard::get () -> request_contents (target (), sigc::mem_fun (this, &Clipboard::on_received));
}

void
Clipboard::on_received (const Gtk::SelectionData & selection_data)
{
	if (selection_data .get_target () == target ())
		string_changed () = selection_data .get_data_as_string ();
}

void
Clipboard::on_owner_change (GdkEventOwnerChange* event)
{
	update ();
}

void
Clipboard::dispose ()
{
	notify_callbacks ();

	X3DBaseNode::dispose ();
}

} // X3D
} // titania

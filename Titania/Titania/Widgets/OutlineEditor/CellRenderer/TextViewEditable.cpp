/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "TextViewEditable.h"

#include "OutlineFields.h"
#include "../X3DOutlineTreeView.h"

#include <Titania/Chrono/Now.h>
#include <Titania/LOG.h>

namespace titania {
namespace puck {

TextViewEditable::TextViewEditable (X3DOutlineTreeView* const treeView,
                                    const X3D::SFNode & node,
                                    X3D::X3DFieldDefinition* const field,
                                    const Glib::ustring & path,
                                    const bool multiline,
                                    const bool useLocale) :
	   Glib::ObjectBase (typeid (TextViewEditable)),
	X3DTextViewEditable (multiline),
	           treeView (treeView),
	               node (node),
	              field (field),
	               path (path),
	          useLocale (useLocale)
{
	get_textview () .signal_populate_popup () .connect (sigc::mem_fun (this, &TextViewEditable::on_textview_populate_popup));
}

void
TextViewEditable::on_textview_populate_popup (Gtk::Menu* menu)
{
	const auto separator = Gtk::manage (new Gtk::SeparatorMenuItem ());

	separator -> show ();
	menu -> append (*separator);
	
	if (field -> getType () == X3D::X3DConstants::SFBool)
	{
		const auto toggleMenuItem = Gtk::manage (new Gtk::MenuItem (_ ("Toggle Value")));

		toggleMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &TextViewEditable::on_toggle_value));
		toggleMenuItem -> show ();
		menu -> append (*toggleMenuItem);
	}

	if (field -> getType () == X3D::X3DConstants::SFTime)
	{
		const auto toggleMenuItem = Gtk::manage (new Gtk::MenuItem (_ ("Set Current Time")));

		toggleMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &TextViewEditable::on_current_time));
		toggleMenuItem -> show ();
		menu -> append (*toggleMenuItem);
	}

	const auto resetMenuItem = Gtk::manage (new Gtk::MenuItem (_ ("Reset To Default Value")));

	resetMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &TextViewEditable::on_reset_activate));
	resetMenuItem -> show ();
	menu -> append (*resetMenuItem);
}

void
TextViewEditable::on_toggle_value ()
{
	set_text (*static_cast <X3D::SFBool*> (field) ? "FALSE" : "TRUE");
		
	editing_done ();
}

void
TextViewEditable::on_current_time ()
{
	std::ostringstream osstream;

	osstream << X3D::SFTime (chrono::now ());

	set_text (osstream .str ());
		
	editing_done ();
}

void
TextViewEditable::on_reset_activate ()
{
	const auto & scene = treeView -> getCurrentScene ();

	try
	{
		const auto defaultField = node -> getInterfaceDeclaration () -> getField (field -> getName ());

		set_text (puck::get_field_value (scene, defaultField, false, useLocale));
	}
	catch (...)
	{
	   // Custom fields support.

		const auto defaultField = node -> getBrowser () -> getSupportedField (field -> getTypeName ());

		set_text (puck::get_field_value (scene, defaultField, false, useLocale));
	}
		
	editing_done ();
}

void
TextViewEditable::on_remove_widget ()
{
	node .dispose ();

	X3DTextViewEditable::on_remove_widget ();
}

TextViewEditable::~TextViewEditable ()
{ }

} // puck
} // titania

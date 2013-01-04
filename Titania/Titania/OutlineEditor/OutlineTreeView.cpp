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
 ******************************************************************************/

#include "OutlineTreeView.h"

#include "../OutlineEditor/OutlineTreeModel.h"
#include <Titania/X3D.h>

namespace titania {
namespace puck {

OutlineTreeView::OutlineTreeView (const X3D::SFNode <X3D::Browser> & browser) :
	   Gtk::TreeView (), 
	X3DBaseInterface ()  
{
	setBrowser (browser);

	set_headers_visible (true);
	set_enable_search (false);

	Gtk::TreeViewColumn* treeviewcolumn_name = Gtk::manage (new Gtk::TreeViewColumn ("Name"));

	Gtk::CellRendererPixbuf* cellrenderer_icon = Gtk::manage (new Gtk::CellRendererPixbuf ());
	treeviewcolumn_name -> pack_start (*cellrenderer_icon, false);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "pixbuf", 0);
	treeviewcolumn_name -> add_attribute (*cellrenderer_icon, "cell-background-gdk", 2);

	Gtk::CellRendererText* cellrenderer_name = Gtk::manage (new Gtk::CellRendererText ());
	treeviewcolumn_name -> pack_start (*cellrenderer_name, true);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "text", 3);
	treeviewcolumn_name -> add_attribute (*cellrenderer_name, "cell-background-gdk", 2);

	append_column (*treeviewcolumn_name);

	//	Gtk::TreeViewColumn* treeviewcolumn_debug = Gtk::manage (new Gtk::TreeViewColumn ("Debug"));
	//
	//	Gtk::CellRendererText* cellrenderer_debug = Gtk::manage (new Gtk::CellRendererText ());
	//	treeviewcolumn_debug -> pack_start (*cellrenderer_debug, false);
	//	treeviewcolumn_debug -> add_attribute (*cellrenderer_debug, "text", 3);
	//	treeviewcolumn_debug -> add_attribute (*cellrenderer_debug, "cell-background-gdk", 2);
	//
	//	append_column (*treeviewcolumn_debug);

	getBrowser () -> initialized .addInterest (this, &OutlineTreeView::set_world);
}

void
OutlineTreeView::set_world ()
{
	set_model (OutlineTreeModel::create (getBrowser ()));
}

} // puck
} // titania

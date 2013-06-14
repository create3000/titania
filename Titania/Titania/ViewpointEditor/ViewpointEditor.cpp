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

#include "ViewpointEditor.h"

#include "../Configuration/config.h"
#include <iostream>

namespace titania {
namespace puck {

ViewpointEditor::ViewpointEditor (const X3D::SFNode <X3D::Browser> & browser) :
	X3DViewpointEditorUI (get_ui ("ViewpointEditor.ui"), gconf_dir ()) 
{
	setBrowser (browser);
}

void
ViewpointEditor::initialize ()
{
	X3DViewpointEditorUI::initialize ();

	getBrowser () -> initialized .addInterest (this, &ViewpointEditor::set_world);

	if (getBrowser () -> initialized)
		set_world ();
}

X3D::ViewpointList &
ViewpointEditor::getViewpoints ()
{
	return getBrowser () -> getExecutionContext () -> getActiveLayer () -> getViewpoints ();
}

void
ViewpointEditor::set_world ()
{
	//	getBrowser () -> getActiveViewpoint () .addInterest (this, &ViewpointEditor::set_currentViewpoint);

	getViewpoints () .addInterest (this, &ViewpointEditor::set_viewpoints);

	set_viewpoints ();
}

void
ViewpointEditor::set_viewpoints ()
{
	// Clear
	getListStore () -> clear ();

	// Fill the TreeView's model
	for (const auto & viewpoint : getViewpoints ())
	{
		if (viewpoint -> description () .length ())
			getListStore () -> append () -> set_value (0, viewpoint -> description () .getValue ());
	}

	getTreeView () .queue_draw ();
}

void
ViewpointEditor::set_currentViewpoint ()
{
	auto viewpoint = dynamic_cast <X3D::Viewpoint*> (getBrowser () -> getActiveViewpoint ());

	if (viewpoint)
		getFieldOfView () .set_value (viewpoint -> fieldOfView ());
}

void
ViewpointEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	auto viewpoint = getViewpoints () [path .front ()];
	
	viewpoint -> resetUserOffsets ();
	
	viewpoint -> set_bind () = true;
}

void
ViewpointEditor::on_fieldOfView_changed ()
{
	auto viewpoint = dynamic_cast <X3D::Viewpoint*> (getBrowser () -> getActiveViewpoint ());

	if (viewpoint)
		viewpoint -> fieldOfView () = getFieldOfView () .get_value ();
}

ViewpointEditor::~ViewpointEditor ()
{
	getViewpoints () .removeInterest (this, &ViewpointEditor::set_viewpoints);
}

} // puck
} // titania

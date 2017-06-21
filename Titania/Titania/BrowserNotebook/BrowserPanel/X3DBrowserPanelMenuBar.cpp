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

#include "X3DBrowserPanelMenuBar.h"

#include "../NotebookPage/NotebookPage.h"
#include "ViewpointObserver.h"

#include <Titania/X3D/Browser/BrowserOptions.h>

namespace titania {
namespace puck {

X3DBrowserPanelMenuBar::X3DBrowserPanelMenuBar () :
	   X3DBrowserPanel (),
	           browser (getPage () -> getMainBrowser ()),
	 viewpointObserver (),
	          changing (false)
{
	addChildObjects (browser);
}

void
X3DBrowserPanelMenuBar::initialize ()
{
	getPage () -> getMainBrowser () -> signal_map ()   .connect (sigc::mem_fun (this, &X3DBrowserPanelMenuBar::on_main_browser_mapped));
	getPage () -> getMainBrowser () -> signal_unmap () .connect (sigc::mem_fun (this, &X3DBrowserPanelMenuBar::on_main_browser_mapped));

	on_main_browser_mapped ();

	getStraightenHorizonMenuItem () .set_active (getConfig () -> get <bool> ("straightenHorizon"));
}

void
X3DBrowserPanelMenuBar::setLocalBrowser (const X3D::BrowserPtr & value)
{
	browser -> getBrowserOptions () -> Shading () .removeInterest (&X3DBrowserPanelMenuBar::set_shading, this);
	browser -> getViewer ()            .removeInterest (&X3DBrowserPanelMenuBar::set_viewer, this);
	browser -> getStraightenHorizon () .removeInterest (&X3DBrowserPanelMenuBar::set_straighten_horizon, this);

	browser = value;

	browser -> getBrowserOptions () -> Shading () .addInterest (&X3DBrowserPanelMenuBar::set_shading, this);
	browser -> getViewer ()            .addInterest (&X3DBrowserPanelMenuBar::set_viewer, this);
	browser -> getStraightenHorizon () .addInterest (&X3DBrowserPanelMenuBar::set_straighten_horizon, this);

	viewpointObserver .reset (new ViewpointObserver (getBrowserWindow (), browser));
	viewpointObserver -> getUndoHistory () .addInterest (&X3DBrowserPanelMenuBar::set_undoHistory, this);

	set_undoHistory ();
	set_shading (browser -> getBrowserOptions () -> Shading ());
	set_viewer ();
	set_straighten_horizon ();
}

void
X3DBrowserPanelMenuBar::on_main_browser_mapped ()
{
	getMainViewMenuItem ()          .set_sensitive (not getPage () -> getMainBrowser () -> get_mapped ());
	getMainViewSeparatorMenuItem () .set_sensitive (not getPage () -> getMainBrowser () -> get_mapped ());
}

void
X3DBrowserPanelMenuBar::on_map ()
{
	getBrowserWindow () -> getEditing () .addInterest (&X3DBrowserPanelMenuBar::set_editing, this);

	set_editing ();
}

void
X3DBrowserPanelMenuBar::on_unmap ()
{
	getBrowserWindow () -> getEditing () .addInterest (&X3DBrowserPanelMenuBar::set_editing, this);
}

void
X3DBrowserPanelMenuBar::set_editing ()
{
	getMenuBar () .set_visible (getBrowserWindow () -> getEditing ());
}

void
X3DBrowserPanelMenuBar::set_undoHistory ()
{
	const auto & undoHistory = viewpointObserver -> getUndoHistory () ;

	getUndoViewMenuItem () .set_sensitive (undoHistory .hasUndo ());
	getRedoViewMenuItem () .set_sensitive (undoHistory .hasRedo ());
}

void
X3DBrowserPanelMenuBar::undo_view_activate ()
{
	viewpointObserver -> getUndoHistory () .undo ();
}

void
X3DBrowserPanelMenuBar::redo_view_activate ()
{
	viewpointObserver -> getUndoHistory () .redo ();
}

void
X3DBrowserPanelMenuBar::set_viewer ()
{
	getStraightenHorizonMenuItem ()          .set_visible (getLocalBrowser () -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
	getStraightenHorizonSeparatorMenuItem () .set_visible (getLocalBrowser () -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
}

void
X3DBrowserPanelMenuBar::set_straighten_horizon ()
{
	changing = true;

	getStraightenHorizonMenuItem () .set_active (getLocalBrowser () -> getStraightenHorizon ());

	changing = false;
}

void
X3DBrowserPanelMenuBar::on_straighten_horizon_toggled ()
{
	if (changing)
		return;

	getConfig () -> set <bool> ("straightenHorizon", getStraightenHorizonMenuItem () .get_active ());
	getLocalBrowser () -> setStraightenHorizon (getStraightenHorizonMenuItem () .get_active ());

	if (getStraightenHorizonMenuItem () .get_active ())
		on_straighten_clicked ();
}

void
X3DBrowserPanelMenuBar::on_straighten_clicked ()
{
	const auto & activeLayer = getLocalBrowser () -> getActiveLayer ();

	if (activeLayer)
		activeLayer -> getViewpoint () -> straighten (getLocalBrowser () -> getCurrentViewer () == X3D::X3DConstants::ExamineViewer);
}

void
X3DBrowserPanelMenuBar::on_look_at_selection_activate ()
{
	getLocalBrowser () -> lookAtSelection ();
}

void
X3DBrowserPanelMenuBar::on_look_at_all_activate ()
{
	getLocalBrowser () -> lookAtAllObjectsInActiveLayer ();
}

void
X3DBrowserPanelMenuBar::on_reset_user_offsets_activate ()
{
	if (not getLocalBrowser () -> getActiveLayer ())
		return;

	const auto viewpoint = getLocalBrowser () -> getActiveLayer () -> getViewpoint ();

	viewpoint -> transitionStart (viewpoint);
}

void
X3DBrowserPanelMenuBar::on_phong_toggled ()
{
	if (getPhongMenuItem () .get_active ())
		on_shading_changed ("PHONG");
}

void
X3DBrowserPanelMenuBar::on_gouraud_toggled ()
{
	if (getGouraudMenuItem () .get_active ())
		on_shading_changed ("GOURAUD");
}

void
X3DBrowserPanelMenuBar::on_flat_toggled ()
{
	if (getFlatMenuItem () .get_active ())
		on_shading_changed ("FLAT");
}

void
X3DBrowserPanelMenuBar::on_wireframe_toggled ()
{
	if (getWireframeMenuItem () .get_active ())
		on_shading_changed ("WIREFRAME");
}

void
X3DBrowserPanelMenuBar::on_pointset_toggled ()
{
	if (getPointsetMenuItem () .get_active ())
		on_shading_changed ("POINTSET");
}

void
X3DBrowserPanelMenuBar::on_shading_changed (const std::string & value)
{
	if (changing)
		return;

	getLocalBrowser () -> getBrowserOptions () -> Shading () .removeInterest (&X3DBrowserPanelMenuBar::set_shading, this);
	getLocalBrowser () -> getBrowserOptions () -> Shading () .addInterest (&X3DBrowserPanelMenuBar::connectShading, this);

	getLocalBrowser () -> getBrowserOptions () -> Shading () = value;
}

void
X3DBrowserPanelMenuBar::set_shading (const X3D::SFString & value)
{
	changing = true;

	if (value == "PHONG")
		getPhongMenuItem () .set_active (true);

	else if (value == "FLAT")
		getFlatMenuItem () .set_active (true);

	else if (value == "WIREFRAME")
		getWireframeMenuItem () .set_active (true);

	else if (value == "POINTSET")
		getPointsetMenuItem () .set_active (true);

	else
		getGouraudMenuItem () .set_active (true);

	changing = false;
}

void
X3DBrowserPanelMenuBar::connectShading (const X3D::SFString & field)
{
	field .removeInterest (&X3DBrowserPanelMenuBar::connectShading, this);
	field .addInterest (&X3DBrowserPanelMenuBar::set_shading, this);
}

void
X3DBrowserPanelMenuBar::on_main_view_activate ()
{
	setType (BrowserPanelType::MAIN);
}

void
X3DBrowserPanelMenuBar::on_perspective_view_activate ()
{
	setType (BrowserPanelType::PERSPECTIVE);
}

void
X3DBrowserPanelMenuBar::on_top_view_activate ()
{
	setType (BrowserPanelType::TOP);
}

void
X3DBrowserPanelMenuBar::on_right_view_activate ()
{
	setType (BrowserPanelType::RIGHT);
}

void
X3DBrowserPanelMenuBar::on_front_view_activate ()
{
	setType (BrowserPanelType::FRONT);
}

void
X3DBrowserPanelMenuBar::on_bottom_view_activate ()
{
	setType (BrowserPanelType::BOTTOM);
}

void
X3DBrowserPanelMenuBar::on_left_view_activate ()
{
	setType (BrowserPanelType::LEFT);
}

void
X3DBrowserPanelMenuBar::on_back_view_activate ()
{
	setType (BrowserPanelType::BACK);
}

void
X3DBrowserPanelMenuBar::dispose ()
{
	viewpointObserver .reset ();
}

X3DBrowserPanelMenuBar::~X3DBrowserPanelMenuBar ()
{ }

} // puck
} // titania

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

#include "BrowserPanel.h"

#include "../NotebookPage/NotebookPage.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Editors/GridEditor/AngleGridTool.h"
#include "../../Editors/GridEditor/AxonometricGridTool.h"
#include "../../Editors/GridEditor/GridTool.h"
#include "../../Revealer/GeometryEditor/GeometryEditor.h"

#include <Titania/X3D/Browser/Navigation/PlaneViewer.h>
#include <Titania/X3D/Browser/Selection.h>
#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Execution/BindableNodeStack.h>

#include <Titania/X3D/Tools/Grids/GridTool.h>
#include <Titania/X3D/Tools/Grids/AngleGridTool.h>
#include <Titania/X3D/Tools/Grids/AxonometricGridTool.h>

namespace titania {
namespace puck {

BrowserPanel::BrowserPanel (X3DBrowserWindow* const browserWindow, NotebookPage* const page, const size_t id) :
	        X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DBrowserPanelInterface (get_ui ("Panels/BrowserPanel.glade"), page, PanelType::BROWSER_PANEL, id),
	         X3DBrowserPanel (),
	  X3DBrowserPanelMenuBar ()
{
	setName ("BrowserPanel" + basic::to_string (id, std::locale::classic ()));

	setup ();
}

void
BrowserPanel::initialize ()
{
	X3DBrowserPanel::initialize ();
	X3DBrowserPanelMenuBar::initialize ();
}

void
BrowserPanel::setLocalBrowser (const X3D::BrowserPtr & value)
{
	X3DBrowserPanel::setLocalBrowser (value);
	X3DBrowserPanelMenuBar::setLocalBrowser (value);
}

void
BrowserPanel::on_map ()
{
	X3DBrowserPanel::on_map ();
	X3DBrowserPanelMenuBar::on_map ();
}

void
BrowserPanel::on_unmap ()
{
	X3DBrowserPanel::on_unmap ();
	X3DBrowserPanelMenuBar::on_unmap ();
}

void
BrowserPanel::dispose ()
{
	X3DBrowserPanelMenuBar::dispose ();
	X3DBrowserPanel::dispose ();
}

BrowserPanel::~BrowserPanel ()
{
	dispose ();
}

} // puck
} // titania

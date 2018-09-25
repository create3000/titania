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

#include "X3DBrowserWindow.h"

#include "../Browser/BrowserSelection.h"
#include "../Browser/MagicImport.h"

#include "../Editors/GridEditor/AngleGridTool.h"
#include "../Editors/GridEditor/AxonometricGridTool.h"
#include "../Editors/GridEditor/GridTool.h"

#include "../Revealer/GeometryEditor/GeometryEditor.h"

#include "../Widgets/Console/Console.h"
#include "../Widgets/Footer/Footer.h"
#include "../Widgets/OutlineEditor/OutlineEditor.h"
#include "../Widgets/OutlineEditor/OutlineTreeModel.h"
#include "../Widgets/OutlineEditor/OutlineTreeViewEditor.h"
#include "../Widgets/ScriptEditor/ScriptEditor.h"
#include "../Widgets/Sidebar/Sidebar.h"

#include <Titania/X3D/Browser/Core/Clipboard.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Geometry3D/IndexedFaceSet.h>
#include <Titania/X3D/Execution/World.h>

namespace titania {
namespace puck {

X3DBrowserWindow::X3DBrowserWindow (const X3D::BrowserPtr & defaultBrowser) :
	   X3DBrowserEditor (defaultBrowser),
	     geometryEditor (new GeometryEditor (this)),
	            sidebar (new Sidebar (this)),
	             footer (new Footer (this)),
	           gridTool (new GridTool (this)),
	      angleGridTool (new AngleGridTool (this)),
	axonometricGridTool (new AxonometricGridTool (this)),
	               keys (),
	       accelerators (true)
{ }  

void
X3DBrowserWindow::initialize ()
{
	X3DBrowserEditor::initialize ();

	geometryEditor -> getWidget () .property_reveal_child () .signal_changed () .connect (sigc::mem_fun (this, &X3DBrowserWindow::on_geometry_editor_reveal_child_changed));

	geometryEditor -> reparent (getBrowserOverlay (), getWindow ());
	sidebar        -> reparent (getSidebarBox (),     getWindow ());
	footer         -> reparent (getFooterBox (),      getWindow ());
}

void
X3DBrowserWindow::configure ()
{
	X3DBrowserEditor::configure ();

	geometryEditor -> getWidget () .set_reveal_child (getConfig () -> getItem <bool> ("geometryEditor"));
}

// Menu

void
X3DBrowserWindow::setAccelerators (const bool value)
{
	accelerators = value;

	if (value)
		getWindow () .add_accel_group (getAccelGroup ());
	else
		getWindow () .remove_accel_group (getAccelGroup ());
}

const std::shared_ptr <OutlineTreeViewEditor> &
X3DBrowserWindow::getOutlineTreeView () const
{
	return sidebar -> getPage <OutlineEditor> ("OutlineEditor") -> getTreeView ();
}

bool
X3DBrowserWindow::save (const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	const auto scriptEditor = footer -> getPage <ScriptEditor> ("ScriptEditor");

	if (scriptEditor -> getModified ())
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Apply Script"));

		scriptEditor -> apply (undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}

	return X3DBrowserEditor::save (worldURL, outputStyle, copy);
}

void
X3DBrowserWindow::expandNodes (const X3D::MFNode & nodes)
{
	if (getConfig () -> getItem <bool> ("followPrimarySelection"))
	{
		Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &X3DBrowserWindow::expandNodesImpl), nodes));
	}
}

void
X3DBrowserWindow::expandNodesImpl (const X3D::MFNode & nodes)
{
	std::vector <Gtk::TreePath> paths;

	for (const auto & node : nodes)
	{
		getOutlineTreeView () -> expand_to (node);

		for (const auto & iter : getOutlineTreeView () -> get_iters (node))
		{
			paths .emplace_back (getOutlineTreeView ()-> get_model () -> get_path (iter));
			break;
		}
	}

	for (const auto & path : paths)
		getOutlineTreeView () -> expand_row (path, false);

	if (paths .empty ())
		return;

	using ScrollToRow = void (OutlineTreeViewEditor::*) (const Gtk::TreePath &, float);

	Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (getOutlineTreeView () .get (), (ScrollToRow) &OutlineTreeViewEditor::scroll_to_row), paths .front (), 2 - math::phi <double>));
}

void
X3DBrowserWindow::on_geometry_editor_clicked ()
{
	geometryEditor -> getWidget () .set_reveal_child (not geometryEditor -> getWidget () .get_reveal_child ());
}

void
X3DBrowserWindow::on_geometry_editor_reveal_child_changed ()
{
	getConfig () -> setItem ("geometryEditor", geometryEditor -> getWidget () .get_reveal_child ());
}

void
X3DBrowserWindow::store ()
{
	X3DBrowserEditor::store ();
}

void
X3DBrowserWindow::dispose ()
{
	geometryEditor      .reset ();
	sidebar             .reset ();
	footer              .reset ();
	gridTool            .reset ();
	angleGridTool       .reset ();
	axonometricGridTool .reset ();

	X3DBrowserEditor::dispose ();
}

X3DBrowserWindow::~X3DBrowserWindow ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania

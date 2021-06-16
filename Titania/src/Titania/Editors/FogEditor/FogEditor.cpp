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

#include "FogEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Editors/NodeIndex/NodeIndex.h"

#include <Titania/X3D/Components/EnvironmentalEffects/Fog.h>
#include <Titania/X3D/Components/EnvironmentalEffects/LocalFog.h>

namespace titania {
namespace puck {

FogEditor::FogEditor (X3DBrowserWindow* const browserWindow) :
	     X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DFogEditorInterface (get_ui ("Editors/FogEditor.glade")),
	              fogList (new FogList (browserWindow)),
	             nodeName (this, getNameEntry (), getRenameButton ()),
	                color (this, getColorButton (), getColorAdjustment (), getColorBox (), "color"),
	              fogType (this, getFogTypeComboBoxText (), "fogType"),
	      visibilityRange (this, getVisibilityRangeAdjustment (), getVisibilityRangeSpinButton (), "visibilityRange"),
                 fogNode ()
{
	addChildObjects (fogNode);

	setup ();
}

void
FogEditor::initialize ()
{
	X3DFogEditorInterface::initialize ();

	fogList -> getSelection () .addInterest (&FogEditor::set_fog, this);

	fogList -> setEditing (true);
	fogList -> getHeaderNotebook () .set_current_page (1);
	fogList -> getHeaderBar () .set_visible (false);
	fogList -> getTreeView () .set_headers_visible (true);
	fogList -> reparent (getFogListBox (), getWindow ());

	set_fog (nullptr);
}

void
FogEditor::configure ()
{
	X3DFogEditorInterface::configure ();

	if (getConfig () -> hasItem ("paned"))
		getPaned () .set_position (getConfig () -> getItem <int32_t> ("paned"));
}

void
FogEditor::set_fog (const X3D::X3DPtr <X3D::Fog> & value)
{
	fogNode = value;

	const bool inScene = (fogNode and fogNode -> getScene () == getCurrentScene () and not inPrototypeInstance ());
	const auto nodes   = fogNode ? X3D::MFNode ({ fogNode }) : X3D::MFNode ();

	getRemoveFogButton () .set_sensitive (inScene and fogNode);
	getFogBox ()          .set_sensitive (inScene);

	nodeName        .setNode  (fogNode);
	color           .setNodes (nodes);
	fogType         .setNodes (nodes);
	visibilityRange .setNodes (nodes);
}

void
FogEditor::on_new_fog_clicked ()
{
	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Create New Fog"));
	const auto node     = X3D::X3DPtr <X3D::Fog> (X3D::X3DEditor::createNode (getCurrentWorld (), getCurrentContext (), "Fog", undoStep));

	node -> set_bind () = true;

	getBrowserWindow () -> getSelection () -> setNodes ({ node }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	fogList -> setSelection (node, true);
}

void
FogEditor::on_remove_fog_clicked ()
{
	const auto   undoStep         = std::make_shared <X3D::UndoStep> (_ ("Remove Fog"));
	const auto & fogNode          = nodeName .getNode ();
	const auto   executionContext = X3D::X3DExecutionContextPtr (fogNode -> getExecutionContext ());

	X3D::X3DEditor::removeNodesFromScene (executionContext, { fogNode }, true, undoStep);

	getBrowserWindow () -> getSelection () -> removeNodes ({ fogNode }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	set_fog (nullptr);
}

void
FogEditor::store ()
{
	getConfig () -> setItem ("paned", getPaned () .get_position ());

	X3DFogEditorInterface::store ();
}

FogEditor::~FogEditor ()
{
	dispose ();
}

} // puck
} // titania

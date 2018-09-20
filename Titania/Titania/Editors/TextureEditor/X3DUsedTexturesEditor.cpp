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

#include "X3DUsedTexturesEditor.h"

#include "../../Browser/IconFactory.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"
#include "../../Editors/NodeIndex/NodeIndex.h"

#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Texturing/X3DTexture2DNode.h>
#include <Titania/X3D/Components/Shape/TwoSidedMaterial.h>

namespace titania {
namespace puck {

static constexpr size_t IMAGE_SIZE = 48;
static constexpr size_t ICON_SIZE  = Gtk::ICON_SIZE_DIALOG;

X3DUsedTexturesEditor::X3DUsedTexturesEditor () :
	X3DTextureEditorInterface (),
	                  preview (X3D::createBrowser (getMasterBrowser (), { get_ui ("Editors/TexturePreview.x3dv") })),
	                nodeIndex (new NodeIndex (getBrowserWindow ()))
{
	addChildObjects (preview);

	nodeIndex -> setName ("UsedTexturesEditor." + nodeIndex -> getName ());
}

void
X3DUsedTexturesEditor::initialize ()
{
	// Off-Screen Browser

	preview -> initialized () .addInterest (&X3DUsedTexturesEditor::set_initialized, this);
	preview -> setFixedPipeline (false);
	preview -> setup ();

	// Selection

	getTexture () .addInterest (&X3DUsedTexturesEditor::set_texture, this);

	// Node index

	nodeIndex -> getNode () .addInterest (&X3DUsedTexturesEditor::set_node, this);
	nodeIndex -> reparent (getUsedTexturesBox (), getWindow ());
	nodeIndex -> setShowWidget (true);
	nodeIndex -> setSelect (false);
	nodeIndex -> setObserveNodes (true);
	nodeIndex -> setTypes ({ X3D::X3DConstants::X3DTextureNode });

	// Tree view column

	nodeIndex -> getListStore () -> signal_row_inserted () .connect (sigc::mem_fun (this, &X3DUsedTexturesEditor::on_row_changed));
	nodeIndex -> getListStore () -> signal_row_changed ()  .connect (sigc::mem_fun (this, &X3DUsedTexturesEditor::on_row_changed));
	nodeIndex -> getImageColumn () -> set_visible (true);
	nodeIndex -> getCellRendererImage () -> property_stock_size () = ICON_SIZE;
}

void
X3DUsedTexturesEditor::set_initialized ()
{
	for (size_t i = 0, size = nodeIndex -> getNodes () .size (); i < size; ++ i)
		nodeIndex -> updateRow (i);
}

void
X3DUsedTexturesEditor::set_texture ()
{
	nodeIndex -> setSelection (getTexture ());
	nodeIndex -> scrollToRow (getTexture ());
}

void
X3DUsedTexturesEditor::on_row_changed (const Gtk::TreePath & path, const Gtk::TreeIter & iter)
{
	try
	{
		// Check.

		if (path .size () > 1)
			return;

		const auto index = path .back ();

		// Configure scene.

		const auto appearance = preview -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		appearance -> texture () = nodeIndex -> getNodes () .at (index);

		set_camera (index);

		// Create Icon.

		getBrowserWindow () -> getIconFactory () -> createIcon (nodeIndex -> getName () + basic::to_string (index),
		                                                        preview -> getSnapshot (IMAGE_SIZE, IMAGE_SIZE, false, 8));
	}
	catch (const std::exception & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DUsedTexturesEditor::set_camera (const size_t index)
{
	const X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (nodeIndex -> getNodes () .at (index));

	if (texture2DNode)
		set_camera (texture2DNode -> width (), texture2DNode -> height ());

	else
		set_camera (512, 512);
}

void
X3DUsedTexturesEditor::set_camera (double width, double height)
{
	const X3D::X3DPtr <X3D::OrthoViewpoint> viewpoint (preview -> getExecutionContext () -> getNamedNode ("Texture2DViewpoint"));
	const X3D::X3DPtr <X3D::Transform>      transform (preview -> getExecutionContext () -> getNamedNode ("Texture2D"));

	if (not width or not height)
	{
		width  = 1;
		height = 1;
	}

	viewpoint -> fieldOfView () = { -width, -height, width, height };
	transform -> scale ()       = X3D::Vector3f (width, height, 1);
}

void
X3DUsedTexturesEditor::set_node (const X3D::SFNode & node)
{
	try
	{
		// Apply selected material to selection.

		const auto selection = getBrowserWindow () -> getSelection () -> getNodes ();

		if (selection .empty ())
			return;

		const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Apply Texture From Used Textures Index"));
		const auto appearances      = getNodes <X3D::Appearance> (selection, { X3D::X3DConstants::Appearance });
		const auto executionContext = X3D::MakePtr (getSelectionContext (appearances, true));

		if (not executionContext)
			return;

		for (const auto & appearance : appearances)
			X3D::X3DEditor::replaceNode (executionContext, appearance, appearance -> texture (), node, undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

X3DUsedTexturesEditor::~X3DUsedTexturesEditor ()
{ }

} // puck
} // titania

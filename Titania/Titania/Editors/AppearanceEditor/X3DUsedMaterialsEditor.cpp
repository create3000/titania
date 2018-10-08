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

#include "X3DUsedMaterialsEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/IconFactory.h"
#include "../../Configuration/config.h"
#include "../../Editors/NodeIndex/NodeIndex.h"

#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Shape/TwoSidedMaterial.h>

namespace titania {
namespace puck {

static constexpr size_t IMAGE_SIZE = 48;
static constexpr size_t ICON_SIZE  = Gtk::ICON_SIZE_DIALOG;

X3DUsedMaterialsEditor::X3DUsedMaterialsEditor () :
	X3DAppearanceEditorInterface (),
	                     preview (X3D::createBrowser (getMasterBrowser (), { get_ui ("Editors/MaterialEditorPreview.x3dv") + "#CloseViewpoint" }, { })),
	                   nodeIndex (new NodeIndex (getBrowserWindow ()))
{
	addChildObjects (preview);

	nodeIndex -> setName ("UsedMaterialsEditor." + nodeIndex -> getName ());
}

void
X3DUsedMaterialsEditor::initialize ()
{
	// Off-Screen Browser

	preview -> initialized () .addInterest (&X3DUsedMaterialsEditor::set_initialized, this);
	preview -> setFixedPipeline (false);
	preview -> setAntialiasing (4);
	preview -> setup ();

	// Selection

	getMaterial () .addInterest (&X3DUsedMaterialsEditor::set_material, this);

	// Node index

	nodeIndex -> getNode () .addInterest (&X3DUsedMaterialsEditor::set_node, this);
	nodeIndex -> reparent (getUsedMaterialsBox (), getWindow ());
	nodeIndex -> setShowWidget (true);
	nodeIndex -> setSelect (false);
	nodeIndex -> setObserveNodes (true);
	nodeIndex -> setTypes ({ X3D::X3DConstants::X3DMaterialNode });

	// Tree view column

	nodeIndex -> getListStore () -> signal_row_inserted () .connect (sigc::mem_fun (this, &X3DUsedMaterialsEditor::on_row_changed));
	nodeIndex -> getListStore () -> signal_row_changed ()  .connect (sigc::mem_fun (this, &X3DUsedMaterialsEditor::on_row_changed));
	nodeIndex -> getImageColumn () -> set_visible (true);
	nodeIndex -> getCellRendererImage () -> property_stock_size () = ICON_SIZE;

	nodeIndex -> getTreeModelSort () -> set_sort_func (NodeIndex::Columns::IMAGE, sigc::mem_fun (this, &X3DUsedMaterialsEditor::on_compare_image));
}

void
X3DUsedMaterialsEditor::set_initialized ()
{
	try
	{
		const X3D::X3DPtr <X3D::Appearance> appearance (preview -> getExecutionContext () -> getNamedNode ("Appearance"));
		const X3D::X3DPtr <X3D::Appearance> lineAppearance (preview -> getExecutionContext () -> getNamedNode ("LineAppearance"));
	
		appearance -> setPrivate (true);
		lineAppearance -> setPrivate (true);
	
		for (size_t i = 0, size = nodeIndex -> getNodes () .size (); i < size; ++ i)
			nodeIndex -> updateRow (i);
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

int
X3DUsedMaterialsEditor::on_compare_image (const Gtk::TreeModel::iterator & lhs, const Gtk::TreeModel::iterator & rhs)
{
	try
	{
		uint32_t lhsIndex = 0;
		uint32_t rhsIndex = 0;
	
		lhs -> get_value (NodeIndex::Columns::INDEX, lhsIndex);
		rhs -> get_value (NodeIndex::Columns::INDEX, rhsIndex);
	
		const auto & lhsColor = nodeIndex -> getNodes () .at (lhsIndex) -> getField <X3D::SFColor> ("diffuseColor");
		const auto & rhsColor = nodeIndex -> getNodes () .at (rhsIndex) -> getField <X3D::SFColor> ("diffuseColor");
		
		if (lhsColor == rhsColor)
			return 0;
	
		return lhsColor .getHSV () < rhsColor .getHSV () ? -1 : 1;
	}
	catch (const std::exception &)
	{
		return -1;
	}
}

void
X3DUsedMaterialsEditor::set_material ()
{
	nodeIndex -> setSelection (getMaterial ());
	nodeIndex -> scrollToRow (getMaterial ());
}

void
X3DUsedMaterialsEditor::on_row_changed (const Gtk::TreePath & path, const Gtk::TreeIter & iter)
{
	try
	{
		// Check.

		if (path .size () > 1)
			return;

		const auto index = path .back ();

		// Configure scene.

		const X3D::X3DPtr <X3D::Material>         material (nodeIndex -> getNodes () .at (index));
		const X3D::X3DPtr <X3D::TwoSidedMaterial> twoSidedMaterial (nodeIndex -> getNodes () .at (index));
		const X3D::X3DPtr <X3D::Appearance>       appearance (preview -> getExecutionContext () -> getNamedNode ("Appearance"));

		if (not (material or twoSidedMaterial) or not appearance)
			return;

		if (material)
			appearance -> material () = material;
	
		else if (twoSidedMaterial)
			appearance -> material () = twoSidedMaterial;

		const X3D::X3DPtr <X3D::Material> backMaterial (preview -> getExecutionContext () -> getNamedNode ("BackMaterial"));

		if (backMaterial and twoSidedMaterial)
		{
			backMaterial -> ambientIntensity () = twoSidedMaterial -> backAmbientIntensity ();
			backMaterial -> diffuseColor ()     = twoSidedMaterial -> backDiffuseColor ();
			backMaterial -> specularColor ()    = twoSidedMaterial -> backSpecularColor ();
			backMaterial -> emissiveColor ()    = twoSidedMaterial -> backEmissiveColor ();
			backMaterial -> shininess ()        = twoSidedMaterial -> backShininess ();
			backMaterial -> transparency ()     = twoSidedMaterial -> backTransparency ();
		}

		const X3D::X3DPtr <X3D::Switch> sphere (preview -> getExecutionContext () -> getNamedNode ("Sphere"));

		sphere -> whichChoice () = twoSidedMaterial;

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
X3DUsedMaterialsEditor::set_node (const X3D::SFNode & node)
{
	try
	{
		// Apply selected material to selection.

		const auto selection = getBrowserWindow () -> getSelection () -> getNodes ();

		if (selection .empty ())
			return;

		const auto undoStep         = std::make_shared <X3D::UndoStep> (_ ("Apply Material From Used Materials Index"));
		const auto appearances      = getNodes <X3D::Appearance> (selection, { X3D::X3DConstants::Appearance });
		const auto executionContext = X3D::MakePtr (getSelectionContext (appearances, true));

		for (const auto & appearance : appearances)
			X3D::X3DEditor::replaceNode (executionContext, appearance, appearance -> material (), node, undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

X3DUsedMaterialsEditor::~X3DUsedMaterialsEditor ()
{ }

} // puck
} // titania

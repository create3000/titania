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
#include "../../Dialogs/NodeIndex/NodeIndex.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Browser/BrowserCellRenderer.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Shape/TwoSidedMaterial.h>

namespace titania {
namespace puck {

X3DUsedMaterialsEditor::X3DUsedMaterialsEditor () :
	X3DAppearanceEditorInterface (),
	                     preview (X3D::createBrowser (getMasterBrowser (), { get_ui ("Editors/MaterialEditorPreview.x3dv") + "#CloseViewpoint" }, { })),
	                   nodeIndex (new NodeIndex (getBrowserWindow ())),
	                cellrenderer (Gtk::manage (new X3D::BrowserCellRenderer ()))
{
	addChildObjects (preview);
}

void
X3DUsedMaterialsEditor::initialize ()
{
	preview -> setFixedPipeline (false);
	preview -> setAntialiasing (4);
	preview -> set_opacity (0);
	preview -> show ();

	getUsedMaterialsBrowserBox () .pack_start (*preview, true, true);

	// Node index

	nodeIndex -> getNode () .addInterest (&X3DUsedMaterialsEditor::set_node, this);
	nodeIndex -> reparent (getUsedMaterialsIndexBox (), getWindow ());
	nodeIndex -> setShowWidget (true);
	nodeIndex -> setSelect (false);
	nodeIndex -> setObserveNodes (true);
	nodeIndex -> setTypes ({ X3D::X3DConstants::X3DMaterialNode });

	// Tree view column

	cellrenderer -> property_callback () .set_value (std::bind (&X3DUsedMaterialsEditor::on_render_node, this));

	nodeIndex -> getCustomImageColumn () -> set_visible (true);
	nodeIndex -> getCustomImageColumn () -> pack_start (*cellrenderer, false);
	nodeIndex -> getCustomImageColumn () -> add_attribute (*cellrenderer, "index", nodeIndex -> getIndexColumn ());
}

X3D::Browser*
X3DUsedMaterialsEditor::on_render_node ()
{
	try
	{
		const auto index = cellrenderer -> property_index () .get_value ();

		const X3D::X3DPtr <X3D::Material>         material (nodeIndex -> getNodes () .at (index));
		const X3D::X3DPtr <X3D::TwoSidedMaterial> twoSidedMaterial (nodeIndex -> getNodes () .at (index));
		const X3D::X3DPtr <X3D::Appearance>       appearance (preview -> getExecutionContext () -> getNamedNode ("Appearance"));

		if (not (material or twoSidedMaterial) or not appearance)
			return preview;

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

		if (sphere)
			sphere -> whichChoice () = twoSidedMaterial;
	}
	catch (const std::exception & error)
	{ 
		__LOG__ << error .what () << std::endl;
	}

	return preview;
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

		const auto undoStep    = std::make_shared <X3D::UndoStep> (_ ("Apply Material From Used Materials Index"));
		const auto appearances = getNodes <X3D::Appearance> (selection, { X3D::X3DConstants::Appearance });

		for (const auto & appearance : appearances)
			getBrowserWindow () -> replaceNode (getCurrentContext (), appearance, appearance -> material (), node, undoStep);

		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

X3DUsedMaterialsEditor::~X3DUsedMaterialsEditor ()
{ }

} // puck
} // titania

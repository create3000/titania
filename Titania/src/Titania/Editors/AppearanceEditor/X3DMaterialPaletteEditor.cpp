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

#include "X3DMaterialPaletteEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/MagicImport.h"

#include <Titania/X3D/Components/Geometry3D/Extrusion.h>
#include <Titania/X3D/Components/Geometry3D/Sphere.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Shape.h>
#include <Titania/X3D/Components/Shape/TwoSidedMaterial.h>

namespace titania {
namespace puck {

using math::pi;

X3DMaterialPaletteEditor::X3DMaterialPaletteEditor () :
	X3DPaletteEditor <X3DAppearanceEditorInterface> ("Materials"),
	                                  frontMaterial (true)
{
	setBoxTransparency (1);
}

void
X3DMaterialPaletteEditor::initialize ()
{
	X3DPaletteEditor <X3DAppearanceEditorInterface>::initialize ();
}

void
X3DMaterialPaletteEditor::configure ()
{
	X3DPaletteEditor <X3DAppearanceEditorInterface>::configure ();

	getPaletteFaceCombo () .set_active (getConfig () -> getItem <int32_t> ("paletteFace"));
}

X3D::SFNode
X3DMaterialPaletteEditor::getObject (const basic::uri & URL)
{
	const auto inlineNode  = getPreview () -> getExecutionContext () -> createNode <X3D::Inline> ();

	inlineNode -> url () = { URL .str () };

	return inlineNode;
}

void
X3DMaterialPaletteEditor::setTouchTime (const basic::uri & URL)
{
	try
	{
		// Apply selected material to selection.

		auto       selection        = getBrowserWindow () -> getSelection () -> getNodes ();
		const auto executionContext = X3D::MakePtr (getSelectionContext (selection, true));

		if (selection .empty ())
			return;

		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Apply Material From Palette"));
		const auto scene    = getCurrentBrowser () -> createX3DFromURL ({ URL .str () });

		MagicImport magicImport (getBrowserWindow ());

		magicImport .setFrontMaterial (frontMaterial);

		if (magicImport .import (executionContext, selection, scene, undoStep))
			getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

bool
X3DMaterialPaletteEditor::createScene (const X3D::X3DScenePtr & scene, const std::string & name, const size_t position)
{
	try
	{
		const X3D::SFNode material (getMaterial () -> copy (scene, X3D::FLAT_COPY));

		updateNamedNode (scene, name, position, material);

		// Create scene.

		if (material -> getType () .back () == X3D::X3DConstants::TwoSidedMaterial)
		{
			static constexpr size_t X_DIMENSION = 33; // Of the half sphere.
			static constexpr size_t Y_DIMENSION = 33; // Of the half sphere.

			const auto transform1  = scene -> createNode <X3D::Transform> ();
			const auto transform2  = scene -> createNode <X3D::Transform> ();
			const auto shape1      = scene -> createNode <X3D::Shape> ();
			const auto shape2      = scene -> createNode <X3D::Shape> ();
			const auto appearance1 = scene -> createNode <X3D::Appearance> ();
			const auto appearance2 = scene -> createNode <X3D::Appearance> ();
			const auto material1   = X3D::X3DPtr <X3D::TwoSidedMaterial> (material);
			const auto material2   = X3D::X3DPtr <X3D::TwoSidedMaterial> (material -> copy (X3D::FLAT_COPY));
			const auto halfSphere1 = scene -> createNode <X3D::Extrusion> ();

			halfSphere1 -> beginCap ()    = false;
			halfSphere1 -> endCap ()      = false;
			halfSphere1 -> creaseAngle () = pi <double>;

			halfSphere1 -> crossSection () .clear ();
			halfSphere1 -> crossSection () .clear ();
			halfSphere1 -> orientation ()  .clear ();
			halfSphere1 -> spine ()        .clear ();

			for (size_t i = 0; i < Y_DIMENSION - 1; ++ i)
				halfSphere1 -> crossSection () .emplace_back (std::sin (pi <double> * i / (Y_DIMENSION - 1)), std::cos (pi <double> * i / (Y_DIMENSION - 1)));

			halfSphere1 -> crossSection () .emplace_back (0, -1);

			for (size_t i = 0; i < X_DIMENSION ; ++ i)
				halfSphere1 -> orientation () .emplace_back (0, 0, 1, pi <double> * i / (X_DIMENSION - 1));

			for (size_t i = 0; i < X_DIMENSION; ++ i)
				halfSphere1 -> spine () .emplace_back (0, 0, 0);

			const auto halfSphere2 = X3D::X3DPtr <X3D::Extrusion> (halfSphere1-> copy (scene, X3D::FLAT_COPY));

			halfSphere2 -> ccw ()   = false;
			halfSphere2 -> solid () = false;

			transform1 -> rotation () = X3D::Rotation4d (1, 0, 0, pi <double> / 2) * X3D::Rotation4d (0, -1, 0, pi <double> / 2);
			transform2 -> rotation () = X3D::Rotation4d (1, 0, 0, pi <double> / 2) * X3D::Rotation4d (0,  1, 0, pi <double> / 2);

			material1 -> separateBackColor () = true;
			material2 -> separateBackColor () = true;
			material2 -> transparency ()      = 1;

			scene -> getRootNodes ()   = X3D::MFNode ({ transform1, transform2 });
			transform1 -> children ()  = X3D::MFNode ({ shape1 });
			transform2 -> children ()  = X3D::MFNode ({ shape2 });
			shape1 -> appearance ()    = appearance1;
			shape2 -> appearance ()    = appearance2;
			shape1 -> geometry ()      = halfSphere1;
			shape2 -> geometry ()      = halfSphere2;
			appearance1 -> material () = material1;
			appearance2 -> material () = material2;
		}
		else
		{
			const auto transform  = scene -> createNode <X3D::Transform> ();
			const auto shape      = scene -> createNode <X3D::Shape> ();
			const auto appearance = scene -> createNode <X3D::Appearance> ();
			const auto sphere     = scene -> createNode <X3D::Sphere> ();

			scene -> getRootNodes ()  = X3D::MFNode ({ transform });
			transform -> children ()  = X3D::MFNode ({ shape });
			shape -> appearance ()    = appearance;
			shape -> geometry ()      = sphere;
			appearance -> material () = material;
		}

		// Setup scene.

		scene -> setMetaData ("titania magic", "Material");

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void
X3DMaterialPaletteEditor::on_palette_face_changed ()
{
	frontMaterial = getPaletteFaceCombo () .get_active_row_number () == 0;
}

void
X3DMaterialPaletteEditor::store ()
{
	getConfig () -> setItem ("paletteFace", getPaletteFaceCombo () .get_active_row_number ());

	X3DPaletteEditor <X3DAppearanceEditorInterface>::store ();
}

X3DMaterialPaletteEditor::~X3DMaterialPaletteEditor ()
{ }

} // puck
} // titania

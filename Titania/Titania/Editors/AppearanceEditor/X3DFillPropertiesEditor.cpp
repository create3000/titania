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

#include "X3DFillPropertiesEditor.h"

namespace titania {
namespace puck {

X3DFillPropertiesEditor::X3DFillPropertiesEditor () :
	X3DAppearanceEditorInterface (),
	                 appearances (),
	              fillProperties (),
	        fillPropertiesBuffer (),
	                    nodeName (this, getFillPropertiesNameEntry (), getFillPropertiesRenameButton ()),
	                      filled (this, getFillPropertiesFilledCheckButton (), "filled"),
	                     hatched (this, getFillPropertiesHatchedCheckButton (), "hatched"),
	                  hatchStyle (this,
	                              getFillPropertiesHatchStyleAdjustment (),
	                              getFillPropertiesHatchStyleSpinButton (),
	                              "hatchStyle"),
	                  hatchColor (this,
	                              getFillPropertiesHatchColorButton (),
	                              getFillPropertiesHatchColorAdjustment (),
	                              getFillPropertiesHatchColorBox (),
	                              "hatchColor"),
	                    undoStep (),
	                    changing (false)
{
	addChildren (appearances,
	             fillProperties,
	             fillPropertiesBuffer);
}

void
X3DFillPropertiesEditor::initialize ()
{
	getPreview () -> initialized () .addInterest (this, &X3DFillPropertiesEditor::set_fillProperties);

	fillPropertiesBuffer .addInterest (this, &X3DFillPropertiesEditor::set_node);
}

void
X3DFillPropertiesEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & appearance : appearances)
		appearance -> fillProperties () .removeInterest (this, &X3DFillPropertiesEditor::set_fillProperties);

	appearances = getNodes <X3D::Appearance> (selection, { X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> fillProperties () .addInterest (this, &X3DFillPropertiesEditor::set_fillProperties);

	set_fillProperties ();
}

void
X3DFillPropertiesEditor::on_fillProperties_unlink_clicked ()
{
	unlinkClone (appearances, "fillProperties", undoStep);
}

void
X3DFillPropertiesEditor::on_fillProperties_toggled ()
{
	try
	{
		if (changing)
			return;

		const auto previewAppearance = getPreview () -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		addUndoFunction <X3D::SFNode> (appearances, "fillProperties", undoStep);

		getFillPropertiesCheckButton () .set_inconsistent (false);
		getFillPropertiesBox ()         .set_sensitive (getFillPropertiesCheckButton () .get_active ());

		for (const auto & appearance : appearances)
		{
			try
			{
				auto & field = appearance -> getField <X3D::SFNode> ("fillProperties");

				field .removeInterest (this, &X3DFillPropertiesEditor::set_fillProperties);
				field .addInterest (this, &X3DFillPropertiesEditor::connectFillProperties);

				if (getFillPropertiesCheckButton () .get_active ())
				{
					getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (appearance), field, X3D::SFNode (fillProperties), undoStep);
					previewAppearance -> fillProperties () = fillProperties;
				}
				else
				{
					getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (appearance), field, nullptr, undoStep);
					previewAppearance -> fillProperties () = nullptr;
				}
			}
			catch (const X3D::X3DError &)
			{ }
		}

		addRedoFunction <X3D::SFNode> (appearances, "fillProperties", undoStep);

		getFillPropertiesUnlinkButton () .set_sensitive (getFillPropertiesCheckButton () .get_active () and fillProperties -> getCloneCount () > 1);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DFillPropertiesEditor::set_fillProperties ()
{
	fillPropertiesBuffer .addEvent ();
}

void
X3DFillPropertiesEditor::set_node ()
{
	try
	{
		const auto previewAppearance = getPreview () -> getExecutionContext () -> getNamedNode <X3D::Appearance> ("Appearance");

		if (fillProperties)
			fillProperties -> removeInterest (*getPreview (), &X3D::X3DBrowser::addEvent);

		undoStep .reset ();

		auto          tuple     = getSelection <X3D::FillProperties> (appearances, "fillProperties");
		const int32_t active    = std::get <1> (tuple);
		const bool    hasParent = std::get <2> (tuple);
		const bool    hasField  = (active not_eq -2);

		fillProperties = std::move (std::get <0> (tuple));

		previewAppearance -> fillProperties () = fillProperties;

		if (fillProperties)
			fillProperties -> addInterest (*getPreview (), &X3D::X3DBrowser::addEvent);

		else
		{
			fillProperties = new X3D::FillProperties (getCurrentContext ());
			fillProperties -> addInterest (*getPreview (), &X3D::X3DBrowser::addEvent);
			fillProperties -> setup ();
		}

		changing = true;

		getSelectFillPropertiesBox ()   .set_sensitive (hasParent);
		getFillPropertiesMainBox ()     .set_sensitive (hasField);
		getFillPropertiesCheckButton () .set_active (active > 0);
		getFillPropertiesCheckButton () .set_inconsistent (active < 0);

		getFillPropertiesUnlinkButton () .set_sensitive (active > 0 and fillProperties -> getCloneCount () > 1);
		getFillPropertiesBox ()          .set_sensitive (active > 0);

		changing = false;

		const X3D::MFNode nodes = { fillProperties };

		nodeName .setNode (nodes .back ());

		filled     .setNodes (nodes);
		hatched    .setNodes (nodes);
		hatchStyle .setNodes (nodes);
		hatchColor .setNodes (nodes);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DFillPropertiesEditor::connectFillProperties (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DFillPropertiesEditor::connectFillProperties);
	field .addInterest (this, &X3DFillPropertiesEditor::set_fillProperties);
}

X3DFillPropertiesEditor::~X3DFillPropertiesEditor ()
{ }

} // puck
} // titania

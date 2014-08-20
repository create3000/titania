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

#include "X3DLinePropertiesEditor.h"

namespace titania {
namespace puck {

X3DLinePropertiesEditor::X3DLinePropertiesEditor () :
	X3DAppearanceEditorInterface ("", ""),
	                 appearances (),
	              lineProperties (),
	        linePropertiesBuffer (),
	                     applied (getBrowserWindow (), getLinePropertiesFilledCheckButton (), "applied"),
	                    linetype (getBrowserWindow (),
	                              getLinePropertiesLinetypeAdjustment (),
	                              getLinePropertiesLinetypeSpinButton (),
	                              "linetype"),
	        linewidthScaleFactor (getBrowserWindow (),
	                              getLinePropertiesLinewidthScaleFactorAdjustment (),
	                              getLinePropertiesLinewidthScaleFactorSpinButton (),
	                              "linewidthScaleFactor")
{
	addChildren (linePropertiesBuffer);
	linePropertiesBuffer .addInterest (this, &X3DLinePropertiesEditor::set_node);
}

void
X3DLinePropertiesEditor::initialize ()
{
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &X3DLinePropertiesEditor::set_selection);

	set_selection ();
}

void
X3DLinePropertiesEditor::set_selection ()
{
	undoStep .reset ();

	for (const auto & appearance : appearances)
		appearance -> lineProperties () .removeInterest (this, &X3DLinePropertiesEditor::set_lineProperties);

	appearances = getSelection <X3D::Appearance> ({ X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> lineProperties () .addInterest (this, &X3DLinePropertiesEditor::set_lineProperties);

	set_lineProperties ();
}

void
X3DLinePropertiesEditor::on_lineProperties_unlink_clicked ()
{
	unlinkClone (appearances, "lineProperties", undoStep);
}

void
X3DLinePropertiesEditor::on_lineProperties_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFNode> (appearances, "lineProperties", undoStep);

	getLinePropertiesCheckButton () .set_inconsistent (false);
	getLinePropertiesBox ()         .set_sensitive (getLinePropertiesCheckButton () .get_active ());

	for (const auto & appearance : appearances)
	{
		try
		{
			auto & field = appearance -> getField <X3D::SFNode> ("lineProperties");

			field .removeInterest (this, &X3DLinePropertiesEditor::set_lineProperties);
			field .addInterest (this, &X3DLinePropertiesEditor::connectLineProperties);

			if (getLinePropertiesCheckButton () .get_active ())
				getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), field, X3D::SFNode (lineProperties), undoStep);
			else
				getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (appearances, "lineProperties", undoStep);

	getLinePropertiesUnlinkButton () .set_sensitive (getLinePropertiesCheckButton () .get_active () and lineProperties -> getCloneCount () > 1);
}

void
X3DLinePropertiesEditor::set_lineProperties ()
{
	linePropertiesBuffer .addEvent ();
}

void
X3DLinePropertiesEditor::set_node ()
{
	auto       pair     = getNode <X3D::LineProperties> (appearances, "lineProperties");
	const int  active   = pair .second;
	const bool hasField = (active not_eq -2);

	lineProperties = std::move (pair .first);

	if (not lineProperties)
	{
		lineProperties = new X3D::LineProperties (getExecutionContext ());
		lineProperties -> setup ();
	}

	changing = true;

	getLinePropertiesMainBox ()     .set_sensitive (hasField);
	getLinePropertiesCheckButton () .set_active (active > 0);
	getLinePropertiesCheckButton () .set_inconsistent (active < 0);

	getLinePropertiesUnlinkButton () .set_sensitive (active > 0 and lineProperties -> getCloneCount () > 1);
	getLinePropertiesBox ()          .set_sensitive (active > 0);

	changing = false;

	const X3D::MFNode nodes = { lineProperties };

	applied              .setNodes (nodes);
	linetype             .setNodes (nodes);
	linewidthScaleFactor .setNodes (nodes);
}

void
X3DLinePropertiesEditor::connectLineProperties (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DLinePropertiesEditor::connectLineProperties);
	field .addInterest (this, &X3DLinePropertiesEditor::set_lineProperties);
}

X3DLinePropertiesEditor::~X3DLinePropertiesEditor ()
{ }

} // puck
} // titania

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

#include "X3DConeEditor.h"

#include <Titania/X3D/Browser/Geometry3D/ConeOptions.h>
#include <Titania/X3D/Components/Geometry3D/Cone.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

X3DConeEditor::X3DConeEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                                side (this, getConeSideCheckButton (),"side"),
	                              bottom (this, getConeBottomCheckButton (), "bottom"),
	                              height (this, getConeHeightAdjustment (), getConeHeightSpinButton (), "height"),
	                        bottomRadius (this, getConeBottomRadiusAdjustment (), getConeBottomRadiusSpinButton (), "bottomRadius"),
	                          xDimension (this, getConeXDimensionAdjustment (), getConeXDimensionSpinButton (), "xDimension"),
	                               nodes (),
	                            changing (false)
{
	addChildObjects (nodes);
}

void
X3DConeEditor::set_geometry ()
{
	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .removeInterest (&X3DConeEditor::set_options, this);

	// Fields

	nodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::Cone });

	getConeBox () .set_visible (not nodes .empty ());

	side         .setNodes (nodes);
	bottom       .setNodes (nodes);
	height       .setNodes (nodes);
	bottomRadius .setNodes (nodes);

	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .addInterest (&X3DConeEditor::set_options, this);

	set_options ();
}

void
X3DConeEditor::on_cone_use_global_options_toggled ()
{
	if (changing)
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Toggle Cone Use Global Options To »%s«", getConeUseGlobalOptionsCheckButton () .get_active () ? "TRUE" : "FALSE")));
	const auto executionContext = X3D::MakePtr (getSelectionContext (nodes, true));

	if (getConeUseGlobalOptionsCheckButton () .get_active ())
	{
		for (const auto & node : nodes)
		{
			auto & options = node -> getField <X3D::SFNode> ("options");

			X3D::X3DEditor::replaceNode (executionContext, node, options, nullptr, undoStep);
		}
	}
	else
	{
		for (const auto & node : nodes)
		{
			auto &     options    = node -> getField <X3D::SFNode> ("options");
			const auto optionNode = X3D::SFNode (getCurrentBrowser () -> getConeOptions () -> copy (executionContext, X3D::FLAT_COPY));

			X3D::X3DEditor::replaceNode (executionContext, node, options, optionNode, undoStep);
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DConeEditor::set_options ()
{
	// Set composed widgets.

	X3D::MFNode optionsNodes;

	for (const auto & node : nodes)
	{
		const auto & optionsNode = node -> getField <X3D::SFNode> ("options");

		if (optionsNode)
			optionsNodes .emplace_back (optionsNode);
	}

	const auto global       = optionsNodes .empty ();
	const auto inconsistent = optionsNodes .size () not_eq nodes .size ();

	if (optionsNodes .empty ())
		optionsNodes .emplace_back (getCurrentBrowser () -> getConeOptions ());

	xDimension .setNodes (optionsNodes);

	// Set global widget.

	changing = true;

	getConeUseGlobalOptionsCheckButton () .set_active (global);
	getConeUseGlobalOptionsCheckButton () .set_inconsistent (inconsistent);

	getConeOptionsGrid () .set_sensitive (not global and not inconsistent);

	changing = false;
}

X3DConeEditor::~X3DConeEditor ()
{ }

} // puck
} // titania

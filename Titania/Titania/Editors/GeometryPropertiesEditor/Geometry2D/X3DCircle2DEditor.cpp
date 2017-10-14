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

#include "X3DCircle2DEditor.h"

#include <Titania/X3D/Browser/Geometry2D/Circle2DOptions.h>
#include <Titania/X3D/Components/Geometry2D/Circle2D.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

X3DCircle2DEditor::X3DCircle2DEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                              radius (this, getCircle2DRadiusAdjustment (), getCircle2DRadiusSpinButton (), "radius"),
	                           dimension (this, getCircle2DDimensionAdjustment (), getCircle2DDimensionSpinButton (), "dimension"),
	                               nodes (),
	                            changing (false)
{
	addChildObjects (nodes);
}

void
X3DCircle2DEditor::set_geometry ()
{
	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .removeInterest (&X3DCircle2DEditor::set_options, this);

	// Fields

	nodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::Circle2D });

	getCircle2DBox () .set_visible (not nodes .empty ());

	radius    .setNodes (nodes);
	dimension .setNodes (nodes);

	// Hidden fields

	for (const auto & node : nodes)
		node -> getField <X3D::SFNode> ("options") .addInterest (&X3DCircle2DEditor::set_options, this);

	set_options ();
}

void
X3DCircle2DEditor::on_circle2d_use_global_options_toggled ()
{
	if (changing)
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> (_ (basic::sprintf ("Toggle Circle2D Use Global Options To »%s«", getCircle2DUseGlobalOptionsCheckButton () .get_active () ? "TRUE" : "FALSE")));
	const auto executionContext = X3D::X3DExecutionContextPtr (getExecutionContext (nodes));

	if (getCircle2DUseGlobalOptionsCheckButton () .get_active ())
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
			const auto optionNode = X3D::SFNode (getCurrentBrowser () -> getCircle2DOptions () -> copy (executionContext, X3D::FLAT_COPY));

			X3D::X3DEditor::replaceNode (executionContext, node, options, optionNode, undoStep);
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DCircle2DEditor::set_options ()
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
		optionsNodes .emplace_back (getCurrentBrowser () -> getCircle2DOptions ());

	dimension .setNodes (optionsNodes);

	// Set global widget.

	changing = true;

	getCircle2DUseGlobalOptionsCheckButton () .set_active (global);
	getCircle2DUseGlobalOptionsCheckButton () .set_inconsistent (inconsistent);

	getCircle2DOptionsGrid () .set_sensitive (not global and not inconsistent);

	changing = false;
}

X3DCircle2DEditor::~X3DCircle2DEditor ()
{ }

} // puck
} // titania

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

#include "GeometryPropertiesEditor.h"

#include "../../Configuration/config.h"

namespace titania {
namespace puck {

GeometryPropertiesEditor::GeometryPropertiesEditor (X3DBrowserWindow* const browserWindow) :
	                     X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	 X3DGeometryPropertiesEditorInterface (get_ui ("Editors/GeometryPropertiesEditor.xml"), gconf_dir ()),
	              X3DPrimitiveCountEditor (),
	                                solid (browserWindow, getSolidCheckButton (),  "solid"),
	                                  ccw (browserWindow, getCCWCheckButton (),    "ccw"),
	                               convex (browserWindow, getConvexCheckButton (), "convex"),
	                          creaseAngle (browserWindow, getCreaseAngleAdjustment (), getCreaseAngleBox (), "creaseAngle"),
	                               shapes (),
	                          nodesBuffer ()
{
	nodesBuffer .addInterest (this, &GeometryPropertiesEditor::set_buffer);
	addChildren (nodesBuffer);

	getCreaseAngleAdjustment () -> set_upper (M_PI); // getExecutionContext () .fromRadiant (M_PI);

	getGeometryChildNotebook () .set_current_page (getConfig () .getInteger ("currentPage"));

	setup ();
}

void
GeometryPropertiesEditor::initialize ()
{
	X3DGeometryPropertiesEditorInterface::initialize ();
	X3DPrimitiveCountEditor::initialize ();

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &GeometryPropertiesEditor::set_selection);

	set_selection ();
}

void
GeometryPropertiesEditor::set_selection ()
{
	for (const auto & shape : shapes)
		shape -> geometry () .removeInterest (this, &GeometryPropertiesEditor::set_nodes);

	shapes = getSelection <X3D::X3DShapeNode> ({ X3D::X3DConstants::X3DShapeNode });
	
	for (const auto & shape : shapes)
		shape -> geometry () .addInterest (this, &GeometryPropertiesEditor::set_nodes);

	set_nodes ();
	
	getRemoveNormalsButton () .set_sensitive (not shapes .empty ());
}

void
GeometryPropertiesEditor::set_nodes ()
{
	nodesBuffer .addEvent ();
}

void
GeometryPropertiesEditor::set_buffer ()
{
	const auto nodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DGeometryNode });

	solid       .setNodes (nodes);
	ccw         .setNodes (nodes);
	convex      .setNodes (nodes);
	creaseAngle .setNodes (nodes);
}

void
GeometryPropertiesEditor::on_remove_normals_clicked ()
{
	const auto geometries = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DGeometryNode });
	const auto undoStep   = std::make_shared <UndoStep> (_ ("Remove Normals"));

	for (const auto geometry : geometries)
	{
		try
		{
			auto & normalIndex = geometry -> getField <X3D::MFInt32> ("normalIndex");

			undoStep -> addObjects (geometry);
			undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (normalIndex), normalIndex);
			undoStep -> addRedoFunction (&X3D::MFInt32::clear, std::ref (normalIndex));
			normalIndex .clear ();
		}
		catch (const X3D::X3DError &)
		{ }

		try
		{
			auto & normal = geometry -> getField <X3D::SFNode> ("normal");

			getBrowserWindow () -> replaceNode (geometry, normal, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

GeometryPropertiesEditor::~GeometryPropertiesEditor ()
{
	getConfig () .setItem ("currentPage", getGeometryChildNotebook () .get_current_page ());

	dispose ();
}

} // puck
} // titania

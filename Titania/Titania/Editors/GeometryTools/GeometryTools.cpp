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

#include "GeometryTools.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Browser/Core/Cast.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Components/Rendering/X3DGeometryNode.h>
#include <Titania/X3D/Tools/Geometry3D/IndexedFaceSetTool.h>
#include <Titania/X3D/Tools/Rendering/X3DGeometryNodeTool.h>

namespace titania {
namespace puck {

GeometryTools::GeometryTools (X3DBrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DGeometryToolsInterface (get_ui ("Editors/GeometryTools.glade"), gconf_dir ()),
	            editableNodes ()
{
	setup ();
}

void
GeometryTools::initialize ()
{
	X3DGeometryToolsInterface::initialize ();

	getShowNormalsToggleButton () .set_active (getConfig () -> getBoolean ("showNormals"));
}

void
GeometryTools::set_selection (const X3D::MFNode & selection)
{
	X3DGeometryToolsInterface::set_selection (selection);

	const bool inScene        = not inPrototypeInstance ();
	const bool haveSelection  = inScene and selection .size ();
	//const bool haveSelections = inScene and selection .size () > 1;

	editableNodes = getNodes <X3D::X3DBaseNode> (selection, { X3D::X3DConstants::IndexedFaceSet });

	getHammerButton () .set_sensitive (haveSelection);
	getEditButton ()   .set_sensitive (not editableNodes .empty ());

	on_show_normals_toggled ();
}

void
GeometryTools::on_hammer_clicked ()
{
	const auto undoStep  = std::make_shared <X3D::UndoStep> (_ ("Smash Selection"));
	auto       selection = getBrowserWindow () -> getSelection () -> getChildren ();

	for (const auto & shape : getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode }))
	{
		if (not shape -> geometry ())
			continue;

		for (const auto & type : basic::make_reverse_range (shape -> geometry () -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::X3DPrototypeInstance :
					{
						try
						{
							const X3D::X3DPtr <X3D::X3DGeometryNode> geometry (shape -> geometry () -> getInnerNode ());

							if (geometry)
							{
								X3D::MFNode        exports ({ geometry });
								basic::ifilestream text (getBrowserWindow () -> exportNodes (getCurrentContext (), exports));

								const auto scene = getCurrentBrowser () -> createX3DFromStream (getCurrentContext () -> getWorldURL (), text);
								const auto nodes = getBrowserWindow () -> importScene (getCurrentContext (), X3D::SFNode (getCurrentContext ()), getCurrentContext () -> getRootNodes (), scene, undoStep);

								getBrowserWindow () -> addToGroup (getCurrentContext (), X3D::SFNode (shape), nodes, undoStep);
							}
						}
						catch (const X3D::X3DError &)
						{ }

						break;
					}
				case X3D::X3DConstants::X3DGeometryNode:
				{
					try
					{
						const X3D::X3DPtr <X3D::X3DGeometryNode> geometry (shape -> geometry ());

						getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (shape), shape -> geometry (), geometry -> toPrimitive (), undoStep);
					}
					catch (const X3D::X3DError &)
					{ }

					break;
				}
				default:
					continue;
			}

			break;
		}
	}

	addUndoStep (undoStep);
}

void
GeometryTools::on_edit_clicked ()
{
	getBrowserWindow () -> getSelection () -> setChildren (editableNodes);
}

void
GeometryTools::on_show_normals_toggled ()
{
	__LOG__ << getShowNormalsToggleButton () .get_active () << std::endl;

	for (const auto & node : getBrowserWindow () -> getSelection () -> getChildren ())
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();
	
			for (const auto & type : basic::make_reverse_range (node -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::IndexedFaceSetTool:
					{
						dynamic_cast <X3D::IndexedFaceSetTool*> (innerNode) -> setShowNormals (getShowNormalsToggleButton () .get_active ());
						break;
					}
					default:
						break;
				}
	
				break;
			}
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
}

GeometryTools::~GeometryTools ()
{
	getConfig () -> setItem ("showNormals", getShowNormalsToggleButton () .get_active ());

	X3DGeometryToolsInterface::dispose ();
}

} // puck
} // titania

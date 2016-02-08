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

#include "GeometryEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Browser/Core/Cast.h>
#include <Titania/X3D/Components/Rendering/X3DGeometryNode.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

GeometryEditor::GeometryEditor (X3DBrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DGeometryEditorInterface (get_ui ("Editors/GeometryEditor.glade"), gconf_dir ()),
	             normalEnabled (this, getNormalEnabledToggleButton (), "enabled"),
	              normalEditor (new X3D::FieldSet (getMasterBrowser ())),
	             geometryNodes ()
{
	normalEnabled .setUndo (false);

	normalEditor -> addUserDefinedField (X3D::inputOutput, "enabled", new X3D::SFBool ());
	normalEditor -> addUserDefinedField (X3D::inputOutput, "color",   new X3D::SFColorRGBA (0.7, 0.85, 1, 1));
	normalEditor -> addUserDefinedField (X3D::inputOutput, "length",  new X3D::SFFloat (1));

	setup ();
}

void
GeometryEditor::initialize ()
{
	X3DGeometryEditorInterface::initialize ();

	normalEditor -> setField <X3D::SFBool> ("enabled", getConfig () -> getBoolean ("normalEnabled"), true);
	normalEditor -> setup ();
	normalEnabled .setNodes ({ normalEditor });
}

void
GeometryEditor::set_selection (const X3D::MFNode & selection)
{
	X3DGeometryEditorInterface::set_selection (selection);

	const bool inScene       = not inPrototypeInstance ();
	const bool haveSelection = inScene and selection .size ();
	//const bool haveSelections = inScene and selection .size () > 1;

	geometryNodes = getNodes <X3D::X3DBaseNode> (selection, { X3D::X3DConstants::IndexedFaceSet });

	getHammerButton () .set_sensitive (haveSelection);
	getEditButton ()   .set_sensitive (not geometryNodes .empty ());

	connect ();
}

void
GeometryEditor::connect ()
{
	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto innerNode = node -> getInnerNode ();

			for (const auto & type : basic::make_reverse_range (node -> getType ()))
			{
				switch (type)
				{
					case X3D::X3DConstants::X3DGeometryNodeTool:
					{
						const auto & normalTool = innerNode -> getField <X3D::SFNode> ("normalTool");

						normalEditor -> getField <X3D::SFBool>      ("enabled") .addInterest (normalTool -> getField <X3D::SFBool>      ("enabled"));
						normalEditor -> getField <X3D::SFFloat>     ("length")  .addInterest (normalTool -> getField <X3D::SFFloat>     ("length"));
						normalEditor -> getField <X3D::SFColorRGBA> ("color")   .addInterest (normalTool -> getField <X3D::SFColorRGBA> ("color"));

						normalTool -> setField <X3D::SFBool>      ("enabled", normalEditor -> getField <X3D::SFBool>      ("enabled"), true);
						normalTool -> setField <X3D::SFFloat>     ("length",  normalEditor -> getField <X3D::SFFloat>     ("length"),  true);
						normalTool -> setField <X3D::SFColorRGBA> ("color",   normalEditor -> getField <X3D::SFColorRGBA> ("color"),   true);
						break;
					}
					default:
						continue;
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

void
GeometryEditor::on_hammer_clicked ()
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
GeometryEditor::on_edit_clicked ()
{
	getBrowserWindow () -> getSelection () -> setChildren (geometryNodes);
}

GeometryEditor::~GeometryEditor ()
{
	getConfig () -> setItem ("normalEnabled", normalEditor -> getField <X3D::SFBool> ("enabled") .getValue ());

	X3DGeometryEditorInterface::dispose ();
}

} // puck
} // titania

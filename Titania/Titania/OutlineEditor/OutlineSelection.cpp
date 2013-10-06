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

#include "OutlineSelection.h"

#include "../Browser/BrowserWindow.h"
#include "OutlineTreeModel.h"
#include "OutlineTreeViewEditor.h"

namespace titania {
namespace puck {

const std::string OutlineSelection::componentName  = "BrowserWindow";
const std::string OutlineSelection::typeName       = "OutlineSelection";
const std::string OutlineSelection::containerField = "selection";

OutlineSelection::OutlineSelection (BrowserWindow* const browserWindow, OutlineTreeViewEditor* const treeView) :
	X3D::X3DBaseNode (browserWindow -> getBrowser (), browserWindow -> getExecutionContext ()),
	X3DBaseInterface (browserWindow),
	        treeView (treeView),
	  selectMultiple (false),
	        children ()
{
	// Register browser interest

	getBrowser () -> initialized () .addInterest (this, &OutlineSelection::clear);
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &OutlineSelection::set_children);
}

void
OutlineSelection::set_select_multiple (bool value)
{
	selectMultiple = value;
}

void
OutlineSelection::set_children (const X3D::MFNode & value)
{
	for (const auto & child : children)
		select (child, false);

	children = value;

	for (const auto & child : children)
		select (child, true);

	treeView -> queue_draw ();
}

void
OutlineSelection::select (const X3D::SFNode & node)
{
	if (node)
	{
		bool selected = std::find (children .begin (), children .end (), node) not_eq children .end ();

		if (selectMultiple)
		{
			if (selected)
				getBrowser () -> getSelection () -> removeChildren ({ node });
		}
		else
			clear ();

		if (not selected)
			getBrowser () -> getSelection () -> addChildren ({ node });
	}
}

void
OutlineSelection::clear ()
{
	getBrowser () -> getSelection () -> clear ();
}

void
OutlineSelection::select (X3D::X3DBaseNode* const node, bool value) const
{
	X3D::ChildObjectSet objects;

	select (node, value, objects);
}

void
OutlineSelection::select (X3D::X3DBaseNode* const node, bool value, X3D::ChildObjectSet & seen) const
{
	if (node)
	{
		if (not seen .insert (node) .second)
			return;

		// Select node

		treeView -> get_user_data (node) -> selected = value;

		// Select children

		for (const auto & field : node -> getFieldDefinitions ())
			select (field, value, seen);
	}
}

void
OutlineSelection::select (X3D::X3DFieldDefinition* const field, bool value, X3D::ChildObjectSet & seen) const
{
	// Select field

	treeView -> get_user_data (field) -> selected = value;

	// Select children

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFNode:
		{
			auto sfnode = static_cast <X3D::SFNode*> (field);
			select (sfnode -> getValue (), value, seen);
			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			auto mfnode = static_cast <X3D::MFNode*> (field);

			for (auto & sfnode : *mfnode)
			{
				treeView -> get_user_data (&sfnode) -> selected = value;

				select (sfnode .getValue (), value, seen);
			}

			break;
		}
		default:
			break;
	}
}

} // puck
} // titania

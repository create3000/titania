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

#include "OutlineSelection.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "OutlineTreeModel.h"
#include "X3DOutlineTreeView.h"

#include <Titania/X3D/Browser/Selection.h>

namespace titania {
namespace puck {

OutlineSelection::OutlineSelection (X3DBrowserWindow* const browserWindow, X3DOutlineTreeView* const treeView) :
	X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	        treeView (treeView),
	  selectMultiple (false),
	           nodes ()
{
	addChildObjects (nodes);

	getBrowserWindow () -> getSelection () -> getNodes () .addInterest (&OutlineSelection::set_nodes, this);

	set_nodes (getBrowserWindow () -> getSelection () -> getNodes ());

	setup ();
}

void
OutlineSelection::set_select_multiple (const bool value)
{
	selectMultiple = value;
}

void
OutlineSelection::set_nodes (const X3D::MFNode & value)
{
	for (const auto & node : nodes)
		select (node, false);

	nodes = value;

	for (const auto & node : nodes)
		select (node, true);

	treeView -> queue_draw ();
}

void
OutlineSelection::select (const X3D::SFNode & node) const
{
	if (node)
	{
		const auto & selection  = node -> getBrowser () -> getSelection ();
		const bool   isSelected = selection -> isSelected (node);

		if (isSelected)
		{
			if (selectMultiple)
				selection -> removeNodes ({ node });
			else
				selection -> clearNodes ();
		}
		else
		{
			if (selectMultiple)
				selection -> addNodes ({ node });
			else
				selection -> setNodes ({ node });	
		}

		//getCurrentBrowser () -> update ();
	}
}

void
OutlineSelection::select (X3D::X3DBaseNode* const node, const bool value)
{
	X3D::ChildObjectSet seen;

	select (node, value, seen);
}

void
OutlineSelection::select (X3D::X3DBaseNode* const node, const bool value, X3D::ChildObjectSet & seen)
{
	if (node)
	{
		if (not seen .emplace (node) .second)
			return;

		// Select node

		select (X3DOutlineTreeView::get_user_data (node), value);

		// Select children

		for (const auto & field : node -> getFieldDefinitions ())
			select (field, value, seen);
	}
}

void
OutlineSelection::select (X3D::X3DFieldDefinition* const field, const bool value, X3D::ChildObjectSet & seen)
{
	// Select field

	select (X3DOutlineTreeView::get_user_data (field), value);

	// Select children

	switch (field -> getType ())
	{
		case X3D::X3DConstants::SFNode:
		{
			const auto sfnode = static_cast <X3D::SFNode*> (field);

			select (sfnode -> getValue (), value, seen);
			break;
		}
		case X3D::X3DConstants::MFNode:
		{
			const auto mfnode = static_cast <X3D::MFNode*> (field);

			for (auto & sfnode : *mfnode)
			{
				select (X3DOutlineTreeView::get_user_data (&sfnode), value);

				select (sfnode .getValue (), value, seen);
			}

			break;
		}
		default:
			break;
	}
}

void
OutlineSelection::select (const UserDataPtr & userData, const bool value)
{
	if (value)
		userData -> selected |= OUTLINE_SELECTED;
	else
		userData -> selected &= ~OUTLINE_SELECTED;
}

/***
 *  Updates the selection status of a @a node depending on its parent @a field.
 */
void
OutlineSelection::update (X3D::X3DFieldDefinition* const field, const X3D::SFNode & node)
{
	if (not node)
		return;

	const auto & selection = node -> getBrowser () -> getSelection ();

	if (node -> getCloneCount () > 1)
	{
		if (selection -> isSelected (node))
			return;

		bool selected = false;

		for (const auto & parent : node -> getParents ())
		{
			if (X3DOutlineTreeView::get_user_data (parent) -> selected & OUTLINE_SELECTED)
			{
				selected = true;
				break;
			}
		}

		if ((X3DOutlineTreeView::get_user_data (node) -> selected & OUTLINE_SELECTED) == selected)
			return;

		select (node, selected);
	}
	else
	{
		const bool selected = (X3DOutlineTreeView::get_user_data (field) -> selected & OUTLINE_SELECTED)
		                      or selection -> isSelected (node);

		if ((X3DOutlineTreeView::get_user_data (node) -> selected & OUTLINE_SELECTED) == selected)
			return;

		select (node, selected);
	}
}

/***
 *  Updates the selection status of @a field depending on its parent @a node.
 */
void
OutlineSelection::update (const X3D::SFNode & node, X3D::X3DFieldDefinition* const field)
{
	const bool selected = X3DOutlineTreeView::get_user_data (node) -> selected & OUTLINE_SELECTED;

	if ((X3DOutlineTreeView::get_user_data (field) -> selected & OUTLINE_SELECTED) == selected)
		return;
	
	X3D::ChildObjectSet seen;

	select (field, selected, seen);
}

OutlineSelection::~OutlineSelection ()
{
	dispose ();
}

} // puck
} // titania

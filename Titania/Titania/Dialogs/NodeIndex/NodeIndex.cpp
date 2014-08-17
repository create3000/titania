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

#include "NodeIndex.h"

#include "../../Configuration/config.h"

#include <Titania/OS.h>

namespace titania {
namespace puck {

namespace Columns {

static constexpr int TYPE_NAME = 0;
static constexpr int NAME      = 1;

};

NodeIndex::NodeIndex (BrowserWindow* const browserWindow) :
	     X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DNodeIndexInterface (get_ui ("Dialogs/NodeIndex.xml"), gconf_dir ()),
	                index (NAMED_NODES_INDEX),
	                types ()
{
	setup ();
}

void
NodeIndex::initialize ()
{
	X3DNodeIndexInterface::initialize ();

	getExecutionContext () .addInterest (this, &NodeIndex::refresh);
}

void
NodeIndex::setNamedNodes ()
{
	index = NAMED_NODES_INDEX;
	setNodes (getNodes ());
}

void
NodeIndex::setTypes (const std::set <X3D::X3DConstants::NodeType> & value)
{
	index = TYPE_INDEX;
	types = value;
	setNodes (getNodes (types));
}

void
NodeIndex::refresh ()
{
	switch (index)
	{
		case NAMED_NODES_INDEX:
		{
			setNodes (getNodes ());
			break;
		}
		case TYPE_INDEX:
		{
			setNodes (getNodes (types));
			break;
		}
	}
}

void
NodeIndex::setNodes (X3D::MFNode && value)
{
	nodes = std::move (value);

	getListStore () -> clear ();

	for (const auto & node : nodes)
	{
		const auto row = getListStore () -> append ();
		row -> set_value (Columns::TYPE_NAME, node -> getTypeName ());
		row -> set_value (Columns::NAME,      node -> getName ());
	}
}

/***
 *  Returns a list of all nodes where type is @a types.
 */
X3D::MFNode
NodeIndex::getNodes (const std::set <X3D::X3DConstants::NodeType> & types)
{
	// Find nodes

	X3D::MFNode nodes;

	X3D::traverse (getExecutionContext () -> getRootNodes (), [&] (X3D::SFNode & node)
	               {
	                  if (node -> getExecutionContext () not_eq getExecutionContext ())
								return true;

	                  for (const auto & type: basic::make_reverse_range (node -> getType ()))
	                  {
	                     if (types .count (type))
	                     {
	                        nodes .emplace_back (node);
	                        break;
								}
							}

	                  return true;
						});

	return nodes;
}

/***
 *  Returns a list of all named nodes.
 */
X3D::MFNode
NodeIndex::getNodes ()
{
	// Find nodes

	X3D::MFNode nodes;

	for (const auto pair : getExecutionContext () -> getNamedNodes ())
		nodes .emplace_back (pair .second -> getLocalNode ());

	return nodes;
}

void
NodeIndex::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	getBrowser () -> getSelection () -> setChildren ({ nodes [path .front ()] });
}

NodeIndex::~NodeIndex ()
{
	dispose ();
}

} // puck
} // titania

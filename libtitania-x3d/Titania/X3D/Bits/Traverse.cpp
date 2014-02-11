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

#include "Traverse.h"

#include "../Basic/NodeSet.h"
#include "../Components/Networking/Inline.h"
#include "../Handles/X3DHandleObject.h"

namespace titania {
namespace X3D {

static
bool
traverse (X3D::SFNode & node, const TraverseCallback & callback, bool distinct, NodeSet & seen)
{
	if (not node)
		return true;

	if (seen .emplace (node) .second)
	{
		for (const auto & field : node -> getFieldDefinitions ())
		{
			switch (field -> getType ())
			{
				case X3DConstants::SFNode :
					{
						auto sfnode = static_cast <X3D::SFNode*> (field);

						if (traverse (*sfnode, callback, distinct, seen))
							continue;

						return false;
					}
				case X3DConstants::MFNode:
				{
					auto mfnode = static_cast <X3D::MFNode*> (field);

					for (auto & value : *mfnode)
					{
						if (traverse (value, callback, distinct, seen))
							continue;

						return false;
					}

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		if (distinct)
			return true;
	}

	if (callback (node))
		return true;

	return false;
}

bool
traverse (X3D::MFNode & nodes, const TraverseCallback & callback, bool distinct)
{
	NodeSet seen;

	for (auto & node : nodes)
	{
		if (traverse (node, callback, distinct, seen))
			continue;

		return false;
	}

	return true;
}

bool
traverse (X3D::SFNode & node, const TraverseCallback & callback, bool distinct)
{
	NodeSet seen;

	return traverse (node, callback, distinct, seen);
}

bool
find (X3DBaseNode* const node, X3DChildObject* const object, bool inScene, std::deque <X3DChildObject*> & hierarchy, NodeSet & seen)
{
	if (not node)
		return false;

	if (not seen .emplace (node) .second)
		return false;

	hierarchy .emplace_back (node);

	if (node == object)
		return true;

	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (field == object)
		{
			hierarchy .emplace_back (field);
			return true;
		}

		switch (field -> getType ())
		{
			case X3DConstants::SFNode:
			{
				hierarchy .emplace_back (field);

				auto sfnode = static_cast <X3D::SFNode*> (field);

				if (find (*sfnode, object, inScene, hierarchy, seen))
					return true;

				hierarchy .pop_back ();
				break;
			}
			case X3DConstants::MFNode:
			{
				hierarchy .emplace_back (field);

				auto mfnode = static_cast <X3D::MFNode*> (field);

				for (const auto & value : *mfnode)
				{
					if (find (value, object, inScene, hierarchy, seen))
						return true;
				}

				hierarchy .pop_back ();
				break;
			}
			default:
				break;
		}
	}

	if (not inScene)
	{
		if (find (node -> getLocalNode (), object, inScene, hierarchy, seen))
			return true;

		Inline* inlineNode = dynamic_cast <Inline*> (node);

		if (inlineNode)
		{
			for (const auto & rootNode : inlineNode -> getRootNodes ())
			{
				if (find (rootNode, object, inScene, hierarchy, seen))
					return true;
			}
		}
		else
		{
			X3DHandleObject* handle = dynamic_cast <X3DHandleObject*> (node);

			if (handle)
			{
				for (const auto & rootNode : handle -> getRootNodes ())
				{
					if (find (rootNode, object, inScene, hierarchy, seen))
						return true;
				}
			}
		}
	}

	hierarchy .pop_back ();
	return false;
}

std::deque <X3DChildObject*>
find (const X3D::MFNode & nodes, X3DChildObject* const object, bool inScene)
{
	std::deque <X3DChildObject*> hierarchy;
	NodeSet                      seen;

	for (const auto & node : nodes)
	{
		if (find (node, object, inScene, hierarchy, seen))
			return hierarchy;
	}

	return hierarchy;
}

std::deque <X3DChildObject*>
find (const X3D::SFNode & node, X3DChildObject* const object, bool inScene)
{
	std::deque <X3DChildObject*> hierarchy;
	NodeSet                      seen;

	find (node, object, inScene, hierarchy, seen);

	return hierarchy;
}

} // X3D
} // titania

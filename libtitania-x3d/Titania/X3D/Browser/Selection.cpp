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

#include "Selection.h"

#include "../Bits/Cast.h"
#include "../Bits/Traverse.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Grouping/Transform.h"
#include "../Components/Layering/LayerSet.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Shape/X3DShapeNode.h"
#include "../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string Selection::componentName  = "Browser";
const std::string Selection::typeName       = "Selection";
const std::string Selection::containerField = "selection";

Selection::Selection (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     enabled (false),
	        mode (SINGLE),
	selectLowest (true),
	        over (),
	      active (),
	selectedTime (),
	    children ()
{
	addType (X3DConstants::Selection);

	X3DChildObject::addChildren (enabled, mode, selectLowest, over, active, selectedTime, children);
}

X3DBaseNode*
Selection::create (X3DExecutionContext* const executionContext) const
{
	return new Selection (executionContext);
}

void
Selection::initialize ()
{
	X3DBaseNode::initialize ();

	getBrowser () -> initialized () .addInterest (this, &Selection::clear);
}

bool
Selection::isSelected (const SFNode & node) const
{
	return std::find (children .begin (), children .end (), node) not_eq children .end ();
}

void
Selection::addChildren (const MFNode & value)
{
	if (getBrowser () -> makeCurrent ())
	{
		for (const auto & child : value)
		{
			if (not child)
				continue;

			if (isSelected (child))
				continue;

			if (child -> getExecutionContext () == getBrowser () -> getExecutionContext ())
				child -> addTool ();

			children .emplace_back (child);
		}
	}
}

void
Selection::removeChildren (const MFNode & value)
{
	if (getBrowser () -> makeCurrent ())
	{
		for (const auto & child : value)
		{
			if (child)
			{
				children .erase (std::remove (children .begin (),
				                              children .end (),
				                              child),
				                 children .end ());

				child -> removeTool ();
			}
		}
	}
}

void
Selection::setChildren (const MFNode & value)
{
	if (not children .empty ())
	{
		MFNode sortedValue = value;
		MFNode difference;

		std::sort (children .begin (), children .end ());
		std::sort (sortedValue .begin (), sortedValue .end ());

		std::set_difference (children .begin (), children .end (),
		                     sortedValue .begin (), sortedValue .end (),
		                     std::back_inserter (difference));

		removeChildren (difference);

		children .clear ();
	}

	addChildren (value);
}

bool
Selection::select ()
{
	if (not isEnabled ())
		return false;

	// Selected highest or lowest Node, or clear selection.

	if (getBrowser () -> getHits () .empty ())
	{
		clear ();
	}
	else
	{
		const auto hierarchy = find (getBrowser () -> getExecutionContext () -> getRootNodes (),
		                             getBrowser () -> getNearestHit () -> shape,
		                             TRAVERSE_ROOT_NODES |
		                             TRAVERSE_PROTOTYPE_INSTANCES |
		                             TRAVERSE_INLINE_NODES |
		                             TRAVERSE_TOOL_OBJECTS);


		if (not hierarchy .empty ())
		{
			SFNode node;

			if (selectLowest)
			{
				for (const auto & object : basic::reverse_adapter (hierarchy))
				{
					const SFNode lowest (object);

					if (not lowest)
						continue;

					if (lowest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
						continue;
						
					if (not node)
						node = lowest;

					if (dynamic_cast <Transform*> (lowest .getValue ()))
					{
						node = lowest;
						break;
					}
				}
			}
			else
			{
				// Find highest Transform
			
				for (const auto & object : hierarchy)
				{
					const SFNode highest (object);

					if (not highest)
						continue;

					if (highest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
						continue;

					if (not node)
						node = highest;

					if (dynamic_cast <Transform*> (highest .getValue ()))
					{
						node = highest;
						break;
					}
				}

				// If highest is a LayerSet, no Transform is found and we search for the highest X3DChildNode.

				if (x3d_cast <LayerSet*> (node))
				{
					for (const auto & object : hierarchy)
					{
						const SFNode highest (object);

						if (not highest)
							continue;

						if (highest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
							continue;

						if (x3d_cast <X3DChildNode*> (highest))
						{
							node = highest;
							break;
						}
					}
				}
			}

			if (node)
			{
				if (isSelected (node))
				{
					if (getMode () == Selection::MULTIPLE)
						removeChildren ({ node });
				}
				else
				{
					if (getMode () == Selection::MULTIPLE)
						addChildren ({ node });
					else
						setChildren ({ node });

					selectedTime = getCurrentTime ();
					return true;
				}
			}
		}
	}
	
	return false;
}

void
Selection::clear ()
{
	removeChildren (MFNode (children)); // Make copy because we erase in children.
}

} // X3D
} // titania

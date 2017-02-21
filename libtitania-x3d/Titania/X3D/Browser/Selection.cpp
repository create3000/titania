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

#include "Selection.h"

#include "../Browser/Core/Cast.h"
#include "../Browser/ContextLock.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Grouping/Transform.h"
#include "../Components/Layering/LayerSet.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Shape/X3DShapeNode.h"
#include "../Components/Rendering/X3DGeometryNode.h"
#include "../Editing/X3DEditor.h"
#include "../Execution/X3DExecutionContext.h"

#include "../Tools/Core/X3DToolObject.h"

namespace titania {
namespace X3D {

const ComponentType Selection::component      = ComponentType::TITANIA;
const std::string   Selection::typeName       = "Selection";
const std::string   Selection::containerField = "selection";

Selection::Selection (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	            enabled (false),
	     selectMultiple (false),
	       selectLowest (false),
	     selectGeometry (false),
	               over (),
	             active (),
	          touchTime (),
	              nodes (),
	       selectedNode (),
	          hierarchy (),
	clearHierarchyState (true)
{
	addType (X3DConstants::Selection);

	addChildObjects (enabled,
	                 selectMultiple,
	                 selectLowest,
	                 selectGeometry,
	                 over,
	                 active,
	                 touchTime,
	                 nodes,
	                 selectedNode,
	                 hierarchy);
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

	getBrowser () -> initialized () .addInterest (&Selection::clearNodes, this);
}

bool
Selection::isSelected (const SFNode & node) const
{
	return std::find (nodes .begin (), nodes .end (), node) not_eq nodes .end ();
}

void
Selection::setSelectGeometry (const bool value)
{
	if (value == selectGeometry)
		return;

	selectGeometry = value;

	if (selectGeometry)
		setNodes (getGeometries (nodes));

	else
	{
		MFNode transforms;

		for (const auto & node : nodes)
		{
			for (const auto & hierarchy : getHierarchies (node))
				transforms .emplace_back (getTransform (MFNode (hierarchy .begin (), hierarchy .end ())));
		}

		setNodes (transforms);
	}
}

void
Selection::addNodes (const MFNode & value)
{
	if (clearHierarchyState)
		clearHierarchy ();

	try
	{
		ContextLock lock (getBrowser ());

		for (const auto & node : selectGeometry ? getGeometries (value) : value)
		{
			if (not node)
				continue;

			if (node -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
				continue;

			if (isSelected (node))
				continue;

			nodes .emplace_back (node);
			node -> addTool ();
		}
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }
}

void
Selection::removeNodes (const MFNode & value)
{
	try
	{
		ContextLock lock (getBrowser ());

		if (clearHierarchyState)
			clearHierarchy ();

		for (const auto & node : value)
		{
			if (node)
			{
				nodes .erase (std::remove (nodes .begin (),
				                           nodes .end (),
				                           node),
				              nodes .end ());
	
				node -> removeTool ();
			}
		}
	}
	catch (const Error <INVALID_OPERATION_TIMING> & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Selection::clearNodes ()
{
	clearHierarchy ();

	removeNodes (MFNode (nodes)); // Make copy because we erase in children.
}

void
Selection::setNodes (const MFNode & value)
{
	clearHierarchyState = false;

	if (value .size () == 1)
	{
		const auto iter = std::find (hierarchy .begin (), hierarchy .end (), value [0]);

		if (iter == hierarchy .end ())
			clearHierarchyState = true;

		else
			selectedNode = value [0];
	}
	else
		clearHierarchyState = true;

	if (nodes .empty ())
	{
		addNodes (value);
		return;
	}

	MFNode sortedNodes = nodes;
	MFNode sortedValue = selectGeometry ? getGeometries (value) : value;
	MFNode difference;
	
	std::sort (sortedNodes .begin (), sortedNodes .end ());
	std::sort (sortedValue .begin (), sortedValue .end ());

	// Remove

	std::set_difference (sortedNodes .begin (), sortedNodes .end (),
	                     sortedValue .begin (), sortedValue .end (),
	                     std::back_inserter (difference));

	removeNodes (difference);
	
	// Add
	
	difference .clear ();

	std::set_difference (sortedValue .begin (), sortedValue .end (),
	                     sortedNodes .begin (), sortedNodes .end (),
	                     std::back_inserter (difference));

	addNodes (difference);

	clearHierarchyState = true;
}

MFNode
Selection::getParents ()
{
	if (selectGeometry)
		return MFNode ();

	if (hierarchy .empty ())
		return MFNode ();

	if (not selectedNode)
		return MFNode ();

	auto iter = std::find (hierarchy .rbegin (), hierarchy .rend (), selectedNode);

	if (iter == hierarchy .rend ())
		return MFNode ();

	++ iter;

	if (iter == hierarchy .rend ())
		return MFNode ();

	return MFNode ({ *iter });
}

MFNode
Selection::getChildren ()
{
	if (selectGeometry)
		return MFNode ();

	if (hierarchy .empty ())
		return MFNode ();

	if (not selectedNode)
		return MFNode ();

	auto iter = std::find (hierarchy .begin (), hierarchy .end (), selectedNode);

	if (iter == hierarchy .end ())
		return MFNode ();

	++ iter;

	if (iter == hierarchy .end ())
		return MFNode ();

	return MFNode ({ *iter });
}

bool
Selection::selectNode ()
{
	if (not getEnabled ())
		return false;

	// Selected highest or lowest Node, or clear selection.

	if (getBrowser () -> getHits () .empty ())
	{
		if (not selectGeometry)
			clearNodes ();

		return false;
	}

	// Get selected node.

	const auto & nearestHit = getBrowser () -> getNearestHit ();

	SFNode node;

	if (selectGeometry)
		node = nearestHit -> shape -> getGeometry ();

	else
		node = getTransform (nearestHit -> hierarchy);

	// Select node or remove frpm selection.

	if (node)
	{
		if (isSelected (node))
		{
		   if (selectGeometry)
		      setHierarchy (node, nearestHit -> hierarchy);

			else if (selectMultiple)
				removeNodes ({ node });

			else
				clearHierarchy ();
	
			return false;
		}
		else
		{
			if (selectMultiple)
				addNodes ({ node });
			else
				setNodes ({ node });

			setHierarchy (node, nearestHit -> hierarchy);

			selectedNode = node;

			touchTime = getCurrentTime ();

			return true;
		}
	}

	return false;
}

void
Selection::setHierarchy (const SFNode & node, const MFNode & otherHierarchy)
{
	// Update hierarchy.

	selectedNode = node;

	hierarchy = otherHierarchy;
}

void
Selection::clearHierarchy ()
{
	selectedNode = nullptr;

	hierarchy .clear ();
}

X3D::MFNode
Selection::getGeometries (const X3D::MFNode & nodes) const
{
	static const std::set <NodeType> geometryTypes = { X3D::X3DConstants::X3DGeometryNode };
	static const std::set <NodeType> protoTypes    = { X3D::X3DConstants::X3DPrototypeInstance };

	auto geometryNodes = X3DEditor::getNodes <X3D::X3DBaseNode> (nodes, geometryTypes);

	const auto protoInstances = X3DEditor::getNodes <X3D::X3DBaseNode> (nodes, protoTypes);

	for (const auto & protoInstance : protoInstances)
	{
		try
		{
			const auto innerNode = protoInstance -> getInnerNode ();

			if (innerNode -> isType (geometryTypes))
				geometryNodes .emplace_back (protoInstance);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return geometryNodes;
}

SFNode
Selection::getTransform (const MFNode & hierarchy) const
{
	SFNode node;

	if (selectLowest)
	{
		// Find lowes Transform.
	
		for (const auto & lowest : basic::make_reverse_range (hierarchy))
		{
			if (not lowest)
				continue;
			
			if (lowest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
				continue;
				
			if (not node)
				node = lowest;

			if (dynamic_cast <Transform*> (lowest .getValue ()))
				return lowest;
		}
	}
	else
	{
		// Find highest Transform.
	
		for (const auto & highest : hierarchy)
		{
			if (not highest)
				continue;

			if (highest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
				continue;
	
			if (not node)
				node = highest;
	
			if (dynamic_cast <Transform*> (highest .getValue ()))
				return highest;
		}
	
		// If highest is a LayerSet, no Transform is found and we search for the highest X3DChildNode.
	
		if (x3d_cast <LayerSet*> (node))
		{
			for (const auto & highest : hierarchy)
			{
				if (not highest)
					continue;

				if (highest -> isPrivate ())
					continue;

				if (highest -> getExecutionContext () not_eq getBrowser () -> getExecutionContext ())
					continue;
	
				if (x3d_cast <X3DChildNode*> (highest))
					return highest;
			}
		}
	}

	return node;
}

Hierarchies
Selection::getHierarchies (const SFNode & node) const
{
	return find (getBrowser () -> getExecutionContext () -> getRootNodes (),
		          node,
		          TRAVERSE_ROOT_NODES |
		          TRAVERSE_PROTOTYPE_INSTANCES |
		          TRAVERSE_INLINE_NODES |
		          TRAVERSE_TOOL_OBJECTS |
		          TRAVERSE_VISIBLE_NODES);
}

} // X3D
} // titania

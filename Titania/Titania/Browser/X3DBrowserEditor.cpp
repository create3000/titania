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

#include "BrowserWindow.h"

namespace titania {
namespace puck {

X3DBrowserEditor::X3DBrowserEditor (const basic::uri & worldURL) :
	X3DBrowserWidget (worldURL),
	          edited (false)
{ }

void
X3DBrowserEditor::addNode (const std::string & typeName)
throw (X3D::Error <X3D::INVALID_NAME>)
{
	auto scene = getBrowser () -> getExecutionContext ();
	auto node  = scene -> createNode (typeName);

	node -> setup ();
	scene -> getRootNodes () .emplace_back (node);

	setEdited (true);
}

void
X3DBrowserEditor::removeNode (const X3D::SFNode & node)
throw (X3D::Error <X3D::INVALID_NODE>)
{
	__LOG__ << node -> getTypeName () << std::endl;

	if (not node)
		throw X3D::Error <X3D::INVALID_NODE> ("Node is empty.");

	auto scene = getBrowser () -> getExecutionContext ();

	// Remove node

	removeNode (scene, node);

	__LOG__ << node -> getTypeName () << std::endl;

	// Delete children of node

	std::set <X3D::SFNode> children;

	traverse (node, [&children] (const X3D::SFNode & node, X3D::MFNode* const mfnode, X3D::SFNode* const sfnode, size_t)
	          {
	             if (*sfnode)
						 children .insert (*sfnode);

	             return false;
				 });

	__LOG__ << children .size () << std::endl;

	for (const auto & child : children)
	{
		if (not traverse (scene, [&child] (const X3D::SFNode &, X3D::MFNode* const, X3D::SFNode* const sfnode, size_t)
		                  {
		                     return child == *sfnode;
								}))
		{
			removeExportedNode (scene, child);
			removeNamedNode (scene, child);
			deleteRoutes (scene, child);
		}
	}

	setEdited (true);

	__LOG__ << std::endl;
}

void
X3DBrowserEditor::removeNode (const X3D::X3DSFNode <X3D::Scene> & scene, const X3D::SFNode & node)
{
	__LOG__ << node -> getTypeName () << std::endl;

	removeNode (scene .getValue (), node);

	// Remove exported node

	removeExportedNode (scene, node);

	__LOG__ << std::endl;
}

void
X3DBrowserEditor::removeExportedNode (const X3D::X3DSFNode <X3D::Scene> & scene, const X3D::SFNode & node)
{
	__LOG__ << node -> getTypeName () << std::endl;

	std::deque <std::string> exportedNames;

	for (const auto & exportedNode : scene -> getExportedNodes ())
	{
		if (exportedNode -> getNode () -> getName () == node -> getName ())
			exportedNames .emplace_back (exportedNode -> getExportedName ());
	}

	for (const auto & exportedName : exportedNames)
		scene -> removeExportedNode (exportedName);

	__LOG__ << std::endl;
}

void
X3DBrowserEditor::removeNode (X3D::X3DExecutionContext* const executionContext, const X3D::SFNode & node)
{
	__LOG__ << node -> getTypeName () << std::endl;

	// Delete node in scene graph

	traverse (executionContext, [&node] (const X3D::SFNode &, X3D::MFNode* const mfnode, X3D::SFNode* const sfnode, size_t)
	          {
	             if (node not_eq * sfnode)
						 return false;

	             if (mfnode)
						 mfnode -> erase (std::remove (mfnode -> begin (), mfnode -> end (), *sfnode), mfnode -> end ());

	             else
						 sfnode -> setValue (nullptr);

	             return false;
				 });

	// Remove named node

	removeNamedNode (executionContext, node);

	// Delete routes from and to node

	deleteRoutes (executionContext, node);

	__LOG__ << std::endl;
}

void
X3DBrowserEditor::removeNamedNode (X3D::X3DExecutionContext* const executionContext, const X3D::SFNode & node)
{
	__LOG__ << node .getValue () << std::endl;
	__LOG__ << node -> getTypeName () << std::endl;

	try
	{
		if (executionContext -> getNamedNode (node -> getName ()) == node)
			executionContext -> removeNamedNode (node -> getName ());
	}
	catch (const X3D::X3DError &)
	{ }

	__LOG__ << std::endl;
}

void
X3DBrowserEditor::deleteRoutes (X3D::X3DExecutionContext* const executionContext, const X3D::SFNode & node)
{
	__LOG__ << node -> getTypeName () << std::endl;

	std::deque <X3D::X3DSFNode <X3D::Route>> routes;

	for (const auto & route : executionContext -> getRoutes ())
	{
		if (route -> getSourceNode () == node or route -> getDestinationNode () == node)
			routes .emplace_back (route);
	}

	for (const auto & route : routes)
		executionContext -> deleteRoute (route);

	__LOG__ << std::endl;
}

bool
X3DBrowserEditor::traverse (X3D::X3DExecutionContext* const executionContext, const TraverseCallback & callback)
{
	X3D::ChildObjectSet seen;
	size_t              index = 0;

	auto & rootNodes = executionContext -> getRootNodes ();
	auto   array     = rootNodes;

	for (auto & rootNode : array)
	{
		if (traverse (rootNode, callback, seen))
			return true;

		if (callback (executionContext, &rootNodes, &rootNodes [index], index))
			return true;

		++ index;
	}

	return false;
}

bool
X3DBrowserEditor::traverse (const X3D::SFNode & node, const TraverseCallback & callback)
{
	X3D::ChildObjectSet seen;

	return traverse (node, callback, seen);
}

bool
X3DBrowserEditor::traverse (const X3D::SFNode & node, const TraverseCallback & callback, X3D::ChildObjectSet & seen)
{
	if (not node)
		return false;

	if (not seen .insert (node) .second)
		return false;

	for (const auto & field : node -> getFieldDefinitions ())
	{
		switch (field -> getType ())
		{
			case X3D::X3DConstants::SFNode :
				{
					auto sfnode = static_cast <X3D::SFNode*> (field);

					if (traverse (*sfnode, callback, seen))
						return true;

					if (callback (node, nullptr, sfnode, 0))
						return true;

					break;
				}
			case X3D::X3DConstants::MFNode:
			{
				auto   mfnode = static_cast <X3D::MFNode*> (field);
				auto   array  = *mfnode;
				size_t index  = 0;

				for (auto & value : array)
				{
					if (traverse (value, callback, seen))
						return true;

					if (callback (node, mfnode, &(*mfnode) [index], index))
						return true;

					++ index;
				}

				break;
			}
			default:
				break;
		}
	}

	return false;
}

X3D::SFNode
X3DBrowserEditor::groupNodes (const X3D::MFNode & nodes)
throw (X3D::Error <X3D::INVALID_NODE>)
{
	__LOG__ << std::endl;

	if (nodes .empty ())
		throw X3D::Error <X3D::INVALID_NODE> ("Nodes are empty.");

	auto scene     = getBrowser () -> getExecutionContext ();
	auto node      = scene -> createNode ("Transform");
	auto transform = dynamic_cast <X3D::Transform*> (node .getValue ());

	for (const auto & child : nodes)
	{
		if (not child)
			continue;

		// Adjust transformation
		{
			X3D::Matrix4f          childModelViewMatrix = findModelViewMatrix (child .getValue ());
			X3D::X3DTransformNode* transform            = dynamic_cast <X3D::X3DTransformNode*> (child .getValue ());

			if (transform)
			{
				childModelViewMatrix .multLeft (transform -> getMatrix ());

				transform -> setMatrix (childModelViewMatrix);
			}
		}

		// Add to group

		child -> remove ({ &nodes });

		transform -> children () .emplace_back (child);
	}

	node -> setup ();
	scene -> getRootNodes () .emplace_back (node);

	setEdited (true);

	return node;
}

X3D::MFNode
X3DBrowserEditor::ungroupNode (const X3D::SFNode & group)
throw (X3D::Error <X3D::INVALID_NODE>)
{
	if (not group)
		throw X3D::Error <X3D::INVALID_NODE> ("Group node is emtpy.");

	auto children = getGroupingField (group);

	if (not children)
		throw X3D::Error <X3D::INVALID_NODE> ("Not a grouping node.");

	X3D::MFNode result;

	for (const auto & child : *children)
	{
		if (not child)
			continue;

		// Adjust transformation

		X3D::Matrix4f          childModelViewMatrix = findModelViewMatrix (child .getValue ());
		X3D::X3DTransformNode* transform            = dynamic_cast <X3D::X3DTransformNode*> (child .getValue ());

		if (transform)
		{
			childModelViewMatrix .multLeft (transform -> getMatrix ());

			transform -> setMatrix (childModelViewMatrix);
		}

		// Add to layer

		getBrowser () -> getExecutionContext () -> getRootNodes () .emplace_back (child);

		result .emplace_back (child);
	}

	group -> remove ({ &group });

	setEdited (true);

	return result;
}

void
X3DBrowserEditor::addToGroup (const X3D::SFNode & group, const X3D::MFNode & nodes)
throw (X3D::Error <X3D::INVALID_NODE>)
{
	__LOG__ << std::endl;

	if (not group)
		throw X3D::Error <X3D::INVALID_NODE> ("Group node is emtpy.");

	auto children = getGroupingField (group);

	if (not children)
		throw X3D::Error <X3D::INVALID_NODE> ("Not a grouping node.");

	// Get group modelview matrix

	X3D::Matrix4f          groupModelViewMatrix = findModelViewMatrix (group .getValue ());
	X3D::X3DTransformNode* transform            = dynamic_cast <X3D::X3DTransformNode*> (group .getValue ());

	if (transform)
		groupModelViewMatrix .multLeft (transform -> getMatrix ());

	// Add children to group

	for (const auto & child : nodes)
	{
		if (not child)
			continue;

		if (child == group)
			continue;

		// Adjust transformation

		X3D::Matrix4f          childModelViewMatrix = findModelViewMatrix (child .getValue ());
		X3D::X3DTransformNode* transform            = dynamic_cast <X3D::X3DTransformNode*> (child .getValue ());

		if (transform)
		{
			try
			{
				childModelViewMatrix .multLeft (transform -> getMatrix ());
				childModelViewMatrix .multRight (~groupModelViewMatrix);

				transform -> setMatrix (childModelViewMatrix);
			}
			catch (const std::domain_error & error)
			{ }
		}

		// Add to group

		child -> remove ({ &group, &nodes, children });

		children -> emplace_back (child);
	}

	setEdited (true);
}

void
X3DBrowserEditor::detachFromGroup (const X3D::SFNode & node, bool detachToLayer0)
throw (X3D::Error <X3D::INVALID_NODE>)
{
	__LOG__ << std::endl;

	if (not node)
		throw X3D::Error <X3D::INVALID_NODE> ("Node is empty.");

	auto layers = dynamic_cast <X3D::X3DNode*> (node .getValue ()) -> getLayers ();

	// Adjust transformation

	X3D::Matrix4f          nodeModelViewMatrix = findModelViewMatrix (node .getValue ());
	X3D::X3DTransformNode* transform           = dynamic_cast <X3D::X3DTransformNode*> (node .getValue ());

	if (transform)
	{
		nodeModelViewMatrix .multLeft (transform -> getMatrix ());

		transform -> setMatrix (nodeModelViewMatrix);
	}

	// Add to layers

	node -> remove ({ &node });

	if (detachToLayer0)
		getBrowser () -> getExecutionContext () -> getRootNodes () .emplace_back (node);

	else
	{
		for (const auto & layer : layers)
		{
			if (layer -> isLayer0 ())
				getBrowser () -> getExecutionContext () -> getRootNodes () .emplace_back (node);

			else
				layer -> children () .emplace_back (node);
		}
	}

	setEdited (true);
}

X3D::SFNode
X3DBrowserEditor::createParentGroup (const X3D::SFNode & child)
throw (X3D::Error <X3D::INVALID_NODE>)
{
	__LOG__ << std::endl;

	if (not child)
		throw X3D::Error <X3D::INVALID_NODE> ("Node is empty.");

	// Replace node with Transform

	auto group     = getBrowser () -> getExecutionContext () -> createNode ("Transform");
	auto transform = dynamic_cast <X3D::Transform*> (group .getValue ());

	transform -> children () .emplace_back (child);
	transform -> setup ();

	transform -> replace (child, { &child, &transform -> children () });

	setEdited (true);

	return group;
}

X3D::Matrix4f
X3DBrowserEditor::findModelViewMatrix (X3D::X3DBaseNode* const node) const
{
	X3D::Matrix4f modelViewMatix;

	std::set <X3D::X3DBaseNode*> seen;

	for (const auto & parentNode : getParentNodes (node))
	{
		if (findModelViewMatrix (parentNode, modelViewMatix, seen))
			break;
	}

	return modelViewMatix;
}

bool
X3DBrowserEditor::findModelViewMatrix (X3D::X3DBaseNode* const node, X3D::Matrix4f & modelViewMatix, std::set <X3D::X3DBaseNode*> & seen) const
{
	if (not seen .insert (node) .second)
		return false;

	auto browser = dynamic_cast <X3D::X3DBrowser*> (node);

	if (browser)
		return false;

	auto scene = dynamic_cast <X3D::Scene*> (node);

	if (scene)
		return false;

	auto layer = dynamic_cast <X3D::X3DLayerNode*> (node);

	if (layer)
		return true;

	for (const auto & parentNode : getParentNodes (node))
	{
		if (findModelViewMatrix (parentNode, modelViewMatix, seen))
		{
			auto transform = dynamic_cast <X3D::X3DTransformNode*> (node);

			if (transform)
				modelViewMatix .multLeft (transform -> getMatrix ());

			return true;
		}
	}

	return false;
}

std::deque <X3D::X3DBaseNode*>
X3DBrowserEditor::getParentNodes (X3D::X3DBaseNode* const node) const
{
	std::deque <X3D::X3DBaseNode*> parentNodes;

	for (const auto & parent : node -> getParents ())
	{
		auto sfnode = dynamic_cast <X3D::SFNode*> (parent);

		if (sfnode)
		{
			for (const auto & secondParent : sfnode -> getParents ())
			{
				auto mfnode = dynamic_cast <X3D::MFNode*> (secondParent);

				if (mfnode)
				{
					for (const auto & thirdParent : mfnode -> getParents ())
					{
						auto baseNode = dynamic_cast <X3D::X3DBaseNode*> (thirdParent);

						if (baseNode)
							parentNodes .emplace_back (baseNode);
					}

					continue;
				}

				auto baseNode = dynamic_cast <X3D::X3DBaseNode*> (secondParent);

				if (baseNode)
				{
					parentNodes .emplace_back (baseNode);
					continue;
				}
			}
		}
	}

	return parentNodes;
}

X3D::MFNode*
X3DBrowserEditor::getGroupingField (const X3D::SFNode & node) const
{
	X3D::X3DFieldDefinition* field = nullptr;

	try
	{
		field = node -> getField ("children");
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		try
		{
			field = node -> getField ("layers");
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{ }
	}

	return dynamic_cast <X3D::MFNode*> (field);
}

} // puck
} // titania

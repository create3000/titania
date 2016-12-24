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

#include "../Browser/Browser.h"

#include "X3DEditor.h"

#include "../Basic/Traverse.h"
#include "../Browser/X3DBrowser.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Geometry3D/IndexedFaceSet.h"
#include "../Components/Geospatial/GeoCoordinate.h"
#include "../Components/Grouping/X3DTransformNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/NURBS/CoordinateDouble.h"
#include "../Components/Networking/Inline.h"
#include "../Components/Rendering/Color.h"
#include "../Components/Rendering/ColorRGBA.h"
#include "../Components/Rendering/Coordinate.h"
#include "../Components/Rendering/Normal.h"
#include "../Components/Shape/X3DShapeNode.h"
#include "../Components/Texturing/TextureCoordinate.h"
#include "../Components/Texturing/MultiTextureCoordinate.h"
#include "../Components/Texturing3D/TextureCoordinate3D.h"
#include "../Components/Texturing3D/TextureCoordinate4D.h"
#include "../Execution/ExportedNode.h"
#include "../Execution/ImportedNode.h"
#include "../Execution/X3DExecutionContext.h"
#include "../Execution/World.h"
#include "../Prototype/ExternProtoDeclaration.h"
#include "../Prototype/ProtoDeclaration.h"

#include <Titania/Utility/Map.h>

#include <regex>

namespace titania {
namespace X3D {

/***
 *
 *
 *
 * Import/Export handling
 *
 *
 *
 */

MFNode
X3DEditor::importScene (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & field, const X3DScenePtr & scene, const UndoStepPtr & undoStep) const
{
	try
	{
		const size_t size = field .size ();

		using resize = void (MFNode::*) (const MFNode::size_type);

		undoStep -> addObjects (parent);
		undoStep -> addUndoFunction ((resize) & MFNode::resize, std::ref (field), size);

		// Imported scene

		MFNode importedNodes;

		executionContext -> import (scene, importedNodes);

		const auto undoRemoveNodes = std::make_shared <UndoStep> ();

		removeNodesFromScene (executionContext, importedNodes, false, undoRemoveNodes);

		undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNodes);
		undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNodes);
		undoRemoveNodes -> undo ();

		using append = X3DArrayField <SFNode> &(MFNode::*) (const X3DArrayField <SFNode>&);

		undoStep -> addRedoFunction ((append) & MFNode::append, std::ref (field), importedNodes);
		field .append (importedNodes);

		return importedNodes;
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		return MFNode ();
	}
}

std::string
X3DEditor::cutNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes_, const UndoStepPtr & undoStep) const
{
	MFNode nodes = nodes_;

	// Detach from group

	detachFromGroup (executionContext, nodes, true, undoStep);

	// Set clipboard text

	const auto string = exportNodes (executionContext, nodes, true);

	// Remove nodes

	removeNodesFromScene (executionContext, nodes, true, undoStep);

	return string;
}

std::string
X3DEditor::copyNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes_) const
{
	MFNode nodes = nodes_;

	// Detach from group

	const auto undoDetachFromGroup = std::make_shared <UndoStep> ();

	const_cast <X3DEditor*> (this) -> detachFromGroup (executionContext, nodes, true, undoDetachFromGroup);

	// Set clipboard text

	const auto string = exportNodes (executionContext, nodes, true);

	// Undo detach from group

	undoDetachFromGroup -> undo ();

	return string;
}

std::string
X3DEditor::exportNodes (const X3DExecutionContextPtr & executionContext, MFNode & nodes, const bool identifier) const
{
	std::ostringstream osstream;

	exportNodes (executionContext, osstream, nodes, identifier);

	return osstream .str ();
}

void
X3DEditor::exportNodes (const X3DExecutionContextPtr & executionContext, std::ostream & ostream, MFNode & nodes, const bool identifier) const
{
	// Find proto declarations

	const auto protoNodes = getUsedPrototypes (executionContext, nodes);
	const auto routes     = getConnectedRoutes (executionContext, nodes);

	// Generate text

	ostream .imbue (std::locale::classic ());

	ostream
		<< "#" << LATEST_VERSION << " utf8 " << executionContext -> getBrowser () -> getName ()
		<< std::endl
		<< std::endl;

	if (identifier)
	{
		ostream
			<< "META \"titania-identifier\" " << SFString (executionContext -> getWorldURL () .str ())
			<< std::endl
			<< std::endl;
	}

	Generator::CompactStyle ();
	Generator::EnterScope ();

	if (not protoNodes .empty ())
	{
		for (const auto & protoNode : protoNodes)
			ostream << protoNode << std::endl;

		ostream << std::endl;
	}

	for (const auto & node : nodes)
		ostream << node << std::endl;

	if (not routes .empty ())
	{
		ostream << std::endl;

		for (const auto & route : routes)
			ostream << *route << std::endl;
	}

	Generator::LeaveScope ();
}

std::vector <X3DProtoDeclarationNodePtr>
X3DEditor::getUsedPrototypes (const X3DExecutionContextPtr & executionContext, MFNode & nodes) const
{
	std::map <X3DProtoDeclarationNodePtr, size_t> protoIndex;

	traverse (nodes, [&] (SFNode & node)
	          {
	             const X3DPrototypeInstancePtr protoInstance (node);

	             if (protoInstance)
	             {
	                traverse (node, [&] (SFNode & node)
	                          {
	                             if (node == protoInstance)
											  return true;

	                             const X3DPrototypeInstancePtr child (node);

	                             if (child)
	                             {
	                                try
	                                {
	                                   if (child -> getProtoNode () == executionContext -> findProtoDeclaration (child -> getTypeName (), AvailableType { }))
													  protoIndex .emplace (child -> getProtoNode (), protoIndex .size ());
											  }
	                                catch (const X3DError &)
	                                { }
										  }

	                             return true;
									  },
	                          true,
	                          TRAVERSE_PROTOTYPE_INSTANCES);

	                protoIndex .emplace (protoInstance -> getProtoNode (), protoIndex .size ());
					 }

	             return true;
				 });

	std::vector <X3DProtoDeclarationNodePtr> protoNodes;

	for (const auto & protoNode : basic::reverse (protoIndex))
		protoNodes .emplace_back (std::move (protoNode .second));

	return protoNodes;
}

std::vector <Route*>
X3DEditor::getConnectedRoutes (const X3DExecutionContextPtr & executionContext, MFNode & nodes) const
{
	// Create node index

	std::set <SFNode> nodeIndex;

	traverse (nodes, [&nodeIndex] (SFNode & node)
	          {
	             nodeIndex .emplace (node);
	             return true;
				 });

	// Find connected routes

	std::vector <Route*> routes;

	traverse (nodes, [&] (SFNode & node)
	          {
	             for (const auto & field: node -> getFieldDefinitions ())
	             {
	                for (const auto & route: field -> getOutputRoutes ())
	                {
	                   if (route -> getExecutionContext () not_eq executionContext)
								 continue;

	                   try
	                   {
	                      if (nodeIndex .count (route -> getDestinationNode ()))
									 routes .emplace_back (route);
							 }
	                   catch (const X3DError &)
	                   { }
						 }
					 }

	             return true;
				 });

	return routes;
}

/***
 *
 *
 *
 * Prototype handling
 *
 *
 *
 */

void
X3DEditor::removeUnusedPrototypes (const X3DExecutionContextPtr & executionContext, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (executionContext);

	// Get ExternProtos and Prototypes

	std::map <ExternProtoDeclarationPtr, size_t> externProtos;

	for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
		externProtos .emplace (externProto, externProtos .size ());

	std::map <ProtoDeclarationPtr, size_t> prototypes;

	for (const auto & prototype : executionContext -> getProtoDeclarations ())
		prototypes .emplace (prototype, prototypes .size ());

	// Find proto declaration not used in prototypes or scene.

	removeUsedPrototypes (executionContext, externProtos, prototypes);

	// Remove ExternProtos.

	const auto externProtoIndex = basic::reverse (externProtos);

	for (const auto & pair : basic::make_reverse_range (externProtoIndex))
	{
		const auto & externProto = pair .second;

		undoStep -> addUndoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, externProto -> getName (), externProto);
		undoStep -> addRedoFunction (&X3DExecutionContext::removeExternProtoDeclaration, executionContext, externProto -> getName ());

		executionContext -> removeExternProtoDeclaration (externProto -> getName ());
	}

	// Remove Prototypes.

	const auto prototypeIndex = basic::reverse (prototypes);

	for (const auto & pair : basic::make_reverse_range (prototypeIndex))
	{
		const auto & prototype = pair .second;

		undoStep -> addUndoFunction (&X3DExecutionContext::updateProtoDeclaration, executionContext, prototype -> getName (), prototype);
		undoStep -> addRedoFunction (&X3DExecutionContext::removeProtoDeclaration, executionContext, prototype -> getName ());

		executionContext -> removeProtoDeclaration (prototype -> getName ());
	}
}

void
X3DEditor::removeUsedPrototypes (const X3DExecutionContextPtr & executionContext,
                                 std::map <ExternProtoDeclarationPtr, size_t> & externProtos,
                                 std::map <ProtoDeclarationPtr, size_t> & prototypes) const
{
	traverse (executionContext -> getRootNodes (), [&] (SFNode & node)
	          {
	             for (const auto & type: basic::make_reverse_range (node -> getType ()))
	             {
	                switch (type)
	                {
							 case X3DConstants::X3DPrototypeInstance:
								 {
								    const X3DPrototypeInstancePtr instance (node);

								    switch (instance -> getProtoNode () -> getType () .back ())
								    {
										 case  X3DConstants::ExternProtoDeclaration:
											 {
											    const ExternProtoDeclarationPtr externProto (instance -> getProtoNode ());

											    externProtos .erase (externProto);
											    break;
											 }
										 case  X3DConstants::ProtoDeclaration:
											 {
											    const ProtoDeclarationPtr prototype (instance -> getProtoNode ());

											    prototypes .erase (prototype);

											    // This is not neccessary:
											    // removeUsedPrototypes (prototype, externProtos, prototypes);
											    break;
											 }
										 default:
											 break;
									 }

								    break;
								 }
							 default:
								 break;
						 }
					 }

	             return true;
				 },
	          true, TRAVERSE_PROTOTYPE_INSTANCES);
}

SFNode
X3DEditor::addPrototypeInstance (const X3DExecutionContextPtr & executionContext, const std::string & name, const UndoStepPtr & undoStep) const
{
	const SFNode instance (executionContext -> createPrototypeInstance (name));

	executionContext -> addUninitializedNode (instance);
	executionContext -> realize ();

	pushBackIntoArray (SFNode (executionContext), executionContext -> getRootNodes (), instance, undoStep);

	return instance;
}

/***
 *
 *
 *
 * Replace operations
 *
 *
 *
 */

/***
 *  Replaces in the entire scene graph of current execution context @a node by @a newValue.
 */
void
X3DEditor::replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode & node, const SFNode & newValue, const UndoStepPtr & undoStep) const
{
	replaceNodes (executionContext, SFNode (executionContext), executionContext -> getRootNodes (), node, newValue, undoStep);

	traverse (executionContext -> getRootNodes (), [&] (SFNode & parent)
	          {
	             for (auto & field: parent -> getFieldDefinitions ())
	             {
	                switch (field -> getType ())
	                {
							 case X3DConstants::SFNode:
								 {
								    const auto sfnode = static_cast <SFNode*> (field);

								    if (*sfnode == node)
										 replaceNode (executionContext, parent, *sfnode, newValue, undoStep);

								    break;
								 }
							 case X3DConstants::MFNode:
								 {
								    const auto mfnode = static_cast <MFNode*> (field);

								    replaceNodes (executionContext, parent, *mfnode, node, newValue, undoStep);
								    break;
								 }
							 default:
								 break;
						 }
					 }

	             return true;
				 });
}

/***
 *  Sets @a sfnode to @a newValue.
 */
void
X3DEditor::replaceNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, SFNode & sfnode, const SFNode & newValue, const UndoStepPtr & undoStep) const
{
	const auto oldValue = sfnode;

	undoStep -> addObjects (parent);

	//

	const auto undoRemoveNode = std::make_shared <UndoStep> ();

	removeNodesFromSceneIfNotExists (executionContext, { newValue }, undoRemoveNode);

	undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	//

	undoStep -> addUndoFunction (&SFNode::setValue, std::ref (sfnode), sfnode);
	undoStep -> addRedoFunction (&SFNode::setValue, std::ref (sfnode), newValue);

	sfnode = newValue;

	removeNodesFromSceneIfNotExists (executionContext, { oldValue }, undoStep);
}

/***
 *  Sets @a mfnode to @a newValue.
 */
void
X3DEditor::replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const MFNode & newValue, const UndoStepPtr & undoStep) const
{
	const auto oldValue = mfnode;

	undoStep -> addObjects (parent);

	//

	const auto undoRemoveNode = std::make_shared <UndoStep> ();

	removeNodesFromSceneIfNotExists (executionContext, newValue, undoRemoveNode);

	undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	//

	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);
	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), newValue);

	mfnode = newValue;

	removeNodesFromSceneIfNotExists (executionContext, oldValue, undoStep);
}

/***
 *  Replaces in @a mfnode all occurences of @a node by @a newValue.
 */
void
X3DEditor::replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const SFNode & node, const SFNode & newValue, const UndoStepPtr & undoStep) const
{
	const auto indices = mfnode .indices_of (node);

	if (indices .empty ())
		return;

	for (const auto & index : indices)
		replaceNode (executionContext, parent, mfnode, index, newValue, undoStep);
}

/***
 *  Sets in @a mfnode at @a index the value to @a newValue.
 */
void
X3DEditor::replaceNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const size_t index, const SFNode & newValue, const UndoStepPtr & undoStep) const
{
	const SFNode oldValue = mfnode [index];

	undoStep -> addObjects (parent);

	//

	const auto undoRemoveNode = std::make_shared <UndoStep> ();

	removeNodesFromSceneIfNotExists (executionContext, { newValue }, undoRemoveNode);

	undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	//

	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

	mfnode [index] = newValue;

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

	removeNodesFromSceneIfNotExists (executionContext, { oldValue }, undoStep);
}

/***
 *
 *
 *
 * Remove operations
 *
 *
 *
 */

void
X3DEditor::removeNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, SFNode & node, const UndoStepPtr & undoStep) const
{
	replaceNode (executionContext, parent, node, nullptr, undoStep);
}

void
X3DEditor::removeNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const size_t index, const UndoStepPtr & undoStep) const
{
	if (index < mfnode .size ())
	{
		const SFNode oldValue = mfnode [index];

		undoStep -> addObjects (parent);
		undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

		mfnode .erase (mfnode .begin () + index);

		undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

		removeNodesFromSceneIfNotExists (executionContext, { oldValue }, undoStep);
	}
}

/***
 *  Removes @a node completely from scene if not exists in scene graph anymore.
 */
void
X3DEditor::removeNodesFromSceneIfNotExists (const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const UndoStepPtr & undoStep) const
{
	MFNode remove;

	for (const auto & node : nodes)
	{
		if (not node)
			continue;

		if (node -> getCloneCount () == 0)
			remove .emplace_back (node);
	}

	if (not remove .empty ())
		removeNodesFromScene (executionContext, remove, false, undoStep);
}

/***
 *  Completely removes @a nodes from @a executionContext.
 */

void
X3DEditor::removeNodesFromScene (const X3DExecutionContextPtr & executionContext, const MFNode & nodes_, const bool removeFromSceneGraph, const UndoStepPtr & undoStep) const
{
	MFNode nodes = nodes_;

	// Remove exported nodes

	if (removeFromSceneGraph)
		removeNodesFromSceneGraph (executionContext, std::set <SFNode> (nodes .begin (), nodes .end ()), undoStep);

	// Delete children of node if not in scene graph

	// Collect children

	std::set <SFNode> children;

	traverse (nodes, [&children] (SFNode & child)
	          {
	             children .emplace (child);
	             return true;
				 },
	          true,
	          TRAVERSE_PROTOTYPE_INSTANCES);

	if (children .size () > nodes .size ())
	{
		// Filter out nodes still in the scene graph.

		traverse (executionContext -> getRootNodes (), [&children] (SFNode & node)
		          {
		             children .erase (node);
		             return true;
					 },
		          true,
		          TRAVERSE_PROTOTYPE_INSTANCES);
	}

	// Remove rest, these are only nodes that are not in the scene graph anymore.
	removeNodesFromExecutionContext (executionContext, children, undoStep, false);
}

void
X3DEditor::removeNodesFromExecutionContext (const X3DExecutionContextPtr & executionContext,
                                            const std::set <SFNode> & nodes,
                                            const UndoStepPtr & undoStep,
                                            const bool doRemoveFromSceneGraph) const
{
	// Remove node from scene graph

	const X3DScenePtr scene (executionContext);

	if (scene)
		removeExportedNodes (scene, nodes, undoStep);

	for (const auto & node : nodes)
		deleteRoutes (executionContext, node, undoStep);

	std::set <X3DPtr <Inline>> inlineNodes;

	for (const auto & node : nodes)
	{
		X3DPtr <Inline> inlineNode (node);

		if (inlineNode and inlineNode -> load ())
			inlineNodes .emplace (inlineNode);
	}

	removeImportedNodes (executionContext, inlineNodes, undoStep);
	removeNamedNodes (executionContext, nodes, undoStep);

	// If it is previously known that the node isn't in the scene graph anymore, it must not removed.

	if (doRemoveFromSceneGraph)
		removeNodesFromSceneGraph (executionContext, nodes, undoStep);

	for (const auto & inlineNode : inlineNodes)
	{
		undoStep -> addUndoFunction (&Inline::requestImmediateLoad, inlineNode);
		undoStep -> addUndoFunction (&Inline::preventNextLoad, inlineNode); // Prevent next load from load field event.

		undoStep -> addUndoFunction (&SFBool::setValue, std::ref (inlineNode -> load ()), true);
		undoStep -> addRedoFunction (&SFBool::setValue, std::ref (inlineNode -> load ()), false);
		inlineNode -> load () = false;
	}

	// Hide node

	for (const auto & node : nodes)
	{
		undoStep -> addUndoFunction (&X3DBaseNode::beginUpdate, node);
		undoStep -> addRedoFunction (&X3DBaseNode::endUpdate,   node);
		node -> endUpdate ();

		using isPrivate = void (X3DBaseNode::*) (const bool);

		undoStep -> addUndoFunction ((isPrivate) & X3DBaseNode::isPrivate, node, false);
		undoStep -> addRedoFunction ((isPrivate) & X3DBaseNode::isPrivate, node, true);
		node -> isPrivate (true);
	}
}

void
X3DEditor::removeNodesFromSceneGraph (const X3DExecutionContextPtr & executionContext, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep) const
{
	const SFNode executionContextNode (executionContext);

	for (const auto & node : nodes)
		removeNode (executionContextNode, executionContext -> getRootNodes (), node, undoStep);

	removeNodesFromSceneGraph (executionContext -> getRootNodes (), nodes, undoStep);
}

void
X3DEditor::removeNodesFromSceneGraph (const MFNode & array, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep) const
{
	traverse (const_cast <MFNode &> (array), [&] (SFNode & parent)
	          {
	             for (auto & field: parent -> getFieldDefinitions ())
	             {
	                switch (field -> getType ())
	                {
							 case X3DConstants::SFNode:
								 {
								    const auto sfnode = static_cast <SFNode*> (field);

								    if (nodes .count (*sfnode))
								    {
								       undoStep -> addObjects (parent);

								       undoStep -> addUndoFunction (&SFNode::setValue, sfnode, *sfnode);
								       undoStep -> addRedoFunction (&SFNode::setValue, sfnode, nullptr);

								       sfnode -> setValue (nullptr);
									 }

								    break;
								 }
							 case X3DConstants::MFNode:
								 {
								    const auto mfnode = static_cast <MFNode*> (field);

								    for (const auto & node: nodes)
										 removeNode (parent, *mfnode, node, undoStep);

								    break;
								 }
							 default:
								 break;
						 }
					 }

	             return true;
				 },
	          true,
	          TRAVERSE_PROTOTYPE_INSTANCES);
}

void
X3DEditor::removeNode (const SFNode & parent, MFNode & mfnode, const SFNode & node, const UndoStepPtr & undoStep) const
{
	if (std::find (mfnode .begin (), mfnode .end (), node) == mfnode .end ())
		return;

	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

	mfnode .remove (node);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);
}

void
X3DEditor::removeExportedNodes (const X3DScenePtr & scene, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep) const
{
	// Remove exported nodes

	for (const auto & pair : ExportedNodeIndex (scene -> getExportedNodes ()))
	{
		try
		{
			const auto & exportedNode = pair .second;
			const auto   localNode    = exportedNode -> getLocalNode ();

			if (nodes .count (localNode))
			{
				undoStep -> addUndoFunction (&X3DScene::updateExportedNode, scene, exportedNode -> getExportedName (), localNode);
				undoStep -> addRedoFunction (&X3DScene::removeExportedNode, scene, exportedNode -> getExportedName ());

				scene -> removeExportedNode (exportedNode -> getExportedName ());
			}
		}
		catch (...)
		{ }
	}
}

void
X3DEditor::removeNamedNodes (const X3DExecutionContextPtr & executionContext, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep) const
{
	// Remove named node

	for (const auto & node : nodes)
	{
		try
		{
			if (executionContext -> getNamedNode (node -> getName ()) == node)
			{
				undoStep -> addUndoFunction (&X3DExecutionContext::updateNamedNode, executionContext, node -> getName (), node);
				undoStep -> addRedoFunction (&X3DExecutionContext::removeNamedNode, executionContext, node -> getName ());

				executionContext -> removeNamedNode (node -> getName ());
			}
		}
		catch (const X3DError &)
		{ }
	}
}

/***
 *  Only pass inline nodes that are loaded and should be unloaded.
 */
void
X3DEditor::removeImportedNodes (const X3DExecutionContextPtr & executionContext, const std::set <X3DPtr <Inline>> & inlineNodes, const UndoStepPtr & undoStep) const
{
	// Remove nodes imported from node

	for (const auto & pair : ImportedNodeIndex (executionContext -> getImportedNodes ()))
	{
		try
		{
			const auto & importedNode = pair .second;
			const auto   inlineNode   = importedNode -> getInlineNode ();

			if (inlineNodes .count (inlineNode))
			{
				// Delete routes.

				try
				{
					deleteRoutes (executionContext, importedNode -> getExportedNode (), undoStep);
				}
				catch (const X3DError &)
				{ }

				// Remove imported node.

				undoStep -> addUndoFunction (&X3DExecutionContext::addImportedNode, executionContext,
				                             inlineNode,
				                             importedNode -> getExportedName (),
				                             importedNode -> getImportedName ());

				undoStep -> addRedoFunction (&X3DExecutionContext::removeImportedNode, executionContext,
				                             importedNode -> getImportedName ());

				executionContext -> removeImportedNode (importedNode -> getImportedName ());
			}
		}
		catch (const X3DError & error)
		{ }
	}
}

/***
 *
 *
 *
 * Named node handling
 *
 *
 *
 */

void
X3DEditor::updateNamedNode (const X3DExecutionContextPtr & executionContext, const std::string & name, const SFNode & node, const UndoStepPtr & undoStep) const
{
	try
	{
		if (name .empty ())
			return;

		if (name == node -> getName ())
			return;

		const auto uniqueName = executionContext -> getUniqueName (name);

		undoStep -> addObjects (executionContext);

		if (node -> getName () .empty ())
			undoStep -> addUndoFunction (&X3DExecutionContext::removeNamedNode, executionContext, uniqueName);
		else
			undoStep -> addUndoFunction (&X3DExecutionContext::updateNamedNode, executionContext, node -> getName (), node);

		undoStep -> addRedoFunction (&X3DExecutionContext::updateNamedNode, executionContext, uniqueName, node);
		executionContext -> updateNamedNode (uniqueName, node);
	}
	catch (...)
	{ }
}

/***
 *
 *
 *
 * Prototype handling
 *
 *
 *
 */

void
X3DEditor::updateProtoDeclaration (const X3DExecutionContextPtr & executionContext,
                                   const std::string & name,
                                   const ProtoDeclarationPtr & prototype,
                                   const UndoStepPtr & undoStep) const
throw (Error <DISPOSED>)
{
	if (name .empty ())
		return;

	if (name == prototype -> getName ())
		return;

	const auto uniqueName = executionContext -> getUniqueProtoName (name);

	undoStep -> addUndoFunction (&X3DExecutionContext::updateProtoDeclaration, executionContext, prototype -> getName (), prototype);
	undoStep -> addRedoFunction (&X3DExecutionContext::updateProtoDeclaration, executionContext, uniqueName, prototype);

	executionContext -> updateProtoDeclaration (uniqueName, prototype);
}

void
X3DEditor::updateExternProtoDeclaration (const X3DExecutionContextPtr & executionContext,
                                         const std::string & name,
                                         const ExternProtoDeclarationPtr & externProto,
                                         const UndoStepPtr & undoStep) const
throw (Error <DISPOSED>)
{
	if (name .empty ())
		return;

	if (name == externProto -> getName ())
		return;

	// Update name.

	const auto uniqueName = executionContext -> getUniqueExternProtoName (name);

	undoStep -> addUndoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, externProto -> getName (), externProto);
	undoStep -> addRedoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, uniqueName, externProto);

	executionContext -> updateExternProtoDeclaration (uniqueName, externProto);

	// Update url.

	if (externProto -> checkLoadState () not_eq COMPLETE_STATE)
		return;

	undoStep -> addUndoFunction (&MFString::setValue, std::ref (externProto -> url ()), externProto -> url ());

	for (auto & URL : externProto -> url ())
	{
		basic::uri uri = URL .str ();

		uri .fragment (externProto -> getProtoDeclaration () -> getName ());

		URL = uri .str ();
	}

	undoStep -> addRedoFunction (&MFString::setValue, std::ref (externProto -> url ()), externProto -> url ());
}

/***
 *
 *
 *
 * Route handling
 *
 *
 *
 */

void
X3DEditor::deleteRoutes (const X3DExecutionContextPtr & executionContext, const SFNode & node, const UndoStepPtr & undoStep) const
{
	// Delete routes from and to node

	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (not field -> getInputRoutes () .empty ())
		{
			for (const auto & route : RouteSet (field -> getInputRoutes ()))
			{
				try
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					deleteRoute (route -> getExecutionContext (),
					             route -> getSourceNode (),
					             route -> getSourceField (),
					             route -> getDestinationNode (),
					             route -> getDestinationField (),
					             undoStep);
				}
				catch (const X3DError &)
				{ }
			}
		}

		if (not field -> getOutputRoutes () .empty ())
		{
			for (const auto & route : RouteSet (field -> getOutputRoutes ()))
			{
				try
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					deleteRoute (route -> getExecutionContext (),
					             route -> getSourceNode (),
					             route -> getSourceField (),
					             route -> getDestinationNode (),
					             route -> getDestinationField (),
					             undoStep);
				}
				catch (const X3DError &)
				{ }
			}
		}
	}
}

void
X3DEditor::addRoute (const X3DExecutionContextPtr & executionContext,
                     const SFNode & sourceNode,
                     const std::string & sourceField,
                     const SFNode & destinationNode,
                     const std::string & destinationField,
                     const UndoStepPtr & undoStep) const
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		using getImportedNode = SFNode          (X3DExecutionContext::*) (const std::string &) const;
		using addRoute        = const RoutePtr &(X3DExecutionContext::*) (const SFNode &, const std::string &, const SFNode &, const std::string &);
		using deleteRoute     = void            (X3DExecutionContext::*) (const SFNode &, const std::string &, const SFNode &, const std::string &);
		using NodeFn          = std::function <SFNode ()>;

		const auto identity = ( [ ] (const SFNode &value){ return value; });
		const auto call     = ( [ ] (const NodeFn &value){ return value (); });

		const NodeFn sourceNodeFn = executionContext -> isImportedNode (sourceNode)
		                            ? NodeFn (std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)))
											 : NodeFn (std::bind (identity, sourceNode));

		const NodeFn destinationNodeFn = executionContext -> isImportedNode (destinationNode)
		                                 ? NodeFn (std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)))
													: NodeFn (std::bind (identity, destinationNode));

		undoStep -> addUndoFunction ((deleteRoute) & X3DExecutionContext::deleteRoute, executionContext,
		                             std::bind (call, sourceNodeFn),
		                             sourceField,
		                             std::bind (call, destinationNodeFn),
		                             destinationField);

		undoStep -> addRedoFunction ((addRoute) & X3DExecutionContext::addRoute, executionContext,
		                             std::bind (call, sourceNodeFn),
		                             sourceField,
		                             std::bind (call, destinationNodeFn),
		                             destinationField);

		executionContext -> addRoute (sourceNode, sourceField, destinationNode, destinationField);
	}
	catch (const X3DError &)
	{ }
}

void
X3DEditor::deleteRoute (const X3DExecutionContextPtr & executionContext,
                        const SFNode & sourceNode,
                        const std::string & sourceField,
                        const SFNode & destinationNode,
                        const std::string & destinationField,
                        const UndoStepPtr & undoStep) const
{
	using getImportedNode = SFNode          (X3DExecutionContext::*) (const std::string &) const;
	using addRoute        = const RoutePtr &(X3DExecutionContext::*) (const SFNode &, const std::string &, const SFNode &, const std::string &);
	using deleteRoute     = void            (X3DExecutionContext::*) (const SFNode &, const std::string &, const SFNode &, const std::string &);

	if (sourceNode -> getScene () -> isPrivate ())
		return;

	if (destinationNode -> getScene () -> isPrivate ())
		return;

	bool sourceImported      = false;
	bool destinationImported = false;

	try
	{
		sourceImported = executionContext -> isImportedNode (sourceNode);
	}
	catch (...)
	{ }

	try
	{
		destinationImported = executionContext -> isImportedNode (destinationNode);
	}
	catch (...)
	{ }

	if (sourceImported and destinationImported)
	{
		undoStep -> addUndoFunction ((addRoute) & X3DExecutionContext::addRoute, executionContext,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3DExecutionContext::deleteRoute, executionContext,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);
	}
	else if (sourceImported and not destinationImported)
	{
		undoStep -> addUndoFunction ((addRoute) & X3DExecutionContext::addRoute, executionContext,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             destinationNode,
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3DExecutionContext::deleteRoute, executionContext,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (sourceNode)),
		                             sourceField,
		                             destinationNode,
		                             destinationField);
	}
	else if (not sourceImported and destinationImported)
	{
		undoStep -> addUndoFunction ((addRoute) & X3DExecutionContext::addRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3DExecutionContext::deleteRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             std::bind ((getImportedNode) & X3DExecutionContext::getImportedNode, executionContext, executionContext -> getLocalName (destinationNode)),
		                             destinationField);
	}
	else
	{
		undoStep -> addUndoFunction ((addRoute) & X3DExecutionContext::addRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             destinationNode,
		                             destinationField);

		undoStep -> addRedoFunction ((deleteRoute) & X3DExecutionContext::deleteRoute, executionContext,
		                             sourceNode,
		                             sourceField,
		                             destinationNode,
		                             destinationField);
	}

	executionContext -> deleteRoute (sourceNode, sourceField, destinationNode, destinationField);
}

std::vector <std::tuple <SFNode, std::string, SFNode, std::string>> 
X3DEditor::getImportedRoutes (const X3DExecutionContextPtr & executionContext, const X3DScenePtr & scene) const
{
	std::vector <std::tuple <SFNode, std::string, SFNode, std::string>>  routes;

	for (const auto & pair : executionContext -> getImportedNodes ())
	{
		try
		{
			const auto & inlineNode = pair .second -> getInlineNode ();

			if (inlineNode -> getInternalScene () not_eq scene)
				continue;

			const auto exportedNode = pair .second -> getExportedNode ();

			for (const auto & field : exportedNode -> getFieldDefinitions ())
			{
				for (const auto & route : field -> getInputRoutes ())
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					try
					{
						routes .emplace_back (route -> getSourceNode (), route -> getSourceField (),
						                      route -> getDestinationNode (), route -> getDestinationField ());
					}
					catch (const X3DError &)
					{ }
				}

				for (const auto & route : field -> getOutputRoutes ())
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					try
					{
						routes .emplace_back (route -> getSourceNode (), route -> getSourceField (),
						                      route -> getDestinationNode (), route -> getDestinationField ());
					}
					catch (const X3DError &)
					{ }
				}
			}
		}
		catch (const X3DError &)
		{ }
	}

	return routes;
}

/***
 *
 *
 *
 * User-defined fields operations
 *
 *
 *
 */

///  This function is with reference handling.
void
X3DEditor::addUserDefinedField (const SFNode & node, X3DFieldDefinition* const field, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (FieldPtr (field));

	undoStep -> addUndoFunction (&X3DBaseNode::removeUserDefinedField, node, field -> getName ());
	undoStep -> addRedoFunction (&X3DBaseNode::addUserDefinedField, node, field -> getAccessType (), field -> getName (), field);

	node -> addUserDefinedField (field -> getAccessType (), field -> getName (), field);
}

///  This function is with reference handling.
void
X3DEditor::replaceUserDefinedField (const SFNode & node, X3DFieldDefinition* const oldField, X3DFieldDefinition* const newField, const UndoStepPtr & undoStep) const
{
	auto userDefinedFields = node -> getUserDefinedFields ();
	auto iter              = std::find (userDefinedFields .begin (), userDefinedFields .end (), oldField);

	if (iter == userDefinedFields .end ())
		return;

	// Handle IS references, if node is proto.

	if (oldField -> getType () == newField -> getType ())
	{
		if (oldField -> getAccessType () not_eq newField -> getAccessType ())
			replaceReferences (ProtoDeclarationPtr (node), oldField, newField, undoStep);
	}
	else
		removeReferences (ProtoDeclarationPtr (node), oldField, undoStep);

	// If possible we want to reassign the routes from the old field to the new fields.  In this step we create addRoutes
	// functions we will execute later.

	std::deque <std::function <void ()>>  addRoutes;

	if (newField -> getType () == oldField -> getType ())
	{
		// Assign the old value to the new field.

		newField -> set (*oldField);

		// Reassign IS reference to the new field.

		for (const auto & reference : oldField -> getReferences ())
		{
			if (newField -> getAccessType () == reference -> getAccessType () or newField -> getAccessType () == inputOutput)
				newField -> addReference (reference);
		}

		// Create addRoutes functions for input and output routes.

		if (newField -> isInput () and oldField -> isInput ())
		{
			for (const auto & route : oldField -> getInputRoutes ())
			{
				const bool selfConnection = route -> getSourceNode () == node and route -> getSourceField () == oldField -> getName ();

				addRoutes .emplace_back (std::bind (&X3DEditor::addRoute,
				                                    this,
				                                    node -> getExecutionContext (),
				                                    route -> getSourceNode (),
				                                    selfConnection ? newField -> getName () : route -> getSourceField (),
				                                    node,
				                                    newField -> getName (),
				                                    undoStep));
			}
		}

		if (newField -> isOutput () and oldField -> isOutput ())
		{
			for (const auto & route : oldField -> getOutputRoutes ())
			{
				const bool selfConnection = route -> getDestinationNode () == node and route -> getDestinationField () == oldField -> getName ();

				addRoutes .emplace_back (std::bind (&X3DEditor::addRoute,
				                                    this,
				                                    node -> getExecutionContext (),
				                                    node,
				                                    newField -> getName (),
				                                    route -> getDestinationNode (),
				                                    selfConnection ? newField -> getName () : route -> getDestinationField (),
				                                    undoStep));
			}
		}
	}

	// Save all involved fields.

	undoStep -> addObjects (FieldArray (userDefinedFields .begin (), userDefinedFields .end ()), FieldPtr (newField));

	// Remove user data from old field.

	undoStep -> addUndoFunction (&X3DFieldDefinition::setUserData, oldField, nullptr);
	undoStep -> addRedoFunction (&X3DFieldDefinition::setUserData, newField, nullptr);
	newField -> setUserData (nullptr);

	// Remove routes from field.  We must do this as routes are associated with a node and we are self responsible for doing this.

	removeRoutes (oldField, undoStep);

	// Restore old user defined fields in undo.

	undoStep -> addUndoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	// Replace old field with new field in temporary array.

	*iter = newField;

	// Set new user defined fields.

	undoStep -> addRedoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	node -> setUserDefinedFields (userDefinedFields);

	// Now process the addRoutes functions recorded above to reassign the routes from the old field to the new field.

	for (const auto & addRoute : addRoutes)
	{
		try
		{
			addRoute ();
		}
		catch (const X3DError &)
		{ }
	}
}

///  This function is with reference handling.
void
X3DEditor::removeUserDefinedField (const SFNode & node, X3DFieldDefinition* const field, const UndoStepPtr & undoStep) const
{
	// Handle IS references, if node is proto.

	removeReferences (ProtoDeclarationPtr (node), field, undoStep);

	// Remove field from node

	const auto userDefinedFields = node -> getUserDefinedFields ();

	undoStep -> addObjects (FieldArray (userDefinedFields .begin (), userDefinedFields .end ()), FieldPtr (field));

	// Remove user data from old field.

	undoStep -> addUndoFunction (&X3DFieldDefinition::setUserData, field, nullptr);

	// Remove routes from field.  We must do this as routes are associated with a node and we are self responsible for doing this.

	removeRoutes (field, undoStep);

	undoStep -> addUndoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);
	undoStep -> addRedoFunction (&X3DBaseNode::removeUserDefinedField, node, field -> getName ());

	node -> removeUserDefinedField (field -> getName ());
}

///  This function is without reference handling.
void
X3DEditor::setUserDefinedFields (const SFNode & node, const FieldDefinitionArray & userDefinedFields, const UndoStepPtr & undoStep) const
{
	// Remove any routes and user data.

	const auto currentUserDefinedFields = node -> getUserDefinedFields ();

	std::set <X3DFieldDefinition*>    lhs (currentUserDefinedFields .begin (), currentUserDefinedFields .end ());
	std::set <X3DFieldDefinition*>    rhs (userDefinedFields .begin (), userDefinedFields .end ());
	std::vector <X3DFieldDefinition*> difference;

	std::set_difference (lhs .begin (), lhs .end (), rhs .begin (), rhs .end (), std::back_inserter (difference));

	for (const auto & field : difference)
	{
		undoStep -> addUndoFunction (&X3DFieldDefinition::setUserData, field, nullptr);
		removeRoutes (field, undoStep);
	}

	// Replace the user defined fields.

	undoStep -> addUndoFunction (&X3DBaseNode::setUserDefinedFields, node, currentUserDefinedFields);
	undoStep -> addRedoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	node -> setUserDefinedFields (userDefinedFields);
}

void
X3DEditor::removeRoutes (X3DFieldDefinition* const field, const UndoStepPtr & undoStep) const
{
	// Remove routes from field.

	for (const auto & route : RouteSet (field -> getInputRoutes ()))
	{
		deleteRoute (route -> getExecutionContext (),
		             route -> getSourceNode (),
		             route -> getSourceField (),
		             route -> getDestinationNode (),
		             route -> getDestinationField (),
		             undoStep);
	}

	for (const auto & route : RouteSet (field -> getOutputRoutes ()))
	{
		deleteRoute (route -> getExecutionContext (),
		             route -> getSourceNode (),
		             route -> getSourceField (),
		             route -> getDestinationNode (),
		             route -> getDestinationField (),
		             undoStep);
	}
}

void
X3DEditor::replaceReferences (const ProtoDeclarationPtr & proto, X3DFieldDefinition* const oldField, X3DFieldDefinition* const newField, const UndoStepPtr & undoStep) const
{
	using namespace std::placeholders;
	
	undoStep -> addObjects (proto);

	if (proto)
		traverse (proto, std::bind (&X3DEditor::replaceReferencesCallback, this, _1, oldField, newField, undoStep));
}

bool
X3DEditor::replaceReferencesCallback (SFNode & node, X3DFieldDefinition* const oldProtoField, X3DFieldDefinition* const newProtoField, const UndoStepPtr & undoStep) const
{
	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (field -> getReferences () .count (oldProtoField))
		{
			undoStep -> addObjects (node);
			removeReference (field, oldProtoField, undoStep);

			// Test if newProtoField is a reference for field.
			if (newProtoField -> isReference (field -> getAccessType ()))
				addReference (field, newProtoField, undoStep);
		}
	}

	return true;
}

void
X3DEditor::removeReferences (const ProtoDeclarationPtr & proto, X3DFieldDefinition* const field, const UndoStepPtr & undoStep) const
{
	using namespace std::placeholders;
	
	undoStep -> addObjects (proto);

	if (proto)
		traverse (proto, std::bind (&X3DEditor::removeReferencesCallback, this, _1, field, undoStep));
}

bool
X3DEditor::removeReferencesCallback (SFNode & node, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep) const
{
	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (field -> getReferences () .count (protoField))
		{
			undoStep -> addObjects (node);
			removeReference (field, protoField, undoStep);
		}
	}

	return true;
}

void
X3DEditor::addReference (X3DFieldDefinition* const field, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (FieldPtr (field), FieldPtr (protoField));

	undoStep -> addUndoFunction (&X3DFieldDefinition::removeReference, field, protoField);
	undoStep -> addRedoFunction (&X3DFieldDefinition::addReference,    field, protoField);
	field -> addReference (protoField);
}

void
X3DEditor::removeReference (X3DFieldDefinition* const field, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (FieldPtr (field), FieldPtr (protoField));

	undoStep -> addUndoFunction (&X3DFieldDefinition::addReference,    field, protoField);
	undoStep -> addRedoFunction (&X3DFieldDefinition::removeReference, field, protoField);
	field -> removeReference (protoField);
}

/***
 *
 *
 *
 * Grouping operations
 *
 *
 *
 */

void
X3DEditor::createClone (const X3DExecutionContextPtr & executionContext, const SFNode & clone, const MFNode & nodes, const UndoStepPtr & undoStep) const
{
	for (const auto & node : nodes)
	{
		if (not node)
			continue;

		traverse (executionContext -> getRootNodes (), [&] (SFNode & parent)
		          {
		             for (auto & field: parent -> getFieldDefinitions ())
		             {
		                switch (field -> getType ())
		                {
								 case X3DConstants::SFNode:
									 {
									    const auto sfnode = static_cast <SFNode*> (field);

									    if (*sfnode == node)
											 replaceNode (executionContext, parent, *sfnode, clone, undoStep);

									    break;
									 }
								 case X3DConstants::MFNode:
									 {
									    const auto mfnode = static_cast <MFNode*> (field);

									    replaceNodes (executionContext, parent, *mfnode, node, clone, undoStep);
									    break;
									 }
								 default:
									 break;
							 }
						 }

		             return true;
					 },
		          true, TRAVERSE_PROTOTYPE_INSTANCES);

		replaceNodes (executionContext, SFNode (executionContext), executionContext -> getRootNodes (), node, clone, undoStep);
	}
}

MFNode
X3DEditor::unlinkClone (const X3DExecutionContextPtr & executionContext, const MFNode & clones, const UndoStepPtr & undoStep) const
{
	MFNode nodes;

	for (const auto & clone : clones)
	{
		nodes .emplace_back (clone);

		bool first = true;

		traverse (executionContext -> getRootNodes (), [&] (SFNode & parent)
		          {
		             for (auto & field: parent -> getFieldDefinitions ())
		             {
		                switch (field -> getType ())
		                {
								 case X3DConstants::SFNode:
									 {
									    try
									    {
									       const auto sfnode = static_cast <SFNode*> (field);

									       if (*sfnode == clone)
									       {
									          if (not first)
									          {
									             const SFNode copy = (*sfnode) -> copy (FLAT_COPY);

									             replaceNode (executionContext, parent, *sfnode, copy, undoStep);

									             nodes .emplace_back (copy);
												 }

									          first = false;
											 }
										 }
									    catch (const Error <NOT_SUPPORTED> &)
									    { }

									    break;
									 }
								 case X3DConstants::MFNode:
									 {
									    const auto mfnode = static_cast <MFNode*> (field);

									    unlinkClone (executionContext, parent, *mfnode, clone, nodes, first, undoStep);

									    break;
									 }
								 default:
									 break;
							 }
						 }

		             return true;
					 },
		          true, TRAVERSE_PROTOTYPE_INSTANCES);

		// Unlink in rootNodes array.

		unlinkClone (executionContext, SFNode (executionContext), executionContext -> getRootNodes (), clone, nodes, first, undoStep);
	}

	// Setup new nodes.
	executionContext -> realize ();

	return nodes;
}

void
X3DEditor::unlinkClone (const X3DExecutionContextPtr & executionContext,
                        const SFNode & parent,
                        MFNode & mfnode,
                        const SFNode & clone,
                        MFNode & nodes,
                        bool & first,
                        const UndoStepPtr & undoStep) const
{
	const auto indices = mfnode .indices_of (clone);

	if (indices .empty ())
		return;

	for (const auto & index : indices)
	{
		try
		{
			if (not first)
			{
				const SFNode copy = mfnode [index] -> copy (FLAT_COPY);

				replaceNode (executionContext, parent, mfnode, index, copy, undoStep);

				nodes .emplace_back (copy);
			}

			first = false;
		}
		catch (const Error <NOT_SUPPORTED> &)
		{ }
	}
}

// Grouping operations

SFNode
X3DEditor::groupNodes (const X3DExecutionContextPtr & executionContext,
                       const std::string & typeName,
                       const MFNode & nodes,
                       const UndoStepPtr & undoStep) const
{
	const auto layers = findParents <X3DLayerNode> (nodes .back ());

	const X3DPtr <X3DGroupingNode> group (executionContext -> createNode (typeName));
	const SFNode                   groupNode (group);

	undoStep -> addObjects (group);

	for (const auto & child : nodes)
	{
		if (not child)
			continue;

		// Adjust transformation
		Matrix4d                        childModelViewMatrix = getModelViewMatrix (executionContext, child);
		const X3DPtr <X3DTransformNode> transform (child);

		if (transform)
		{
			childModelViewMatrix .mult_left (transform -> getMatrix ());

			setMatrix (transform, childModelViewMatrix, undoStep);
		}

		// Remove child from scene graph

		removeNodesFromSceneGraph (executionContext, { child }, undoStep);

		// Add to group

		pushBackIntoArray (groupNode, group -> children (), child, undoStep);
	}

	group -> setup ();

	// Add to layer

	SFNode node (group);

	addToLayers (executionContext, layers, node, undoStep);

	return node;
}

MFNode
X3DEditor::ungroupNodes (const X3DExecutionContextPtr & executionContext,
                         const MFNode & groups,
                         const UndoStepPtr & undoStep) const
{
	MFNode children;

	for (const auto & node : groups)
	{
		try
		{
			const X3DPtr <X3DGroupingNode> group (node);

			if (not group)
				continue;

			const auto layers = findParents <X3DLayerNode> (node);

			undoStep -> addObjects (node);

			// Ungroup children

			const auto groupingField = static_cast <MFNode*> (node -> getField ("children"));

			for (const auto & child : *groupingField)
			{
				if (not child)
					continue;

				// Adjust transformation

				Matrix4d                        childModelViewMatrix = getModelViewMatrix (executionContext, child);
				const X3DPtr <X3DTransformNode> transform (child);

				if (transform)
				{
					childModelViewMatrix .mult_left (transform -> getMatrix ());

					setMatrix (transform, childModelViewMatrix, undoStep);
				}

				// Add to layer

				addToLayers (executionContext, layers, child, undoStep);

				undoStep -> addUndoFunction (&MFNode::push_front, groupingField, child);

				children .emplace_back (child);
			}

			undoStep -> addRedoFunction (&MFNode::clear, groupingField);

			groupingField -> clear ();

			// Remove group from scene

			removeNodesFromScene (executionContext, { node }, true, undoStep);
		}
		catch (const Error <INVALID_NODE> &)
		{
			// No grouping field found
		}
	}

	return children;
}

bool
X3DEditor::addToGroup (const X3DExecutionContextPtr & executionContext,
                       const SFNode & group,
                       const MFNode & children,
                       const UndoStepPtr & undoStep) const
{
	if (not group)
		return false;

	bool added = false;

	undoStep -> addObjects (group);

	for (const auto & child : children)
	{
		try
		{
			if (child == group)
				continue;

			if (not child)
				continue;

			const auto containerField = getContainerField (group, child);

			try
			{
				// Handle X3DTransformNode nodes.

				const X3DPtr <X3DTransformNode> childTransform (child);

				if (childTransform)
				{
					// Get group modelview matrix

					Matrix4d groupModelViewMatrix (getModelViewMatrix (executionContext, group));

					const X3DPtr <X3DTransformMatrix3DObject> groupTransform (group);

					if (groupTransform)
						groupModelViewMatrix .mult_left (groupTransform -> getMatrix ());

					// Adjust child transformation

					Matrix4d childModelViewMatrix = getModelViewMatrix (executionContext, child);

					childModelViewMatrix .mult_left (childTransform -> getMatrix ());
					childModelViewMatrix .mult_right (inverse (groupModelViewMatrix));

					setMatrix (childTransform, childModelViewMatrix, undoStep);
				}
			}
			catch (const std::domain_error & error)
			{ }

			// Remove child from scene graph

			removeNodesFromSceneGraph (executionContext, { child }, undoStep);

			// Add child to group

			const auto sfnode = dynamic_cast <SFNode*> (containerField);

			if (sfnode)
			{
				replaceNode (executionContext, group, *sfnode, child, undoStep);

				added = true; // XXX Remove previous child completely from scene if not in scene anymore
			}
			else
			{
				const auto mfnode = dynamic_cast <MFNode*> (containerField);

				if (mfnode)
				{
					pushBackIntoArray (child, *mfnode, child, undoStep);

					added = true;
				}
			}
		}
		catch (const Error <INVALID_NODE> &)
		{
			// No container field found
		}
	}

	return added;
}

void
X3DEditor::detachFromGroup (const X3DExecutionContextPtr & executionContext,
                            const MFNode & children_,
                            const bool detachToLayer0,
                            const UndoStepPtr & undoStep) const
{
	MFNode children = children_;

	for (const auto & child : children)
	{
		// Get layers before removing from scene graph.

		const auto layers = findParents <X3DLayerNode> (child);

		// Adjust transformation

		const X3DPtr <X3DTransformNode> transform (child);

		if (transform)
		{
			Matrix4d childModelViewMatrix = getModelViewMatrix (executionContext, child);

			childModelViewMatrix .mult_left (transform -> getMatrix ());

			setMatrix (transform, childModelViewMatrix, undoStep);
		}

		// Remove child from scene graph

		removeNodesFromSceneGraph (executionContext, { child }, undoStep);

		// Add to layers

		if (detachToLayer0)
			pushBackIntoArray (SFNode (executionContext), executionContext -> getRootNodes (), child, undoStep);

		else
		   addToLayers (executionContext, layers, child, undoStep);
	}
}

SFNode
X3DEditor::createParentGroup (const X3DExecutionContextPtr & executionContext,
                              const std::string & typeName,
                              const std::string & fieldName,
                              const MFNode & children,
                              const UndoStepPtr & undoStep) const
{
	// Add node to group

	const SFNode group (executionContext -> createNode (typeName));

	if (not children .empty ())
	{
		executionContext -> addUninitializedNode (group);
		undoStep -> addObjects (group);
	
		const auto & leader = children .back ();
	
		traverse (executionContext -> getRootNodes (), [&] (SFNode & parent)
		          {
		             for (auto & field: parent -> getFieldDefinitions ())
		             {
		                switch (field -> getType ())
		                {
								 case X3DConstants::SFNode:
									 {
									    const auto sfnode = static_cast <SFNode*> (field);
	
									    if (*sfnode == leader)
									    {
									       // Replace node with Transform
	
									       undoStep -> addObjects (parent);
	
									       undoStep -> addUndoFunction (&SFNode::setValue, sfnode, leader);
									       undoStep -> addRedoFunction (&SFNode::setValue, sfnode, group);
	
									       sfnode -> setValue (group);
										 }
	
									    break;
									 }
								 case X3DConstants::MFNode:
									 {
									    const auto mfnode = static_cast <MFNode*> (field);
	
									    createParentGroup (executionContext, group, *mfnode, leader, parent, undoStep);
	
									    break;
									 }
								 default:
									 break;
							 }
						 }
	
		             return true;
					 });
	
		createParentGroup (executionContext, group, executionContext -> getRootNodes (), leader, SFNode (executionContext), undoStep);
	
		MFNode tail (children .begin (), children .end () - 1);
	
		addToGroup (executionContext, group, tail, undoStep);
		pushBackIntoArray (group, group -> getField <MFNode> (fieldName), leader, undoStep);
	}

	executionContext -> realize ();
	return group;
}

void
X3DEditor::createParentGroup (const X3DExecutionContextPtr & executionContext,
                              const SFNode & group,
                              MFNode & children,
                              const SFNode & child,
                              const SFNode & parent,
                              const UndoStepPtr & undoStep) const
{
	const auto indices = children .indices_of (child);

	if (indices .empty ())
		return;

	undoStep -> addObjects (parent);

	for (const auto & index : indices)
	{
		// Replace node with Transform

		using set1Value = void (MFNode::*) (const MFNode::size_type, const SFNode &);

		undoStep -> addUndoFunction ((set1Value) & MFNode::set1Value, std::ref (children), index, child);
		undoStep -> addRedoFunction ((set1Value) & MFNode::set1Value, std::ref (children), index, SFNode (group));

		children [index] = group;
	}
}

///  Add node to active layer root nodes or layer 0.
void
X3DEditor::addNodesToActiveLayer (const WorldPtr & world, const MFNode & nodes, const UndoStepPtr & undoStep) const
{
	const auto & activeLayer = world -> getActiveLayer ();
	auto &       children    = activeLayer and activeLayer not_eq world -> getLayer0 ()
	                           ? activeLayer -> children ()
										: world -> getExecutionContext () -> getRootNodes ();

	undoStep -> addObjects (world -> getExecutionContext (), activeLayer);
	children .append (nodes);

	const auto removeUndoStep = std::make_shared <X3D::UndoStep> ("");

	removeNodesFromScene (world -> getExecutionContext (), nodes, true, removeUndoStep);
	undoStep -> addUndoFunction (&UndoStep::redo, removeUndoStep);
	removeUndoStep -> undo ();
	undoStep -> addRedoFunction (&UndoStep::undo, removeUndoStep);
}

/// Add node to layers.
void
X3DEditor::addToLayers (const X3DExecutionContextPtr & executionContext, const std::vector <X3DLayerNode*> & layers, const SFNode & node, const UndoStepPtr & undoStep) const
{
	bool added = false;

	for (const auto & layer : layers)
	{
		if (layer -> isLayer0 ())
		   continue;

		added = true;

		undoStep -> addObjects (SFNode (layer));

		pushBackIntoArray (SFNode (layer), layer -> children (), node, undoStep);
	}

	if (not added)
		pushBackIntoArray (SFNode (executionContext), executionContext -> getRootNodes (), node, undoStep);
}

/***
 *
 *
 *
 * Node handling
 *
 *
 *
 */

MFNode
X3DEditor::getParentNodes (const SFNode & child) const
{
	MFNode parentNodes;

	for (const auto & parent : child -> getParents ())
	{
		const auto sfnode = dynamic_cast <SFNode*> (parent);

		if (sfnode)
		{
			for (const auto & secondParent : sfnode -> getParents ())
			{
				const auto mfnode = dynamic_cast <MFNode*> (secondParent);

				if (mfnode)
				{
					for (const auto & thirdParent : mfnode -> getParents ())
					{
						const auto baseNode = dynamic_cast <X3DBaseNode*> (thirdParent);

						if (baseNode)
						{
							if (baseNode not_eq child)
							{
								if (not baseNode -> isPrivate ())
									parentNodes .emplace_back (baseNode);
							}
						}
					}

					continue;
				}

				const auto baseNode = dynamic_cast <X3DBaseNode*> (secondParent);

				if (baseNode)
				{
					if (baseNode not_eq child)
					{
						if (not baseNode -> isPrivate ())
							parentNodes .emplace_back (baseNode);
					}

					continue;
				}
			}
		}
	}

	return parentNodes;
}

X3DFieldDefinition*
X3DEditor::getContainerField (const SFNode & parent, const SFNode & child) const
throw (Error <INVALID_NODE>)
{
	try
	{
		const auto field = parent -> getField (child -> getInnerNode () -> getContainerField ());

		if (field -> getAccessType () & initializeOnly)
		{
			if (field -> getType () == X3DConstants::SFNode or field -> getType () == X3DConstants::MFNode)
				return field;
		}
	}
	catch (const X3DError &)
	{ }

	// Find appropriate field

	try
	{
		const auto field = parent -> getField ("children");

		if (field -> getAccessType () == initializeOnly or field -> getAccessType () == inputOutput)
		{
			if (field -> getType () == X3DConstants::MFNode)
				return field;
		}
	}
	catch (const X3DError &)
	{ }

	// Find last MFNode or SFNode and not metadata

	for (auto & field : parent -> getFieldDefinitions ())
	{
		if (field -> getAccessType () == initializeOnly or field -> getAccessType () == inputOutput)
		{
			if (field -> getType () == X3DConstants::MFNode or (field -> getType () == X3DConstants::SFNode and field -> getName () not_eq "metadata"))
				return field;
		}
	}

	throw Error <INVALID_NODE> ("No appropriate container field found.");
}

/***
 *
 *
 *
 * Modelview matrix handling
 *
 *
 *
 */

void
X3DEditor::transformToZero (const MFNode & children, const UndoStepPtr & undoStep) const
{
	Matrix4dStack           modelViewMatrix;
	std::set <X3DBaseNode*> coords;

	transformToZero (children, modelViewMatrix, coords, undoStep);
}

void
X3DEditor::transformToZero (const MFNode & children,
                            Matrix4dStack & modelViewMatrix,
                            std::set <X3DBaseNode*> & coords,
                            const UndoStepPtr & undoStep) const
{
	for (const auto & child : children)
	{
		if (child)
			transformToZero (child, modelViewMatrix, coords, undoStep);
	}
}

void
X3DEditor::transformToZero (const SFNode & child,
                            Matrix4dStack & modelViewMatrix,
                            std::set <X3DBaseNode*> & coords,
                            const UndoStepPtr & undoStep) const
{
	for (const auto & type : basic::make_reverse_range (child -> getType ()))
	{
		switch (type)
		{
			case X3DConstants::ScreenGroup:
			case X3DConstants::Viewport:
			{
				// Skip these nodes and their children.
				return;
			}
			case X3DConstants::X3DTransformNode:
			{
				X3DPtr <X3DTransformNode> transform (child);

				const auto matrix = transform -> getCurrentMatrix ();

				undoStep -> addObjects (transform);
				undoStep -> addUndoFunction (&X3DTransformNode::setMatrixWithCenter, transform, matrix,      transform -> center () .getValue ());
				undoStep -> addRedoFunction (&X3DTransformNode::setMatrixWithCenter, transform, Matrix4d (), Vector3d ());

				transform -> setMatrixWithCenter (Matrix4d (), Vector3d ());

				modelViewMatrix .push ();
				modelViewMatrix .mult_left (matrix);

				transformToZero (transform -> children (), modelViewMatrix, coords, undoStep);

				modelViewMatrix .pop ();
				return;
			}
			case X3DConstants::X3DGroupingNode:
			{
				transformToZero (child -> getField <MFNode> ("children"), modelViewMatrix, coords, undoStep);
				return;
			}
			case X3DConstants::X3DShapeNode:
			{
				X3DPtr <X3DShapeNode>    shape (child);
				X3DPtr <X3DGeometryNode> geometry (shape -> geometry ());

				if (geometry)
					transformToZero (geometry, modelViewMatrix .get (), coords, undoStep);

				return;
			}
			default:
				continue;
		}
	}
}

void
X3DEditor::transformToZero (const X3DPtr <X3DGeometryNode> & geometry,
                            const Matrix4d & matrix,
                            std::set <X3DBaseNode*> & coords,
                            const UndoStepPtr & undoStep) const
{
	for (const auto & type : basic::make_reverse_range (geometry -> getType ()))
	{
		switch (type)
		{
			case X3DConstants::IndexedFaceSet:
			{
				X3DPtr <IndexedFaceSet>    indexedFaceSet (geometry);
				X3DPtr <X3DCoordinateNode> coord (indexedFaceSet -> coord ());

				if (coord)
					transformToZero (coord, matrix, coords, undoStep);

				return;
			}
			default:
				continue;
		}
	}
}

void
X3DEditor::transformToZero (const X3DPtr <X3DCoordinateNode> & coord,
                            const Matrix4d & matrix,
                            std::set <X3DBaseNode*> & coords,
                            const UndoStepPtr & undoStep) const
{
	if (not coords .emplace (coord) .second)
		return;

	switch (coord -> getType () .back ())
	{
		case X3DConstants::Coordinate:
		{
			X3DPtr <Coordinate> coordinate (coord);

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());

			for (auto & point : coordinate -> point ())
				point = matrix .mult_vec_matrix (point .getValue ());

			undoStep -> addRedoFunction (&MFVec3f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			return;
		}
		case X3DConstants::CoordinateDouble:
		{
			X3DPtr <CoordinateDouble> coordinate (coord);

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());

			for (auto & point : coordinate -> point ())
				point = matrix .mult_vec_matrix (point);

			undoStep -> addRedoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			return;
		}
		case X3DConstants::GeoCoordinate:
		{
			X3DPtr <GeoCoordinate> coordinate (coord);

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());

			for (size_t i = 0, size = coordinate -> getSize (); i < size; ++ i)
				coordinate -> set1Point (i, matrix .mult_vec_matrix (coordinate -> get1Point (i)));

			undoStep -> addRedoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			return;
		}
		default:
			return;
	}
}

void
X3DEditor::storeMatrix (const SFNode & node, const UndoStepPtr & undoStep) const
{
	X3DPtr <X3DTransformNode> transform (node);

	if (transform)
	{
		undoStep -> addUndoFunction (&X3DTransformNode::setMatrix,
		                             transform,
		                             transform -> getMatrix ());
	}
}

void
X3DEditor::setMatrix (const X3DPtr <X3DTransformNode> & transform, const Matrix4d & matrix, const UndoStepPtr & undoStep) const
{
	undoStep -> addUndoFunction (&X3DTransformNode::setMatrix,
	                             transform,
	                             transform -> getMatrix ());

	undoStep -> addRedoFunction (&X3DTransformNode::setMatrix,
	                             transform,
	                             matrix);

	transform -> setMatrix (matrix);
}

Matrix4d
X3DEditor::getModelViewMatrix (const X3DExecutionContextPtr & executionContext, const SFNode & node) const
{
	Matrix4d modelViewMatrix;

	auto hierarchy = find (executionContext, node, TRAVERSE_ROOT_NODES | TRAVERSE_PROTOTYPE_INSTANCES);

	if (hierarchy .empty ())
		return modelViewMatrix;

	hierarchy .pop_back ();

	for (const auto & object : basic::make_reverse_range (hierarchy))
	{
		const auto node = dynamic_cast <X3DBaseNode*> (object);

		if (not node)
			continue;

		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::X3DLayerNode:
				case X3DConstants::X3DProtoDeclarationNode:
				case X3DConstants::X3DScriptNode:
				case X3DConstants::X3DProgrammableShaderObject:
				case X3DConstants::X3DBaseNode:
					goto END;
				case X3DConstants::X3DTransformMatrix3DObject:
				{
					const auto transform = dynamic_cast <X3DTransformMatrix3DObject*> (object);

					modelViewMatrix .mult_right (transform -> getMatrix ());
					break;
				}
				case X3DConstants::X3DNode:
					break;
				default:
					continue;
			}

			break;
		}
	}

END:
	return modelViewMatrix;
}

/***
 *
 *
 *
 * Polygon reducing
 *
 *
 *
 */

void
X3DEditor::mergePoints (const X3DPtr <IndexedFaceSet> & geometryNode, const double distance, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (geometryNode);
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> colorIndex    ()), geometryNode -> colorIndex    ());
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> texCoordIndex ()), geometryNode -> texCoordIndex ());
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> normalIndex   ()), geometryNode -> normalIndex   ());
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> coordIndex    ()), geometryNode -> coordIndex    ());

	undoSetCoordPoint (geometryNode -> getCoord (), undoStep);

	geometryNode -> mergePoints (distance);

	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> colorIndex    ()), geometryNode -> colorIndex    ());
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> texCoordIndex ()), geometryNode -> texCoordIndex ());
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> normalIndex   ()), geometryNode -> normalIndex   ());
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> coordIndex    ()), geometryNode -> coordIndex    ());

	redoSetCoordPoint (geometryNode -> getCoord (), undoStep);
}

/***
 *
 *
 *
 * X3DComposedGeometryNode
 *
 *
 *
 */

void
X3DEditor::undoSetColorColor (const X3DPtr <X3DColorNode> & colorNode, const UndoStepPtr & undoStep) const
{
	if (not colorNode)
	   return;

	undoStep -> addObjects (colorNode);

	switch (colorNode -> getType () .back ())
	{
		case X3DConstants::Color:
		{
			const X3DPtr <Color> node (colorNode);

			undoStep -> addUndoFunction (&MFColor::setValue, std::ref (node -> color ()), node -> color ());
			break;
		}
		case X3DConstants::ColorRGBA:
		{
			const X3DPtr <ColorRGBA> node (colorNode);

			undoStep -> addUndoFunction (&MFColorRGBA::setValue, std::ref (node -> color ()), node -> color ());
			break;
		}
		default:
			break;
	}
}

void
X3DEditor::undoSetTexCoordPoint (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep) const
{
	if (not texCoordNode)
	   return;

	switch (texCoordNode -> getType () .back ())
	{
		case X3DConstants::MultiTextureCoordinate:
		{
			const X3DPtr <MultiTextureCoordinate> node (texCoordNode);

			for (const auto & texCoordNode : node -> getTexCoord ())
				undoSetTexCoordPointImpl (texCoordNode, undoStep);

			break;
		}
		default:
		   undoSetTexCoordPointImpl (texCoordNode, undoStep);
			break;
	}
}

void
X3DEditor::undoSetTexCoordPointImpl (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (texCoordNode);

	switch (texCoordNode -> getType () .back ())
	{
		case X3DConstants::TextureCoordinate:
		{
			const X3DPtr <TextureCoordinate> coordinate (texCoordNode);

			undoStep -> addUndoFunction (&MFVec2f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		case X3DConstants::TextureCoordinate3D:
		{
			const X3DPtr <TextureCoordinate3D> coordinate (texCoordNode);

			undoStep -> addUndoFunction (&MFVec3f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		case X3DConstants::TextureCoordinate4D:
		{
			const X3DPtr <TextureCoordinate4D> coordinate (texCoordNode);

			undoStep -> addUndoFunction (&MFVec4f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			break;
		}
		default:
			break;
	}
}

void
X3DEditor::undoSetNormalVector (const X3DPtr <X3DNormalNode> & normalNode, const UndoStepPtr & undoStep) const
{
	if (not normalNode)
	   return;

	undoStep -> addObjects (normalNode);

	switch (normalNode -> getType () .back ())
	{
		case X3DConstants::Normal:
		{
			const X3DPtr <Normal> node (normalNode);

			undoStep -> addUndoFunction (&MFVec3f::setValue, std::ref (node -> vector ()), node -> vector ());
			break;
		}
		default:
			break;
	}
}

void
X3DEditor::undoSetCoordPoint (const X3DPtr <X3DCoordinateNode> & coordNode, const UndoStepPtr & undoStep) const
{
	if (not coordNode)
	   return;

	undoStep -> addObjects (coordNode);

	switch (coordNode -> getType () .back ())
	{
		case X3DConstants::Coordinate:
		{
			const X3DPtr <Coordinate> node (coordNode);

			undoStep -> addUndoFunction (&MFVec3f::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		case X3DConstants::CoordinateDouble:
		{
			const X3DPtr <CoordinateDouble> node (coordNode);

			undoStep -> addUndoFunction (&MFVec3d::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		case X3DConstants::GeoCoordinate:
		{
			const X3DPtr <GeoCoordinate> node (coordNode);

			undoStep -> addUndoFunction (&MFVec3d::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		default:
			break;
	}
}

void
X3DEditor::redoSetColorColor (const X3DPtr <X3DColorNode> & colorNode, const UndoStepPtr & undoStep) const
{
	if (not colorNode)
	   return;

	undoStep -> addObjects (colorNode);

	switch (colorNode -> getType () .back ())
	{
		case X3DConstants::Color:
		{
			const X3DPtr <Color> node (colorNode);

			undoStep -> addRedoFunction (&MFColor::setValue, std::ref (node -> color ()), node -> color ());
			break;
		}
		case X3DConstants::ColorRGBA:
		{
			const X3DPtr <ColorRGBA> node (colorNode);

			undoStep -> addRedoFunction (&MFColorRGBA::setValue, std::ref (node -> color ()), node -> color ());
			break;
		}
		default:
			break;
	}
}

void
X3DEditor::redoSetTexCoordPoint (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep) const
{
	if (not texCoordNode)
	   return;

	switch (texCoordNode -> getType () .back ())
	{
		case X3DConstants::MultiTextureCoordinate:
		{
			const X3DPtr <MultiTextureCoordinate> node (texCoordNode);

			for (const auto & texCoordNode : node -> getTexCoord ())
				redoSetTexCoordPointImpl (texCoordNode, undoStep);

			break;
		}
		default:
		   redoSetTexCoordPointImpl (texCoordNode, undoStep);
			break;
	}
}

void
X3DEditor::redoSetTexCoordPointImpl (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (texCoordNode);

	switch (texCoordNode -> getType () .back ())
	{
		case X3DConstants::TextureCoordinate:
		{
			const X3DPtr <TextureCoordinate> node (texCoordNode);

			undoStep -> addRedoFunction (&MFVec2f::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		case X3DConstants::TextureCoordinate3D:
		{
			const X3DPtr <TextureCoordinate3D> node (texCoordNode);

			undoStep -> addRedoFunction (&MFVec3f::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		case X3DConstants::TextureCoordinate4D:
		{
			const X3DPtr <TextureCoordinate4D> node (texCoordNode);

			undoStep -> addRedoFunction (&MFVec4f::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		default:
			break;
	}
}

void
X3DEditor::redoSetNormalVector (const X3DPtr <X3DNormalNode> & normalNode, const UndoStepPtr & undoStep) const
{
	if (not normalNode)
	   return;

	undoStep -> addObjects (normalNode);

	switch (normalNode -> getType () .back ())
	{
		case X3DConstants::Normal:
		{
			const X3DPtr <Normal> node (normalNode);

			undoStep -> addRedoFunction (&MFVec3f::setValue, std::ref (node -> vector ()), node -> vector ());
			break;
		}
		default:
			break;
	}
}

void
X3DEditor::redoSetCoordPoint (const X3DPtr <X3DCoordinateNode> & coordNode, const UndoStepPtr & undoStep) const
{
	if (not coordNode)
	   return;

	undoStep -> addObjects (coordNode);

	switch (coordNode -> getType () .back ())
	{
		case X3DConstants::Coordinate:
		{
			const X3DPtr <Coordinate> node (coordNode);

			undoStep -> addRedoFunction (&MFVec3f::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		case X3DConstants::CoordinateDouble:
		{
			const X3DPtr <CoordinateDouble> node (coordNode);

			undoStep -> addRedoFunction (&MFVec3d::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		case X3DConstants::GeoCoordinate:
		{
			const X3DPtr <GeoCoordinate> node (coordNode);

			undoStep -> addRedoFunction (&MFVec3d::setValue, std::ref (node -> point ()), node -> point ());
			break;
		}
		default:
			break;
	}
}

/***
 *
 *
 *
 * Array handling
 *
 *
 *
 */

void
X3DEditor::pushBackIntoArray (const SFNode & parent, MFNode & array, const SFNode & node, const UndoStepPtr & undoStep) const
{
	// Add to group

	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .emplace_back (node);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);
}

void
X3DEditor::insertIntoArray (const SFNode & parent, MFNode & array, const size_t index, const SFNode & node, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .emplace (array .begin () + index, node);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);
}

void
X3DEditor::insertIntoArray (const SFNode & parent, MFNode & array, const size_t index, const MFNode::iterator & first, const MFNode::iterator & last, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .insert (array .begin () + index, first, last);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);
}

void
X3DEditor::moveValueWithinArray (const SFNode & parent, MFNode & array, const size_t fromIndex, const size_t toIndex, const UndoStepPtr & undoStep) const
{
	// Undo

	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	// Insert

	const auto fromIter = array .begin () + fromIndex;
	const auto toIter   = array .begin () + toIndex;

	array .emplace (toIter, std::move (*fromIter)); // XXX: array .emplace (toIter, std::move (*fromIter));

	// Erase

	if (fromIndex < toIndex)
		array .erase (array .begin () + fromIndex);
	else
		array .erase (array .begin () + (fromIndex + 1));

	// Redo

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);
}

void
X3DEditor::eraseFromArray (const SFNode & parent, MFNode & array, const size_t index, const UndoStepPtr & undoStep) const
{
	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .erase (array .begin () + index);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);
}

} // X3D
} // titania

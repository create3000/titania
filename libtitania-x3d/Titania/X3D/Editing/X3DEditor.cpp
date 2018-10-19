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

#include "../../X3D.h"
#include "../Bits/Traverse.h"
#include "../Browser/X3DBrowser.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/WorldInfo.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/EnvironmentalEffects/Background.h"
#include "../Components/EnvironmentalSensor/X3DEnvironmentalSensorNode.h"
#include "../Components/Geometry3D/Extrusion.h"
#include "../Components/Geometry3D/IndexedFaceSet.h"
#include "../Components/Geospatial/GeoCoordinate.h"
#include "../Components/Grouping/X3DTransformNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Lighting/DirectionalLight.h"
#include "../Components/Lighting/PointLight.h"
#include "../Components/Lighting/SpotLight.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Networking/Inline.h"
#include "../Components/NURBS/CoordinateDouble.h"
#include "../Components/Rendering/Color.h"
#include "../Components/Rendering/ColorRGBA.h"
#include "../Components/Rendering/Coordinate.h"
#include "../Components/Rendering/Normal.h"
#include "../Components/Shape/X3DShapeNode.h"
#include "../Components/Sound/Sound.h"
#include "../Components/Text/X3DFontStyleNode.h"
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
#include "../Tools/Geometry3D/IndexedFaceSet/IndexedFaceSetTool.h"

#include <Titania/Utility/Map.h>

#include <regex>

#include <Titania/X3D/Debug.h>

namespace titania {
namespace X3D {

/***
 *
 *
 *
 * Scene handling
 *
 *
 *
 */

///  Changes the world URL of @a scene to @a worldURL and transforms all relative url's in the scene graph.
void
X3DEditor::setWorldURL (const X3DScenePtr & scene, const basic::uri & worldURL, const UndoStepPtr & undoStep)
{
	if (worldURL == scene -> getWorldURL ())
		return;

	using namespace std::placeholders;

	traverse (scene,
	          std::bind (&X3DEditor::transform, scene -> getWorldURL (), worldURL, undoStep, _1),
	          TRAVERSE_EXTERNPROTO_DECLARATIONS |
	          TRAVERSE_PROTO_DECLARATIONS |
	          TRAVERSE_PROTO_DECLARATION_BODY |
	          TRAVERSE_ROOT_NODES);

	undoStep -> addUndoFunction (&X3DScene::setWorldURL, scene, scene -> getWorldURL ());
	undoStep -> addRedoFunction (&X3DScene::setWorldURL, scene, worldURL);
	scene -> setWorldURL (worldURL);
}

///  Transforms all relative url's in in @a node if node is a Background, X3DFontStyleNode, or X3DUrlObject.
bool
X3DEditor::transform (const basic::uri & oldWorldURL, const basic::uri & newWorldURL, const UndoStepPtr & undoStep, SFNode & node)
{
	using MFString = X3DField <MFString::internal_type>;
	using set      = void (MFString::*) (const MFString &);

	for (const auto & type : basic::make_reverse_range (node -> getType ()))
	{
		switch (type)
		{
			case X3DConstants::Background :
				{
					X3DPtr <Background> background (node);

					undoStep -> addObjects (background);
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> frontUrl ()),  background -> frontUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> backUrl ()),   background -> backUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> leftUrl ()),   background -> leftUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> rightUrl ()),  background -> rightUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> topUrl ()),    background -> topUrl ());
					undoStep -> addUndoFunction ((set) & MFString::set, std::ref (background -> bottomUrl ()), background -> bottomUrl ());

					X3DUrlObject::transform (background -> frontUrl (),  oldWorldURL, newWorldURL);
					X3DUrlObject::transform (background -> backUrl (),   oldWorldURL, newWorldURL);
					X3DUrlObject::transform (background -> leftUrl (),   oldWorldURL, newWorldURL);
					X3DUrlObject::transform (background -> rightUrl (),  oldWorldURL, newWorldURL);
					X3DUrlObject::transform (background -> topUrl (),    oldWorldURL, newWorldURL);
					X3DUrlObject::transform (background -> bottomUrl (), oldWorldURL, newWorldURL);

					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> frontUrl ()),  background -> frontUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> backUrl ()),   background -> backUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> leftUrl ()),   background -> leftUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> rightUrl ()),  background -> rightUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> topUrl ()),    background -> topUrl ());
					undoStep -> addRedoFunction ((set) & MFString::set, std::ref (background -> bottomUrl ()), background -> bottomUrl ());
					break;
				}
			case X3DConstants::X3DFontStyleNode:
			{
				X3DPtr <X3DFontStyleNode> fontStyleNode (node);

				undoStep -> addObjects (fontStyleNode);
				undoStep -> addUndoFunction ((set) & MFString::set, std::ref (fontStyleNode -> family ()), fontStyleNode -> family ());

				X3DFontStyleNode::transform (fontStyleNode -> family (), oldWorldURL, newWorldURL);

				undoStep -> addRedoFunction ((set) & MFString::set, std::ref (fontStyleNode -> family ()), fontStyleNode -> family ());
				break;
			}
			case X3DConstants::X3DUrlObject:
			{
				X3DPtr <X3DUrlObject> urlObject (node);

				undoStep -> addObjects (urlObject);
				undoStep -> addUndoFunction ((set) & MFString::set, std::ref (urlObject -> url ()), urlObject -> url ());

				X3DUrlObject::transform (urlObject -> url (), oldWorldURL, newWorldURL);

				undoStep -> addRedoFunction ((set) & MFString::set, std::ref (urlObject -> url ()), urlObject -> url ());
				break;
			}
			default:
				break;
		}
	}

	return true;
}

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
X3DEditor::importScene (const X3DExecutionContextPtr & executionContext, const X3DScenePtr & scene, const UndoStepPtr & undoStep)
{
	try
	{
		// Restore protos

		undoStep -> addUndoFunction (&X3DEditor::restoreExternProtoDeclarations, executionContext, executionContext -> getExternProtoDeclarations ());
		undoStep -> addUndoFunction (&X3DEditor::restoreProtoDeclarations,       executionContext, executionContext -> getProtoDeclarations ());

		for (const auto & externproto : scene -> getExternProtoDeclarations ())
		{
			try
			{
				const auto existing  = executionContext -> getExternProtoDeclaration (externproto -> getName ());
				const auto instances = existing -> getInstances ();

				for (const auto & node : instances)
				{
					const auto instance = X3DPtr <X3DPrototypeInstance> (node);

					undoStep -> addUndoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, existing);
					undoStep -> addRedoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, externproto);
					instance -> setProtoDeclarationNode (externproto);
				}
			}
			catch (const Error <INVALID_NAME> &)
			{ }

			try
			{
				const auto existing  = executionContext -> getProtoDeclaration (externproto -> getName ());
				const auto instances = existing -> getInstances ();

				for (const auto & node : instances)
				{
					const auto instance = X3DPtr <X3DPrototypeInstance> (node);

					undoStep -> addUndoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, existing);
					undoStep -> addRedoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, externproto);
					instance -> setProtoDeclarationNode (externproto);
				}

				undoStep -> addRedoFunction (&X3DExecutionContext::removeProtoDeclaration, executionContext, externproto -> getName ());
				executionContext -> removeProtoDeclaration (externproto -> getName ());
			}
			catch (const Error <INVALID_NAME> &)
			{ }
		}

		for (const auto & prototype : scene -> getProtoDeclarations ())
		{
			try
			{
				const auto existing  = executionContext -> getProtoDeclaration (prototype -> getName ());
				const auto instances = existing -> getInstances ();
	
				for (const auto & node : instances)
				{
					const auto instance = X3DPtr <X3DPrototypeInstance> (node);

					undoStep -> addUndoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, existing);
					undoStep -> addRedoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, prototype);
					instance -> setProtoDeclarationNode (prototype);
				}
			}
			catch (const Error <INVALID_NAME> &)
			{ }

			try
			{
				const auto existing  = executionContext -> getExternProtoDeclaration (prototype -> getName ());
				const auto instances = existing -> getInstances ();

				for (const auto & node : instances)
				{
					const auto instance = X3DPtr <X3DPrototypeInstance> (node);

					undoStep -> addUndoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, existing);
					undoStep -> addRedoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, prototype);
					instance -> setProtoDeclarationNode (prototype);
				}

				undoStep -> addRedoFunction (&X3DExecutionContext::removeExternProtoDeclaration, executionContext, prototype -> getName ());
				executionContext -> removeExternProtoDeclaration (prototype -> getName ());
			}
			catch (const Error <INVALID_NAME> &)
			{ }
		}

		// Imported scene

		const auto importedNodes = executionContext -> importScene (scene);

		// Remove nodes undo step

		const auto undoRemoveNodes = std::make_shared <UndoStep> ();
		removeNodesFromScene (executionContext, importedNodes, false, undoRemoveNodes);
		undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNodes);
		undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNodes);
		undoRemoveNodes -> undo ();

		// Restore protos

		undoStep -> addRedoFunction (&X3DEditor::restoreExternProtoDeclarations, executionContext, executionContext -> getExternProtoDeclarations ());
		undoStep -> addRedoFunction (&X3DEditor::restoreProtoDeclarations,       executionContext, executionContext -> getProtoDeclarations ());

		// Prototype support

		requestUpdateInstances (executionContext, undoStep);

		return importedNodes;
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		return MFNode ();
	}
}

std::string
X3DEditor::cutNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const UndoStepPtr & undoStep)
{
	// Detach from group

	detachFromGroup (executionContext, nodes, true, undoStep);

	// Set clipboard text

	const auto string = exportNodes (executionContext, nodes, "XML", true);

	// Remove nodes

	removeNodesFromScene (executionContext, nodes, true, undoStep);

	return string;
}

std::string
X3DEditor::copyNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes)
{
	// Detach from group

	const auto undoDetachFromGroup = std::make_shared <UndoStep> ();

	detachFromGroup (executionContext, nodes, true, undoDetachFromGroup);

	// Set clipboard text

	const auto string = exportNodes (executionContext, nodes, "XML", true);

	// Undo detach from group

	undoDetachFromGroup -> undo ();

	return string;
}

MFNode
X3DEditor::deepCopyNodes (const X3DExecutionContextPtr & sourceContext, const X3DExecutionContextPtr & destContext, const MFNode & nodes, const UndoStepPtr &)
{
	basic::ifilestream text (exportNodes (sourceContext, nodes, "XML", true));

	const auto scene         = destContext -> getBrowser () -> createX3DFromStream (destContext -> getWorldURL (), text);
	const auto importedNodes = destContext -> importScene (scene);

	return importedNodes;
}

std::string
X3DEditor::exportNodes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const std::string & encoding, const bool identifier)
{
	const auto browser = createBrowser ();

	browser -> setLoadUrlObjects (false);

	if (nodes .empty ())
		return exportScene (browser -> createScene (), encoding);

	std::ostringstream osstream;

	// Find proto declarations

	const auto protoNodes = getUsedPrototypes (executionContext, nodes);
	const auto routes     = getConnectedRoutes (executionContext, nodes);

	// Generate text

	osstream .imbue (std::locale::classic ());

	osstream
		<< "#" << LATEST_VERSION << " utf8 " << executionContext -> getBrowser () -> getName ()
		<< std::endl
		<< std::endl;

	if (identifier)
	{
		osstream
			<< "META \"titania-identifier\" " << SFString (executionContext -> getWorldURL () .str ())
			<< std::endl
			<< std::endl;
	}

	Generator::CompactStyle (osstream);
	Generator::EnterScope (osstream);
	Generator::Units (osstream, false);

	if (not protoNodes .empty ())
	{
		for (const auto & protoNode : protoNodes)
			osstream << protoNode << std::endl;

		osstream << std::endl;
	}

	for (const auto & node : nodes)
		osstream << node << std::endl;

	if (not routes .empty ())
	{
		osstream << std::endl;

		for (const auto & route : routes)
			osstream << *route << std::endl;
	}

	Generator::LeaveScope (osstream);

	// Convert to encoding.

	basic::ifilestream stream (osstream .str ());

	const auto scene = browser -> createX3DFromStream (executionContext -> getWorldURL (), stream);

	return exportScene (scene, encoding);
}

std::string
X3DEditor::exportScene (const X3DScenePtr & scene, const std::string & encoding)
{
	if (encoding == "VRML")
		return scene -> toString ();

	else if (encoding == "JSON")
		return scene -> toJSONString ();

	return scene -> toXMLString ();
}

std::vector <X3DProtoDeclarationNodePtr>
X3DEditor::getUsedPrototypes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes)
{
	std::map <X3DProtoDeclarationNodePtr, size_t> protoIndex;

	// Find used protots.

	traverse (const_cast <MFNode &> (nodes), [&] (SFNode & node)
	{
		const X3DPrototypeInstancePtr protoInstance (node);
		
		if (protoInstance)
		{
			traverse (node, [&] (SFNode & subNode)
			{
				if (subNode == protoInstance)
					return true;
				
				const X3DPrototypeInstancePtr child (subNode);
				
				if (child)
				{
					try
					{
						if (child -> getProtoDeclarationNode () == executionContext -> findProtoDeclaration (child -> getTypeName (), AvailableType { }))
							protoIndex .emplace (child -> getProtoDeclarationNode (), protoIndex .size ());
					}
					catch (const X3DError &)
					{ }
				}
				
				return true;
			},
			TRAVERSE_PROTOTYPE_INSTANCES);

			protoIndex .emplace (protoInstance -> getProtoDeclarationNode (), protoIndex .size ());
		}
		
		return true;
	},
	TRAVERSE_PROTO_DECLARATIONS |
	TRAVERSE_PROTO_DECLARATION_BODY |
	TRAVERSE_ROOT_NODES);

	// Sort out sub protos, ie protos in proto.

	const auto protoIndexCopy = protoIndex;

	for (const auto & pair : protoIndexCopy)
	{
		if (isSubContext (executionContext, pair .first -> getExecutionContext ()))
			protoIndex .erase (pair .first);
	}

	// Make array of protos in right order.

	std::vector <X3DProtoDeclarationNodePtr> protoNodes;

	for (const auto & protoNode : basic::reverse (protoIndex))
		protoNodes .emplace_back (std::move (protoNode .second));

	return protoNodes;
}

bool
X3DEditor::isSubContext (const X3DExecutionContext* executionContext, X3DExecutionContext* subContext)
{
	if (subContext == executionContext)
		return false;

	while (not subContext -> isType ({ X3DConstants::X3DScene }))
	{
		subContext = subContext -> getExecutionContext ();

		if (subContext == executionContext)
			return true;
	}

	return false;
}

std::vector <Route*>
X3DEditor::getConnectedRoutes (const X3DExecutionContextPtr & executionContext, const MFNode & nodes)
{
	// Create node index

	std::set <SFNode> nodeIndex;

	traverse (const_cast <MFNode &> (nodes), [&nodeIndex] (SFNode & node)
	{
		nodeIndex .emplace (node);
		return true;
	});

	// Find connected routes

	std::vector <Route*> routes;

	traverse (const_cast <MFNode &> (nodes), [&] (SFNode & node)
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
 *  Creates node @a typeName and adds node to current layer.
 */
SFNode
X3DEditor::createNode (const WorldPtr & world,
                       const X3DExecutionContextPtr & executionContext,
                       const std::string & typeName,
                       const UndoStepPtr & undoStep)
{
	const auto node = executionContext -> createNode (typeName);

	addNodesToActiveLayer (world, { node }, undoStep);
	requestUpdateInstances (executionContext, undoStep);

	return node;
}

/***
 *  Creates node @a typeName and adds node to current layer.
 */
SFNode
X3DEditor::createProto (const WorldPtr & world,
                        const X3DExecutionContextPtr & executionContext,
                        const std::string & typeName,
                        const UndoStepPtr & undoStep)
{
	const auto node = executionContext -> createProto (typeName);

	addNodesToActiveLayer (world, { node }, undoStep);
	requestUpdateInstances (executionContext, undoStep);

	return node;
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
X3DEditor::replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode & node, const SFNode & newValue, const UndoStepPtr & undoStep)
{
	replaceNodes (executionContext, executionContext, executionContext -> getRootNodes (), node, newValue, undoStep);

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

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

/***
 *  Sets @a sfnode to @a newValue.
 */
void
X3DEditor::replaceNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, SFNode & sfnode, const SFNode & newValue, const UndoStepPtr & undoStep)
{
	const auto oldValue = sfnode;

	undoStep -> addObjects (parent);

	//

	const auto undoRemoveNode = std::make_shared <UndoStep> ();

	removeNodesFromSceneIfNotExistsInSceneGraph (executionContext, { newValue }, undoRemoveNode);

	undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	//

	undoStep -> addUndoFunction (&SFNode::setValue, std::ref (sfnode), sfnode);
	undoStep -> addRedoFunction (&SFNode::setValue, std::ref (sfnode), newValue);

	sfnode = newValue;

	removeNodesFromSceneIfNotExistsInSceneGraph (executionContext, { oldValue }, undoStep);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

/***
 *  Sets @a mfnode to @a newValue.
 */
void
X3DEditor::replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const MFNode & newValue, const UndoStepPtr & undoStep)
{
	const auto oldValue = mfnode;

	undoStep -> addObjects (parent);

	//

	const auto undoRemoveNode = std::make_shared <UndoStep> ();

	removeNodesFromSceneIfNotExistsInSceneGraph (executionContext, newValue, undoRemoveNode);

	undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	//

	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);
	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), newValue);

	mfnode = newValue;

	removeNodesFromSceneIfNotExistsInSceneGraph (executionContext, oldValue, undoStep);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

/***
 *  Replaces in @a mfnode all occurences of @a node by @a newValue.
 */
void
X3DEditor::replaceNodes (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const SFNode & node, const SFNode & newValue, const UndoStepPtr & undoStep)
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
X3DEditor::replaceNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const size_t index, const SFNode & newValue, const UndoStepPtr & undoStep)
{
	const SFNode oldValue = mfnode [index];

	undoStep -> addObjects (parent);

	//

	const auto undoRemoveNode = std::make_shared <UndoStep> ();

	removeNodesFromSceneIfNotExistsInSceneGraph (executionContext, { newValue }, undoRemoveNode);

	undoStep -> addUndoFunction (&UndoStep::redo, undoRemoveNode);
	undoStep -> addRedoFunction (&UndoStep::undo, undoRemoveNode);
	undoRemoveNode -> undo ();

	//

	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

	mfnode [index] = newValue;

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

	removeNodesFromSceneIfNotExistsInSceneGraph (executionContext, { oldValue }, undoStep);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
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
X3DEditor::removeNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, SFNode & node, const UndoStepPtr & undoStep)
{
	replaceNode (executionContext, parent, node, nullptr, undoStep);
}

void
X3DEditor::removeNode (const X3DExecutionContextPtr & executionContext, const SFNode & parent, MFNode & mfnode, const size_t index, const UndoStepPtr & undoStep)
{
	if (index < mfnode .size ())
	{
		const SFNode oldValue = mfnode [index];

		undoStep -> addObjects (parent);
		undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

		mfnode .erase (mfnode .begin () + index);

		undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

		removeNodesFromSceneIfNotExistsInSceneGraph (executionContext, { oldValue }, undoStep);

		// Prototype support
	
		requestUpdateInstances (executionContext, undoStep);
	}
}

/***
 *  Removes @a node completely from scene if not exists in scene graph anymore.
 */
void
X3DEditor::removeNodesFromSceneIfNotExistsInSceneGraph (const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const UndoStepPtr & undoStep)
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
X3DEditor::removeNodesFromScene (const X3DExecutionContextPtr & executionContext, const MFNode & nodes, const bool removeFromSceneGraph, const UndoStepPtr & undoStep)
{
	// Remove nodes

	removeNodesFromExecutionContext (executionContext, std::set <SFNode> (nodes .cbegin (), nodes .cend ()), removeFromSceneGraph, undoStep);

	// Remove children of node if not in scene graph.

	// Collect children.

	std::set <SFNode> children;

	traverse (const_cast <MFNode &> (nodes), [&children] (SFNode & child)
	{
		children .emplace (child);
		return true;
	},
	TRAVERSE_PROTOTYPE_INSTANCES);

	if (children .size () > nodes .size ())
	{
		// Filter out nodes still in the scene graph.

		traverse (executionContext -> getRootNodes (), [&children] (SFNode & node)
		{
			children .erase (node);
			return true;
		},
		TRAVERSE_PROTOTYPE_INSTANCES | TRAVERSE_META_DATA);
	}

	// Remove rest, these are only nodes that are not in the scene graph anymore.

	removeNodesFromExecutionContext (executionContext, children, false, undoStep);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

void
X3DEditor::removeNodesFromExecutionContext (const X3DExecutionContextPtr & executionContext,
                                            const std::set <SFNode> & nodes,
                                            const bool removeFromSceneGraph,
                                            const UndoStepPtr & undoStep)
{
	// Remove node from scene graph

	const X3DScenePtr scene (executionContext);

	for (const auto & node : nodes)
		removeReferences (node, undoStep);

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

	if (removeFromSceneGraph)
		removeNodesFromSceneGraph (executionContext, nodes, undoStep);

	// Hide node

	for (const auto & node : nodes)
	{
		undoStep -> addUndoFunction (&X3DBaseNode::beginUpdate, node);
		undoStep -> addRedoFunction (&X3DBaseNode::endUpdate,   node);
		node -> endUpdate ();

		undoStep -> addUndoFunction (&X3DBaseNode::setPrivate, node, false);
		undoStep -> addRedoFunction (&X3DBaseNode::setPrivate, node, true);
		node -> setPrivate (true);
	}

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

void
X3DEditor::removeNodesFromSceneGraph (const X3DExecutionContextPtr & executionContext, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep)
{
	const SFNode executionContextNode (executionContext);

	for (const auto & node : nodes)
		removeNode (executionContextNode, executionContext -> getRootNodes (), node, undoStep);

	removeNodesFromSceneGraph (executionContext -> getRootNodes (), nodes, undoStep);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

void
X3DEditor::removeNodesFromSceneGraph (const MFNode & array, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep)
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
	TRAVERSE_PROTOTYPE_INSTANCES);
}

void
X3DEditor::removeNode (const SFNode & parent, MFNode & mfnode, const SFNode & node, const UndoStepPtr & undoStep)
{
	if (std::find (mfnode .cbegin (), mfnode .cend (), node) == mfnode .cend ())
		return;

	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);

	mfnode .remove (node);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (mfnode), mfnode);
}

void
X3DEditor::removeExportedNodes (const X3DScenePtr & scene, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep)
{
	// Remove exported nodes

	const auto exportedNodes = scene -> getExportedNodes ();

	for (const auto & pair : exportedNodes)
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
X3DEditor::removeNamedNodes (const X3DExecutionContextPtr & executionContext, const std::set <SFNode> & nodes, const UndoStepPtr & undoStep)
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

void
X3DEditor::updateImportedNode (const X3DExecutionContextPtr & executionContext,
                               const X3DPtr <Inline> & inlineNode,
                               const std::string & exportedName,
                               const std::string & importedName,
                               const UndoStepPtr & undoStep)
{
	const auto & iter = executionContext -> getImportedNodes () .find (importedName);

	if (iter == executionContext -> getImportedNodes () .end ())
	{
		undoStep -> addUndoFunction (&X3DExecutionContext::removeImportedNode,
		                             executionContext,
		                             importedName);
	}
	else
	{
		const auto & importedNode = iter -> second;
	
		if (importedNode -> getInlineNode () not_eq inlineNode or importedNode -> getExportedName () not_eq exportedName)
		{
			removeImportedNode (executionContext, importedName, undoStep);
		}
		else
		{
			undoStep -> addUndoFunction (&X3DExecutionContext::updateImportedNode,
			                             executionContext,
			                             importedNode -> getInlineNode (),
			                             importedNode -> getExportedName (),
			                             importedNode -> getImportedName ());
		}
	}

	try
	{
		undoStep -> addRedoFunction (&X3DExecutionContext::updateImportedNode,
		                             executionContext,
		                             inlineNode,
		                             exportedName,
		                             importedName);
	
		executionContext -> updateImportedNode (inlineNode, exportedName, importedName);
	}
	catch (...)
	{ }
}

/***
 *  Only pass inline nodes that are loaded and should be unloaded.
 */
void
X3DEditor::removeImportedNodes (const X3DExecutionContextPtr & executionContext, const std::set <X3DPtr <Inline>> & inlineNodes, const UndoStepPtr & undoStep)
{
	// Remove nodes imported from node

	const auto importedNodes = executionContext -> getImportedNodes ();

	for (const auto & pair : importedNodes)
	{
		const auto & importedNode = pair .second;
		const auto   inlineNode   = importedNode -> getInlineNode ();

		if (inlineNodes .count (inlineNode))
			removeImportedNode (executionContext, pair .first, undoStep);
	}
}

/***
 *  Removes imported node importedName.
 */
void
X3DEditor::removeImportedNode (const X3DExecutionContextPtr & executionContext, const std::string & importedName, const UndoStepPtr & undoStep)
{
	try
	{
		// Remove nodes imported from node
	
		const auto iter = executionContext -> getImportedNodes () .find (importedName);
	
		if (iter == executionContext -> getImportedNodes () .end ())
			return;

		const auto & importedNode = iter -> second;
		const auto   inlineNode   = importedNode -> getInlineNode ();

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
	catch (const X3DError & error)
	{ }
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
X3DEditor::updateNamedNode (const X3DExecutionContextPtr & executionContext, const std::string & name, const SFNode & node, const UndoStepPtr & undoStep)
{
	try
	{
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

		// Prototype support
	
		requestUpdateInstances (executionContext, undoStep);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DEditor::removeNamedNode (const X3DExecutionContextPtr & executionContext, const SFNode & node, const UndoStepPtr & undoStep)
{
	try
	{
		if (node -> getName () .empty ())
			return;

		undoStep -> addObjects (executionContext);

		undoStep -> addUndoFunction (&X3DExecutionContext::updateNamedNode, executionContext, node -> getName (), node);
		undoStep -> addRedoFunction (&X3DExecutionContext::removeNamedNode, executionContext, node -> getName ());
		executionContext -> removeNamedNode (node -> getName ());

		// Prototype support
	
		requestUpdateInstances (executionContext, undoStep);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
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
                                   const UndoStepPtr & undoStep)
{
	if (name .empty ())
		return;

	// Restore prototypes
	undoStep -> addUndoFunction (&X3DEditor::restoreProtoDeclarations, executionContext, executionContext -> getProtoDeclarations ());

	// Update prototype

	undoStep -> addUndoFunction (&X3DExecutionContext::updateProtoDeclaration, executionContext, prototype -> getName (), prototype);
	undoStep -> addRedoFunction (&X3DExecutionContext::updateProtoDeclaration, executionContext, name, prototype);
	executionContext -> updateProtoDeclaration (name, prototype);

	// Restore prototypes
	undoStep -> addRedoFunction (&X3DEditor::restoreProtoDeclarations, executionContext, executionContext -> getProtoDeclarations ());

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

void
X3DEditor::convertProtoToExternProto (const ProtoDeclarationPtr & prototype, const MFString & url, const UndoStepPtr & undoStep)
{
	const auto executionContext = X3DExecutionContextPtr (prototype -> getExecutionContext ());
	const auto externproto      = prototype -> createExternProtoDeclaration (executionContext, url);

	undoStep -> addObjects (executionContext, externproto, prototype);

	externproto -> requestAsyncLoad ();

	// Update extern proto

	try
	{
		const auto currentExternproto = executionContext -> getExternProtoDeclaration (prototype -> getName ());

		undoStep -> addUndoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, prototype -> getName (), currentExternproto);
	}
	catch (const X3DError &)
	{
		undoStep -> addUndoFunction (&X3DExecutionContext::removeExternProtoDeclaration, executionContext, prototype -> getName ());
	}

	undoStep -> addRedoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, prototype -> getName (), externproto);
	executionContext -> updateExternProtoDeclaration (prototype -> getName (), externproto);

	// Remove prototype

	undoStep -> addUndoFunction (&X3DExecutionContext::updateProtoDeclaration, executionContext, prototype -> getName (), prototype);
	undoStep -> addRedoFunction (&X3DExecutionContext::removeProtoDeclaration, executionContext, prototype -> getName ());
	executionContext -> removeProtoDeclaration (prototype -> getName ());

	// Update instances

	const auto instances = prototype -> getInstances ();

	for (const auto & node : instances)
	{
		const X3DPtr <X3DPrototypeInstance> instance (node);

		undoStep -> addObjects (instance);

		undoStep -> addUndoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, prototype);
		undoStep -> addRedoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, externproto);
		instance -> setProtoDeclarationNode (externproto);
	}
}

void
X3DEditor::restoreProtoDeclarations (const X3DExecutionContextPtr & executionContext, const ProtoDeclarationArray & protos)
{
	const auto currentProtos = executionContext -> getProtoDeclarations ();

	for (const auto & prototype : currentProtos)
		executionContext -> removeProtoDeclaration (prototype -> getName ());

	for (const auto & prototype : protos)
		executionContext -> updateProtoDeclaration (prototype -> getName (), prototype);
}

void
X3DEditor::updateExternProtoDeclaration (const X3DExecutionContextPtr & executionContext,
                                         const std::string & name,
                                         const ExternProtoDeclarationPtr & externProto,
                                         const UndoStepPtr & undoStep)
{
	if (name .empty ())
		return;

	// Restore externprotos.
	undoStep -> addUndoFunction (&X3DEditor::restoreExternProtoDeclarations, executionContext, executionContext -> getExternProtoDeclarations ());

	// Update name.
	undoStep -> addUndoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, externProto -> getName (), externProto);
	undoStep -> addRedoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, name, externProto);
	executionContext -> updateExternProtoDeclaration (name, externProto);

	// Update url.

	if (externProto -> checkLoadState () not_eq COMPLETE_STATE)
		return;

	undoStep -> addUndoFunction (&MFString::setValue, std::ref (externProto -> url ()), externProto -> url ());

	for (MFString::reference URL : externProto -> url ())
	{
		basic::uri uri (URL .get ());

		uri .fragment (externProto -> getProtoDeclaration () -> getName ());

		URL .set (uri .str ());
	}

	undoStep -> addRedoFunction (&MFString::setValue, std::ref (externProto -> url ()), externProto -> url ());

	// Restore externprotos.
	undoStep -> addRedoFunction (&X3DEditor::restoreExternProtoDeclarations, executionContext, executionContext -> getExternProtoDeclarations ());

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

void
X3DEditor::foldExternProtoBackIntoScene (const ExternProtoDeclarationPtr & externproto, const UndoStepPtr & undoStep)
{
	const auto executionContext = X3DExecutionContextPtr (externproto -> getExecutionContext ());
	const auto browser          = executionContext -> getBrowser ();
	const auto internalScene    = externproto -> getInternalScene ();
	const auto prototype        = ProtoDeclarationPtr (externproto -> getProtoDeclaration ());

	undoStep -> addObjects (executionContext, externproto);

	// Remove extern proto

	undoStep -> addUndoFunction (&X3DExecutionContext::updateExternProtoDeclaration, executionContext, externproto -> getName (), externproto);
	undoStep -> addRedoFunction (&X3DExecutionContext::removeExternProtoDeclaration, executionContext, externproto -> getName ());
	executionContext -> removeExternProtoDeclaration (externproto -> getName ());

	// Import proto and all what is needed.

	basic::ifilestream ifstream (exportNodes (internalScene, { prototype }, "XML", false));

	const auto scene         = browser -> createX3DFromStream (internalScene -> getWorldURL (), ifstream);
	const auto importedNodes = importScene (executionContext, scene, undoStep);

	for (const auto & node : importedNodes)
		pushBackIntoArray (executionContext, executionContext -> getRootNodes (), node, undoStep);

	// Update instances

	const auto importedProto = executionContext -> getProtoDeclaration (prototype -> getName ());
	const auto instances     = externproto -> getInstances ();

	for (const auto & node : instances)
	{
		const X3DPtr <X3DPrototypeInstance> instance (node);

		undoStep -> addObjects (instance);

		undoStep -> addUndoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, externproto);
		undoStep -> addRedoFunction (&X3DPrototypeInstance::setProtoDeclarationNode, instance, importedProto);
		instance -> setProtoDeclarationNode (importedProto);
	}
}

void
X3DEditor::restoreExternProtoDeclarations (const X3DExecutionContextPtr & executionContext, const ExternProtoDeclarationArray & externprotos)
{
	const auto currentExternProtos = executionContext -> getExternProtoDeclarations ();

	for (const auto & externproto : currentExternProtos)
		executionContext -> removeExternProtoDeclaration (externproto -> getName ());

	for (const auto & externproto : externprotos)
		executionContext -> updateExternProtoDeclaration (externproto -> getName (), externproto);
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
X3DEditor::deleteRoutes (const X3DExecutionContextPtr & executionContext, const SFNode & node, const UndoStepPtr & undoStep)
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

					deleteRoute (X3DExecutionContextPtr (route -> getExecutionContext ()),
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

					deleteRoute (X3DExecutionContextPtr (route -> getExecutionContext ()),
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

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

void
X3DEditor::addRoute (const X3DExecutionContextPtr & executionContext,
                     const SFNode & sourceNode,
                     const std::string & sourceField,
                     const SFNode & destinationNode,
                     const std::string & destinationField,
                     const UndoStepPtr & undoStep)
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

		if (sourceNode -> getName () .empty ())
			updateNamedNode (X3DExecutionContextPtr (sourceNode -> getExecutionContext ()), sourceNode -> getExecutionContext () -> getUniqueName (), sourceNode, undoStep);

		if (destinationNode -> getName () .empty ())
			updateNamedNode (X3DExecutionContextPtr (destinationNode -> getExecutionContext ()), destinationNode -> getExecutionContext () -> getUniqueName (), destinationNode, undoStep);

		undoStep -> addRedoFunction ((addRoute) & X3DExecutionContext::addRoute, executionContext,
		                             std::bind (call, sourceNodeFn),
		                             sourceField,
		                             std::bind (call, destinationNodeFn),
		                             destinationField);

		executionContext -> addRoute (sourceNode, sourceField, destinationNode, destinationField);

		// Prototype support

		requestUpdateInstances (executionContext, undoStep);
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DEditor::deleteRoute (const X3DExecutionContextPtr & executionContext,
                        const SFNode & sourceNode,
                        const std::string & sourceField,
                        const SFNode & destinationNode,
                        const std::string & destinationField,
                        const UndoStepPtr & undoStep)
{
	using getImportedNode = SFNode          (X3DExecutionContext::*) (const std::string &) const;
	using addRoute        = const RoutePtr &(X3DExecutionContext::*) (const SFNode &, const std::string &, const SFNode &, const std::string &);
	using deleteRoute     = void            (X3DExecutionContext::*) (const SFNode &, const std::string &, const SFNode &, const std::string &);

	if (sourceNode -> getScene () -> getPrivate ())
		return;

	if (destinationNode -> getScene () -> getPrivate ())
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

	if (sourceNode -> getName () .empty ())
		updateNamedNode (X3DExecutionContextPtr (sourceNode -> getExecutionContext ()), sourceNode -> getExecutionContext () -> getUniqueName (), sourceNode, undoStep);

	if (destinationNode -> getName () .empty ())
		updateNamedNode (X3DExecutionContextPtr (destinationNode -> getExecutionContext ()), destinationNode -> getExecutionContext () -> getUniqueName (), destinationNode, undoStep);

	executionContext -> deleteRoute (sourceNode, sourceField, destinationNode, destinationField);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

std::vector <std::tuple <SFNode, std::string, SFNode, std::string>> 
X3DEditor::getImportedRoutes (const X3DExecutionContextPtr & executionContext, const X3DScenePtr & scene)
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
 * Prototype operations
 *
 *
 *
 */

bool
X3DEditor::isProtoUsedInProto (ProtoDeclaration* const child, ProtoDeclaration* const parent)
{
	const auto traversed = traverse (parent, [&] (SFNode & node)
	{
		const auto instance = dynamic_cast <X3DPrototypeInstance*> (node .getValue ());

		if (instance)
		{
			if (instance -> getProtoDeclarationNode () == child)
				return false;
		}

		return true;
	},
	TRAVERSE_PROTO_DECLARATIONS |
	TRAVERSE_PROTO_DECLARATION_BODY |
	TRAVERSE_ROOT_NODES |
	TRAVERSE_PROTOTYPE_INSTANCES);

	return not traversed;
}

void
X3DEditor::removeUnusedPrototypes (const X3DExecutionContextPtr & executionContext, const UndoStepPtr & undoStep)
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

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

void
X3DEditor::removeUsedPrototypes (const X3DExecutionContextPtr & executionContext,
                                 std::map <ExternProtoDeclarationPtr, size_t> & externProtos,
                                 std::map <ProtoDeclarationPtr, size_t> & prototypes)
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
					
					switch (instance -> getProtoDeclarationNode () -> getType () .back ())
					{
						case  X3DConstants::ExternProtoDeclaration:
						{
							const ExternProtoDeclarationPtr externProto (instance -> getProtoDeclarationNode ());
							
							externProtos .erase (externProto);
							break;
						}
						case  X3DConstants::ProtoDeclaration:
						{
							const ProtoDeclarationPtr prototype (instance -> getProtoDeclarationNode ());
							
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
	TRAVERSE_PROTOTYPE_INSTANCES |
	TRAVERSE_META_DATA);
}

void
X3DEditor::requestUpdateInstances (const ExternProtoDeclarationPtr & externProto, const UndoStepPtr & undoStep)
{
	requestUpdateInstances (X3DProtoDeclarationNodePtr (externProto), undoStep);
}

void
X3DEditor::requestUpdateInstances (const ProtoDeclarationPtr & prototype, const UndoStepPtr & undoStep)
{
	requestUpdateInstances (X3DProtoDeclarationNodePtr (prototype), undoStep);
}

void
X3DEditor::requestUpdateInstances (const X3DProtoDeclarationNodePtr & protoNode, const UndoStepPtr & undoStep)
{
	if (not protoNode)
		return;

	std::vector <X3DProtoDeclarationNodePtr> protoNodes;

	X3DExecutionContext* executionContext = dynamic_cast <X3DExecutionContext*> (protoNode .getValue ());

	if (protoNode -> isExternproto ())
	{
		protoNodes .emplace_back (protoNode);

		executionContext = protoNode -> getExecutionContext ();
	}

	while (executionContext and executionContext -> isType ({ X3DConstants::ProtoDeclaration }))
	{
		ProtoDeclarationPtr prototype (executionContext);

		if (protoNode)
			protoNodes .emplace_back (std::move (protoNode));

		executionContext = executionContext -> getExecutionContext ();
	}

	for (const auto & protoNode : protoNodes)
	{
		undoStep -> addUndoFunction (&X3DProtoDeclarationNode::requestUpdateInstances, protoNode);
		undoStep -> addRedoFunction (&X3DProtoDeclarationNode::requestUpdateInstances, protoNode);
		protoNode -> requestUpdateInstances ();
	}
}

void
X3DEditor::requestUpdateInstances (const X3DExecutionContextPtr & executionContext, const UndoStepPtr & undoStep)
{
	requestUpdateInstances (X3DProtoDeclarationNodePtr (executionContext), undoStep);
}

void
X3DEditor::requestUpdateInstances (const SFNode & node, const UndoStepPtr & undoStep)
{
	requestUpdateInstances (X3DProtoDeclarationNodePtr (node -> getExecutionContext ()), undoStep);
}

void
X3DEditor::addReference (const SFNode & node, X3DFieldDefinition* const field, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (node, FieldPtr (field), FieldPtr (protoField));

	undoStep -> addUndoFunction (&X3DFieldDefinition::addEvent, field);
	undoStep -> addUndoFunction (&X3DFieldDefinition::set, field, std::bind (&X3DEditor::getField, FieldPtr (field -> copy (FLAT_COPY))));
	undoStep -> addUndoFunction (&X3DFieldDefinition::removeReference, field, protoField);
	undoStep -> addRedoFunction (&X3DFieldDefinition::addReference,    field, protoField);
	field -> addReference (protoField);

	requestUpdateInstances (node, undoStep);
}

void
X3DEditor::removeReference (const SFNode & node, X3DFieldDefinition* const field, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (node, FieldPtr (field), FieldPtr (protoField));

	undoStep -> addUndoFunction (&X3DFieldDefinition::addReference,    field, protoField);
	undoStep -> addRedoFunction (&X3DFieldDefinition::removeReference, field, protoField);
	field -> removeReference (protoField);

	requestUpdateInstances (node, undoStep);
}

void
X3DEditor::removeReferences (const SFNode & node, const UndoStepPtr & undoStep)
{
	for (const auto & field : node -> getFieldDefinitions ())
	{
		const auto references = field -> getReferences ();

		for (const auto & protoField : references)
			removeReference (node, field, protoField, undoStep);
	}
}

const X3DFieldDefinition &
X3DEditor::getField (const FieldPtr & fieldPtr)
{
	return *fieldPtr .getValue ();
}

/***
 *
 *
 *
 * Fields operations
 *
 *
 *
 */

void
X3DEditor::setValue (const SFNode & node, X3DFieldDefinition & field, const X3DFieldDefinition & value, const UndoStepPtr & undoStep)
{
	if (field .getType () not_eq value .getType ())
		return;

	undoStep -> addObjects (node, FieldPtr (&field));
	
	undoStep -> addUndoFunction (&X3DFieldDefinition::addEvent, &field);
	undoStep -> addUndoFunction (&X3DFieldDefinition::set, &field, std::bind (&X3DEditor::getField, FieldPtr (field .copy (FLAT_COPY))));

	field .set (value);
	field .addEvent ();

	undoStep -> addRedoFunction (&X3DFieldDefinition::set, &field, std::bind (&X3DEditor::getField, FieldPtr (field .copy (FLAT_COPY))));
	undoStep -> addUndoFunction (&X3DFieldDefinition::addEvent, &field);

	requestUpdateInstances (node, undoStep);
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
X3DEditor::addUserDefinedField (const SFNode & node, X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (FieldPtr (field));

	undoStep -> addUndoFunction (&X3DBaseNode::removeUserDefinedField, node, field -> getName ());
	undoStep -> addRedoFunction (&X3DBaseNode::addUserDefinedField, node, field -> getAccessType (), field -> getName (), field);

	node -> addUserDefinedField (field -> getAccessType (), field -> getName (), field);

	// Prototype support

	requestUpdateInstances (node, undoStep);
}

///  This function is with reference handling.
void
X3DEditor::updateUserDefinedField (const SFNode & node, const AccessType accessType, const std::string & name, X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	auto userDefinedFields = node -> getUserDefinedFields ();
	auto iter              = std::find (userDefinedFields .cbegin (), userDefinedFields .cend (), field);

	if (iter == userDefinedFields .end ())
		return;

	const ProtoDeclarationPtr proto (node);

	// Save all involved fields.

	undoStep -> addObjects (FieldArray (userDefinedFields .cbegin (), userDefinedFields .cend ()));

	// If possible we want to reassign the routes from the old field to the new fields.  In this step we create addRoutes
	// functions we will execute later.

	// Reassign IS reference to the new field.

	const auto references = field -> getReferences ();

	undoStep -> addObjects (FieldArray (references .cbegin (), references .cend ()));

	for (const auto & reference : references)
	{
		undoStep -> addUndoFunction (&X3DFieldDefinition::addReference,    field, reference);
		undoStep -> addRedoFunction (&X3DFieldDefinition::removeReference, field, reference);
		field -> removeReference (reference);
	}

	for (const auto & reference : references)
	{
		if (reference -> isReference (accessType))
		{
			undoStep -> addUndoFunction (&X3DFieldDefinition::removeReference, field, reference);
			undoStep -> addRedoFunction (&X3DFieldDefinition::addReference,    field, reference);
			field -> addReference (reference);
		}
	}

	// Create addRoutes functions for input and output routes.

	std::deque <std::function <void ()>>  addRouteFunctions;

	getAddRouteFunctions (node, accessType, name, field, undoStep, addRouteFunctions);

	// Remove routes from field.  We must do this as routes are associated with a node and we are self responsible for doing this.

	removeRoutes (field, undoStep);

	// Proto instance handling

	std::map <X3DPrototypeInstance*, FieldDefinitionSet> referencesIndex;

	if (proto)
	{
		for (const auto instance : proto -> getInstances ())
		{
			try
			{
				const SFNode instanceNode (instance);

				// Create addRoutes functions for input and output routes.
	
				const auto instanceField = instance -> getField (field -> getName ());
	
				// Collect IS reference to the new field.

				const auto references = instanceField -> getReferences ();
	
				undoStep -> addObjects (FieldArray (references .cbegin (), references .cend ()));

				referencesIndex .emplace (instance, references);

				undoStep -> addUndoFunction (&X3DEditor::addReferences, instanceNode, field -> getName (), references);
	
				// Create addRoutes functions for input and output routes.
	
				getAddRouteFunctions (instanceNode, accessType, name, instanceField, undoStep, addRouteFunctions);
	
				removeRoutes (instanceField, undoStep);

				requestUpdateInstances (instanceNode, undoStep);
			}
			catch (const X3DError &)
			{ }
		}

		undoStep -> addUndoFunction (&ProtoDeclaration::updateInstances, proto);
	}

	// Update user defined fields.

	undoStep -> addUndoFunction (&X3DBaseNode::updateUserDefinedField, node, field -> getAccessType (), field -> getName (), field);
	undoStep -> addRedoFunction (&X3DBaseNode::updateUserDefinedField, node, accessType, name, field);
	node -> updateUserDefinedField (accessType, name, field);

	// Prototype support

	if (proto)
	{
		// Immediately update instance to add possible routes now.
		undoStep -> addRedoFunction (&ProtoDeclaration::updateInstances, proto);
		proto -> updateInstances (); 

		for (const auto instance : proto -> getInstances ())
		{
			const SFNode instanceNode (instance);

			const auto iter = referencesIndex .find (instance);

			if (iter == referencesIndex .end ())
				continue;

			undoStep -> addRedoFunction (&X3DEditor::addReferences, instanceNode, name, iter -> second);
			addReferences (instanceNode, name, iter -> second);
		}
	}

	// Now process the addRoutes functions recorded above to reassign the routes from the old field to the new field and create undo/redo steps.

	for (const auto & addRouteFunction : addRouteFunctions)
	{
		try
		{
			addRouteFunction ();
		}
		catch (const X3DError &)
		{ }
	}

	// Handle IS references, if node is proto.

	replaceReferences (proto, field, field, undoStep);
}

///  This function is with reference handling.
void
X3DEditor::replaceUserDefinedField (const SFNode & node, X3DFieldDefinition* const oldField, X3DFieldDefinition* const newField, const UndoStepPtr & undoStep)
{
	auto userDefinedFields = node -> getUserDefinedFields ();
	auto iter              = std::find (userDefinedFields .begin (), userDefinedFields .end (), oldField);

	if (iter == userDefinedFields .end ())
		return;

	// Save all involved fields.

	undoStep -> addObjects (FieldArray (userDefinedFields .cbegin (), userDefinedFields .cend ()), FieldPtr (newField));

	// Handle IS references, if node is proto.

	const ProtoDeclarationPtr proto (node);

	if (oldField -> getType () == newField -> getType ())
		replaceReferences (proto, oldField, newField, undoStep);

	else
		removeReferences (proto, oldField, undoStep);

	// If possible we want to reassign the routes from the old field to the new fields.  In this step we create addRoutes
	// functions we will execute later.

	std::deque <std::function <void ()>>  addRouteFunctions;

	if (newField -> getType () == oldField -> getType ())
	{
		// Assign the old value to the new field.

		newField -> set (*oldField);

		// Reassign IS reference to the new field.

		const auto references = oldField -> getReferences ();
	
		undoStep -> addObjects (FieldArray (references .cbegin (), references .cend ()));

		for (const auto & reference : references)
		{
			if (reference -> isReference (newField -> getAccessType ()))
			{
				undoStep -> addUndoFunction (&X3DFieldDefinition::removeReference, newField, reference);
				undoStep -> addRedoFunction (&X3DFieldDefinition::addReference,    newField, reference);
				newField -> addReference (reference);
			}
		}

		// Create addRoutes functions for input and output routes.

		getAddRouteFunctions (node, newField -> getAccessType (), newField -> getName (), oldField,  undoStep, addRouteFunctions);
	}

	// Remove user data from old field.

	undoStep -> addUndoFunction (&X3DFieldDefinition::setUserData, oldField, nullptr);
	undoStep -> addRedoFunction (&X3DFieldDefinition::setUserData, newField, nullptr);
	newField -> setUserData (nullptr);

	// Remove routes from field.  We must do this as routes are associated with a node and we are self responsible for doing this.

	removeRoutes (oldField, undoStep);

	// Proto instance handling

	std::map <X3DPrototypeInstance*, FieldDefinitionSet> referencesIndex;

	if (proto)
	{
		for (const auto instance : proto -> getInstances ())
		{
			try
			{
				const SFNode instanceNode (instance);

				const auto instanceField = instance -> getField (oldField -> getName ());
	
				// Collect IS reference to the new field.

				const auto references = instanceField -> getReferences ();
	
				undoStep -> addObjects (FieldArray (references .cbegin (), references .cend ()));

				referencesIndex .emplace (instance, references);

				undoStep -> addUndoFunction (&X3DEditor::addReferences, instanceNode, oldField -> getName (), references);
	
				// Create addRoutes functions for input and output routes.

				if (newField -> getType () == oldField -> getType ())
					getAddRouteFunctions (instanceNode, newField -> getAccessType (), newField -> getName (), instanceField, undoStep, addRouteFunctions);

				removeRoutes (instanceField, undoStep);

				requestUpdateInstances (instanceNode, undoStep);
			}
			catch (const X3DError &)
			{ }
		}

		undoStep -> addUndoFunction (&ProtoDeclaration::updateInstances, proto);
	}

	// Restore old user defined fields in undo.

	undoStep -> addUndoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	// Replace old field with new field in temporary array.

	*iter = newField;

	// Set new user defined fields.

	undoStep -> addRedoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	node -> setUserDefinedFields (userDefinedFields);

	// Prototype support

	if (proto)
	{
		// Immediately update instance to add possible routes now.
		undoStep -> addRedoFunction (&ProtoDeclaration::updateInstances, proto);
		proto -> updateInstances ();

		for (const auto instance : proto -> getInstances ())
		{
			const SFNode instanceNode (instance);

			const auto iter = referencesIndex .find (instance);

			if (iter == referencesIndex .end ())
				continue;

			undoStep -> addRedoFunction (&X3DEditor::addReferences, instanceNode, newField -> getName (), iter -> second);
			addReferences (instanceNode, newField -> getName (), iter -> second);
		}
	}

	// Now process the addRoutes functions recorded above to reassign the routes from the old field to the new field and create undo/redo steps.

	for (const auto & addRouteFunction : addRouteFunctions)
	{
		try
		{
			addRouteFunction ();
		}
		catch (const X3DError &)
		{ }
	}
}

void
X3DEditor::addReferences (const SFNode & node, const std::string & fieldName, const FieldDefinitionSet & references)
{
	try
	{
		const auto field = node -> getField (fieldName);
	
		for (const auto & reference : references)
		{
			field -> removeReference (reference);

			if (reference -> getType () == field -> getType ())
			{
				if (reference -> isReference (field -> getAccessType ()))
					field -> addReference (reference);
			}
		}
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DEditor::getAddRouteFunctions (const SFNode & node,
                                 const AccessType accessType,
                                 const std::string & name,
                                 X3DFieldDefinition* const field,
                                 const UndoStepPtr & undoStep,
                                 std::deque <std::function <void ()>> & functions)
{
	if (accessType & inputOnly and field -> isInput ())
	{
		for (const auto & route : field -> getInputRoutes ())
		{
			const bool selfConnection = route -> getSourceNode () == node and route -> getSourceField () == field -> getName ();

			functions .emplace_back (std::bind (&X3DEditor::addRoute,
			                                    X3DExecutionContextPtr (node -> getExecutionContext ()),
			                                    route -> getSourceNode (),
			                                    selfConnection ? name : route -> getSourceField (),
			                                    node,
			                                    name,
			                                    undoStep));
		}
	}

	if (accessType & outputOnly and field -> isOutput ())
	{
		for (const auto & route : field -> getOutputRoutes ())
		{
			const bool selfConnection = route -> getDestinationNode () == node and route -> getDestinationField () == field -> getName ();

			functions .emplace_back (std::bind (&X3DEditor::addRoute,
			                                    X3DExecutionContextPtr (node -> getExecutionContext ()),
			                                    node,
			                                    name,
			                                    route -> getDestinationNode (),
			                                    selfConnection ? name : route -> getDestinationField (),
			                                    undoStep));
		}
	}
}

///  This function is with reference handling.
void
X3DEditor::removeUserDefinedField (const SFNode & node, X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	// Handle IS references, if node is proto.

	removeReferences (ProtoDeclarationPtr (node), field, undoStep);

	// Remove field from node

	const auto userDefinedFields = node -> getUserDefinedFields ();

	undoStep -> addObjects (FieldArray (userDefinedFields .cbegin (), userDefinedFields .cend ()), FieldPtr (field));

	// Remove user data from old field.

	undoStep -> addUndoFunction (&X3DFieldDefinition::setUserData, field, nullptr);

	// Remove routes from field.  We must do this as routes are associated with a node and we are self responsible for doing this.

	removeRoutes (field, undoStep);

	undoStep -> addUndoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);
	undoStep -> addRedoFunction (&X3DBaseNode::removeUserDefinedField, node, field -> getName ());

	node -> removeUserDefinedField (field -> getName ());

	// Prototype support

	requestUpdateInstances (node, undoStep);
}

///  This function is without reference handling.
void
X3DEditor::setUserDefinedFields (const SFNode & node, const FieldDefinitionArray & userDefinedFields, const UndoStepPtr & undoStep)
{
	// Remove any routes and user data.

	const auto currentUserDefinedFields = node -> getUserDefinedFields ();

	std::set <X3DFieldDefinition*>    lhs (currentUserDefinedFields .cbegin (), currentUserDefinedFields .cend ());
	std::set <X3DFieldDefinition*>    rhs (userDefinedFields .cbegin (), userDefinedFields .cend ());
	std::vector <X3DFieldDefinition*> difference;

	std::set_difference (lhs .cbegin (), lhs .cend (), rhs .cbegin (), rhs .cend (), std::back_inserter (difference));

	for (const auto & field : difference)
	{
		undoStep -> addUndoFunction (&X3DFieldDefinition::setUserData, field, nullptr);
		removeRoutes (field, undoStep);
	}

	// Replace the user defined fields.

	undoStep -> addUndoFunction (&X3DBaseNode::setUserDefinedFields, node, currentUserDefinedFields);
	undoStep -> addRedoFunction (&X3DBaseNode::setUserDefinedFields, node, userDefinedFields);

	node -> setUserDefinedFields (userDefinedFields);

	// Prototype support

	requestUpdateInstances (node, undoStep);
}

void
X3DEditor::removeRoutes (X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	// Remove routes from field.

	for (const auto & route : RouteSet (field -> getInputRoutes ()))
	{
		deleteRoute (X3DExecutionContextPtr (route -> getExecutionContext ()),
		             route -> getSourceNode (),
		             route -> getSourceField (),
		             route -> getDestinationNode (),
		             route -> getDestinationField (),
		             undoStep);
	}

	for (const auto & route : RouteSet (field -> getOutputRoutes ()))
	{
		deleteRoute (X3DExecutionContextPtr (route -> getExecutionContext ()),
		             route -> getSourceNode (),
		             route -> getSourceField (),
		             route -> getDestinationNode (),
		             route -> getDestinationField (),
		             undoStep);
	}
}

void
X3DEditor::replaceReferences (const ProtoDeclarationPtr & proto, X3DFieldDefinition* const oldField, X3DFieldDefinition* const newField, const UndoStepPtr & undoStep)
{
	using namespace std::placeholders;
	
	undoStep -> addObjects (proto);

	if (proto)
		traverse (proto, std::bind (&X3DEditor::replaceReferencesCallback, _1, oldField, newField, undoStep));
}

bool
X3DEditor::replaceReferencesCallback (SFNode & node, X3DFieldDefinition* const oldProtoField, X3DFieldDefinition* const newProtoField, const UndoStepPtr & undoStep)
{
	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (field -> getReferences () .count (oldProtoField))
		{
			undoStep -> addObjects (node);
			removeReference (node, field, oldProtoField, undoStep);

			// Test if newProtoField is a reference for field.
			if (newProtoField -> isReference (field -> getAccessType ()))
				addReference (node, field, newProtoField, undoStep);
		}
	}

	return true;
}

void
X3DEditor::removeReferences (const ProtoDeclarationPtr & proto, X3DFieldDefinition* const field, const UndoStepPtr & undoStep)
{
	using namespace std::placeholders;
	
	undoStep -> addObjects (proto);

	if (proto)
		traverse (proto, std::bind (&X3DEditor::removeReferencesCallback, _1, field, undoStep));
}

bool
X3DEditor::removeReferencesCallback (SFNode & node, X3DFieldDefinition* const protoField, const UndoStepPtr & undoStep)
{
	for (const auto & field : node -> getFieldDefinitions ())
	{
		if (field -> getReferences () .count (protoField))
		{
			undoStep -> addObjects (node);
			removeReference (node, field, protoField, undoStep);
		}
	}

	return true;
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

MFNode
X3DEditor::replaceNodes (const X3DExecutionContextPtr & executionContext,
                         const std::string & x3dSyntax,
                         const MFNode & nodes,
                         const bool assign,
                         const UndoStepPtr & undoStep)
{
	if (nodes .empty ())
		return MFNode ();

	const auto scene         = executionContext -> getBrowser () -> createX3DFromString (x3dSyntax);
	auto       importedNodes = importScene (executionContext, scene, undoStep);

	if (importedNodes .empty ())
		return MFNode ();

	// Replace or assign importedNodes.

	auto selection = MFNode ({ importedNodes .front () });
	auto unused    = MFNode ();

	if (assign)
		selection = assignNode (executionContext, importedNodes .front (), nodes, undoStep);
	else
		createClone (executionContext, importedNodes .front (), nodes, undoStep);

	// Remove unused nodes.

	std::sort (importedNodes .begin (), importedNodes .end ());
	std::sort (selection .begin (), selection .end ());

	std::set_difference (importedNodes .cbegin (), importedNodes .cend (), selection .cbegin (), selection .cend (), std::back_inserter (unused));

	removeNodesFromScene (executionContext, unused, false, undoStep);

	return selection;
}

MFNode
X3DEditor::assignNode (const X3DExecutionContextPtr & executionContext,
                       const SFNode & other,
                       const MFNode & nodes,
                       const UndoStepPtr & undoStep)
{
	auto selection = MFNode ();
	auto added     = false;

	for (const auto & node : nodes)
	{
		if (assignNode (node, other, undoStep))
		{
			selection .emplace_back (node);
			continue;
		}

		if (not added)
		{
			added = true;
			selection .emplace_back (other);
		}

		// Replace node by other.
		createClone (executionContext, other, { node }, undoStep);
	}

	return selection;
}

bool
X3DEditor::assignNode (const SFNode & node, const SFNode & other, const UndoStepPtr & undoStep)
{
	// Compare types with tool support.

	if (node -> isType ({ other -> getType () .back () }) or other -> isType ({ node -> getType () .back () }))
	{
		for (const auto lhs : node -> getFieldDefinitions ())
		{
			try
			{
				if (not lhs -> isInitializable ())
					continue;

				const auto rhs = other -> getField (lhs -> getName ());

				if (lhs -> equals (*rhs))
					continue;

				switch (lhs -> getType ())
				{
					case X3DConstants::SFNode:
					{
						const auto lhsSFNode = static_cast <SFNode*> (lhs);
						const auto rhsSFNode = static_cast <SFNode*> (rhs);
		
						if (assignNode (*lhsSFNode, *rhsSFNode, undoStep))
							continue;

						setValue (node, *lhs, *rhs, undoStep);
						continue;
					}
					case X3DConstants::MFNode:
					{
						const auto lhsMFNode = static_cast <MFNode*> (lhs);
						const auto rhsMFNode = static_cast <MFNode*> (rhs);

						if (lhsMFNode -> size () == rhsMFNode -> size ())
						{
							size_t m    = 0;
							size_t size = lhsMFNode -> size ();

							// Collect next undo steps
							std::vector <UndoStepPtr> childUndoSteps;

							for (; m < size; ++ m)
							{
								const auto childUndoStep = std::make_shared <UndoStep> ();

								childUndoSteps .emplace_back (childUndoStep);

								if (assignNode ((*lhsMFNode) [m], (*rhsMFNode) [m], childUndoStep))
									continue;

								break;
							}

							if (m == size)
							{
								// If arrays are equal in size and types add undo step.

								for (const auto & childUndoStep : childUndoSteps)
								{
									undoStep -> addUndoFunction (&UndoStep::undo, childUndoStep);
									undoStep -> addRedoFunction (&UndoStep::redo, childUndoStep);
								}

								continue;
							}
						}

						// Unless arrays are equal in size and types assign rhs to lhs.

						setValue (node, *lhs, *rhs, undoStep);
						continue;
					}
					default:
					{
						setValue (node, *lhs, *rhs, undoStep);
						continue;
					}
				}
			}
			catch (const X3DError & error)
			{ }
		}
	
		return true;
	}

	return false;
}

void
X3DEditor::createClone (const X3DExecutionContextPtr & executionContext,
                        const SFNode & clone,
                        const MFNode & nodes,
                        const UndoStepPtr & undoStep)
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
		TRAVERSE_PROTOTYPE_INSTANCES);

		replaceNodes (executionContext, executionContext, executionContext -> getRootNodes (), node, clone, undoStep);
	}

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

MFNode
X3DEditor::unlinkClone (const X3DExecutionContextPtr & executionContext,
                        const MFNode & clones,
                        const UndoStepPtr & undoStep)
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
									const SFNode copy ((*sfnode) -> copy (FLAT_COPY));
									
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
		TRAVERSE_PROTOTYPE_INSTANCES);

		// Unlink in rootNodes array.

		unlinkClone (executionContext, executionContext, executionContext -> getRootNodes (), clone, nodes, first, undoStep);
	}

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);

	return nodes;
}

void
X3DEditor::unlinkClone (const X3DExecutionContextPtr & executionContext,
                        const SFNode & parent,
                        MFNode & mfnode,
                        const SFNode & clone,
                        MFNode & nodes,
                        bool & first,
                        const UndoStepPtr & undoStep)
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
				const SFNode copy (mfnode [index] -> copy (FLAT_COPY));

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
                       const UndoStepPtr & undoStep)
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
		auto       childModelMatrix = getModelMatrix (executionContext, child);
		const auto transform        = X3DPtr <X3DTransformNode> (child);

		if (transform)
		{
			childModelMatrix .mult_left (transform -> getMatrix ());

			setMatrix (transform, childModelMatrix, undoStep);
		}

		// Remove child from scene graph

		removeNodesFromSceneGraph (executionContext, { child }, undoStep);

		// Add to group

		pushBackIntoArray (groupNode, group -> children (), child, undoStep);
	}

	// Add to layer

	SFNode node (group);

	addToLayers (executionContext, layers, node, undoStep);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);

	return node;
}

MFNode
X3DEditor::ungroupNodes (const X3DExecutionContextPtr & executionContext,
                         const MFNode & groups,
                         const UndoStepPtr & undoStep)
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

				auto       childModelMatrix = getModelMatrix (executionContext, child);
				const auto transform        = X3DPtr <X3DTransformNode> (child);

				if (transform)
				{
					childModelMatrix .mult_left (transform -> getMatrix ());

					setMatrix (transform, childModelMatrix, undoStep);
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

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);

	return children;
}

bool
X3DEditor::addToGroup (const X3DExecutionContextPtr & executionContext,
                       const SFNode & group,
                       const MFNode & children,
                       const UndoStepPtr & undoStep)
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

					auto       groupModelMatrix = getModelMatrix (executionContext, group);
					const auto groupTransform   = X3DPtr <X3DTransformMatrix3DObject> (group);

					if (groupTransform)
						groupModelMatrix .mult_left (groupTransform -> getMatrix ());

					// Adjust child transformation

					auto childModelMatrix = getModelMatrix (executionContext, child);

					childModelMatrix .mult_left (childTransform -> getMatrix ());
					childModelMatrix .mult_right (inverse (groupModelMatrix));

					setMatrix (childTransform, childModelMatrix, undoStep);
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

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);

	return added;
}

void
X3DEditor::detachFromGroup (const X3DExecutionContextPtr & executionContext,
                            const MFNode & children_,
                            const bool detachToLayer0,
                            const UndoStepPtr & undoStep)
{
	MFNode children = children_;

	for (const auto & child : children)
	{
		// Get layers before removing from scene graph.

		const auto layers = findParents <X3DLayerNode> (child);

		// Adjust transformation

		const auto transform = X3DPtr <X3DTransformNode> (child);

		if (transform)
		{
			auto childModelMatrix = getModelMatrix (executionContext, child);

			childModelMatrix .mult_left (transform -> getMatrix ());

			setMatrix (transform, childModelMatrix, undoStep);
		}

		// Remove child from scene graph

		removeNodesFromSceneGraph (executionContext, { child }, undoStep);

		// Add to layers

		if (detachToLayer0)
			pushBackIntoArray (executionContext, executionContext -> getRootNodes (), child, undoStep);

		else
		   addToLayers (executionContext, layers, child, undoStep);
	}

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

SFNode
X3DEditor::createParentGroup (const X3DExecutionContextPtr & executionContext,
                              const std::string & typeName,
                              const std::string & fieldName,
                              const MFNode & children,
                              const UndoStepPtr & undoStep)
{
	// Add node to group

	const SFNode group = executionContext -> createNode (typeName);

	if (not children .empty ())
	{
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
	
		createParentGroup (executionContext, group, executionContext -> getRootNodes (), leader, executionContext, undoStep);
	
		MFNode tail (children .cbegin (), children .cend () - 1);
	
		addToGroup (executionContext, group, tail, undoStep);
		pushBackIntoArray (group, group -> getField <MFNode> (fieldName), leader, undoStep);
	}

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);

	return group;
}

void
X3DEditor::createParentGroup (const X3DExecutionContextPtr & executionContext,
                              const SFNode & group,
                              MFNode & children,
                              const SFNode & child,
                              const SFNode & parent,
                              const UndoStepPtr & undoStep)
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
		undoStep -> addRedoFunction ((set1Value) & MFNode::set1Value, std::ref (children), index, group);

		children [index] = group;
	}
}

///  Add node to active layer root nodes or layer 0.
void
X3DEditor::addNodesToActiveLayer (const WorldPtr & world, const MFNode & nodes, const UndoStepPtr & undoStep)
{
	const auto & activeLayer = world -> getActiveLayer ();
	auto &       children    = activeLayer and activeLayer not_eq world -> getLayer0 ()
	                           ? activeLayer -> children ()
										: world -> getExecutionContext () -> getRootNodes ();

	undoStep -> addObjects (world -> getExecutionContext (), activeLayer);
	children .append (nodes);

	const auto removeUndoStep = std::make_shared <UndoStep> ("");

	removeNodesFromScene (X3DExecutionContextPtr (world -> getExecutionContext ()), nodes, true, removeUndoStep);
	undoStep -> addUndoFunction (&UndoStep::redo, removeUndoStep);
	removeUndoStep -> undo ();
	undoStep -> addRedoFunction (&UndoStep::undo, removeUndoStep);

	// Prototype support

	requestUpdateInstances (world, undoStep);
}

/// Add node to layers.
void
X3DEditor::addToLayers (const X3DExecutionContextPtr & executionContext, const std::vector <X3DLayerNode*> & layers, const SFNode & node, const UndoStepPtr & undoStep)
{
	bool added = false;

	for (const auto & layer : layers)
	{
		if (layer -> getExecutionContext () not_eq executionContext)
		   continue;

		if (layer -> isLayer0 ())
		   continue;

		added = true;

		undoStep -> addObjects (layer);

		pushBackIntoArray (SFNode (layer), layer -> children (), node, undoStep);
	}

	if (not added)
		pushBackIntoArray (executionContext, executionContext -> getRootNodes (), node, undoStep);

	// Prototype support

	requestUpdateInstances (executionContext, undoStep);
}

/***
 *
 *
 *
 * Snap Target handling
 *
 *
 *
 */

///  Returns bounding box of nodes in model space with tool support.
Box3d
X3DEditor::getBoundingBox (const X3DExecutionContextPtr & executionContext,
                           const MFNode & nodes)
{
	auto bbox = Box3d ();

	for (const auto & node : nodes)
	{
		for (const auto & type : basic::make_reverse_range (node -> getType ()))
		{
			switch (type)
			{
				case X3DConstants::DirectionalLight:
				{
					const auto lightNode   = X3DPtr <DirectionalLight> (node);
					const auto modelMatrix = getModelMatrix (executionContext, node);
					const auto location    = lightNode -> getMetaData <Vector3f> ("/DirectionalLight/location");
					const auto subBBox     = Box3d (Vector3d (), Vector3d (location));
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::Extrusion:
				{
					const auto extrusionNode = X3DPtr <Extrusion> (node);
					const auto modelMatrix   = getModelMatrix (executionContext, node);
					const auto subBBox       = extrusionNode -> getBBox ();
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::IndexedFaceSetTool:
				{
					const auto tool = X3DPtr <IndexedFaceSetTool> (node);
					
					if (tool -> getSelectedPoints () .empty ())
						continue; // Proceed with X3DComposedGeometryNode.
	
					auto points = std::vector <Vector3d> ();
	
					for (const auto & pair : tool -> getSelectedPoints ())
						points .emplace_back (pair .second);
	
					const auto modelMatrix = getModelMatrix (executionContext, node);
					const auto subBBox     = Box3d (points .begin (), points .end (), iterator_type ());
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::PointLight:
				{
					const auto lightNode   = X3DPtr <PointLight> (node);
					const auto modelMatrix = getModelMatrix (executionContext, node);
					const auto subBBox     = Box3d (Vector3d (), Vector3d (lightNode -> location () .getValue ()) );
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::SpotLight:
				{
					const auto lightNode   = X3DPtr <SpotLight> (node);
					const auto modelMatrix = getModelMatrix (executionContext, node);
					const auto subBBox     = Box3d (Vector3d (), Vector3d (lightNode -> location () .getValue ()) );
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::Sound:
				{
					const auto soundNode   = X3DPtr <Sound> (node);
					const auto modelMatrix = getModelMatrix (executionContext, node);
					const auto subBBox     = Box3d (Vector3d (), Vector3d (soundNode -> location () .getValue ()) );
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::X3DBoundedObject:
				{
					const auto boundedObject = X3DPtr <X3DBoundedObject> (node);
					const auto modelMatrix   = getModelMatrix (executionContext, node);
					const auto subBBox       = boundedObject -> getBBox ();
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::X3DEnvironmentalSensorNode:
				{
					const auto sensorNode  = X3DPtr <X3DEnvironmentalSensorNode> (node);
					const auto modelMatrix = getModelMatrix (executionContext, node);
					const auto subBBox     = Box3d (sensorNode -> size () .getValue (), sensorNode -> center () .getValue ());
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::X3DGeometryNode:
				{
					const auto geometryNode = X3DPtr <X3DGeometryNode> (node);
					const auto modelMatrix  = getModelMatrix (executionContext, node);
					const auto subBBox      = geometryNode -> getBBox ();
	
					bbox += subBBox * modelMatrix;
					break;
				}
				case X3DConstants::X3DViewpointNode:
				{
					const auto viewpointNode = X3DPtr <X3DViewpointNode> (node);
					const auto modelMatrix   = getModelMatrix (executionContext, node);
					const auto subBBox       = Box3d (Vector3d (), viewpointNode -> getPosition ());
	
					bbox += subBBox * modelMatrix;
					break;
				}
				default:
				{
					continue;
				}
			}
	
			break;
		}
	}

	return bbox;
}

///  Moves nodes center to @a targetPosition and @a sourceNormal aligned to @a targetNormal.  All vectors must be in world space.
Matrix4d
X3DEditor::moveNodesCenterToTarget (const X3DExecutionContextPtr & executionContext,
                                    const MFNode & nodes,
                                    const Vector3d & targetPosition,
                                    const Vector3d & targetNormal,
                                    const Vector3d & sourcePosition,
                                    const Vector3d & sourceNormal,
                                    const bool moveCenter,
                                    const UndoStepPtr & undoStep)
{
	if (nodes .empty ())
		return Matrix4d ();

	// Determine bbox of nodes in model space.

	const auto bbox = getBoundingBox (executionContext, { nodes .back () });

	// Determine absolute matrix that should be added to nodes.

	const auto nearestPoint = [&bbox, &targetNormal] ()
	{
		const auto extents = bbox .extents ();
		const auto min     = extents .first;
		const auto max     = extents .second;
		const auto center  = bbox .center ();
		auto       centers = std::vector <Vector3d> ();
		
		centers .emplace_back (center .x (), min .y (), center .z ()); // bottom
		centers .emplace_back (center .x (), max .y (), center .z ()); // top
		centers .emplace_back (min .x (), center .y (), center .z ()); // left
		centers .emplace_back (max .x (), center .y (), center .z ()); // right
		centers .emplace_back (center .x (), center .y (), min .z ()); // front
		centers .emplace_back (center .x (), center .y (), max .z ()); // back

		const auto iter = std::min_element (centers .cbegin (), centers .cend (),
		[&targetNormal, &center] (const Vector3d & lhs, const Vector3d & rhs)
		{
			return dot (normalize (lhs - center), targetNormal) < dot (normalize (rhs - center), targetNormal);
		});

		return *iter;
	};

	const auto rotation             = sourceNormal == Vector3d () ? Rotation4d () : Rotation4d (negate (sourceNormal), targetNormal);
	const auto center               = moveCenter ? bbox .center () : (sourceNormal == Vector3d () ? nearestPoint () : sourcePosition);
	const auto translation          = targetPosition - center;
	const auto scale                = Vector3d (1, 1, 1);
	const auto scaleOrientation     = Rotation4d ();
	auto       transformationMatrix = Matrix4d ();

	transformationMatrix .set (translation, rotation, scale, scaleOrientation, center);

	// Move nodes and maybe align to targetNormal.

	for (const auto & node : nodes)
	{
		try
		{
			for (const auto & type : basic::make_reverse_range (node -> getType ()))
			{
				switch (type)
				{
					case X3DConstants::DirectionalLight:
					{
						const auto lightNode   = X3DPtr <DirectionalLight> (node);
						const auto modelMatrix = getModelMatrix (executionContext, node);
						const auto location    = lightNode -> getMetaData <Vector3f> ("/DirectionalLight/location");
						const auto rotation    = Rotation4d (Vector3d (0, 0, 1), Vector3d (lightNode -> direction () .getValue ()));

						Matrix4d matrix;
						matrix .set (location, rotation);

						const auto transformedMatrix = matrix * modelMatrix * transformationMatrix * inverse (modelMatrix);

						Vector3d transformedLocation;
						Rotation4d r;

						transformedMatrix .get (transformedLocation, r);

						const auto transformedDirection = Vector3d (0, 0, 1) * r;

						undoStep -> addObjects (lightNode);
						undoStep -> addUndoFunction (&X3DNode::setMetaData <Vector3f>, lightNode, "/DirectionalLight/location", location);
						undoStep -> addRedoFunction (&X3DNode::setMetaData <Vector3f>, lightNode, "/DirectionalLight/location", transformedLocation);
						lightNode -> setMetaData <Vector3f> ("/DirectionalLight/location", transformedLocation);

						X3D::X3DEditor::setValue (lightNode, lightNode -> direction (), SFVec3f (transformedDirection), undoStep);
						break;
					}
					case X3DConstants::Extrusion:
					{
						const auto extrusionNode = X3DPtr <Extrusion> (node);
						const auto modelMatrix   = getModelMatrix (executionContext, node);
						const auto matrix        = modelMatrix * transformationMatrix * inverse (modelMatrix);
						auto       spine         = std::vector <Vector3d> (extrusionNode -> spine () .cbegin (), extrusionNode -> spine () .cend ());

						for (auto & point : spine)
							point = point * matrix;

						X3D::X3DEditor::setValue (extrusionNode, extrusionNode -> spine (), MFVec3f (spine .cbegin (), spine .cend ()), undoStep);
						break;
					}
					case X3DConstants::IndexedFaceSetTool:
					{
						const auto tool = X3DPtr <IndexedFaceSetTool> (node);
						
						if (tool -> getSelectedPoints () .empty ())
							continue; // Proceed with X3DComposedGeometryNode.

						const auto & coordNode   = tool -> getCoord ();
						const auto   modelMatrix = getModelMatrix (executionContext, node);
						const auto   matrix      = modelMatrix * transformationMatrix * inverse (modelMatrix);

						undoSetCoord (coordNode, undoStep);

						for (const auto & pair : tool -> getSelectedPoints ())
							coordNode -> set1Point (pair .first, pair .second * matrix);

						redoSetCoord (coordNode, undoStep);
						break;
					}
					case X3DConstants::PointLight:
					{
						const auto lightNode           = X3DPtr <PointLight> (node);
						const auto modelMatrix         = getModelMatrix (executionContext, node);
						const auto matrix              = modelMatrix * transformationMatrix * inverse (modelMatrix);
						const auto transformedLocation = Vector3d (lightNode -> location () .getValue ()) * matrix;

						X3D::X3DEditor::setValue (lightNode, lightNode -> location (), SFVec3f (transformedLocation), undoStep);
						break;
					}
					case X3DConstants::SpotLight:
					{
						const auto lightNode   = X3DPtr <SpotLight> (node);
						const auto modelMatrix = getModelMatrix (executionContext, node);
						const auto location    = Vector3d (lightNode -> location () .getValue ());
						const auto rotation    = Rotation4d (Vector3d (0, 0, 1), Vector3d (lightNode -> direction () .getValue ()));

						Matrix4d matrix;
						matrix .set (location, rotation);

						const auto transformedMatrix = matrix * modelMatrix * transformationMatrix * inverse (modelMatrix);

						Vector3d transformedLocation;
						Rotation4d r;

						transformedMatrix .get (transformedLocation, r);

						const auto transformedDirection = Vector3d (0, 0, 1) * r;

						X3D::X3DEditor::setValue (lightNode, lightNode -> location (), SFVec3f (transformedLocation), undoStep);
						X3D::X3DEditor::setValue (lightNode, lightNode -> direction (), SFVec3f (transformedDirection), undoStep);
						break;
					}
					case X3DConstants::Sound:
					{
						const auto soundNode   = X3DPtr <Sound> (node);
						const auto modelMatrix = getModelMatrix (executionContext, node);
						const auto location    = Vector3d (soundNode -> location () .getValue ());
						const auto rotation    = Rotation4d (Vector3d (0, 0, 1), Vector3d (soundNode -> direction () .getValue ()));

						Matrix4d matrix;
						matrix .set (location, rotation);

						const auto transformedMatrix = matrix * modelMatrix * transformationMatrix * inverse (modelMatrix);

						Vector3d transformedLocation;
						Rotation4d r;

						transformedMatrix .get (transformedLocation, r);

						const auto transformedDirection = Vector3d (0, 0, 1) * r;

						X3D::X3DEditor::setValue (soundNode, soundNode -> location (), SFVec3f (transformedLocation), undoStep);
						X3D::X3DEditor::setValue (soundNode, soundNode -> direction (), SFVec3f (transformedDirection), undoStep);
						break;
					}
					case X3DConstants::X3DTransformNode:
					{
						const auto transformNode     = X3DPtr <X3DTransformNode> (node);
						const auto center            = Vector3d (transformNode -> center () .getValue ());
						const auto modelMatrix       = getModelMatrix (executionContext, node);
						const auto matrix            = transformNode -> getMatrix ();
						const auto transformedMatrix = matrix * modelMatrix * transformationMatrix * inverse (modelMatrix);

						undoStep -> addObjects (transformNode);
						undoStep -> addUndoFunction (&X3DTransformNode::setMatrixWithCenter, transformNode, transformNode -> getMatrix (), center);
						undoStep -> addRedoFunction (&X3DTransformNode::setMatrixWithCenter, transformNode, transformedMatrix, center);

						transformNode -> setMatrixWithCenter (transformedMatrix, center);
						break;
					}
					case X3DConstants::X3DComposedGeometryNode:
					{
						const auto   geometryNode = X3DPtr <X3DComposedGeometryNode> (node);
						const auto & coordNode    = geometryNode -> getCoord ();
						const auto   modelMatrix  = getModelMatrix (executionContext, node);
						const auto   matrix       = modelMatrix * transformationMatrix * inverse (modelMatrix);

						undoSetCoord (coordNode, undoStep);

						for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
							coordNode -> set1Point (i, coordNode -> get1Point (i) * matrix);

						redoSetCoord (coordNode, undoStep);
						break;
					}
					case X3DConstants::X3DEnvironmentalSensorNode:
					{
						const auto sensorNode        = X3DPtr <X3DEnvironmentalSensorNode> (node);
						const auto modelMatrix       = getModelMatrix (executionContext, node);
						const auto matrix            = modelMatrix * transformationMatrix * inverse (modelMatrix);
						const auto transformedCenter = Vector3d (sensorNode -> center () .getValue ()) * matrix;

						X3D::X3DEditor::setValue (sensorNode, sensorNode -> center (), SFVec3f (transformedCenter), undoStep);
						break;
					}
					case X3DConstants::X3DViewpointNode:
					{
						const auto viewpointNode = X3DPtr <X3DViewpointNode> (node);
						const auto modelMatrix   = getModelMatrix (executionContext, node);

						Matrix4d matrix;
						matrix .set (viewpointNode -> getPosition (), viewpointNode -> getOrientation ());

						const auto transformedMatrix = matrix * modelMatrix * transformationMatrix * inverse (modelMatrix);

						Vector3d position;
						Rotation4d orientation;

						transformedMatrix .get (position, orientation);
					
						undoStep -> addObjects (viewpointNode);
						undoStep -> addUndoFunction (&X3DViewpointNode::resetUserOffsets,    viewpointNode);
						undoStep -> addUndoFunction (&X3DViewpointNode::setCenterOfRotation, viewpointNode, viewpointNode -> getCenterOfRotation ());
						undoStep -> addUndoFunction (&X3DViewpointNode::setOrientation,      viewpointNode, viewpointNode -> getOrientation ());
						undoStep -> addUndoFunction (&X3DViewpointNode::setPosition,         viewpointNode, viewpointNode -> getPosition ());
							
						undoStep -> addRedoFunction (&X3DViewpointNode::setPosition,         viewpointNode, position);
						undoStep -> addRedoFunction (&X3DViewpointNode::setOrientation,      viewpointNode, orientation);
						undoStep -> addRedoFunction (&X3DViewpointNode::setCenterOfRotation, viewpointNode, viewpointNode -> getCenterOfRotation ());
						undoStep -> addRedoFunction (&X3DViewpointNode::resetUserOffsets,    viewpointNode);

						viewpointNode -> setPosition (position);
						viewpointNode -> setOrientation (orientation);
						viewpointNode -> resetUserOffsets ();
						break;
					}
					default:
					{
						continue;
					}
				}
	
				break;
			}
		}
		catch (const std::exception & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	requestUpdateInstances (executionContext, undoStep);

	return transformationMatrix;
}

/***
 *
 *
 *
 * Viewpoint handling
 *
 *
 *
 */

void
X3DEditor::updateViewpoint (const X3DPtr <X3DViewpointNode> & viewpointNode,
                            const UndoStepPtr & undoStep)
{
	// Make copy, don't use references.
	const auto position         = viewpointNode -> getUserPosition ();
	const auto orientation      = viewpointNode -> getUserOrientation ();
	const auto centerOfRotation = viewpointNode -> getUserCenterOfRotation ();

	undoStep -> addObjects (viewpointNode);
	undoStep -> addUndoFunction (&X3DViewpointNode::transitionStart,     viewpointNode, viewpointNode);
	undoStep -> addUndoFunction (&X3DViewpointNode::resetUserOffsets,    viewpointNode);
	undoStep -> addUndoFunction (&X3DViewpointNode::setCenterOfRotation, viewpointNode, viewpointNode -> getCenterOfRotation ());
	undoStep -> addUndoFunction (&X3DViewpointNode::setOrientation,      viewpointNode, viewpointNode -> getOrientation ());
	undoStep -> addUndoFunction (&X3DViewpointNode::setPosition,         viewpointNode, viewpointNode -> getPosition ());
	undoStep -> addUndoFunction (&X3DViewpointNode::setAnimate,          viewpointNode, true);
	undoStep -> addUndoFunction (&SFBool::setValue, std::ref (viewpointNode -> set_bind ()), true);

	undoStep -> addRedoFunction (&SFBool::setValue, std::ref (viewpointNode -> set_bind ()), true);
	undoStep -> addRedoFunction (&X3DViewpointNode::setAnimate,          viewpointNode, true);
	undoStep -> addRedoFunction (&X3DViewpointNode::setPosition,         viewpointNode, position);
	undoStep -> addRedoFunction (&X3DViewpointNode::setOrientation,      viewpointNode, orientation);
	undoStep -> addRedoFunction (&X3DViewpointNode::setCenterOfRotation, viewpointNode, centerOfRotation);
	undoStep -> addRedoFunction (&X3DViewpointNode::resetUserOffsets,    viewpointNode);
	undoStep -> addRedoFunction (&X3DViewpointNode::transitionStart,     viewpointNode, viewpointNode);

	viewpointNode -> setPosition (position);
	viewpointNode -> setOrientation (orientation);
	viewpointNode -> setCenterOfRotation (centerOfRotation);
	viewpointNode -> resetUserOffsets ();

	// Proto support

	requestUpdateInstances (viewpointNode, undoStep);
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
X3DEditor::getParentNodes (const SFNode & child)
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
								if (not baseNode -> getPrivate ())
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
						if (not baseNode -> getPrivate ())
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
X3DEditor::getContainerField (const SFNode & parent, const SFNode & child)
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

void
X3DEditor::setExecutionContext (const MFNode & nodes, const X3DExecutionContextPtr & executionContext, const UndoStepPtr & undoStep)
{
	traverse (const_cast <MFNode &> (nodes),
   [&] (const SFNode & node)
	{
		undoStep -> addObjects (node);
		undoStep -> addUndoFunction (&X3DBaseNode::setExecutionContext, node, node -> getExecutionContext ());
		undoStep -> addRedoFunction (&X3DBaseNode::setExecutionContext, node, executionContext);

		node -> setExecutionContext (executionContext);
		return true;
	});
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
X3DEditor::transformToZero (const MFNode & children, const UndoStepPtr & undoStep)
{
	Matrix4dStack           modelViewMatrix;
	std::set <X3DBaseNode*> coords;

	transformToZero (children, modelViewMatrix, coords, undoStep);

	// Prototype support

	for (const auto & child : children)
		requestUpdateInstances (child, undoStep);
}

void
X3DEditor::transformToZero (const MFNode & children,
                            Matrix4dStack & modelViewMatrix,
                            std::set <X3DBaseNode*> & coords,
                            const UndoStepPtr & undoStep)
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
                            const UndoStepPtr & undoStep)
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
				const X3DPtr <X3DTransformNode> transform (child);

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
			case X3DConstants::Collision:
			{
				const auto & proxy = child -> getField <SFNode> ("proxy");

				if (proxy)
					transformToZero (proxy, modelViewMatrix, coords, undoStep);

				// Proceed with next step.
			}
			case X3DConstants::X3DGroupingNode:
			{
				transformToZero (child -> getField <MFNode> ("children"), modelViewMatrix, coords, undoStep);
				return;
			}
			case X3DConstants::X3DShapeNode:
			{
				const X3DPtr <X3DShapeNode>    shape (child);
				const X3DPtr <X3DGeometryNode> geometry (shape -> geometry ());

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
                            const UndoStepPtr & undoStep)
{
	try
	{
		const X3DPtr <X3DCoordinateNode> coord (geometry -> getField <SFNode> ("coord"));

		if (coord)
			transformToZero (coord, matrix, coords, undoStep);
	}
	catch (const X3DError &)
	{ }
}

void
X3DEditor::transformToZero (const X3DPtr <X3DCoordinateNode> & coord,
                            const Matrix4d & matrix,
                            std::set <X3DBaseNode*> & coords,
                            const UndoStepPtr & undoStep)
{
	if (not coords .emplace (coord) .second)
		return;

	switch (coord -> getType () .back ())
	{
		case X3DConstants::Coordinate:
		{
			const X3DPtr <Coordinate> coordinate (coord);

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());

			for (MFVec3f::reference point : coordinate -> point ())
				point = matrix .mult_vec_matrix (point .get ());

			undoStep -> addRedoFunction (&MFVec3f::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			return;
		}
		case X3DConstants::CoordinateDouble:
		{
			const X3DPtr <CoordinateDouble> coordinate (coord);

			undoStep -> addObjects (coordinate);
			undoStep -> addUndoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());

			for (MFVec3d::reference point : coordinate -> point ())
				point = matrix .mult_vec_matrix (point .get ());

			undoStep -> addRedoFunction (&MFVec3d::setValue, std::ref (coordinate -> point ()), coordinate -> point ());
			return;
		}
		case X3DConstants::GeoCoordinate:
		{
			const X3DPtr <GeoCoordinate> coordinate (coord);

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
X3DEditor::storeMatrix (const SFNode & node, const UndoStepPtr & undoStep)
{
	X3DPtr <X3DTransformNode> transform (node);

	if (transform)
	{
		undoStep -> addUndoFunction (&X3DTransformNode::setMatrix,
		                             transform,
		                             transform -> getMatrix ());

		// Prototype support

		requestUpdateInstances (node, undoStep);
	}
}

void
X3DEditor::setMatrix (const X3DPtr <X3DTransformNode> & transform, const Matrix4d & matrix, const UndoStepPtr & undoStep)
{
	undoStep -> addUndoFunction (&X3DTransformNode::setMatrix,
	                             transform,
	                             transform -> getMatrix ());

	undoStep -> addRedoFunction (&X3DTransformNode::setMatrix,
	                             transform,
	                             matrix);

	transform -> setMatrix (matrix);

	// Prototype support

	requestUpdateInstances (transform, undoStep);
}

Matrix4d
X3DEditor::getModelMatrix (const X3DExecutionContextPtr & executionContext, const SFNode & node)
{
	Matrix4d modelViewMatrix;

	auto hierarchies = find (executionContext, node, TRAVERSE_ROOT_NODES | TRAVERSE_PROTOTYPE_INSTANCES);

	if (hierarchies .empty ())
		return modelViewMatrix;

	auto & hierarchy = hierarchies .front ();

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
X3DEditor::mergePoints (const X3DPtr <IndexedFaceSet> & geometryNode, const double distance, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (geometryNode);
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> colorIndex    ()), geometryNode -> colorIndex    ());
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> texCoordIndex ()), geometryNode -> texCoordIndex ());
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> normalIndex   ()), geometryNode -> normalIndex   ());
	undoStep -> addUndoFunction (&MFInt32::setValue, std::ref (geometryNode -> coordIndex    ()), geometryNode -> coordIndex    ());

	undoSetCoord (geometryNode -> getCoord (), undoStep);

	geometryNode -> mergePoints (distance);

	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> colorIndex    ()), geometryNode -> colorIndex    ());
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> texCoordIndex ()), geometryNode -> texCoordIndex ());
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> normalIndex   ()), geometryNode -> normalIndex   ());
	undoStep -> addRedoFunction (&MFInt32::setValue, std::ref (geometryNode -> coordIndex    ()), geometryNode -> coordIndex    ());

	redoSetCoord (geometryNode -> getCoord (), undoStep);

	// Prototype support

	requestUpdateInstances (geometryNode, undoStep);
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
X3DEditor::undoSetColor (const X3DPtr <X3DColorNode> & colorNode, const UndoStepPtr & undoStep)
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

	// Prototype support

	requestUpdateInstances (colorNode, undoStep);
}

void
X3DEditor::undoSetTexCoord (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep)
{
	if (not texCoordNode)
	   return;

	switch (texCoordNode -> getType () .back ())
	{
		case X3DConstants::MultiTextureCoordinate:
		{
			const X3DPtr <MultiTextureCoordinate> node (texCoordNode);

			for (const auto & texCoordNode : node -> getTexCoord ())
				undoSetTexCoordImpl (texCoordNode, undoStep);

			break;
		}
		default:
		   undoSetTexCoordImpl (texCoordNode, undoStep);
			break;
	}

	// Prototype support

	requestUpdateInstances (texCoordNode, undoStep);
}

void
X3DEditor::undoSetTexCoordImpl (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep)
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
X3DEditor::undoSetNormal (const X3DPtr <X3DNormalNode> & normalNode, const UndoStepPtr & undoStep)
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

	// Prototype support

	requestUpdateInstances (normalNode, undoStep);
}

void
X3DEditor::undoSetCoord (const X3DPtr <X3DCoordinateNode> & coordNode, const UndoStepPtr & undoStep)
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

	// Prototype support

	requestUpdateInstances (coordNode, undoStep);
}

void
X3DEditor::redoSetColor (const X3DPtr <X3DColorNode> & colorNode, const UndoStepPtr & undoStep)
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

	// Prototype support

	requestUpdateInstances (colorNode, undoStep);
}

void
X3DEditor::redoSetTexCoord (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep)
{
	if (not texCoordNode)
	   return;

	switch (texCoordNode -> getType () .back ())
	{
		case X3DConstants::MultiTextureCoordinate:
		{
			const X3DPtr <MultiTextureCoordinate> node (texCoordNode);

			for (const auto & texCoordNode : node -> getTexCoord ())
				redoSetTexCoordImpl (texCoordNode, undoStep);

			break;
		}
		default:
		   redoSetTexCoordImpl (texCoordNode, undoStep);
			break;
	}

	// Prototype support

	requestUpdateInstances (texCoordNode, undoStep);
}

void
X3DEditor::redoSetTexCoordImpl (const X3DPtr <X3DTextureCoordinateNode> & texCoordNode, const UndoStepPtr & undoStep)
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
X3DEditor::redoSetNormal (const X3DPtr <X3DNormalNode> & normalNode, const UndoStepPtr & undoStep)
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

	// Prototype support

	requestUpdateInstances (normalNode, undoStep);
}

void
X3DEditor::redoSetCoord (const X3DPtr <X3DCoordinateNode> & coordNode, const UndoStepPtr & undoStep)
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

	// Prototype support

	requestUpdateInstances (coordNode, undoStep);
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
X3DEditor::pushBackIntoArray (const SFNode & parent, MFNode & array, const SFNode & node, const UndoStepPtr & undoStep)
{
	// Add to group

	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .emplace_back (node);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);

	// Prototype support

	requestUpdateInstances (parent, undoStep);
}

void
X3DEditor::insertIntoArray (const SFNode & parent, MFNode & array, const size_t index, const SFNode & node, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .emplace (array .begin () + index, node);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);

	// Prototype support

	requestUpdateInstances (parent, undoStep);
}

void
X3DEditor::insertIntoArray (const SFNode & parent, MFNode & array, const size_t index, const MFNode::const_iterator & first, const MFNode::const_iterator & last, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .insert (array .begin () + index, first, last);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);

	// Prototype support

	requestUpdateInstances (parent, undoStep);
}

void
X3DEditor::moveValueWithinArray (const SFNode & parent, MFNode & array, const size_t fromIndex, const size_t toIndex, const UndoStepPtr & undoStep)
{
	// Undo

	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	// Insert

	const auto fromIter = array .begin () + fromIndex;
	const auto toIter   = array .begin () + toIndex;

	array .emplace (toIter, std::move (*fromIter));

	// Erase

	if (fromIndex < toIndex)
		array .erase (array .begin () + fromIndex);
	else
		array .erase (array .begin () + (fromIndex + 1));

	// Redo

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);

	// Prototype support

	requestUpdateInstances (parent, undoStep);
}

void
X3DEditor::eraseFromArray (const SFNode & parent, MFNode & array, const size_t index, const UndoStepPtr & undoStep)
{
	undoStep -> addObjects (parent);
	undoStep -> addUndoFunction (&MFNode::setValue, std::ref (array), array);

	array .erase (array .begin () + index);

	undoStep -> addRedoFunction (&MFNode::setValue, std::ref (array), array);

	// Prototype support

	requestUpdateInstances (parent, undoStep);
}

} // X3D
} // titania

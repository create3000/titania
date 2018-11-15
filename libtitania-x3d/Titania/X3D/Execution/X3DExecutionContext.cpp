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

#include "X3DExecutionContext.h"

#include "../Bits/Cast.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Networking/Inline.h"
#include "../Execution/ImportedNode.h"
#include "../Execution/NamedNode.h"
#include "../Parser/Filter.h"
#include "../Prototype/ExternProtoDeclaration.h"
#include "../Prototype/ProtoDeclaration.h"

#include <Titania/String/to_string.h>
#include <Titania/Utility/Range.h>

#include <algorithm>
#include <random>

namespace titania {
namespace X3D {

static std::default_random_engine
random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

X3DExecutionContext::X3DExecutionContext () :
	          X3DBaseNode (),
	           namedNodes (),
	        importedNodes (),
	  importedNodesOutput (),
	           prototypes (),
	     prototypesOutput (),
	         externProtos (),
	externProtosLoadCount (),
	   externProtosOutput (),
	               routes (),
	         routesOutput (),
	            rootNodes (new MFNode ()),
	     sceneGraphOutput (),
	           bboxOutput (),
	   uninitializedNodes (),
	             realized (false)
{
	addType (X3DConstants::X3DExecutionContext);

	addChildObjects (namedNodesOutput,
	                 importedNodesOutput,
	                 prototypes,
	                 prototypesOutput,
	                 externProtos,
	                 externProtosLoadCount,
	                 externProtosOutput,
	                 routesOutput,
	                 sceneGraphOutput,
	                 bboxOutput,
	                 uninitializedNodes);

	// Root nodes must be added and removed as/from child in the node that derives from X3DExecutionContext.
	getRootNodes () .setName ("rootNodes");
	getRootNodes () .addCloneCount (1);

	sceneGraphOutput .setName ("sceneGraphOutput");
	bboxOutput       .setName ("bboxOutput");
}

void
X3DExecutionContext::setExecutionContext (X3DExecutionContext* const value)
{
	const bool sameBrowser = value -> getBrowser () == getBrowser ();

	X3DBaseNode::setExecutionContext (value);

	if (sameBrowser)
		return;

	const MFNode nodes (getParents () .cbegin (), getParents () .cend ());

	for (const auto & node : nodes)
	{
		if (not node)
			continue;

		if (node == this)
			continue;

		if (node -> getExecutionContext () not_eq this)
			continue;

		// Reset executionContext to set browser.
		node -> setExecutionContext (this);
	}

	for (const auto & uninitializedNode : uninitializedNodes)
		uninitializedNode -> setExecutionContext (this);
}

void
X3DExecutionContext::initialize ()
{
	sceneGraphOutput .addInterest (&X3DExecutionContext::set_sceneGraph, this);
	bboxOutput       .addInterest (&X3DExecutionContext::set_bbox, this);

	uninitializedNodes .setTainted (true); // !!! Prevent generating events when protos add nodes.

	if (not isType ({ X3DConstants::ProtoDeclaration }))
		realize ();
}

///  throws Error <DISPOSED>
void
X3DExecutionContext::realize ()
{
	try
	{
		realized = true;

		getBrowser () -> prepareEvents () .removeInterest (&X3DExecutionContext::realize, this);

		requestImmediateLoadOfExternProtos ();

		ContextLock lock (getBrowser ());

		while (not uninitializedNodes .empty ())
		{
			for (const auto & uninitializedNode : MFNode (std::move (uninitializedNodes)))
				uninitializedNode -> setup ();
		}
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::addUninitializedNode (X3DBaseNode* const uninitializedNode)
{
	uninitializedNodes .emplace_back (uninitializedNode);

	if (getRealized ())
	{
		getBrowser () -> prepareEvents () .addInterest (&X3DExecutionContext::realize, this);
		getBrowser () -> addEvent ();
	}
}

// Component/Profile handling

bool
X3DExecutionContext::hasComponent (const ComponentType & component) const
{
	if (getProfile () or not getComponents () .empty ())
	{
		if (getProfile ())
		{
			if (getProfile () -> getComponents () .count (component))
				return true;
		}

		if (getComponents () .count (component))
			return true;

		return false;;
	}

	return true;
}

// Node handling

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
SFNode
X3DExecutionContext::createNode (const std::string & typeName)
{
	try
	{
		const X3DBaseNode* const declaration = getBrowser () -> getSupportedNode (typeName);
	
		//if (not hasComponent (declaration -> getComponent ()))
		//throw Error <INVALID_NAME> ("Node type '" + typeName + "' not supported by profile or component specification.");
	
		SFNode node (declaration -> create (this));
	
		if (getRealized ())
		{
			ContextLock lock (getBrowser ());
	
			node -> setup ();
		}
		//else
		//	addUninitializedNode (node);
	
		return node;
	}
	catch (const Error <NOT_SUPPORTED> & error)
	{
		throw Error <INVALID_NAME> (error .what ());
	}
}

///  throws Error <INVALID_NAME>, Error <INVALID_X3D>, Error <INVALID_FIELD>, Error <INVALID_ACCESS_TYPE>, Error <URL_UNAVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DPrototypeInstancePtr
X3DExecutionContext::createProto (const std::string & typeName)
{
	X3DPrototypeInstancePtr node (findProtoDeclaration (typeName, AvailableType { }) -> createInstance (this));

	if (getRealized ())
	{
		ContextLock lock (getBrowser ());

		node -> setup ();
	}
	//else
	//	addUninitializedNode (node);

	return node;
}

// Named node handling

// DONE: A node may be part of more than one run-time name scope. A node shall be removed from a name scope when it is
// removed from the scene graph. See: http://www.web3d.org/files/specifications/19775-1/V3.3/Part01/concepts.html#Runtimenamescope

///  throws Error <NODE_IN_USE>, Error <IMPORTED_NODE>, Error <INVALID_NODE>, Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::addNamedNode (const std::string & name, const SFNode & node)
{
	if (namedNodes .count (name))
		throw Error <NODE_IN_USE> ("Couldn't add named node: node name '" + name + "' is already in use.");

	updateNamedNode (name, node);
}

///  throws Error <IMPORTED_NODE>, Error <INVALID_NODE>, Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::updateNamedNode (const std::string & name, const SFNode & node)
{
	if (not node)
		throw Error <INVALID_NODE> ("Couldn't update named node: node IS NULL.");

	if (node -> getExecutionContext () not_eq this)
		throw Error <IMPORTED_NODE> ("Couldn't update named node: the node does not belong to this execution context.");

	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't update named node: node name is empty.");

	// Remove named node.

	removeNamedNode (node -> getName ());
	removeNamedNode (name);

	// Update named node.

	node -> setName (name);

	auto & namedNode = namedNodes .emplace (name, new NamedNode (this, node)) .first -> second;

	namedNode .setTainted (true);
	namedNode .addParent (this);

	if (getRealized ())
		namedNode -> setup ();
	else
		addUninitializedNode (namedNode);

	namedNodesOutput = getCurrentTime ();
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::removeNamedNode (const std::string & name)
{
	const auto iter = namedNodes .find (name);

	if (iter == namedNodes .end ())
		return;

	try
	{
		const auto & namedNode = iter -> second;

		namedNode -> getLocalNode () -> setName ("");
	}
	catch (const X3DError &)
	{ }

	namedNodes .erase (iter);

	namedNodesOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
SFNode
X3DExecutionContext::getNamedNode (const std::string & name) const
{
	const auto namedNode = namedNodes .find (name);

	if (namedNode not_eq namedNodes .end ())
		return namedNode -> second -> getLocalNode ();

	throw Error <INVALID_NAME> ("Named node '" + name + "' not found.");
}

/***
 *  Returns a name that is unique in this execution contentext.
 *  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
 */
std::string
X3DExecutionContext::getUniqueName (std::string name) const
{
	name = RemoveTrailingNumber (name);
 	
	auto   uniqueName = name;
	size_t i          = 0;

	while (namedNodes .count (uniqueName) or uniqueName .empty ())
	{
		i = std::max <size_t> (64, i);

		const auto                             min = i;
		std::uniform_int_distribution <size_t> random (min, i <<= 1);

		uniqueName  = name;
		uniqueName += '_';
		uniqueName += basic::to_string (random (random_engine), std::locale::classic ());
	}

	return uniqueName;
}

/***
 *  Returns a name that is both unique in this execution context and in @a executionContext, if the name exits in this context otherwise return @a name.
 */
std::string
X3DExecutionContext::getVeryUniqueName (X3DExecutionContext* const executionContext, std::string name) const
{
	if (not name .empty () and not namedNodes .count (name))
		return name;

	name = RemoveTrailingNumber (name);

	auto   uniqueName = name;
	size_t i          = 0;

	while (namedNodes .count (uniqueName) or executionContext -> namedNodes .count (uniqueName) or uniqueName .empty ())
	{
		i = std::max <size_t> (64, i);

		const auto                             min = i;
		std::uniform_int_distribution <size_t> random (min, i <<= 1);

		uniqueName  = name;
		uniqueName += '_';
		uniqueName += basic::to_string (random (random_engine), std::locale::classic ());
	}

	return uniqueName;
}

// Imported nodes handling

///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
bool
X3DExecutionContext::isImportedNode (const SFNode & node) const
{
	if (not node)
		throw Error <INVALID_NODE> ("Node is NULL.");

	const auto count = std::count_if (importedNodes .cbegin (),
	                                  importedNodes .cend (),
	                                  [&] (const ImportedNodeIndex::value_type & pair)
	{
		try
		{
			const auto & importedNode = pair .second;
			
			if (importedNode -> getExportedNode () == node)
				return true;
		}
		catch (const X3DError &)
		{ }

		return false;
	});

	return count;
}

///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <NODE_IN_USE>, Error <URL_UNAVAILABLE> Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const ImportedNodePtr &
X3DExecutionContext::addImportedNode (const X3DPtr <Inline> & inlineNode, const std::string & exportedName, std::string importedName)
{
	if (importedNodes .count (importedName))
		throw Error <NODE_IN_USE> ("Couldn't add imported node: imported name '" + importedName + "' already in use.");

	return updateImportedNode (inlineNode, exportedName, importedName);
}

///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <URL_UNAVAILABLE>, Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const ImportedNodePtr &
X3DExecutionContext::updateImportedNode (const X3DPtr <Inline> & inlineNode, const std::string & exportedName, std::string importedName)
{
	if (not inlineNode)
		throw Error <INVALID_NODE> ("Couldn't update imported node: Inline node is NULL.");

	// We do not throw Error <IMPORTED_NODE> as X3DPrototypeInctances can be of type Inline.std::find_if

	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Couldn't update imported node: exported node name is empty.");

	if (importedName .empty ())
		importedName = exportedName;

	const auto iter = std::find_if (importedNodes .cbegin (),
	                                importedNodes .cend (),
	                                [&] (const ImportedNodeIndex::value_type & pair)
	{
		const auto & importedNode = pair .second;
		
		if (importedNode -> getInlineNode () == inlineNode and importedNode -> getExportedName () == exportedName)
			return true;

		return false;
	});

	if (iter not_eq importedNodes .end ())
	{
		const auto importedNode = iter -> second;
		
		importedNodes .erase (iter);

		const auto & renamedImportedNode = importedNodes .emplace (importedName, importedNode) .first -> second;

		renamedImportedNode -> setImportedName (importedName);

		importedNodesOutput = getCurrentTime ();

		return renamedImportedNode;
	}

	// Remove imported node!!!

	removeImportedNode (importedName);

	// Update imported node.

	auto & importedNode = importedNodes .emplace (importedName, new ImportedNode (this, inlineNode, exportedName, importedName)) .first -> second;

	importedNode .setTainted (true);
	importedNode .addParent (this);

	if (getRealized ())
		importedNode -> setup ();
	else
		addUninitializedNode (importedNode);

	importedNodesOutput = getCurrentTime ();

	return importedNode;
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::removeImportedNode (const std::string & importedName)
{
	importedNodes .erase (importedName);

	importedNodesOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
SFNode
X3DExecutionContext::getImportedNode (const std::string & importedName) const
{
	const auto iter = importedNodes .find (importedName);

	if (iter not_eq importedNodes .end ())
		return iter -> second -> getExportedNode ();

	throw Error <INVALID_NAME> ("Imported node '" + importedName + "' not found.");
}

/***
 *  Returns a name that is both unique in this context and in @a executionContext, if the name exits in this context otherwise return @a importedName.
 */
std::string
X3DExecutionContext::getVeryUniqueImportedName (const X3DExecutionContext* const executionContext, std::string importedName) const
{
	if (not importedName .empty () and not importedNodes .count (importedName))
		return importedName;

	importedName = RemoveTrailingNumber (importedName);

	auto   uniqueName = importedName;
	size_t i       = 0;

	while (importedNodes .count (uniqueName) or executionContext -> importedNodes .count (uniqueName) or uniqueName .empty ())
	{
		i = std::max <size_t> (64, i);

		const auto                             min = i;
		std::uniform_int_distribution <size_t> random (min, i <<= 1);

		uniqueName  = importedName;
		uniqueName += '_';
		uniqueName += basic::to_string (random (random_engine), std::locale::classic ());
	}

	return uniqueName;
}

///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
bool
X3DExecutionContext::isLocalNode (const SFNode & node) const
{
	if (not node)
		throw Error <INVALID_NODE> ("Node is NULL.");

	if (node -> getExecutionContext () == this)
		return true;

	if (isImportedNode (node))
		return true;

	return false;
}

///  Returns either a X3DNode with name @a name or a node of type ImportedNode.
///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
SFNode
X3DExecutionContext::getLocalNode (const std::string & name) const
{
	try
	{
		return getNamedNode (name);
	}
	catch (const X3DError &)
	{
		const auto iter = importedNodes .find (name);
	
		if (iter not_eq importedNodes .end ())
			return SFNode (iter -> second);

		throw Error <INVALID_NAME> ("Unknown named or imported node '" + name + "'.");
	}
}

///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const std::string &
X3DExecutionContext::getLocalName (const SFNode & node) const
{
	if (not node)
		throw Error <INVALID_NODE> ("Couldn't get local name: node is NULL.");

	if (node -> getExecutionContext () == this)
		return node -> getName ();

	const auto iter = std::find_if (importedNodes .cbegin (),
	                                importedNodes .cend (),
	                                [&] (const ImportedNodeIndex::value_type & pair)
	{
		try
		{
			const auto & importedNode = pair .second;
			
			if (importedNode -> getExportedNode () == node)
				return true;
		}
		catch (const X3DError &)
		{ }

		return false;
	});

	if (iter == importedNodes .end ())
		throw Error <INVALID_NODE> ("Couldn't get local name: node is shared.");

	return iter -> first;
}

//	Proto declaration handling

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
ProtoDeclarationPtr
X3DExecutionContext::createProtoDeclaration (const std::string & name, const FieldDefinitionArray & interfaceDeclarations)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't create proto declaration: proto name is empty.");

	const ProtoDeclarationPtr prototype (new ProtoDeclaration (this));

	prototype -> setName (name);

	for (const auto & field : interfaceDeclarations)
	{
		prototype -> addUserDefinedField (field -> getAccessType (),
		                                  field -> getName (),
		                                  field);
	}

	if (getRealized ())
		prototype -> setup ();
	else
		addUninitializedNode (prototype);

	return prototype;
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::addProtoDeclaration (const std::string & name, const ProtoDeclarationPtr & prototype)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't add proto declaration: proto name is empty.");

	if (hasProtoDeclaration (name))
		throw Error <INVALID_NAME> ("Couldn't add proto declaration: proto '" + name + "' is already in use.");

	prototype -> setName (name);
	prototypes .emplace_back (prototype);

	prototypesOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::updateProtoDeclaration (const std::string & name, const ProtoDeclarationPtr & prototype)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't update proto declaration: proto name is empty.");

	if (hasProtoDeclaration (name))
		throw Error <INVALID_NAME> ("Couldn't update proto declaration: proto '" + name + "' is already in use.");

	if (hasProtoDeclaration (prototype -> getName ()))
	{
		if (getProtoDeclaration (prototype -> getName ()) == prototype)
			prototype -> setName (name);
		else
			addProtoDeclaration (name, prototype);
	}
	else
		addProtoDeclaration (name, prototype);

	prototypesOutput = getCurrentTime ();
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::removeProtoDeclaration (const std::string & name)
{
	const auto iter = std::remove_if (prototypes .begin (), prototypes .end (),
	[&name] (const ProtoDeclarationPtr & prototype)
	{
		return prototype -> getName () == name;
	});

	prototypes .erase (iter, prototypes .end ());

	prototypesOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const ProtoDeclarationPtr &
X3DExecutionContext::getProtoDeclaration (const std::string & name) const
{
	const auto iter = std::find_if (prototypes .cbegin (), prototypes .cend (),
	[&name] (const ProtoDeclarationPtr & prototype)
	{
		return prototype -> getName () == name;
	});

	if (iter == prototypes .end ())
		throw Error <INVALID_NAME> ("Proto declaration '" + name + "' not found.");

	return *iter;
}

bool
X3DExecutionContext::hasProtoDeclaration (const std::string & name) const
{
	return std::count_if (prototypes .cbegin (), prototypes .cend (),
	[&name] (const ProtoDeclarationPtr & prototype)
	{
		return prototype -> getName () == name;
	});
}

///  throws Error <DISPOSED>
std::string
X3DExecutionContext::getUniqueProtoName (std::string name) const
{
	if (not name .empty () and not hasProtoDeclaration (name))
		return name;

	name = RemoveTrailingNumber (name);

	auto   uniqueName = name;
	size_t i          = 0;

	if (uniqueName .empty ())
		uniqueName = "Prototype";

	while (hasProtoDeclaration (uniqueName))
	{
		i = std::max <size_t> (64, i);

		const auto                             min = i;
		std::uniform_int_distribution <size_t> random (min, i <<= 1);

		uniqueName  = name;
		uniqueName += basic::to_string (random (random_engine), std::locale::classic ());
	}

	return uniqueName;
}

//	ExternProto declaration handling

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
ExternProtoDeclarationPtr
X3DExecutionContext::createExternProtoDeclaration (const std::string & name, const FieldDefinitionArray & externInterfaceDeclarations, const MFString & URLList)
{
	const ExternProtoDeclarationPtr externProto (new ExternProtoDeclaration (this));

	externProto -> setName (name);

	for (const auto & field : externInterfaceDeclarations)
	{
		externProto -> addUserDefinedField (field -> getAccessType (),
		                                    field -> getName (),
		                                    field);
	}

	externProto -> url () = URLList;

	if (getRealized ())
		externProto -> setup ();
	else
		addUninitializedNode (externProto);

	return externProto;
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::addExternProtoDeclaration (const std::string & name, const ExternProtoDeclarationPtr & externProto)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't add extern proto declaration: extern proto name is empty.");

	if (hasExternProtoDeclaration (name))
		throw Error <INVALID_NAME> ("Couldn't add extern extern proto declaration: extern proto '" + name + "' is already in use.");

	externProto -> setName (name);
	externProtos .emplace_back (externProto);

	externProtosOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::updateExternProtoDeclaration (const std::string & name, const ExternProtoDeclarationPtr & externProto)
{
	if (name .empty ())
		throw Error <INVALID_NAME> ("Couldn't update extern proto declaration: extern proto name is empty.");

	if (hasExternProtoDeclaration (name))
		throw Error <INVALID_NAME> ("Couldn't update extern extern proto declaration: extern proto '" + name + "' is already in use.");

	if (hasExternProtoDeclaration (externProto -> getName ()))
	{
		if (getExternProtoDeclaration (externProto -> getName ()) == externProto)
			externProto -> setName (name);
		else
			addExternProtoDeclaration (name, externProto);
	}
	else
		addExternProtoDeclaration (name, externProto);

	externProtosOutput = getCurrentTime ();
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::removeExternProtoDeclaration (const std::string & name)
{
	const auto iter = std::remove_if (externProtos .begin (), externProtos .end (),
	[&name] (const ExternProtoDeclarationPtr & externProto)
	{
		return externProto -> getName () == name;
	});

	externProtos .erase (iter, externProtos .end ());

	externProtosOutput = getCurrentTime ();
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const ExternProtoDeclarationPtr &
X3DExecutionContext::getExternProtoDeclaration (const std::string & name) const
{
	const auto iter = std::find_if (externProtos .cbegin (), externProtos .cend (),
	[&name] (const ExternProtoDeclarationPtr & externProto)
	{
		return externProto -> getName () == name;
	});

	if (iter == externProtos .end ())
		throw Error <INVALID_NAME> ("Extern proto declaration '" + name + "' not found.");

	return *iter;
}

bool
X3DExecutionContext::hasExternProtoDeclaration (const std::string & name) const
{
	return std::count_if (externProtos .cbegin (), externProtos .cend (),
	[&name] (const ExternProtoDeclarationPtr & externProto)
	{
		return externProto -> getName () == name;
	});
}

///  throws Error <DISPOSED>
std::string
X3DExecutionContext::getUniqueExternProtoName (std::string name) const
{
	if (not name .empty () and not hasExternProtoDeclaration (name))
		return name;

	name = RemoveTrailingNumber (name);

	auto   uniqueName = name;
	size_t i          = 0;

	if (uniqueName .empty ())
		uniqueName = "Prototype";

	while (hasExternProtoDeclaration (uniqueName))
	{
		i = std::max <size_t> (64, i);

		const auto                             min = i;
		std::uniform_int_distribution <size_t> random (min, i <<= 1);

		uniqueName  = name;
		uniqueName += basic::to_string (random (random_engine), std::locale::classic ());
	}

	return uniqueName;
}

void
X3DExecutionContext::requestImmediateLoadOfExternProtos ()
{
	// Parallel load all extern protos, then sync.
	requestAsyncLoadOfExternProtos ();

	for (const auto&  externProto : getExternProtoDeclarations ())
	{
		if (externProto -> getInstances () .empty ())
		   continue;

		externProto -> requestImmediateLoad ();
	}

	for (const auto & proto : getProtoDeclarations ())
		proto -> requestImmediateLoadOfExternProtos ();
}

void
X3DExecutionContext::requestAsyncLoadOfExternProtos ()
{
	externProtosLoadCount .setTainted (false);
	externProtosLoadCount .addEvent ();

	for (const auto & externProto : getExternProtoDeclarations ())
	{
		if (externProto -> getInstances () .empty ())
		   continue;

		externProto -> requestAsyncLoad ();
	}

	for (const auto & proto : getProtoDeclarations ())
	{
	   proto -> requestAsyncLoadOfExternProtos ();
	}
}

void
X3DExecutionContext::addExternProtoLoadCount (const ExternProtoDeclaration* const externProto)
{
	loadingExternProtos .emplace (externProto);
		
	externProtosLoadCount = loadingExternProtos .size ();
}

void
X3DExecutionContext::removeExternProtoLoadCount (const ExternProtoDeclaration* const externProto)
{
	if (not loadingExternProtos .erase (externProto))
		return;
 
	externProtosLoadCount = loadingExternProtos .size ();
}

// ProtoObject handling

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DProtoDeclarationNode*
X3DExecutionContext::findProtoDeclaration (const std::string & name, const AvailableType & available) const
{
	try
	{
		return getProtoDeclaration (name) .getValue ();
	}
	catch (const Error <INVALID_NAME> &)
	{
		try
		{
			return getExternProtoDeclaration (name) .getValue ();
		}
		catch (const Error <INVALID_NAME> &)
		{
			if (not isType ({ X3DConstants::X3DScene }))
				return getExecutionContext () -> findProtoDeclaration (name, available);

			throw Error <INVALID_NAME> ("Unknown proto or externproto type '" + name + "'.");
		}
	}
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DProtoDeclarationNode*
X3DExecutionContext::findProtoDeclaration (const std::string & name) const
{
	const auto protoNodes = findProtoDeclarations ();
	const auto iter       = protoNodes .find (name);

	if (iter not_eq protoNodes .end ())
		return iter -> second;

	throw Error <INVALID_NAME> ("Unknown proto object '" + name + "'.");
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
std::map <std::string, X3DProtoDeclarationNode*>
X3DExecutionContext::findProtoDeclarations () const
{
	const X3DExecutionContext* executionContext = this;

	// Find all available prototypes

	std::map <std::string, X3DProtoDeclarationNode*> prototypes;
	std::string                                      current;

	for (; ;)
	{
		// Skip all prototypes that are below a current prototype.

		bool skip = true;

		for (const auto & prototype : basic::make_reverse_range (executionContext -> getProtoDeclarations ()))
		{
			if (skip and not current .empty ())
			{
				if (current == prototype -> getName ())
					skip = false;

				continue;
			}

			prototypes .emplace (prototype -> getName (), prototype);
		}

		for (const auto & prototype : executionContext -> getExternProtoDeclarations ())
			prototypes .emplace (prototype -> getName (), prototype);

		if (executionContext -> isType ({ X3DConstants::X3DScene }))
			break;

		if (executionContext -> isType ({ X3DConstants::ProtoDeclaration }))
			current = executionContext -> getName ();
		else
			current .clear ();

		current = executionContext -> isType ({ X3DConstants::ProtoDeclaration }) ? executionContext -> getName () : "";

		executionContext = executionContext -> getExecutionContext ();
	}

	return prototypes;
}

// Root node handling

void
X3DExecutionContext::set_sceneGraph ()
{
	if (getExecutionContext () == this)
		return;

	const_cast <SFTime &> (getExecutionContext () -> sceneGraph_changed ()) = getCurrentTime ();

	const_cast <SFTime &> (bbox_changed ()) = getCurrentTime ();
}

void
X3DExecutionContext::set_bbox ()
{
	if (getExecutionContext () == this)
		return;

	const_cast <SFTime &> (getExecutionContext () -> bbox_changed ()) = getCurrentTime ();
}

//	Dynamic route handling

///  throws Error <INVALID_NODE>, Error <INVALID_FIELD>, Error <IMPORTED_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const RoutePtr &
X3DExecutionContext::addRoute (const SFNode & sourceNode,      const std::string & sourceFieldId,
                               const SFNode & destinationNode, const std::string & destinationFieldId)
{
	if (not sourceNode)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: source node is NULL.");

	if (not destinationNode)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: destination node is NULL.");

	// Imported nodes handling.

	X3DPtr <ImportedNode> importedSourceNode (sourceNode);
	X3DPtr <ImportedNode> importedDestinationNode (destinationNode);

	try
	{
		// If sourceNode is shared node try to find the corresponding ImportedNode.
		if (sourceNode -> getExecutionContext () != this)
			importedSourceNode = getLocalNode (getLocalName (sourceNode));
	}
	catch (const X3DError &)
	{
		// Source node is shared but not imported.
	}

	try
	{
		// If destinationNode is shared node try to find the corresponding ImportedNode.
		if (destinationNode -> getExecutionContext () != this)
			importedDestinationNode = getLocalNode (getLocalName (destinationNode));
	}
	catch (const X3DError &)
	{
		// Destination node is shared but not imported.
	}

	if (importedSourceNode)
		importedSourceNode -> addRoute (importedSourceNode, sourceFieldId, destinationNode, destinationFieldId);

	if (importedDestinationNode)
		importedDestinationNode -> addRoute (sourceNode, sourceFieldId, importedDestinationNode, destinationFieldId);

	// If either sourceNode or destinationNode is an ImportedNode return here without value.
	if (importedSourceNode == sourceNode or importedDestinationNode == destinationNode)
		throw Error <IMPORTED_NODE> ("Bad ROUTE specification: either sourceNode or destinationNode are imported nodes.");

	return addSimpleRoute (sourceNode, sourceFieldId, destinationNode, destinationFieldId);
}

///  throws Error <INVALID_NODE>, Error <INVALID_FIELD>, Error <IMPORTED_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const RoutePtr &
X3DExecutionContext::addSimpleRoute (const SFNode & sourceNode,      const std::string & sourceFieldId,
                                     const SFNode & destinationNode, const std::string & destinationFieldId)
{
	// Create route and return.

	X3DFieldDefinition* sourceField = nullptr;

	try
	{
		sourceField = sourceNode -> getField (sourceFieldId);

		if (not sourceField -> isOutput ())
			throw Error <INVALID_FIELD> ("Bad ROUTE specification: Field named '" + sourceField -> getName () + "' in node named '" + sourceNode -> getName () + "' of type " + sourceNode -> getTypeName () + " is not an output field.");
	}
	catch (const Error <INVALID_NAME> &)
	{
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventOut '" + sourceFieldId + "' in node named '" + sourceNode -> getName () + "' of type " + sourceNode -> getTypeName () + ".");
	}

	X3DFieldDefinition* destinationField = nullptr;

	try
	{
		destinationField = destinationNode -> getField (destinationFieldId);

		if (not destinationField -> isInput ())
			throw Error <INVALID_FIELD> ("Bad ROUTE specification: Field named '" + destinationField -> getName () + "' in node named '" + destinationNode -> getName () + "' of type " + destinationNode -> getTypeName () + " is not an input field.");
	}
	catch (const Error <INVALID_NAME> &)
	{
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventIn '" + destinationFieldId + "' in node named '" + destinationNode -> getName () + "' of type " + destinationNode -> getTypeName () + ".");
	}

	if (sourceField -> getType () not_eq destinationField -> getType ())
		throw Error <INVALID_FIELD> ("ROUTE types " + sourceField -> getTypeName () + " and " + destinationField -> getTypeName () + " do not match.");

	const auto routeKey = std::make_pair (sourceField, destinationField);

	try
	{
		// Silently return if route already exists.

		return routes .rfind (routeKey);
	}
	catch (const std::out_of_range &)
	{
		// Add route.

		const auto & route = addRoute (new Route (this, sourceNode, routeKey .first, destinationNode, routeKey .second));

		if (getRealized ())
			route -> setup ();
		else
			addUninitializedNode (route);

		return route;
	}
}

///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
const RoutePtr &
X3DExecutionContext::addRoute (Route* const value)
{
	if (not value)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route is NULL in deleteRoute.");

	if (value -> getExecutionContext () not_eq this)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route does not belong to this execution context.");

	routes .push_back (value -> getKey (), RoutePtr (value));

	auto & route = routes .back ();

	route .setTainted (true);
	route .addParent (this);

	routesOutput = getCurrentTime ();

	return route;
}

///  throws Error <INVALID_NODE>, Error <INVALID_FIELD>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::deleteRoute (const SFNode & sourceNode,      const std::string & sourceFieldId,
                                  const SFNode & destinationNode, const std::string & destinationFieldId)
{
	try
	{
		if (not sourceNode)
			return;

		if (not destinationNode)
			return;

		const auto routeKey = RouteId (sourceNode -> getField (sourceFieldId), destinationNode -> getField (destinationFieldId));

		routes .rfind (routeKey) -> disconnect ();
		routes .erase (routeKey);

		routesOutput = getCurrentTime ();
	}
	catch (const std::exception & error)
	{
		// Silently return if route not exists.
	}
}

///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::deleteRoute (Route* const route)
{
	if (not route)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route is NULL in deleteRoute.");

	if (route -> getExecutionContext () not_eq this)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route does not belong to this execution context.");

	const auto routeKey = route -> getKey ();

	route -> disconnect ();
	routes .erase (routeKey);
}

///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
X3DExecutionContext::changeViewpoint (const std::string & name)
{
	try
	{
		const auto namedNode = getNamedNode (name);
		const auto viewpoint = x3d_cast <X3DViewpointNode*> (namedNode);

		if (not viewpoint)
			throw Error <INVALID_NAME> ("Warning: Node named '" + name + "' is not a viewpoint node.");

		viewpoint -> setAnimate (true);

		if (viewpoint -> isBound ())
			viewpoint -> transitionStart (viewpoint);

		else
			viewpoint -> set_bind () = true;
	}
	catch (const X3DError & error)
	{
		if (not isType ({ X3DConstants::X3DScene }))
			getExecutionContext () -> changeViewpoint (name);
		else
			throw;
	}
}

// Import handling

///	 importScene
///
///	 throws:
///      Error <INVALID_NAME>,
///	   Error <NOT_SUPPORTED>,
///	   Error <INVALID_OPERATION_TIMING>,
///	   Error <DISPOSED>
MFNode
X3DExecutionContext::importScene (X3DExecutionContext* const executionContext)
{
	ContextLock lock (getBrowser ());

	updateNamedNodes (executionContext);

	importNodes (executionContext);
	importNamedNodes (executionContext);
	importExternProtos (executionContext, CLONE);
	importProtos (executionContext, CLONE);

	updateImportedNodes (executionContext);
	copyImportedNodes (executionContext, CLONE);
	importRoutes (executionContext);

	realize ();

	return executionContext -> getRootNodes ();
}

void
X3DExecutionContext::updateNamedNodes (X3DExecutionContext* const executionContext) const
{
	// Rename nodes that the name is unique in both contexts.

	for (const auto & pair : NamedNodeIndex (executionContext -> getNamedNodes ()))
	{
		try
		{
			const auto & name       = pair .first;
			const auto & namedNode  = pair .second;
			const auto   uniqueName = getVeryUniqueName (executionContext, name);
	
			executionContext -> updateNamedNode (uniqueName, namedNode -> getLocalNode ());
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DExecutionContext::updateImportedNodes (X3DExecutionContext* const executionContext) const
{
	for (const auto & pair : ImportedNodeIndex (executionContext -> getImportedNodes ()))
	{
		try
		{
			const auto & importedNode       = pair .second;
			const auto   uniqueImportedName = getVeryUniqueImportedName (executionContext, importedNode -> getImportedName ()); // TODO: getVeryUniqueImportedName
	
			executionContext -> updateImportedNode (importedNode -> getInlineNode (), importedNode -> getExportedName (), uniqueImportedName);
	
			if (uniqueImportedName not_eq importedNode -> getImportedName ())
				executionContext -> removeImportedNode (importedNode -> getImportedName ());
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DExecutionContext::importNodes (const X3DExecutionContext* const executionContext)
{
	const MFNode nodes (executionContext -> getParents () .cbegin (), executionContext -> getParents () .cend ());

	for (const auto & node : nodes)
	{
		if (not node)
			continue;

		if (node == executionContext)
			continue;

		if (node -> getExecutionContext () not_eq executionContext)
			continue;

		node -> setExecutionContext (this);
	}
}

void
X3DExecutionContext::importNamedNodes (const X3DExecutionContext* const executionContext)
{
	for (const auto & node : executionContext -> getNamedNodes ())
	{
		try
		{
			updateNamedNode (node .first, node .second -> getLocalNode ());
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DExecutionContext::importRoutes (X3DExecutionContext* const executionContext)
{
	for (const auto & route : executionContext -> routes)
	{
		try
		{
			addRoute (route);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}

	executionContext -> routes .clear ();
}

// X3DProtoInstance import handling

void
X3DExecutionContext::importExternProtos (const X3DExecutionContext* const executionContext, const CopyType type)
{
	for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
	{
		try
		{
			externProto -> copy (this, type);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DExecutionContext::importProtos (const X3DExecutionContext* const executionContext, const CopyType type)
{
	for (const auto & proto : executionContext -> getProtoDeclarations ())
	{
		try
		{
			proto -> copy (this, type);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DExecutionContext::copyRootNodes (const X3DExecutionContext* const executionContext, const CopyType type)
{
	for (const auto & rootNode : executionContext -> getRootNodes ())
	{
		if (rootNode)
			getRootNodes () .emplace_back (rootNode -> copy (this, type));

		else
			getRootNodes () .emplace_back ();
	}
}

void
X3DExecutionContext::copyImportedNodes (const X3DExecutionContext* const executionContext, const CopyType type)
{
	for (const auto & importedNode : executionContext -> getImportedNodes ())
	{
		try
		{
			importedNode .second -> copy (this, type);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DExecutionContext::copyRoutes (const X3DExecutionContext* const executionContext, const CopyType type)
{
	for (const auto & route : executionContext -> getRoutes ())
	{
		try
		{
			route -> copy (this, type);
		}
		catch (const Error <DISPOSED> & error)
		{ }
	}
}

void
X3DExecutionContext::toStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	Generator::PushExecutionContext (ostream, this);
	Generator::EnterScope (ostream);
	Generator::ImportedNodes (ostream, getImportedNodes ());

	for (const auto & externProto : getExternProtoDeclarations ())
	{
		ostream
			<< externProto
			<< Generator::TidyBreak
			<< Generator::TidyBreak;
	}

	for (const auto & proto : getProtoDeclarations ())
	{
		ostream
			<< proto
			<< Generator::TidyBreak
			<< Generator::TidyBreak;
	}

	if (not getRootNodes () .empty ())
	{
		for (const auto & rootNode : std::make_pair (getRootNodes () .cbegin (), getRootNodes () .cend () - 1))
		{
			ostream
				<< Generator::Indent
				<< rootNode
				<< Generator::TidyBreak
				<< Generator::TidyBreak;
		}

		ostream
			<< Generator::Indent
			<< getRootNodes () .back ()
			<< Generator::Break;
	}

	if (not getImportedNodes () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & importedNode : getImportedNodes ())
		{
			try
			{
				ostream
					<< importedNode .second
					<< Generator::Break;
			}
			catch (const X3DError &)
			{ }
		}
	}

	if (not getRoutes () .empty ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & route : getRoutes ())
		{
			try
			{
				ostream
					<< route
					<< Generator::Break;
			}
			catch (const X3DError & error)
			{ }
		}
	}

	if (not getInnerComments () .empty () and not isType ({ X3DConstants::X3DScene }))
	{
		ostream << Generator::TidyBreak;

		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::ForceBreak;
		}
	}

	Generator::LeaveScope (ostream);
	Generator::PopExecutionContext (ostream);
}

void
X3DExecutionContext::toXMLStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	Generator::PushExecutionContext (ostream, this);
	Generator::EnterScope (ostream);
	Generator::ImportedNodes (ostream, getImportedNodes ());

	for (const auto & externProto : getExternProtoDeclarations ())
	{
		ostream
			<< XMLEncode (externProto)
			<< Generator::TidyBreak;
	}

	for (const auto & proto : getProtoDeclarations ())
	{
		ostream
			<< XMLEncode (proto)
			<< Generator::TidyBreak;
	}

	if (not getRootNodes () .empty ())
	{
		ostream
			<< XMLEncode (getRootNodes ())
			<< Generator::TidyBreak;
	}

	for (const auto & importedNode : getImportedNodes ())
	{
		try
		{
			ostream
				<< XMLEncode (importedNode .second)
				<< Generator::TidyBreak;
		}
		catch (const X3DError &)
		{ }
	}

	for (const auto & route : getRoutes ())
	{
		try
		{
			ostream
				<< XMLEncode (route)
				<< Generator::TidyBreak;
		}
		catch (const X3DError &)
		{ }
	}

	Generator::LeaveScope (ostream);
	Generator::PopExecutionContext (ostream);
}

void
X3DExecutionContext::toJSONStream (std::ostream & ostream) const
{
	ostream .imbue (std::locale::classic ());

	Generator::PushExecutionContext (ostream, this);
	Generator::EnterScope (ostream);
	Generator::ImportedNodes (ostream, getImportedNodes ());

	bool lastProperty = false;


	// Extern proto declarations

	if (not getExternProtoDeclarations () .empty ())
	{
		if (lastProperty)
		{
			ostream
				<< ','
				<< Generator::TidyBreak;
		}
	
		for (const auto & externProto : getExternProtoDeclarations ())
		{
			ostream
				<< Generator::Indent
				<< JSONEncode (externProto);
	
			if (externProto not_eq getExternProtoDeclarations () .back ())
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
		}
	
		lastProperty = true;
	}


	// Proto declarations

	if (not getProtoDeclarations () .empty ())
	{
		if (lastProperty)
		{
			ostream
				<< ','
				<< Generator::TidyBreak;
		}
	
		for (const auto & proto : getProtoDeclarations ())
		{
			ostream
				<< Generator::Indent
				<< JSONEncode (proto);
	
			if (proto not_eq getProtoDeclarations () .back ())
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
		}
	
		lastProperty = true;
	}


	// Root nodes

	if (not getRootNodes () .empty ())
	{
		if (lastProperty)
		{
			ostream
				<< ','
				<< Generator::TidyBreak;
		}

		for (const auto & value : std::make_pair (getRootNodes () .cbegin (), getRootNodes () .cend ()))
		{
			if (value)
			{
				ostream
					<< Generator::Indent
					<< JSONEncode (value);
			}
			else
			{
				ostream
					<< Generator::Indent
					<< "null";
			}

			if (&value not_eq &getRootNodes () .back ())
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
		}

		lastProperty = true;
	}


	// Imported nodes

	if (not getImportedNodes () .empty ())
	{
		std::vector <std::string> importedNodes;

		for (const auto & importedNode : getImportedNodes ())
		{
			try
			{
				std::ostringstream osstream;
	
				osstream << SetGenerator (ostream) << JSONEncode (importedNode .second);
	
				importedNodes .emplace_back (osstream .str ());
			}
			catch (const X3DError &)
			{ }
		}

		if (not importedNodes .empty ())
		{
			if (lastProperty)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
	
			for (const auto & importedNode : importedNodes)
			{
				ostream
					<< Generator::Indent
					<< importedNode;
		
				if (&importedNode not_eq &importedNodes .back ())
				{
					ostream
						<< ','
						<< Generator::TidyBreak;
				}
			}
	
			lastProperty = true;
		}
	}


	// Routes

	if (not getRoutes () .empty ())
	{
		std::vector <std::string> routes;

		for (const auto & route : getRoutes ())
		{
			try
			{
				std::ostringstream osstream;
	
				osstream << SetGenerator (ostream) << JSONEncode (route);
	
				routes .emplace_back (osstream .str ());
			}
			catch (const X3DError & error)
			{
				__LOG__ << error .what () << std::endl;
			}
		}

		if (not routes .empty ())
		{
			if (lastProperty)
			{
				ostream
					<< ','
					<< Generator::TidyBreak;
			}
	
			for (const auto & route : routes)
			{
				ostream
					<< Generator::Indent
					<< route;
		
				if (&route not_eq &routes .back ())
				{
					ostream
						<< ','
						<< Generator::TidyBreak;
				}
			}
	
			lastProperty = true;
		}
	}


	Generator::LeaveScope (ostream);
	Generator::PopExecutionContext (ostream);
}

void
X3DExecutionContext::dispose ()
{
	namedNodes    .clear ();
	importedNodes .clear ();
	prototypes    .clear ();
	externProtos  .clear ();
	routes        .clear ();
	
	rootNodes -> clear ();
}

X3DExecutionContext::~X3DExecutionContext ()
{ }

} // X3D
} // titania

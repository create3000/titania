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

#include "X3DExecutionContext.h"

#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Networking/Inline.h"
#include "../Prototype/ExternProto.h"
#include "../Prototype/Proto.h"

#include <Titania/Utility/Adapter.h>

#include <algorithm>
#include <iostream>

namespace titania {
namespace X3D {

X3DExecutionContext::X3DExecutionContext () :
	        X3DChildNode (),              
	            worldURL (),              
	            encoding ("X3D"),         
	specificationVersion ("3.0"),         
	   characterEncoding ("utf8"),        
	             comment ("Titania 0.1"), 
	          components (),              
	             profile (NULL),          
	          namedNodes (),              
	       exportedNodes (),              
	       importedNodes (),              
	              protos (),              
	        externProtos (),              
	              routes (),              
	           rootNodes ()               
{ }

void
X3DExecutionContext::setup ()
{
	X3DChildNode::setup ();

	// Add rootNodes here as child. This prevents X3DProtoypeInstances from being disposed on construction.
	setChildren (rootNodes);
}

void
X3DExecutionContext::assign (const X3DExecutionContext* const executionContext)
{
	setEncoding             (executionContext -> getEncoding ());
	setSpecificationVersion (executionContext -> getSpecificationVersion ());
	setCharacterEncoding    (executionContext -> getCharacterEncoding ());
	setComment              (executionContext -> getComment ());

	setWorldURL (executionContext -> getWorldURL ());

	addComponents (components);
	setProfile (profile);

	for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
		updateExternProtoDeclaration (externProto .getNodeName (), externProto);

	for (const auto & proto : executionContext -> getProtoDeclarations ())
		updateProtoDeclaration (proto .getNodeName (), proto);

	for (const auto & rootNode : executionContext -> getRootNodes ())
		addRootNode (rootNode .getValue () -> clone (this));

	for (const auto & importedNode : executionContext -> getImportedNodes ())
		addImportedNode (SFNode <Inline> (dynamic_cast <Inline*> (getNamedNode (importedNode .getName () [0]) .getValue ())),
		                 importedNode .getName () [1],
		                 importedNode .getName () [2]);

	for (const auto & exportedNode : executionContext -> getExportedNodes ())
		addExportedNode (exportedNode .getName () [0], exportedNode .getName () [1]);

	for (const auto & route : executionContext -> getRoutes ())
		route -> add (this);
}

void
X3DExecutionContext::setWorldURL (const basic::uri & value)
{
	worldURL = value;
}

const basic::uri &
X3DExecutionContext::getWorldURL () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return worldURL;
}

void
X3DExecutionContext::setEncoding (const std::string & value)
{
	encoding = value;
}

const std::string &
X3DExecutionContext::getEncoding () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return encoding;
}

void
X3DExecutionContext::setSpecificationVersion (const std::string & value)
{
	specificationVersion = value;
}

const std::string &
X3DExecutionContext::getSpecificationVersion () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return specificationVersion;
}

void
X3DExecutionContext::setCharacterEncoding (const std::string & value)
{
	characterEncoding = value;
}

const std::string &
X3DExecutionContext::getCharacterEncoding () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return characterEncoding;
}

void
X3DExecutionContext::setComment (const std::string & value)
{
	comment = value;
}

const std::string &
X3DExecutionContext::getComment () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return comment;
}

void
X3DExecutionContext::addComponents (const ComponentInfoArray & value)
{
	for (const auto & component : value)
		components .push_back (component -> getName (), component);
}

const ComponentInfoArray &
X3DExecutionContext::getComponents () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return components;
}

void
X3DExecutionContext::setProfile (const ProfileInfo* value)
{
	profile = value;
}

const ProfileInfo*
X3DExecutionContext::getProfile ()  const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return profile;
}

SFNode <X3DBaseNode>
X3DExecutionContext::createNode (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DBaseNode* declaration = getBrowser () -> getNode (name);

	if (getProfile () or getComponents () .size ())
	{
		bool found = false;
	
		if (getProfile ())
		{
			try
			{
				profile -> getComponents () .rfind (declaration -> getComponentName ());
				
				found = true;
			}
			catch (const std::out_of_range &)
			{ }
		}
	
		if (not found)
		{
			try
			{
				components .rfind (declaration -> getComponentName ());
				
				found = true;
			}
			catch (const std::out_of_range &)
			{ }
		}

		if (not found)
			throw Error <INVALID_NAME> ("Node type '" + name + "' not supported by profile or component specification.");
	}

	SFNode <X3DBaseNode> node = declaration -> create (this);

	return node;
}

SFNode <X3DPrototypeInstance>
X3DExecutionContext::createProtoInstance (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_X3D>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return protos .rfind (name) -> createInstance ();
	}
	catch (const std::out_of_range &)
	{
		try
		{
			return externProtos .rfind (name) -> createInstance ();
		}
		catch (const std::out_of_range &)
		{
			if (not isScene ())
				return getExecutionContext () -> createProtoInstance (name);

			throw Error <INVALID_NAME> ("Unknown proto type '" + name + "'.");
		}
		catch (const X3DError & error)
		{
			throw Error <INVALID_X3D> (error .what ());		
		}
	}
}

// Named node handling

const SFNode <X3DBaseNode> &
X3DExecutionContext::getNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return getNamedNode (name);
	}
	catch (const Error <INVALID_NAME> &)
	{
		try
		{
			return getImportedNode (name);
		}
		catch (const Error <INVALID_NAME> &)
		{
			throw Error <INVALID_NAME> ("Unknown named node '" + name + "'.");
		}
	}
}

void
X3DExecutionContext::updateNamedNode (const std::string & name, const SFNode <X3DBaseNode> & node)
throw (Error <IMPORTED_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	namedNodes .erase (node .getNodeName ());

	if (name .length ())
	{
		namedNodes [name] = node;
		namedNodes [name] .addParent (this);
		namedNodes [name] .setName (name);
	}

	node .setNodeName (name);
}

const SFNode <X3DBaseNode> &
X3DExecutionContext::getNamedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto iter = namedNodes .find (name);

	if (iter not_eq namedNodes .end ())
		return iter -> second;

	throw Error <INVALID_NAME> ("Named node '" + name + "' not found.");
}

// Exported nodes handling

void
X3DExecutionContext::addExportedNode (const std::string & localName, const std::string & exportedNameId)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (localName .empty ())
		throw Error <INVALID_NAME> ("Bad exported node specification: local node name is empty.");

	const std::string & exportedName = exportedNameId .size () ? exportedNameId : localName;

	const SFNode <X3DBaseNode> & node = getNode (localName);

	SFNode <X3DBaseNode> & exportedNode = exportedNodes .push_back (exportedName, node);
	exportedNode .addParent (this);
	exportedNodes .back () .setName ({ localName, exportedName });
	exportedNodes .back () .addParent (this);
}

void
X3DExecutionContext::removeExportedNode (const std::string & exportedName)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

void
X3DExecutionContext::updateExportedNode (const std::string & localName, const std::string & exportedName)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

const SFNode <X3DBaseNode> &
X3DExecutionContext::getExportedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return exportedNodes .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Exported node '" + name + "' not found.");
	}
}

// Imported nodes handling

void
X3DExecutionContext::addImportedNode (const SFNode <Inline> & inlineNode, const std::string & exportedName, const std::string & localNameId)
throw (Error <INVALID_NAME>,
       Error <NODE_IN_USE>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not inlineNode)
		throw Error <INVALID_NODE> ("Bad imported node specification: '" + inlineNode .getNodeName () + "' is not an Inline node.");

	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Bad imported node specification: exported node name is empty.");

	const std::string & localName = localNameId .size () ? localNameId : exportedName;

	// if the inline node is inside a proto it should not be loaded. And maybe inline shouldn have a scene at startup.

	inlineNode -> requestImmediateLoad ();

	if (inlineNode -> checkLoadState () == COMPLETE_STATE)
	{
		SFNode <X3DBaseNode> node = inlineNode -> getScene () -> getExportedNode (exportedName);

		SFNode <X3DBaseNode> & importedNode = importedNodes .push_back (localName, node);
		importedNode .setName ({ inlineNode .getNodeName (), exportedName, localName });
		importedNode .addParent (this);
		
		importedNodes .back () .setName ({ inlineNode .getNodeName (), exportedName, localName });
		importedNodes .back () .addParent (this);
	}
	else
		throw Error <URL_UNAVAILABLE> ("Imported node error: Could not load Inline '" + inlineNode .getNodeName () + "'.");
}

void
X3DExecutionContext::removeImportedNode (const std::string & exportedName, const std::string & localName)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

void
X3DExecutionContext::updateImportedNode (const SFNode <Inline> & inlineNode, const std::string & exportedName, const std::string & localName)
throw (Error <INVALID_NAME>,
       Error <NODE_IN_USE>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

const SFNode <X3DBaseNode> &
X3DExecutionContext::getImportedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return importedNodes .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Imported node '" + name + "' not found.");
	}
}

//	Proto declaration handling

const SFNode <Proto> &
X3DExecutionContext::addProtoDeclaration (const std::string & name, const FieldDefinitionArray & interfaceDeclarations)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto & proto = protos .push_back (name, createProtoDeclaration (name, interfaceDeclarations));

	proto .addParent (this);
	protos .back () .addParent (this);

	return protos .back ();
}

void
X3DExecutionContext::removeProtoDeclaration (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	protos .erase (name);
}

void
X3DExecutionContext::updateProtoDeclaration (const std::string & name, const SFNode <Proto> & protoDeclaration)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto & proto = protos .replace (protoDeclaration .getNodeName (), name, protoDeclaration);

	proto .addParent (this);
	protos .back () .addParent (this);

	proto .setNodeName (name);
}

const SFNode <Proto> &
X3DExecutionContext::getProtoDeclaration (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return protos .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("PROTO '" + name + "' not found.");
	}
}

SFNode <Proto>
X3DExecutionContext::createProtoDeclaration (const std::string & name, const FieldDefinitionArray & interfaceDeclarations)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	Proto* proto = new Proto (this);

	proto -> setName (name);

	for (const auto & field : interfaceDeclarations)
	{
		proto -> addUserDefinedField (field -> getAccessType (),
		                              field -> getName (),
		                              field);
	}

	proto -> setup ();

	return proto;
}

//	externprotoDeclarationHandling

const SFNode <ExternProto> &
X3DExecutionContext::addExternProtoDeclaration (const std::string & name, const FieldDefinitionArray & externInterfaceDeclarations, const MFString & URLList)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto & externProto = externProtos .push_back (name, createExternProtoDeclaration (name, externInterfaceDeclarations, URLList));

	externProto .addParent (this);
	externProtos .back () .addParent (this);

	return externProtos .back ();
}

void
X3DExecutionContext::removeExternProtoDeclaration (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	externProtos .erase (name);
}

void
X3DExecutionContext::updateExternProtoDeclaration (const std::string & name, const SFNode <ExternProto> & externProtoDeclaration)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto & externProto = externProtos .replace (externProtoDeclaration .getNodeName (), name, externProtoDeclaration);

	externProto .addParent (this);
	externProtos .back () .addParent (this);

	externProto .setNodeName (name);
}

const
SFNode <ExternProto> &
X3DExecutionContext::getExternProtoDeclaration (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return externProtos .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("EXTERNPROTO '" + name + "' not found.");
	}
}

SFNode <ExternProto>
X3DExecutionContext::createExternProtoDeclaration (const std::string & name, const FieldDefinitionArray & externInterfaceDeclarations, const MFString & URLList)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	ExternProto* externProto = new ExternProto (this);

	externProto -> setName (name);

	for (const auto & field : externInterfaceDeclarations)
	{
		externProto -> addUserDefinedField (field -> getAccessType (),
		                                    field -> getName (),
		                                    field);
	}

	externProto -> url () = URLList;

	externProto -> setup ();

	return externProto;
}

// Root nodes handling

void
X3DExecutionContext::addRootNode (const SFNode <X3DBaseNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	rootNodes .emplace_back (rootNode);
}

void
X3DExecutionContext::removeRootNode (const SFNode <X3DBaseNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::remove (rootNodes .begin (), rootNodes .end (), rootNode);
}

void
X3DExecutionContext::setRootNode (size_t index, const SFNode <X3DBaseNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (rootNodes .size () <= index)
		rootNodes .resize (index + 1);

	rootNodes [index] = rootNode;
}

const SFNode <X3DBaseNode> &
X3DExecutionContext::getRootNode (size_t index)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (rootNodes .size () <= index)
		rootNodes .resize (index + 1);

	return rootNodes [index];
}

const MFNode &
X3DExecutionContext::getRootNodes () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return rootNodes;
}

//	Dynamic route handling

const SFNode <Route> &
X3DExecutionContext::addRoute (const SFNode <X3DBaseNode> & sourceNode,      const std::string & sourceFieldId,
                               const SFNode <X3DBaseNode> & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto fields = createRouteFieldsPair (sourceNode, sourceFieldId, destinationNode, destinationFieldId);

	try
	{
		// Silently return if route already exists.

		return routes .rfind (fields);
	}
	catch (const std::out_of_range &)
	{
		// Add route.

		auto & route = routes .push_back (fields, new Route (this, sourceNode, fields .first, destinationNode, fields .second));

		route .addParent (this);
		routes .back () .addParent (this);
		return routes .back ();
	}
}

void
X3DExecutionContext::deleteRoute (const SFNode <X3DBaseNode> & sourceNode,      const std::string & sourceFieldId,
                                  const SFNode <X3DBaseNode> & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto fields = createRouteFieldsPair (sourceNode, sourceFieldId, destinationNode, destinationFieldId);

	try
	{
		routes .rfind (fields) -> disconnect ();
		routes .erase (fields);
	}
	catch (const std::out_of_range &)
	{
		// Silently return if route not exists.
	}
}

std::pair <X3DFieldDefinition*, X3DFieldDefinition*>
X3DExecutionContext::createRouteFieldsPair (const SFNode <X3DBaseNode> & sourceNode,      const std::string & sourceFieldId,
                                            const SFNode <X3DBaseNode> & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>)
{
	if (not sourceNode .getValue ())
		throw Error <INVALID_NODE> ("Bad ROUTE specification: source node is NULL.");

	if (not destinationNode .getValue ())
		throw Error <INVALID_NODE> ("Bad ROUTE specification: destination node is NULL.");

	X3DFieldDefinition* sourceField = sourceNode .getField (sourceFieldId);

	if (not sourceField)
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventOut '" + sourceFieldId + "' in node '" + sourceNode .getNodeName () + "'.");

	X3DFieldDefinition* destinationField = destinationNode .getField (destinationFieldId);

	if (not destinationField)
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventIn '" + destinationFieldId + "' in node '" + destinationNode .getNodeName () + "'.");

	return std::make_pair (sourceField, destinationField);
}

const RouteArray &
X3DExecutionContext::getRoutes () const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return routes;
}

void
X3DExecutionContext::changeViewpoint (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		auto viewpoint = dynamic_cast <X3DViewpointNode*> (getNamedNode (name) .getValue ());

		if (viewpoint)
			viewpoint -> set_bind () = true;

		else
			throw Error <INVALID_NAME> ("Warning: Node named '" + name + "' is not a viewpoint node.");
	}
	catch (const Error <INVALID_NAME> & error)
	{
		if (not isScene ())
			getExecutionContext () -> changeViewpoint (name);
		else
			throw error;
	}
}

void
X3DExecutionContext::toStream (std::ostream & ostream) const
{
	Generator::PushLevel ();

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

	if (getRootNodes () .size ())
	{
		for (const auto & child : basic::adapter (getRootNodes () .cbegin (), getRootNodes () .cend () - 1))
		{
			ostream
				<< Generator::Indent
				<< child
				<< Generator::TidyBreak
				<< Generator::TidyBreak;
		}

		ostream
			<< Generator::Indent
			<< getRootNodes () .back ()
			<< Generator::TidyBreak;
	}

	if (getImportedNodes () .size ())
		ostream << Generator::TidyBreak;

	for (const auto & importedNode : getImportedNodes ())
	{
		ostream
			<< Generator::Indent
			<< "IMPORT"
			<< Generator::Space
			<< importedNode .getName () [0]
			<< '.'
			<< importedNode .getName () [1];

		if (importedNode .getName () [1] not_eq importedNode .getName () [2])
		{
			ostream
				<< Generator::Space
				<< "AS"
				<< Generator::Space
				<< importedNode .getName () [2];
		}

		ostream << Generator::Break;
	}

	if (getExportedNodes () .size ())
		ostream << Generator::TidyBreak;

	for (const auto & exportedNode : getExportedNodes ())
	{
		ostream
			<< Generator::Indent
			<< "EXPORT"
			<< Generator::Space
			<< exportedNode .getName () [0];

		if (exportedNode .getName () [0] not_eq exportedNode .getName () [1])
		{
			ostream
				<< Generator::Space
				<< "AS"
				<< Generator::Space
				<< exportedNode .getName () [1];
		}

		ostream << Generator::Break;
	}

	if (getRoutes () .size ())
		ostream << Generator::TidyBreak;

	for (const auto & route : getRoutes ())
	{
		ostream
			<< route
			<< Generator::Break;
	}

	if (getInnerComments () .size ())
	{
		ostream << Generator::TidyBreak;
	
		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::Break;
		}
	}

	Generator::PopLevel ();
}

void
X3DExecutionContext::dispose ()
{
	profile = NULL;
	components .clear ();

	namedNodes    .clear ();
	exportedNodes .clear ();
	importedNodes .clear ();
	protos        .clear ();
	externProtos  .clear ();
	routes        .clear ();

	rootNodes .dispose ();

	X3DChildNode::dispose ();
}

X3DExecutionContext::~X3DExecutionContext ()
{ }

} // X3D
} // titania

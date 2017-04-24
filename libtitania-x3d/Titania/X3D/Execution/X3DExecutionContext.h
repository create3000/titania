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

#ifndef __TITANIA_X3D_EXECUTION_X3DEXECUTION_CONTEXT_H__
#define __TITANIA_X3D_EXECUTION_X3DEXECUTION_CONTEXT_H__

#include "../Basic/X3DBaseNode.h"
#include "../Browser/ContextLock.h"
#include "../Configuration/ProfileInfo.h"
#include "../Configuration/SupportedComponents.h"
#include "../Execution/EncodingType.h"
#include "../Execution/ImportedNodeIndex.h"
#include "../Execution/NamedNodeIndex.h"
#include "../Fields.h"
#include "../Prototype/ExternProtoArray.h"
#include "../Prototype/ProtoArray.h"
#include "../Routing/Route.h"
#include "../Routing/RouteArray.h"

#include <Titania/Basic/URI.h>
#include <map>

namespace titania {
namespace X3D {

struct AvailableType { };

class X3DProtoDeclarationNode;
class X3DPrototypeInstance;

class X3DExecutionContext :
	virtual public X3DBaseNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Construction

	bool
	getRealized () const
	throw (Error <DISPOSED>)
	{ return realized; }

	void
	realize ()
	throw (Error <DISPOSED>);

	///  @name Member access

	virtual
	bool
	isScene () const
	throw (Error <DISPOSED>)
	{ return false; }

	///  @name Member access

	virtual
	std::string
	getTitle () const
	throw (Error <DISPOSED>)
	{ return getExecutionContext () -> getTitle (); }

	virtual
	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	EncodingType
	getEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	SpecificationVersionType
	getSpecificationVersion () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	const std::string &
	getCharacterEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	const std::string &
	getComment () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	///  @name Profile/Component handling

	virtual
	const ProfileInfoPtr &
	getProfile ()  const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	void
	updateComponent (const ComponentInfoPtr & component)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	const ComponentInfoArray &
	getComponents () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	bool
	hasComponent (const ComponentType &) const;

	///  @name WorldInfo handling

	void
	setWorldInfo (const X3DPtr <WorldInfo> & value)
	throw (Error <DISPOSED>);

	X3DPtr <WorldInfo>
	getWorldInfo () const
	throw (Error <DISPOSED>);

	///  @name Node handling

	template <class Type, class ... Args>
	X3DPtr <Type>
	createNode (Args && ... args)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	SFNode
	createNode (const std::string & typeName)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DPrototypeInstancePtr
	createProto (const std::string & typeName)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_X3D>,
	       Error <INVALID_FIELD>,
	       Error <INVALID_ACCESS_TYPE>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Named node handling

	void
	addNamedNode (const std::string &, const SFNode &)
	throw (Error <NODE_IN_USE>,
	       Error <IMPORTED_NODE>,
	       Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	updateNamedNode (const std::string &, const SFNode &)
	throw (Error <IMPORTED_NODE>,
	       Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeNamedNode (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	SFNode
	getNamedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	template <class Type>
	X3DPtr <Type>
	getNamedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const NamedNodeIndex &
	getNamedNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return namedNodes; }

	std::string
	getUniqueName (std::string = "") const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const SFTime &
	namedNodes_changed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return namedNodesOutput; }

	///  @name Imported nodes handling

	const ImportedNodePtr &
	addImportedNode (const X3DPtr <Inline> &, const std::string &, std::string = "")
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <NODE_IN_USE>,
	       Error <URL_UNAVAILABLE>,
	       Error <NODE_NOT_AVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ImportedNodePtr &
	updateImportedNode (const X3DPtr <Inline> &, const std::string &, std::string = "")
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <URL_UNAVAILABLE>,
	       Error <NODE_NOT_AVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeImportedNode (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	SFNode
	getImportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	template <class Type>
	X3DPtr <Type>
	getImportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ImportedNodeIndex &
	getImportedNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return importedNodes; }

	bool
	isImportedNode (const SFNode & node) const
	throw (Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const SFTime &
	importedNodes_changed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return importedNodesOutput; }

	///  @name Named/Imported node handling

	SFNode
	getLocalNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const std::string &
	getLocalName (const SFNode &) const
	throw (Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	bool
	isLocalNode (const SFNode & node) const
	throw (Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Proto declaration handling

	ProtoDeclarationPtr
	createProtoDeclaration (const std::string &, const FieldDefinitionArray &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ProtoDeclarationPtr &
	addProtoDeclaration (const std::string &, const ProtoDeclarationPtr &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	updateProtoDeclaration (const std::string &, const ProtoDeclarationPtr &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeProtoDeclaration (const std::string & name)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ProtoDeclarationPtr &
	getProtoDeclaration (const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ProtoArray &
	getProtoDeclarations () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return prototypes; }

	std::string
	getUniqueProtoName (std::string) const
	throw (Error <DISPOSED>);

	const SFTime &
	prototypes_changed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return prototypesOutput; }

	///  @name Exterproto declaration handling

	ExternProtoDeclarationPtr
	createExternProtoDeclaration (const std::string &, const FieldDefinitionArray &, const MFString &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ExternProtoDeclarationPtr &
	addExternProtoDeclaration (const std::string &, const ExternProtoDeclarationPtr &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	updateExternProtoDeclaration (const std::string &, const ExternProtoDeclarationPtr &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeExternProtoDeclaration (const std::string & name)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ExternProtoDeclarationPtr &
	getExternProtoDeclaration (const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ExternProtoArray &
	getExternProtoDeclarations () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return externProtos; }

	std::string
	getUniqueExternProtoName (std::string) const
	throw (Error <DISPOSED>);

	void
	requestImmediateLoadOfExternProtos ();

	void
	requestAsyncLoadOfExternProtos ();

	void
	addExternProtoLoadCount (const ExternProtoDeclaration* const);

	void
	removeExternProtoLoadCount (const ExternProtoDeclaration* const);
	       
	const SFInt32 &
	getExternProtosLoadCount () const
	{ return externProtosLoadCount; }

	const SFTime &
	externProtos_changed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return externProtosOutput; }

	///  @name ProtoObject handling

	virtual
	X3DProtoDeclarationNode*
	findProtoDeclaration (const std::string &, const AvailableType &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DProtoDeclarationNode*
	findProtoDeclaration (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	std::map <std::string, X3DProtoDeclarationNode*>
	findProtoDeclarations () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Root node handling

	MFNode &
	getRootNodes ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return *rootNodes; }

	const MFNode &
	getRootNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return *rootNodes; }

	const SFTime &
	sceneGraph_changed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return sceneGraphOutput; }

	const SFTime &
	bbox_changed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return bboxOutput; }

	///  @name Dynamic route node handling

	const RoutePtr &
	addRoute (const SFNode &, const std::string &,
	          const SFNode &, const std::string &)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const RoutePtr &
	addRoute (Route* const)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	deleteRoute (const SFNode &, const std::string &,
	             const SFNode &, const std::string &)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	deleteRoute (Route* const)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const RouteArray &
	getRoutes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return routes; }

	const SFTime &
	routes_changed () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return routesOutput; }

	///  @name Viewpoint handling

	void
	changeViewpoint (const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Import handling

	virtual
	void
	import (X3DExecutionContext* const, MFNode &)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream &) const override;

	virtual
	void
	toXMLStream (std::ostream &) const override;

	virtual
	void
	toJSONStream (std::ostream &) const override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DExecutionContext () override;


protected:

	friend class ExternProtoDeclaration;
	friend class Inline;
	friend class X3DBaseNode;
	friend class X3DParser;

	///  @name Construction

	X3DExecutionContext ();

	/// Initialize this node.
	virtual
	void
	initialize () override;

	/// Add uninitialized node. The node will be initialize if this execution context becomes initialized.
	void
	addUninitializedNode (X3DBaseNode* const node)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Import handling

	void
	importExternProtos (const X3DExecutionContext* const executionContext, const CopyType type)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	void
	importProtos (const X3DExecutionContext* const executionContext, const CopyType type)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	void
	copyRootNodes (const X3DExecutionContext* const executionContext, const CopyType type)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	void
	copyImportedNodes (const X3DExecutionContext* const executionContext, const CopyType type)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	void
	copyRoutes (const X3DExecutionContext* const executionContext, const CopyType type)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);


private:

	///  @name Member types

	using ImportedNamesIndex = std::multimap <size_t, std::string>;

	///  @name Operations

	std::string
	getVeryUniqueName (X3DExecutionContext* const, std::string = "") const;

	std::string
	getUniqueImportedName (const X3DExecutionContext* const, std::string = "") const;

	void
	removeImportedName (const ImportedNamesIndex::iterator &);

	void
	set_sceneGraph ();

	void
	set_bbox ();

	RouteId
	getRouteId (const SFNode &, const std::string &,
	            const SFNode &, const std::string &)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>);

	///  @name Import handling

	void
	updateNamedNodes (X3DExecutionContext* const executionContext) const
	throw (Error <IMPORTED_NODE>,
	       Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	updateImportedNodes (X3DExecutionContext* const executionContext) const
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <URL_UNAVAILABLE>,
	       Error <NODE_NOT_AVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	importNodes (const X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	importNamedNodes (const X3DExecutionContext* const)
	throw (Error <IMPORTED_NODE>,
	       Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	importRoutes (X3DExecutionContext* const executionContext);

	///  @name Members

	X3DWeakPtr <WorldInfo>                   worldInfo; 
	NamedNodeIndex                           namedNodes;
	SFTime                                   namedNodesOutput;
	ImportedNodeIndex                        importedNodes;
	ImportedNamesIndex                       importedNames;
	SFTime                                   importedNodesOutput;
	ProtoArray                               prototypes;
	SFTime                                   prototypesOutput;
	ExternProtoArray                         externProtos;
	SFInt32                                  externProtosLoadCount;
	std::set <const ExternProtoDeclaration*> loadingExternProtos;
	SFTime                                   externProtosOutput;
	RouteArray                               routes;
	SFTime                                   routesOutput;
	MFNode* const                            rootNodes;
	SFTime                                   sceneGraphOutput;
	SFTime                                   bboxOutput;
	MFNode                                   uninitializedNodes;
	bool                                     realized;

};

template <class Type, class ... Args>
X3DPtr <Type>
X3DExecutionContext::createNode (Args && ... args)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DPtr <Type> node (new Type (this, std::forward <Args> (args) ...));

	if (getRealized ())
	{
		ContextLock lock (this);

		node -> setup ();
	}
	else
		addUninitializedNode (node);

	return node;
}

template <class Type>
X3DPtr <Type>
X3DExecutionContext::getNamedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DPtr <Type> node (getNamedNode (name));

	if (node)
		return node;

	throw Error <INVALID_NODE> ("Invalid node: node '" + name + "' has other type.");
}

template <class Type>
X3DPtr <Type>
X3DExecutionContext::getImportedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DPtr <Type> node (getImportedNode (name));

	if (node)
		return node;

	throw Error <INVALID_NODE> ("Invalid node: node '" + name + "' has other type.");
}

} // X3D
} // titania

#endif

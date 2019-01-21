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
#include "../Configuration/ProfileInfo.h"
#include "../Configuration/SupportedComponents.h"
#include "../Configuration/UnitCategory.h"
#include "../Execution/EncodingType.h"
#include "../Execution/ImportedNodeIndex.h"
#include "../Execution/NamedNodeIndex.h"
#include "../Fields.h"
#include "../Prototype/ExternProtoDeclarationArray.h"
#include "../Prototype/ProtoDeclarationArray.h"
#include "../RenderingSurface/ContextLock.h"
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
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Construction

	///  throws Error <DISPOSED>
	bool
	getRealized () const
	{ return realized; }

	///  throws Error <DISPOSED>
	void
	realize ();

	///  @name Member access

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const basic::uri &
	getWorldURL () const = 0;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	EncodingType
	getEncoding () const = 0;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	SpecificationVersionType
	getSpecificationVersion () const = 0;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const std::string &
	getCharacterEncoding () const = 0;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const std::string &
	getComment () const = 0;

	///  @name Profile/Component handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const ProfileInfoPtr &
	getProfile () const = 0;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const ComponentInfoArray &
	getComponents () const = 0;

	///  @name Unit handling

	///  throws Error <DISPOSED>
	virtual
	long double
	fromUnit (const UnitCategory category, const long double value) const = 0;

	///  throws Error <DISPOSED>
	virtual
	long double
	toUnit (const UnitCategory category, const long double value) const = 0;

	///  @name Node handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	template <class Type, class ... Args>
	X3DPtr <Type>
	createNode (Args && ... args);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	SFNode
	createNode (const std::string & typeName);

	///  throws Error <INVALID_NAME>, Error <INVALID_X3D>, Error <INVALID_FIELD>, Error <INVALID_ACCESS_TYPE>, Error <URL_UNAVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	X3DPrototypeInstancePtr
	createProto (const std::string & typeName);

	///  @name Named node handling

	///  throws Error <NODE_IN_USE>, Error <IMPORTED_NODE>, Error <INVALID_NODE>, Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	addNamedNode (const std::string &, const SFNode &);

	///  throws Error <IMPORTED_NODE>, Error <INVALID_NODE>, Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	updateNamedNode (const std::string &, const SFNode &);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	removeNamedNode (const std::string &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	SFNode
	getNamedNode (const std::string &) const;

	///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	template <class Type>
	X3DPtr <Type>
	getNamedNode (const std::string &) const;

	///  throw rror <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const NamedNodeIndex &
	getNamedNodes () const
	{ return namedNodes; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	std::string
	getUniqueName (std::string = "") const;

	const SFTime &
	namedNodes_changed () const
	{ return namedNodesOutput; }

	///  @name Imported nodes handling

	///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	bool
	isImportedNode (const SFNode & node) const;

	///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <NODE_IN_USE>, Error <URL_UNAVAILABLE>, Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ImportedNodePtr &
	addImportedNode (const X3DPtr <Inline> &, const std::string &, std::string = "");

	///  throws Error <INVALID_NODE>, Error <INVALID_NAME>, Error <URL_UNAVAILABLE>, Error <NODE_NOT_AVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ImportedNodePtr &
	updateImportedNode (const X3DPtr <Inline> &, const std::string &, std::string = "");

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	removeImportedNode (const std::string &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	SFNode
	getImportedNode (const std::string &) const;

	///  throws Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	template <class Type>
	X3DPtr <Type>
	getImportedNode (const std::string &) const;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ImportedNodeIndex &
	getImportedNodes () const
	{ return importedNodes; }

	const SFTime &
	importedNodes_changed () const
	{ return importedNodesOutput; }

	///  @name Named/Imported node handling

	///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	bool
	isLocalNode (const SFNode & node) const;

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	SFNode
	getLocalNode (const std::string &) const;

	///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const std::string &
	getLocalName (const SFNode &) const;

	///  @name Proto declaration handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	ProtoDeclarationPtr
	createProtoDeclaration (const std::string &, const FieldDefinitionArray &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	addProtoDeclaration (const std::string &, const ProtoDeclarationPtr &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	updateProtoDeclaration (const std::string &, const ProtoDeclarationPtr &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	removeProtoDeclaration (const std::string & name);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ProtoDeclarationPtr &
	getProtoDeclaration (const std::string &) const;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ProtoDeclarationArray &
	getProtoDeclarations () const
	{ return *prototypes; }

	bool
	hasProtoDeclaration (const std::string & name) const;

	///  throws Error <DISPOSED>
	std::string
	getUniqueProtoName (std::string) const;

	const SFTime &
	prototypes_changed () const
	{ return prototypesOutput; }

	///  @name Exterproto declaration handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	ExternProtoDeclarationPtr
	createExternProtoDeclaration (const std::string &, const FieldDefinitionArray &, const MFString &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	addExternProtoDeclaration (const std::string &, const ExternProtoDeclarationPtr &);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	updateExternProtoDeclaration (const std::string &, const ExternProtoDeclarationPtr &);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	removeExternProtoDeclaration (const std::string & name);

	///  throws Error <INVALID_NAME>, Error <URL_UNAVAILABLE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ExternProtoDeclarationPtr &
	getExternProtoDeclaration (const std::string &) const;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const ExternProtoDeclarationArray &
	getExternProtoDeclarations () const
	{ return *externProtos; }

	bool
	hasExternProtoDeclaration (const std::string & name) const;

	///  throws Error <DISPOSED>
	std::string
	getUniqueExternProtoName (std::string) const;

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
	{ return externProtosOutput; }

	///  @name ProtoObject handling

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	X3DProtoDeclarationNode*
	findProtoDeclaration (const std::string &, const AvailableType &) const;

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	X3DProtoDeclarationNode*
	findProtoDeclaration (const std::string &) const;

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	std::map <std::string, X3DProtoDeclarationNode*>
	findProtoDeclarations () const;

	///  @name Root node handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	addRootNode (const SFNode & value);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	removeRootNode (const SFNode & value);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	setRootNodes (const MFNode & value)
	{ *rootNodes = value; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	MFNode &
	getRootNodes ()
	{ return *rootNodes; }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const MFNode &
	getRootNodes () const
	{ return *rootNodes; }

	const SFTime &
	sceneGraph_changed () const
	{ return sceneGraphOutput; }

	const SFTime &
	bbox_changed () const
	{ return bboxOutput; }

	///  @name Dynamic route node handling

	///  throws Error <INVALID_NODE>, Error <INVALID_FIELD>, Error <IMPORTED_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const RoutePtr &
	addRoute (const SFNode & sourceNode, const std::string & sourceFieldId,
	          const SFNode & destinationNode, const std::string & destinationFieldId);

	///  throws Error <INVALID_NODE>, Error <INVALID_FIELD>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	deleteRoute (const SFNode & sourceNode, const std::string & sourceFieldId,
	             const SFNode & destinationNode, const std::string & destinationFieldId);

	///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	deleteRoute (Route* const);

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const RouteArray &
	getRoutes () const
	{ return *routes; }

	const SFTime &
	routes_changed () const
	{ return routesOutput; }

	///  @name Viewpoint handling

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	changeViewpoint (const std::string &);

	///  @name Import handling

	virtual
	MFNode
	importScene (X3DExecutionContext* const executionContext);

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

	///  @name Friends

	friend class ExternProtoDeclaration;
	friend class ImportedNode;
	friend class Inline;
	friend class WorldInfo;
	friend class X3DBaseNode;
	friend class X3DParser;

	///  @name Construction

	X3DExecutionContext ();

	/// Initialize this node.
	virtual
	void
	initialize () override;

	///  Add uninitialized node. The node will be initialize if this execution context becomes initialized.
	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	void
	addUninitializedNode (X3DBaseNode* const node);

	///  @name Route handling

	///  throws Error <INVALID_NODE>, Error <INVALID_FIELD>, Error <IMPORTED_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const RoutePtr &
	addSimpleRoute (const SFNode & sourceNode,      const std::string & sourceFieldId,
	                const SFNode & destinationNode, const std::string & destinationFieldId);

	///  @name Import handling

	void
	importExternProtos (const X3DExecutionContext* const executionContext, const CopyType type);

	void
	importProtos (const X3DExecutionContext* const executionContext, const CopyType type);

	void
	copyRootNodes (const X3DExecutionContext* const executionContext, const CopyType type);

	void
	copyImportedNodes (const X3DExecutionContext* const executionContext, const CopyType type);

	void
	copyRoutes (const X3DExecutionContext* const executionContext, const CopyType type);


private:

	///  @name Member types

	using ImportedNamesIndex = std::multimap <size_t, std::string>;

	///  @name Operations

	std::string
	getVeryUniqueName (X3DExecutionContext* const, std::string = "") const;

	std::string
	getVeryUniqueImportedName (const X3DExecutionContext* const, std::string = "") const;

	void
	set_sceneGraph ();

	void
	set_bbox ();

	///  @name Route handling

	///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	const RoutePtr &
	addRoute (Route* const);

	///  @name Import handling

	void
	updateNamedNodes (X3DExecutionContext* const executionContext) const;

	void
	updateImportedNodes (X3DExecutionContext* const executionContext) const;

	void
	importNodes (const X3DExecutionContext* const);

	void
	importNamedNodes (const X3DExecutionContext* const);

	void
	importRoutes (X3DExecutionContext* const executionContext);

	///  @name Members

	NamedNodeIndex                           namedNodes;
	SFTime                                   namedNodesOutput;
	ImportedNodeIndex                        importedNodes;
	SFTime                                   importedNodesOutput;
	ProtoDeclarationArrayPtr                 prototypes;
	SFTime                                   prototypesOutput;
	ExternProtoDeclarationArrayPtr           externProtos;
	SFInt32                                  externProtosLoadCount;
	std::set <const ExternProtoDeclaration*> loadingExternProtos;
	SFTime                                   externProtosOutput;
	RouteArrayPtr                            routes;
	SFTime                                   routesOutput;
	MFNode* const                            rootNodes;
	SFTime                                   sceneGraphOutput;
	SFTime                                   bboxOutput;
	MFNode                                   uninitializedNodes;
	bool                                     realized;

};

///  throws EError <INVALID_OPERATION_TIMING>, Error <DISPOSED>
template <class Type, class ... Args>
X3DPtr <Type>
X3DExecutionContext::createNode (Args && ... args)
{
	const X3DPtr <Type> node (new Type (this, std::forward <Args> (args) ...));

	if (getRealized ())
	{
		ContextLock lock (getBrowser ());

		node -> setup ();
	}
	else
		addUninitializedNode (node);

	return node;
}

///  throws Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
template <class Type>
X3DPtr <Type>
X3DExecutionContext::getNamedNode (const std::string & name) const
{
	X3DPtr <Type> node (getNamedNode (name));

	if (node)
		return node;

	throw Error <INVALID_NODE> ("Invalid node: node '" + name + "' has other type.");
}

///  throws Error <INVALID_NAME>, Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
template <class Type>
X3DPtr <Type>
X3DExecutionContext::getImportedNode (const std::string & name) const
{
	X3DPtr <Type> node (getImportedNode (name));

	if (node)
		return node;

	throw Error <INVALID_NODE> ("Invalid node: node '" + name + "' has other type.");
}

} // X3D
} // titania

#endif

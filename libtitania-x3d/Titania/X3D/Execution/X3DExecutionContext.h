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

#ifndef __TITANIA_X3D_EXECUTION_X3DEXECUTION_CONTEXT_H__
#define __TITANIA_X3D_EXECUTION_X3DEXECUTION_CONTEXT_H__

#include "../Components/Core/X3DNode.h"
#include "../Configuration/ComponentInfoArray.h"
#include "../Configuration/ProfileInfo.h"
#include "../Execution/ImportedNodeArray.h"
#include "../Prototype/ExternProtoArray.h"
#include "../Prototype/ProtoArray.h"
#include "../Routing/RouteArray.h"

#include <Titania/Basic/URI.h>
#include <map>
#include <mutex>

namespace titania {
namespace X3D {

class Inline;
class X3DProto;
class X3DBrowser;
class X3DPrototypeInstance;

typedef std::map <std::string, SFNode> NamedNodeIndex;

class X3DExecutionContext :
	virtual public X3DNode
{
public:

	///  @name Tests

	virtual
	bool
	isScene () const
	{ return false; }

	virtual
	bool
	isProto () const
	{ return false; }

	///  @name Member access

	void
	setWorldURL (const basic::uri & value)
	{ worldURL = value; }

	virtual
	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return worldURL; }

	void
	setEncoding (const std::string & value)
	{ encoding = value; }

	const std::string &
	getEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return encoding; }

	void
	setSpecificationVersion (const std::string & value)
	{ specificationVersion = value; }

	const std::string &
	getSpecificationVersion () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return specificationVersion; }

	void
	setCharacterEncoding (const std::string & value)
	{ characterEncoding = value; }

	const std::string &
	getCharacterEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return characterEncoding; }

	void
	setComment (const std::string & value)
	{ comment = value; }

	const std::string &
	getComment () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return comment; }

	void
	addComponents (const ComponentInfoArray &);

	const ComponentInfoArray &
	getComponents () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return components; }

	void
	setProfile (const ProfileInfo* value)
	{ profile = value; }

	const ProfileInfo*
	getProfile ()  const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return profile; }

	SFNode
	createNode (const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DSFNode <X3DPrototypeInstance>
	createProtoInstance (const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_X3D>,
	       Error <INVALID_FIELD>,
	       Error <INVALID_ACCESS_TYPE>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	/// Add uninitialized node
	virtual
	void
	addUninitializedNode (X3DBaseNode* const uninitializedNode)
	{ uninitializedNodes .emplace_back (uninitializedNode); }

	/// Get uninitialized nodes
	MFNode
	getUninitializedNodes ()
	{ return std::move (uninitializedNodes); }

	///  @name Named/Imported node handling

	const SFNode &
	getNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const std::string &
	getLocalName (const SFNode &) const
	throw (Error <INVALID_NODE>,
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

	const SFNode &
	getNamedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const NamedNodeIndex &
	getNamedNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return namedNodes; }

	std::string
	getUniqueName (std::string, bool = false) const;

	std::string
	getUniqueName () const;

	///  @name Imported nodes handling

	const X3DSFNode <ImportedNode> &
	addImportedNode (const X3DSFNode <Inline> &, const std::string &, std::string = "")
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <NODE_IN_USE>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeImportedNode (const std::string &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	updateImportedNode (const X3DSFNode <Inline> &, const std::string &, std::string = "")
	throw (Error <INVALID_NODE>,
	       Error <INVALID_NAME>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const SFNode &
	getImportedNode (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ImportedNodeArray &
	getImportedNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return importedNodes; }

	///  @name Proto declaration handling

	const X3DSFNode <Proto> &
	addProtoDeclaration (const std::string &, const FieldDefinitionArray &)
	throw (Error <INVALID_NAME>,
          Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeProtoDeclaration (const std::string & name)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ protos .erase (name); }

	void
	updateProtoDeclaration (const std::string &, const X3DSFNode <Proto> &)
	throw (Error <INVALID_NAME>,
          Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const X3DSFNode <Proto> &
	getProtoDeclaration (const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const ProtoArray &
	getProtoDeclarations () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return protos; }

	X3DProto*
	findProtoDeclaration (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_X3D>,
	       Error <URL_UNAVAILABLE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Exterproto declaration handling

	const X3DSFNode <ExternProto> &
	addExternProtoDeclaration (const std::string &, const FieldDefinitionArray &, const MFString &)
	throw (Error <INVALID_NAME>,
          Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	void
	removeExternProtoDeclaration (const std::string & name)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ externProtos .erase (name); }

	void
	updateExternProtoDeclaration (const std::string &, const X3DSFNode <ExternProto> &)
	throw (Error <INVALID_NAME>,
          Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const X3DSFNode <ExternProto> &
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

	///  @name Root node handling

	MFNode &
	getRootNodes ()
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return rootNodes; }

	const MFNode &
	getRootNodes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return rootNodes; }

	///  @name Dynamic route node handling

	const X3DSFNode <Route> &
	addRoute (const SFNode &, const std::string &,
	          const SFNode &, const std::string &)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>,
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
	deleteRoute (Route*)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	const RouteArray &
	getRoutes () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return routes; }

	///  @name Viewpoint handling

	void
	changeViewpoint (const std::string &)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Input/Output

	void
	toStream (std::ostream &) const;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DExecutionContext ();


protected:

	///  @name Construction

	X3DExecutionContext ();

	virtual
	void
	initialize () override;

	///  @name Import handling

	void
	cloneExternProtos (const X3DExecutionContext* const);

	void
	copyExternProtos (const X3DExecutionContext* const);

	void
	cloneProtos (const X3DExecutionContext* const);

	void
	copyProtos (const X3DExecutionContext* const)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	void
	copyRootNodes (const X3DExecutionContext* const)
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>);

	void
	copyImportedNodes (const X3DExecutionContext* const);

	void
	copyRoutes (const X3DExecutionContext* const);


private:

	X3DSFNode <Proto>
	createProtoDeclaration (const std::string &, const FieldDefinitionArray &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	X3DSFNode <ExternProto>
	createExternProtoDeclaration (const std::string &, const FieldDefinitionArray &, const MFString &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	RouteId
	getRouteId (const SFNode &, const std::string &,
	            const SFNode &, const std::string &)
	throw (Error <INVALID_NODE>,
	       Error <INVALID_FIELD>);

	typedef std::map <X3DBaseNode*, std::string> ImportedNamesIndex;

	basic::uri  worldURL;
	std::string encoding;
	std::string specificationVersion;
	std::string characterEncoding;
	std::string comment;

	ComponentInfoArray components;
	const ProfileInfo* profile;

	NamedNodeIndex     namedNodes;
	ImportedNodeArray  importedNodes;
	ImportedNamesIndex importedNames;
	ProtoArray         protos;
	ExternProtoArray   externProtos;
	RouteArray         routes;
	MFNode             rootNodes;

	MFNode uninitializedNodes;
};

} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_COMPONENTS_CORE_X3DPROTOTYPE_INSTANCE_H__
#define __TITANIA_X3D_COMPONENTS_CORE_X3DPROTOTYPE_INSTANCE_H__

#include "../../Execution/X3DExecutionContext.h"
#include "../../Prototype/ProtoDeclaration.h"
#include "../Core/X3DNode.h"

namespace titania {
namespace X3D {

class X3DProtoDeclarationNode;

class X3DPrototypeInstance :
	public X3DNode, public X3DExecutionContext
{
public:

	using X3DNode::getField;

	///  @name Construction

	X3DPrototypeInstance (X3DExecutionContext* const, const X3DProtoDeclarationNodePtr & protoNode);

	virtual
	X3DPrototypeInstance*
	create (X3DExecutionContext* const executionContext) const final override;

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	///  throws Error <DISPOSED>
	virtual
	const std::string &
	getTypeName () const final override;

	const SFTime &
	typeName_changed () const
	{ return typeNameOutput; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  throws Error <DISPOSED>
	virtual
	const ProtoDeclaration*
	getInterfaceDeclaration () const final override
	{ return protoNode -> getProtoDeclaration (); }

	///  @name Fields

	virtual
	SFNode &
	metadata ();

	virtual
	const SFNode &
	metadata () const;

	///  @name Member access

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const basic::uri &
	getWorldURL () const final override
	{ return protoNode -> getProtoDeclaration () -> getWorldURL (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	EncodingType
	getEncoding () const final override
	{ return protoNode -> getProtoDeclaration () -> getEncoding (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	SpecificationVersionType
	getSpecificationVersion () const final override
	{ return protoNode -> getProtoDeclaration () -> getSpecificationVersion (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const std::string &
	getCharacterEncoding () const final override
	{ return protoNode -> getProtoDeclaration () -> getCharacterEncoding (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const std::string &
	getComment () const final override
	{ return protoNode -> getProtoDeclaration () -> getComment (); }

	///  @name Profile/Component handling

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const ProfileInfoPtr &
	getProfile ()  const final override
	{ return protoNode -> getProtoDeclaration () -> getProfile (); }

	///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	const ComponentInfoArray &
	getComponents () const final override
	{ return protoNode -> getProtoDeclaration () -> getComponents (); }

	///  @name Unit handling

	///  throws Error <DISPOSED>
	virtual
	long double
	fromUnit (const UnitCategory category, const long double value) const final override
	{ return protoNode -> getProtoDeclaration () -> fromUnit (category, value); }

	///  throws Error <DISPOSED>
	virtual
	long double
	toUnit (const UnitCategory category, const long double value) const final override
	{ return protoNode -> getProtoDeclaration () -> toUnit (category, value); }

	///  @name Member access

	///  throws Error <DISPOSED>
	virtual
	X3DBaseNode*
	getInnerNode () final override;

	///  throws Error <DISPOSED>
	virtual
	const X3DBaseNode*
	getInnerNode () const final override;

	///  throws Error <DISPOSED>
	X3DBaseNode*
	getRootNode () const;

	const X3DProtoDeclarationNodePtr &
	getProtoDeclarationNode () const
	{ return protoNode; }

	void
	setProtoDeclarationNode (const X3DProtoDeclarationNodePtr & value);

	///  throws Error <INVALID_NAME>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	X3DProtoDeclarationNode*
	findProtoDeclaration (const std::string & name, const AvailableType & available) const final override;

	///  @name Event handling

	virtual
	SFBool &
	isLive () final override
	{ return live; }

	virtual
	const SFBool &
	isLive () const final override
	{ return live; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool = false) final override;

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream & ostream) const final override;

	virtual
	void
	toXMLStream (std::ostream & ostream) const final override;

	virtual
	void
	toJSONStream (std::ostream & ostream) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;


protected:

	///  @name Friends

	friend class X3DProtoDeclarationNode;

	///  @name Construction

	void
	update ();


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	construct ();

	///  @name Event handlers

	void
	set_live ();

	///  @name Member access

	bool
	hasTool () const;

	///  @name Static members

	static const std::string componentName;
	static const std::string containerField;

	///  @name Members

	X3DProtoDeclarationNodePtr                          protoNode;
	SFTime                                              typeNameOutput;
	SFBool                                              live;
	std::map <X3DFieldDefinition*, X3DFieldDefinition*> fieldMappings;

};

} // X3D
} // titania

#endif

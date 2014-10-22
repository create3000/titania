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

	///  @name Construction

	X3DPrototypeInstance (X3DExecutionContext* const, const X3DProtoDeclarationNodePtr &);

	virtual
	X3DPrototypeInstance*
	create (X3DExecutionContext* const) const final override;

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override;

	virtual
	const X3DBaseNode*
	getDeclaration () const
	throw (Error <DISPOSED>) final override;

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Member access

	virtual
	std::string
	getTitle () const
	throw (Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getTitle (); }

	virtual
	const basic::uri &
	getWorldURL () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getWorldURL (); }

	virtual
	const std::string &
	getEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getEncoding (); }

	virtual
	SpecificationVersionType
	getSpecificationVersion () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getSpecificationVersion (); }

	virtual
	const std::string &
	getCharacterEncoding () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getCharacterEncoding (); }

	virtual
	const std::string &
	getComment () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getComment (); }

	///  @name Profile/Component handling

	virtual
	const ProfileInfoPtr &
	getProfile ()  const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getProfile (); }

	virtual
	void
	updateComponent (const ComponentInfoPtr & component)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> updateComponent (component); }

	virtual
	const ComponentInfoArray &
	getComponents () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getComponents (); }

	///  @name Unit handling

	virtual
	const UnitArray &
	getUnits () const
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ return protoNode -> getProtoDeclaration () -> getUnits (); }

	///  @name Member access

	virtual
	X3DBaseNode*
	getInnerNode ()
	throw (Error <DISPOSED>) final override;

	virtual
	const X3DBaseNode*
	getInnerNode () const
	throw (Error <DISPOSED>) final override;

	X3DBaseNode*
	getRootNode () const
	throw (Error <DISPOSED>);

	const X3DProtoDeclarationNodePtr &
	getProtoNode () const
	{ return protoNode; }

	virtual
	X3DProtoDeclarationNode*
	findProtoDeclaration (const std::string &, const AvailableType &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

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
	traverse (const TraverseType) final override;

	virtual
	void
	addTool () final override;

	virtual
	void
	removeTool (const bool = false) final override;

	///  @name Input/Output

	virtual
	void
	isCompressed (const bool value) final override
	{ protoNode -> getProtoDeclaration () -> isCompressed (value); }

	virtual
	bool
	isCompressed () const final override
	{ return protoNode -> getProtoDeclaration () -> isCompressed (); }

	virtual
	void
	toStream (std::ostream &) const final override;

	virtual
	void
	toXMLStream (std::ostream &) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_live ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   containerField;

	///  @name Members

	X3DProtoDeclarationNodePtr protoNode;
	SFBool                     live;

};

} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_PROTOTYPE_EXTERN_PROTO_H__
#define __TITANIA_X3D_PROTOTYPE_EXTERN_PROTO_H__

#include "../Components/Networking/X3DUrlObject.h"
#include "../Execution/X3DScene.h"
#include "../Fields.h"
#include "../Prototype/ProtoDeclaration.h"

namespace titania {
namespace X3D {

class SceneFuture;

class ExternProtoDeclaration :
	public X3DProtoDeclarationNode, public X3DUrlObject
{
public:

	using X3DProtoDeclarationNode::createInstance;

	///  @name Construction

	ExternProtoDeclaration (X3DExecutionContext* const executionContext);

	virtual
	ExternProtoDeclaration*
	create (X3DExecutionContext* const executionContext) const final override;

	virtual
	ExternProtoDeclaration*
	copy (const CopyType type) const final override
	{ return copy (getExecutionContext (), type); }

	virtual
	ExternProtoDeclaration*
	copy (X3DExecutionContext* const, const CopyType) const final override;
	
	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Test

	///  throws Error <DISPOSED>
	virtual
	bool
	isExternproto () const final override
	{ return true; }

	///  @name Member access

	///  throws Error <DISPOSED>
	virtual
	ProtoDeclaration*
	getProtoDeclaration () final override;

	const X3DScenePtr &
	getInternalScene () const
	{ return scene; }

	///  @name Instances handling

	virtual
	X3DPrototypeInstance*
	createInstance (X3DExecutionContext* const executionContext) final override;

	///  @name Load handling

	virtual
	void
	requestImmediateLoad () final override;

	virtual
	void
	requestAsyncLoad () final override;

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const final override
	{ return X3DUrlObject::checkLoadState (); }

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream &) const final override;

	virtual
	void
	toXMLStream (std::ostream &) const final override;

	virtual
	void
	toJSONStream (std::ostream &) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~ExternProtoDeclaration () final override;


protected:

	///  @name Instance handling

	virtual
	void
	addInstance (X3DPrototypeInstance* const instance) final override;

	virtual
	void
	removeInstance (X3DPrototypeInstance* const instance) final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	setProtoDeclaration (ProtoDeclaration*);

	void
	setSceneAsync (X3DScenePtr &&);

	void
	setScene (X3DScenePtr && value);

	///  @name Event handlers

	void
	set_live ();

	void
	set_url ();

	///  @name Input/Output

	void
	toStreamField (std::ostream &, X3DFieldDefinition* const, size_t, size_t) const;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	X3DScenePtr          scene;
	ProtoDeclarationPtr  protoDeclaration;
	X3DPtr <SceneFuture> future;

};

} // X3D
} // titania

#endif

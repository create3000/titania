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

#ifndef __TITANIA_X3D_PROTOTYPE_X3DPROTO_DECLARATION_NODE_H__
#define __TITANIA_X3D_PROTOTYPE_X3DPROTO_DECLARATION_NODE_H__

#include "../Components/Core/X3DNode.h"

namespace titania {
namespace X3D {

class ProtoDeclaration;
class X3DPrototypeInstance;

class X3DProtoDeclarationNode :
	virtual public X3DNode
{
public:

	///  @name Tests

	///  throws Error <DISPOSED>
	virtual
	bool
	isExternproto () const = 0;

	///  @name Member access

	virtual
	bool
	canUserDefinedFields () const final override
	{ return true; }

	///  throws Error <DISPOSED>
	virtual
	ProtoDeclaration*
	getProtoDeclaration () = 0;

	///  @name Instances handling

	X3DPrototypeInstance*
	createInstance ()
	{ return createInstance (getExecutionContext ()); }

	virtual
	X3DPrototypeInstance*
	createInstance (X3DExecutionContext* const executionContext) = 0;

	std::set <X3DPrototypeInstance*>
	getInstances () const
	{ return instances; }

	void
	requestUpdateInstances ();

	///  @name Load handling

	virtual
	void
	requestImmediateLoad () = 0;

	virtual
	void
	requestAsyncLoad () = 0;

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const = 0;

	///  @name Comment handling

	void
	addInterfaceComments (const std::vector <std::string> & value)
	{ comments .insert (comments .end (), value .cbegin (), value .cend ()); }

	const std::vector <std::string> &
	getInterfaceComments () const
	{ return comments; }


protected:

	friend class ExternProtoDeclaration;
	friend class X3DEditor;
	friend class X3DPrototypeInstance;

	///  @name Construction

	X3DProtoDeclarationNode ();

	virtual
	void
	initialize () override;

	///  @name Instance handling

	virtual
	void
	addInstance (X3DPrototypeInstance* const instance)
	{ instances .emplace (instance); }

	virtual
	void
	removeInstance (X3DPrototypeInstance* const instance)
	{ instances .erase (instance); }

	void
	updateInstances ();


private:

	///  @name Event handlers

	void
	set_fields ();

	///  @name Members

	SFTime                           updateInstancesBuffer;
	std::set <X3DPrototypeInstance*> instances;
	std::vector <std::string>        comments;

};

} // X3D
} // titania

#endif

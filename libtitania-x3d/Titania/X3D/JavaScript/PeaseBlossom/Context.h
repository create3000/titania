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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_CONTEXT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_CONTEXT_H__

#include "../../Components/Scripting/Script.h"
#include "../X3DJavaScriptContext.h"
#include "ObjectType.h"

#include <Titania/PeaseBlossom/pb.h>

namespace titania {
namespace X3D {
namespace peaseblossom {

class Context :
	public X3D::X3DJavaScriptContext
{
public:

	///  @name Construction

	///  throws std::exception
	Context (X3D::Script* const, const std::string &, const basic::uri &);

	virtual
	X3DBaseNode*
	create (X3D::X3DExecutionContext* const) const final override;

	virtual
	void
	setExecutionContext (X3D::X3DExecutionContext* const) final override;

	///  @name Common members

	///  throws Error <DISPOSED>
	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Member access
	
	const pb::ptr <pb::pbExecutionContext> &
	getProgram () const
	{ return program; }

	///  throws std::out_of_range
	const pb::ptr <pb::NativeFunction> &
	getClass (const ObjectType type) const;

	///  throws std::invalid_argument
	void
	addObject (X3D::X3DChildObject* const, X3D::X3DFieldDefinition* const, pb::pbObject* const);

	void
	removeObject (X3D::X3DChildObject* const, X3D::X3DFieldDefinition* const)
	noexcept (true);

	pb::pbObject*
	getObject (X3DChildObject* const field) const
	noexcept (true);

	X3DPtr <X3D::SceneFuture> &
	getFuture ()
	{ return future; }

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~Context () final override;


private:

	///  @name Operations

	void
	addClasses ();

	void
	addUserDefinedFields ();

	void
	defineProperty (pb::ptr <pb::pbObject> const, X3DFieldDefinition* const, const std::string &, const size_t);

	bool
	resolve (pb::pbObject* const, const pb::Identifier &);

	pb::var
	setProperty (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, X3D::X3DFieldDefinition* const);

	pb::var
	getBuildInProperty (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, X3D::X3DFieldDefinition* const);

	pb::var
	getProperty (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, const pb::var &);

	/// Event handlers

	virtual
	void
	initialize () final override;

	void
	set_live ();

	void
	prepareEvents ();

	void
	set_field (X3D::X3DFieldDefinition* const, const pb::Identifier &);

	void
	eventsProcessed ();

	void
	finish ();

	void
	set_shutdown ();

	void
	connectEventsProcessed ();

	void
	setError (const pb::pbError &) const;

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;
	
	///  @name Members

	std::vector <basic::uri>                           worldURL;
	pb::ptr <pb::pbExecutionContext>                   program;
	pb::Callbacks                                      callbacks;
	mutable std::vector <pb::ptr <pb::NativeFunction>> classes;
	std::map <X3D::X3DChildObject*, pb::pbObject*>     objects;
	std::vector <pb::var>                              values;
	X3DPtr <X3D::SceneFuture>                          future;

};

} // peaseblossom
} // X3D
} // titania

#endif

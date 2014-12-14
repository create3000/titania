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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_V8_V8CONTEXT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_V8_V8CONTEXT_H__

#include <v8.h>

#include "../../Components/Scripting/Script.h"
#include "../X3DJavaScriptContext.h"
#include "ObjectType.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

class Context :
	public X3D::X3DJavaScriptContext
{
public:

	///  @name Construction

	Context (Script* const, const std::string &, const basic::uri &);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Member access

	///  @name Member access

	const v8::Persistent <v8::FunctionTemplate> &
	getClass (const ObjectType type)
	throw (std::out_of_range)
	{ return classes .at (size_t (type)); }

	v8::Local <v8::Object>
	createObject (const ObjectType, X3D::X3DFieldDefinition* const) const
	throw (std::out_of_range);

	void
	addObject (X3D::X3DFieldDefinition* const, const v8::Persistent <v8::Object> &)
	throw (Error <INVALID_FIELD>);

	void
	removeObject (X3D::X3DFieldDefinition* const);

	v8::Local <v8::Object>
	getObject (X3D::X3DFieldDefinition* const) const
	throw (std::out_of_range);

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~Context ();


private:

	///  @name Operations

	void
	setContext ();

	void
	setFields ();

	void
	addClass (const ObjectType, const v8::Local <v8::FunctionTemplate> &);

	v8::Local <v8::Function>
	getFunction (const std::string &) const;

	/// Event handlers

	virtual
	void
	initialize () final override;

	void
	setEventHandler ();

	void
	set_live ();

	void
	prepareEvents ();

	void
	set_field (X3D::X3DFieldDefinition*);

	void
	eventsProcessed ();

	void
	finish ();

	void
	set_shutdown ();

	void
	error (const v8::TryCatch &) const;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	std::vector <basic::uri> worldURL;

	v8::Isolate*                 isolate;
	v8::Persistent <v8::Context> context;
	v8::Persistent <v8::Script>  program;

	std::vector <v8::Persistent <v8::FunctionTemplate>>              classes;
	std::map <X3D::X3DFieldDefinition*, v8::Persistent <v8::Object>> objects;
	std::map <X3DFieldDefinition*, v8::Persistent <v8::Function>>    functions;

	v8::Persistent <v8::Function> initializeFn;
	v8::Persistent <v8::Function> prepareEventsFn;
	v8::Persistent <v8::Function> eventsProcessedFn;
	v8::Persistent <v8::Function> shutdownFn;

};

inline
Context*
getContext (const v8::AccessorInfo & info)
{
	return static_cast <Context*> (v8::Handle <v8::External>::Cast (info .Data ()) -> Value ());
}

inline
Context*
getContext (const v8::Arguments & args)
{
	return static_cast <Context*> (v8::Handle <v8::External>::Cast (args .Data ()) -> Value ());
}

} // GoogleV8
} // X3D
} // titania

#endif

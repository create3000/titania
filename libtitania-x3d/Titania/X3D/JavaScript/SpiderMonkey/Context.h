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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_CONTEXT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_CONTEXT_H__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include "../../Components/Networking/X3DUrlObject.h"
#include "../../Components/Scripting/Script.h"
#include "../X3DJavaScriptContext.h"
#include "ObjectType.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#include <jsapi.h>
#pragma GCC diagnostic pop

#include <map>
#include <mutex>

namespace titania {
namespace X3D {
namespace spidermonkey {

class Context :
	public X3DJavaScriptContext
{
public:

	///  @name Construction

	Context (X3D::Script* const, const std::string &, const basic::uri &);

	virtual
	X3DBaseNode*
	create (X3D::X3DExecutionContext* const) const final override;

	virtual
	void
	setExecutionContext (X3D::X3DExecutionContext* const) final override;

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

	///  @name Member access

	JSObject*
	getGlobal () const
	{ return global; }

	JSObject*
	getProto (const ObjectType type) const
	{ return protos [size_t (type)]; }

	void
	addObject (X3D::X3DChildObject* const key, X3D::X3DFieldDefinition* const field, JSObject* const object);

	void
	removeObject (X3D::X3DChildObject* const key, X3D::X3DFieldDefinition* const field);

	JSObject*
	getObject (X3D::X3DChildObject* const key) const;

	void
	setReference (X3D::X3DFieldDefinition* const array, const size_t index, X3D::X3DFieldDefinition* const reference);

	void
	removeReference (X3D::X3DFieldDefinition* const array, const size_t index);

	X3D::X3DFieldDefinition*
	getReference (X3D::X3DFieldDefinition* const array, const size_t index);

	X3DPtr <X3D::SceneFuture> &
	getFuture ()
	{ return future; }

	///  @name Functions

	JSBool
	require (const basic::uri &, jsval &);

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~Context () final override;


private:

	///  throws std::runtime_error
	void
	addClasses ();

	void
	addProto (const ObjectType type, JSObject* const proto)
	{ protos [size_t (type)] = proto; }

	void
	addUserDefinedFields ();

	void
	addUserDefinedField (X3D::X3DFieldDefinition* const);

	void
	defineProperty (JSObject* const, X3D::X3DFieldDefinition* const, const std::string &, const uint32_t);

	JSBool
	evaluate (const std::string &, const std::string &);

	JSBool
	evaluate (const std::string &, const std::string &, jsval &);

	static
	JSBool
	setProperty (JSContext*, JSObject*, jsid, JSBool, jsval*);

	static
	JSBool
	getBuildInProperty (JSContext*, JSObject*, jsid, jsval*);

	static
	JSBool
	getProperty (JSContext*, JSObject*, jsid, jsval*);

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
	set_field (X3D::X3DFieldDefinition* const, const jsval &);

	void
	eventsProcessed ();

	void
	set_shutdown ();

	void
	finish ();

	jsval
	getFunction (const std::string &) const;

	void
	callFunction (const std::string &) const;

	void
	callFunction (jsval) const;

	void
	connectEventsProcessed ();

	static
	void
	error (JSContext*, const char*, JSErrorReport*);

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	static JSClass globalClass;

	///  @name Members

	JSRuntime*               rt;
	JSContext*               cx;
	JSObject*                global;
	std::vector <basic::uri> worldURL;

	jsval initializeFn;
	jsval prepareEventsFn;
	jsval eventsProcessedFn;

	std::vector <JSObject*>                                         protos;
	std::map <std::string, jsval>                                   fields;
	std::map <X3D::X3DFieldDefinition*, jsval>                      functions;
	std::map <size_t, JSObject*>                                    objects;
	std::map <std::pair <size_t, size_t>, X3D::X3DFieldDefinition*> references;
	std::map <basic::uri, jsval>                                    files;
	std::map <X3D::X3DFieldDefinition*, time_type>                  callbackTimes;

	X3DPtr <X3D::SceneFuture> future;

	size_t frame;

};

inline
Context*
getContext (JSContext* const cx)
{
	return static_cast <Context*> (JS_GetContextPrivate (cx));
}

} // spidermonkey
} // X3D
} // titania

#pragma GCC diagnostic pop

#endif

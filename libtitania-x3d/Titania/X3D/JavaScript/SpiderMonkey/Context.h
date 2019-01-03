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

#include "../../Components/Scripting/Script.h"
#include "../X3DJavaScriptContext.h"
#include "ObjectType.h"

#include <jsapi.h>
#include <thread>

namespace titania {
namespace X3D {
namespace spidermonkey {

class Context :
	public X3D::X3DJavaScriptContext
{
public:

	///  @name Construction

	///  throws std::exception
	Context (JSContext* const cx, X3D::Script* const, const std::string &, const basic::uri &);

	virtual
	X3DBaseNode*
	create (X3D::X3DExecutionContext* const) const final override;

	virtual
	void
	setExecutionContext (X3D::X3DExecutionContext* const) final override;

	///  @name Common members

	///  throws Error <DISPOSED>
	virtual
	X3D::ComponentType
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
	
	JS::HandleObject
	getGlobal () const
	{ return *global; }

	JS::HandleObject
	getProto (const ObjectType type) const
	{ return *protos [size_t (type)]; }

	void
	addObject (const size_t key, X3D::X3DFieldDefinition* const field, JSObject* const obj);

	void
	removeObject (const size_t key);

	JSObject*
	getObject (const size_t key) const;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~Context () final override;


protected:

	virtual
	void
	initialize () final override;


private:

	///  @name Operations

	void
	addClasses ();
	
	void
	addUserDefinedFields ();

	void
	defineProperty (JS::HandleObject obj,
	                X3D::X3DFieldDefinition* const field,
	                const std::string & name,
	                const uint32_t attrs);
	
	static
	bool
	setProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp, JS::ObjectOpResult& result);

	static
	bool
	getBuildInProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp);
	
	static
	bool
	getProperty (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp);

	bool
	evaluate (const std::string & string, const std::string & filename);

	void
	call (const std::string & value);

	void
	call (JS::HandleValue value);

	bool
	getFunction (const std::string & name, JS::MutableHandleValue value) const;

	void
	addProto (const ObjectType type, JSObject* const proto);

	void
	set_live ();

	void
	prepareEvents (const std::shared_ptr <JS::PersistentRooted <JS::Value>> & functionValue);

	void
	set_field (X3D::X3DFieldDefinition* const field, const std::shared_ptr <JS::PersistentRooted <JS::Value>> & inputFunction);

	void
	eventsProcessed (const std::shared_ptr <JS::PersistentRooted <JS::Value>> & functionValue);

	void
	finish ();

	void
	set_shutdown ();

	void
	reportException ();

	static
	void
	reportError (JSContext* cx, JSErrorReport* const report);

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	static const JSClassOps globalOps;
	static const JSClass    globalClass;

	///  @name Member types

	using Objects = std::map <size_t, std::pair <X3D::X3DFieldDefinition*, JSObject*>>;

	///  @name Members

	basic::uri                                                                 worldURL;
	JSContext* const                                                           cx;
	std::unique_ptr <JS::PersistentRooted <JSObject*>>                         global;
	std::map <std::string, std::unique_ptr <JS::PersistentRooted <JS::Value>>> fields;
	std::vector <std::unique_ptr <JS::PersistentRooted <JSObject*>>>           protos;
	Objects                                                                    objects;

};

} // spidermonkey
} // X3D
} // titania

#endif

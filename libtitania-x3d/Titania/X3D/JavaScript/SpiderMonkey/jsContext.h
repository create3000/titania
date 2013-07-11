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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_CONTEXT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_CONTEXT_H__

#include "../../Components/Networking/X3DUrlObject.h"
#include "../../Components/Scripting/X3DScriptNode.h"
#include "../X3DJavaScriptContext.h"
#include <map>
#include <map>

#include <jsapi.h>

namespace titania {
namespace X3D {

class jsContext :
	public X3DJavaScriptContext, public X3DUrlObject
{
public:

	using X3DJavaScriptContext::addField;
	using X3DJavaScriptContext::removeField;

	jsContext (X3DScriptNode*, const std::string &, const basic::uri &, size_t);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	X3DScriptNode*
	getNode () const
	{ return script; }

	JSBool
	require (const basic::uri &, jsval &);

	JSObject*
	getGlobal () const
	{ return global; }

	jsval
	getFunction (const std::string &) const;

	virtual
	void
	set_initialized () final;

	virtual
	void
	eventsProcessed () final;

	void
	addObject (X3DFieldDefinition*, JSObject*);

	void
	removeObject (X3DFieldDefinition*);

	JSObject*
	getObject (X3DFieldDefinition*);

	virtual
	void
	dispose () final;

	virtual
	~jsContext ();


private:

	virtual
	void
	initialize () final;

	void
	initContext ();

	void
	initNode ();

	void
	addUserDefinedField (X3DFieldDefinition* const);

	static
	void
	defineProperty (JSContext *, JSObject *, X3DFieldDefinition * const, const std::string &, uintN);

	void
	initEventHandler ();

	JSBool
	evaluate (const std::string &, const std::string &);

	JSBool
	evaluate (const std::string &, const std::string &, jsval &);

	static JSBool getBuildInProperty (JSContext *, JSObject *, jsid, jsval*);
	static JSBool getProperty        (JSContext *, JSObject *, jsid, jsval*);
	static JSBool setProperty        (JSContext *, JSObject *, jsid, JSBool, jsval*);

	void
	set_field (X3DFieldDefinition*);

	void
	prepareEvents ();

	void
	shutdown ();

	void
	callFunction (const std::string &) const;

	void
	callFunction (jsval) const;

	static
	void
	error (JSContext* context, const char* message, JSErrorReport* report);

	virtual
	void
	addEvent (X3DChildObject*) final
	{ }

	virtual
	void
	addEvent (X3DChildObject*, const Event &) final
	{ }

	static JSClass global_class;

	JSRuntime*              runtime;
	JSContext*              context;
	JSObject*               global;
	X3DScriptNode*          script;
	std::deque <basic::uri> worldURL;
	size_t                  index;

	jsval initializeFn;
	jsval prepareEventsFn;
	jsval eventsProcessedFn;
	jsval shutdownFn;

	std::map <std::string, jsval>             fields;
	std::map <X3DFieldDefinition*, jsval>     functions;
	std::map <X3DFieldDefinition*, JSObject*> objects;
	std::map <basic::uri, jsval>              files;

};

} // X3D
} // titania

#endif

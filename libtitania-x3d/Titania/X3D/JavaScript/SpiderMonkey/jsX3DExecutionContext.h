/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DEXECUTION_CONTEXT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DEXECUTION_CONTEXT_H__

#include "../../Execution/X3DExecutionContext.h"
#include <jsapi.h>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

class jsX3DExecutionContext
{
public:

	static
	JSObject*
	init (JSContext* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, X3DExecutionContext* const, jsval* const);

	static
	JSClass*
	getClass ()
	{ return &static_class; }

	static JSBool addRoute (JSContext *, uintN, jsval*);


protected:

	enum Property
	{
		SPECIFICATION_VERSION,
		ENCODING,
		WORLD_URL,
		PROFILE,
		COMPONENTS,
		EXTERNPROTOS,
		PROTOS,
		ROOT_NODES,
		ROUTES
	};

	static JSBool specificationVersion (JSContext* context, JSObject* obj, jsid id, jsval* vp);
	static JSBool encoding             (JSContext* context, JSObject* obj, jsid id, jsval* vp);
	static JSBool worldURL             (JSContext* context, JSObject* obj, jsid id, jsval* vp);
	static JSBool profile              (JSContext* context, JSObject* obj, jsid id, jsval* vp);
	static JSBool components           (JSContext* context, JSObject* obj, jsid id, jsval* vp);

	static JSBool externprotos (JSContext *, JSObject *, jsid, jsval*);
	static JSBool protos       (JSContext *, JSObject *, jsid, jsval*);
	static JSBool rootNodes    (JSContext *, JSObject *, jsid, jsval*);
	static JSBool routes       (JSContext *, JSObject *, jsid, jsval*);

	static JSBool createNode  (JSContext *, uintN, jsval*);
	static JSBool createProto (JSContext *, uintN, jsval*);

	static JSBool addNamedNode    (JSContext *, uintN, jsval*);
	static JSBool removeNamedNode (JSContext *, uintN, jsval*);
	static JSBool updateNamedNode (JSContext *, uintN, jsval*);
	static JSBool getNamedNode    (JSContext *, uintN, jsval*);

	static JSBool addImportedNode    (JSContext *, uintN, jsval*);
	static JSBool removeImportedNode (JSContext *, uintN, jsval*);
	static JSBool updateImportedNode (JSContext *, uintN, jsval*);
	static JSBool getImportedNode    (JSContext *, uintN, jsval*);

	static JSBool deleteRoute (JSContext *, uintN, jsval*);

	static JSBool toVRMLString (JSContext *, uintN, jsval*);
	static JSBool toXMLString  (JSContext *, uintN, jsval*);

private:

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

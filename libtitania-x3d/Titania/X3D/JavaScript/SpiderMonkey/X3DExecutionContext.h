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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DEXECUTION_CONTEXT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DEXECUTION_CONTEXT_H__

#include "../../Execution/X3DExecutionContext.h"
#include "ObjectType.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#include <js/jsapi.h>
#pragma GCC diagnostic pop

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DExecutionContext
{
public:

	///  @name Member types

	using internal_type = X3D::X3DExecutionContext;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, X3D::X3DExecutionContext* const, jsval* const);

	static
	JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr ObjectType
	getId ()
	{ return ObjectType::X3DExecutionContext; }


protected:

	///  @name Member types

	enum Property
	{
		LENGTH,
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

	///  @name Properties

	static JSBool set1Value (JSContext*, JSObject*, jsid, JSBool, jsval*);
	static JSBool get1Value (JSContext*, JSObject*, jsid, jsval*);
	static JSBool length    (JSContext*, JSObject*, jsid, JSBool, jsval*);
	static JSBool length    (JSContext*, JSObject*, jsid, jsval*);

	static JSBool specificationVersion (JSContext* cx, JSObject* obj, jsid id, jsval* vp);
	static JSBool encoding             (JSContext* cx, JSObject* obj, jsid id, jsval* vp);
	static JSBool worldURL             (JSContext* cx, JSObject* obj, jsid id, jsval* vp);
	static JSBool profile              (JSContext* cx, JSObject* obj, jsid id, jsval* vp);
	static JSBool components           (JSContext* cx, JSObject* obj, jsid id, jsval* vp);

	static JSBool externprotos (JSContext*, JSObject*, jsid, jsval*);
	static JSBool protos       (JSContext*, JSObject*, jsid, jsval*);
	static JSBool rootNodes    (JSContext*, JSObject*, jsid, jsval*);
	static JSBool routes       (JSContext*, JSObject*, jsid, jsval*);

	///  @name Functions

	static JSBool createNode  (JSContext*, uint32_t, jsval*);
	static JSBool createProto (JSContext*, uint32_t, jsval*);

	static JSBool addNamedNode    (JSContext*, uint32_t, jsval*);
	static JSBool removeNamedNode (JSContext*, uint32_t, jsval*);
	static JSBool updateNamedNode (JSContext*, uint32_t, jsval*);
	static JSBool getNamedNode    (JSContext*, uint32_t, jsval*);

	static JSBool addImportedNode    (JSContext*, uint32_t, jsval*);
	static JSBool removeImportedNode (JSContext*, uint32_t, jsval*);
	static JSBool updateImportedNode (JSContext*, uint32_t, jsval*);
	static JSBool getImportedNode    (JSContext*, uint32_t, jsval*);

	static JSBool getRootNodes (JSContext*, uint32_t, jsval*);

	static JSBool addRoute (JSContext*, uint32_t, jsval*);
	static JSBool deleteRoute (JSContext*, uint32_t, jsval*);

	static JSBool toVRMLString (JSContext*, uint32_t, jsval*);
	static JSBool toXMLString  (JSContext*, uint32_t, jsval*);

	///  @name Static members

	static const std::set <int32_t> unitCategories;


private:

	///  @name Functions

	static JSBool fromUnit (JSContext *, uint32_t, jsval*);
	static JSBool toUnit   (JSContext *, uint32_t, jsval*);

	///  @name Static members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

} // spidermonkey
} // X3D
} // titania

#endif

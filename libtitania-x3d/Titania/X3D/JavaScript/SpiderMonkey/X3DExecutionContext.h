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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DEXECUTION_CONTEXT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DEXECUTION_CONTEXT_H__

#include "ObjectType.h"

#include "../../Execution/X3DExecutionContext.h"

#include <jsapi.h>

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DExecutionContext
{
public:

	///  @name Member types

	using internal_type = X3D::X3DExecutionContextPtr;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JS::Value
	create (JSContext* const cx, X3D::X3DExecutionContext* const executionContext);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ return ObjectType::X3DExecutionContext; }


private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Properties

	static bool getSpecificationVersion (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getEncoding             (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getWorldURL             (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getProfile              (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getComponents           (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	static bool getExternprotos (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getProtos       (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getRootNodes    (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getRoutes       (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	///  @name Functions
//
//	static bool fromUnit (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool toUnit   (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	static bool createNode  (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool createProto (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	static bool addNamedNode    (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool removeNamedNode (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool updateNamedNode (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getNamedNode    (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	static bool addImportedNode    (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool removeImportedNode (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool updateImportedNode (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool getImportedNode    (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	static bool getRootNodes (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	static bool addRoute (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool deleteRoute (JSContext* cx, unsigned argc, JS::Value* vp);
//
//	static bool toVRMLString (JSContext* cx, unsigned argc, JS::Value* vp);
//	static bool toXMLString  (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Destruction

	static void finalize (JSFreeOp* fop, JSObject* obj);

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

	///  @name Static members

	static const std::set <int32_t> unitCategories;

};

} // spidermonkey
} // X3D
} // titania

#endif

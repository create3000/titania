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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DFIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DFIELD_H__

#include "Arguments.h"
#include "Context.h"
#include "Error.h"
#include "ObjectType.h"
#include "SlotType.h"

#include <jsapi.h>

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DField
{
public:

	///  @name Member types

	using internal_type = X3D::X3DFieldDefinition;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ return ObjectType::X3DField; }


protected:

	///  @name Construction

	template <class Type>
	static
	std::pair <JS::Value, bool>
	create (JSContext* const cx, typename Type::internal_type* const field);

	static
	const X3D::X3DChildObject*
	getKey (const X3D::X3DFieldDefinition* const field)
	{ return field; }

	static
	X3D::X3DFieldDefinition*
	getField (X3D::X3DFieldDefinition* const field)
	{ return field; }

	///  @name Destruction

	template <class Type>
	static
	void
	finalize (JSFreeOp* fop, JSObject* obj);


private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Functions

	static bool getName     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool getTypeName (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool getType     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool isReadable  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool isWritable  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool toString    (JSContext* cx, unsigned argc, JS::Value* vp);

	static bool equals (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool assign (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class Type>
std::pair <JS::Value, bool>
X3DField::create (JSContext* const cx, typename Type::internal_type* const field)
{
	const auto context = getContext (cx);
	const auto object  = context -> getObject (Type::getKey (field));

	if (object)
	{
		return std::pair (JS::ObjectValue (*object), false);
	}
	else
	{
		const auto object = JS_NewObjectWithGivenProto (cx, Type::getClass (), context -> getProto (Type::getId ()));

		if (object == nullptr)
			throw std::runtime_error ("out of memory");

		const auto value = Type::getField (field);

		setObject (object, value);
		setContext (object, context);

		context -> addObject (Type::getKey (field), value, object);

		return std::pair (JS::ObjectValue (*object), true);
	}
}

template <class Type>
void
X3DField::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = getContext (obj);
	const auto field   = getObject <typename Type::internal_type*> (obj);

	// Proto objects have no private.

	if (field)
		context -> removeObject (Type::getKey (field));
}

} // spidermonkey
} // X3D
} // titania

#endif

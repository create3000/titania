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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DCONST_ARRAY_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DCONST_ARRAY_H__

#include "Arguments.h"
#include "Error.h"
#include "ObjectType.h"
#include "SlotType.h"

#include <jsapi.h>

extern "C"
{
	#include "C-bind/bind.h"
}

namespace titania {
namespace X3D {
namespace spidermonkey {

template <class ValueType, class InternalType>
class X3DConstArray
{
public:

	///  @name Member types

	using value_type    = ValueType;
	using internal_type = InternalType;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JS::Value
	create (JSContext* const cx, const typename InternalType::internal_type & array);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr ObjectType
	getId ()
	{ throw std::runtime_error ("getId"); return ObjectType (); }


private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool enumerate (JSContext* cx, JS::HandleObject obj, JS::MutableHandleIdVector properties, bool enumerableOnly);
	static bool resolve   (JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool* resolvedp);

	///  @name Properties

	static bool get1Value (const size_t index, JSContext* cx, unsigned argc, JS::Value* vp);
	static bool getLength (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Destruction

	static void finalize (JSFreeOp* fop, JSObject* obj);

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class ValueType, class InternalType>
const JSClassOps X3DConstArray <ValueType, InternalType>::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // enumerate
	enumerate, // newEnumerate
	resolve, // resolve
	nullptr, // mayResolve
	finalize, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

template <class ValueType, class InternalType>
const JSPropertySpec X3DConstArray <ValueType, InternalType>::properties [ ] = {
	JS_PSGS ("length", getLength, nullptr, JSPROP_PERMANENT),
	JS_PS_END
};

template <class ValueType, class InternalType>
const JSFunctionSpec X3DConstArray <ValueType, InternalType>::functions [ ] = {
	JS_FS_END
};

template <class ValueType, class InternalType>
JSObject*
X3DConstArray <ValueType, InternalType>::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

template <class ValueType, class InternalType>
JS::Value
X3DConstArray <ValueType, InternalType>::create (JSContext* const cx, const typename InternalType::internal_type & array)
{
	const auto context = getContext (cx);
	const auto key     = size_t (array);
	const auto obj     = context -> getObject (key);

	if (obj)
	{
		return JS::ObjectValue (*obj);
	}
	else
	{
		const auto obj = JS_NewObjectWithGivenProto (cx, getClass (), context -> getProto (getId ()));

		if (not obj)
			throw std::runtime_error ("out of memory");

		const auto self = new internal_type (array);

		setObject (obj, self);
		setContext (obj, context);
		setKey (obj, key);

		context -> addObject (key, self, obj);

		return JS::ObjectValue (*obj);
	}
}

template <class ValueType, class InternalType>
bool
X3DConstArray <ValueType, InternalType>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

template <class ValueType, class InternalType>
bool
X3DConstArray <ValueType, InternalType>::enumerate (JSContext* cx, JS::HandleObject obj, JS::MutableHandleIdVector properties, bool enumerableOnly)
{
	try
	{
		const auto & array = *getThis <X3DConstArray> (cx, obj);

		for (size_t i = 0, size = array -> size (); i < size; ++ i)
			properties .append (INT_TO_JSID (i));

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .enumerate: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DConstArray <ValueType, InternalType>::resolve (JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool* resolvedp)
{
	if (JSID_IS_INT (id))
	{
		const int32_t index = JSID_TO_INT (id);

		JS_DefineProperty (cx,
		                   obj,
		                   basic::to_string (index, std::locale::classic ()) .c_str (),
			               JSNative (partial_bind ((void*) &X3DConstArray::get1Value, 4, 1, index)),
		                   nullptr,
		                   JSPROP_RESOLVING);

		*resolvedp = true;
		return true;
	}

	*resolvedp = false;
	return true;
}

template <class ValueType, class InternalType>
bool
X3DConstArray <ValueType, InternalType>::get1Value (const size_t index, JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args  = JS::CallArgsFromVp (argc, vp);
		const auto & array = *getThis <X3DConstArray> (cx, args);

		if (index >= 0 and index < array -> size ())
		{
			args .rval () .set (ValueType::create (cx, (*array) [index]));
		}
		else
		{
			args .rval () .setUndefined ();
		}

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .length: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DConstArray <ValueType, InternalType>::getLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args  = JS::CallArgsFromVp (argc, vp);
		const auto & array = *getThis <X3DConstArray> (cx, args);

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .length: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
void
X3DConstArray <ValueType, InternalType>::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = getContext (obj);
	const auto self    = getObject <internal_type*> (obj);

	// Proto objects have no private.

	if (self)
		context -> removeObject (getKey (obj));
}

} // spidermonkey
} // X3D
} // titania

#endif

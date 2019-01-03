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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_X3DARRAY_FIELD_H__

#include "X3DField.h"
#include "NativeArrayReference.h"

#include "Fields/SFImage.h"
#include "Fields/SFNode.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DArrayField
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
	{ return ObjectType::X3DArrayField; }


private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class ValueType, class InternalType>
class X3DArrayFieldTemplate :
	public X3DField
{
public:

	///  @name Member types

	using value_type    = ValueType;
	using internal_type = InternalType;
	using single_type   = typename ValueType::internal_type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JS::Value
	create (JSContext* const cx, InternalType* const field);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ throw std::runtime_error ("getId"); }

private:

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool enumerate (JSContext* cx, JS::HandleObject obj, JS::AutoIdVector & properties, bool enumerableOnly);
	static bool resolve   (JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool* resolvedp);

	///  @name Member access

	static bool set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp, JS::ObjectOpResult & result);
	static bool get1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp);

	static bool setLength (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool getLength (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Functions

	static bool pop     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool push    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool shift   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool splice  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool unshift (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Member access

	///  throws std::invalid_argument, std::domain_error
	template <class Class>
	static
	std::enable_if_t <
		not (std::is_integral_v <typename Class::internal_type::internal_type> or
		     std::is_floating_point_v <typename Class::internal_type::internal_type> or
		     std::is_same_v <typename Class::internal_type::internal_type, std::string> or
		     std::is_same_v <typename Class::internal_type::internal_type, X3D::String>),
		typename Class::internal_type &
	>
	getArgument (JSContext* const cx, const JS::HandleValue & value, const size_t index)
	{
		return *spidermonkey::getArgument <Class> (cx, value, index);
	}

	///  throws std::invalid_argument, std::domain_error
	template <class Class>
	static
	std::enable_if_t <
		std::is_integral_v <typename Class::internal_type> or
		std::is_floating_point_v <typename Class::internal_type> or
		std::is_same_v <typename Class::internal_type, std::string> or
		std::is_same_v <typename Class::internal_type, X3D::String>,
		typename Class::internal_type
	>
	getArgument (JSContext* const cx, const JS::HandleValue & value, const size_t index)
	{
		return spidermonkey::getArgument <typename Class::internal_type> (cx, value, index);
	}

	///  throws std::invalid_argument, std::domain_error
	template <class Class>
	static
	std::enable_if_t <
		not (std::is_integral_v <typename Class::internal_type::internal_type> or
		     std::is_floating_point_v <typename Class::internal_type::internal_type> or
		     std::is_same_v <typename Class::internal_type::internal_type, std::string> or
		     std::is_same_v <typename Class::internal_type::internal_type, X3D::String>),
		typename Class::internal_type &
	>
	getArgument (JSContext* const cx, const JS::CallArgs & args, const size_t index)
	{
		return *spidermonkey::getArgument <Class> (cx, args, index);
	}

	///  throws std::invalid_argument, std::domain_error
	template <class Class>
	static
	std::enable_if_t <
		std::is_integral_v <typename Class::internal_type> or
		std::is_floating_point_v <typename Class::internal_type> or
		std::is_same_v <typename Class::internal_type, std::string> or
		std::is_same_v <typename Class::internal_type, X3D::String>,
		typename Class::internal_type
	>
	getArgument (JSContext* const cx, const JS::CallArgs & args, const size_t index)
	{
		return spidermonkey::getArgument <typename Class::internal_type> (cx, args, index);
	}

	template <class Class>
	static
	std::enable_if_t <
		std::is_integral_v <typename Class::internal_type> or
		std::is_floating_point_v <typename Class::internal_type> or
		std::is_same_v <typename Class::internal_type, X3D::String>,
		JS::Value
	>
	getValue (JSContext* const cx, const typename Class::internal_type & value)
	{
		return Class::create (cx, value);
	}

	template <class Class>
	static
	std::enable_if_t <
		not (std::is_integral_v <typename Class::internal_type> or
		     std::is_floating_point_v <typename Class::internal_type> or
		     std::is_same_v <typename Class::internal_type, X3D::String> or
		     std::is_same_v <Class, SFNode>),
		JS::Value
	>
	getValue (JSContext* cx, const typename Class::internal_type::internal_type & value)
	{
		return Class::create (cx, new typename Class::internal_type (value));
	}

	template <class Class>
	static
	std::enable_if_t <
		std::is_same_v <Class, SFNode>,
		JS::Value
	>
	getValue (JSContext* cx, const X3D::SFNode & value)
	{
		return SFNode::create (cx, const_cast <X3D::SFNode*> (&value));
	}

	template <class Class>
	static
	std::enable_if_t <
		std::is_integral_v <Class> or
		std::is_floating_point_v <Class> or
		std::is_same_v <Class, X3D::String>,
		JS::Value
	>
	getReference (JSContext* cx, InternalType* const array, const size_t index)
	{
		return ValueType::create (cx, array -> get1Value (index));
	}

	template <class Class>
	static
	std::enable_if_t <
		std::is_same_v <Class, X3D::SFImage> or
		std::is_same_v <Class, X3D::SFNode>,
		JS::Value
	>
	getReference (JSContext* cx, InternalType* const array, const size_t index)
	{
		return ValueType::create (cx, &array -> get1Value (index));
	}

	template <class Class>
	static
	std::enable_if_t <
		not (std::is_integral_v <Class> or
		     std::is_floating_point_v <Class> or
		     std::is_same_v <Class, X3D::String> or
		     std::is_base_of_v <X3D::X3DFieldDefinition, Class>),
		JS::Value
	>
	getReference (JSContext* cx, InternalType* const array, const size_t index)
	{
		return ValueType::create (cx, new NativeArrayReference <InternalType, single_type> (array, index));
	}

	///  @name Static members

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class ValueType, class InternalType>
const JSClassOps X3DArrayFieldTemplate <ValueType, InternalType>::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate, // enumerate
	resolve, // resolve
	nullptr, // mayResolve
	finalize <X3DArrayFieldTemplate>, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

template <class ValueType, class InternalType>
const JSPropertySpec X3DArrayFieldTemplate <ValueType, InternalType>::properties [ ] = {
	JS_PSGS ("length", getLength, setLength, JSPROP_PERMANENT),
	JS_PS_END
};

template <class ValueType, class InternalType>
const JSFunctionSpec X3DArrayFieldTemplate <ValueType, InternalType>::functions [ ] = {
	JS_FS ("pop",     pop,     0, JSPROP_PERMANENT),
	JS_FS ("push",    push,    0, JSPROP_PERMANENT),
	JS_FS ("shift",   shift,   0, JSPROP_PERMANENT),
	JS_FS ("splice",  splice,  2, JSPROP_PERMANENT),
	JS_FS ("unshift", unshift, 0, JSPROP_PERMANENT),
	JS_FS_END
};

template <class ValueType, class InternalType>
JSObject*
X3DArrayFieldTemplate <ValueType, InternalType>::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

template <class ValueType, class InternalType>
JS::Value
X3DArrayFieldTemplate <ValueType, InternalType>::create (JSContext* const cx, InternalType* const field)
{
	return X3DField::create <X3DArrayFieldTemplate> (cx, field) .first;
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc == 0)
		{
			JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new InternalType ()));
			return true;
		}
		else
		{
			const auto args  = JS::CallArgsFromVp (argc, vp);
			const auto array = new InternalType ();
			
			for (unsigned i = 0; i < argc; ++ i)
			{
				array -> emplace_back (getArgument <ValueType> (cx, args, i));
			}

			args .rval () .set (create (cx, array));
			return true;
		}
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::enumerate (JSContext* cx, JS::HandleObject obj, JS::AutoIdVector & properties, bool enumerableOnly)
{
	try
	{
//		__LOG__ << std::endl;

		const auto array = getThis <X3DArrayFieldTemplate> (cx, obj);

//		__LOG__ << array -> getTypeName () << std::endl;
//		__LOG__ << array -> size () << std::endl;

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
X3DArrayFieldTemplate <ValueType, InternalType>::resolve (JSContext* cx, JS::HandleObject obj, JS::HandleId id, bool* resolvedp)
{
	if (JSID_IS_INT (id))
	{
		const int32_t index = JSID_TO_INT (id);

		JS_DefineProperty (cx,
		                   obj,
		                   basic::to_string (index, std::locale::classic ()) .c_str (),
		                   JS::UndefinedHandleValue,
		                   JSPROP_PROPOP_ACCESSORS | JSPROP_RESOLVING,
		                   JS_PROPERTYOP_GETTER (&X3DArrayFieldTemplate::get1Value),
		                   JS_PROPERTYOP_SETTER (&X3DArrayFieldTemplate::set1Value));

		*resolvedp = true;
		return true;
	}

	*resolvedp = false;
	return true;
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp, JS::ObjectOpResult & result)
{
	try
	{
		const auto array = getThis <X3DArrayFieldTemplate> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index >= 0)
			array -> set1Value (index, getArgument <ValueType> (cx, vp, 0));

		result .succeed ();
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: out of memory.", getClass () -> name, JSID_TO_INT (id));
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, JSID_TO_INT (id), error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::get1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	try
	{
		const auto array = getThis <X3DArrayFieldTemplate> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index < 0)
		{
			vp .setUndefined ();
		}
		else
		{
			vp .set (getReference <typename InternalType::value_type> (cx, array, index));
		}

		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: out of memory.", getClass () -> name, JSID_TO_INT (id));
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, JSID_TO_INT (id), error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::setLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);
		const auto size  = spidermonkey::getArgument <uint32_t> (cx, args, 0);

		array -> resize (size);
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::getLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::pop (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .pop: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);

		if (array -> empty ())
		{
			args .rval () .setUndefined ();
			return true;
		}

		const auto & value = const_cast <const InternalType*> (array) -> back ();

		args .rval () .set (getValue <ValueType> (cx, value));

		array -> pop_back ();

		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .pop: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .pop: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::push (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);

		for (unsigned i = 0; i < argc; ++ i)
			array -> emplace_back (getArgument <ValueType> (cx, args, i));

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .push: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .push: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::shift (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .shift: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);

		if (array -> empty ())
		{
			args .rval () .setUndefined ();
			return true;
		}

		const auto & value = const_cast <const InternalType*> (array) -> front ();

		args .rval () .set (getValue <ValueType> (cx, value));

		array -> pop_front ();

		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .shift: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .shift: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::splice (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc < 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .splice: wrong number of arguments.", getClass () -> name);

		const auto args        = JS::CallArgsFromVp (argc, vp);
		const auto array       = getThis <X3DArrayFieldTemplate> (cx, args);
		auto       index       = spidermonkey::getArgument <int32_t> (cx, args, 0);
		auto       deleteCount = spidermonkey::getArgument <int32_t> (cx, args, 1);
		auto       result      = new InternalType ();

		if (index > (int32_t) array -> size ())
			index = array -> size ();

		if (index < 0)
		{
			index = array -> size () + index;

			if (index < 0)
				index = 0;
		}

		if (deleteCount < 0)
			deleteCount = 0;

		if (index + deleteCount > (int32_t) array -> size ())
			deleteCount = array -> size () - index;

		result -> insert (result -> begin (), array -> cbegin () + index, array -> cbegin () + (index + deleteCount));
		array  -> erase (array -> begin () + index, array -> begin () + (index + deleteCount));

		for (ssize_t i = argc - 1; i >= 2; -- i)
			array -> emplace (array -> begin () + index, getArgument <ValueType> (cx, args, i));

		args .rval () .set (create (cx, result));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .splice: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .splice: %s.", getClass () -> name, error .what ());
	}
}

template <class ValueType, class InternalType>
bool
X3DArrayFieldTemplate <ValueType, InternalType>::unshift (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <X3DArrayFieldTemplate> (cx, args);

		for (ssize_t i = argc - 1; i >= 0; -- i)
			array -> emplace_front (getArgument <ValueType> (cx, args, i));

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .unshift: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .unshift: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania

#endif

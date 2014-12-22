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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__

#include "jsContext.h"
#include "jsError.h"
#include "jsString.h"
#include "jsX3DField.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type, class InternalType>
class jsX3DArrayField :
	public jsX3DField
{
public:

	///  @name Member types

	using value_type    = Type;
	using internal_type = InternalType;
	using single_type   = typename InternalType::value_type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JS::Value
	create (JSContext* const, InternalType*)
	throw (std::invalid_argument);

	static
	JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr ObjectType
	getId ()
	{ throw std::domain_error ("getId"); }


private:

	///  @name Member types

	enum Property {LENGTH};

	///  @name Construction

	static JSBool construct (JSContext*, unsigned, JS::Value*);

	///  @name Member access

	static JSBool enumerate (JSContext*, JS::HandleObject, JSIterateOp, JS::MutableHandleValue, JS::MutableHandleId);
	static JSBool set1Value (JSContext*, JS::HandleObject, JS::HandleId, JSBool, JS::MutableHandleValue);
	static JSBool get1Value (JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);

	///  @name Functions

	static JSBool unshift (JSContext*, unsigned, JS::Value*);
	static JSBool push    (JSContext*, unsigned, JS::Value*);
	static JSBool shift   (JSContext*, unsigned, JS::Value*);
	static JSBool pop     (JSContext*, unsigned, JS::Value*);

	///  @name Properties

	static JSBool setLength (JSContext*, unsigned, JS::Value*);
	static JSBool getLength (JSContext*, unsigned, JS::Value*);

	template <class T = Type, class ... Args>
	static
	typename std::enable_if <
		not (std::is_integral <typename T::internal_type::internal_type>::value or
	        std::is_floating_point <typename T::internal_type::internal_type>::value or
	        std::is_same <typename T::internal_type::internal_type, std::string>::value or
	        std::is_same <typename T::internal_type::internal_type, X3D::String>::value),
		const typename T::internal_type &
	>::type
	get1Argument (Args && ... args)
	{
		return *getArgument <T> (std::forward <Args> (args) ...);
	}

	template <class T = Type, class ... Args>
	static
	typename std::enable_if <
	   std::is_integral <typename T::internal_type::internal_type>::value or
	   std::is_floating_point <typename T::internal_type::internal_type>::value or
	   std::is_same <typename T::internal_type::internal_type, std::string>::value or
	   std::is_same <typename T::internal_type::internal_type, X3D::String>::value,
	   typename T::internal_type::internal_type
	>::type
	get1Argument (Args && ... args)
	{
		return getArgument <typename T::internal_type::internal_type> (std::forward <Args> (args) ...);
	}

	///  @name Static members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class Type, class InternalType>
JSPropertySpec jsX3DArrayField <Type, InternalType>::properties [ ] = {
	JS_PSGS ("length", getLength, setLength, JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PS_END

};

template <class Type, class InternalType>
JSFunctionSpec jsX3DArrayField <Type, InternalType>::functions [ ] = {
	JS_FS ("unshift", unshift, 1, JSPROP_PERMANENT),
	JS_FS ("push",    push,    1, JSPROP_PERMANENT),
	JS_FS ("shift",   shift,   0, JSPROP_PERMANENT),
	JS_FS ("pop",     pop,     0, JSPROP_PERMANENT),
	JS_FS_END

};

template <class Type, class InternalType>
JSObject*
jsX3DArrayField <Type, InternalType>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

template <class Type, class InternalType>
JS::Value
jsX3DArrayField <Type, InternalType>::create (JSContext* const cx, InternalType* const array)
throw (std::invalid_argument)
{
	return jsX3DField::create (cx, &static_class, array);
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
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
			const auto args = JS::CallArgsFromVp (argc, vp);
			const auto array = new InternalType ();

			for (unsigned i = 0; i < argc; ++ i)
			{
				array -> emplace_back (get1Argument (cx, args, i));
			}

			args .rval () .set (create (cx, array));
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::enumerate (JSContext* cx, JS::HandleObject obj, JSIterateOp enum_op, JS::MutableHandleValue statep, JS::MutableHandleId idp)
{
	try
	{
		const auto array = getThis <jsX3DArrayField> (cx, obj);

		size_t* index = nullptr;

		switch (enum_op)
		{
			case JSENUMERATE_INIT:
			case JSENUMERATE_INIT_ALL:
			{
				index = new size_t (0);
				statep .set (PRIVATE_TO_JSVAL (index));
				idp .set (INT_TO_JSID (array -> size ()));
				break;
			}
			case JSENUMERATE_NEXT:
			{
				index = (size_t*) JSVAL_TO_PRIVATE (statep);

				if (*index < array -> size ())
				{
					idp .set (INT_TO_JSID (*index));
					*index = *index + 1;
					break;
				}

				//else done -- cleanup.
			}
			case JSENUMERATE_DESTROY:
			{
				index = (size_t*) JSVAL_TO_PRIVATE (statep);
				delete index;
				statep .setNull ();
			}
		}

		return true;
	}
	catch (const std::exception &)
	{
		statep .setNull ();
		return true;
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	try
	{
		if (not JSID_IS_INT (id))
			return true;

		const auto array = getThis <jsX3DArrayField> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index < 0)
			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);

		array -> set1Value (index, get1Argument (cx, vp .get (), 0));

		return true;
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::get1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	try
	{
		if (not JSID_IS_INT (id))
			return true;

		const auto array = getThis <jsX3DArrayField> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index < 0)
			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);

		vp .set (Type::create (cx, &array -> get1Value (index)));
		return true;
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::unshift (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .unshift: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField> (cx, args);

		array -> emplace_front (get1Argument (cx, args, 0));

		args .rval () .setNumber ((uint32_t) array -> size ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .unshift: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::push (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .push: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField> (cx, args);

		array -> emplace_back (get1Argument (cx, args, 0));

		args .rval () .setNumber ((uint32_t)array -> size ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .push: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::shift (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .shift: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField> (cx, args);

		if (array -> empty ())
		{
			args .rval () .setUndefined ();
			return true;
		}

		const auto value = new single_type (array -> front ());

		array -> pop_front ();

		args .rval () .set (Type::create (cx, value));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .shift: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::pop (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .pop: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField> (cx, args);

		if (array -> empty ())
		{
			args .rval () .setUndefined ();
			return true;
		}

		const auto value = new single_type (array -> back ());

		array -> pop_back ();

		args .rval () .set (Type::create (cx, value));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .pop: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::setLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField> (cx, args);
		const auto size  = getArgument <uint32_t> (cx, args, 0);

		array -> resize (size);

		return true;
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::getLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <jsX3DArrayField> (cx, args);

		args .rval () .setNumber ((uint32_t) array -> size ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC4_H__

#include "../../../Fields/SFVec4.h"
#include "../jsContext.h"
#include "../jsError.h"
#include "../jsX3DField.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type>
class jsSFVec4 :
	public jsX3DField
{
public:

	///  @name Member types

	using internal_type = Type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JS::Value
	create (JSContext* const, Type*)
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

	enum Property {X, Y, Z, W};

	///  @name Construction

	static JSBool construct (JSContext*, unsigned, JS::Value*);

	///  @name Member access

	static JSBool set1Value (JSContext*, JS::HandleObject, JS::HandleId, JSBool, JS::MutableHandleValue);
	static JSBool get1Value (JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);

	template <size_t Index>
	static JSBool setProperty (JSContext*, unsigned, JS::Value*);

	template <size_t Index>
	static JSBool getProperty (JSContext*, unsigned, JS::Value*);

	///  @name Functions

	static JSBool negate    (JSContext*, unsigned, JS::Value*);
	static JSBool add       (JSContext*, unsigned, JS::Value*);
	static JSBool subtract  (JSContext*, unsigned, JS::Value*);
	static JSBool multiply  (JSContext*, unsigned, JS::Value*);
	static JSBool multVec   (JSContext*, unsigned, JS::Value*);
	static JSBool divide    (JSContext*, unsigned, JS::Value*);
	static JSBool divVec    (JSContext*, unsigned, JS::Value*);
	static JSBool dot       (JSContext*, unsigned, JS::Value*);
	static JSBool normalize (JSContext*, unsigned, JS::Value*);
	static JSBool length    (JSContext*, unsigned, JS::Value*);

	///  @name Static members

	static constexpr size_t size = 4;

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class Type>
JSPropertySpec jsSFVec4 <Type>::properties [ ] = {
	JS_PSGS ("x", getProperty <X>, setProperty <X>, JSPROP_PERMANENT),
	JS_PSGS ("y", getProperty <Y>, setProperty <Y>, JSPROP_PERMANENT),
	JS_PSGS ("z", getProperty <Z>, setProperty <Z>, JSPROP_PERMANENT),
	JS_PSGS ("w", getProperty <W>, setProperty <W>, JSPROP_PERMANENT),
	JS_PS_END

};

template <class Type>
JSFunctionSpec jsSFVec4 <Type>::functions [ ] = {
	JS_FS ("negate",    negate,    0, JSPROP_PERMANENT),
	JS_FS ("add",       add,       1, JSPROP_PERMANENT),
	JS_FS ("subtract",  subtract,  1, JSPROP_PERMANENT),
	JS_FS ("multiply",  multiply,  1, JSPROP_PERMANENT),
	JS_FS ("multVec",   multVec,   1, JSPROP_PERMANENT),
	JS_FS ("divide",    divide,    1, JSPROP_PERMANENT),
	JS_FS ("divVec",    divVec,    1, JSPROP_PERMANENT),
	JS_FS ("normalize", normalize, 0, JSPROP_PERMANENT),
	JS_FS ("dot",       dot,       1, JSPROP_PERMANENT),
	JS_FS ("length",    length,    0, JSPROP_PERMANENT),
	JS_FS_END
};

template <class Type>
JSObject*
jsSFVec4 <Type>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineProperty (cx, proto, (char*) X, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) Y, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) Z, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) W, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	
	return proto;
}

template <class Type>
JS::Value
jsSFVec4 <Type>::create (JSContext* const cx, Type* const field)
throw (std::invalid_argument)
{
	return jsX3DField::create (cx, &static_class, field);
}

template <class Type>
JSBool
jsSFVec4 <Type>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new Type ()));
				return true;
			}
			case size:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto x    = getArgument <double> (cx, args, X);
				const auto y    = getArgument <double> (cx, args, Y);
				const auto z    = getArgument <double> (cx, args, Z);
				const auto w    = getArgument <double> (cx, args, W);

				args .rval () .set (create (cx, new Type (x, y, z, w)));
				return true;
			}
			default:
				return ThrowException (cx, "%s .new: wrong number of arguments.", getClass () -> name);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFVec4> (cx, obj);
		const auto rhs = getArgument <double> (cx, vp .get (), 0);

		lhs -> set1Value (JSID_TO_INT (id), rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::get1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFVec4> (cx, obj);

		vp .setDouble (lhs -> get1Value (JSID_TO_INT (id)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
template <size_t Index>
JSBool
jsSFVec4 <Type>::setProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <double> (cx, args, 0);

		lhs -> set1Value (Index, rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .x: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
template <size_t Index>
JSBool
jsSFVec4 <Type>::getProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);

		args .rval () .setDouble (lhs -> get1Value (Index));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .x: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::negate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .negate: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);

		args .rval () .set (create (cx, lhs -> negate ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .negate: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::add (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .add: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <jsSFVec4> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> add (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .add: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::subtract (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .subtract: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <jsSFVec4> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> subtract (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .subtract: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::multiply (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multiply: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <double> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> multiply (rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multiply: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::multVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <jsSFVec4> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> multiply (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::divide (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .divide: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <double> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> divide (rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divide: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::divVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .divVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <jsSFVec4> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> divide (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::normalize (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .normalize: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);

		args .rval () .set (create (cx, lhs -> normalize ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .normalize: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::dot (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .dot: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);
		const auto rhs  = getArgument <jsSFVec4> (cx, args, 0);

		args .rval () .setDouble (lhs -> dot (*rhs));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .dot: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec4 <Type>::length (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .length: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFVec4> (cx, args);

		args .rval () .setDouble (lhs -> length ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <>
constexpr ObjectType
jsSFVec4 <X3D::SFVec4d>::getId ()
{ return ObjectType::SFVec4d; }

template <>
constexpr ObjectType
jsSFVec4 <X3D::SFVec4f>::getId ()
{ return ObjectType::SFVec4f; }

using jsSFVec4d = jsSFVec4 <X3D::SFVec4d>;
using jsSFVec4f = jsSFVec4 <X3D::SFVec4f>;

extern template class jsSFVec4 <X3D::SFVec4d>;
extern template class jsSFVec4 <X3D::SFVec4f>;

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDERMONKEY_FIELDS_SFVEC3_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDERMONKEY_FIELDS_SFVEC3_H__

#include "../../../Fields/SFVec3.h"
#include "../Context.h"
#include "../Error.h"
#include "../X3DField.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <class Type>
class SFVec3 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = Type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JS::Value
	create (JSContext* const cx, Type* const field);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr ObjectType
	getId ()
	{ throw std::runtime_error ("getId"); }


private:

	///  @name Member types

	enum Property {X, Y, Z};

	///  @name Construction

	static
	bool
	construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Member access

	template <size_t Index>
	static bool setProperty (JSContext* cx, unsigned argc, JS::Value* vp);

	template <size_t Index>
	static bool getProperty (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Functions

	static bool add       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool cross     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool distance  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool divide    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool divVec    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool dot       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool length    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool lerp      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multiply  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multVec   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool negate    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool normalize (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool subtract  (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static members

	static constexpr size_t size = 3;

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class Type>
const JSPropertySpec SFVec3 <Type>::properties [ ] = {
	JS_PSGS ("x", getProperty <X>, setProperty <X>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("y", getProperty <Y>, setProperty <Y>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("z", getProperty <Z>, setProperty <Z>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("0", getProperty <X>, setProperty <X>, JSPROP_PERMANENT),
	JS_PSGS ("1", getProperty <Y>, setProperty <Y>, JSPROP_PERMANENT),
	JS_PSGS ("2", getProperty <Z>, setProperty <Z>, JSPROP_PERMANENT),
	JS_PS_END
};

template <class Type>
const JSFunctionSpec SFVec3 <Type>::functions [ ] = {
	JS_FS ("add",       add,       1, JSPROP_PERMANENT),
	JS_FS ("cross",     cross,     1, JSPROP_PERMANENT),
	JS_FS ("distance",  distance,  1, JSPROP_PERMANENT),
	JS_FS ("divide",    divide,    1, JSPROP_PERMANENT),
	JS_FS ("divVec",    divVec,    1, JSPROP_PERMANENT),
	JS_FS ("dot",       dot,       1, JSPROP_PERMANENT),
	JS_FS ("length",    length,    0, JSPROP_PERMANENT),
	JS_FS ("lerp",      lerp,      2, JSPROP_PERMANENT),
	JS_FS ("multiply",  multiply,  1, JSPROP_PERMANENT),
	JS_FS ("multVec",   multVec,   1, JSPROP_PERMANENT),
	JS_FS ("negate",    negate,    0, JSPROP_PERMANENT),
	JS_FS ("normalize", normalize, 0, JSPROP_PERMANENT),
	JS_FS ("subtract",  subtract,  1, JSPROP_PERMANENT),
	JS_FS_END
};

template <class Type>
JSObject*
SFVec3 <Type>::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

template <class Type>
JS::Value
SFVec3 <Type>::create (JSContext* const cx, Type* const field)
{
	return X3DField::create (cx, &static_class, getId (), field);
}

template <class Type>
bool
SFVec3 <Type>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
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
				const auto x    = getArgument <typename Type::value_type> (cx, args, X);
				const auto y    = getArgument <typename Type::value_type> (cx, args, Y);
				const auto z    = getArgument <typename Type::value_type> (cx, args, Z);

				args .rval () .set (create (cx, new Type (x, y, z)));
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
template <size_t Index>
bool
SFVec3 <Type>::setProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <typename Type::value_type> (cx, args, 0);

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
bool
SFVec3 <Type>::getProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);

		args .rval () .setDouble (lhs -> get1Value (Index));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .x: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::negate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .negate: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);

		args .rval () .set (create (cx, new Type (lhs -> negate ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .negate: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::add (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .add: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);

		args .rval () .set (create (cx, new Type (lhs -> add (*rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .add: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::cross (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .cross: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);

		args .rval () .set (create (cx, new Type (lhs -> cross (*rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .cross: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::distance (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .distance: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);

		args .rval () .setDouble (lhs -> distance (*rhs));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .distance: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::divide (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .divide: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <typename Type::value_type> (cx, args, 0);

		args .rval () .set (create (cx, new Type (lhs -> divide (rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divide: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::divVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .divVec: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);

		args .rval () .set (create (cx, new Type (lhs -> divide (*rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::dot (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .dot: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);

		args .rval () .setDouble (lhs -> dot (*rhs));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .dot: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::length (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .length: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);

		args .rval () .setDouble (lhs -> length ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::lerp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException (cx, "%s .lerp: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);
		const auto t    = getArgument <typename Type::value_type> (cx, args, 1);

		args .rval () .set (create (cx, new Type (lhs -> lerp (*rhs, t))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .lerp: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::multiply (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .multiply: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <typename Type::value_type> (cx, args, 0);

		args .rval () .set (create (cx, new Type (lhs -> multiply (rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multiply: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::multVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .multVec: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);

		args .rval () .set (create (cx, new Type (lhs -> multiply (*rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::normalize (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .normalize: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);

		args .rval () .set (create (cx, new Type (lhs -> normalize ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .normalize: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
bool
SFVec3 <Type>::subtract (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException (cx, "%s .subtract: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFVec3> (cx, args);
		const auto rhs  = getArgument <SFVec3> (cx, args, 0);

		args .rval () .set (create (cx, new Type (lhs -> subtract (*rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .subtract: %s.", getClass () -> name, error .what ());
	}
}

template <>
constexpr ObjectType
SFVec3 <X3D::SFVec3d>::getId ()
{
	return ObjectType::SFVec3d;
}

template <>
constexpr ObjectType
SFVec3 <X3D::SFVec3f>::getId ()
{
	return ObjectType::SFVec3f;
}

using SFVec3d = SFVec3 <X3D::SFVec3d>;
using SFVec3f = SFVec3 <X3D::SFVec3f>;

extern template class SFVec3 <X3D::SFVec3d>;
extern template class SFVec3 <X3D::SFVec3f>;

} // spidermonkey
} // X3D
} // titania

#endif

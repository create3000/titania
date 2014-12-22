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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX3_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX3_H__

#include "../../../Fields/SFMatrix3.h"
#include "../jsContext.h"
#include "../jsError.h"
#include "../jsX3DField.h"
#include "jsSFVec2.h"
#include "jsSFVec3.h"

#include <complex>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type>
class jsSFMatrix3 :
	public jsX3DField
{
public:

	///  @name Member types

	using internal_type = Type;
	using vector2_type  = jsSFVec2 <typename Type::vector2_type>;
	using vector3_type  = jsSFVec3 <typename X3D::SFVec3 <typename math::vector3 <typename Type::value_type>>>;

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

	///  @name Construction

	static JSBool construct (JSContext*, unsigned, JS::Value*);
	static JSBool resolve (JSContext*, JS::HandleObject, JS::HandleId);

	///  @name Member access

	static JSBool set1Value (JSContext*, JS::HandleObject, JS::HandleId, JSBool, JS::MutableHandleValue);
	static JSBool get1Value (JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);

	///  @name Functions

	static JSBool setTransform (JSContext*, unsigned, JS::Value*);
	static JSBool getTransform (JSContext*, unsigned, JS::Value*);

	static JSBool transpose     (JSContext*, unsigned, JS::Value*);
	static JSBool inverse       (JSContext*, unsigned, JS::Value*);
	static JSBool multLeft      (JSContext*, unsigned, JS::Value*);
	static JSBool multRight     (JSContext*, unsigned, JS::Value*);
	static JSBool multVecMatrix (JSContext*, unsigned, JS::Value*);
	static JSBool multMatrixVec (JSContext*, unsigned, JS::Value*);
	static JSBool multDirMatrix (JSContext*, unsigned, JS::Value*);
	static JSBool multMatrixDir (JSContext*, unsigned, JS::Value*);

	///  @name Static members

	static constexpr size_t size = 9;

	static JSClass        static_class;
	static JSFunctionSpec functions [ ];

};

template <class Type>
JSFunctionSpec jsSFMatrix3 <Type>::functions [ ] = {
	JS_FS ("setTransform",  setTransform,  5, JSPROP_PERMANENT),
	JS_FS ("getTransform",  getTransform,  3, JSPROP_PERMANENT),

	JS_FS ("transpose",     transpose,     0, JSPROP_PERMANENT),
	JS_FS ("inverse",       inverse,       0, JSPROP_PERMANENT),
	JS_FS ("multLeft",      multLeft,      1, JSPROP_PERMANENT),
	JS_FS ("multRight",     multRight,     1, JSPROP_PERMANENT),
	JS_FS ("multVecMatrix", multVecMatrix, 1, JSPROP_PERMANENT),
	JS_FS ("multMatrixVec", multMatrixVec, 1, JSPROP_PERMANENT),
	JS_FS ("multDirMatrix", multDirMatrix, 1, JSPROP_PERMANENT),
	JS_FS ("multMatrixDir", multMatrixDir, 1, JSPROP_PERMANENT),
	JS_FS_END

};

template <class Type>
JSObject*
jsSFMatrix3 <Type>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, nullptr, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

template <class Type>
JS::Value
jsSFMatrix3 <Type>::create (JSContext* const cx, Type* const field)
throw (std::invalid_argument)
{
	return jsX3DField::create (cx, &static_class, field);
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
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
				const auto m11  = getArgument <double> (cx, args, 0);
				const auto m12  = getArgument <double> (cx, args, 1);
				const auto m13  = getArgument <double> (cx, args, 2);
				const auto m21  = getArgument <double> (cx, args, 3);
				const auto m22  = getArgument <double> (cx, args, 4);
				const auto m23  = getArgument <double> (cx, args, 5);
				const auto m31  = getArgument <double> (cx, args, 6);
				const auto m32  = getArgument <double> (cx, args, 7);
				const auto m33  = getArgument <double> (cx, args, 8);

				args .rval () .set (create (cx, new Type (m11, m12, m13, m21, m22, m23, m31, m32, m33)));
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
jsSFMatrix3 <Type>::resolve (JSContext* cx, JS::HandleObject obj, JS::HandleId id)
{
	if (not JSID_IS_INT (id))
		return true;

	const auto index = JSID_TO_INT (id);

	if (index < 0 or index >= int32_t (size))
		return true;

	JS_DefineProperty (cx, obj, (char*) (size_t) index, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	return true;
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFMatrix3> (cx, obj);
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
jsSFMatrix3 <Type>::get1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFMatrix3> (cx, obj);

		vp .setDouble (lhs -> get1Value (JSID_TO_INT (id)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::setTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc > 5)
		return ThrowException (cx, "%s .setTransform: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);

		typename Type::vector2_type translation;
		typename Type::value_type rotation;
		typename Type::vector2_type scale (1, 1);
		typename Type::value_type scaleOrientation;
		typename Type::vector2_type center;

		try
		{
			if (argc > 0)
				translation = *getArgument <vector2_type> (cx, args, 0);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 1)
				rotation = getArgument <double> (cx, args, 1);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 2)
				scale = *getArgument <vector2_type> (cx, args, 2);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 3)
				scaleOrientation = getArgument <double> (cx, args, 3);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 4)
				center = *getArgument <vector2_type> (cx, args, 4);
		}
		catch (const std::domain_error &)
		{ }

		lhs -> setTransform (translation, rotation, scale, scaleOrientation, center);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setTransform: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::getTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc > 5)
		return ThrowException (cx, "%s .getTransform: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);

		typename Type::vector2_type translation;
		typename Type::value_type rotation;
		typename Type::vector2_type scale (1, 1);
		typename Type::value_type scaleOrientation;
		typename Type::vector2_type center;

		try
		{
			if (argc > 4)
				center = *getArgument <vector2_type> (cx, args, 4);
		}
		catch (const std::domain_error &)
		{ }

		lhs -> getTransform (translation, rotation, scale, scaleOrientation, center);

		try
		{
			if (argc > 0)
				getArgument <vector2_type> (cx, args, 0) -> setValue (translation);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 1)
			{
				const auto complex = std::polar <typename Type::value_type> (1, rotation);

				getArgument <vector3_type> (cx, args, 1) -> setValue (std::real (complex), std::imag (complex), rotation);
			}
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 2)
				getArgument <vector2_type> (cx, args, 2) -> setValue (scale);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 3)
			{
				const auto complex = std::polar <typename Type::value_type> (1, scaleOrientation);

				getArgument <vector3_type> (cx, args, 3) -> setValue (std::real (complex), std::imag (complex), scaleOrientation);
			}
		}
		catch (const std::domain_error &)
		{ }

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getTransform: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::transpose (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .transpose: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);

		args .rval () .set (create (cx, lhs -> transpose ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .transpose: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::inverse (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .inverse: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);

		args .rval () .set (create (cx, lhs -> inverse ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .inverse: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multLeft (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multLeft: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);
		const auto rhs  = getArgument <jsSFMatrix3> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> multLeft (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multLeft: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multRight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multRight: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);
		const auto rhs  = getArgument <jsSFMatrix3> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> multLeft (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multRight: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multVecMatrix (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVecMatrix: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);
		const auto rhs  = getArgument <vector2_type> (cx, args, 0);

		args .rval () .set (vector2_type::create (cx, lhs -> multVecMatrix (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVecMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multMatrixVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multMatrixVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);
		const auto rhs  = getArgument <vector2_type> (cx, args, 0);

		args .rval () .set (vector2_type::create (cx, lhs -> multMatrixVec (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multMatrixVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multDirMatrix (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multDirMatrix: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);
		const auto rhs  = getArgument <vector2_type> (cx, args, 0);

		args .rval () .set (vector2_type::create (cx, lhs -> multDirMatrix (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multDirMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix3 <Type>::multMatrixDir (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multMatrixDir: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFMatrix3> (cx, args);
		const auto rhs  = getArgument <vector2_type> (cx, args, 0);

		args .rval () .set (vector2_type::create (cx, lhs -> multMatrixDir (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multMatrixDir: %s.", getClass () -> name, error .what ());
	}
}

template <>
constexpr ObjectType
jsSFMatrix3 <X3D::SFMatrix3d>::getId ()
{ return ObjectType::SFMatrix3d; }

template <>
constexpr ObjectType
jsSFMatrix3 <X3D::SFMatrix3f>::getId ()
{ return ObjectType::SFMatrix3f; }

using jsSFMatrix3d = jsSFMatrix3 <X3D::SFMatrix3d>;
using jsSFMatrix3f = jsSFMatrix3 <X3D::SFMatrix3f>;

extern template class jsSFMatrix3 <X3D::SFMatrix3d>;
extern template class jsSFMatrix3 <X3D::SFMatrix3f>;

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

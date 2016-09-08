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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX4_H__

#include "../../../Fields/SFMatrix4.h"
#include "../../../Fields/VrmlMatrix.h"
#include "../Context.h"
#include "../Error.h"
#include "../X3DField.h"
#include "SFRotation.h"
#include "SFVec3.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <class Type>
class SFMatrix4 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = Type;
	using value_type    = typename Type::value_type;
	using vector3_type  = SFVec3 <typename Type::vector3_type>;
	using rotation_type = SFRotation;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

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

	static JSBool construct (JSContext*, uint32_t, jsval*);
	static JSBool resolve (JSContext*, JSObject*, jsid);

	///  @name Member access

	static JSBool enumerate (JSContext*, JSObject*, JSIterateOp, jsval*, jsid*);
	static JSBool set1Value (JSContext*, JSObject*, jsid, JSBool, jsval*);
	static JSBool get1Value (JSContext*, JSObject*, jsid, jsval*);

	///  @name Functions

	static JSBool setTransform (JSContext*, uint32_t, jsval*);
	static JSBool getTransform (JSContext*, uint32_t, jsval*);

	static JSBool transpose     (JSContext*, uint32_t, jsval*);
	static JSBool inverse       (JSContext*, uint32_t, jsval*);
	static JSBool multLeft      (JSContext*, uint32_t, jsval*);
	static JSBool multRight     (JSContext*, uint32_t, jsval*);
	static JSBool multVecMatrix (JSContext*, uint32_t, jsval*);
	static JSBool multMatrixVec (JSContext*, uint32_t, jsval*);
	static JSBool multDirMatrix (JSContext*, uint32_t, jsval*);
	static JSBool multMatrixDir (JSContext*, uint32_t, jsval*);

	///  @name Static members

	static const size_t   size;
	static JSClass        static_class;
	static JSFunctionSpec functions [ ];

};

template <class Type>
const size_t SFMatrix4 <Type>::size = 16;

template <class Type>
JSClass SFMatrix4 <Type>::static_class = {
	"Type", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, resolve, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type>
JSFunctionSpec SFMatrix4 <Type>::functions [ ] = {
	{ "setTransform",  setTransform,  5, 0 },
	{ "getTransform",  getTransform,  3, 0 },

	{ "transpose",     transpose,     0, 0 },
	{ "inverse",       inverse,       0, 0 },
	{ "multLeft",      multLeft,      1, 0 },
	{ "multRight",     multRight,     1, 0 },
	{ "multVecMatrix", multVecMatrix, 1, 0 },
	{ "multMatrixVec", multMatrixVec, 1, 0 },
	{ "multDirMatrix", multDirMatrix, 1, 0 },
	{ "multMatrixDir", multMatrixDir, 1, 0 },

	{ 0 }

};

template <class Type>
JSObject*
SFMatrix4 <Type>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, nullptr, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

template <class Type>
JSBool
SFMatrix4 <Type>::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create <SFMatrix4> (cx, new Type (), &JS_RVAL (cx, vp));
			}
			case 16:
			{
				const auto argv = JS_ARGV (cx, vp);
				const auto m11  = getArgument <value_type> (cx, argv,  0);
				const auto m12  = getArgument <value_type> (cx, argv,  1);
				const auto m13  = getArgument <value_type> (cx, argv,  2);
				const auto m14  = getArgument <value_type> (cx, argv,  3);
				const auto m21  = getArgument <value_type> (cx, argv,  4);
				const auto m22  = getArgument <value_type> (cx, argv,  5);
				const auto m23  = getArgument <value_type> (cx, argv,  6);
				const auto m24  = getArgument <value_type> (cx, argv,  7);
				const auto m31  = getArgument <value_type> (cx, argv,  8);
				const auto m32  = getArgument <value_type> (cx, argv,  9);
				const auto m33  = getArgument <value_type> (cx, argv, 10);
				const auto m34  = getArgument <value_type> (cx, argv, 11);
				const auto m41  = getArgument <value_type> (cx, argv, 12);
				const auto m42  = getArgument <value_type> (cx, argv, 13);
				const auto m43  = getArgument <value_type> (cx, argv, 14);
				const auto m44  = getArgument <value_type> (cx, argv, 15);

				return create <SFMatrix4> (cx, new Type (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44), &JS_RVAL (cx, vp));
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
SFMatrix4 <Type>::resolve (JSContext* cx, JSObject* obj, jsid id)
{
	if (not JSID_IS_INT (id))
		return true;

	const auto index = JSID_TO_INT (id);

	if (index >= 0 and index < int32_t (size))
	{
		JS_DefineProperty (cx, obj, reinterpret_cast <char*> (index), JSVAL_VOID, get1Value, set1Value, JSPROP_PERMANENT | JSPROP_INDEX);
		return true;
	}

	return ThrowException (cx, "%s: array index out of range.", getClass () -> name);
}

template <class Type>
JSBool
SFMatrix4 <Type>::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	if (not JS_GetPrivate (cx, obj))
	{
		*statep = JSVAL_NULL;
		return true;
	}

	size_t* index;

	switch (enum_op)
	{
		case JSENUMERATE_INIT:
		case JSENUMERATE_INIT_ALL:
		{
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				*idp = INT_TO_JSID (size);

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < size)
			{
				if (idp)
					*idp = INT_TO_JSID (*index);

				*index = *index + 1;
				break;
			}

			//else done -- cleanup.
		}
		case JSENUMERATE_DESTROY:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);
			delete index;
			*statep = JSVAL_NULL;
		}
	}

	return true;
}

template <class Type>
JSBool
SFMatrix4 <Type>::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFMatrix4> (cx, obj);
		const auto index = JSID_TO_INT (id);
		const auto value = getArgument <value_type> (cx, vp, 0);

		lhs -> set1Value (index, value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFMatrix4> (cx, obj);
		const auto index = JSID_TO_INT (id);

		return JS_NewNumberValue (cx, lhs -> get1Value (index), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::setTransform (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc > 5)
		return ThrowException (cx, "%s .setTransform: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);

		typename Type::vector3_type translation;
		typename Type::rotation4_type rotation;
		typename Type::vector3_type scale (1, 1, 1);
		typename Type::rotation4_type scaleOrientation;
		typename Type::vector3_type center;

		try
		{
			if (argc > 0)
				translation = *getArgument <vector3_type> (cx, argv, 0);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 1)
				rotation = *getArgument <rotation_type> (cx, argv, 1);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 2)
				scale = *getArgument <vector3_type> (cx, argv, 2);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 3)
				scaleOrientation = *getArgument <rotation_type> (cx, argv, 3);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 4)
				center = *getArgument <vector3_type> (cx, argv, 4);
		}
		catch (const std::domain_error &)
		{ }

		lhs -> setTransform (translation, rotation, scale, scaleOrientation, center);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setTransform: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::getTransform (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc > 5)
		return ThrowException (cx, "%s .getTransform: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);

		typename Type::vector3_type translation;
		typename Type::rotation4_type rotation;
		typename Type::vector3_type scale (1, 1, 1);
		typename Type::rotation4_type scaleOrientation;
		typename Type::vector3_type center;

		try
		{
			if (argc > 4)
				center = *getArgument <vector3_type> (cx, argv, 4);
		}
		catch (const std::domain_error &)
		{ }

		lhs -> getTransform (translation, rotation, scale, scaleOrientation, center);

		try
		{
			if (argc > 0)
				getArgument <vector3_type> (cx, argv, 0) -> setValue (translation);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 1)
				getArgument <rotation_type> (cx, argv, 1) -> setValue (rotation);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 2)
				getArgument <vector3_type> (cx, argv, 2) -> setValue (scale);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 3)
				getArgument <rotation_type> (cx, argv, 3) -> setValue (scaleOrientation);
		}
		catch (const std::domain_error &)
		{ }

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getTransform: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::transpose (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .transpose: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <SFMatrix4> (cx, vp);

		return create <SFMatrix4> (cx, lhs -> transpose (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .transpose: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::inverse (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .inverse: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <SFMatrix4> (cx, vp);

		return create <SFMatrix4> (cx, lhs -> inverse (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .inverse: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::multLeft (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multLeft: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);
		const auto rhs  = getArgument <SFMatrix4> (cx, argv, 0);

		return create <SFMatrix4> (cx, lhs -> multLeft (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multLeft: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::multRight (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multRight: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);
		const auto rhs  = getArgument <SFMatrix4> (cx, argv, 0);

		return create <SFMatrix4> (cx, lhs -> multLeft (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multRight: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::multVecMatrix (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVecMatrix: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return create <vector3_type> (cx, lhs -> multVecMatrix (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVecMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::multMatrixVec (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multMatrixVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return create <vector3_type> (cx, lhs -> multMatrixVec (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multMatrixVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::multDirMatrix (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multDirMatrix: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return create <vector3_type> (cx, lhs -> multDirMatrix (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multDirMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
SFMatrix4 <Type>::multMatrixDir (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multMatrixDir: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return create <vector3_type> (cx, lhs -> multMatrixDir (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multMatrixDir: %s.", getClass () -> name, error .what ());
	}
}

using SFMatrix4d = SFMatrix4 <X3D::SFMatrix4d>;
using SFMatrix4f = SFMatrix4 <X3D::SFMatrix4f>;
using VrmlMatrix = SFMatrix4 <X3D::VrmlMatrix>;

template <>
constexpr ObjectType
SFMatrix4d::getId ()
{ return ObjectType::SFMatrix4d; }

template <>
constexpr ObjectType
SFMatrix4f::getId ()
{ return ObjectType::SFMatrix4f; }

template <>
constexpr ObjectType
VrmlMatrix::getId ()
{ return ObjectType::VrmlMatrix; }

extern template class SFMatrix4 <X3D::SFMatrix4d>;
extern template class SFMatrix4 <X3D::SFMatrix4f>;
extern template class SFMatrix4 <X3D::VrmlMatrix>;

} // spidermonkey
} // X3D
} // titania

#endif

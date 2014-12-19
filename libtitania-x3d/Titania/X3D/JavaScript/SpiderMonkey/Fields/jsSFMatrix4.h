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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFMATRIX4_H__

#include "../../../Fields/SFMatrix4.h"
#include "../../../Fields/VrmlMatrix.h"
#include "../jsContext.h"
#include "../jsError.h"
#include "../jsX3DField.h"
#include "jsSFRotation.h"
#include "jsSFVec3.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type>
class jsSFMatrix4 :
	public jsX3DField
{
public:

	using internal_type = Type;
	using vector3_type  = jsSFVec3 <typename Type::vector3_type>;
	using rotation_type = jsSFRotation;

	static
	void
	init (JSContext* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, Type*, jsval* const);

	static
	JSClass*
	getClass ()
	{ return &static_class; }


private:

	static JSBool construct (JSContext*, uint32_t, jsval*);
	static JSBool enumerate (JSContext*, JSObject*, JSIterateOp, jsval*, jsid*);
	static JSBool resolve (JSContext*, JSObject*, jsid);

	static JSBool set1Value (JSContext*, JSObject*, jsid, JSBool, jsval*);
	static JSBool get1Value (JSContext*, JSObject*, jsid, jsval*);

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

	static const size_t   size;
	static JSClass        static_class;
	static JSFunctionSpec functions [ ];

};

template <class Type>
const size_t jsSFMatrix4 <Type>::size = 16;

template <class Type>
JSClass jsSFMatrix4 <Type>::static_class = {
	"Type", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, resolve, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type>
JSFunctionSpec jsSFMatrix4 <Type>::functions [ ] = {
	{ "getName",       getName <Type>,     0, 0 },
	{ "getTypeName",   getTypeName <Type>, 0, 0 },
	{ "getType",       getType,            0, 0 },
	{ "isReadable",    isReadable,         0, 0 },
	{ "isWritable",    isWritable,         0, 0 },

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

	{ "toString",      toString <Type>, 0, 0 },

	{ 0 }

};

template <class Type>
void
jsSFMatrix4 <Type>::init (JSContext* const cx, JSObject* const global)
{
	const auto proto = JS_InitClass (cx, global, nullptr, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::create (JSContext* const cx, Type* const field, jsval* const vp)
{
	return jsX3DField::create (cx, &static_class, field, vp);
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create (cx, new Type (), &JS_RVAL (cx, vp));
			}
			case 16:
			{
				const auto argv = JS_ARGV (cx, vp);
				const auto m11  = getArgument <double> (cx, argv,  0);
				const auto m12  = getArgument <double> (cx, argv,  1);
				const auto m13  = getArgument <double> (cx, argv,  2);
				const auto m14  = getArgument <double> (cx, argv,  3);
				const auto m21  = getArgument <double> (cx, argv,  4);
				const auto m22  = getArgument <double> (cx, argv,  5);
				const auto m23  = getArgument <double> (cx, argv,  6);
				const auto m24  = getArgument <double> (cx, argv,  7);
				const auto m31  = getArgument <double> (cx, argv,  8);
				const auto m32  = getArgument <double> (cx, argv,  9);
				const auto m33  = getArgument <double> (cx, argv, 10);
				const auto m34  = getArgument <double> (cx, argv, 11);
				const auto m41  = getArgument <double> (cx, argv, 12);
				const auto m42  = getArgument <double> (cx, argv, 13);
				const auto m43  = getArgument <double> (cx, argv, 14);
				const auto m44  = getArgument <double> (cx, argv, 15);

				return create (cx, new Type (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44), &JS_RVAL (cx, vp));
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
jsSFMatrix4 <Type>::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
jsSFMatrix4 <Type>::resolve (JSContext* cx, JSObject* obj, jsid id)
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
jsSFMatrix4 <Type>::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <jsSFMatrix4> (cx, obj);
		const auto index = JSID_TO_INT (id);
		const auto value = getArgument <double> (cx, vp, 0);

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
jsSFMatrix4 <Type>::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <jsSFMatrix4> (cx, obj);
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
jsSFMatrix4 <Type>::setTransform (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc > 5)
		return ThrowException (cx, "%s .setTransform: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);

		typename Type::vector3_type translation;
		typename Type::rotation4f_type rotation;
		typename Type::vector3_type scale (1, 1, 1);
		typename Type::rotation4f_type scaleOrientation;
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
jsSFMatrix4 <Type>::getTransform (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc > 5)
		return ThrowException (cx, "%s .getTransform: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);

		typename Type::vector3_type translation;
		typename Type::rotation4f_type rotation;
		typename Type::vector3_type scale (1, 1, 1);
		typename Type::rotation4f_type scaleOrientation;
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
jsSFMatrix4 <Type>::transpose (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .transpose: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFMatrix4> (cx, vp);

		return create (cx, lhs -> transpose (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .transpose: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::inverse (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .inverse: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFMatrix4> (cx, vp);

		return create (cx, lhs -> inverse (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .inverse: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multLeft (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multLeft: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);
		const auto rhs  = getArgument <jsSFMatrix4> (cx, argv, 0);

		return create (cx, lhs -> multLeft (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multLeft: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multRight (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multRight: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);
		const auto rhs  = getArgument <jsSFMatrix4> (cx, argv, 0);

		return create (cx, lhs -> multLeft (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multRight: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multVecMatrix (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVecMatrix: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return vector3_type::create (cx, lhs -> multVecMatrix (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVecMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multMatrixVec (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multMatrixVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return vector3_type::create (cx, lhs -> multMatrixVec (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multMatrixVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multDirMatrix (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multDirMatrix: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return vector3_type::create (cx, lhs -> multDirMatrix (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multDirMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFMatrix4 <Type>::multMatrixDir (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multMatrixDir: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFMatrix4> (cx, vp);
		const auto rhs  = getArgument <vector3_type> (cx, argv, 0);

		return vector3_type::create (cx, lhs -> multMatrixDir (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multMatrixDir: %s.", getClass () -> name, error .what ());
	}
}

extern template class jsSFMatrix4 <X3D::SFMatrix4d>;
extern template class jsSFMatrix4 <X3D::SFMatrix4f>;
extern template class jsSFMatrix4 <X3D::VrmlMatrix>;

using jsSFMatrix4d = jsSFMatrix4 <X3D::SFMatrix4d>;
using jsSFMatrix4f = jsSFMatrix4 <X3D::SFMatrix4f>;
using jsVrmlMatrix = jsSFMatrix4 <X3D::VrmlMatrix>;

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

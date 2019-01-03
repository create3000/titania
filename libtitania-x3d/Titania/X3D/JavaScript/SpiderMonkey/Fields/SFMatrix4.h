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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_SFMATRIX4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_SFMATRIX4_H__

#include "../X3DField.h"
#include "SFRotation.h"
#include "SFVec3.h"

#include "../../../Fields/SFMatrix4.h"
#include "../../../Fields/VrmlMatrix.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <class InternalType>
class SFMatrix4 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = InternalType;
	using vector_type   = SFVec3 <typename InternalType::vector_type>;
	using rotation_type = SFRotation;

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

	///  @name Member access

	template <size_t Index>
	static bool set1Value (JSContext* cx, unsigned argc, JS::Value* vp);

	template <size_t Index>
	static bool get1Value (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Functions

	static bool setTransform (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool getTransform (JSContext* cx, unsigned argc, JS::Value* vp);

	static bool transpose     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool inverse       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multLeft      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multRight     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multVecMatrix (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multMatrixVec (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multDirMatrix (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multMatrixDir (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static members

	static constexpr size_t Size = std::tuple_size <typename InternalType::internal_type> ();

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class InternalType>
const JSClassOps SFMatrix4 <InternalType>::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

template <class InternalType>
const JSPropertySpec SFMatrix4 <InternalType>::properties [ ] = {
	JS_PSGS ("0",  get1Value <0>,  set1Value <0>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("1",  get1Value <1>,  set1Value <1>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("2",  get1Value <2>,  set1Value <2>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("3",  get1Value <3>,  set1Value <3>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("4",  get1Value <4>,  set1Value <4>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("5",  get1Value <5>,  set1Value <5>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("6",  get1Value <6>,  set1Value <6>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("7",  get1Value <7>,  set1Value <7>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("8",  get1Value <8>,  set1Value <8>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("9",  get1Value <9>,  set1Value <9>,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("10", get1Value <10>, set1Value <10>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("11", get1Value <11>, set1Value <11>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("12", get1Value <12>, set1Value <12>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("13", get1Value <13>, set1Value <13>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("14", get1Value <14>, set1Value <14>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("15", get1Value <15>, set1Value <15>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

template <class InternalType>
const JSFunctionSpec SFMatrix4 <InternalType>::functions [ ] = {
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

template <class InternalType>
JSObject*
SFMatrix4 <InternalType>::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

template <class InternalType>
JS::Value
SFMatrix4 <InternalType>::create (JSContext* const cx, InternalType* const field)
{
	return X3DField::create <SFMatrix4> (cx, field) .first;
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new InternalType ()));
				return true;
			}
			case Size:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto m11  = getArgument <double> (cx, args,  0);
				const auto m12  = getArgument <double> (cx, args,  1);
				const auto m13  = getArgument <double> (cx, args,  2);
				const auto m14  = getArgument <double> (cx, args,  3);
				const auto m21  = getArgument <double> (cx, args,  4);
				const auto m22  = getArgument <double> (cx, args,  5);
				const auto m23  = getArgument <double> (cx, args,  6);
				const auto m24  = getArgument <double> (cx, args,  7);
				const auto m31  = getArgument <double> (cx, args,  8);
				const auto m32  = getArgument <double> (cx, args,  9);
				const auto m33  = getArgument <double> (cx, args, 10);
				const auto m34  = getArgument <double> (cx, args, 11);
				const auto m41  = getArgument <double> (cx, args, 12);
				const auto m42  = getArgument <double> (cx, args, 13);
				const auto m43  = getArgument <double> (cx, args, 14);
				const auto m44  = getArgument <double> (cx, args, 15);

				args .rval () .set (create (cx, new InternalType (m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)));
				return true;
			}
			default:
				return ThrowException <JSProto_Error> (cx, "new %s: wrong number of arguments.", getClass () -> name);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
template <size_t Index>
bool
SFMatrix4 <InternalType>::set1Value (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto self  = getThis <SFMatrix4> (cx, args);
		const auto value = getArgument <typename InternalType::value_type> (cx, args, 0);

		self -> set1Value (Index, value);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, Index, error .what ());
	}
}

template <class InternalType>
template <size_t Index>
bool
SFMatrix4 <InternalType>::get1Value (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFMatrix4> (cx, args);

		args .rval () .setDouble (self -> get1Value (Index));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, Index, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::setTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc > 5)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .setTransform: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFMatrix4> (cx, args);

		typename InternalType::vector_type translation;
		typename InternalType::rotation_type rotation;
		typename InternalType::vector_type scale (1, 1, 1);
		typename InternalType::rotation_type scaleOrientation;
		typename InternalType::vector_type center;

		try
		{
			if (argc > 0)
				translation = *getArgument <vector_type> (cx, args, 0);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 1)
				rotation = *getArgument <rotation_type> (cx, args, 1);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 2)
				scale = *getArgument <vector_type> (cx, args, 2);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 3)
				scaleOrientation = *getArgument <rotation_type> (cx, args, 3);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 4)
				center = *getArgument <vector_type> (cx, args, 4);
		}
		catch (const std::domain_error &)
		{ }

		self -> setTransform (translation, rotation, scale, scaleOrientation, center);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .setTransform: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::getTransform (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc > 5)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getTransform: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFMatrix4> (cx, args);

		typename InternalType::vector_type::internal_type translation;
		typename InternalType::rotation_type::internal_type rotation;
		typename InternalType::vector_type::internal_type scale (1, 1, 1);
		typename InternalType::rotation_type::internal_type scaleOrientation;
		typename InternalType::vector_type::internal_type center;

		try
		{
			if (argc > 4)
				center = *getArgument <vector_type> (cx, args, 4);
		}
		catch (const std::domain_error &)
		{ }

		self -> getTransform (translation, rotation, scale, scaleOrientation, center);

		try
		{
			if (argc > 0)
				getArgument <vector_type> (cx, args, 0) -> setValue (translation);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 1)
				getArgument <rotation_type> (cx, args, 1) -> setValue (rotation);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 2)
				getArgument <vector_type> (cx, args, 2) -> setValue (scale);
		}
		catch (const std::domain_error &)
		{ }

		try
		{
			if (argc > 3)
				getArgument <rotation_type> (cx, args, 3) -> setValue (scaleOrientation);
		}
		catch (const std::domain_error &)
		{ }

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getTransform: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::transpose (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .transpose: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFMatrix4> (cx, args);

		args .rval () .set (create (cx, new InternalType (self -> transpose ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .transpose: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::inverse (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .inverse: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFMatrix4> (cx, args);

		args .rval () .set (create (cx, new InternalType (self -> inverse ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .inverse: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::multLeft (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multLeft: wrong number of arguments.", getClass () -> name);
	
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFMatrix4> (cx, args);
		const auto matrix = getArgument <SFMatrix4> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> multLeft (*matrix))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multLeft: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::multRight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multRight: wrong number of arguments.", getClass () -> name);
	
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFMatrix4> (cx, args);
		const auto matrix = getArgument <SFMatrix4> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> multRight (*matrix))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multRight: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::multVecMatrix (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multVecMatrix: wrong number of arguments.", getClass () -> name);
	
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFMatrix4> (cx, args);
		const auto vector = getArgument <vector_type> (cx, args, 0);

		args .rval () .set (vector_type::create (cx, new typename vector_type::internal_type (self -> multVecMatrix (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multVecMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::multMatrixVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multMatrixVec: wrong number of arguments.", getClass () -> name);
	
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFMatrix4> (cx, args);
		const auto vector = getArgument <vector_type> (cx, args, 0);

		args .rval () .set (vector_type::create (cx, new typename vector_type::internal_type (self -> multMatrixVec (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multMatrixVec: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::multDirMatrix (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multDirMatrix: wrong number of arguments.", getClass () -> name);
	
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFMatrix4> (cx, args);
		const auto vector = getArgument <vector_type> (cx, args, 0);

		args .rval () .set (vector_type::create (cx, new typename vector_type::internal_type (self -> multDirMatrix (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multDirMatrix: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFMatrix4 <InternalType>::multMatrixDir (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multMatrixDir: wrong number of arguments.", getClass () -> name);
	
		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFMatrix4> (cx, args);
		const auto vector = getArgument <vector_type> (cx, args, 0);

		args .rval () .set (vector_type::create (cx, new typename vector_type::internal_type (self -> multMatrixDir (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multMatrixDir: %s.", getClass () -> name, error .what ());
	}
}

template <>
constexpr
ObjectType
SFMatrix4 <X3D::SFMatrix4d>::getId ()
{ return ObjectType::SFMatrix4d; }

template <>
constexpr
ObjectType
SFMatrix4 <X3D::SFMatrix4f>::getId ()
{ return ObjectType::SFMatrix4f; }

template <>
constexpr
ObjectType
SFMatrix4 <X3D::VrmlMatrix>::getId ()
{ return ObjectType::VrmlMatrix; }

using SFMatrix4d = SFMatrix4 <X3D::SFMatrix4d>;
using SFMatrix4f = SFMatrix4 <X3D::SFMatrix4f>;
using VrmlMatrix = SFMatrix4 <X3D::VrmlMatrix>;

extern template class SFMatrix4 <X3D::SFMatrix4d>;
extern template class SFMatrix4 <X3D::SFMatrix4f>;
extern template class SFMatrix4 <X3D::VrmlMatrix>;

} // spidermonkey
} // X3D
} // titania

#endif

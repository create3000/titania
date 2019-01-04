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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFMATRIX4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFMATRIX4_H__

#include "../../../Fields/SFMatrix4.h"
#include "../../../Fields/VrmlMatrix.h"
#include "../X3DField.h"
#include "SFRotation.h"
#include "SFVec3.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

template <class InternalType>
class SFMatrix4 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = InternalType;
	using vector3_type  = SFVec3 <typename InternalType::vector_type>;
	using rotation_type = SFRotation;

	///  @name Common members

	static
	constexpr ObjectType
	getType ()
	{ throw std::invalid_argument ("SFMatrix4::getType"); }

	static
	const std::string &
	getTypeName ()
	{ return typeName; }

	static
	const pb::Callbacks &
	getCallbacks ()
	{ return callbacks; }

	///  @name Construction

	static
	pb::ptr <pb::NativeFunction>
	initialize (Context* const, const pb::ptr <pb::Program> &);


private:

	///  @name Construction

	static
	pb::var
	construct (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Member access

	static
	pb::var
	set1Value (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, const size_t);

	static
	pb::var
	get1Value (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, const size_t);

	///  @name Functions

	static
	pb::var
	setTransform (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	getTransform (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	transpose (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	inverse (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multLeft (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multRight (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multVecMatrix (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multMatrixVec (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multDirMatrix (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multMatrixDir (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Static members

	static const std::string typeName;
	static const pb::Callbacks callbacks;

};

template <class InternalType>
pb::ptr <pb::NativeFunction>
SFMatrix4 <InternalType>::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 16);
	const auto prototype = context -> getClass (ObjectType::X3DField) -> createInstance (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	// Properties

	for (size_t index = 0; index < 16; ++ index)
	{
		const auto name = basic::to_string (index, std::locale::classic ());

		prototype -> addOwnProperty (name,
		                             pb::undefined,
		                             pb::ENUMERABLE,
		                             new pb::NativeFunction (ec, name, std::bind (get1Value, _1, _2, _3, index), 0),
		                             new pb::NativeFunction (ec, name, std::bind (set1Value, _1, _2, _3, index), 1));
	}

	// Functions

	prototype -> addOwnProperty ("setTransform",  new pb::NativeFunction (ec, "setTransform",  setTransform,  5), pb::NONE);
	prototype -> addOwnProperty ("getTransform",  new pb::NativeFunction (ec, "getTransform",  getTransform,  3), pb::NONE);
	prototype -> addOwnProperty ("transpose",     new pb::NativeFunction (ec, "transpose",     transpose,     0), pb::NONE);
	prototype -> addOwnProperty ("inverse",       new pb::NativeFunction (ec, "inverse",       inverse,       0), pb::NONE);
	prototype -> addOwnProperty ("multLeft",      new pb::NativeFunction (ec, "multLeft",      multLeft,      1), pb::NONE);
	prototype -> addOwnProperty ("multRight",     new pb::NativeFunction (ec, "multRight",     multRight,     1), pb::NONE);
	prototype -> addOwnProperty ("multVecMatrix", new pb::NativeFunction (ec, "multVecMatrix", multVecMatrix, 1), pb::NONE);
	prototype -> addOwnProperty ("multMatrixVec", new pb::NativeFunction (ec, "multMatrixVec", multMatrixVec, 1), pb::NONE);
	prototype -> addOwnProperty ("multDirMatrix", new pb::NativeFunction (ec, "multDirMatrix", multDirMatrix, 1), pb::NONE);
	prototype -> addOwnProperty ("multMatrixDir", new pb::NativeFunction (ec, "multMatrixDir", multMatrixDir, 1), pb::NONE);

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <SFMatrix4> (ec, object, new InternalType ());
			break;
		}
		case 16:
		{
			setUserData <SFMatrix4> (ec, object, new InternalType (get1Argument <double> (args,  0),
			                                                       get1Argument <double> (args,  1),
			                                                       get1Argument <double> (args,  2),
			                                                       get1Argument <double> (args,  3),
			                                                       get1Argument <double> (args,  4),
			                                                       get1Argument <double> (args,  5),
			                                                       get1Argument <double> (args,  6),
			                                                       get1Argument <double> (args,  7),
			                                                       get1Argument <double> (args,  8),
			                                                       get1Argument <double> (args,  9),
			                                                       get1Argument <double> (args, 10),
			                                                       get1Argument <double> (args, 11),
			                                                       get1Argument <double> (args, 12),
			                                                       get1Argument <double> (args, 13),
			                                                       get1Argument <double> (args, 14),
			                                                       get1Argument <double> (args, 15)));
			break;
		}
		default:
			throw pb::Error (getTypeName () + ".constructor: wrong number of arguments.");
	}

	return pb::undefined;
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::set1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);

		lhs -> set1Value (index, get1Argument <double> (args, 0));

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index, std::locale::classic ()) + "] is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::get1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);

		return lhs -> get1Value (index);
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index, std::locale::classic ()) + "] is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::setTransform (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () > 5)
		throw pb::Error (getTypeName () + ".prototype.setTransform: wrong number of arguments.");

	try
	{
		const auto argc = args .size ();
		const auto lhs  = getThis <SFMatrix4> (ec, object);

		typename InternalType::vector_type::internal_type translation;
		typename InternalType::rotation_type::internal_type rotation;
		typename InternalType::vector_type::internal_type scale (1, 1, 1);
		typename InternalType::rotation_type::internal_type scaleOrientation;
		typename InternalType::vector_type::internal_type center;

		if (argc > 0 and not args [0] .isNull ())
			translation = *get1Argument <vector3_type> (args, 0);

		if (argc > 1 and not args [1] .isNull ())
			rotation = *get1Argument <rotation_type> (args, 1);

		if (argc > 2 and not args [2] .isNull ())
			scale = *get1Argument <vector3_type> (args, 2);

		if (argc > 3 and not args [3] .isNull ())
			scaleOrientation = *get1Argument <rotation_type> (args, 3);

		if (argc > 4 and not args [4] .isNull ())
			center = *get1Argument <vector3_type> (args, 4);

		lhs -> setTransform (translation, rotation, scale, scaleOrientation, center);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.setTransform is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::getTransform (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () > 5)
		throw pb::Error (getTypeName () + ".prototype.getTransform: wrong number of arguments.");

	try
	{
		const auto argc = args .size ();
		const auto lhs  = getThis <SFMatrix4> (ec, object);

		typename InternalType::vector_type::internal_type translation;
		typename InternalType::rotation_type::internal_type rotation;
		typename InternalType::vector_type::internal_type scale (1, 1, 1);
		typename InternalType::rotation_type::internal_type scaleOrientation;
		typename InternalType::vector_type::internal_type center;

		if (argc > 4 and not args [4] .isNull ())
			center = *get1Argument <vector3_type> (args, 4);

		lhs -> getTransform (translation, rotation, scale, scaleOrientation, center);

		if (argc > 0 and not args [0] .isNull ())
			get1Argument <vector3_type> (args, 0) -> setValue (translation);

		if (argc > 1 and not args [1] .isNull ())
			get1Argument <rotation_type> (args, 1) -> setValue (rotation);

		if (argc > 2 and not args [2] .isNull ())
			get1Argument <vector3_type> (args, 2) -> setValue (scale);

		if (argc > 3 and not args [3] .isNull ())
			get1Argument <rotation_type> (args, 3) -> setValue (scaleOrientation);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.getTransform is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::transpose (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.transpose: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);

		return create <SFMatrix4> (ec, new InternalType (lhs -> transpose ()));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.transpose is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::inverse (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.inverse: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);

		return create <SFMatrix4> (ec, new InternalType (lhs -> inverse ()));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.inverse is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::multLeft (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multLeft: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);
		const auto rhs = get1Argument <SFMatrix4> (args, 0);

		return create <SFMatrix4> (ec, new InternalType (lhs -> multLeft (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multLeft is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::multRight (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multRight: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);
		const auto rhs = get1Argument <SFMatrix4> (args, 0);

		return create <SFMatrix4> (ec, new InternalType (lhs -> multRight (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multRight is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::multVecMatrix (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multVecMatrix: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);
		const auto rhs = get1Argument <vector3_type> (args, 0);

		return create <vector3_type> (ec, new typename InternalType::vector_type (lhs -> multVecMatrix (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multVecMatrix is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::multMatrixVec (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multMatrixVec: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);
		const auto rhs = get1Argument <vector3_type> (args, 0);

		return create <vector3_type> (ec, new typename InternalType::vector_type (lhs -> multMatrixVec (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multMatrixVec is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::multDirMatrix (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multDirMatrix: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);
		const auto rhs = get1Argument <vector3_type> (args, 0);

		return create <vector3_type> (ec, new typename InternalType::vector_type (lhs -> multDirMatrix (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multDirMatrix is not generic.");
	}
}

template <class InternalType>
pb::var
SFMatrix4 <InternalType>::multMatrixDir (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multMatrixDir: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFMatrix4> (ec, object);
		const auto rhs = get1Argument <vector3_type> (args, 0);

		return create <vector3_type> (ec, new typename InternalType::vector_type (lhs -> multMatrixDir (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multMatrixDir is not generic.");
	}
}

using SFMatrix4d = SFMatrix4 <X3D::SFMatrix4d>;
using SFMatrix4f = SFMatrix4 <X3D::SFMatrix4f>;
using VrmlMatrix = SFMatrix4 <X3D::VrmlMatrix>;

template <>
constexpr ObjectType
SFMatrix4d::getType ()
{
	return ObjectType::SFMatrix4d;
}

template <>
constexpr ObjectType
SFMatrix4f::getType ()
{
	return ObjectType::SFMatrix4f;
}

template <>
constexpr ObjectType
VrmlMatrix::getType ()
{
	return ObjectType::VrmlMatrix;
}

} // peaseblossom
} // X3D
} // titania

#endif

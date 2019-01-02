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

#include "MFNode.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <>
const JSClass MFNode::static_class = {
	"MFNode",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

template <>
bool
MFNode::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc == 0)
		{
			JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new X3D::MFNode ()));
			return true;
		}
		else
		{
			const auto args  = JS::CallArgsFromVp (argc, vp);
			const auto array = new X3D::MFNode ();

			for (unsigned i = 0; i < argc; ++ i)
			{
				try
				{
					array -> emplace_back (getArgument <SFNode> (cx, args, i));
				}
				catch (const std::domain_error &)
				{
					array -> emplace_back ();
				}	
			}

			args .rval () .set (create (cx, array));
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, error .what ());
	}
}

//template <>
//bool
//MFNode::set1Value (JSContext* cx, JSObject* obj, jsid id, bool strict, jsval* vp)
//{
//	try
//	{
//		if (not JSID_IS_INT (id))
//			return true;
//
//		const auto array = getThis <MFNode> (cx, obj);
//		const auto index = JSID_TO_INT (id);
//
//		if (index < 0)
//			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);
//
//		try
//		{
//			array -> set1Value (index, getArgument <SFNode> (cx, vp, 0));
//		}
//		catch (const std::domain_error &)
//		{
//			array -> set1Value (index, nullptr);
//		}
//
//		*vp = JSVAL_VOID;
//		return true;
//	}
//	catch (const std::bad_alloc &)
//	{
//		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
//	}
//}
//
//template <>
//bool
//MFNode::unshift (JSContext* cx, uint32_t argc, jsval* vp)
//{
//	if (argc == 0)
//		return ThrowException (cx, "%s .unshift: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv  = JS_ARGV (cx, vp);
//		const auto array = getThis <MFNode> (cx, vp);
//
//		for (ssize_t i = argc - 1; i >= 0; -- i)
//		{
//			try
//			{
//				array -> emplace_front (getArgument <SFNode> (cx, argv, i));
//			}
//			catch (const std::domain_error &)
//			{
//				array -> emplace_front (nullptr);
//			}
//		}
//
//		return JS_NewNumberValue (cx, array -> size (), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException (cx, "%s .unshift: %s.", getClass () -> name, error .what ());
//	}
//}
//
//template <>
//bool
//MFNode::push (JSContext* cx, uint32_t argc, jsval* vp)
//{
//	if (argc == 0)
//		return ThrowException (cx, "%s .push: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv  = JS_ARGV (cx, vp);
//		const auto array = getThis <MFNode> (cx, vp);
//
//		for (uint32_t i = 0; i < argc; ++ i)
//		{
//			try
//			{
//				array -> emplace_back (getArgument <SFNode> (cx, argv, i));
//			}
//			catch (const std::domain_error &)
//			{
//				array -> emplace_back (nullptr);
//			}
//		}
//
//		return JS_NewNumberValue (cx, array -> size (), vp);
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException (cx, "%s .push: %s.", getClass () -> name, error .what ());
//	}
//}
//
//template <>
//bool
//MFNode::splice (JSContext* cx, uint32_t argc, jsval* vp)
//{
//	if (argc < 2)
//		return ThrowException (cx, "%s .splice: wrong number of arguments.", getClass () -> name);
//
//	try
//	{
//		const auto argv        = JS_ARGV (cx, vp);
//		const auto array       = getThis <X3DArrayField> (cx, vp);
//		auto       index       = spidermonkey::getArgument <int32_t> (cx, argv, 0);
//		auto       deleteCount = spidermonkey::getArgument <int32_t> (cx, argv, 1);
//		auto       result      = new X3D::MFNode ();
//
//		if (index > (int32_t) array -> size ())
//			index = array -> size ();
//
//		if (index + deleteCount > (int32_t) array -> size ())
//			deleteCount = array -> size () - index;
//
//		result -> insert (result -> begin (), array -> begin () + index, array -> begin () + (index + deleteCount));
//		array -> erase (array -> begin () + index, array -> begin () + (index + deleteCount));
//
//		for (ssize_t i = argc - 1; i >= 2; -- i)
//		{
//			try
//			{
//				array -> emplace (array -> begin () + index, getArgument <SFNode> (cx, argv, i));
//			}
//			catch (const std::domain_error &)
//			{
//				array -> emplace (array -> begin () + index, nullptr);
//			}
//		}
//
//		return create <MFNode> (cx, result, &JS_RVAL (cx, vp));
//	}
//	catch (const std::exception & error)
//	{
//		return ThrowException (cx, "%s .unshift: %s.", getClass () -> name, error .what ());
//	}
//}

} // spidermonkey
} // X3D
} // titania

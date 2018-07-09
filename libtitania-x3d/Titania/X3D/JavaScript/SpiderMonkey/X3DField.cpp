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

#include "X3DField.h"

#include "../../Basic/X3DField.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSClass X3DField::static_class = {
	"X3DField", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSFunctionSpec X3DField::functions [ ] = {
	{ "getName",     getName,     0, 0 },
	{ "getTypeName", getTypeName, 0, 0 },
	{ "getType",     getType,     0, 0 },
	{ "isReadable",  isReadable,  0, 0 },
	{ "isWritable",  isWritable,  0, 0 },

	{ "equals",      equals,      1, 0 },
	{ "assign",      assign,      1, 0 },

	{ "toString",    toString,    0, 0 },

	{ 0 }

};

JSObject*
X3DField::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, nullptr, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JSBool
X3DField::getName (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getName: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <X3DField> (cx, vp);

		return JS_NewStringValue (cx, lhs -> getName (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getName: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DField::getTypeName (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getTypeName: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <X3DField> (cx, vp);

		return JS_NewStringValue (cx, lhs -> getTypeName (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getTypeName: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DField::getType (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getType: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <X3DField> (cx, vp);

		return JS_NewNumberValue (cx, lhs -> getType (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getType: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DField::isReadable (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .isReadable: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <X3DField> (cx, vp);

		JS_SET_RVAL (cx, vp, lhs -> getAccessType () not_eq inputOnly ? JSVAL_TRUE : JSVAL_FALSE);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isReadable: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DField::isWritable (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .isWritable: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <X3DField> (cx, vp);

		JS_SET_RVAL (cx, vp, lhs -> getAccessType () not_eq initializeOnly ? JSVAL_TRUE : JSVAL_FALSE);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isWritable: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DField::equals (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .equals: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <X3DField> (cx, vp);

		try
		{
			const auto rhs = getArgument <X3DField> (cx, argv, 0);
	
			if (lhs -> getType () not_eq rhs -> getType ())
				return ThrowException (cx, "%s .equals: both arguments must be of same type.");
	
			JS_SET_RVAL (cx, vp, lhs -> equals (*rhs) ? JSVAL_TRUE : JSVAL_FALSE);
			return true;
		}
		catch (const std::exception & error)
		{
			if (lhs -> getType () != X3D::X3DConstants::SFNode)
				throw;

			JS_SET_RVAL (cx, vp, static_cast <X3D::SFNode*> (lhs) -> getValue () == nullptr ? JSVAL_TRUE : JSVAL_FALSE);
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .equals: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DField::assign (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .assign: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <X3DField> (cx, vp);

		try
		{
			const auto rhs = getArgument <X3DField> (cx, argv, 0);
	
			if (lhs -> getType () not_eq rhs -> getType ())
				return ThrowException (cx, "%s .assign: both arguments must be of same type.");

			lhs -> set (*rhs);
			lhs -> addEvent ();

			JS_SET_RVAL (cx, vp, JSVAL_VOID);
			return true;
		}
		catch (const std::exception & error)
		{
			if (lhs -> getType () != X3D::X3DConstants::SFNode)
				throw;

			JS_SET_RVAL (cx, vp, static_cast <X3D::SFNode*> (lhs) -> getValue () == nullptr ? JSVAL_TRUE : JSVAL_FALSE);
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .assign: %s.", getClass () -> name, error .what ());
	}
}

JSBool
X3DField::toString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toString: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <X3DField> (cx, vp);

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		Generator::NicestStyle (osstream);

		lhs -> toStream (osstream);

		return JS_NewStringValue (cx, osstream .str (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toString: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // Titania

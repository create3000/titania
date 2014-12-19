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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DFIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DFIELD_H__

#include "../../InputOutput/Generator.h"
#include "jsArguments.h"
#include "jsContext.h"
#include "jsError.h"
#include "jsString.h"

#include <jsapi.h>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

class jsX3DField
{
protected:

	///  @name Construction

	template <class Type>
	static
	JSBool
	create (JSContext* const, JSClass* const, Type* const, jsval* const);

	///  @name Functions

	template <class Class>
	static
	JSBool
	getName (JSContext*, uint32_t, jsval*);

	template <class Class>
	static
	JSBool
	getTypeName (JSContext*, uint32_t, jsval*);

	template <class Class>
	static
	JSBool
	getType (JSContext*, uint32_t, jsval*);

	template <class Class>
	static
	JSBool
	isReadable (JSContext*, uint32_t, jsval*);

	template <class Class>
	static
	JSBool
	isWritable (JSContext*, uint32_t, jsval*);

	template <class Class>
	static
	JSBool
	toString (JSContext*, uint32_t, jsval*);

	///  @name Destruction

	static
	void
	finalize (JSContext*, JSObject*);

};

template <class InternalType>
JSBool
jsX3DField::create (JSContext* const cx, JSClass* const static_class, InternalType* const field, jsval* const vp)
{
	const auto context = getContext (cx);

	try
	{
		*vp = OBJECT_TO_JSVAL (context -> getObject (field));
	}
	catch (const std::out_of_range &)
	{
		const auto result = JS_NewObject (cx, static_class, nullptr, nullptr);

		if (result == nullptr)
			return ThrowException (cx, "out of memory");

		JS_SetPrivate (cx, result, field);

		context -> addObject (field, result);

		*vp = OBJECT_TO_JSVAL (result);
	}

	return true;
}

template <class Class>
JSBool
jsX3DField::getName (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getName: wrong number of arguments.", Class::getClass () -> name);

	try
	{
		const auto lhs = getThis <Class> (cx, vp);

		return JS_NewStringValue (cx, lhs -> getName (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getName: %s.", Class::getClass () -> name, error .what ());
	}
}

template <class Class>
JSBool
jsX3DField::getTypeName (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getTypeName: wrong number of arguments.", Class::getClass () -> name);

	try
	{
		const auto lhs = getThis <Class> (cx, vp);

		return JS_NewStringValue (cx, lhs -> getTypeName (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getTypeName: %s.", Class::getClass () -> name, error .what ());
	}
}

template <class Class>
JSBool
jsX3DField::getType (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getType: wrong number of arguments.", Class::getClass () -> name);

	try
	{
		const auto lhs = getThis <Class> (cx, vp);

		return JS_NewNumberValue (cx, lhs -> getType (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getType: %s.", Class::getClass () -> name, error .what ());
	}
}

template <class Class>
JSBool
jsX3DField::isReadable (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .isReadable: wrong number of arguments.", Class::getClass () -> name);

	try
	{
		const auto lhs = getThis <Class> (cx, vp);

		JS_SET_RVAL (cx, vp, lhs -> getAccessType () not_eq inputOnly ? JSVAL_TRUE : JSVAL_FALSE);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isReadable: %s.", Class::getClass () -> name, error .what ());
	}
}

template <class Class>
JSBool
jsX3DField::isWritable (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .isWritable: wrong number of arguments.", Class::getClass () -> name);

	try
	{
		const auto lhs = getThis <Class> (cx, vp);

		JS_SET_RVAL (cx, vp, lhs -> getAccessType () not_eq initializeOnly ? JSVAL_TRUE : JSVAL_FALSE);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isWritable: %s.", Class::getClass () -> name, error .what ());
	}
}

template <class Class>
JSBool
jsX3DField::toString (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .toString: wrong number of arguments.", Class::getClass () -> name);

	try
	{
		const auto lhs = getThis <Class> (cx, vp);

		Generator::NicestStyle ();

		return JS_NewStringValue (cx, lhs -> toString (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toString: %s.", Class::getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

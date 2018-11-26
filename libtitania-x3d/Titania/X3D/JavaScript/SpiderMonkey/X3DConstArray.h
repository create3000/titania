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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DCONST_ARRAY_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DCONST_ARRAY_H__

#include "Arguments.h"
#include "Error.h"
#include "ObjectType.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#include <js/jsapi.h>
#pragma GCC diagnostic pop

namespace titania {
namespace X3D {
namespace spidermonkey {

template <class Type, class InternalType>
class X3DConstArray
{
public:

	///  @name Member types

	using value_type    = Type;
	using internal_type = InternalType;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, const InternalType* const, jsval* const);

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

	enum Property {LENGTH};

	///  @name Member access

	static JSBool enumerate (JSContext*, JSObject*, JSIterateOp, jsval*, jsid*);

	static JSBool get1Value (JSContext*, JSObject*, jsid, jsval*);

	///  @name Properties

	static JSBool length (JSContext*, JSObject*, jsid, jsval*);

	///  @name Static members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];

};

template <class Type, class InternalType>
JSPropertySpec X3DConstArray <Type, InternalType>::properties [ ] = {
	{ "length", LENGTH, JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT, length, nullptr },
	{ 0 }

};

template <class Type, class InternalType>
JSObject*
X3DConstArray <Type, InternalType>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, nullptr, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

template <class Type, class InternalType>
JSBool
X3DConstArray <Type, InternalType>::create (JSContext* const cx, const InternalType* const array, jsval* const vp)
{
	const auto result = JS_NewObject (cx, &static_class, nullptr, nullptr);

	if (result == nullptr)
		return ThrowException (cx, "out of memory");

	JS_SetPrivate (cx, result, const_cast <InternalType*> (array));

	*vp = OBJECT_TO_JSVAL (result);

	return true;
}

template <class Type, class InternalType>
JSBool
X3DConstArray <Type, InternalType>::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	try
	{
		const auto array = getThis <X3DConstArray> (cx, obj);

		size_t* index;

		switch (enum_op)
		{
			case JSENUMERATE_INIT:
			case JSENUMERATE_INIT_ALL:
			{
				index   = new size_t (0);
				*statep = PRIVATE_TO_JSVAL (index);

				if (idp)
					*idp = INT_TO_JSID (array -> size ());

				break;
			}
			case JSENUMERATE_NEXT:
			{
				index = (size_t*) JSVAL_TO_PRIVATE (*statep);

				if (*index < array -> size ())
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
	catch (const std::exception &)
	{
		*statep = JSVAL_NULL;
		return true;
	}
}

template <class Type, class InternalType>
JSBool
X3DConstArray <Type, InternalType>::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		if (not JSID_IS_INT (id))
			return true;

		const auto array = getThis <X3DConstArray> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index < 0 or index >= (int32_t) array -> size ())
			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);

		return Type::create (cx, array -> at (index), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
X3DConstArray <Type, InternalType>::length (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto array = getThis <X3DConstArray> (cx, obj);

		return JS_NewNumberValue (cx, array -> size (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania

#endif

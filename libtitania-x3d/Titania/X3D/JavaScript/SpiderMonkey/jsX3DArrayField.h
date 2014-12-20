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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DARRAY_FIELD_H__

#include "jsContext.h"
#include "jsError.h"
#include "jsString.h"
#include "jsX3DField.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type, class InternalType>
class jsX3DArrayField :
	public jsX3DField
{
public:

	///  @name Member types

	using value_type    = Type;
	using internal_type = InternalType;
	using single_type   = typename InternalType::value_type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, InternalType* const, jsval* const);

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

	///  @name Construction

	static JSBool construct (JSContext*, uint32_t, jsval*);

	///  @name Member access

	static JSBool enumerate (JSContext*, JSObject*, JSIterateOp, jsval*, jsid*);
	static JSBool set1Value (JSContext*, JSObject*, jsid, JSBool, jsval*);
	static JSBool get1Value (JSContext*, JSObject*, jsid, jsval*);

	///  @name Functions

	static JSBool unshift (JSContext*, uint32_t, jsval*);
	static JSBool push    (JSContext*, uint32_t, jsval*);
	static JSBool shift   (JSContext*, uint32_t, jsval*);
	static JSBool pop     (JSContext*, uint32_t, jsval*);

	///  @name Properties

	static JSBool length (JSContext*, JSObject*, jsid, JSBool, jsval*);
	static JSBool length (JSContext*, JSObject*, jsid, jsval*);

	///  @name Static members

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class Type, class InternalType>
JSClass jsX3DArrayField <Type, InternalType>::static_class = {
	"X3DArrayField", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type, class InternalType>
JSPropertySpec jsX3DArrayField <Type, InternalType>::properties [ ] = {
	{ "length", LENGTH, JSPROP_SHARED | JSPROP_PERMANENT, length, length },
	{ 0 }

};

template <class Type, class InternalType>
JSFunctionSpec jsX3DArrayField <Type, InternalType>::functions [ ] = {
	{ "unshift",     unshift, 1, 0 },
	{ "push",        push,    1, 0 },
	{ "shift",       shift,   0, 0 },
	{ "pop",         pop,     0, 0 },

	{ 0 }

};

template <class Type, class InternalType>
JSObject*
jsX3DArrayField <Type, InternalType>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::create (JSContext* const cx, InternalType* const array, jsval* const vp)
{
	return jsX3DField::create (cx, &static_class, array, vp);
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		if (argc == 0)
		{
			return create (cx, new InternalType (), &JS_RVAL (cx, vp));
		}
		else
		{
			const auto array = new InternalType ();
			const auto argv  = JS_ARGV (cx, vp);

			for (uint32_t i = 0; i < argc; ++ i)
			{
				array -> emplace_back (*getArgument <value_type> (cx, argv, i));
			}

			return create (cx, array, &JS_RVAL (cx, vp));
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	try
	{
		const auto array = getThis <jsX3DArrayField> (cx, obj);

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
jsX3DArrayField <Type, InternalType>::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		if (not JSID_IS_INT (id))
			return true;

		const auto array = getThis <jsX3DArrayField> (cx, obj);
		const auto index = JSID_TO_INT (id);
		const auto value = getArgument <Type> (cx, vp, 0);

		if (index < 0)
			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);

		array -> set1Value (index, *value);

		*vp = JSVAL_VOID;
		return true;
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		if (not JSID_IS_INT (id))
			return true;

		const auto array = getThis <jsX3DArrayField> (cx, obj);
		const auto index = JSID_TO_INT (id);

		if (index < 0)
			return ThrowException (cx, "%s: array index out of range.", getClass () -> name);

		return Type::create (cx, &array -> get1Value (index), vp);
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::unshift (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .unshift: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv  = JS_ARGV (cx, vp);
		const auto array = getThis <jsX3DArrayField> (cx, vp);
		const auto value = getArgument <Type> (cx, argv, 0);

		array -> emplace_front (*value);

		return JS_NewNumberValue (cx, array -> size (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .unshift: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::push (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .push: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv  = JS_ARGV (cx, vp);
		const auto array = getThis <jsX3DArrayField> (cx, vp);
		const auto value = getArgument <Type> (cx, argv, 0);

		array -> emplace_back (*value);

		return JS_NewNumberValue (cx, array -> size (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .push: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::shift (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .shift: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto array = getThis <jsX3DArrayField> (cx, vp);

		if (array -> empty ())
		{
			*vp = JSVAL_VOID;
			return true;
		}

		const auto value = new single_type (array -> front ());

		array -> pop_front ();

		return Type::create (cx, value, vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .shift: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::pop (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .pop: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto array = getThis <jsX3DArrayField> (cx, vp);

		if (array -> empty ())
		{
			*vp = JSVAL_VOID;
			return true;
		}

		const auto value = new single_type (array -> back ());

		array -> pop_back ();

		return Type::create (cx, value, vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .pop: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::length (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto array = getThis <jsX3DArrayField> (cx, obj);
		const auto size  = getArgument <uint32_t> (cx, vp, 0);

		array -> resize (size);

		return true;
	}
	catch (const std::bad_alloc &)
	{
		return ThrowException (cx, "%s: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <class Type, class InternalType>
JSBool
jsX3DArrayField <Type, InternalType>::length (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto array = getThis <jsX3DArrayField> (cx, obj);

		return JS_NewNumberValue (cx, array -> size (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

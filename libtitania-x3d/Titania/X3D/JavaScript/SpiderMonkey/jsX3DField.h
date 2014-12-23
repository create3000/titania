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
#include "jsObjectType.h"
#include "jsString.h"

#include <jsapi.h>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

class jsX3DField
{
public:

	///  @name Member types

	using internal_type = X3D::X3DFieldDefinition;

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
	{ return ObjectType::X3DField; }


protected:

	///  @name Construction

	template <class Type>
	static
	JSBool
	create (JSContext* const, JSClass* const, Type* const, jsval* const);

	///  @name Destruction

	static
	void
	finalize (JSContext*, JSObject*);


private:

	///  @name Functions

	static JSBool getName (JSContext*, uint32_t, jsval*);
	static JSBool getTypeName (JSContext*, uint32_t, jsval*);
	static JSBool getType (JSContext*, uint32_t, jsval*);
	static JSBool isReadable (JSContext*, uint32_t, jsval*);
	static JSBool isWritable (JSContext*, uint32_t, jsval*);
	static JSBool toString (JSContext*, uint32_t, jsval*);

	///  @name Static members

	static JSClass        static_class;
	static JSFunctionSpec functions [ ];

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

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DFIELD_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_JS_X3DFIELD_H__

#include "../../InputOutput/Generator.h"
#include "Arguments.h"
#include "Context.h"
#include "Error.h"
#include "ObjectType.h"
#include "String.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wignored-attributes"
#include <js/jsapi.h>
#pragma GCC diagnostic pop

namespace titania {
namespace X3D {
namespace spidermonkey {

class X3DField
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

	///  @name Construction

	template <class Class>
	static
	JSBool
	get (JSContext* const cx, const typename Class::internal_type & field, jsval* const vp);

	template <class Class>
	static
	JSBool
	create (JSContext* const cx, typename Class::internal_type* const field, jsval* const vp);


protected:

	///  @name Operations

	template <class Class>
	static
	X3D::X3DChildObject*
	getKey (typename Class::internal_type* const field)
	{ return field; }

	///  @name Destruction

	template <class Class>
	static
	void
	finalize (JSContext*, JSObject*);


private:

	///  @name Functions

	static JSBool getName     (JSContext* cx, uint32_t argc, jsval* vp);
	static JSBool getTypeName (JSContext* cx, uint32_t argc, jsval* vp);
	static JSBool getType     (JSContext* cx, uint32_t argc, jsval* vp);
	static JSBool isReadable  (JSContext* cx, uint32_t argc, jsval* vp);
	static JSBool isWritable  (JSContext* cx, uint32_t argc, jsval* vp);

	static JSBool equals (JSContext* cx, uint32_t argc, jsval* vp);
	static JSBool assign (JSContext* cx, uint32_t argc, jsval* vp);

	static JSBool toString (JSContext* cx, uint32_t argc, jsval* vp);

	///  @name Static members

	static JSClass        static_class;
	static JSFunctionSpec functions [ ];

};

template <class Class>
JSBool
X3DField::get (JSContext* const cx, const typename Class::internal_type & field, jsval* const vp)
{
	const auto context = getContext (cx);
	const auto object  = context -> getObject (getKey <Class> (const_cast <typename Class::internal_type*> (&field)));

	if (object)
	{
		*vp = OBJECT_TO_JSVAL (object);
		return true;
	}

	return create <Class> (cx, const_cast <typename Class::internal_type*> (&field), vp);
}

template <class Class>
JSBool
X3DField::create (JSContext* const cx, typename Class::internal_type* const field, jsval* const vp)
{
	const auto context = getContext (cx);
	const auto result  = JS_NewObject (cx, Class::getClass (), nullptr, nullptr);

	if (result == nullptr)
		return ThrowException (cx, "out of memory");

	JS_SetPrivate (cx, result, field);

	context -> addObject (getKey <Class> (field), field, result);

	*vp = OBJECT_TO_JSVAL (result);
	return true;
}

template <class Class>
void
X3DField::finalize (JSContext* cx, JSObject* obj)
{
	const auto context = getContext (cx);
	const auto field   = getObject <typename Class::internal_type*> (cx, obj);

	// Proto objects have no private

	if (field)
		context -> removeObject (getKey <Class> (field), field);
}

} // spidermonkey
} // X3D
} // titania

#endif

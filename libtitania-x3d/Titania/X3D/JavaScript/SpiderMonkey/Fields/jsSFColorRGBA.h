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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFCOLOR_RGBA_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFCOLOR_RGBA_H__

#include "../../../Fields/SFColorRGBA.h"
#include "../jsX3DField.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

class jsSFColorRGBA :
	public jsX3DField
{
public:

	///  @name Member types

	using internal_type = X3D::SFColorRGBA;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JS::Value
	create (JSContext* const, X3D::SFColorRGBA* const)
	throw (std::invalid_argument);

	static
	JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr ObjectType
	getId ()
	{ return ObjectType::SFColorRGBA; }


private:

	///  @name Member types

	enum Property {R, G, B, A};

	///  @name Construction

	static JSBool construct (JSContext*, unsigned, JS::Value*);
	
	///  @name Member access

	static JSBool set1Value (JSContext*, JS::HandleObject, JS::HandleId, JSBool, JS::MutableHandleValue);
	static JSBool get1Value (JSContext*, JS::HandleObject, JS::HandleId, JS::MutableHandleValue);

	template <size_t Index>
	static JSBool setProperty (JSContext*, unsigned, JS::Value*);

	template <size_t Index>
	static JSBool getProperty (JSContext*, unsigned, JS::Value*);

	///  @name Functions

	static JSBool getHSV (JSContext*, unsigned, JS::Value*);
	static JSBool setHSV (JSContext*, unsigned, JS::Value*);

	///  @name Static members

	static constexpr size_t size = 4;

	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <size_t Index>
JSBool
jsSFColorRGBA::setProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFColorRGBA> (cx, args);
		const auto rhs  = getArgument <double> (cx, args, 0);

		lhs -> set1Value (Index, rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .x: %s.", getClass () -> name, error .what ());
	}
}

template <size_t Index>
JSBool
jsSFColorRGBA::getProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFColorRGBA> (cx, args);

		args .rval () .setDouble (lhs -> get1Value (Index));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .x: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif

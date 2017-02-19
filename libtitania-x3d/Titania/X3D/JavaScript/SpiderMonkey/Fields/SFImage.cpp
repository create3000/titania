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

#include "SFImage.h"

#include "../Context.h"
#include "../Error.h"
#include "ArrayFields.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

JSClass SFImage::static_class = {
	"SFImage", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, finalize <SFImage>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec SFImage::properties [ ] = {
	{ "width",  WIDTH,  JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT, width,  width },
	{ "x",      WIDTH,                     JSPROP_SHARED | JSPROP_PERMANENT, width,  width },  // VRML
	{ "height", HEIGHT, JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT, height, height },
	{ "y",      HEIGHT,                    JSPROP_SHARED | JSPROP_PERMANENT, height, height }, // VRML
	{ "comp",   COMP,   JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT, comp,   comp },
	{ "array",  ARRAY,  JSPROP_ENUMERATE | JSPROP_SHARED | JSPROP_PERMANENT, array,  array },
	{ 0 }

};

JSObject*
SFImage::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, nullptr, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JSBool
SFImage::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create <SFImage> (cx, new X3D::SFImage (), &JS_RVAL (cx, vp));
			}
			case 4:
			{
				const auto argv   = JS_ARGV (cx, vp);
				const auto width  = getArgument <uint32_t> (cx, argv, WIDTH);
				const auto height = getArgument <uint32_t> (cx, argv, HEIGHT);
				const auto comp   = getArgument <uint32_t> (cx, argv, COMP);
				const auto array  = getArgument <MFInt32> (cx, argv, ARRAY);

				return create <SFImage> (cx, new X3D::SFImage (width, height, comp, *array), &JS_RVAL (cx, vp));
			}
			default:
				return ThrowException (cx, "%s .new: wrong number of arguments.", getClass () -> name);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::width (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <SFImage> (cx, obj);

		return JS_NewNumberValue (cx, lhs -> getWidth (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .width: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::width (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFImage> (cx, obj);
		const auto value = getArgument <int32_t> (cx, vp, 0);

		if (value < 0)
			throw std::runtime_error ("value less than 0");

		lhs -> setWidth (value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .width: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::height (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <SFImage> (cx, obj);

		return JS_NewNumberValue (cx, lhs -> getHeight (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .height: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::height (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFImage> (cx, obj);
		const auto value = getArgument <int32_t> (cx, vp, 0);

		if (value < 0)
			throw std::runtime_error ("value less than 0");

		lhs -> setHeight (value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .height: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::comp (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <SFImage> (cx, obj);

		return JS_NewNumberValue (cx, lhs -> getComponents (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .comp: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::comp (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFImage> (cx, obj);
		const auto value = getArgument <int32_t> (cx, vp, 0);

		if (value < 0 or value > 4)
			throw std::runtime_error ("value less than 0 or greater than 4");

		lhs -> setComponents (value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .comp: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::array (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <SFImage> (cx, obj);

		return create <MFInt32> (cx, &lhs -> getArray (), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .array: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFImage::array (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFImage> (cx, obj);
		const auto value = getArgument <MFInt32> (cx, vp, 0);

		lhs -> setArray (*value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .array: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania

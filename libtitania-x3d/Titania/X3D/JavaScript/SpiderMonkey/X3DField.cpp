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

#include "X3DField.h"

#include "../../Basic/X3DField.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DField::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	nullptr, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass X3DField::static_class = {
	"X3DField",
	JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSFunctionSpec X3DField::functions [ ] = {
	JS_FS ("getName",     getName,     0, JSPROP_PERMANENT),
	JS_FS ("getTypeName", getTypeName, 0, JSPROP_PERMANENT),
	JS_FS ("getType",     getType,     0, JSPROP_PERMANENT),
	JS_FS ("isReadable",  isReadable,  0, JSPROP_PERMANENT),
	JS_FS ("isWritable",  isWritable,  0, JSPROP_PERMANENT),
	JS_FS ("toString",    toString,    0, JSPROP_PERMANENT),
	{ 0 }
};

JSObject*
X3DField::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, nullptr, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

bool
X3DField::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException (cx, "%s .new: %s.", getClass () -> name, "object is not constructable");
}

bool
X3DField::getName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .getName: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto field = getThis <X3DField> (cx, args);

		args .rval () .set (StringValue (cx, field -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getName: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::getTypeName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .getTypeName: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto field = getThis <X3DField> (cx, args);

		args .rval () .set (StringValue (cx, field -> getTypeName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getTypeName: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::getType (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .getType: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto field = getThis <X3DField> (cx, args);

		args .rval () .setInt32 (field -> getType ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getType: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::isReadable (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .isReadable: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto field = getThis <X3DField> (cx, args);

		args .rval () .setBoolean (field -> getAccessType () not_eq inputOnly);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isReadable: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::isWritable (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .isWritable: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto field = getThis <X3DField> (cx, args);

		args .rval () .setBoolean (field -> getAccessType () not_eq initializeOnly);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .isWritable: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::toString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException (cx, "%s .toString: wrong number of arguments.", getClass () -> name);
	
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto field = getThis <X3DField> (cx, args);

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		Generator::NicestStyle (osstream);

		field -> toStream (osstream);

		args .rval () .set (StringValue (cx, osstream .str ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .toString: %s.", getClass () -> name, error .what ());
	}
}

void
X3DField::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = static_cast <Context*> (JS_GetReservedSlot (obj, SLOT_CONTEXT) .toPrivate ());
	const auto field   = getObject <X3D::X3DFieldDefinition*> (obj);

	// Proto objects have no private.

	if (field)
		context -> removeObject (field);
}

} // spidermonkey
} // X3D
} // Titania

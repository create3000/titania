/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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
#include "../../InputOutput/Generator.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps X3DField::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // enumerate
	nullptr, // newEnumerate
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
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec X3DField::properties [ ] = {
	JS_PS_END
};

const JSFunctionSpec X3DField::functions [ ] = {
	JS_FN ("getName",     getName,     0, JSPROP_PERMANENT),
	JS_FN ("getTypeName", getTypeName, 0, JSPROP_PERMANENT),
	JS_FN ("getType",     getType,     0, JSPROP_PERMANENT),
	JS_FN ("isReadable",  isReadable,  0, JSPROP_PERMANENT),
	JS_FN ("isWritable",  isWritable,  0, JSPROP_PERMANENT),
	JS_FN ("toString",    toString,    0, JSPROP_PERMANENT),

	JS_FN ("equals",      equals,      1, JSPROP_PERMANENT),
	JS_FN ("assign",      assign,      1, JSPROP_PERMANENT),

	JS_FS_END
};

JSObject*
X3DField::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

bool
X3DField::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

bool
X3DField::getName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getName: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <X3DField> (cx, args);

		args .rval () .set (StringValue (cx, self -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getName: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::getTypeName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getTypeName: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <X3DField> (cx, args);

		args .rval () .set (StringValue (cx, self -> getTypeName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getTypeName: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::getType (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getType: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <X3DField> (cx, args);

		args .rval () .setInt32 (self -> getType ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getType: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::isReadable (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .isReadable: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <X3DField> (cx, args);

		args .rval () .setBoolean (self -> getAccessType () not_eq inputOnly);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .isReadable: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::isWritable (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .isWritable: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <X3DField> (cx, args);

		args .rval () .setBoolean (self -> getAccessType () not_eq initializeOnly);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .isWritable: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::equals (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .equals: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <X3DField> (cx, args);

		try
		{
			const auto field = getArgument <X3DField> (cx, args, 0);

			if (self -> getType () not_eq field -> getType ())
				return ThrowException <JSProto_Error> (cx, "%s .prototype .equals: both arguments must be of same type.");

			args .rval () .setBoolean (self -> equals (*field));
			return true;
		}
		catch (const std::domain_error & error)
		{
			if (self -> getType () != X3D::X3DConstants::SFNode)
				throw;

			args .rval () .setBoolean (static_cast <X3D::SFNode*> (self) -> getValue () == nullptr);
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .equals: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::assign (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .assign: wrong number of arguments.", getClass () -> name);

		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto self  = getThis <X3DField> (cx, args);
		const auto field = getArgument <X3DField> (cx, args, 0);

		if (self -> getType () == X3D::X3DConstants::SFNode)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .assign: invalid operation.");

		if (self -> getType () not_eq field -> getType ())
			return ThrowException <JSProto_Error> (cx, "%s .prototype .assign: both arguments must be of same type.");

		self -> set (*field);
		self -> addEvent ();

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .assign: %s.", getClass () -> name, error .what ());
	}
}

bool
X3DField::toString (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .toString: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <X3DField> (cx, args);

		std::ostringstream osstream;

		osstream .imbue (std::locale::classic ());

		Generator::NicestStyle (osstream);

		self -> toStream (osstream);

		args .rval () .set (StringValue (cx, osstream .str ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .toString: %s.", getClass () -> name, error .what ());
	}
}

void
X3DField::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = getContext (obj);
	const auto self    = getObject <internal_type*> (obj);

	// Proto objects have no private.

	if (self)
	{
		const auto array = getArray (obj);

		context -> removeObject (spidermonkey::getKey (obj));

		if (array)
			context -> removeReference (array, getIndex (obj));
	}
}

} // spidermonkey
} // X3D
} // Titania

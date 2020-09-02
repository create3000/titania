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

#include "FieldDefinitionArray.h"

#include "Arguments.h"
#include "Context.h"
#include "Error.h"
#include "SlotType.h"

#include "X3DFieldDefinition.h"

extern "C"
{
	#include "C-bind/bind.h"
}

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps FieldDefinitionArray::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // enumerate
	nullptr, // newEnumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass FieldDefinitionArray::static_class = {
	"FieldDefinitionArray",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec FieldDefinitionArray::properties [ ] = {
	JS_PSGS ("length", getLength, nullptr, JSPROP_PERMANENT),
	JS_PS_END
};

const JSFunctionSpec FieldDefinitionArray::functions [ ] = {
	JS_FS_END
};

JSObject*
FieldDefinitionArray::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

bool
FieldDefinitionArray::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, "object is not constructible");
}

JS::Value
FieldDefinitionArray::create (JSContext* const cx, const X3D::FieldDefinitionArray & fieldDefinitions)
{
	const auto context = getContext (cx);
	const auto key     = size_t (&fieldDefinitions);
	const auto obj     = context -> getObject (key);

	if (obj)
	{
		return JS::ObjectValue (*obj);
	}
	else
	{
		const auto obj = JS_NewObjectWithGivenProto (cx, getClass (), context -> getProto (getId ()));

		if (not obj)
			throw std::runtime_error ("out of memory");

		const auto array = new internal_type (fieldDefinitions .begin (), fieldDefinitions .end ());

		setObject (obj, array);
		setContext (obj, context);
		setKey (obj, key);

		context -> addObject (key, array, obj);

		const auto object = JS::RootedObject (cx, obj);

		for (size_t i = 0, size = array -> size (); i < size; ++ i)
		{
			JS_DefineProperty (cx,
			                   object,
			                   basic::to_string (i, std::locale::classic ()) .c_str (),
			                   JSNative (partial_bind ((void*) &FieldDefinitionArray::get1Value, 4, 1, i)),
			                   nullptr,
			                   JSPROP_PERMANENT | JSPROP_ENUMERATE);
		}

		return JS::ObjectValue (*obj);
	}
}

bool
FieldDefinitionArray::get1Value (const size_t index, JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <FieldDefinitionArray> (cx, args);

		args .rval () .set (X3DFieldDefinition::create (cx, (*array) [index]));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, index, error .what ());
	}
}

bool
FieldDefinitionArray::getLength (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto array = getThis <FieldDefinitionArray> (cx, args);

		args .rval () .setNumber (uint32_t (array -> size ()));
		return true;
	}
	catch (const std::bad_alloc & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: out of memory.", getClass () -> name);
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

void
FieldDefinitionArray::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context = getContext (obj);
	const auto array   = getObject <internal_type*> (obj);

	// Proto objects have no private.

	if (array)
		context -> removeObject (getKey (obj));
}

} // spidermonkey
} // X3D
} // titania

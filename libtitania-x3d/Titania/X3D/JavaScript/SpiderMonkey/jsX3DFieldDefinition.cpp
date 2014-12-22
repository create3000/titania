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

#include "jsX3DFieldDefinition.h"

#include "jsArguments.h"
#include "jsContext.h"
#include "jsError.h"
#include "jsString.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsX3DFieldDefinition::static_class = {
	"X3DFieldDefinition",
	JSCLASS_HAS_PRIVATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	JS_PropertyStub,
	JS_StrictPropertyStub,
	JS_EnumerateStub,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsX3DFieldDefinition::properties [ ] = {
	JS_PSG ("name",       getName,       JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSG ("accessType", getAccessType, JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PSG ("dataType",   getDataType,   JSPROP_ENUMERATE | JSPROP_READONLY | JSPROP_SHARED | JSPROP_PERMANENT),
	JS_PS_END

};

JSObject*
jsX3DFieldDefinition::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, nullptr, 0, properties, nullptr, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JS::Value
jsX3DFieldDefinition::create (JSContext* const cx, const X3D::X3DFieldDefinition* const field)
throw (std::invalid_argument)
{
	const auto result = JS_NewObject (cx, &static_class, nullptr, nullptr);

	if (result == nullptr)
		throw std::invalid_argument ("out of memory");

	const auto context  = getContext (cx);
	const auto fieldPtr = new X3D::FieldPtr (const_cast <X3D::X3DFieldDefinition*> (field));

	fieldPtr -> addParent (context);

	JS_SetPrivate (result, fieldPtr);

	return JS::ObjectValue (*result);
}

JSBool
jsX3DFieldDefinition::getName (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args     = JS::CallArgsFromVp (argc, vp);
		const auto & fieldPtr = *getThis <jsX3DFieldDefinition> (cx, args);

		args .rval () .set (StringValue (cx, fieldPtr -> getName ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .name: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsX3DFieldDefinition::getAccessType (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args     = JS::CallArgsFromVp (argc, vp);
		const auto & fieldPtr = *getThis <jsX3DFieldDefinition> (cx, args);

		args .rval () .setInt32 (fieldPtr -> getAccessType ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .accessType: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsX3DFieldDefinition::getDataType (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto   args     = JS::CallArgsFromVp (argc, vp);
		const auto & fieldPtr = *getThis <jsX3DFieldDefinition> (cx, args);

		args .rval () .setInt32 (fieldPtr -> getType ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .accessType: %s.", getClass () -> name, error .what ());
	}
}

void
jsX3DFieldDefinition::finalize (JSFreeOp* fop, JSObject* obj)
{
	const auto context  = getContext (fop -> runtime ());
	const auto fieldPtr = getObject <X3D::FieldPtr*> (obj);

	// Proto objects have no private

	if (fieldPtr)
		fieldPtr -> removeParent (context);
}

} // MozillaSpiderMonkey
} // X3D
} // titania

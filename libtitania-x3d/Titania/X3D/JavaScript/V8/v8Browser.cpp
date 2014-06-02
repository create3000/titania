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

#include "v8Browser.h"

#include "../../Browser/X3DBrowser.h"
#include "v8String.h"

namespace titania {
namespace X3D {

void
v8Browser::initialize (v8Context* const javaScript, const v8::Local <v8::Object> & globalObject)
{
	const auto browserClass = v8::FunctionTemplate::New ();

	browserClass -> SetClassName (make_v8_string ("Browser"));

	// X3D properties

	const auto prototype = browserClass -> InstanceTemplate ();

	prototype -> SetAccessor (make_v8_string ("name"),                name,                nullptr,     v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	prototype -> SetAccessor (make_v8_string ("version"),             version,             nullptr,     v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	prototype -> SetAccessor (make_v8_string ("currentSpeed"),        currentSpeed,        nullptr,     v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	prototype -> SetAccessor (make_v8_string ("currentFrameRate"),    currentFrameRate,    nullptr,     v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	prototype -> SetAccessor (make_v8_string ("description"),         description,         description, v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::DontDelete));
//	prototype -> SetAccessor (make_v8_string ("supportedComponents"), supportedComponents, nullptr,     v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
//	prototype -> SetAccessor (make_v8_string ("supportedProfiles"),   supportedProfiles,   nullptr,     v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete);
	prototype -> SetAccessor (make_v8_string ("currentScene"),        currentScene,        nullptr,     v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));

	globalObject -> Set (make_v8_string ("Browser"), browserClass -> GetFunction () -> NewInstance (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
}

v8::Handle <v8::Value>
v8Browser::name (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = get_v8_context (info) -> getBrowser ();

	return make_v8_string (browser -> getName ());
}

v8::Handle <v8::Value>
v8Browser::version (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = get_v8_context (info) -> getBrowser ();

	return make_v8_string (browser -> getVersion ());
}

v8::Handle <v8::Value>
v8Browser::currentSpeed (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = get_v8_context (info) -> getBrowser ();

	return v8::Number::New (browser -> getCurrentSpeed ());
}

v8::Handle <v8::Value>
v8Browser::currentFrameRate (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = get_v8_context (info) -> getBrowser ();

	return v8::Number::New (browser -> getCurrentFrameRate ());
}

void
v8Browser::description (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	const auto browser = get_v8_context (info) -> getBrowser ();

	browser -> setDescription (*v8::String::Utf8Value (value));
}

v8::Handle <v8::Value>
v8Browser::description (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = get_v8_context (info) -> getBrowser ();

	return make_v8_string (browser -> getDescription ());
}

//v8::Handle <v8::Value>
//v8Browser::supportedComponents (v8::Local <v8::String> property, const v8::AccessorInfo & info)
//{
//	const auto browser = get_v8_context (info) -> getBrowser ();
//
//	return make_v8_string (browser -> getName ());
//}

//v8::Handle <v8::Value>
//v8Browser::supportedProfiles (v8::Local <v8::String> property, const v8::AccessorInfo & info)
//{
//	const auto browser = get_v8_context (info) -> getBrowser ();
//
//	return make_v8_string (browser -> getName ());
//}

v8::Handle <v8::Value>
v8Browser::currentScene (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = get_v8_context (info) -> getBrowser ();

	return make_v8_string (browser -> getName ());
}

} // X3D
} // titania

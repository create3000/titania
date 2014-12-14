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

#include "Browser.h"

#include "../../Browser/X3DBrowser.h"
#include "String.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

void
Browser::initialize (const v8::Local <v8::External> & context, const v8::Local <v8::Object> & global)
{
	const auto functionTemplate = v8::FunctionTemplate::New ();

	functionTemplate -> SetClassName (String ("Browser"));

	// X3D properties

	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetAccessor (String ("name"),                name,                nullptr,     context, v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	instanceTemplate -> SetAccessor (String ("version"),             version,             nullptr,     context, v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	instanceTemplate -> SetAccessor (String ("currentSpeed"),        currentSpeed,        nullptr,     context, v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	instanceTemplate -> SetAccessor (String ("currentFrameRate"),    currentFrameRate,    nullptr,     context, v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
	instanceTemplate -> SetAccessor (String ("description"),         description,         description, context, v8::DEFAULT, v8::PropertyAttribute (v8::DontDelete));
//	instanceTemplate -> SetAccessor (String ("supportedComponents"), supportedComponents, nullptr,     context, v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
//	instanceTemplate -> SetAccessor (String ("supportedProfiles"),   supportedProfiles,   nullptr,     context, v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete);
	instanceTemplate -> SetAccessor (String ("currentScene"),        currentScene,        nullptr,     context, v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));

	// VRML functions

	instanceTemplate -> Set (String ("getName"),              v8::FunctionTemplate::New (getName,              context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("getVersion"),           v8::FunctionTemplate::New (getVersion,           context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("getCurrentSpeed"),      v8::FunctionTemplate::New (getCurrentSpeed,      context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("getCurrentFrameRate"),  v8::FunctionTemplate::New (getCurrentFrameRate,  context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("getWorldURL"),          v8::FunctionTemplate::New (getWorldURL,          context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("setDescription"),       v8::FunctionTemplate::New (setDescription,       context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("createVrmlFromString"), v8::FunctionTemplate::New (createVrmlFromString, context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("createVrmlFromURL"),    v8::FunctionTemplate::New (createVrmlFromURL,    context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("addRoute"),             v8::FunctionTemplate::New (addRoute,             context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));
	instanceTemplate -> Set (String ("deleteRoute"),          v8::FunctionTemplate::New (deleteRoute,          context) -> GetFunction (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));

	global -> Set (String ("Browser"), functionTemplate -> GetFunction () -> NewInstance (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
}

// X3D properties

v8::Handle <v8::Value>
Browser::name (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = getContext (info) -> getBrowser ();

	return String (browser -> getName ());
}

v8::Handle <v8::Value>
Browser::version (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = getContext (info) -> getBrowser ();

	return String (browser -> getVersion ());
}

v8::Handle <v8::Value>
Browser::currentSpeed (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = getContext (info) -> getBrowser ();

	return v8::Number::New (browser -> getCurrentSpeed ());
}

v8::Handle <v8::Value>
Browser::currentFrameRate (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = getContext (info) -> getBrowser ();

	return v8::Number::New (browser -> getCurrentFrameRate ());
}

void
Browser::description (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	const auto browser = getContext (info) -> getBrowser ();

	browser -> setDescription (to_string (value));
}

v8::Handle <v8::Value>
Browser::description (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = getContext (info) -> getBrowser ();

	return String (browser -> getDescription ());
}

//v8::Handle <v8::Value>
//Browser::supportedComponents (v8::Local <v8::String> property, const v8::AccessorInfo & info)
//{
//	const auto browser = getContext (info) -> getBrowser ();
//
//	return v8::Undefined ();
//}

//v8::Handle <v8::Value>
//Browser::supportedProfiles (v8::Local <v8::String> property, const v8::AccessorInfo & info)
//{
//	const auto browser = getContext (info) -> getBrowser ();
//
//	return v8::Undefined ();
//}

v8::Handle <v8::Value>
Browser::currentScene (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	//const auto browser = getContext (info) -> getBrowser ();

	return v8::Undefined ();
}

// VRML functions

v8::Handle <v8::Value>
Browser::getName (const v8::Arguments & args)
{
	const auto browser = getContext (args) -> getBrowser ();

	return String (browser -> getName ());
}

v8::Handle <v8::Value>
Browser::getVersion (const v8::Arguments & args)
{
	const auto browser = getContext (args) -> getBrowser ();

	return String (browser -> getVersion ());
}

v8::Handle <v8::Value>
Browser::getCurrentSpeed (const v8::Arguments & args)
{
	const auto browser = getContext (args) -> getBrowser ();

	return v8::Number::New (browser -> getCurrentSpeed ());
}

v8::Handle <v8::Value>
Browser::getCurrentFrameRate (const v8::Arguments & args)
{
	const auto browser = getContext (args) -> getBrowser ();

	return v8::Number::New (browser -> getCurrentFrameRate ());
}

v8::Handle <v8::Value>
Browser::getWorldURL (const v8::Arguments & args)
{
	const auto browser = getContext (args) -> getBrowser ();

	return String (browser -> getWorldURL ());
}

v8::Handle <v8::Value>
Browser::setDescription (const v8::Arguments & args)
{
	if (args. Length () == 1)
	{
		const auto browser = getContext (args) -> getBrowser ();

		browser -> setDescription (to_string (args [0]));

		return v8::Undefined ();
	}

	return v8::ThrowException (String ("RuntimeError: wrong number of arguments."));
}

v8::Handle <v8::Value>
Browser::createVrmlFromString (const v8::Arguments & args)
{
	//const auto browser = getContext (args) -> getBrowser ();

	return v8::Undefined ();
}

v8::Handle <v8::Value>
Browser::createVrmlFromURL (const v8::Arguments & args)
{
	//const auto browser = getContext (args) -> getBrowser ();

	return v8::Undefined ();
}

v8::Handle <v8::Value>
Browser::addRoute (const v8::Arguments & args)
{
	//const auto browser = getContext (args) -> getBrowser ();

	return v8::Undefined ();
}

v8::Handle <v8::Value>
Browser::deleteRoute (const v8::Arguments & args)
{
	//const auto browser = getContext (args) -> getBrowser ();

	return v8::Undefined ();
}

} // GoogleV8
} // X3D
} // titania

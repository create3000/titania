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

#include "Context.h"

#include "../../Browser/X3DBrowser.h"

#include "Browser.h"
#include "Fields.h"
#include "Globals.h"
#include "String.h"

#include <cassert>

namespace titania {
namespace X3D {
namespace GoogleV8 {

const ComponentType Context::component      = ComponentType::TITANIA;
const std::string   Context::typeName       = "Context";
const std::string   Context::containerField = "context";

Context::Context (Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3D::X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3D::X3DJavaScriptContext (script, ecmascript),
	                 worldURL ({ uri }),
	                  isolate (v8::Isolate::New ()),
	                  context (),
	                  program (),
	                  classes (),
	                  objects (),
	                functions (),
	             initializeFn (),
	          prepareEventsFn (),
	        eventsProcessedFn (),
	               shutdownFn ()
{
	__LOG__ << std::endl;

	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;

	context = v8::Context::New (nullptr, v8::ObjectTemplate::New ());

	v8::Context::Scope contextScope (context);

	setContext ();
	setFields ();

	// Compile.

	v8::TryCatch trycatch;
	program = v8::Persistent <v8::Script>::New (v8::Script::Compile (String (getECMAScript ()),
	                                                                 String (worldURL .back () == getExecutionContext () -> getWorldURL ()
	                                                                         ? "<inline>"
																									 : worldURL .back ())));

	if (program .IsEmpty ())
	{
		error (trycatch);
		throw std::invalid_argument ("Couldn't not compile JavaScript.");
	}
}

X3DBaseNode*
Context::create (X3DExecutionContext* const) const
{
	return new Context (getScriptNode (), getECMAScript (), worldURL .front ());
}

void
Context::setContext ()
{
	const auto global   = context -> Global ();
	const auto external = v8::External::New (this);

	Globals::initialize (external, global);
	Browser::initialize (external, global);

	classes .resize (size_t (ObjectType::SIZE));

	addClass (SFVec3d::Type (), SFVec3d::initialize (external));
	addClass (SFVec3f::Type (), SFVec3f::initialize (external));
	addClass (SFVec4d::Type (), SFVec4d::initialize (external));
	addClass (SFVec4f::Type (), SFVec4f::initialize (external));
}

void
Context::setFields ()
{ }

void
Context::addClass (const ObjectType type, const v8::Local <v8::FunctionTemplate> & functionTemplate)
{
	const auto function = functionTemplate -> GetFunction ();

	context -> Global () -> Set (function -> GetName () -> ToString (), function, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));

	classes [size_t (type)] = v8::Persistent <v8::FunctionTemplate>::New (functionTemplate);
}

v8::Local <v8::Object>
Context::createObject (const ObjectType type, X3D::X3DFieldDefinition* const field) const
throw (std::out_of_range)
{
	constexpr size_t argc = 1;

	v8::Handle <v8::Value> argv [argc] = { v8::External::New (field) };

	return classes .at (size_t (type)) -> GetFunction () -> NewInstance (argc, argv);
}

void
Context::addObject (X3D::X3DFieldDefinition* const field, const v8::Persistent <v8::Object> & object)
throw (Error <INVALID_FIELD>)
{
	const auto iter = objects .emplace (field, object);

	if (not iter .second)
		throw Error <INVALID_FIELD> ("Object already exists in v8 Context.");

	field -> addParent (this);
}

void
Context::removeObject (X3D::X3DFieldDefinition* const field)
{
	__LOG__ << this << " : " << field -> getTypeName () << std::endl;

	if (objects .erase (field))
		field -> removeParent (this);
}

v8::Local <v8::Object>
Context::getObject (X3D::X3DFieldDefinition* const field) const
throw (std::out_of_range)
{
	return v8::Local <v8::Object>::New (objects .at (field));
}

v8::Local <v8::Function>
Context::getFunction (const std::string & name) const
{
	return v8::Local <v8::Function>::Cast (context -> Global () -> GetRealNamedProperty (String (name)));
}

void
Context::initialize ()
{
	__LOG__ << std::endl;

	X3D::X3DJavaScriptContext::initialize ();

	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	// Run program.

	v8::TryCatch tryCatch;
	program -> Run ();

	if (tryCatch .HasCaught ())
		error (tryCatch);

	setEventHandler ();

	getExecutionContext () -> isLive () .addInterest (this, &Context::set_live);
	isLive () .addInterest (this, &Context::set_live);

	set_live ();

	if (not initializeFn .IsEmpty ())
	{
		v8::TryCatch tryCatch;
		initializeFn -> Call (context -> Global (), 0, nullptr);

		if (tryCatch .HasCaught ())
			error (tryCatch);
	}

	v8::V8::LowMemoryNotification ();
}

void
Context::setEventHandler ()
{
	initializeFn      = v8::Persistent <v8::Function>::New (getFunction ("initialize"));
	prepareEventsFn   = v8::Persistent <v8::Function>::New (getFunction ("prepareEvents"));
	eventsProcessedFn = v8::Persistent <v8::Function>::New (getFunction ("eventsProcessed"));
	shutdownFn        = v8::Persistent <v8::Function>::New (getFunction ("shutdown"));

	if (not shutdownFn .IsEmpty ())
		shutdown () .addInterest (this, &Context::set_shutdown);

	for (const auto & field : getScriptNode () -> getUserDefinedFields ())
	{
		switch (field -> getAccessType ())
		{
			case inputOnly   :
			case inputOutput :
				{
					const auto function = field -> getAccessType () == inputOnly
					                      ? getFunction (field -> getName ())
												 : getFunction ("set_" + field -> getName ());

					if (not function .IsEmpty ())
						functions [field] = v8::Persistent <v8::Function>::New (function);

					break;
				}
			default :
				break;
		}
	}
}

void
Context::set_live ()
{
	if (getExecutionContext () -> isLive () and isLive ())
	{
		if (not prepareEventsFn .IsEmpty ())
			getBrowser () -> prepareEvents () .addInterest (this, &Context::prepareEvents);

		if (not eventsProcessedFn .IsEmpty ())
			getScriptNode () -> addInterest (this, &Context::eventsProcessed);

		getScriptNode () -> addInterest (this, &Context::finish);

		for (const auto & field: getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly:
				case inputOutput:
				{
					if (functions .count (field))
						field -> addInterest (this, &Context::set_field, field);

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		if (not prepareEventsFn .IsEmpty ())
			getBrowser () -> prepareEvents () .removeInterest (this, &Context::prepareEvents);

		if (not eventsProcessedFn .IsEmpty ())
			getScriptNode () -> removeInterest (this, &Context::eventsProcessed);

		getScriptNode () -> removeInterest (this, &Context::finish);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly   :
				case inputOutput :
					{
						if (functions .count (field))
							field -> removeInterest (this, &Context::set_field);

						break;
					}
				default:
					break;
			}
		}
	}
}

void
Context::prepareEvents ()
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	v8::TryCatch tryCatch;

	prepareEventsFn -> Call (context -> Global (), 0, nullptr);

	if (tryCatch .HasCaught ())
		error (tryCatch);
}

void
Context::set_field (X3D::X3DFieldDefinition* const field)
{
	const auto t0 = chrono::now ();

	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	field -> isTainted (true);

	constexpr size_t argc = 2;

	v8::Handle <v8::Value> argv [argc] = {
		v8::Undefined (), //getValue (field),
		v8::Number::New (getCurrentTime ())
	};

	v8::TryCatch tryCatch;
	functions [field] -> Call (context -> Global (), argc, argv);

	if (tryCatch .HasCaught ())
		error (tryCatch);

	field -> isTainted (false);

	__LOG__ << chrono::now () - t0 << std::endl;
}

void
Context::eventsProcessed ()
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	v8::TryCatch tryCatch;

	eventsProcessedFn -> Call (context -> Global (), 0, nullptr);

	if (tryCatch .HasCaught ())
		error (tryCatch);
}

void
Context::finish ()
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	v8::V8::LowMemoryNotification ();

	__LOG__ << getScriptNode () -> getName () << std::endl;
}

void
Context::set_shutdown ()
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	v8::TryCatch tryCatch;

	shutdownFn -> Call (context -> Global (), 0, nullptr);

	if (tryCatch .HasCaught ())
		error (tryCatch);
}

void
Context::error (const v8::TryCatch & tryCatch) const
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolateScope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	X3D::X3DJavaScriptContext::error (to_string (tryCatch .Exception ()),
	                                  to_string (tryCatch .Message () -> GetScriptResourceName ()),
	                                  tryCatch .Message () -> GetLineNumber (),
	                                  tryCatch .Message () -> GetStartColumn (),
	                                  to_string (tryCatch .Message () -> GetSourceLine ()));
}

void
Context::dispose ()
{
	__LOG__ << this << std::endl;

	{
		v8::Locker         locker (isolate);
		v8::Isolate::Scope isolateScope (isolate);
		v8::HandleScope    handleScope;
		v8::Context::Scope contextScope (context);

		const auto global = context -> Global ();
		const auto names  = global -> GetPropertyNames ();

		for (size_t i = 0, size = names -> Length (); i < size; ++ i)
			global -> ForceDelete (names -> Get (i));

		v8::V8::LowMemoryNotification ();
	}

	isolate -> Dispose ();

	__LOG__ << this << " : " << objects .size () << std::endl;

	assert (objects .empty ());

	X3D::X3DJavaScriptContext::dispose ();
}

Context::~Context ()
{
	__LOG__ << this << std::endl;
}

} // GoogleV8
} // X3D
} // titania

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

#include "v8Context.h"

#include "../../Browser/X3DBrowser.h"

#include "v8Browser.h"
#include "v8Fields.h"
#include "v8Globals.h"
#include "v8String.h"

#include <cassert>

namespace titania {
namespace X3D {
namespace GoogleV8 {

const std::string Context::componentName  = "Browser";
const std::string Context::typeName       = "Context";
const std::string Context::containerField = "context";

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
	v8::Isolate::Scope isolate_scope (isolate);
	v8::HandleScope    handleScope;

	//v8::V8::AdjustAmountOfExternalAllocatedMemory (std::numeric_limits <int16_t>::max ());

	context = v8::Context::New (nullptr, v8::ObjectTemplate::New ());

	v8::Context::Scope contextScope (context);

	setContext ();
	setFields ();

	// Compile.

	v8::TryCatch trycatch;
	program = v8::Persistent <v8::Script>::New (v8::Script::Compile (make_v8_string (getECMAScript ()),
	                                                                 make_v8_string (worldURL .back () == getExecutionContext () -> getWorldURL ()
	                                                                                 ? "<inline>"
																												: worldURL .back ())));

	if (program .IsEmpty ())
	{
		error (trycatch);
		throw std::invalid_argument ("Couldn't not compile JavaScript.");
	}
}

void
Context::setContext ()
{
	// v8::Context::Scope

	const auto globalObject = context -> Global ();
	const auto self         = v8::External::New (this);

	Globals::initialize (this, globalObject);
	Browser::initialize (this, globalObject);

	addClass (SFVec4d::getTypeName (), SFVec4d::initialize (self));
	addClass (SFVec4f::getTypeName (), SFVec4f::initialize (self));
}

void
Context::setFields ()
{ }

X3DBaseNode*
Context::create (X3DExecutionContext* const) const
{
	return new Context (getScriptNode (), getECMAScript (), worldURL .front ());
}

void
Context::addClass (const std::string & typeName, const v8::Local <v8::FunctionTemplate> & functionTemplate)
{
	const auto function = functionTemplate -> GetFunction ();

	context -> Global () -> Set (function -> GetName () -> ToString (), function, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete | v8::DontEnum));

	classes .emplace (typeName, v8::Persistent <v8::FunctionTemplate>::New (functionTemplate));
}

v8::Local <v8::Object>
Context::createObject (X3D::X3DFieldDefinition* const field) const
throw (std::out_of_range)
{
	constexpr size_t argc = 1;

	v8::Handle <v8::Value> argv [argc] = { v8::External::New (field) };

	return classes .at (field -> getTypeName ()) -> GetFunction () -> NewInstance (argc, argv);
}

void
Context::addObject (X3D::X3DFieldDefinition* const field, const v8::Local <v8::Object> & object, void* parameters, v8::WeakReferenceCallback callback)
throw (Error <INVALID_FIELD>)
{
	const auto iter = objects .emplace (field, v8::Persistent <v8::Object>::New (object));

	if (not iter .second)
		throw Error <INVALID_FIELD> ("Object already exists in v8 Context.");

	iter .first -> second .MakeWeak (parameters, callback);

	field -> addParent (this);
}

void
Context::removeObject (X3D::X3DFieldDefinition* const field)
{
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
	return v8::Local <v8::Function>::Cast (context -> Global () -> GetRealNamedProperty (make_v8_string (name)));
}

void
Context::initialize ()
{
	__LOG__ << std::endl;

	X3D::X3DJavaScriptContext::initialize ();

	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolate_scope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	// Run program.

	v8::TryCatch           trycatch;
	v8::Handle <v8::Value> result = program -> Run ();

	if (result .IsEmpty ())
		error (trycatch);

	setEventHandler ();

	getExecutionContext () -> isLive () .addInterest (this, &Context::set_live);
	isLive () .addInterest (this, &Context::set_live);

	set_live ();

	if (not initializeFn .IsEmpty ())
		initializeFn -> Call (context -> Global (), 0, nullptr);
}

void
Context::setEventHandler ()
{
	initializeFn      = v8::Persistent <v8::Function>::New (getFunction ("initialize"));
	prepareEventsFn   = v8::Persistent <v8::Function>::New (getFunction ("prepareEvents"));
	eventsProcessedFn = v8::Persistent <v8::Function>::New (getFunction ("eventsProcessed"));
	shutdownFn        = v8::Persistent <v8::Function>::New (getFunction ("shutdown"));

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
	v8::Isolate::Scope isolate_scope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	prepareEventsFn -> Call (context -> Global (), 0, nullptr);
}

void
Context::set_field (X3D::X3DFieldDefinition* const field)
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolate_scope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	field -> isTainted (true);

	static constexpr int argc = 2;

	v8::Handle <v8::Value> argv [argc] = {
		v8::Undefined (), //getValue (field),
		v8::Number::New (getCurrentTime ())
	};

	functions [field] -> Call (context -> Global (), argc, argv);

	field -> isTainted (false);
}

void
Context::eventsProcessed ()
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolate_scope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	eventsProcessedFn -> Call (context -> Global (), 0, nullptr);
}

void
Context::finish ()
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolate_scope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	//while (not v8::V8::IdleNotification ())
	//	;

	__LOG__ << getScriptNode () -> getName () << std::endl;
}

void
Context::shutdown ()
{
	//shutdownFn -> Call (context -> Global (), 0, nullptr);
}

void
Context::error (const v8::TryCatch & trycatch) const
{
	v8::Locker         locker (isolate);
	v8::Isolate::Scope isolate_scope (isolate);
	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	X3D::X3DJavaScriptContext::error (get_utf8_string (trycatch .Exception ()),
	                                  get_utf8_string (trycatch .Message () -> GetScriptResourceName ()),
	                                  trycatch .Message () -> GetLineNumber (),
	                                  trycatch .Message () -> GetStartColumn (),
	                                  get_utf8_string (trycatch .Message () -> GetSourceLine ()));
}

void
Context::dispose ()
{
	__LOG__ << this << " : " << getScriptNode () -> getName () << std::endl;

	{
		v8::Locker         locker (isolate);
		v8::Isolate::Scope isolate_scope (isolate);
		v8::HandleScope    handleScope;
		v8::Context::Scope contextScope (context);

		shutdown ();

		initializeFn      .Dispose ();
		prepareEventsFn   .Dispose ();
		eventsProcessedFn .Dispose ();
		shutdownFn        .Dispose ();

		functions .clear ();
		classes   .clear ();
	
		program .Dispose ();
		context .Dispose ();

		while (not v8::V8::IdleNotification ())
			;
	}

	isolate -> Dispose ();

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

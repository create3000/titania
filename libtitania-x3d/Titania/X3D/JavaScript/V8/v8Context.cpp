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
#include "v8Globals.h"

namespace titania {
namespace X3D {

const std::string v8Context::componentName  = "Browser";
const std::string v8Context::typeName       = "v8Context";
const std::string v8Context::containerField = "context";

v8Context::v8Context (Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3DJavaScriptContext (script, ecmascript),
	            worldURL ({ uri }),
	             context (),
	        globalObject (),
	             program ()
{
	__LOG__ << std::endl;

	v8::HandleScope handleScope;

	globalObject = v8::Persistent <v8::ObjectTemplate>::New (v8::ObjectTemplate::New ());
	context      = v8::Context::New (nullptr, globalObject);

	v8::Context::Scope contextScope (context);

	setContext ();
	setFields ();

	// Compile.

	v8::TryCatch trycatch;
	program = v8::Persistent <v8::Script>::New (v8::Script::Compile (v8::String::New (getECMAScript () .c_str (), getECMAScript () .size ())));

	if (program .IsEmpty ())
	{
		error (trycatch);
		throw std::invalid_argument ("Couldn't not compile JavaScript.");
	}
}

void
v8Context::setContext ()
{
	v8Browser::initialize (this, context -> Global ());
	v8Globals::initialize (this, context -> Global ());
}

void
v8Context::setFields ()
{ }

X3DBaseNode*
v8Context::create (X3DExecutionContext* const) const
{
	return new v8Context (getScriptNode (), getECMAScript (), worldURL .front ());
}

void
v8Context::initialize ()
{
	__LOG__ << std::endl;

	X3DJavaScriptContext::initialize ();

	v8::HandleScope    handleScope;
	v8::Context::Scope contextScope (context);

	v8::TryCatch trycatch;
	v8::Handle <v8::Value> result = program -> Run ();

	if (result .IsEmpty ())
		error (trycatch);
}

void
v8Context::prepareEvents ()
{ }

void
v8Context::set_live ()
{ }

void
v8Context::set_field (X3DFieldDefinition* const field)
{ }

void
v8Context::eventsProcessed ()
{ }

void
v8Context::finish ()
{ }

void
v8Context::shutdown ()
{ }

void
v8Context::error (const v8::TryCatch & trycatch) const
{
	v8::HandleScope handleScope;

	X3DJavaScriptContext::error (*v8::String::Utf8Value (trycatch .Exception ()),
	                             worldURL .back () == getExecutionContext () -> getWorldURL () ? "<inline>" : worldURL .back (),
	                             trycatch .Message () -> GetLineNumber (),
	                             trycatch .Message () -> GetStartColumn (),
	                             *v8::String::Utf8Value (trycatch .Message () -> GetSourceLine ()));
}

void
v8Context::dispose ()
{
	__LOG__ << std::endl;

	shutdown ();

	program      .Dispose ();
	context      .Dispose ();
	globalObject .Dispose ();

	X3DJavaScriptContext::dispose ();
}

v8Context::~v8Context ()
{ }

} // X3D
} // titania

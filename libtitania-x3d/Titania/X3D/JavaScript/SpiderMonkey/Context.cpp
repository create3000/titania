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

#include "Context.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "Arguments.h"
#include "Globals.h"

#include <cassert>

namespace titania {
namespace X3D {
namespace spidermonkey {

const ComponentType Context::component      = ComponentType::TITANIA;
const std::string   Context::typeName       = "SpiderMonkeyContext";
const std::string   Context::containerField = "context";

const JSClassOps Context::globalOps = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	JS_GlobalObjectTraceHook
};

const JSClass Context::globalClass = {
	"global",
	JSCLASS_GLOBAL_FLAGS,
	&globalOps
};

///  throws std::exception
Context::Context (JSContext* const cx, X3D::Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3D::X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3D::X3DJavaScriptContext (script, ecmascript),
	                 worldURL (uri),
	                       cx (cx),
	                   global ()
{
	if (not cx)
		throw std::runtime_error ("Couldn't create JavaScript context.");

	const JSAutoRequest ar (cx);

	JS::CompartmentOptions options;

	global = std::make_unique <JS::PersistentRooted <JSObject*>> (cx, JS_NewGlobalObject (cx, &globalClass, nullptr, JS::FireOnNewGlobalHook, options));

	if (not *global)
		throw std::runtime_error ("Couldn't create JavaScript global object.");

	const JSAutoCompartment ac (cx, *global);

	if (not JS_InitStandardClasses (cx, *global))
		throw std::runtime_error ("Couldn't create JavaScript standard classes.");

	JS::SetWarningReporter (cx, error);

	addClasses ();
	addUserDefinedFields ();
}

X3DBaseNode*
Context::create (X3D::X3DExecutionContext* const) const
{
	return new Context (cx, getScriptNode (), getECMAScript (), worldURL);
}

void
Context::setExecutionContext (X3D::X3DExecutionContext* const executionContext)
{
	//if (future)
	//	future -> setExecutionContext (executionContext);

	X3DJavaScriptContext::setExecutionContext (executionContext);
}

void
Context::addClasses ()
{
	Globals::init (cx, *global);
}

void
Context::addUserDefinedFields ()
{

}

bool
Context::evaluate (const std::string & string, const std::string & filename)
{
	JS::CompileOptions options (cx, JSVERSION_LATEST);

	options .setUTF8 (true);

	JS::RootedScript        rootedScript (cx);
	JS::MutableHandleScript script (&rootedScript);

	if (JS_CompileScript (cx, string .c_str (), string .length (), options, script))
		return JS_ExecuteScript (cx, script);

	return false;
}

void
Context::call (const std::string & value)
{
	JS::RootedValue fval (cx);

	if (getFunction (value, &fval))
		call (fval);
}

void
Context::call (JS::HandleValue value)
{
	JS::RootedValue rval (cx);

	JS_CallFunctionValue (cx, *global, value, JS::HandleValueArray::empty (), &rval);
}

bool
Context::getFunction (const std::string & name, JS::MutableHandleValue value) const
{
	if (JS_GetProperty (cx, *global, name .c_str (), value))
	{
		if (value .isObjectOrNull ())
		{
			const auto obj = value .toObjectOrNull ();

			if (obj and JS_ObjectIsFunction (cx, obj))
				return true;
		}
	}

	return false;
}

void
Context::initialize ()
{
	X3DJavaScriptContext::initialize ();

	const JSAutoRequest ar (cx);
	const JSAutoCompartment ac (cx, *global);

	JS_SetContextPrivate (cx, this);

	if (not evaluate (getECMAScript (), worldURL))
		throw std::invalid_argument ("Couldn't evaluate script.");

	getExecutionContext () -> isLive () .addInterest (this, &Context::set_live);
	isLive () .addInterest (this, &Context::set_live);

	set_live ();

	call ("initialize");

	shutdown () .addInterest (&Context::set_shutdown, this);

	finish ();
}

void
Context::set_live ()
{
	const JSAutoRequest ar (cx);
	const JSAutoCompartment ac (cx, *global);

	if (getExecutionContext () -> isLive () and isLive ())
	{
		const auto prepareEvents   = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);
		const auto eventsProcessed = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);

		if (getFunction ("prepareEvents", prepareEvents .get ()))
			getBrowser () -> prepareEvents () .addInterest (&Context::prepareEvents, this, prepareEvents);

		if (getFunction ("eventsProcessed", eventsProcessed .get ()))
			getScriptNode () -> addInterest (&Context::eventsProcessed, this, eventsProcessed);

		getScriptNode () -> addInterest (&Context::finish, this);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
		{
			switch (field -> getAccessType ())
			{
				case inputOnly:
				{
					const auto inputFunction = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);

					if (getFunction (field -> getName (), inputFunction .get ()))
						field -> addInterest (&Context::set_field, this, field, inputFunction);

					break;
				}
				case inputOutput:
				{
					const auto inputFunction = std::make_shared <JS::PersistentRooted <JS::Value>> (cx);

					if (getFunction ("set_" + field -> getName (), inputFunction .get ()))
						field -> addInterest (&Context::set_field, this, field, inputFunction);

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		getBrowser () -> prepareEvents () .removeInterest (&Context::prepareEvents, this);
		getScriptNode () -> removeInterest (&Context::eventsProcessed, this);
		getScriptNode () -> removeInterest (&Context::finish, this);

		for (const auto & field : getScriptNode () -> getUserDefinedFields ())
			field -> removeInterest (&Context::set_field, this);
	}
}

void
Context::prepareEvents (const std::shared_ptr <JS::PersistentRooted <JS::Value>> & functionValue)
{
	const JSAutoRequest ar (cx);
	const JSAutoCompartment ac (cx, *global);

	JS_SetContextPrivate (cx, this);

	call (*functionValue);
}

void
Context::set_field (X3D::X3DFieldDefinition* const field, const std::shared_ptr <JS::PersistentRooted <JS::Value>> & inputFunction)
{
	const JSAutoRequest ar (cx);
	const JSAutoCompartment ac (cx, *global);

	field -> setTainted (true);

	JS_SetContextPrivate (cx, this);

	JS::RootedValue     rval (cx);
	JS::AutoValueVector args (cx);

	args .append (JS::NullValue ());
	args .append (JS::DoubleValue (getCurrentTime ()));

	JS_CallFunctionValue (cx, *global, *inputFunction, args, &rval);

	field -> setTainted (false);
}

void
Context::eventsProcessed (const std::shared_ptr <JS::PersistentRooted <JS::Value>> & functionValue)
{
	const JSAutoRequest ar (cx);
	const JSAutoCompartment ac (cx, *global);

	JS_SetContextPrivate (cx, this);

	call (*functionValue);
}

void
Context::finish ()
{
	const JSAutoRequest ar (cx);
	const JSAutoCompartment ac (cx, *global);

	JS_SetContextPrivate (cx, this);

	JS_GC (cx);
}

void
Context::set_shutdown ()
{
	const JSAutoRequest ar (cx);
	const JSAutoCompartment ac (cx, *global);

	JS_SetContextPrivate (cx, this);

	call ("shutdown");
}

void
Context::error (JSContext* cx, JSErrorReport* const report)
{
__LOG__ << std::endl;

	const auto context = getContext (cx);

	glong   items_read    = 0;
	glong   items_written = 0;
	GError* error         = nullptr;

	const auto lb = g_utf16_to_utf8 ((gunichar2*) report -> linebuf (),
	                                 report -> linebufLength (),
	                                 &items_read,
	                                 &items_written,
	                                 &error);

	if (error)
	{
		__LOG__ << error -> message << std::endl;
	}
	else
	{
		const std::string linebuf (lb, lb + items_written);

		g_free (lb);

		context -> setError (report -> message () .c_str (),
		                     report -> filename ? report -> filename : "<inline>",
		                     report -> lineno,
		                     report -> column,
		                     linebuf);
	}
}

void
Context::dispose ()
{
	const JSAutoRequest ar (cx);

	global .reset ();

	X3D::X3DJavaScriptContext::dispose ();
}

Context::~Context ()
{ }

} // spidermonkey
} // X3D
} // titania

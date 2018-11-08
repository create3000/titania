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

#include "Browser.h"

#include "Arguments.h"
#include "Fields/ArrayFields.h"
#include "Fields/SFNode.h"
#include "value.h"

#include "../../Browser/Core/BrowserOptions.h"
#include "../../Browser/Core/BrowserProperties.h"
#include "../../Browser/Core/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../InputOutput/FileLoader.h"
#include "../../Thread/SceneFuture.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

const std::string Browser::typeName = "X3DBrowser";

pb::ptr <pb::NativeFunction>
Browser::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), nullptr, nullptr, 4);
	const auto prototype = new pb::Object (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	// Properties

	prototype -> addOwnProperty ("name",             pb::undefined, pb::NONE, new pb::NativeFunction (ec, "getName",             getName,             0), nullptr);
	prototype -> addOwnProperty ("version",          pb::undefined, pb::NONE, new pb::NativeFunction (ec, "getVersion",          getVersion,          0), nullptr);
	prototype -> addOwnProperty ("currentSpeed",     pb::undefined, pb::NONE, new pb::NativeFunction (ec, "getCurrentSpeed",     getCurrentSpeed,     0), nullptr);
	prototype -> addOwnProperty ("currentFrameRate", pb::undefined, pb::NONE, new pb::NativeFunction (ec, "getCurrentFrameRate", getCurrentFrameRate, 0), nullptr);

	prototype -> addOwnProperty ("description",
	                             pb::undefined,
	                             pb::NONE,
	                             new pb::NativeFunction (ec, "getDescription", getDescription, 0),
	                             new pb::NativeFunction (ec, "setDescription", setDescription, 1));

	// X3D functions

	//	prototype -> addOwnProperty ("replaceWorld",         new pb::NativeFunction (ec, "replaceWorld",         replaceWorld,         1), pb::NONE);
	//	prototype -> addOwnProperty ("createX3DFromString",  new pb::NativeFunction (ec, "createX3DFromString",  createX3DFromString,  1), pb::NONE);
	//	prototype -> addOwnProperty ("createX3DFromURL",     new pb::NativeFunction (ec, "createX3DFromURL",     createX3DFromURL,     1), pb::NONE);
	//	prototype -> addOwnProperty ("loadURL",              new pb::NativeFunction (ec, "loadURL",              loadURL,              2), pb::NONE);
	prototype -> addOwnProperty ("getRenderingProperty", new pb::NativeFunction (ec, "getRenderingProperty", getRenderingProperty, 1), pb::NONE);
	prototype -> addOwnProperty ("getBrowserProperty",   new pb::NativeFunction (ec, "getBrowserProperty",   getBrowserProperty,   1), pb::NONE);
	prototype -> addOwnProperty ("getBrowserOption",     new pb::NativeFunction (ec, "getBrowserOption",     getBrowserOption,     1), pb::NONE);
	prototype -> addOwnProperty ("setBrowserOption",     new pb::NativeFunction (ec, "setBrowserOption",     setBrowserOption,     2), pb::NONE);

	prototype -> addOwnProperty ("firstViewpoint",       new pb::NativeFunction (ec, "firstViewpoint",       firstViewpoint,       0), pb::NONE);
	prototype -> addOwnProperty ("previousViewpoint",    new pb::NativeFunction (ec, "previousViewpoint",    previousViewpoint,    0), pb::NONE);
	prototype -> addOwnProperty ("nextViewpoint",        new pb::NativeFunction (ec, "nextViewpoint",        nextViewpoint,        0), pb::NONE);
	prototype -> addOwnProperty ("lastViewpoint",        new pb::NativeFunction (ec, "lastViewpoint",        lastViewpoint,        0), pb::NONE);

	prototype -> addOwnProperty ("print",                new pb::NativeFunction (ec, "print",                print,                1), pb::NONE);
	prototype -> addOwnProperty ("println",              new pb::NativeFunction (ec, "println",              println,              1), pb::NONE);

	// VRML Functions

	prototype -> addOwnProperty ("getName",              new pb::NativeFunction (ec, "getName",              getName,              0), pb::NONE);
	prototype -> addOwnProperty ("getVersion",           new pb::NativeFunction (ec, "getVersion",           getVersion,           0), pb::NONE);
	prototype -> addOwnProperty ("getCurrentSpeed",      new pb::NativeFunction (ec, "getCurrentSpeed",      getCurrentSpeed,      0), pb::NONE);
	prototype -> addOwnProperty ("getCurrentFrameRate",  new pb::NativeFunction (ec, "getCurrentFrameRate",  getCurrentFrameRate,  0), pb::NONE);
	prototype -> addOwnProperty ("getWorldURL",          new pb::NativeFunction (ec, "getWorldURL",          getWorldURL,          0), pb::NONE);
	prototype -> addOwnProperty ("setDescription",       new pb::NativeFunction (ec, "setDescription",       setDescription,       1), pb::NONE);
	prototype -> addOwnProperty ("createVrmlFromString", new pb::NativeFunction (ec, "createVrmlFromString", createVrmlFromString, 1), pb::NONE);
	prototype -> addOwnProperty ("createVrmlFromURL",    new pb::NativeFunction (ec, "createVrmlFromURL",    createVrmlFromURL,    3), pb::NONE);
	prototype -> addOwnProperty ("addRoute",             new pb::NativeFunction (ec, "addRoute",             addRoute,             4), pb::NONE);
	prototype -> addOwnProperty ("deleteRoute",          new pb::NativeFunction (ec, "deleteRoute",          deleteRoute,          4), pb::NONE);

	// Prototype

	function -> addOwnProperty ("prototype", prototype, pb::NONE);

	// Instance

	ec -> getGlobalObject () -> addOwnProperty ("Browser", function -> createInstance (ec), pb::NONE);

	return function;
}

pb::var
Browser::getName (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	return browser -> getName ();
}

pb::var
Browser::getVersion (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	return browser -> getVersion ();
}

pb::var
Browser::getCurrentSpeed (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	return browser -> getCurrentSpeed ();
}

pb::var
Browser::getCurrentFrameRate (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	return browser -> getCurrentFrameRate ();
}

pb::var
Browser::getWorldURL (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	return browser -> getWorldURL () .str ();
}

pb::var
Browser::setDescription (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	browser -> setDescription (get1Argument <X3D::String> (args, 0));

	return pb::undefined;
}

pb::var
Browser::getDescription (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	return browser -> getDescription () .getValue ();
}

/*
 *  X3D functions
 */

pb::var
Browser::getRenderingProperty (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.getRenderingProperty: wrong number of arguments.");

	const auto context = getContext (ec);
	const auto browser = context -> getBrowser ();
	const auto name    = get1Argument <std::string> (args, 0);

	try
	{
		const auto field = browser -> getRenderingProperties () -> getField (name);

		return getValue (context, field);
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		throw pb::Error (getTypeName () + ".prototype.getRenderingProperty: unknown property '" + name + "'.");
	}
}

pb::var
Browser::getBrowserProperty (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.getBrowserProperty: wrong number of arguments.");

	const auto context = getContext (ec);
	const auto browser = context -> getBrowser ();
	const auto name    = get1Argument <std::string> (args, 0);

	try
	{
		const auto field = browser -> getBrowserProperties () -> getField (name);

		return getValue (context, field);
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		throw pb::Error (getTypeName () + ".prototype.getBrowserProperty: unknown property '" + name + "'.");
	}
}

pb::var
Browser::getBrowserOption (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.getBrowserOption: wrong number of arguments.");

	const auto context = getContext (ec);
	const auto browser = context -> getBrowser ();
	const auto name    = get1Argument <std::string> (args, 0);

	try
	{
		const auto field = browser -> getBrowserOptions () -> getField (name);

		return getValue (context, field);
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		throw pb::Error (getTypeName () + ".prototype.getBrowserOption: unknown property '" + name + "'.");
	}
}

pb::var
Browser::setBrowserOption (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 2)
		throw pb::Error (getTypeName () + ".prototype.setBrowserOption: wrong number of arguments.");

	const auto browser = getContext (ec) -> getBrowser ();
	const auto name    = get1Argument <std::string> (args, 0);

	try
	{
		const auto field = browser -> getBrowserOptions () -> getField (name);

		setValue (field, args [1]);

		return pb::undefined;
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		throw pb::Error (getTypeName () + ".prototype.setBrowserOption: unknown property '" + name + "'.");
	}
}

pb::var
Browser::firstViewpoint (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	browser -> firstViewpoint ();

	return pb::undefined;
}

pb::var
Browser::previousViewpoint (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	browser -> previousViewpoint ();

	return pb::undefined;
}

pb::var
Browser::nextViewpoint (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	browser -> nextViewpoint ();

	return pb::undefined;
}

pb::var
Browser::lastViewpoint (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	browser -> lastViewpoint ();

	return pb::undefined;
}

pb::var
Browser::print (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	for (const auto & arg : args)
		browser -> print (arg .toString ());

	return pb::undefined;
}

pb::var
Browser::println (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	print (ec, object, args);

	browser -> print ("\n");

	return pb::undefined;
}

/*
 *  VRML functions
 */

pb::var
Browser::createVrmlFromURL (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 3)
		throw pb::Error (getTypeName () + ".prototype.createVrmlFromURL: wrong number of arguments.");

	const auto   context = getContext (ec);
	const auto & script  = context -> getScriptNode ();
	const auto   url     = get1Argument <MFString> (args, 0);
	const auto   node    = get1Argument <SFNode> (args, 1);
	const auto   event   = get1Argument <std::string> (args, 2);

	try
	{
		if (not node -> getValue ())
			throw pb::TypeError ("Browser.prototype.createVrmlFromURL: node is NULL.");

		const auto field = node -> getValue () -> getField (event);

		if (not field -> isInput ())
			throw pb::TypeError ("Browser.prototype.createVrmlFromURL: field '" + event + "' is not an eventIn.");

		if (field -> getType () not_eq X3D::X3DConstants::MFNode)
			throw pb::TypeError ("Browser.prototype.createVrmlFromURL: field '" + event + "' is not a MFNode.");

		if (context -> getFuture ())
			context -> getFuture () -> wait ();

		using namespace std::placeholders;

		context -> getFuture () .setValue (new X3D::SceneFuture (script -> getExecutionContext (),
		                                                         *url,
		                                                         false,
		                                                         std::bind (&Browser::setSceneAsync,
		                                                                    X3D::SFNode (script),
		                                                                    *node,
		                                                                    static_cast <X3D::MFNode*> (field),
		                                                                    _1)));

		return pb::undefined;
	}
	catch (const Error <INVALID_NAME> &)
	{
	throw pb::TypeError ("Browser.prototype.createVrmlFromURL: no such field '" + event + "' in node " + node -> getValue () -> getTypeName () + ".");
	}
}

//  XXX: use FieldPtr here for "X3D::MFNode* const field"
void
Browser::setSceneAsync (const X3D::SFNode & script, const X3D::SFNode & node, X3D::MFNode* const field, X3D::X3DScenePtr && scene)
{
	if (scene)
	{
		const auto executionContext = script -> getExecutionContext ();

		executionContext -> isLive () .addInterest (scene -> isLive ());

		scene -> isLive () = executionContext -> isLive ();
		scene -> setup ();

		*field = scene -> getRootNodes ();
	}
}

pb::var
Browser::createVrmlFromString (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.createVrmlFromString: wrong number of arguments.");

	try
	{
		const auto script     = getContext (ec) -> getScriptNode ();
		const auto vrmlSyntax = get1Argument <std::string> (args, 0);
		const auto scene      = X3D::FileLoader (script -> getExecutionContext (), script -> getWorldURL ()) .createX3DFromString (vrmlSyntax);

		return X3DField::create <MFNode> (ec, new X3D::MFNode (scene -> getRootNodes ()));
	}
	catch (const X3DError & error)
	{
		getContext (ec) -> getBrowser () -> getConsole () -> error (error .what (), "\n");

		return X3DField::create <MFNode> (ec, new X3D::MFNode ());
	}
}

pb::var
Browser::addRoute (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 4)
		throw pb::Error (getTypeName () + ".prototype.addRoute: wrong number of arguments.");

	try
	{
		const auto executionContext = getContext (ec) -> getExecutionContext ();
		const auto fromNode         = get1Argument <SFNode> (args, 0);
		const auto fromEventOut     = get1Argument <std::string> (args, 1);
		const auto toNode           = get1Argument <SFNode> (args, 2);
		const auto toEventIn        = get1Argument <std::string> (args, 3);

		executionContext -> addRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		return pb::undefined;
	}
	catch (const X3D::X3DError & error)
	{
		throw pb::Error (getTypeName () + ".prototype.addRoute: :" + error .what ());
	}
}

pb::var
Browser::deleteRoute (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 4)
		throw pb::Error (getTypeName () + ".prototype.deleteRoute: wrong number of arguments.");

	try
	{
		const auto executionContext = getContext (ec) -> getExecutionContext ();
		const auto fromNode         = get1Argument <SFNode> (args, 0);
		const auto fromEventOut     = get1Argument <std::string> (args, 1);
		const auto toNode           = get1Argument <SFNode> (args, 2);
		const auto toEventIn        = get1Argument <std::string> (args, 3);

		executionContext -> deleteRoute (*fromNode, fromEventOut, *toNode, toEventIn);

		return pb::undefined;
	}
	catch (const X3D::X3DError & error)
	{
		throw pb::Error (getTypeName () + ".prototype.deleteRoute: :" + error .what ());
	}
}

} // peaseblossom
} // X3D
} // titania

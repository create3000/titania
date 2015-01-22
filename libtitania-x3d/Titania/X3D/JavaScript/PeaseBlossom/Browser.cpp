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

#include "Browser.h"

#include "../../Browser/X3DBrowser.h"
#include "Fields/SFNode.h"
#include "Arguments.h"

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

	// Functions

	prototype -> addOwnProperty ("getName",  new pb::NativeFunction (ec, "getName",  getName,  0), pb::NONE);
	prototype -> addOwnProperty ("addRoute", new pb::NativeFunction (ec, "addRoute", addRoute, 4), pb::NONE);

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	
	// Instance

	ec -> getGlobalObject () -> addOwnProperty ("Browser", function -> createInstance (ec), pb::NONE);

	return function;
}

pb::var
Browser::getName (const pb::ptr <pb::pbExecutionContext> & ec, pb::pbObject* const object, const std::vector <pb::var> & args)
{
	const auto browser = getContext (ec) -> getBrowser ();

	return browser -> getName ();
}

pb::var
Browser::addRoute (const pb::ptr <pb::pbExecutionContext> & ec, pb::pbObject* const object, const std::vector <pb::var> & args)
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
		throw pb::Error (getTypeName () + ".prototype.addRoute: %s:" + error .what ());
	}
}

} // peaseblossom
} // X3D
} // titania

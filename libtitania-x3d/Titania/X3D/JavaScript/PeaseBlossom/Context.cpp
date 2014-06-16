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
#include "../../PeaseBlossom/pb.h"

#include "../../PeaseBlossom/Debug.h"
#include <cassert>

namespace titania {
namespace X3D {
namespace peaseblossom {

const std::string Context::componentName  = "Browser";
const std::string Context::typeName       = "Context";
const std::string Context::containerField = "context";

Context::Context (Script* const script, const std::string & ecmascript, const basic::uri & uri) :
	         X3D::X3DBaseNode (script -> getBrowser (), script -> getExecutionContext ()),
	X3D::X3DJavaScriptContext (script, ecmascript),
	                 worldURL ({ uri })
{
	__LOG__ << std::endl;
}

void
Context::setContext ()
{ }

void
Context::setFields ()
{ }

X3DBaseNode*
Context::create (X3DExecutionContext* const) const
{
	return new Context (getScriptNode (), getECMAScript (), worldURL .front ());
}

void
Context::initialize ()
{
	const auto t0 = chrono::now ();

	std::istringstream istream (getECMAScript ());

	try
	{
		const auto program = pb::createProgram ();

		program -> fromStream (istream);

		getBrowser () -> println ("result:  ", program -> run (), " : ", SFTime (chrono::now () - t0));
		getBrowser () -> println ("istream: ", SFBool (istream));

		{
			const auto t0 = chrono::now ();

			getBrowser () -> println ("result:  ", program -> run (), " : ", SFTime (chrono::now () - t0));
		}
	}
	catch (const pb::jsException & error)
	{
		getBrowser () -> println (error);
	}

	if (istream)
		getBrowser () -> println (">>", istream .rdbuf (), "<<");

	pb::debug_roots (pb::getUndefined () .get ());
	pb::debug_roots (pb::getFalse () .get ());
	pb::debug_roots (pb::getTrue () .get ());
	pb::debug_roots (pb::getNull () .get ());

	assert (pb::getUndefined () -> getParents () .size () == 1);
	assert (pb::getFalse ()     -> getParents () .size () == 1);
	assert (pb::getTrue ()      -> getParents () .size () == 1);
	assert (pb::getNull ()      -> getParents () .size () == 1);

	pb::Program::deleteObjectsAsync ();
}

void
Context::setEventHandler ()
{ }

void
Context::set_live ()
{ }

void
Context::prepareEvents ()
{ }

void
Context::set_field (X3D::X3DFieldDefinition* const field)
{ }

void
Context::eventsProcessed ()
{ }

void
Context::finish ()
{ }

void
Context::shutdown ()
{ }

void
Context::error (const std::string & trycatch) const
{
	X3D::X3DJavaScriptContext::error (trycatch,
	                                  "filename",
	                                  0 /*"lineNumber"*/,
	                                  0 /*"startColumn"*/,
	                                  "sourceLine");
}

void
Context::dispose ()
{
	X3D::X3DJavaScriptContext::dispose ();
}

Context::~Context ()
{ }

} // peaseblossom
} // X3D
} // titania

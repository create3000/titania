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

#include "V8/v8Context.h"

#include "../JavaScript/V8.h"

#include "../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string V8::componentName  = "Browser";
const std::string V8::typeName       = "V8";
const std::string V8::containerField = "javaScript";

V8::V8 (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DJavaScriptEngine (),
	             vendor ("Google"),
	        description (),
	            version ()
{
	setName ("V8");
}

V8*
V8::create (X3DExecutionContext* const executionContext)  const
{
	return new V8 (executionContext);
}

void
V8::initialize ()
{
	X3DJavaScriptEngine::initialize ();

	description = "";
	version     = v8::V8::GetVersion ();
}

X3DPtr <X3DJavaScriptContext>
V8::createContext (Script* script, const std::string & ecmascript, const basic::uri & uri)
{
	return new GoogleV8::Context (script, ecmascript, uri);
}

void
V8::toStream (std::ostream & stream) const
{
	stream
		<< "\tCurrent Javascript Engine" << std::endl
		<< "\t\tName: " << vendor << ' ' << getName () << std::endl
		<< "\t\tDescription: " << description << std::endl
		<< "\t\tVersion: " << version;
}

void
V8::dispose ()
{
	X3DJavaScriptEngine::dispose ();
}

} // X3D
} // titania

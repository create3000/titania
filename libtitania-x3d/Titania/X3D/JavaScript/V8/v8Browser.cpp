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

namespace titania {
namespace X3D {

void
v8Browser::initialize (v8Context* const javaScript, const v8::Local <v8::Object> & globalObject)
{
	const auto browserClass = v8::FunctionTemplate::New ();

	browserClass -> SetClassName (v8::String::New ("Browser"));

	// X3D properties

	const auto prototype = browserClass -> InstanceTemplate ();

	prototype -> SetAccessor (v8::String::New ("name"),                name, nullptr, v8::External::New (javaScript), v8::DEFAULT, v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
//	prototype -> SetAccessor (v8::String::New ("version"),             );
//	prototype -> SetAccessor (v8::String::New ("currentSpeed"),        );
//	prototype -> SetAccessor (v8::String::New ("currentFrameRate"),    );
//	prototype -> SetAccessor (v8::String::New ("description"),         );
//	prototype -> SetAccessor (v8::String::New ("supportedComponents"), );
//	prototype -> SetAccessor (v8::String::New ("supportedProfiles"),   );
//	prototype -> SetAccessor (v8::String::New ("currentScene"),        );

	globalObject -> Set (v8::String::New ("Browser"), browserClass -> GetFunction () -> NewInstance (), v8::PropertyAttribute (v8::ReadOnly | v8::DontDelete));
}

v8::Handle <v8::Value>
v8Browser::name (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	const auto browser = static_cast <v8Context*> (v8::Handle <v8::External>::Cast (info .Data ()) -> Value ()) -> getBrowser ();

	return v8::String::New (browser -> getName () .c_str (), browser -> getName () .size ());
}

} // X3D
} // titania

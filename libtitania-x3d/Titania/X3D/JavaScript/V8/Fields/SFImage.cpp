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

#include "SFImage.h"

namespace titania {
namespace X3D {
namespace GoogleV8 {

template <>
const std::string X3DObject <X3D::SFImage>::typeName = "SFImage";

template <>
const ObjectType X3DObject <X3D::SFImage>::type = ObjectType::SFImage;

v8::Local <v8::FunctionTemplate>
SFImage::initialize (const v8::Local <v8::External> & context)
{
	const auto functionTemplate = createFunctionTemplate (context, construct);
	const auto instanceTemplate = functionTemplate -> InstanceTemplate ();

	instanceTemplate -> SetAccessor (String ("width"),   width,  width, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("x"),       width,  width, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("height"), height, height, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("y"),      height, height, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("comp"),     comp,   comp, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());
	instanceTemplate -> SetAccessor (String ("array"),   array,  array, v8::Handle <v8::Value> (), v8::DEFAULT, getPropertyAttributes ());

	instanceTemplate -> Set (String ("getName"),     v8::FunctionTemplate::New (getName,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getTypeName"), v8::FunctionTemplate::New (getTypeName, context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("getType"),     v8::FunctionTemplate::New (getType,     context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isReadable"),  v8::FunctionTemplate::New (isReadable,  context) -> GetFunction (), getFunctionAttributes ());
	instanceTemplate -> Set (String ("isWritable"),  v8::FunctionTemplate::New (isWritable,  context) -> GetFunction (), getFunctionAttributes ());

	instanceTemplate -> Set (String ("toString"),    v8::FunctionTemplate::New (toString,    context) -> GetFunction (), getFunctionAttributes ());

	return functionTemplate;
}

v8::Handle <v8::Value>
SFImage::construct (const v8::Arguments & args)
{
	if (args .IsConstructCall ())
	{
		const auto context = getContext (args);
		const auto object  = args .This ();

		switch (args .Length ())
		{
			case 0:
			{
				addObject (context, object, new T ());
				break;
			}
			case 4:
			{
				addObject (context, object, new T (args [0] -> ToUint32 () -> Value (),
				                                   args [1] -> ToUint32 () -> Value (),
				                                   args [2] -> ToUint32 () -> Value (),
				                                   X3D::MFInt32 ()));
				break;
			}
			case 1:
			{
				const auto arg = args [0];

				if (arg -> IsExternal ())
				{
					const auto value = getObject (arg);

					if (value -> getParents () .empty ())
						addObject (context, object, value);
					else
						addProperty (context, object, value);

					break;
				}

				// Proceed with next case.
			}
			default:
				return v8::ThrowException (String ("RuntimeError: wrong number of arguments."));
		}

		return v8::Undefined ();
	}

	return v8::ThrowException (String ("RuntimeError: cannot call constructor as function."));
}

void
SFImage::array (v8::Local <v8::String> property, v8::Local <v8::Value> value, const v8::AccessorInfo & info)
{
	//getObject (info) -> setComponents (value -> ToNumber () -> Value ());
}

v8::Handle <v8::Value>
SFImage::array (v8::Local <v8::String> property, const v8::AccessorInfo & info)
{
	//return v8::Number::New (getObject (info) -> getComponents ());
	return v8::Undefined ();
}

} // GoogleV8
} // X3D
} // titania

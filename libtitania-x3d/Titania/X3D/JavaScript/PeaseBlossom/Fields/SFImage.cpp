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

#include "SFImage.h"

#include "ArrayFields.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

const std::string SFImage::typeName = "SFImage";

const pb::Callbacks SFImage::callbacks = {
	pb::EnumerateCallback (),
	pb::HasPropertyCallback (),
	pb::PropertyGetter (),
	pb::PropertySetter (),
	pb::ResolveCallback (),
	dispose <SFImage>

};

pb::ptr <pb::NativeFunction>
SFImage::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 4);
	const auto prototype = context -> getClass (ObjectType::X3DField) -> createInstance (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	// Properties

	pb::NativeFunction* getter = nullptr;
	pb::NativeFunction* setter = nullptr;

	prototype -> addOwnProperty ("width",
	                             pb::undefined,
	                             pb::ENUMERABLE,
	                             getter = new pb::NativeFunction (ec, "width", getWidth, 0),
	                             setter = new pb::NativeFunction (ec, "width", setWidth, 1));

	prototype -> addOwnProperty ("x", pb::undefined, pb::NONE, getter, setter);

	prototype -> addOwnProperty ("height",
	                             pb::undefined,
	                             pb::ENUMERABLE,
	                             getter = new pb::NativeFunction (ec, "height", getHeight, 0),
	                             setter = new pb::NativeFunction (ec, "height", setHeight, 1));

	prototype -> addOwnProperty ("y", pb::undefined, pb::NONE, getter, setter);

	prototype -> addOwnProperty ("comp",
	                             pb::undefined,
	                             pb::ENUMERABLE,
	                             new pb::NativeFunction (ec, "comp", getComp, 0),
	                             new pb::NativeFunction (ec, "comp", setComp, 1));

	prototype -> addOwnProperty ("array",
	                             pb::undefined,
	                             pb::ENUMERABLE,
	                             new pb::NativeFunction (ec, "array", getArray, 0),
	                             new pb::NativeFunction (ec, "array", setArray, 1));

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

pb::var
SFImage::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <SFImage> (ec, object, new X3D::SFImage ());
			break;
		}
		case 4:
		{
			const auto width  = get1Argument <uint32_t> (args, 0);
			const auto height = get1Argument <uint32_t> (args, 1);
			const auto comp   = get1Argument <uint32_t> (args, 2);
			const auto array  = get1Argument <MFInt32> (args, 3);

			setUserData <SFImage> (ec, object, new X3D::SFImage (width, height, comp, *array));
			break;
		}
		default:
			throw pb::Error (getTypeName () + ".constructor: wrong number of arguments.");
	}

	return pb::undefined;
}

pb::var
SFImage::setWidth (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs   = getThis <SFImage> (ec, object);
		const auto value = get1Argument <int32_t> (args, 0);

		if (value < 0)
			throw pb::TypeError (getTypeName () + ".prototype.width: value less than 0.");

		lhs -> setWidth (value);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.width is not generic.");
	}
}

pb::var
SFImage::getWidth (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs = getThis <SFImage> (ec, object);

		return lhs -> getWidth ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.width is not generic.");
	}
}

pb::var
SFImage::setHeight (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs   = getThis <SFImage> (ec, object);
		const auto value = get1Argument <int32_t> (args, 0);

		if (value < 0)
			throw pb::TypeError (getTypeName () + ".prototype.height: value less than 0.");

		lhs -> setHeight (value);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.height is not generic.");
	}
}

pb::var
SFImage::getHeight (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs = getThis <SFImage> (ec, object);

		return lhs -> getHeight ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.height is not generic.");
	}
}

pb::var
SFImage::setComp (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs   = getThis <SFImage> (ec, object);
		const auto value = get1Argument <int32_t> (args, 0);

		if (value < 0 or value > 4)
			throw pb::TypeError (getTypeName () + ".prototype.comp: value less than 0 or greater than 4.");

		lhs -> setComponents (value);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.comp is not generic.");
	}
}

pb::var
SFImage::getComp (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs = getThis <SFImage> (ec, object);

		return lhs -> getComponents ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.comp is not generic.");
	}
}

pb::var
SFImage::setArray (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto lhs = getThis <SFImage> (ec, object);
		const auto rhs = get1Argument <MFInt32> (args, 0);

		lhs -> setArray (*rhs);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.array is not generic.");
	}
}

pb::var
SFImage::getArray (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	try
	{
		const auto context = getContext (ec);
		const auto lhs     = getThis <SFImage> (ec, object);

		return get <MFInt32> (context, lhs -> getArray ());
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.array is not generic.");
	}
}

} // peaseblossom
} // X3D
} // titania

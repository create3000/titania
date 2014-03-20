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

template <>
const std::string X3DField <Image>::typeName ("SFImage");

template <>
const X3DConstants::FieldType X3DField <Image>::type = X3DConstants::SFImage;

template class X3DField <Image>;

SFImage::SFImage () :
	X3DField <Image> ()
{
	get () .array () .addParent (this);
}

SFImage::SFImage (const SFImage & field) :
	X3DField <Image> (field)
{
	get () .array () .addParent (this);
}

SFImage::SFImage (const Image & value) :
	X3DField <Image> (value)
{
	get () .array () .addParent (this);
}

SFImage::SFImage (const size_type width, const size_type height, const size_type components, const MFInt32 & array) :
	X3DField <Image> (Image (width, height, components, array))
{
	get () .array () .addParent (this);
}

void
SFImage::setWidth (const size_type value)
{
	get () .width (value);
	addEvent ();
}

SFImage::size_type
SFImage::getWidth () const
{
	return getValue () .width ();
}

void
SFImage::setHeight (const size_type value)
{
	get () .height (value);
	addEvent ();
}

SFImage::size_type
SFImage::getHeight () const
{
	return getValue () .height ();
}

void
SFImage::setComponents (const size_type value)
{
	get () .components (value);
	addEvent ();
}

SFImage::size_type
SFImage::getComponents () const
{
	return getValue () .components ();
}

void
SFImage::setArray (const MFInt32 & value)
{
	get () .array (value);
}

MFInt32 &
SFImage::getArray ()
{
	return get () .array ();
}

const MFInt32 &
SFImage::getArray () const
{
	return getValue () .array ();
}

void
SFImage::setValue (const size_type width, const size_type height, const size_type components, const MFInt32 & array)
{
	get () .set (width, height, components, array);
}

void
SFImage::getValue (size_type & width, size_type & height, size_type & components, MFInt32 & array) const
{
	getValue () .get (width, height, components, array);
}

void
SFImage::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whitespaces;
	
	size_type width, height, components;
	MFInt32   array;

	Grammar::whitespaces (istream, whitespaces);
	
	if (istream >> width)
	{
		if (Grammar::whitespaces (istream, whitespaces))
		{
			if (istream >> height)
			{
				if (Grammar::whitespaces (istream, whitespaces))
				{
					if (istream >> components)
					{
						for (size_t i = 0, size = width * height; i < size; ++ i)
						{
							int32_t pixel;
							
							if (Grammar::whitespaces (istream, whitespaces) and Grammar::Int32 (istream, pixel))
								array .emplace_back (pixel);

							else
								return;
						}

						if (istream)
							setValue (width, height, components, array);
					}
				}
			}
		}
	}
}

void
SFImage::toStream (std::ostream & ostream) const
{
	const Image & image = getValue ();

	ostream
		<< image .width ()
		<< Generator::Space
		<< image .height ()
		<< Generator::Space
		<< image .components ();

	if (image .width () and image .height ())
	{
		ostream
			<< std::hex
			<< std::showbase
			<< Generator::ListBreak
			<< Generator::IncIndent;

		Image::size_type y = 0;

		for (; y < image .height () - 1; ++ y)
		{
			if (Generator::HasListBreak ())
				ostream << Generator::Indent;

			const Image::size_type s = y * image .width ();
			Image::size_type       x = 0;

			for (Image::size_type size = image .width () - 1; x < size; ++ x)
			{
				ostream
					<< image .array () [x + s]
					<< Generator::Space;
			}

			ostream
				<< image .array () [x + s]
				<< Generator::ListBreak;
		}

		if (Generator::HasListBreak ())
			ostream << Generator::Indent;

		const Image::size_type s = y * image .width ();
		Image::size_type       x = 0;

		for (Image::size_type size = image .width () - 1; x < size; ++ x)
		{
			ostream
				<< image .array () [x + s]
				<< Generator::Space;
		}

		ostream
			<< image .array () [x + s]
			<< Generator::DecIndent
			<< std::dec;
	}
}

void
SFImage::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

void
SFImage::dispose ()
{
	X3DField <Image>::dispose ();

	get () .array () .dispose ();
}

} // X3D
} // titania

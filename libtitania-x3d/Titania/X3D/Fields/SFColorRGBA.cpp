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

#include "SFColorRGBA.h"

#include "../Parser/Colors.h"

#include <Titania/String/tolower.h>

namespace titania {
namespace X3D {

template <>
const std::string X3DField <Color4f>::typeName ("SFColorRGBA");

template <>
const X3DConstants::FieldType X3DField <Color4f>::type = X3DConstants::SFColorRGBA;

template class X3DField <Color4f>;

SFColorRGBA::SFColorRGBA () :
	X3DField <Color4f> ()
{ }

SFColorRGBA::SFColorRGBA (const SFColorRGBA & field) :
	X3DField <Color4f> (field)
{ }

SFColorRGBA::SFColorRGBA (const internal_type & value) :
	X3DField <Color4f> (value)
{ }

SFColorRGBA::SFColorRGBA (const value_type & r, const value_type & g, const value_type & b, const value_type & a) :
	X3DField <Color4f> (Color4f (r, g, b, a))
{ }

void
SFColorRGBA::setRed (const value_type & r)
{
	get () .r (r);
	addEvent ();
}

void
SFColorRGBA::setGreen (const value_type & g)
{
	get () .g (g);
	addEvent ();
}

void
SFColorRGBA::setBlue (const value_type & b)
{
	get () .b (b);
	addEvent ();
}

void
SFColorRGBA::setAlpha (const value_type & a)
{
	get () .a (a);
	addEvent ();
}

void
SFColorRGBA::set1Value (const size_type & index, const value_type & value)
{
	get () [index] = math::clamp <value_type> (value, 0, 1);
	addEvent ();
}

SFColorRGBA::value_type
SFColorRGBA::get1Value (const size_type & index) const
{
	return getValue () [index];
}

SFColorRGBA::value_type
SFColorRGBA::operator [ ] (const size_type & index) const
{
	return getValue () [index];
}

void
SFColorRGBA::setHSVA (const vector4 <value_type> & hsva)
{
	get () .hsva (hsva);
	addEvent ();
}

vector4 <SFColorRGBA::value_type>
SFColorRGBA::getHSVA () const
{
	return getValue () .hsva ();
}

SFColorRGBA::internal_type
SFColorRGBA::lerp (const internal_type & dest, const value_type & t) const
{
	return make_hsva (hsva_lerp (getValue () .hsva (), dest .hsva (), t));
}

void
SFColorRGBA::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whiteSpaces;

	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	// Parse C hex colors.

	{
		uint32_t value;

		if (Grammar::Hex (istream, value))
		{
			const value_type r = (value >> 24 & 0xff) / value_type (0xff);
			const value_type g = (value >> 16 & 0xff) / value_type (0xff);
			const value_type b = (value >> 8  & 0xff) / value_type (0xff);
			const value_type a = (value >> 0  & 0xff) / value_type (0xff);
	
			setValue (internal_type (r, g, b, a));
			return;
		}
	}

	// Parse X3D colors.

	{
		value_type r, g, b, a;
	
		if (Grammar::Number <value_type> (istream, r))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
			if (Grammar::Number <value_type> (istream, g))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
				if (Grammar::Number <value_type> (istream, b))
				{
					Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
					if (Grammar::Number <value_type> (istream, a))
					{
						setValue (internal_type (r, g, b, a));
						return;
					}
				}
		   }
	
			return;
		}
	
		istream .clear ();
	}

	// Parse HTML hex colors and shorthand hex colors.

	if (Grammar::NumberSign (istream))
	{
		const auto pos = istream .tellg ();

		int32_t number;

		if (Grammar::HexValue (istream, number))
		{
			istream .clear ();

			if (istream .tellg () - pos == 3)
			{
				// Shorthand hex color

				static const auto sc = [ ] (const uint32_t c) { return c << 4 | c; };

				const auto b = sc ((number >>= 0) & 0xf) / value_type (0xff);
				const auto g = sc ((number >>= 4) & 0xf) / value_type (0xff);
				const auto r = sc ((number >>= 4) & 0xf) / value_type (0xff);

				setValue (internal_type (r, g, b, 1));
				return;
			}

			const auto b = ((number >>= 0) & 0xff) / value_type (0xff);
			const auto g = ((number >>= 8) & 0xff) / value_type (0xff);
			const auto r = ((number >>= 8) & 0xff) / value_type (0xff);

			setValue (internal_type (r, g, b, 1));
			return;
		}

		return;
	}

	// Parse named colors.

	{
		std::string colorName;
	
		if (Grammar::NamedColor (istream, colorName))
		{
			try
			{
				const auto & color = Colors::get (basic::tolower (colorName, std::locale::classic ()));

				setValue (internal_type (color .r (), color .g (), color .b (), 1));
				return;
			}
			catch (const std::out_of_range &)
			{
				for (size_t i = 0, size = colorName .size (); i < size; ++ i)
					istream .unget ();
			}
		}
	}

	istream .setstate (std::ios::failbit);
}

void
SFColorRGBA::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::SetPrecision <value_type> << getValue ();
}

void
SFColorRGBA::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

void
SFColorRGBA::toJSONStream (std::ostream & ostream) const
{
	ostream
		<< '['
		<< X3DGenerator::TidySpace;

	toJSONStreamValue (ostream);

	ostream
		<< X3DGenerator::TidySpace
		<< ']';
}

void
SFColorRGBA::toJSONStreamValue (std::ostream & ostream) const
{
	ostream
		<< X3DGenerator::SetPrecision <value_type>
		<< getValue () .r ()
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () .g ()
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () .b ()
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () .a ();
}

} // X3D
} // titania

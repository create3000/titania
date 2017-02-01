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

#include "SFColor.h"

#include "../Parser/Colors.h"

#include <Titania/String/tolower.h>

namespace titania {
namespace X3D {

template <>
const std::string X3DField <Color3f>::typeName ("SFColor");

template <>
const X3DConstants::FieldType X3DField <Color3f>::type = X3DConstants::SFColor;

template class X3DField <Color3f>;

SFColor::SFColor () :
	X3DField <Color3f> (Color3f ())
{ }

SFColor::SFColor (const SFColor & field) :
	X3DField <Color3f> (field)
{ }

SFColor::SFColor (const Color3f & value) :
	X3DField <Color3f> (value)
{ }

SFColor::SFColor (const value_type & r, const value_type & g, const value_type & b) :
	X3DField <Color3f> (Color3f (r, g, b))
{ }

void
SFColor::setRed (const value_type & r)
{
	get () .r (r);
	addEvent ();
}

void
SFColor::setGreen (const value_type & g)
{
	get () .g (g);
	addEvent ();
}

void
SFColor::setBlue (const value_type & b)
{
	get () .b (b);
	addEvent ();
}

void
SFColor::set1Value (const size_type & index, const value_type & value)
{
	get () [index] = math::clamp <value_type> (value, 0, 1);
	addEvent ();
}

typename SFColor::value_type
SFColor::get1Value (const size_type & index) const
{
	return getValue () [index];
}

void
SFColor::setHSV (const value_type & h, const value_type & s, const value_type & v)
{
	get () .set_hsv (h, s, v);
	addEvent ();
}

void
SFColor::getHSV (value_type & h, value_type & s, value_type & v) const
{
	getValue () .get_hsv (h, s, v);
}

SFColor*
SFColor::lerp (const SFColor & dest, const value_type & t) const
{
	return new SFColor (math::clerp (getValue (), dest .getValue (), t));
}

void
SFColor::fromStream (std::istream & istream)
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
			const value_type r = (value >> 16 & 0xff) / value_type (0xff);
			const value_type g = (value >> 8  & 0xff) / value_type (0xff);
			const value_type b = (value >> 0  & 0xff) / value_type (0xff);

			setValue (internal_type (r, g, b));
			return;
		}
	}

	// Parse X3D colors.

	{
		value_type r, g, b;
	
		if (Grammar::Number <value_type> (istream, r))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
			if (Grammar::Number <value_type> (istream, g))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
				if (Grammar::Number <value_type> (istream, b))
				{
					setValue (internal_type (r, g, b));
					return;
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

				setValue (internal_type (r, g, b));
				return;
			}

			const auto b = ((number >>= 0) & 0xff) / value_type (0xff);
			const auto g = ((number >>= 8) & 0xff) / value_type (0xff);
			const auto r = ((number >>= 8) & 0xff) / value_type (0xff);

			setValue (internal_type (r, g, b));
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
				setValue (Colors::get (basic::tolower (colorName, std::locale::classic ())));
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
SFColor::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::SetPrecision <value_type> << getValue ();
}

void
SFColor::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

void
SFColor::toJSONStream (std::ostream & ostream) const
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
SFColor::toJSONStreamValue (std::ostream & ostream) const
{
	ostream
		<< X3DGenerator::SetPrecision <value_type>
		<< getValue () .r ()
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () .g ()
		<< ','
		<< X3DGenerator::TidySpace
		<< getValue () .b ();
}

} // X3D
} // titania

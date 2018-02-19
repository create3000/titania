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

#include "MiniParser.h"

#include "../Parser/Colors.h"

#include <Titania/String/tolower.h>

namespace titania {
namespace X3D {

bool
MiniParser::Decode (std::istream & istream, bool & value)
{
	std::string whiteSpaces;
	
	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	if (Grammar::TRUE_ (istream))
	{
		value = true;
		return true;
	}

	if (Grammar::FALSE_ (istream))
	{
		value = false;
		return true;
	}

	istream .setstate (std::ios_base::failbit);
	return false;
}

bool
MiniParser::Decode (std::istream & istream, Color3f & value)
{
	std::string whiteSpaces;

	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	// Parse C hex colors.

	{
		uint32_t hex;
	
		if (Grammar::Hex (istream, hex))
		{
			const float r = (hex >> 16 & 0xff) / float (0xff);
			const float g = (hex >> 8  & 0xff) / float (0xff);
			const float b = (hex >> 0  & 0xff) / float (0xff);

			value = Color3f (r, g, b);
			return true;
		}
	}

	// Parse X3D colors.

	{
		float r, g, b;
	
		if (Grammar::Number <float> (istream, r))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
			if (Grammar::Number <float> (istream, g))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
				if (Grammar::Number <float> (istream, b))
				{
					value = Color3f (r, g, b);
					return true;
				}
		   }
	
			return false;
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

				const auto b = sc ((number >>= 0) & 0xf) / float (0xff);
				const auto g = sc ((number >>= 4) & 0xf) / float (0xff);
				const auto r = sc ((number >>= 4) & 0xf) / float (0xff);

				value = Color3f (r, g, b);
				return true;
			}

			const auto b = ((number >>= 0) & 0xff) / float (0xff);
			const auto g = ((number >>= 8) & 0xff) / float (0xff);
			const auto r = ((number >>= 8) & 0xff) / float (0xff);

			value = Color3f (r, g, b);
			return true;
		}

		return false;
	}

	// Parse named colors.

	{
		std::string colorName;
	
		if (Grammar::NamedColor (istream, colorName))
		{
			try
			{
				value = Colors::get (basic::tolower (colorName, std::locale::classic ()));
				return true;
			}
			catch (const std::out_of_range &)
			{
				for (size_t i = 0, size = colorName .size (); i < size; ++ i)
					istream .unget ();
			}
		}
	}

	istream .setstate (std::ios::failbit);
	return false;
}

bool
MiniParser::Decode (std::istream & istream, Color4f & value)
{
	std::string whiteSpaces;

	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	// Parse C hex colors.

	{
		uint32_t hex;

		if (Grammar::Hex (istream, hex))
		{
			const float r = (hex >> 24 & 0xff) / float (0xff);
			const float g = (hex >> 16 & 0xff) / float (0xff);
			const float b = (hex >> 8  & 0xff) / float (0xff);
			const float a = (hex >> 0  & 0xff) / float (0xff);
	
			value = Color4f (r, g, b, a);
			return true;
		}
	}

	// Parse X3D colors.

	{
		float r, g, b, a;
	
		if (Grammar::Number <float> (istream, r))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
			if (Grammar::Number <float> (istream, g))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
				if (Grammar::Number <float> (istream, b))
				{
					Grammar::WhiteSpacesNoComma (istream, whiteSpaces);
	
					if (Grammar::Number <float> (istream, a))
					{
						value = Color4f (r, g, b, a);
						return true;
					}
				}
		   }
	
			return false;
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

				const auto b = sc ((number >>= 0) & 0xf) / float (0xff);
				const auto g = sc ((number >>= 4) & 0xf) / float (0xff);
				const auto r = sc ((number >>= 4) & 0xf) / float (0xff);

				value = Color4f (r, g, b, 1);
				return true;
			}

			const auto b = ((number >>= 0) & 0xff) / float (0xff);
			const auto g = ((number >>= 8) & 0xff) / float (0xff);
			const auto r = ((number >>= 8) & 0xff) / float (0xff);

			value = Color4f (r, g, b, 1);
			return true;
		}

		return false;
	}

	// Parse named colors.

	{
		std::string colorName;
	
		if (Grammar::NamedColor (istream, colorName))
		{
			try
			{
				const auto & color = Colors::get (basic::tolower (colorName, std::locale::classic ()));

				value = Color4f (color .r (), color .g (), color .b (), 1);
				return true;
			}
			catch (const std::out_of_range &)
			{
				for (size_t i = 0, size = colorName .size (); i < size; ++ i)
					istream .unget ();
			}
		}
	}

	istream .setstate (std::ios::failbit);
	return false;
}

bool
MiniParser::Decode (std::istream & istream, double & value)
{
	std::string whiteSpaces;
	
	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	return Grammar::Double (istream, value);
}

bool
MiniParser::Decode (std::istream & istream, float & value)
{
	std::string whiteSpaces;
	
	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	return Grammar::Float (istream, value);
}

bool
MiniParser::Decode (std::istream & istream, int32_t & value)
{
	std::string whiteSpaces;

	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	return Grammar::Int32 (istream, value);
}

bool
MiniParser::Decode (std::istream & istream, X3D::String & value)
{
	std::string whiteSpaces;

	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	std::string string;

	if (Grammar::String (istream, string))
	{
		value = string;
		return true;
	}

	istream .setstate (std::ios_base::failbit);
	return false;
}

} // X3D
} // titania

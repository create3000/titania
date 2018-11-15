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

#include "Font.h"

#include <Titania/LOG.h>

namespace titania {
namespace X3D {

Font::Font () :
	pattern (FontPatternPtr (FcPatternCreate (), PatternDeleter { }))
{ }

Font::Font (const Font & other) :
	pattern (other .pattern)
{ }

Font::Font (const FontPatternPtr & pattern) :
	pattern (pattern)
{ }

const FontPatternPtr &
Font::getPattern () const
{
	return pattern;
}

void
Font::setFamilyName (const std::string & value)
{
	FcPatternAddString (pattern .get (), FC_FAMILY, (FcChar8*) value .c_str ());
}

std::string
Font::getFamilyName () const
{
	FcChar8* familyName = nullptr;

	if (FcPatternGetString (pattern .get (), FC_FAMILY, 0, &familyName) == FcResultMatch)
		return (char*) familyName;

	return "";
}

void
Font::setSlant (const Slant & value)
{
	switch (value)
	{
		case Slant::ROMAN:
			FcPatternAddInteger (pattern .get (), FC_SLANT, FC_SLANT_ROMAN);
			break;
		case Slant::ITALIC:
			FcPatternAddInteger (pattern .get (), FC_SLANT, FC_SLANT_ITALIC);
			break;
	}
}

void
Font::setWeight (const Weight & value)
{
	switch (value)
	{
		case Weight::NORMAL:
			FcPatternAddInteger (pattern .get (), FC_WEIGHT, FC_WEIGHT_NORMAL);
			break;
		case Weight::BOLD:
			FcPatternAddInteger (pattern .get (), FC_WEIGHT, FC_WEIGHT_BOLD);
			break;
	}
}


void
Font::setStyle (const std::string & value)
{
	FcPatternAddString (pattern .get (), "style", (FcChar8*) value .c_str ());
}

void
Font::setScalable (const bool value)
{
	FcPatternAddBool (pattern .get (), FC_SCALABLE, value ? FcTrue : FcFalse);
}

void
Font::setFilename (const std::string & value)
{
	FcPatternAddString (pattern .get (), FC_FILE, (FcChar8*) value .c_str ());
}

std::string
Font::getFilename () const
{
	FcChar8* filename = nullptr;

	if (FcPatternGetString (pattern .get (), FC_FILE, 0, &filename) == FcResultMatch)
		return (char*) filename;

	return "";
}

void
Font::substitute ()
{
	FcConfigSubstitute (nullptr, pattern .get (), FcMatchPattern);
	FcDefaultSubstitute (pattern .get ());
}

Font
Font::getMatch () const
{
	FcResult result;

	return Font (FontPatternPtr (FcFontMatch (nullptr, pattern .get (), &result), PatternDeleter ()));
}

///  throws std::runtime_error
FontFace
Font::getFace () const
{
	FcInit ();

	FT_Library freetype;

	if (FT_Init_FreeType (&freetype))
		throw std::runtime_error ("FT_Init_FreeType failed");

	FT_Face face = nullptr;

	if (FT_New_Face (freetype, getFilename () .c_str (), 0, &face))
		throw std::runtime_error ("FT_New_Face failed (there is probably a problem with your font file)");

	return FontFace (FTLibraryPtr (freetype, FTLibraryDeleter ()), FTFacePtr (face, FTFaceDeleter ()));
}

void
Font::dispose ()
{
	pattern .reset ();
}

Font::~Font ()
{ }

} // X3D
} // titania/

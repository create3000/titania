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

#ifndef __TITANIA_X3D_BROWSER_TEXT_FONT_FACE_H__
#define __TITANIA_X3D_BROWSER_TEXT_FONT_FACE_H__

#include "../../Types/String.h"

#include <memory>
#include <string>
#include <type_traits>

extern "C"
{
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>
}

// See include/freetype2/freetype.h

namespace titania {
namespace X3D {

class Font;

using FreeTypePtr = std::shared_ptr <std::remove_pointer <FT_Library>::type>;
using FontFacePtr = std::shared_ptr <std::remove_pointer <FT_Face>::type>;

struct FreeTypeDeleter
{
	void
	operator () (FT_Library library) const
	{
		FT_Done_FreeType (library);
	}

};

class FontFace
{
public:

	struct FaceDeleter
	{
		void
		operator () (FT_Face face) const
		{
			FT_Done_Face (face);
		}

	};

	///  @name Construction

	FontFace ();

	FontFace (const FontFace &);

	FontFace &
	operator = (const FontFace &);

	///  @name Member access

	const FreeTypePtr &
	getLibrary () const
	{ return freetype; }

	const FontFacePtr &
	getFace () const;

	unsigned short
	getUnitsPerEm () const
	{ return face -> units_per_EM; }

	short
	getAscender () const
	{ return face -> ascender; }

	short
	getDescender () const
	{ return face -> descender; }

	short
	getHeight () const
	{ return face -> height; }

	///  @name Destruction

	void
	dispose ();

	virtual
	~FontFace ();


private:

	///  @name Friends

	friend Font;

	///  @name Construction

	FontFace (const FreeTypePtr &, const FontFacePtr &);

	///  @name Members

	FreeTypePtr freetype;
	FontFacePtr face;

};

} // X3D
} // titania

#endif

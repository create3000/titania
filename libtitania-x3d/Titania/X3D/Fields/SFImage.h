/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#ifndef __TITANIA_X3D_FIELDS_SFIMAGE_H__
#define __TITANIA_X3D_FIELDS_SFIMAGE_H__

#include "../Basic/X3DField.h"
#include "../Types/Image.h"

namespace titania {
namespace X3D {

extern template class X3DField <Image>;

class SFImage :
	public X3DField <Image>
{
public:

	typedef Image::size_type size_type;

	using X3DField <Image>::setValue;
	using X3DField <Image>::getValue;
	using X3DField <Image>::operator =;

	SFImage ();

	SFImage (const SFImage &);

	explicit
	SFImage (const Image &);

	SFImage (const size_type, const size_type, const size_type, const MFInt32 &);

	virtual
	SFImage*
	copy () const;

	void
	setWidth (const size_type);

	size_type
	getWidth () const;

	void
	setHeight (const size_type);

	size_type
	getHeight () const;

	void
	setComp (const size_type);

	size_type
	getComp () const;

	void
	setArray (const MFInt32 &);

//	MFInt32 &
//	getArray ();

	const MFInt32 &
	getArray () const;

	void
	setValue (const size_type, const size_type, const size_type, const MFInt32 &);

	void
	getValue (size_type &, size_type &, size_type &, MFInt32 &) const;


private:

	using X3DField <Image>::get;

};

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const Image & image)
{
	ostream << image .width () << Generator::Space << image .height () << Generator::Space << image .comp ();

	if (image .width () and image .height ())
	{
		ostream
			<< std::hex
			<< std::showbase
			<< Generator::ListBreak
			<< Generator::IncIndent;

		Image::size_type y;

		for (y = 0; y < image .height () - 1; ++ y)
		{
			if (Generator::HasListBreak ())
				ostream << Generator::Indent;

			Image::size_type x;

			for (x = 0; x < image .width () - 1; ++ x)
			{
				ostream
					<< image .array () .at (x + y * image .width ())
					<< Generator::Space;
			}

			ostream
				<< image .array () .at (x + y * image .width ())
				<< Generator::ListBreak;
		}

		if (Generator::HasListBreak ())
			ostream << Generator::Indent;

		Image::size_type x;

		for (x = 0; x < image .width () - 1; ++ x)
		{
			ostream
				<< image .array () .at (x + y * image .width ())
				<< Generator::Space;
		}

		ostream
			<< image .array () .at (x + y * image .width ())
			<< Generator::DecIndent
			<< std::dec;
	}

	return ostream;
}

extern template
std::ostream &
operator << (std::ostream &, const Image &);

} // X3D
} // titania

#endif

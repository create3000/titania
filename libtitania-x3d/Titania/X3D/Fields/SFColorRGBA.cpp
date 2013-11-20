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

#include "SFColorRGBA.h"

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

SFColorRGBA::SFColorRGBA (const Color4f & value) :
	X3DField <Color4f> (value)
{ }

SFColorRGBA::SFColorRGBA (const value_type & r, const value_type & g, const value_type & b, const value_type & a) :
	X3DField <Color4f> (Color4f (r, g, b, a))
{ }

void
SFColorRGBA::setR (const value_type & r)
{
	get () .r (r);
	addEvent ();
}

typename SFColorRGBA::value_type
SFColorRGBA::getR () const
{
	return getValue () .r ();
}

void
SFColorRGBA::setG (const value_type & g)
{
	get () .g (g);
	addEvent ();
}

typename SFColorRGBA::value_type
SFColorRGBA::getG () const
{
	return getValue () .g ();
}

void
SFColorRGBA::setB (const value_type & b)
{
	get () .b (b);
	addEvent ();
}

typename SFColorRGBA::value_type
SFColorRGBA::getB () const
{
	return getValue () .b ();
}

void
SFColorRGBA::setA (const value_type & a)
{
	get () .a (a);
	addEvent ();
}

typename SFColorRGBA::value_type
SFColorRGBA::getA () const
{
	return getValue () .a ();
}

void
SFColorRGBA::set1Value (const size_type & index, const value_type & value)
{
	get () [index] = math::clamp <value_type> (value, 0, 1);
	addEvent ();
}

typename SFColorRGBA::value_type
SFColorRGBA::get1Value (const size_type & index) const
{
	return getValue () [index];
}

void
SFColorRGBA::setValue (const value_type & r, const value_type & g, const value_type & b, const value_type & a)
{
	setValue (Color4f (r, g, b, a));
}

void
SFColorRGBA::getValue (value_type & r, value_type & g, value_type & b, value_type & a) const
{
	getValue () .get (r, g, b, a);
}

void
SFColorRGBA::setHSV (const value_type  & h, const value_type  & s, const value_type  & v)
{
	get () .setHSV (h, s, v);
	addEvent ();
}

void
SFColorRGBA::getHSV (value_type & h, value_type & s, value_type & v) const
{
	getValue () .getHSV (h, s, v);
}

void
SFColorRGBA::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whitespaces;

	Grammar::whitespaces (istream, whitespaces);

	uint32_t value;

	if (Grammar::Hex (istream, value))
	{
		float r = (value >> 24 & 0xff) / 255.0f;
		float g = (value >> 16 & 0xff) / 255.0f;
		float b = (value >> 8 & 0xff) / 255.0f;
		float a = (value & 0xff) / 255.0f;

		get () .set (r, g, b, a);

		addEvent ();
	}
	else
	{
		istream >> get ();

		if (istream)
			addEvent ();
	}
}

void
SFColorRGBA::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <value_type> << getValue ();
}

} // X3D
} // titania

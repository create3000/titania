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

#include "SFColor.h"

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

SFColor::SFColor (const scalar_type & r, const scalar_type & g, const scalar_type & b) :
	X3DField <Color3f> (Color3f (r, g, b))
{ }

SFColor*
SFColor::clone () const
{
	return new SFColor (*this);
}

void
SFColor::setR (const scalar_type & r)
{
	get () .r (r);
	addEvent ();
}

typename SFColor::scalar_type
SFColor::getR () const
{
	return getValue () .r ();
}

void
SFColor::setG (const scalar_type & g)
{
	get () .g (g);
	addEvent ();
}

typename SFColor::scalar_type
SFColor::getG () const
{
	return getValue () .g ();
}

void
SFColor::setB (const scalar_type & b)
{
	get () .b (b);
	addEvent ();
}

typename SFColor::scalar_type
SFColor::getB () const
{
	return getValue () .b ();
}

void
SFColor::set1Value (const size_type & index, const scalar_type & value)
{
	get () [index] = math::clamp <scalar_type> (value, 0, 1);
	addEvent ();
}

typename SFColor::scalar_type
SFColor::get1Value (const size_type & index) const
{
	return getValue () [index];
}

void
SFColor::setValue (const scalar_type & r, const scalar_type & g, const scalar_type & b)
{
	setValue (Color3f (r, g, b));
}

void
SFColor::getValue (scalar_type & r, scalar_type & g, scalar_type & b) const
{
	getValue () .get (r, g, b);
}

void
SFColor::setHSV (const scalar_type & h, const scalar_type & s, const scalar_type & v)
{
	get () .setHSV (h, s, v);
	addEvent ();
}

void
SFColor::getHSV (scalar_type & h, scalar_type & s, scalar_type & v) const
{
	getValue () .getHSV (h, s, v);
}

void
SFColor::fromStream (std::istream & istream)
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
		float r = (value >> 16 & 0xff) / 255.0f;
		float g = (value >> 8  & 0xff) / 255.0f;
		float b = (value       & 0xff) / 255.0f;

		get () .set (r, g, b);

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
SFColor::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <scalar_type> << getValue ();
}

} // X3D
} // titania

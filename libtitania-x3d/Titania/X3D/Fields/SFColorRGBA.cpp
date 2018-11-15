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

#include "../InputOutput/VRMLGenerator.h"
#include "../InputOutput/XMLGenerator.h"
#include "../InputOutput/JSONGenerator.h"
#include "../Parser/MiniParser.h"

namespace titania {
namespace X3D {

template <>
const std::string X3DField <Color4f>::typeName = "SFColorRGBA";

template <>
const FieldType X3DField <Color4f>::type = X3DConstants::SFColorRGBA;

template class X3DField <Color4f>;

SFColorRGBA::SFColorRGBA () :
	X3DField <Color4f> ()
{ }

SFColorRGBA::SFColorRGBA (const SFColorRGBA & other) :
	X3DField <Color4f> (other)
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

SFColorRGBA::value_type
SFColorRGBA::at (const size_type & index) const
{
	return getValue () .at (index);
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
{
	Color4f value;

	if (MiniParser::Decode (istream, value))
		setValue (value);
}

void
SFColorRGBA::toStream (std::ostream & ostream) const
{
	VRMLGenerator::Encode (ostream, getValue (), getUnit ());
}

void
SFColorRGBA::toXMLStream (std::ostream & ostream) const
{
	XMLGenerator::Encode (ostream, getValue (), getUnit ());
}

void
SFColorRGBA::toJSONStream (std::ostream & ostream) const
{
	ostream
		<< '['
		<< X3DGenerator::TidySpace;

	JSONGenerator::Encode (ostream, getValue (), getUnit ());

	ostream
		<< X3DGenerator::TidySpace
		<< ']';
}

} // X3D
} // titania

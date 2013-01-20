/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

SFColorRGBA::SFColorRGBA (const scalar_type & r, const scalar_type & g, const scalar_type & b, const scalar_type & a) :
	X3DField <Color4f> (Color4f (r, g, b, a))
{ }

SFColorRGBA*
SFColorRGBA::clone () const
{
	return new SFColorRGBA (*this);
}

void
SFColorRGBA::setR (const scalar_type & r)
{
	get () .r (r);
	notifyParents ();
}

typename SFColorRGBA::scalar_type
SFColorRGBA::getR () const
{
	return getValue () .r ();
}

void
SFColorRGBA::setG (const scalar_type & g)
{
	get () .g (g);
	notifyParents ();
}

typename SFColorRGBA::scalar_type
SFColorRGBA::getG () const
{
	return getValue () .g ();
}

void
SFColorRGBA::setB (const scalar_type & b)
{
	get () .b (b);
	notifyParents ();
}

typename SFColorRGBA::scalar_type
SFColorRGBA::getB () const
{
	return getValue () .b ();
}

void
SFColorRGBA::setA (const scalar_type & a)
{
	get () .a (a);
	notifyParents ();
}

typename SFColorRGBA::scalar_type
SFColorRGBA::getA () const
{
	return getValue () .a ();
}

void
SFColorRGBA::set1Value (const size_type & index, const scalar_type & value)
{
	get () [index] = value;
	notifyParents ();
}

typename SFColorRGBA::scalar_type
SFColorRGBA::get1Value (const size_type & index) const
{
	return getValue () [index];
}

void
SFColorRGBA::setValue (const scalar_type & r, const scalar_type & g, const scalar_type & b, const scalar_type & a)
{
	setValue (Color4f (r, g, b, a));
}

void
SFColorRGBA::getValue (scalar_type & r, scalar_type & g, scalar_type & b, scalar_type & a) const
{
	getValue () .get (r, g, b, a);
}

void
SFColorRGBA::setHSV (const scalar_type  & h, const scalar_type  & s, const scalar_type  & v)
{
	get () .setHSV (h, s, v);
	notifyParents ();
}

void
SFColorRGBA::getHSV (scalar_type & h, scalar_type & s, scalar_type & v) const
{
	getValue () .getHSV (h, s, v);
}

void
SFColorRGBA::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <scalar_type><< getValue ();
}

} // X3D
} // titania

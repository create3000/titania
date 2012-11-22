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
 ******************************************************************************/

#include "SFImage.h"

namespace titania {
namespace X3D {

template <>
const FieldType X3DField <Image>::type ("SFImage");

template class X3DField <Image>;
template
std::ostream &
operator << (std::ostream & ostream, const Image & image);

SFImage::SFImage () :
	X3DField <Image> () 
{
	setChild (get () .array ());
}

SFImage::SFImage (const SFImage & field) :
	X3DField <Image> (field)
{
	setChild (get () .array ());
}

SFImage::SFImage (const Image & value) :
	X3DField <Image> (value)
{
	setChild (get () .array ());
}

SFImage::SFImage (const size_type width, const size_type height, const size_type components, const MFInt32 & array) :
	X3DField <Image> (Image (width, height, components, array))
{
	setChild (get () .array ());
}

SFImage*
SFImage::copy () const
{
	return new SFImage (*this);
}

void
SFImage::setWidth (const size_type value)
{
	get () .width (value);
	notifyParents ();
}

SFImage::size_type
SFImage::getWidth () const
{
	return getValue () .width ();
}

void
SFImage::setHeight (const size_type value)
{
	get () .height (value);
	notifyParents ();
}

SFImage::size_type
SFImage::getHeight () const
{
	return getValue () .height ();
}

void
SFImage::setComponents (const size_type value)
{
	get () .components (value);
	notifyParents ();
}

SFImage::size_type
SFImage::getComponents () const
{
	return getValue () .components ();
}

void
SFImage::setArray (const MFInt32 & value)
{
	get () .array (value);
}

//MFInt32 &
//SFImage::getArray ()
//{
//	return get () .array ();
//}

const MFInt32 &
SFImage::getArray () const
{
	return getValue () .array ();
}

void
SFImage::setValue (const size_type width, const size_type height, const size_type components, const MFInt32 & array)
{
	get () .set (width, height, components, array);
}

void
SFImage::getValue (size_type & width, size_type & height, size_type & components, MFInt32 & array) const
{
	getValue () .get (width, height, components, array);
}

void
SFImage::dispose ()
{
	X3DField <Image>::dispose ();

	get () .array () .dispose ();
}

} // X3D
} // titania

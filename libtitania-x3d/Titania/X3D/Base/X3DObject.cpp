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

#include "X3DObject.h"

namespace titania {
namespace X3D {

GarbageCollector X3DObject::garbageCollector;

X3DObject::X3DObject () :
	 X3DInput (), 
	X3DOutput (), 
	     name (),
	 userData (NULL)  
{ }

//const basic::id &
//X3DObject::getTypeName () const
//{
//
//}

// Type

void
X3DObject::setName (const basic::id & value)
{
	name = value;
}

const basic::id &
X3DObject::getName () const
{
	return name;
}

// InputOutput

bool
X3DObject::isInput () const
{
	return true;
}

bool
X3DObject::isOutput () const
{
	return true;
}

// Garbage collection

GarbageCollector &
X3DObject::getGarbageCollector ()
{
	return garbageCollector;
}

// String

std::string
X3DObject::toString () const
{
	std::ostringstream ostringstream;

	toStream (ostringstream);

	return ostringstream .str ();
}

// Object

void
X3DObject::dispose ()
{
	X3DInput::dispose  ();
	X3DOutput::dispose ();
}

X3DObject::~X3DObject ()
{ }

template std::istream & operator >> (std::istream &, X3DObject &);
template std::ostream & operator << (std::ostream &, const X3DObject &);
//template std::wistream & operator >> (std::wistream &, const X3DObject &);
//template std::wostream & operator << (std::wostream &, const X3DObject &);

} // X3D
} // titania

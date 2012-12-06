// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#include "X3DObject.h"

#include <algorithm>
#include <cassert>
#include <sstream>

#include <iostream> //

namespace titania {
namespace X3D {

GarbageCollector X3DObject::garbageCollector;

X3DObject::X3DObject () :
	  X3DBase (),
	 X3DInput (), 
	X3DOutput ()
{ }

//const basic::id &
//X3DObject::getTypeName () const
//{
//
//}

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

// Object

void
X3DObject::dispose ()
{
	X3DInput::dispose  ();
	X3DOutput::dispose ();
	X3DBase::dispose   ();
}

X3DObject::~X3DObject ()
{ }

} // X3D
} // titania
